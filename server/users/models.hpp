// DTO - data transfer objects
// Эти объекты учавствуют при непосредственно запроссаъ и ответовх
#pragma once
#include <string>

struct CreateUserRequest{
    std::string full_name;
    std::string username;
    std::string password;
    std::optional<std::string> pfp_path;
};

struct UserResponse{
    int id;
    std::string username;
    std::string full_name;

    UserResponse(const User& u) : username(u.username), full_name(u.full_name.value_or("")), id(u.id) {}
};

struct UpdateUserRequest{
    std::optional<std::string> new_username;
    std::optional<std::string> new_full_name;
    std::optional<std::string> new_hashed_password;
    std::optional<std::string> new_pfp_path;
    std::optional<int> age;    
};
