from models import Route, Method


routes = [
    Route("Create user", "/users/create_user/", Method.POST, {"username": "string", "password": "string"}),
    Route("Get user", "/users/get_user/{id:int}", Method.GET, {"id": "integer"}),
    Route("Get all users", "/users/get_all/", Method.GET, {}),
]
