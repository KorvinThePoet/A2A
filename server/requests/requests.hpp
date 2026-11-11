#pragma once

#include <string>
namespace Requests {
class Request{
    // 0 - POST
    // 1 - GET
    // 2 - PUT
    // 3 - DELETE
    int method;
    
    std::string route;
    std::string host;
    std::string content_type;
    int content_length;
    std::string body;

    bool bad_request;
public:
    Request(const std::string&);

    std::string get_route() const;
    std::string get_host() const;
    std::string get_content_type() const;
    std::string get_body() const;
    int get_content_length() const;
    bool is_bad_request() const;
    std::string get_method() const;

    std::string to_what_mama_rodila();
};
}