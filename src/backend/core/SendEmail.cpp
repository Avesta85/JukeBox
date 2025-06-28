#include <QCoreApplication>
#include <QDebug>
#include <curl/curl.h>
#include <cstring>
#include <QFile>

size_t payloadSource(void* ptr, size_t size, size_t nmemb, void* userp)
{
    const char** payload = (const char**)userp;
    if (*payload && **payload) {
        size_t buffer_size = size * nmemb;
        size_t len = strlen(*payload);
        if (len > buffer_size) len = buffer_size;
        memcpy(ptr, *payload, len);
        *payload += len;
        return len;
    }
    return 0;
}

bool sendMail(char* from, char* to, char* verifiMessage)
{
    CURL* curl = curl_easy_init();
    if (!curl) {
        qDebug() << "curl_easy_init failed!";
        return false;
    }

    // 🔐 WARNING: You should NOT hardcode credentials like this in production
    const char* password = "zizpjuzuanejognr";

    // Build the email message with headers
    std::string payload =
        "To: " + std::string(to) + "\r\n"
                                   "From: " + std::string(from) + "\r\n"
                              "Subject: Verify Code\r\n"
                              "Content-Type: text/plain; charset=\"UTF-8\"\r\n"
                              "\r\n" +
        std::string(verifiMessage) + "\r\n";

    char* payload_data = strdup(payload.c_str());
    char* payload_ptr = payload_data;

    // Build the full path to cacert.pem
    QString certPath = QCoreApplication::applicationDirPath() + "/cacert.pem";
    if (!QFile::exists(certPath)) {
        qDebug() << "CA cert file not found at:" << certPath;
        free(payload_data);
        curl_easy_cleanup(curl);
        return false;
    }

    struct curl_slist* recipients = nullptr;

    curl_easy_setopt(curl, CURLOPT_USERNAME, from);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");

    // Set up SSL certificate options
    curl_easy_setopt(curl, CURLOPT_CAINFO, certPath.toStdString().c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
    recipients = curl_slist_append(nullptr, to);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payloadSource);
    curl_easy_setopt(curl, CURLOPT_READDATA, &payload_ptr);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    qDebug() << "Sending email...";

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        qDebug() << "Failed:" << curl_easy_strerror(res);
    else
        qDebug() << "Email sent successfully ✔️";

    // Clean up resources
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
    free(payload_data);

    return (res == CURLE_OK);
}
