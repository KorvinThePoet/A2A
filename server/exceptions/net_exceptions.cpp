#include "net_exceptions.hpp"

BasicException::BasicException(size_t code, const std::string& message) {
    this->code = code;
    this->message = message;
}

size_t BasicException::exception_code() const{
    return code;
}

std::string BasicException::exception_message() const{
    return message;
}

UserNotFound::UserNotFound(const std::string &message) : BasicException(100, message) {}

