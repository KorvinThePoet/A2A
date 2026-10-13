from models import Route, Method


routes = [
    Route("Say hi to manica gucci", "/hello/manica/", Method.POST, {"message": "string"}),
    Route("Send a message", "/send/", Method.POST, {
            "chat_id": "integer", 
            "message": "string", 
            "user_auth_token": "string"
        }),
    Route("get some money", "/get_money/", Method.GET, None),
]