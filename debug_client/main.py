import sys
from PyQt6.QtWidgets import QApplication
from PyQt6.QtNetwork import QTcpSocket, QAbstractSocket
from widgets import MainWindow



def readyread(socket: QTcpSocket, window: MainWindow):
    data = socket.readAll().data().decode("utf-8")
    window.set_response(data)
    window.show()


def main():
    app = QApplication(sys.argv)
    socket = QTcpSocket()
    window = MainWindow(socket)

    socket.readyRead.connect(lambda: readyread(socket, window))
    socket.errorOccurred.connect(lambda: print(socket.errorString()))

    
    window.show()


    socket.connectToHost("127.0.0.1", 8080)
    sys.exit(app.exec())

if __name__ == '__main__':
    main()
