#include "controllers.hpp"

// GOD BLESS

bool Chats::ChatController::check_auth(int user_id, const std::string& token) {
    auto db_out = storage.get_all<Token>(sqlite_orm::where(sqlite_orm::c(&Token::user_id) == user_id));
    if(db_out.empty()) {
        return false;
    }

    Token _token = db_out[0];

    return _token.hashed_token == hash_token(token);

}


Requests::Response Chats::ChatController::get_chat_from(const std::string& body) {
    // body: 
    //  user_id - отправитель, 
    //  token - токен мой ну или другой
    //  chat_id - куда кидаем, точнее в какой чат черт его дери, что там платина несет
    //  time - от какого время кинуть сообщения, 
    //          делема! скидываем в DD.MM.YYYYTH:mm:ss 
    //          или же как число же есть, удобнее как будто первый вариант
    //  Лады, сделаем первый варик      


    auto data = to_map(body);

    Chats::AuthenticatedUserRequest req = {data["token"], data["user_id"]};

    if(req.is_empty() || !check_auth(req.user_id, req.token)) {
        return Requests::MessageResponse(404, "ur no t uthenticcc(");
    }
    
    // user_id chat_id
    // select * from ChatMember where chat_id=chat_id && user_id = user_id

    auto db_out = storage.get_all<ChatMember>(sqlite_orm::where(
        sqlite_orm::c(&ChatMember::chat_id) == string_to_int(data["chat_id"]) 
        and sqlite_orm::c(&ChatMember::user_id) == string_to_int(data["user_id"])
    ));

    if (db_out.empty()) {
        return Requests::MessageResponse(404, "U dont hav akces");
    }
    ChatMember mem = db_out[0];

    // thx gemini
    std::tm tm = {};
    std::istringstream ss(data["time"]);
    ss >> std::get_time(&tm, "%d.%m.%YT%H:%M:%S");
    if (ss.fail()) {
        return Requests::MessageResponse(400, "Wrong time че то там");
    }
    tm.tm_isdst = -1;
    std::time_t time_from = std::mktime(&tm);
    
    // нужна ли обновлят last_onliне в чатмембер жиесть хз ато я хз(не знаю я хз)
    // Вообще mafaker может сохранять инфу в себя на стороне бич
    auto messages = storage.get_all<Message>(
        sqlite_orm::where(
                sqlite_orm::c(&Message::chat_id) == mem.chat_id 
                and
                sqlite_orm::c(&Message::created_at) > time_from
        )
    );


    return Requests::Response(200, {}, Chats::ChatResponse(messages).to_json());
}


Requests::Response Chats::ChatController::create_chat(const std::string& body) {
    // только аутентифицированный пльлзователь сможет создать чат и не более 3 на рыло
    // ну проверку потом реализуем, хотя почему 3, пусть бедт 5
    // ни тебе ни мне 6
    // body:
    //  user_id - создатель
    //  token - его токен
    //  title - има
    //  is_group - создаем группу или личный чат
    auto json = to_map(body);
    // че за прикол, там было req а здесь aur
    // хз, поприколу
    Chats::CreateChatRequest ccr = {json["user_id"], json["token"], json["title"], json["is_group"]};
    if (ccr.is_empty() || !check_auth(ccr.user_id, ccr.token) ) {
        return Requests::MessageResponse(404, "SUKA T bI 4E O T II P A B N V" );
    }
    
    try {
        Chat chat {-1, ccr.title, ccr.is_group};
        int new_chat_id = storage.insert(chat);
        ChatMember admin {new_chat_id, ccr.user_id, "admin", std::time_t(std::time(nullptr)), std::time_t(std::time(nullptr))};
        storage.insert(admin);
    }catch(const std::runtime_error& e) {
        return Requests::MessageResponse(404, e.what());
    }

    return Requests::MessageResponse(200, "создали нормально. А. Надо на англинском. created successfully");

}

