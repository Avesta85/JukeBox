#include "src/backend/security/SecurityManager.h"
#include "qdebug.h"
#include <botan/auto_rng.h>
#include <botan/hash.h>
#include <botan/hex.h>
#include <botan/aead.h>
#include <botan/pbkdf.h>

SecurityManager::SecurityManager()
    :passphrase("a1b2c3d4e5f6a7b8c9d0e1f2a3b4c5d6")
{
    ;
}


QString SecurityManager::encrypt(const QString &plainText, const QString& baseSecret) const
{

    const std::vector<uint8_t> salt = Botan::hex_decode(this->passphrase);

    std::unique_ptr<Botan::PBKDF> pbkdf = Botan::PBKDF::create("PBKDF2(SHA-256)");
    const auto key = pbkdf->derive_key(32, baseSecret.toStdString(), salt.data(), salt.size(), 100000).bits_of();

    try {
        Botan::AutoSeeded_RNG rng;
        const auto nonce = rng.random_vec(12);

        std::unique_ptr<Botan::AEAD_Mode> enc = Botan::AEAD_Mode::create("AES-256/GCM", Botan::Cipher_Dir::Encryption);
        enc->set_key(key);
        enc->start(nonce);

        Botan::secure_vector<uint8_t> data(plainText.toUtf8().begin(), plainText.toUtf8().end());
        enc->finish(data);

        QByteArray result = QByteArray::fromStdString(std::string(nonce.begin(), nonce.end()));
        result.append(QByteArray::fromStdString(std::string(data.begin(), data.end())));

        return QString(result.toBase64());

    } catch (const Botan::Exception& e) {
        qDebug() << "Botan encryption failed:" << e.what();
        return "";
    }
}

QString SecurityManager::decrypt(const QString &encryptedText, const QString &baseSecret) const
{
    const std::vector<uint8_t> salt = Botan::hex_decode("a1b2c3d4e5f6a7b8c9d0e1f2a3b4c5d6");
    std::unique_ptr<Botan::PBKDF> pbkdf = Botan::PBKDF::create("PBKDF2(SHA-256)");
    const auto key = pbkdf->derive_key(32, baseSecret.toStdString(), salt.data(), salt.size(), 100000).bits_of();

    try {
        QByteArray data = QByteArray::fromBase64(encryptedText.toUtf8());
        if (data.size() < 12) return "";

        const std::vector<uint8_t> nonce(data.begin(), data.begin() + 12);
        Botan::secure_vector<uint8_t> ciphertext(data.begin() + 12, data.end());

        std::unique_ptr<Botan::AEAD_Mode> dec = Botan::AEAD_Mode::create("AES-256/GCM", Botan::Cipher_Dir::Decryption);
        dec->set_key(key);
        dec->start(nonce);
        dec->finish(ciphertext);

        return QString::fromUtf8(reinterpret_cast<const char*>(ciphertext.data()), ciphertext.size());

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


