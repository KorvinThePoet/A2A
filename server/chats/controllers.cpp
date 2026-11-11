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


    auto data = to_map(body);

    Chats::AuthenticatedUserRequest req = {data["token"], data["user_id"]};

    if(req.token.empty() || req.user_id.empty() || !check_auth(string_to_int(req.user_id), req.token)) {
        return Requests::MessageResponse(404, "ur no t uthenticcc(");
    }
    
    // user_id chat_id
    // select * from ChatMember where chat_id=chat_id && user_id = user_id

    auto db_out = storage.get_all<ChatMember>(sqlite_orm::where(
        sqlite_orm::and_(
            sqlite_orm::c(&ChatMember::chat_id) == string_to_int(data["chat_id"]),
            sqlite_orm::c(&ChatMember::user_id) == string_to_int(data["user_id"])
        )
    ));

    if (db_out.empty()) {
        return Requests::MessageResponse(404, "U dont hav akces");
    }
    ChatMember mem = db_out[0];
    
    // нужна ли обновлят last_onliне в чатмембер жиесть хз ато я хз(не знаю я хз)
    // Вообще mafaker может сохранять инфу в себя на стороне бич
    auto messages = storage.get_all<Message>(
        sqlite_orm::where(sqlite_orm::c(&Message::chat_id) == mem.chat_id)
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
    Chats::AuthenticatedUserRequest aur = {json["token"], json["user_id"]};
    if (aur.token.empty() or aur.user_id.empty() || !check_auth(string_to_int(aur.user_id), aur.token)
                        or json["title"].empty() || json["is_group"].empty()
          or tolower(json["is_group"]) != "true" && tolower(json["is_group"]) != "false" ) {
        return Requests::MessageResponse(404, "SUKA T bI 4E O T II P A B N V" );
    }
    bool is_group = true;
    if (tolower(json["is_group"]) == "true") {
        is_group = false;
    }
    
    try {

        Chat chat {-1, json["title"], is_group};
        storage.insert(chat);
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
    if (aur.token.empty() or aur.user_id.empty() || !check_auth(string_to_int(aur.user_id), aur.token)
                        or json["chat_id"].empty() || json["user_id"].empty()
                        or json["text"].empty()) {
        return Requests::MessageResponse(404, "подкинули" );
    }
    // на завтра оставлю, сделаю наверное пофик мне кароче мне че, фига, ухты, 
    // это что за общество,нуладно
    // интересно это будет читать кроме меня же(меня это яане ты,🤔)
}

Requests::Response Chats::ChatController::delete_message(const std::string& body) {

}

Requests::Response Chats::ChatController::add_user(const std::string& body) {

}

Requests::Response Chats::ChatController::send_file(const std::string& body) {

}

Requests::Response Chats::ChatController::change_user_role(const std::string& body) {

}

Requests::Response Chats::ChatController::get_chat(const std::string& body) {

}

Requests::Response Chats::ChatController::get_chat_from(const std::string& body) {

}


Requests::Response Chats::ChatController::delete_chat(const std::string& body) {

}
