#pragma once
#include <string>

class BasicException{
protected:
    size_t code;
    std::string message;
    BasicException(size_t code, const std::string& message);
public:
    size_t exception_code() const;
    std::string exception_message() const;
};

class UserNotFound : public BasicException {
public:
    UserNotFound(const std::string &message);
};


