#include <string>
#include "tools/datetime.hpp"

class UserBaseModel{
private:
    size_t id;
    std::string username;
    std::string hashed_password;
    std::string full_name;
    DateTime::datetime registered_at;
};
