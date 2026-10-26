// Как хранятся данные в табличке
#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <optional>

struct User{
    int id;
    std::optional<int>  age;

    std::string username;
    std::optional<std::string> full_name;
    std::string hashed_password;
    std::string pfp_path;
    std::time_t last_online;

    User() : pfp_path("/static/default/balbes.jpg") {}
};

inline auto makeUserTable() {
    using namespace sqlite_orm;
    return make_table("users",
            make_column("id", &User::id, primary_key().autoincrement()),
            make_column("username", &User::username, unique()),
            make_column("full_name", &User::full_name),
            make_column("hashed_password", &User::hashed_password),
            make_column("age", &User::age),
            make_column("last_online", &User::last_online),
            make_column("pfp_path", &User::pfp_path, default_value("/static/default/balbes.jpg"))
    );
}
