#pragma once
#include "server/users/schemas.hpp"
#include "sqlite_orm/sqlite_orm.h"

inline auto makeStorage() {
    using namespace sqlite_orm;
    return make_storage("temp.db",
        makeUserTable()    
    );
}

using Storage = decltype(makeStorage());
