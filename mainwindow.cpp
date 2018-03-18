#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    matrix = new Matrix();

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
        qDebug() << (QString) buffer;
        interpretarMensaje((QString) buffer);
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

void MainWindow::interpretarMensaje(QString mensaje)
{
    QString log;
    QStringList lstMsg = mensaje.split("^");
    qDebug() << lstMsg;
    QTextStream out(&log);

    if (mensaje.startsWith("LOGIN"))
    {
        out << "***** LOGIN *****" << "\n* NICKNAME\n\t";
        out << lstMsg[1] << "\n* PASSWORD\n\t";
        out<< lstMsg[2] << "\n ******************** \n";
        flush(out);
    }
    else if (mensaje.startsWith("LOGUP"))
    {
        out << "***** LOGUP *****" << "\n* NICKNAME\n\t";
        out << lstMsg[1] << "\n* PASSWORD\n\t";
        out << lstMsg[2] << "\n* NOMBRE\n\t";
        out << lstMsg[3] << "\n* CORREO\n\t";
        out<< lstMsg[4] << "\n ******************** \n";
        flush(out);
    }
    else if (mensaje.startsWith("LOGOUT"))
    {
    }
    ui->edtLog->append(log);
}
