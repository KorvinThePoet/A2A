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
        Response create_chat(const std::string& name);
        bool delete_chat(int id);
    };
}
