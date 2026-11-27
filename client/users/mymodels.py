from dataclasses import dataclass, field
from typing import List, Optional

@dataclass
class User:
    id: int
    username: str

    @classmethod
    def from_dict(cls, data: dict) -> "User":
        return cls(id = data["id"], username = data["username"])


@dataclass
class Message:
    id: int
    sender_id: int
    receiver_id: int
    content: str
    timestamp: Optional[str] = None
    is_read: bool = False

    @classmethod
    def from_dict(cls, data: dict) -> "Message":
        return cls(id = data["id"],
                   sender_id = data["sender_id"],
                   receiver_id = data["receiver_id"],
                   content = data["content"],
                   timestamp = data.get("timestamp"),
                   is_read = data.get("is_read", False)
                   )


@dataclass
class Chat:
    chat_id: int
    title: str
    messages: List[Message] = field(default_factory=list)
    users: List[User] = field(default_factory=list)
    unread_count: int = 0

    @property
    def last_message(self) -> Optional[Message]:
        return self.messages[-1] if self.messages else None

    @classmethod
    def from_dict(cls, data: dict) -> "Chat":
        return cls(
            chat_id = data["chat_id"],
            title = data["title"],
            messages = [Message.from_dict(m) for m in data.get("messages", [])],
            users = [User.from_dict(u) for u in data.get("users", [])]
        )
