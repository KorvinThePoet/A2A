import os
from PyQt6.QtWidgets import QWidget
from PyQt6 import uic
from main_window import MainWindow
from auth_windows import RegisterWindow, MailWindow


class LoginWindow(QWidget):
    def __init__(self):
        super().__init__()
        ui_path = os.path.join(os.path.dirname(__file__), "ui_files/login.ui")
        uic.loadUi(ui_path, self)

        self.login_btn.clicked.connect(self.handle_login)
        self.register_btn.clicked.connect(self.registr)
        self.mail_btn.clicked.connect(self.mail_pas)

    def handle_login(self):
        self.main_win = MainWindow()
        self.main_win.show()
        self.close()

    def registr(self):
        self.register_win = RegisterWindow()
        self.register_win.show()
        self.close()

    def mail_pas(self):
        self.mail_win = MailWindow()
        self.mail_win.show()
        self.close()
