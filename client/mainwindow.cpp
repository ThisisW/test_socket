#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();

    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);

    ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("192.168.1.100");
    ui->lineEdit_Port->setText("8765");
}

MainWindow::~MainWindow()
{
    delete this->socket;
    delete ui;
}


void MainWindow::on_pushButton_Connect_clicked()
{
    if(ui->pushButton_Connect->text() == tr("Connect"))
    {
        QString IP;
        int port;
        IP = ui->lineEdit_IP->text();
        port = ui->lineEdit_Port->text().toInt();
        socket->abort();
        socket->connectToHost(IP, port);

        if(!socket->waitForConnected(30000))
        {
            qDebug() << "Connection failed!";
            return;
        }

        qDebug() << "Connect successfully!";
        ui->pushButton_Send->setEnabled(true);
        ui->pushButton_Connect->setText("Disconnect");
    }
    else
    {
        socket->disconnectFromHost();
        ui->pushButton_Connect->setText("Connect");
        ui->pushButton_Send->setEnabled(false);
    }
}

void MainWindow::on_pushButton_Send_clicked()
{
    qDebug() << "Send: " << ui->textEdit_Send->toPlainText();
    //ASCII
    socket->write(ui->textEdit_Send->toPlainText().toLatin1());
    socket->flush();
}

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;

    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        QString str = ui->textEdit_Recv->toPlainText();
        str += tr(buffer);
        ui->textEdit_Recv->setText(str);
    }
}

void MainWindow::socket_Disconnected()
{
    ui->pushButton_Send->setEnabled(false);
    ui->pushButton_Connect->setText("Connect");
    qDebug() << "Disconnected!";
}
