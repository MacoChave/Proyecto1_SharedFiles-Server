#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    tcpServer->setMaxPendingConnections(2);

    tcpCliente = new QTcpSocket(this);

    tcpServer->listen(QHostAddress::LocalHost, 1234);
    connect(
                tcpServer,
                SIGNAL (newConnection()),
                this,
                SLOT (nuevaConexion())
            );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nuevaConexion()
{
    tcpCliente = tcpServer->nextPendingConnection();
    connect(
                tcpCliente,
                SIGNAL (readyRead()),
                this,
                SLOT (leerSocketCliente())
            );
}

void MainWindow::leerSocketCliente()
{
    if (tcpCliente->bytesAvailable() > 0)
    {
        QByteArray buffer;
        buffer.resize(tcpCliente->bytesAvailable());
        tcpCliente->read(buffer.data(), buffer.size());
        ui->edtLog->append((QString) buffer);
        qDebug() << (QString) buffer;
    }
}

void MainWindow::on_btnMensaje_clicked()
{
    tcpCliente->write(
                ui->edtMensaje->text().toLatin1().data(),
                ui->edtMensaje->text().size()
            );
    ui->edtMensaje->clear();
}