Requests::Response Chats::ChatController::create_message(const std::string& body) {
    // // bdoy
    //  user_id - creator
    //  token - creators' token fff
    //  chat_id - destination
    //  text - obvious
    //  других нету

    auto json = to_map(body);
    Chats::AuthenticatedUserRequest aur = {json["token"], json["user_id"]};
    if (aur.is_empty() || !check_auth(aur.user_id, aur.token)
                      or json["chat_id"].empty() || json["user_id"].empty()
                         or json["text"].empty() || json["time"].empty()) {
        return Requests::MessageResponse(404, "подкинули" );
    }
    
    Message msg {-1, string_to_int(json["chat_id"]), string_to_int(json["user_id"]),
                 json["text"], std::time_t(std::time(nullptr))};
    try {
        storage.insert(msg);
    } catch(const std::runtime_error& e) {
        return Requests::MessageResponse(228, e.what());
    }
    return Requests::MessageResponse(200, "все ок бич");
}

Requests::Response Chats::ChatController::delete_message(const std::string& body) {
    // bode
    // user_id - whos message
    // token - users token
    // chat_id - in which chat
    // message_id - message,  зачем тогда chat_id если мы удаляем сообщение, типа 
    //              в двух чатах не может быть два сообщения с одинаковым ид

    auto json = to_map(body);
    Chats::AuthenticatedUserRequest aur = {json["token"], json["user_id"]};
    if(aur.is_empty() || !check_auth(aur.user_id, aur.token) 
        || json["message_id"].empty()) {
            return Requests::MessageResponse(400, "Wrong wha");
        }
    auto messages = storage.get_all<Message>(
        sqlite_orm::where(
            sqlite_orm::c(&Message::id) == string_to_int(json["message_id"])
            and sqlite_orm::c(&Message::user_id) == string_to_int(json["user_id"])
        )
    );
    if (messages.empty()) {
        return Requests::MessageResponse(400, "No messages found");
    }
    try {
        Message message = messages[0];
        storage.remove<Message>(message.id);
    } catch(const std::runtime_error& e) {
        return Requests::MessageResponse(501, "Error while deletin shi");
    }
    return Requests::MessageResponse(200, "deleted successfully");
}

Requests::Response Chats::ChatController::add_user(const std::string& body) {
    // body
    //  user_id - admin in chat, who wants to add add user
    //  new_user_id - whos adding
    //  token - admins token
    //  chat_id

    auto json = to_map(body);
    Chats::AddUserRequest aur {json["user_id"], json["token"], json["new_user_id"], json["chat_id"]};

    if(!aur.is_empty() || !check_auth(aur.user_id, aur.token)) {
        return Requests::MessageResponse(400, "Wrong årgument√");
    }
    
    try {
        ChatMember new_user {aur.chat_id, aur.user_id, "member", std::time_t(std::time(nullptr)), std::time_t(std::time(nullptr))};
        auto members = storage.get_all<ChatMember>(
            sqlite_orm::where(
                sqlite_orm::c(&ChatMember::user_id) == new_user.user_id
            )
        );

        if(!members.empty()) {
            return Requests::MessageResponse(400, "User already in chat");
        }
        storage.insert(new_user);
    } catch(const std::runtime_error& e) {
        return Requests::MessageResponse(400, e.what());
    }
    return Requests::MessageResponse(200, "Done");
}

Requests::Response Chats::ChatController::send_file(const std::string& body) {
    return Requests::MessageResponse(200, "Сделаем выд что все ок, окей?");
}

