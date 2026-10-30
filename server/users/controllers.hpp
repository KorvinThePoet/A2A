#pragma once

#include "../db/storage.hpp"
#include "models.hpp"
#include "exceptions.hpp"
#include "../tools/strtools.hpp"
#include "../requests/response.hpp"

#include <iostream>
#include <vector>

namespace Users {

class UserController{
private:
    Storage& storage;
    CreateUserRequest parseCreateUserRequest(const std::string &body);
public:
    UserController(Storage& storage) : storage(storage) {}
    

    Response create_user(const std::string& body);
    Response get_user(const std::string& route);
    Response get_all_users();

    bool update_user(int id, UpdateUserRequest& uur);
    bool delete_user(int id);
};
}