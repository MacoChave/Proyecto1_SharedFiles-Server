#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    user = NULL;
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
    delete matrix;
}

void MainWindow::nuevaConexion()
{
    tcpCliente = tcpServer->nextPendingConnection();
    connect(
                tcpCliente,
                SIGNAL (readyRead()),
                this,
                SLOT (consumer())
            );
}

void MainWindow::consumer()
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

void MainWindow::producer(QString value)
{
    tcpCliente->write(
                value.toLatin1().data(),
                value.size()
            );
}

void MainWindow::on_btnMensaje_clicked()
{
    graficar();
//    producer(ui->edtMensaje->text());
//    ui->edtMensaje->clear();

//    QString filename;
    /* ARCHIVO USUARIOS */
//    filename = QFileDialog::getOpenFileName(
//                this,
//                "Selector de archivos",
//                "/home/marco/Escritorio",
//                "Archivo JSON(*.json)");
//    QFile file(filename);
//    if (!file.open(QFile::ReadOnly))
//        return;

//    jsd = QJsonDocument::fromJson(file.readAll());
//    file.close();
//    if (!cargarUsuario())
//        return;

    /* ARCHIVO ARCHIVOS */
//    filename = QFileDialog::getOpenFileName(
//                this,
//                "Selector de archivos",
//                "/home/marco/Escritorio",
//                "Archivos JSON(*.json)");
//    file.setFileName(filename);
//    if (!file.open(QFile::ReadOnly))
//        return;

//    jsd = QJsonDocument::fromJson(file.readAll());
//    file.close();
//    if (!cargarArchivo())
//        return;

//    graficar();
}

bool MainWindow::cargarUsuario()
{
    if (jsd.isEmpty())
        return false;

    QJsonArray jsa = jsd.array();

    for (int i = 0; i < jsa.count(); i++)
    {
        QJsonObject jso = jsa.at(i).toObject();
        QString nombre = jso["nombre"].toString();
        QString correo = jso["correo"].toString();
        QString nick = jso["nickname"].toString();
        QString pass = jso["clave"].toString();

        TADRow *row = new TADRow(nombre, correo, nick, pass);
        matrix->getHeaderRow()->insert(row);
    }
    return true;
}

bool MainWindow::cargarArchivo()
{
    if (jsd.isEmpty())
        return false;

    QJsonArray jsa = jsd.array();

    for (int i = 0; i < jsa.count(); i++)
    {
        TADRow *row = NULL, *rowTemp = NULL;
        TADMatrixNode *duenio = NULL;
        MatrixNode *nodo = NULL;

        QJsonObject jso = jsa.at(i).toObject();

        QString nombre = jso["nombre"].toString();
        QString tipo = jso["tipo"].toString();
        QString fechaCreacion = jso["fecha_creacion"].toString();
        QString nickCreacion = jso["creador"].toString();
        QString fechaUltimoCambio = jso["fecha_modificacion"].toString();
        QString nickUltimoCambio = jso["modificador"].toString();
        QString filepath = jso["ruta_contenido"].toString();
        QJsonArray jsaPermiso = jso["permisos"].toArray();

        TADColumn *column = new TADColumn(
                        nombre, -1, fechaCreacion, nickCreacion,
                        fechaUltimoCambio, nickUltimoCambio,
                        filepath
                    );
        if (tipo.compare("documento") == 0)
            column->setTipo(column->DOCUMENTO);
        else if (tipo.compare("presentacion") == 0)
            column->setTipo(column->PRESENTACION);
        else if (tipo.compare("lienzo") == 0)
            column->setTipo(column->LIENZO);

        matrix->getHeaderColumn()->insert(column);

        rowTemp = new TADRow();
        rowTemp->setNickname(nickCreacion);
        row = matrix->getHeaderRow()->get(rowTemp)->getData();

        delete rowTemp;
        rowTemp = NULL;

        duenio = new TADMatrixNode(nickCreacion, nombre, -1);
        duenio->setPermiso(duenio->DUENIO);

        nodo = new MatrixNode(duenio);
        column->addInternalColumn(nodo);
        row->addInternalRow(nodo);

        for (int j = 0; j < jsaPermiso.count(); j++)
        {
            QJsonObject jsoPermiso = jsaPermiso.at(j).toObject();

            QString usuario = jsoPermiso["usuario"].toString();
            QString permiso = jsoPermiso["permiso"].toString();

            TADMatrixNode *tadMatrixNode = new TADMatrixNode(usuario, nombre, -1);
            if (permiso.compare("editar") == 0)
                tadMatrixNode->setPermiso(tadMatrixNode->EDITAR);
            else if (permiso.compare("ver") == 0)
                tadMatrixNode->setPermiso(tadMatrixNode->VER);

            rowTemp = new TADRow();
            rowTemp->setNickname(usuario);
            row = matrix->getHeaderRow()->get(rowTemp)->getData();

            delete rowTemp;
            rowTemp = NULL;

            nodo = new MatrixNode(tadMatrixNode);
            column->addInternalColumn(nodo);
            row->addInternalRow(nodo);
        }
    }
    return true;
}

