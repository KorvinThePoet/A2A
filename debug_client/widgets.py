from PyQt6.QtWidgets import QSpinBox, QLineEdit, QHBoxLayout, QStackedWidget, QScrollArea
from PyQt6.QtCore import pyqtSignal, Qt
from PyQt6.QtWidgets import QWidget, QMainWindow, QPushButton, QVBoxLayout, QLabel
from PyQt6.QtNetwork import QTcpSocket, QAbstractSocket

from db import routes


class TestRoute(QWidget):
    clicked = pyqtSignal(int)

    def __init__(self, route_id: int):
        super().__init__()
        if route_id >= len(routes) or route_id < 0:
            raise ValueError()
        self.route_id = route_id
        self.main_button = QPushButton(routes[self.route_id].name)
        self.main_button.resize(300, 100)
        self.main_button.clicked.connect(self.on_click)

        layout = QVBoxLayout()
        layout.addWidget(self.main_button)
        layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(layout)


    def on_click(self):
        self.clicked.emit(self.route_id)


class RouteInfo(QWidget):
    send_request = pyqtSignal(dict)

    def __init__(self, route_id: int):
        super().__init__()
        self.route = routes[route_id]

        self.inputs: dict = {}

        layout = QVBoxLayout()
        self.title = QLabel()
        self.title.setText(f"{self.route.name}: {self.route.path}\nMethod: {self.route.method}")
        layout.addWidget(self.title)

        if self.route.fields is not None:
            for field_name, type in self.route.fields.items():
                input_widget = QLineEdit()

                if type == "integer":
                    input_widget = QSpinBox()
                input_widget.resize(200, 100)
                
                line = QHBoxLayout()
                line.addWidget(QLabel(f"{field_name}:"))
                line.addWidget(input_widget)
                layout.addLayout(line)
                self.inputs[field_name] = input_widget

        self.send_btn = QPushButton("Send request")
        self.send_btn.clicked.connect(self.send)
        layout.addWidget(self.send_btn)
        self.setLayout(layout)

    def send(self):
        payload: dict = {}
        payload["route"] = self.route
        for field_name, widget in self.inputs.items():
            if isinstance(widget, QSpinBox):
                payload[field_name] = widget.value()
            if isinstance(widget, QLineEdit):
                payload[field_name] = widget.text()
        self.send_request.emit(payload)

class ServerResponse(QWidget):
    def __init__(self):
        super().__init__()
        self.response = QLabel()
        self.response.setText("No response yet")
        self.response.setWordWrap(True)

        layout = QHBoxLayout()
        layout.addWidget(self.response)
        self.setLayout(layout)

    

class MainWindow(QMainWindow):
    def __init__(self, soket: QTcpSocket):
        super().__init__()
        self.setWindowTitle("Debug client")
        self.resize(500, 500)
        self.routes: list[TestRoute] = []
        self.main_layout = QHBoxLayout()
        self.socket = soket
        self.stacked_widget = QStackedWidget()
        route_list = QVBoxLayout()

        for i in range(len(routes)):
            self.routes.append(TestRoute(i))
            self.routes[i].clicked.connect(self.handle_route)
            route_list.addWidget(self.routes[i])
            ri = RouteInfo(i)
            ri.send_request.connect(self.handle_send)
            self.stacked_widget.addWidget(ri)

        self.stacked_widget.setCurrentIndex(0)

        scroll_area = QScrollArea()
        scroll_area.setWidgetResizable(True)
        scroll_area.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)
        scroll_area.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)

        route_list_widget = QWidget()
        route_list_widget.setLayout(route_list)
        scroll_area.setWidget(route_list_widget)

        self.main_layout.addWidget(scroll_area)
        hlayout = QVBoxLayout()
        hlayout.addWidget(self.stacked_widget)
        self.response_widget = ServerResponse()
        hlayout.addWidget(self.response_widget)
        self.main_layout.addLayout(hlayout)
        container = QWidget()
        container.setLayout(self.main_layout)

        self.setCentralWidget(container) 


    def handle_route(self, route_id):
        self.stacked_widget.setCurrentIndex(route_id)

    def handle_send(self, payload: dict):
        if self.socket.isOpen():
            print("Sending msg")
            self.socket.write(str(payload).encode())
            print("MSG sent")

    def set_response(self, response: str):
        self.response_widget.response.setText(response)