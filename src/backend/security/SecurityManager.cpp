#include "src/backend/security/SecurityManager.h"
#include "qdebug.h"
#include <botan/auto_rng.h>
#include <botan/hash.h>
#include <botan/hex.h>
#include <botan/aead.h>
#include <botan/pbkdf.h>
#include <botan/base64.h>
#include <src/backend/core/SendEmail.h>
#include <QRandomGenerator>
SecurityManager::SecurityManager()
    :passphrase("a1b2c3d4e5f6a7b8c9d0e1f2a3b4c5d6")
{
    ;
}

SecurityManager::~SecurityManager()
{
    ;
}


QString SecurityManager::encrypt(const QString &plainText, const QString& baseSecret) const
{
    try {

        auto pbkdf = Botan::PBKDF::create("PBKDF2(SHA-256)");
        const std::vector<uint8_t> salt = Botan::hex_decode(this->passphrase);
        const auto key = pbkdf->derive_key(32, baseSecret.toStdString(), salt.data(), salt.size(), 100000);


        Botan::AutoSeeded_RNG rng;
        const auto nonce = rng.random_vec(12);

        auto enc = Botan::AEAD_Mode::create("AES-256/GCM", Botan::Cipher_Dir::Encryption);
        enc->set_key(key);
        enc->start(nonce);
        std::string plainStdString = plainText.toStdString();
        Botan::secure_vector<uint8_t> buffer(plainStdString.begin(), plainStdString.end());
        enc->finish(buffer);

        std::vector<uint8_t> combined_data;
        combined_data.insert(combined_data.end(), nonce.begin(), nonce.end());
        combined_data.insert(combined_data.end(), buffer.begin(), buffer.end());


        return QString::fromStdString(Botan::base64_encode(combined_data));

    } catch (const Botan::Exception& e) {
        qDebug() << "Botan encryption failed:" << e.what();
        return "";
    }
}

QString SecurityManager::decrypt(const QString &encryptedText, const QString &baseSecret) const
{
    try {
        auto pbkdf = Botan::PBKDF::create("PBKDF2(SHA-256)");
        const Botan::secure_vector<uint8_t> salt = Botan::hex_decode_locked(this->passphrase);
        const auto key = pbkdf->derive_key(32, baseSecret.toStdString(), salt.data(), salt.size(), 100000);

        const auto combined_data = Botan::base64_decode(encryptedText.toStdString());
        if (combined_data.size() < 12) return "";

        const Botan::secure_vector<uint8_t> nonce(combined_data.begin(), combined_data.begin() + 12);
        Botan::secure_vector<uint8_t> ciphertext(combined_data.begin() + 12, combined_data.end());

        auto dec = Botan::AEAD_Mode::create("AES-256/GCM", Botan::Cipher_Dir::Decryption);
        dec->set_key(key);
        dec->start(nonce);
        dec->finish(ciphertext);

        return QString::fromStdString(std::string(ciphertext.begin(), ciphertext.end()));

    } catch (const Botan::Exception& e) {
        qDebug() << "Botan decryption failed:" << e.what();
        return "";
    }
}


QString SecurityManager::Hash(const QString &plainText) const
{
    std::unique_ptr<Botan::HashFunction> hash = Botan::HashFunction::create("SHA-256");
    hash->update(plainText.toStdString());
    return QString::fromStdString(Botan::hex_encode(hash->final()));
}

QString SecurityManager::securityKey_gen() const
{
    const QString characterSet = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    const int charSetSize = characterSet.length();

    Botan::AutoSeeded_RNG rng;

    const auto random_bytes = rng.random_vec(64);
    size_t byte_index = 0;

    QStringList keyParts;
    for (int i = 0; i < 4; ++i) {

        if (byte_index + 7 > random_bytes.size()) {
            qWarning() << "Ran out of random bytes for key generation.";
            return "";
        }

        QString currentPart;

        const size_t partLength = 4 + (random_bytes[byte_index++] % 3);

        for (size_t j = 0; j < partLength; ++j) {
            const uint8_t randomIndex = random_bytes[byte_index++] % charSetSize;
            currentPart.append(characterSet.at(randomIndex));
        }
        keyParts.append(currentPart);
    }

    return keyParts.join('-');
}

const QString SecurityManager::SendVerification_Code(QString &Email)
{

    QString code = QString::number(QRandomGenerator::global()->bounded(12345, 100000));
    sendMail("mohammad1384esf@gmail.com",Email.toStdString().c_str(),code.toStdString().c_str());
    return code;
}

