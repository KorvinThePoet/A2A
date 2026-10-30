from models import Route, Method


routes = [
    Route("Create user", "/users/create_user/", Method.POST, {"username": "string", "password": "string"})
]