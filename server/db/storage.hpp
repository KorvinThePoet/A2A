#pragma once
#include "../users/schemas.hpp"
#include "sqlite_orm/sqlite_orm.h"

inline auto makeStorage() {
    using namespace sqlite_orm;
    return make_storage("temp.db",
        makeUserTable(),
        makeTokenTable()
    );
}


using Storage = decltype(makeStorage());