Requests::Response Chats::ChatController::change_user_role(const std::string& body) {
    // body
    // user_id
    // token
    // role - admin or member
    // target_user_id
    // chat_id
    auto json = to_map(body);

    AuthenticatedUserRequest aur {json["token"], json["user_id"]};

    if(aur.is_empty() || !check_auth(aur.user_id, aur.token) || 
        json["role"].empty() || json["chat_id"].empty() || json["target_user_id"].empty()) {
            return Requests::MessageResponse(404, "ZNAET kto?");
        }
    try {
        auto cur_user = storage.get_all<ChatMember>(
            sqlite_orm::where(
                sqlite_orm::c(&ChatMember::chat_id) == string_to_int(json["chat_di"])
                and sqlite_orm::c(&ChatMember::user_id) == aur.user_id
            )
        );
        if(cur_user.empty() || cur_user[0].role != "admin") {
            return Requests::MessageResponse(400, "UZHE BOUC");
        }
        auto target_members = storage.get_all<ChatMember>(
            sqlite_orm::where(
                sqlite_orm::c(&ChatMember::chat_id) == string_to_int(json["chat_id"])
                and sqlite_orm::c(&ChatMember::user_id) == string_to_int(json["target_user_id"])
            )
        );

        if (target_members.empty()) {
            return Requests::MessageResponse(404, "hz pochemu");
        }
        auto member = target_members[0];
        if(member.role == json["role"]) {
            return Requests::MessageResponse(200, "WTF");
        }
        if(json["role"] == "admin") {
            member.role = "admin";
        }else
        if(json["role"] == "member") {
            member.role = "member";
        } else {
            return Requests::MessageResponse(400, "Dont know dis rol");
        }
        storage.update(target_members[0]);

    } catch(const std::runtime_error& e) {
        return Requests::MessageResponse(404, e.what());
    }
    return Requests::MessageResponse(200, "OK");
}

Requests::Response Chats::ChatController::get_chat(const std::string& body) {
    // body
    // user_id
    // token
    // chat_id
    auto json = to_map(body);

    json["time"] = "13.11.2026T19:40:00"; // Это щас у меня столько время
    // бля уже 13.11.2026T19:41:00
    std::string new_body = map_to_string(json);
    return get_chat_from(new_body);
}

Requests::Response Chats::ChatController::get_users_chats(const std::string& body) {
    // body
    // user_id
    // token
    // chat_id
    auto json = to_map(body);

    AuthenticatedUserRequest aur {json["token"], json["user_id"]};
    if(aur.is_empty() || json["chat_id"].empty() || !check_auth(aur.user_id, aur.token)) {
        return Requests::MessageResponse(302, "FUck u");
    }
    try {
        auto members = storage.get_all<ChatMember>(
            sqlite_orm::where(
                sqlite_orm::c(&ChatMember::chat_id) == string_to_int("chat_id")
            )
        );

        if(members.empty()) {
            return Requests::MessageResponse(3033, "FF");
        }
        bool in_chat = false;
        for (auto member : members) {
            if(member.user_id == aur.user_id) {
                in_chat = true;
                break;
            }
        }
        if(!in_chat) {
            return Requests::MessageResponse(812, "paris");
        }
        return Requests::Response(67, {}, ChatMembersResponse(members[0].chat_id, members).to_json());
    }catch(const std::runtime_error& e){
        return Requests::MessageResponse(404, e.what());
    } catch(...){
        return Requests::MessageResponse(404, "What if it throws someth else");
    }
}

Requests::Response Chats::ChatController::delete_chat(const std::string& body) {
    // body
    // user_id
    // token
    // chat_id\ 
    
    auto json = to_map(body);

    AuthenticatedUserRequest aur {json["token"], json["user_id"]};
    if(aur.is_empty() || json["chat_id"].empty()) {
        return Requests::MessageResponse(302, "FUck u");
    }
    try{
        auto memberr = storage.get_all<ChatMember>(
            sqlite_orm::where(
                sqlite_orm::c(&ChatMember::chat_id) == string_to_int(json["chat_id"])
                and sqlite_orm::c(&ChatMember::user_id) == aur.user_id
            )
        );

        if(memberr.empty() || memberr[0].role != "admin") {
            return Requests::MessageResponse(200, "BAL🥀");
        }
        storage.remove<Chat>(string_to_int(json["chat_id"]));
    } catch(const std::runtime_error& e) {
        return Requests::MessageResponse(404, e.what());
    } catch(...) {
        return Requests::MessageResponse(403, "я закончил это дермо");
    }
    return Requests::MessageResponse(200, "УРАА");
}
