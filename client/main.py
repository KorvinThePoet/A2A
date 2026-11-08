<<<<<<< HEAD
=======
from mainwindow import MainWindow
from PyQt6.QtWidgets import QApplication

import sys

def main():
    app = QApplication(sys.argv)
    window = MainWindow()

    window.show()

    sys.exit(app.exec())


if __name__ == "__main__":
    main()
>>>>>>> 74d8b68f672b625e61f2b0d268a8a794dd713d8d
