#include "../db/storage.hpp"
#include "models.hpp"

#include <vector>

class UserController{
private:
    Storage& storage;
public:
    UserController(Storage& storage) : storage(storage) {}
    UserResponse create_user(const CreateUserRequest& cur);
    std::optional<UserResponse> get_user(int id);
    std::optional<std::vector<UserResponse>> get_all_users();

    bool update_user(int id, UpdateUserRequest& uur);
    bool delete_user(int id);
};
