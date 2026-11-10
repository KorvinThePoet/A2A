#pragma once

#include <string>
#include <map>
#include <iostream>

class Response{
protected: 
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

class MessageResponse : public Response {
public:
    MessageResponse(int status, std::string message){
        body = "{'message':'" + message + "'}";
        this->status = status;
    }
};

