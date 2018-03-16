#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    tcpServer->setMaxPendingConnections(2);

    for (int i = 0; i < tcpServer->maxPendingConnections(); i++)
        tcpCliente[i] = new QTcpSocket(this);

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
    static int j = 0;
    tcpCliente[j] = tcpServer->nextPendingConnection();
    connect(
                tcpCliente[j],
                SIGNAL (readyRead()),
                this,
                SLOT (leerSocketCliente())
            );
    j++;
}

void MainWindow::leerSocketCliente()
{
    if (tcpCliente[0]->bytesAvailable() > 0)
    {
        QByteArray buffer;
        buffer.resize(tcpCliente[0]->bytesAvailable());
        tcpCliente[0]->read(buffer.data(), buffer.size());
        ui->edtLog->append((QString) buffer);
        qDebug() << (QString) buffer;
    }
    else if (tcpCliente[1]->bytesAvailable() > 0)
    {
        QByteArray buffer;
        buffer.resize(tcpCliente[1]->bytesAvailable());
        tcpCliente[1]->read(buffer.data(), buffer.size());
        qDebug() << (QString) buffer;
    }
    else
        qDebug() << "No se puede conectarse con el servidor";
}

void MainWindow::on_btnMensaje_clicked()
{
    tcpCliente[0]->write(
                ui->edtMensaje->text().toLatin1().data(),
                ui->edtMensaje->text().size()
            );
}
