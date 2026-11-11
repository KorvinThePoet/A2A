#pragma once

#include "../db/storage.hpp"
#include "models.hpp"
#include "exceptions.hpp"
#include "../tools/strtools.hpp"
#include "../requests/response.hpp"

#include <iostream>
#include <vector>
using namespace Requests;

namespace Users {

class UserController{
private:
    Storage& storage;
    CreateUserRequest parseCreateUserRequest(const std::string &body);
    LoginUserRequest parseLoginUserRequest(const std::string &body);
public:
    UserController(Storage& storage) : storage(storage) {}
    

    Response create_user(const std::string& body);
    Response get_user(const std::string& route);
    Response get_personal_info(const std::string& body);
    Response get_all_users();
    Response login(const std::string& body);

    bool update_user(int id, UpdateUserRequest& uur);
    bool delete_user(int id);
};
}