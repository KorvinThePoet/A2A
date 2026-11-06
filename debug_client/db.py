from models import Route, Method


routes = [
    Route("Create user", "/users/create_user/", Method.POST, {"username": "string", "password": "string"}),
    Route("Get user", "/users/get_user/{username:str}", Method.GET, {"username": "string"}),
    Route("Get all users", "/users/get_all/", Method.GET, {}),
    Route("Get my personal info", "/users/get_personal_info/", Method.GET, {"user_id": "string", "token": "string"}),
    Route("Login maafaka", "/users/login/", Method.POST, {"username": "string", "password": "string"})
]
