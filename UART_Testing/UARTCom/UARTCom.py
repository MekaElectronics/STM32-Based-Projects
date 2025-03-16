import sys
import serial
import serial.tools.list_ports
from PyQt5.QtWidgets import *
from PyQt5.QtCore import QTimer


class UARTTool(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("UARTCom")

        # Serial port instance
        self.ser = None

        # UI elements
        self.BaudRate = QComboBox()
        self.InputPort = QComboBox()
        self.TXMsg = QLineEdit()
        self.TxButton = QPushButton("Send")
        self.RXWindow = QTextEdit()
        self.ConnectionButton = QPushButton("Connect")

        self.timer = QTimer()
        self.timer.timeout.connect(self.MsgRecv)

        self.InitUI()

    def InitUI(self):
        # === LEFT SIDE LAYOUT ===
        LeftLayout = QVBoxLayout()

        LeftLayout.addWidget(QLabel("Port:"))
        self.InputPort.addItems(self.DetectPorts())
        LeftLayout.addWidget(self.InputPort)
        LeftSpacer = QSpacerItem(50,100)
        LeftLayout.addItem(LeftSpacer)

        LeftLayout.addWidget(QLabel("Baudrate:"))
        self.BaudRate.addItems(["9600", "115200"])
        self.BaudRate.setCurrentText("115200")
        LeftLayout.addWidget(self.BaudRate)
        LeftLayout.addWidget(self.ConnectionButton)
        
        # === RIGHT SIDE LAYOUT ===
        RightLayout = QVBoxLayout()

        RxLabel = QLabel("Received Messages:")
        RightLayout.addWidget(RxLabel)

        self.RXWindow.setReadOnly(True)
        RightLayout.addWidget(self.RXWindow)
        RightLayout.addWidget(QLabel("Message to Send:"))
        RightLayout.addWidget(self.TXMsg)

        RightLayout.addWidget(self.TxButton)

        # === MAIN LAYOUT ===
        MainLayout = QHBoxLayout()
        MainLayout.addLayout(LeftLayout, 1)   # 1 part width
        MainLayout.addLayout(RightLayout, 4)  # 2 parts width

        self.setLayout(MainLayout)

        # Connect signals
        self.TxButton.clicked.connect(self.MsgSend)
        self.ConnectionButton.clicked.connect(self.SetConnection)

    def DetectPorts(self):
        Ports = serial.tools.list_ports.comports()
        for Port in Ports:
            if "STM" in Port.description or "STLink" in Port.description:
                return [Port.device]
        return ["None"]

    def SetConnection(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            self.timer.stop()
            self.ConnectionButton.setText("Connect")
            self.RXWindow.append("Disconnected.\n")
        else:
            try:
                port = self.InputPort.currentText()
                baud = int(self.BaudRate.currentText())
                self.ser = serial.Serial(port, baud, timeout=0.1)
                self.timer.start(100)
                self.ConnectionButton.setText("Disconnect")
                self.RXWindow.append(f"Connected to {port} at {baud} baud.\n")
            except Exception as e:
                self.RXWindow.append(f"Connection error: {e}\n")

    def MsgSend(self):
        if self.ser and self.ser.is_open:
            msg = self.TXMsg.text()
            if not msg.endswith("\n"):
                msg += "\n"
            self.ser.write(msg.encode())

    def MsgRecv(self):
        if self.ser and self.ser.is_open and self.ser.in_waiting:
            try:
                line = self.ser.readline().decode(errors='replace').strip()
                if line:
                    self.RXWindow.append(f"STM32 >> {line}")
            except Exception as e:
                self.RXWindow.append(f"Read error: {e}\n")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    with open("Style.qss", "r") as f:
        app.setStyleSheet(f.read())
    window = UARTTool()
    window.resize(600, 300)
    window.show()
    sys.exit(app.exec_())
