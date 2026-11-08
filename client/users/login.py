import sys
import os
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget
from PyQt6 import uic

class PassWindow(QWidget):
    def __init__(self):
        super().__init__()
        ui_path = os.path.join(os.path.dirname(__file__), "pas.ui")
        uic.loadUi(ui_path, self)

        self.mail_rename_btn.clicked.connect(self.mail_rename)

    def mail_rename(self):
        self.mail_win = MailWindow()
        self.mail_win.show()
        self.close()

class MailWindow(QWidget):
    def __init__(self):
        super().__init__()
        ui_path = os.path.join(os.path.dirname(__file__), "mail.ui")
        uic.loadUi(ui_path, self)

        self.login_btn.clicked.connect(self.login)
        self.pass_btn.clicked.connect(self.mail_pass)

    def login(self):
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
        ui_path = os.path.join(os.path.dirname(__file__), "register.ui")
        uic.loadUi(ui_path, self)

        self.login_btn.clicked.connect(self.handle_login)

    def handle_login(self):
        self.login_win = LoginWindow()
        self.login_win.show()
        self.close()

class LoginWindow(QWidget):
    def __init__(self):
        super().__init__()
        
        # Загружаем ваш интерфейс из .ui файла
        # Убедитесь, что этот Python-файл и ваш .ui файл лежат в одной папке!
        ui_path = os.path.join(os.path.dirname(__file__), "login.ui") # Укажите точное имя вашего .ui файла вместо untitled.ui
        uic.loadUi(ui_path, self)
        
        # СВЯЗЫВАЕМ КНОПКУ С КОДОМ:
        # self.имя_кнопки_из_дизайнера.clicked.connect(self.функция_которая_сработает)
        self.login_btn.clicked.connect(self.handle_login) 
        self.register_btn.clicked.connect(self.registr)
        self.mail_btn.clicked.connect(self.mail_pas)

    def handle_login(self):
        print("Кнопка 'Войти' была успешно нажата!")
        
        # Пример: как получить текст, который пользователь ввел в поля "Логин" и "Пароль"
        # Для этого посмотрите objectName ваших полей QLineEdit (например, lineEdit_login и lineEdit_password)
        # login_text = self.lineEdit_login.text()
        # password_text = self.lineEdit_password.text()
        # print(f"Введенный логин: {login_text}, Пароль: {password_text}")
    
    def registr(self):
        self.register_win = RegisterWindow()
        self.register_win.show()
        self.close()

    def mail_pas(self):
        self.mail_win = MailWindow()
        self.mail_win.show()
        self.close()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = LoginWindow()
    window.show()
    sys.exit(app.exec())
