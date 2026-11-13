#pragma once

#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <optional>
#include "../users/schemas.hpp"

// надо буде каритку для чатков
// какая еще калитка ты че
struct Chat{
    int id;
    std::string title;
    bool is_group;
};

struct Message{
    int id;
    int chat_id;
    int user_id;
    std::string text;
    std::time_t created_at;
};

struct File{
    int id;
    std::string file_name;
    int size;
    std::string path;
};

struct MessageFile{
    int message_id;
    int file_id;
};

struct ChatMember{
    int chat_id;
    int user_id;
    std::string role = "member";
    std::time_t joined_at;
    std::time_t last_online;
};

inline auto makeChatTable() {
    using namespace sqlite_orm;
    return make_table("chats",
        make_column("id", &Chat::id, primary_key().autoincrement()),
        make_column("title", &Chat::title),
        make_column("is_group", &Chat::is_group)
    );
}

inline auto makeChatMemberTable(){
    using namespace sqlite_orm;

    return make_table("chat_members",
        make_column("chat_id", &ChatMember::chat_id),
        make_column("user_id", &ChatMember::user_id),
        make_column("joined_at", &ChatMember::joined_at),
        make_column("last_online", &ChatMember::last_online),
        make_column("role", &ChatMember::role),
        primary_key(&ChatMember::chat_id, &ChatMember::user_id),
        foreign_key(&ChatMember::chat_id).references(&Chat::id).on_delete.cascade(),
        foreign_key(&ChatMember::user_id).references(&User::id).on_delete.cascade()
    );
}

inline auto makeMessageTable(){
    using namespace sqlite_orm;

    return make_table("messages",
        make_column("id", &Message::id, primary_key().autoincrement()),
        make_column("chat_id", &Message::chat_id),
        make_column("user_id", &Message::user_id),
        make_column("text", &Message::text),
        make_column("created_at", &Message::created_at),
        foreign_key(&Message::chat_id).references(&Chat::id).on_delete.cascade(),
        foreign_key(&Message::user_id).references(&User::id)
    );
}

inline auto makeFileTable(){
    using namespace sqlite_orm;
    return make_table("files",
        make_column("id", &File::id, primary_key().autoincrement()),
        make_column("file_name", &File::file_name),
        make_column("size", &File::size),
        make_column("path", &File::path)
    );
}

inline auto makeMessageFileTable(){
    using namespace sqlite_orm;
    return make_table("message_file",
        make_column("message_id", &MessageFile::message_id),
        make_column("file_id", &MessageFile::file_id),
        primary_key(&MessageFile::message_id, &MessageFile::file_id),
        foreign_key(&MessageFile::message_id).references(&Message::id).on_delete.cascade(),
        foreign_key(&MessageFile::file_id).references(&File::id)
    );
}
