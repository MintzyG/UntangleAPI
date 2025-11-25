#include "http_client.h"
#include <curl/curl.h>
#include <sstream>
#include <iostream>

HttpClient::HttpClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

HttpClient::~HttpClient() {
    curl_global_cleanup();
}

size_t HttpClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t HttpClient::headerCallback(char* buffer, size_t size, size_t nitems, void* userdata) {
    auto* headers = (std::map<std::string, std::string>*)userdata;
    std::string header(buffer, size * nitems);
    
    size_t separator = header.find(':');
    if (separator != std::string::npos) {
        std::string key = header.substr(0, separator);
        std::string value = header.substr(separator + 1);
        
        value.erase(0, value.find_first_not_of(" \t\r\n"));
        value.erase(value.find_last_not_of(" \t\r\n") + 1);
        
        headers->insert({key, value});
    }
    
    return size * nitems;
}

HttpResponse HttpClient::performRequest(const std::string& method, const std::string& url, 
                                       const std::string& body, const std::map<std::string, std::string>& headers) {
    HttpResponse response;
    response.success = false;
    response.status_code = 0;
    
    CURL* curl = curl_easy_init();
    if (!curl) {
        response.error_message = "Failed to initialize CURL";
        return response;
    }
    
    std::string response_body;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
    
    struct curl_slist* curl_headers = nullptr;
    for (const auto& [key, value] : headers) {
        std::string header = key + ": " + value;
        curl_headers = curl_slist_append(curl_headers, header.c_str());
    }
    if (curl_headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
    }
    
    if (!body.empty()) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    }
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        response.error_message = curl_easy_strerror(res);
        response.success = false;
    } else {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        response.status_code = static_cast<int>(http_code);
        response.body = response_body;
        response.success = true;
    }
    
    if (curl_headers) {
        curl_slist_free_all(curl_headers);
    }
    curl_easy_cleanup(curl);
    
    return response;
}

HttpResponse HttpClient::get(const std::string& url, const std::map<std::string, std::string>& headers) {
    return performRequest("GET", url, "", headers);
}

HttpResponse HttpClient::post(const std::string& url, const std::string& body, const std::map<std::string, std::string>& headers) {
    return performRequest("POST", url, body, headers);
}

HttpResponse HttpClient::put(const std::string& url, const std::string& body, const std::map<std::string, std::string>& headers) {
    return performRequest("PUT", url, body, headers);
}

HttpResponse HttpClient::del(const std::string& url, const std::map<std::string, std::string>& headers) {
    return performRequest("DELETE", url, "", headers);
}
