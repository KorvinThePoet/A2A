// DTO - data transfer objects
// Эти объекты учавствуют при непосредственно запроссаъ и ответовх
#pragma once
#include <string>
#include <map>
#include "schemas.hpp"
#include "../tools/strtools.hpp"

namespace Users {
struct CreateUserRequest{
    std::string full_name;
    std::string username;
    std::string email;
    std::string password;
    std::optional<std::string> pfp_path;

    CreateUserRequest(std::string full_name, std::string username, std::string email, std::string password, std::string pfp_path) :
        full_name(full_name), email(email), password(password), username(username) {
            if(pfp_path == "") {
                this->pfp_path = std::nullopt;
            }
        }
};

struct LoginUserRequest{
    std::string username;
    std::string password;
};

struct UserResponse{
    int id;
    std::string username;
    std::string full_name;
    std::string pfp_path;

    std::map<std::string, std::string> json;

    UserResponse(const User& u) : username(u.username), full_name(u.full_name.value_or("")), id(u.id), pfp_path(u.pfp_path) {
        json["id"] = std::to_string(id);
        json["username"] = username;
        json["full_name"] = full_name;
        json["pfp_path"] = pfp_path;
    }

    std::string to_json() {
        return map_to_string(json);
    }
};

struct DetailedUserResponse : public UserResponse {
    std::string email;
    DetailedUserResponse(const User& u) : UserResponse(u), email(u.email) {
        json["email"] = email;
    }
};

struct UpdateUserRequest{
    std::optional<std::string> new_username;
    std::optional<std::string> new_full_name;
    std::optional<std::string> new_hashed_password;
    std::optional<std::string> new_pfp_path;
    std::optional<int> new_age;
};
}