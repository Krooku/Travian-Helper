#pragma once

#include <string>
#include <curl/curl.h>

static std::string payloadText[11];

std::string dateTimeNow();
std::string generateMessageId();

void setPayloadText(const std::string& to, const std::string& from, const std::string& cc, const std::string& nameFrom, const std::string& subject, const std::string& body);

std::string dateTimeNow();
std::string generateMessageId();

struct upload_status { int lines_read; };

static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp);

CURLcode sendEmail(const std::string& to, const std::string& from, const std::string& cc, const std::string& nameFrom, const std::string& subject, const std::string& body, const std::string& url, const std::string& password);