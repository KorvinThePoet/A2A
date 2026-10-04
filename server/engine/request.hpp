#include <string>

class Request{
    // 0 - POST
    // 1 - GET
    // 2 - PUT
    // 3 - DELETE
    size_t method;
    std::string route;
    std::string host;
    std::string content_type;
    size_t content_length;
    std::string body;

    bool bad_request;
public:
    Request(const std::string&);
};


