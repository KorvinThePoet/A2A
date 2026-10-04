#include <string>

class Request{
    std::string method;
    std::string route;
    std::string host;
    std::string content_type;
    size_t content_length;
    std::string body;

    Request(const std::string&);
};

