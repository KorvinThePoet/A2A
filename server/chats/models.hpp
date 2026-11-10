#pragma once

struct AuthenticatedUserRequest {
    std::string token;
    std::string user_id;
};

struct FileResponse : File {
    std::string to_json() {
        // TODO
    }
};

struct MessageResponse : Message{
    std::vector<File> attached_files;

    MessageResponse(const Message& msg, const std::vector<File>& attached_files) :
            Message(msg), attached_files(attached_files) {}
    
    std::string to_json() {
        // TODO
    }
};

struct ChatResponse {
    std::vector<Message> messages;

    ChatResponse(const std::vector<Message> &messages) : messages(messages) {}
    std::string to_json() {
        // TODO 
    }
};
