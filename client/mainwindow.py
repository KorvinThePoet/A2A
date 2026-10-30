from PyQt6.QtWidgets import QMainWindow, QVBoxLayout, QWidget
from chatWidgets.chatle import ChatListElement

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("A2A")
        self.resize(500, 500)

        self.main_layout = QVBoxLayout()
        for i in range(1, 4):
            self.main_layout.addWidget(ChatListElement(i))
        container = QWidget()
        container.setLayout(self.main_layout)
        self.setCentralWidget(container)