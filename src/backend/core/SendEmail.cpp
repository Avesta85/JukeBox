#include <QCoreApplication>
#include <QDebug>
#include <curl/curl.h>
#include <cstring>

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

bool sendMail(char* from , char* to , char* verifiMessege)
{
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        qDebug() << "❌ curl_easy_init failed!";
        return false;
    }

    char* password = "zizpjuzuanejognr";
    char* username = from;
    std::string payload_ =
    "To: " + std::string(to) + "\r\n"
    "From: " + std::string(from) + "\r\n"
    "Subject: vrifi code:\r\n" +
    std::string(verifiMessege) + "\r\n";

    char* payload_text = strdup(payload_.c_str());



    char* payload = payload_text;
    struct curl_slist* recipients = nullptr;

    curl_easy_setopt(curl, CURLOPT_USERNAME, username);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");

    // استفاده از گواهی معتبر
    curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
    recipients = curl_slist_append(nullptr, to);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payloadSource);
    curl_easy_setopt(curl, CURLOPT_READDATA, &payload);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    qDebug() << "📤 در حال ارسال ایمیل امن...";
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        qDebug() << "❌ خطا در ارسال:" << curl_easy_strerror(res);
    else
        qDebug() << "✅ ایمیل با موفقیت و امنیت ارسال شد!";

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
    return (res == CURLE_OK);
}