void MainWindow::graficar()
{
    matrix->graph("Matriz");
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
        out << lstMsg[2] << "\n ******************** \n";
        flush(out);

        TADRow *rowTemp = new TADRow();
        rowTemp->setNickname(lstMsg[1]);
        rowTemp->setPassword(lstMsg[2]);

        Node<TADRow *> *row = matrix->getHeaderRow()->get(rowTemp);
        if (row != NULL)
        {
            if (row->getData()->comparePass(rowTemp) == 0)
            {
                user = row->getData();
                producer("LOGIN^CORRECTO");
                out << "CORRECTO" << "\n ******************** \n";
            }
            else
            {
                producer("LOGIN^INCORRECTO");
                out << "INCORRECTO" << "\n ******************** \n";
            }
        }
        else
        {
            producer("LOGIN^INCORRECTO");
            out << "INCORRECTO" << "\n ******************** \n";
        }

        flush(out);
        delete rowTemp;
        rowTemp = NULL;
    }
    else if (mensaje.startsWith("LOGUP"))
    {
        out << "******** LOGUP ********" << "\n* NICKNAME\n\t";
        out << lstMsg[1] << "\n* PASSWORD\n\t";
        out << lstMsg[2] << "\n* NOMBRE\n\t";
        out << lstMsg[3] << "\n* CORREO\n\t";
        out << lstMsg[4] << "\n ******************** \n\t";
        flush(out);

        TADRow *rowTemp = new TADRow(lstMsg[3], lstMsg[4], lstMsg[1], lstMsg[2]);
        if (matrix->getHeaderRow()->get(rowTemp) != NULL)
        {
            producer("LOGUP^INCORRECTO");
            out << "INCORRECTO" << "\n ******************** \n";

            delete rowTemp;
            rowTemp = NULL;
        }
        else
        {
            if (matrix->getHeaderRow()->insert(rowTemp) != NULL)
            {
                producer("LOGUP^CORRECTO");
                out << "CORRECTO" << "\n ******************** \n";
            }
            else
            {
                producer("LOGUP^INCORRECTO");
                out << "INCORRECTO" << "\n ******************** \n";

                delete rowTemp;
                rowTemp = NULL;
            }
        }

        flush(out);
    }
    else if (mensaje.startsWith("LOGOUT"))
    {
        user = NULL;

        flush(out);
        out << "******** LOGUP ********\t";
        out << "CORRECTO" << "\n ******************** \n";
    }
    else if (mensaje.startsWith("SESSION"))
    {
        out << "******** SESSION ********\t";
        if (user != NULL)
        {
            QString currentUser("SESSION^");
            currentUser.append(user->getNickname());
            producer(currentUser);
            out << user->getNickname() << "\n ******************** \n";
        }
        else
        {
            out << "INACTIVO" << "\n ******************** \n";
        }
    }
    ui->edtLog->append(log);
}
