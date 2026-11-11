#pragma once

namespace Chats {
struct AuthenticatedUserRequest {
    std::string token;
    std::string user_id;
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
}