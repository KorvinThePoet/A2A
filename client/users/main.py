import sys
import os
from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget
from PyQt6 import uic
from PyQt6.QtCore import Qt

# // все в одном файле прям как мы любим
# можно было бы раскидать все кроме MainWindow в файл типа widgets.py
# че, а че MainWindow наследуется от QWidget а не от QMainWindow хмммм ....
# понятно нафик
# а че картинки в этой же директории, создай че то типа static/app/pics ну или че то другое но не 
# на одном уровне с main.py
# балямба, еще можно было бы *.ui файлы тоже выкинут в отдельный папка, а то странно выгледет
# ну это я делать не будет, аслан, сделай пж ну ил артур 

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        ui_path = os.path.join(os.path.dirname(__file__), "main.ui")
        uic.loadUi(ui_path, self)

        # 1. Создаем подложку-блокировщик на всё главное окно
        self.overlay = QWidget(self)
        self.overlay.setStyleSheet("background-color: rgba(0, 0, 0, 120);") # Полупрозрачный черный фон
        self.overlay.hide()

        # 2. Загружаем ChatOrGroup.ui ВНУТРЬ подложки
        popup_path = os.path.join(os.path.dirname(__file__), "ChatOrGroup.ui")
        self.popup = QWidget(self.overlay)
        uic.loadUi(popup_path, self.popup)
        
        self.popup.setAttribute(Qt.WidgetAttribute.WA_StyledBackground, True)

        create_chat_path = os.path.join(os.path.dirname(__file__), "CreateChat.ui")
        self.create_chat_popup = QWidget(self.overlay)
        uic.loadUi(create_chat_path, self.create_chat_popup)
        self.create_chat_popup.setAttribute(Qt.WidgetAttribute.WA_StyledBackground, True)

        create_group_path = os.path.join(os.path.dirname(__file__), "CreateGroup.ui")
        self.create_group_popup = QWidget(self.overlay)
        uic.loadUi(create_group_path, self.create_group_popup)
        self.create_group_popup.setAttribute(Qt.WidgetAttribute.WA_StyledBackground, True)

        

        # Привязываем кнопки
        self.createChat.clicked.connect(self.open_chat_or_group)
        self.popup.close_btn.clicked.connect(self.close_all_popups)
        self.popup.newChat_btn.clicked.connect(self.open_create_chat)
        self.popup.newGroup_btn.clicked.connect(self.open_create_group)
        self.create_group_popup.close_btn.clicked.connect(self.close_all_popups)
        # Замените close_btn на реальный objectName кнопки с крестиком из CreateChat.ui
        self.create_chat_popup.close_btn.clicked.connect(self.close_all_popups)
        self.create_chat_popup.create_btn.clicked.connect(self.add_new_chat)
        self.run.clicked.connect(self.sendMessage)

    def sendMessage(self):
        if self.username_label.text().strip() == "":
            self.message.clear()
            return
        msg = self.message.text().strip()
        if not msg:
            return

        scroll_content = self.chat_now.widget()
        scroll_layout = scroll_content.layout()

        chat_now_item = QWidget()
        item_path = os.path.join(os.path.dirname(__file__), "outMessage.ui")
        uic.loadUi(item_path, chat_now_item)

        chat_now_item.message_label.setText(msg)
        if scroll_layout is not None:
            if scroll_layout.count() > 0:
                scroll_layout.insertWidget(scroll_layout.count() - 1, chat_now_item)
            else:
                scroll_layout.addWidget(chat_now_item)

        self.message.clear()


    def add_new_chat(self):
        chat_name = self.create_chat_popup.search_line.text().strip()
        if not chat_name:
            chat_name = "Новый чат"

        scroll_content = self.chats.widget()
        scroll_layout = scroll_content.layout()

        chat_item = QWidget()
        item_path = os.path.join(os.path.dirname(__file__), "ChatItem.ui")
        uic.loadUi(item_path, chat_item)

        chat_item.name_label.setText(chat_name)
        chat_item.msg_label.setText("")
        chat_item.setCursor(Qt.CursorShape.PointingHandCursor)
        chat_item.mousePressEvent = lambda event, name=chat_name: self.open_chat(name)

        if scroll_layout is not None:
            if scroll_layout.count() > 0:
                scroll_layout.insertWidget(scroll_layout.count() - 1, chat_item)
            else:
                scroll_layout.addWidget(chat_item)

        self.create_chat_popup.search_line.clear()
        self.close_all_popups()

    def open_chat(self, name):
        self.username_label.setText(name)



    def show_centered_popup(self, target_widget):
        self.popup.hide()
        self.create_chat_popup.hide()
        self.create_group_popup.hide()

        self.overlay.setGeometry(0, 0, self.width(), self.height())
        target_widget.adjustSize()
        
        x = (self.overlay.width() - target_widget.width()) // 2
        y = (self.overlay.height() - target_widget.height()) // 2
        target_widget.move(x, y)

        self.overlay.show()
        target_widget.show()
        self.overlay.raise_()
        target_widget.raise_()

    def open_chat_or_group(self):
        self.show_centered_popup(self.popup)

    def open_create_chat(self):
        # Первое окно автоматически скроется, а второе встанет на его место
        self.show_centered_popup(self.create_chat_popup)

    def open_create_group(self):
        self.show_centered_popup(self.create_group_popup)

    def close_all_popups(self):
        self.popup.hide()
        self.create_chat_popup.hide()
        self.create_group_popup.hide()
        self.overlay.hide()

    def resizeEvent(self, event):
        super().resizeEvent(event)
        if hasattr(self, 'overlay') and self.overlay.isVisible():
            self.overlay.setGeometry(0, 0, self.width(), self.height())
            for widget in (self.popup, self.create_chat_popup, self.create_group_popup):
                if widget.isVisible():
                    x = (self.overlay.width() - widget.width()) // 2
                    y = (self.overlay.height() - widget.height()) // 2
                    widget.move(x, y)

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
        

        ui_path = os.path.join(os.path.dirname(__file__), "login.ui")
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

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = LoginWindow()
    window.show()
    sys.exit(app.exec())
