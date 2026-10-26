#include <string>
#include <map>

class Response{
    int status;
    std::map<std::string, std::string> headers;
    std::string body;
public:
    Response() : status(404), headers(), body(){}
    Response(int status, std::map<std::string, std::string> headers, std::string body) :
            status(status),
            headers(headers),
            body(body)
    {}

    
    std::string to_string();
};
