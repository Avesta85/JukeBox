#ifndef SENDEMAIL_H
#define SENDEMAIL_H
#include <QCoreApplication>
#include <QDebug>
#include <curl/curl.h>
#include <cstring>



size_t payloadSource(void* ptr, size_t size, size_t nmemb, void* userp);

bool sendMail(char* from , char* to , char* verifiMessege);


#endif
