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
        Response create_message(const std::string& body);
        Response delete_message(const std::string& body);
        Response add_user(const std::string& body);
        Response send_file(const std::string& body);
        Response change_user_role(const std::string& body);

        Response delete_chat(const std::string& body);
    };
}
