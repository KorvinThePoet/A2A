from PyQt6.QtWidgets import QWidget, QPushButton, QVBoxLayout
from PyQt6.QtCore import QSize
from PyQt6.QtGui import QIcon

db = {
    1: {"username": "first user", "icon_path": None},
    2: {"username": "second user", "icon_path": None},
}

class ChatListElement(QWidget):
    def __init__(self, chat_id: int):
        default_icon = QIcon("static/default_icon.jpg")
        
        super().__init__()
        if chat_id not in db.keys():
            self.plate = QPushButton("Deleted user")
            self.plate.setIcon(default_icon)
            self.plate.setIconSize(QSize(24, 24))
            
        else:
            self.plate = QPushButton(db[chat_id]["username"])
            if db[chat_id]["icon_path"]:
                self.plate.setIcon(QIcon(db[chat_id]["icon_path"]))
            else:
                self.plate.setIcon(QIcon("static/default_icon.jpg"))
        layout = QVBoxLayout()
        layout.addWidget(self.plate)
        self.setLayout(layout)
