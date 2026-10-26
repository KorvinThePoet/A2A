#include "controllers.hpp"

std::string hash_pwd(const std::string& pwd) {
    return "damn_that_password's hashed:" + pwd;
}

UserResponse UserController::create_user(const CreateUserRequest& cur) {
    if(cur.password.size() < 5) {
        throw PasswordShort();
    }

    User new_user;
    new_user.username = cur.username;
    new_user.full_name = cur.full_name;
    new_user.hashed_password = hash_pwd(cur.password);
    new_user.last_online = std::time(nullptr);
    if(cur.pfp_path.has_value()) {
        new_user.pfp_path = cur.pfp_path.value();
    }
    

    try {
        storage.insert(new_user);
    } catch(const std::system_error& e) {
        throw UsernameNotUnique(new_user.username);
    }
    return UserResponse(new_user);
}

std::optional<UserResponse> UserController::get_user(int id) {
    try {
        auto user = storage.get<User>(id);
        return UserResponse(user);
    } catch(const std::system_error& e) {
        return std::nullopt;
    } 
    return std::nullopt;
}
