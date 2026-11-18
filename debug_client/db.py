from models import Route, Method


routes = [
    Route("Create user", "/users/create_user/", Method.POST, {"username": "string", "password": "string", "email": "string"}),
    Route("Get user", "/users/get_user/{username:str}", Method.GET, {"username": "string"}),
    Route("Get all users", "/users/get_all/", Method.GET, {}),
    Route("Get my personal info", "/users/get_personal_info/", Method.GET, {"user_id": "string", "token": "string"}),
    Route("Login maafaka", "/users/login/", Method.POST, {"username": "string", "password": "string"}),
    
    Route("Create chat", "/chats/create_chat/", Method.POST, {"user_id": "string", "token": "string", "title": "string", "is_group": "string"}),
    Route("Create message", "/chats/create_message/", Method.POST, {"user_id": "string", "token": "string", "chat_id": "string", "text": "string"}),
    Route("Add user to chat", "/chats/add_user/", Method.POST, {"user_id": "string", "token": "string", "new_user_id": "string", "chat_id": "string"}),
    Route("Change user role", "/chats/change_user_role/", Method.POST, {"user_id": "string", "token": "string", "role": "string", "target_user_id": "string", "chat_id": "string"}),
    Route("Get chat from", "/chats/get_chat_from/", Method.GET, {"user_id": "string", "token": "string", "chat_id": "string", "time": "string"}),
    Route("Get chat", "/chats/get_chat/", Method.GET, {"user_id": "string", "token": "string", "chat_id": "string"}),
    Route("Get chat", "/chats/get_users_chat/", Method.GET, {"user_id": "string", "token": "string", "chat_id": "string"}),
    Route("Delete Chat", "/chats/delete_chat/", Method.DELETE, {"user_id": "string", "token": "string", "chat_id": "string"}),
    Route("Delete message", "/chats/delete_message/", Method.DELETE, {"user_id": "string", "token": "string", "chat_id": "string", "message_id": "string"}),
]
