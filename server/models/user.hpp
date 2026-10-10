#include <string>
#include <file.hpp>

class User{
private:
    size_t id;
    std::string username;
    std::string hashed_password;
    // Q? нужно ли поле info
    // std::string info;
    std::vector<size_t> chat_ids;
public:
    User(const std::string &username, const std::string &hashed_password, const std::vector<size_t> &chat_ids);
    void send_message(size_t chat_id);
    void delete_message(size_t chat_id, size_t message_id);
    void upload_on_server(const File&);
    void create_chat();

};