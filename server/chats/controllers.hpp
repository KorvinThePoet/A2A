#pragma once

#include "../db/storage.hpp"
#include "models.hpp"
#include "exceptions.hpp"
#include "../tools/strtools.hpp"
#include "../requests/response.hpp"
#include "../users/schemas.hpp"

#include <iostream>
#include <vector>

namespace Chats {

    class ChatController {
    private:
        Storage& storage;

        bool check_auth(int user_id, const std::string& hashed_token);
    public:
        ChatController(Storage& storage) : storage(storage) {}
        Requests::Response create_chat(const std::string& body);
        Requests::Response create_message(const std::string& body);
        Requests::Response delete_message(const std::string& body);
        Requests::Response add_user(const std::string& body);
        Requests::Response send_file(const std::string& body);
        Requests::Response change_user_role(const std::string& body);
        Requests::Response get_chat(const std::string& body);
        Requests::Response get_chat_from(const std::string& body);

        Requests::Response delete_chat(const std::string& body);
    };
}
