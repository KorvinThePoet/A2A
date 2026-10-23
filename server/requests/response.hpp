#include <string>
#include <map>

class Response{
    int status;
    std::map<std::string, std::string> headers;
    std::string body;
public:
    std::string to_string();
};