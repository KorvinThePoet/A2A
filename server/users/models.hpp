// DTO - data transfer objects
// Эти объекты учавствуют при непосредственно запроссаъ и ответовх
#pragma once
#include <string>
#include "schemas.hpp"

namespace Users {
struct CreateUserRequest{
    std::string full_name;
    std::string username;
    std::string password;
    std::optional<std::string> pfp_path;

    CreateUserRequest(std::string full_name, std::string usernmae, std::string password, std::string pfp_path) :
        full_name(full_name), password(password), username(usernmae) {
            if(pfp_path == "") {
                this->pfp_path = std::nullopt;
            }
        }
};

struct UserResponse{
    int id;
    std::string username;
    std::string full_name;
    std::string pfp_path;

    UserResponse(const User& u) : username(u.username), full_name(u.full_name.value_or("")), id(u.id), pfp_path(u.pfp_path) {}
};

struct UpdateUserRequest{
    std::optional<std::string> new_username;
    std::optional<std::string> new_full_name;
    std::optional<std::string> new_hashed_password;
    std::optional<std::string> new_pfp_path;
    std::optional<int> new_age;
};
}