#include "controllers.hpp"

// GOD BLESS

bool Chats::ChatController::check_auth(size_t user_id, const std::string& token) {
    auto db_out = storage.get_all<Token>(sqlite_orm::where(sqlite_orm::c(&Token::user_id) == user_id));
    if(db_out.empty()) {
        return false;
    }

    Token _token = db_out[0];

    return _token.hashed_token == hash_token(token);

}



Response Chats::ChatController::get_chat_from(const std::string& body) {
    auto data = to_map(body);

    AuthenticatedUserRequest req = {data["token"], data["user_id"]};

    if(req.token.empty() || req.user_id.empty() || !check_auth(string_to_size_t(req.user_id), req.token)) {
        return MessageResponse(404, "ur no t uthenticcc(");
    }
    
    // user_id chat_id
    // select * from ChatMember where chat_id=chat_id && user_id = user_id

    auto db_out = storage.get_all<ChatMember>(sqlite_orm::where(
        sqlite_orm::and_(
            sqlite_orm::c(&ChatMember::chat_id) == string_to_size_t(data["chat_id"]),
            sqlite_orm::c(&ChatMember::user_id) == string_to_size_t(data["user_id"])
        )
    ));

    if (db_out.empty()) {
        return MessageResponse(404, "U dont hav akces");
    }
    ChatMember mem = db_out[0];
    
    // нужна ли обновлят last_onliне в чатмембер жиесть хз ато я хз(не знаю я хз)
    auto messages = storage.get_all<Message>(
        sqlite_orm::where(sqlite_orm::c(&Message::chat_id) == mem.chat_id)
    );

    return Response(200, {}, ChatResponse(messages).to_json());
    
}