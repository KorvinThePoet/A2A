#include "router.hpp"
// мда, зачем мы выбрали это т проект, щас бы уже шахматы доделали черти

Requests::Response Chats::route(const Request& req, Storage& storage) {
    if (req.is_bad_request()) {
        return Requests::MessageResponse(404, "Bad request");
    }

    std::vector<std::string> divided_request = divide_in_two(req.get_route(), 1, '/');
    std::string prefix = divided_request[0];
    std::string postfix = divided_request[1];

    if (prefix != "chats") {
        return Requests::MessageResponse(404, "Wrong route method");
    }

    // бля может надо было сделать типа мап мапов типа
    // routes["POST"]["create_chat"] = create_chat и передавать боди всегда
    // а то я подумал, хотя зря наверное, ну ладно
    // да, фигню сказанул
     // о доп пробел, короче оставлю так, хотя нужна консультанция
    // бял🥀

    Chats::ChatController chat_ctrl(storage);
    if(starts_with(postfix, "create_chat") && req.get_method() == "POST") {
        return chat_ctrl.create_chat(req.get_body());
    }
    if(starts_with(postfix, "create_message") && req.get_method() == "POST") {
        return chat_ctrl.create_message(req.get_body());
    }
    if(starts_with(postfix, "add_user") && req.get_method() == "POST") {
        return chat_ctrl.add_user(req.get_body());
    }
    if(starts_with(postfix, "send_file") && req.get_method() == "POST") {
        return chat_ctrl.send_file(req.get_body());
    }
    if(starts_with(postfix, "change_user_role") && req.get_method() == "POST") {
        return chat_ctrl.change_user_role(req.get_body());
    }
    if(starts_with(postfix, "get_chat_from") && req.get_method() == "GET") {
        return chat_ctrl.get_chat_from(req.get_body());
    }
    if(starts_with(postfix, "get_chat") && req.get_method() == "GET") {
        return chat_ctrl.get_chat(req.get_body());
    }
    if(starts_with(postfix, "delete_chat") && req.get_method() == "DELETE") {
        return chat_ctrl.delete_chat(req.get_body());
    }
    if(starts_with(postfix, "delete_message") && req.get_method() == "DELETE") {
        return chat_ctrl.delete_message(req.get_body());
    }

    return Requests::MessageResponse(404, "Unknown route");
}