#pragma once

#include "../db/storage.hpp"
#include "models.hpp"
#include "exceptions.hpp"
#include "../tools/strtools.hpp"
#include "../requests/response.hpp"

#include <iostream>
#include <vector>

namespace Chats {

    class ChatController {
    private:
        Storage& storage;
    public:
        ChatController(Storage& storage) : storage(storage) {}
        Response create_chat(const std::string& body);
        Response create_message_to_chat(const std::string& body);
        Response delete_message_from_chat(const std::string& id);
        Response add_user_to_chat(const std::string& body);
        Response send_file_to_chat(const std::string& body);
        Response change_user_role(const std::string& body);

        bool delete_chat(int id);
    };
}
