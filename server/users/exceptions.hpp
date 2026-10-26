#pragma once
#include <exception>

class PasswordShort : public std::runtime_error {
public:   PasswordShort() : 
            std::runtime_error("Password is too short") {}
};

class UsernameNotUnique : public std::runtime_error {
public:    UsernameNotUnique(const std::string& username) : 
            std::runtime_error("Username " + username + " is already taken") {}

};

class UserNotFound : public std::runtime_error {
public:    UserNotFound(int id) : 
              std::runtime_error("User with id=" + std::to_string(id) + " not found") {}
}


;