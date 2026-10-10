#include <string>
#include <vector>
#include "tools/datetime.hpp"

class Message{
private:
    size_t id;
    size_t sender_id;
    size_t chat_id;
    DateTime::datetime send_time;
    std::string text;
    std::vector<size_t> ids_attached_files;
};