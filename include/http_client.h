#pragma once
#include <string>
#include <map>
#include <functional>

struct HttpResponse {
    int status_code;
    std::string body;
    std::map<std::string, std::string> headers;
    std::string error_message;
    bool success;
};

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    HttpResponse get(const std::string& url, const std::map<std::string, std::string>& headers = {});
    HttpResponse post(const std::string& url, const std::string& body, const std::map<std::string, std::string>& headers = {});
    HttpResponse put(const std::string& url, const std::string& body, const std::map<std::string, std::string>& headers = {});
    HttpResponse del(const std::string& url, const std::map<std::string, std::string>& headers = {});

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static size_t headerCallback(char* buffer, size_t size, size_t nitems, void* userdata);
    
    HttpResponse performRequest(const std::string& method, const std::string& url, 
                                const std::string& body, const std::map<std::string, std::string>& headers);
};
