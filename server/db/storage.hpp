#pragma once
#include "../users/schemas.hpp"
#include "../chats/schemas.hpp"
#include "sqlite_orm/sqlite_orm.h"

inline auto makeStorage() {
    using namespace sqlite_orm;
    return make_storage("temp.db",
        makeUserTable(),
        makeTokenTable(),
        makeChatTable(),
        makeChatMemberTable(),
        makeMessageTable(),
        makeFileTable(),
        makeMessageFileTable()
    );
}


using Storage = decltype(makeStorage());
