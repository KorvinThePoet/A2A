#pragma once

namespace Chats {
struct AuthenticatedUserRequest {
    virtual bool is_empty() const{
        return token.empty() || user_id == 0;
    }
    std::string token;

    int user_id;

    AuthenticatedUserRequest(const std::string& token, const std::string& user_id) : token(token), user_id(string_to_int(user_id)) {}

    virtual ~AuthenticatedUserRequest() = default;
};

struct FileResponse : File {
    std::string to_json() {
        std::map<std::string, std::string> json_shi;
        json_shi["id"] = std::to_string(this->id);
        json_shi["path"] = this->path;
        json_shi["file_name"] = this->file_name;
        json_shi["size"] = this->size;
        
        return map_to_string(json_shi);
    }

    FileResponse(const File& file) : File(file) {}
};

struct MessageResponse : Message{
    std::vector<File> attached_files;

    MessageResponse(const Message& msg, const std::vector<File>& attached_files) :
            Message(msg), attached_files(attached_files) {}
    
    MessageResponse(const Message& msg) : Message(msg) {}
    
    std::string to_json() {
        std::map<std::string, std::string> json_shi;
        json_shi["id"] = std::to_string(this->id);
        json_shi["chat_id"] = std::to_string(this->chat_id);
        json_shi["user_id"] = std::to_string(this->user_id);
        // hz где экринрование проверияь а то вдруг буеде text = "Arthur's code is unsafe"
        // типа в  json тогда получится {..., 'text': 'Arthur's code....}
        // и типа плохо навперное ну это надао с асланом разговаривать
        json_shi["text"] = this->text;
        // сделать чтобы std::time_t в че то типа DD:MM:YY HH:MM:SS
        // ну как то пофик мине
        // кто-то сделайте пжж
        std::map<std::string, std::string> files_map;
        for(int i = 0; i < attached_files.size(); ++i) {
            files_map[std::to_string(i)] = FileResponse(attached_files[i]).to_json();
        }
        json_shi["attacheck_files"] = map_to_string(files_map);

        return map_to_string(json_shi);
    }
};



struct ChatMembersResponse {
    int chat_id;
    std::vector<ChatMember> members;

    ChatMembersResponse(int chat_id, const std::vector<ChatMember>& mems) : members(mems) {

    }

    std::string to_json() {
        std::map<std::string, std::string> res;
        res["chat_id"] = std::to_string(chat_id);

        std::map<std::string, std::string> members_map;
        for(int i = 0; i < members.size(); ++i) {
            std::map<std::string, std::string> cur_member;
            cur_member["user_id"] = std::to_string(members[i].user_id);
            cur_member["chat_id"] = std::to_string(members[i].chat_id);
            cur_member["role"] = members[i].role;
            cur_member["chat_id"] = std::to_string(members[i].chat_id);
            cur_member["last_online"] = formatTime(members[i].last_online);
            cur_member["joined_at"] = formatTime(members[i].joined_at);

            members_map[std::to_string(members[i].user_id)] = map_to_string(cur_member);
        }
        res["members"] = map_to_string(members_map);
        return map_to_string(res);
    }
};

struct ChatResponse {
    std::vector<Message> messages;

    ChatResponse(const std::vector<Message> &messages) : messages(messages) {}
    std::string to_json() {
        std::map<std::string, std::string> json_shi;
        for(int i = 0; i < messages.size(); ++i) {
            json_shi[std::to_string(i)] = Chats::MessageResponse(messages[i]).to_json();
        }
        return map_to_string(json_shi);
    }
};

struct CreateChatRequest : public AuthenticatedUserRequest {
    std::string title;
    bool is_group;
    bool bad_request;

    CreateChatRequest(const std::string& user_id, const std::string& token, const std::string& title, const std::string& is_group)
        : AuthenticatedUserRequest(token, user_id), title(title) {
            bad_request = false;
            if(tolower(is_group) == "true") {
                this->is_group = false;
                return;
            }
            if(tolower(is_group) == "false") {
                this->is_group = false;
                return;
            }
            bad_request = true;
        }
    
    bool is_empty() const override {
        return AuthenticatedUserRequest::is_empty() || bad_request || title.empty();
    }
};

struct AddUserRequest : public AuthenticatedUserRequest {
    int new_user_id;
    int chat_id;

    AddUserRequest(const std::string& user_id, const std::string& token, 
        const std::string& new_user_id, const std::string& chat_id) : 
                AuthenticatedUserRequest(token, user_id),
                new_user_id(string_to_int(new_user_id)), 
                chat_id(string_to_int(chat_id)) {}
    
    bool is_empty() const override{
        return AuthenticatedUserRequest::is_empty() || new_user_id == 0 || chat_id == 0;
    }
};
}