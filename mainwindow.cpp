#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->setStyleSheet(R"(
        QWidget {
            background-color: #D0E8FF;
            color: #003366;
            font-size: 14px;
        }

        QPushButton {
            background-color: #64B5F6;
            color: #FFFFFF;
            border-radius: 4px;
            padding: 6px 14px;
            border: 1px solid #42A5F5;
        }

        QPushButton:hover {
            background-color: #42A5F5;
            border: 1px solid #1E88E5;
        }

        QPushButton:pressed {
            background-color: #1E88E5;
            border: 1px solid #1565C0;
        }

        QSlider::groove:horizontal {
            border: 1px solid #42A5F5;
            height: 6px;
            border-radius: 3px;
            background: #90CAF9;
        }

        QSlider::handle:horizontal {
            background: #1E88E5;
            border: 1px solid #1565C0;
            width: 14px;
            height: 14px;
            margin: -4px 0;
            border-radius: 7px;
        }

        QLabel, QCheckBox {
            color: #003366;
        }

        QCheckBox::indicator {
            width: 16px;
            height: 16px;
        }

        QCheckBox::indicator:unchecked {
            border: 1px solid #64B5F6;
            background-color: #90CAF9;
        }

        QCheckBox::indicator:checked {
            border: 1px solid #1E88E5;
            background-color: #42A5F5;
        }

        QGroupBox {
            border: 1px solid #64B5F6;
            border-radius: 5px;
            margin-top: 10px;
            color: #003366;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 2px 5px;
            color: #003366;
        }

        QMessageBox {
            background-color: #D0E8FF;
            color: #003366;
        }

        QLineEdit, QTextEdit, QComboBox {
            border: 1px solid #42A5F5;
            border-radius: 4px;
            padding: 4px;
            color: #003366;
            background-color: #FFFFFF;
            selection-background-color: #1E88E5;
            selection-color: #FFFFFF;
        }

        QComboBox QAbstractItemView {
            selection-background-color: #1E88E5;
            color: #FFFFFF;
        }

        QTabWidget::pane {
            border: 1px solid #64B5F6;
            background: #D0E8FF;
            border-radius: 5px;
        }

        QTabBar::tab {
            background: #90CAF9;
            color: #003366;
            border: 1px solid #64B5F6;
            padding: 6px 12px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }

        QTabBar::tab:hover {
            background: #64B5F6;
            border: 1px solid #42A5F5;
        }

        QTabBar::tab:selected {
            background: #42A5F5;
            border: 1px solid #1E88E5;
            color: #FFFFFF;
        }

        QTableWidget {
            border: 1px solid #64B5F6;
            border-radius: 4px;
            gridline-color: #90CAF9;
            background-color: #D0E8FF;
            color: #003366;
            selection-background-color: #42A5F5;
            selection-color: #FFFFFF;
        }

        QTableWidget::item {
            border: none;
            padding: 4px;
        }

        QTableWidget::item:selected {
            background-color: #42A5F5;
            color: #FFFFFF;
        }

        QHeaderView::section {
            background-color: #90CAF9;
            padding: 4px;
            border: 1px solid #64B5F6;
            color: #003366;
        }

        QHeaderView::section:horizontal {
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }

        QHeaderView::section:vertical {
            border-left: none;
            border-right: none;
        }
    )");

    // Connect UI actions
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectButtonClicked);  // ← 新增连接

    // Connect WebSocket signals
    connect(&m_webSocket, &QWebSocket::connected, this, &MainWindow::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &MainWindow::onTextMessageReceived);
}

MainWindow::~MainWindow() {
    m_webSocket.close();
    delete ui;
}

void MainWindow::onConnectButtonClicked() {
    QString address = ui->addressLineEdit->text();
    if (!address.isEmpty()) {
        m_webSocket.open(QUrl(address));
        ui->textBrowser->append(">= Connecting to: " + address);
    } else {
        ui->textBrowser->append(">= Please enter a WebSocket address.");
    }
}

void MainWindow::onDisconnectButtonClicked() {
    if (m_webSocket.state() == QAbstractSocket::ConnectedState) {
        m_webSocket.close();
        ui->textBrowser->append(">= Manually disconnected.");
    } else {
        ui->textBrowser->append(">= Not connected.");
    }
}

void MainWindow::onConnected() {
    ui->textBrowser->append(">= Connected to WebSocket server.");
}

void MainWindow::onDisconnected() {
    ui->textBrowser->append(">= Disconnected from WebSocket server.");
}

void MainWindow::onTextMessageReceived(QString message) {
    ui->textBrowser->append("<font color='blue'>>= Received: <br>" + message + "</font>");
}

void MainWindow::onSendButtonClicked() {
    QString message = ui->lineEdit->text();
    if (!message.isEmpty()) {
        m_webSocket.sendTextMessage(message);
        ui->textBrowser->append("<font color='purple'>>= Sent: <br>" + message + "</font>");
        ui->lineEdit->clear();
    }
}
