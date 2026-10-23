// DTO - data transfer objects
// Эти объекты учавствуют при непосредственно запроссаъ и ответовх

#include <string>

struct CreateUserRequest{
    std::string full_name;
    std::string username;
    std::string password;
};

struct UserResponse{
    std::string username;
    std::string full_name;
};

struct UpdateUserRequest{
    std::optional<std::string> new_username;
    std::optional<std::string> new_full_name;
    std::optional<std::string> new_hashed_password;
    std::optional<std::string> new_pfp_path;
    std::optional<int> age;    
};
