
class Message:
    def __init__(self):
        # Подумай какие поля должны быть в сообщении, чтобы было удобно 
        ...

class Chat:
    def __init__(self, chat_id: int, title: str, icon_path: str):
        self.chat_id = chat_id
        self.title = title
        self.icon_path = icon_path
        self.messages: list[Message]
        