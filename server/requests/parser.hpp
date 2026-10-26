// PARSE TO DTO

#pragma once
#include <map>
#include <string>
#include "requests.hpp"
#include "../users/models.hpp"

struct Parser{

    // parse to request + parsing the payload typeshi

    // USER PARSE SHI
    CreateUserRequest toCUR(const std::string&);
    UserResponse toUR(const std::string&);
    UpdateUserRequest toUUR(const std::string&);
    int toUMR(const std::string&);
};
