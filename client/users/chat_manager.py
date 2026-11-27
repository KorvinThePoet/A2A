from typing import Dict, List, Optional
from mymodels import Chat, Message

class ChatManager:
    def __init__(self, current_user_id: int):
        self.current_user_id = current_user_id
        self.chats: List[Chat] = []
        self._cache: Dict[int, Chat] = {}

    def load_chat_from_json(self, json_data: dict) -> Chat:
        chat = Chat.from_dict(json_data)
        
        if chat.chat_id in self._cache:
            existing_chat = self._cache[chat.chat_id]
            existing_chat.title = chat.title
            existing_chat.users = chat.users
            existing_chat.messages = chat.messages
            return existing_chat
        else:
            self.chats.append(chat)
            self._cache[chat.chat_id] = chat
            return chat

    def get_chat(self, chat_id: int) -> Optional[Chat]:
        return self._cache.get(chat_id)

    def get_messages(self, chat_id: int) -> List[Message]:
        chat = self._cache.get(chat_id)
        return chat.messages if chat else []

    def add_message(self, chat_id: int, message: Message):
        chat = self._cache.get(chat_id)
        if chat:
            chat.messages.append(message)
