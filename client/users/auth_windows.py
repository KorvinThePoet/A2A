import os
from PyQt6.QtWidgets import QWidget
from PyQt6 import uic


class PassWindow(QWidget):
    def __init__(self):
        super().__init__()
        ui_path = os.path.join(os.path.dirname(__file__), "ui_files/pas.ui")
        uic.loadUi(ui_path, self)

        self.mail_rename_btn.clicked.connect(self.mail_rename)

    def mail_rename(self):
        self.mail_win = MailWindow()
        self.mail_win.show()
        self.close()


class MailWindow(QWidget):
    def __init__(self):
        super().__init__()
        ui_path = os.path.join(os.path.dirname(__file__), "ui_files/mail.ui")
        uic.loadUi(ui_path, self)

        self.login_btn.clicked.connect(self.login)
        self.pass_btn.clicked.connect(self.mail_pass)

    def login(self):
        from login import LoginWindow
        self.login_win = LoginWindow()
        self.login_win.show()
        self.close()

    def mail_pass(self):
        self.pass_win = PassWindow()
        self.pass_win.show()
        self.close()


class RegisterWindow(QWidget):
    def __init__(self):
        super().__init__()
        ui_path = os.path.join(os.path.dirname(__file__), "ui_files/register.ui")
        uic.loadUi(ui_path, self)

        self.login_btn.clicked.connect(self.handle_login)

    def handle_login(self):
        from login import LoginWindow
        self.login_win = LoginWindow()
        self.login_win.show()
        self.close()
