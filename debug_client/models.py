from enum import Enum

class Method(Enum):
    POST = "POST"
    GET = "GET"
    PUT = "PUT"
    DELETE = "DELETE"

class Route:
    def __init__(self, name: str, path: str, method: Method, fields: None | dict):
        self.name = name
        self.path = path
        self.method = method
        self.fields = fields

    def __repr__(self):
        return f"Route({self.name}, {self.path}, {self.method})"
