#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentUserSession = NULL;
    matrix = new Matrix();

    tcpServer = new QTcpServer(this);
    tcpServer->setMaxPendingConnections(1);

    tcpCliente = new QTcpSocket(this);

    if (!tcpServer->listen(QHostAddress::Any, 1234))
        qDebug() << tcpCliente->errorString();
    else
    {
        qDebug() << "Servidor iniciado";
        connect(
                    tcpServer,
                    SIGNAL (newConnection()),
                    this,
                    SLOT (nuevaConexion())
                );
    }

    cargar();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete matrix;
}

/***********************************************************************************
 * MANEJO DE CONEXION SERVIDOR
 **********************************************************************************/
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
                currentUserSession = row->getData();
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
        currentUserSession = NULL;

        out << "******** LOGUP ********\t";
        out << "CORRECTO" << "\n ******************** \n";
        flush(out);
    }
    else if (mensaje.startsWith("SESSION"))
    {
        out << "******** SESSION ********\t";
        if (currentUserSession != NULL)
        {
            QString currentUser("SESSION^");
            currentUser.append(currentUserSession->getNickname());
            producer(currentUser);
            out << currentUserSession->getNickname() << "\n ******************** \n";
        }
        else
        {
            out << "INACTIVO" << "\n ******************** \n";
        }
        flush(out);
    }
    else if (mensaje.startsWith("LISTDOCS"))
    {
        /* VALIDAR EXISTENCIA DE NODOS */
        if (matrix->getHeaderColumn()->isEmpty())
        {
            producer("LISTDOCS");
            flush(out);

            return;
        }

        /* NODOS TAD TEMPORALES */
        Node<TADColumn *> *nodeColumnTemporal = NULL;
        MatrixNode *matrixNodeTemporal = NULL;
        TADColumn *tadColumnaTemporal = NULL;
        TADMatrixNode *tadMatrixNodeTemporal = NULL;

        /* IGUALAR NODOS TAD TEMPORALES */
        nodeColumnTemporal = matrix->getHeaderColumn()->first();
        matrixNodeTemporal = currentUserSession->getInternalRow()->first();

        /* RECORRER NODOS, VALIDANDO PRESENCIA DE USUARIO ACTUAL */
        QString result("LISTDOCS^");
        while (nodeColumnTemporal != NULL || matrixNodeTemporal != NULL)
        {
            tadColumnaTemporal = nodeColumnTemporal->getData();
            tadMatrixNodeTemporal = matrixNodeTemporal->getData();

            if (tadColumnaTemporal->getNombre().compare(tadMatrixNodeTemporal->getArchivo()) == 0)
            {
                result.append(tadMatrixNodeTemporal->getArchivo());
                result.append("^");

                if (tadColumnaTemporal->getTipo() == tadColumnaTemporal->DOCUMENTO)
                    result.append("Documento");
                else if (tadColumnaTemporal->getTipo() == tadColumnaTemporal->LIENZO)
                    result.append("Lienzo");
                else if (tadColumnaTemporal->getTipo() == tadColumnaTemporal->PRESENTACION)
                    result.append("Presentacion");

                result.append("^");

                if (tadMatrixNodeTemporal->getPermiso() == tadMatrixNodeTemporal->DUENIO)
                    result.append("Propietario");
                else if (tadMatrixNodeTemporal->getPermiso() == tadMatrixNodeTemporal->EDITAR)
                    result.append("Editar");
                else if (tadMatrixNodeTemporal->getPermiso() == tadMatrixNodeTemporal->VER)
                    result.append("Ver");

                result.append("^");
                nodeColumnTemporal = nodeColumnTemporal->getNext();
                matrixNodeTemporal = matrixNodeTemporal->getNext();

                tadColumnaTemporal = NULL;
                tadMatrixNodeTemporal = NULL;
            }
            else
            {
                nodeColumnTemporal = nodeColumnTemporal->getNext();
            }
            flush(out);
        }

        producer(result);
        qDebug() << result;
    }
    else if (mensaje.startsWith("INFODOC"))
    {
        /* VALIDAR EXISTENCIA DE ARCHIVOS */
        if (matrix->getHeaderColumn()->isEmpty())
        {
            producer("INFODOC^");

            return;
        }

        /* NODOS TAD TEMPORAL */
        TADColumn *tadColumnaTemporal = new TADColumn(lstMsg[1]);

        /* NODOS RESULTADO BUSQUEDA */
        TADColumn *tadColumnResult = matrix->getHeaderColumn()->get(tadColumnaTemporal)->getData();
        if (tadColumnResult != NULL)
        {
            QString filename;
            QString result("INFODOC^");
            filename = tadColumnResult->getFilePath();

            QFile fileTemp(filename);
            if (fileTemp.open(QFile::ReadOnly))
            {
                QTextStream fileRead(&fileTemp);
                result.append(fileRead.readAll());
                out << lstMsg[1];
            }

            producer(result);
        }

        flush(out);
        delete tadColumnaTemporal;
        tadColumnaTemporal = NULL;
    }
    else if (mensaje.startsWith("CREATEDOCS"))
    {
        // CREAR ARCHIVO Y NODO MATRIZ PARA USUARIO ACTUAL
    }
    else if (mensaje.startsWith("UPDATEDOCS"))
    {
        // BUACAR ARCHIVO Y ACTUALIZAR
    }
    else if (mensaje.startsWith("DELETEDOCS"))
    {
        // ELIMINAR ARCHIVO Y SU LISTA INTERNA.
        // ELIMINAR NODO MATRIZ DE TODOS LOS USUARIOS QUE TENGAN ESE ARCHIVO
    }
    ui->edtLog->append(log);
}

/***********************************************************************************
 * MANEJO DE CARGA DE ARCHIVOS JSON
 **********************************************************************************/
void MainWindow::cargar()
{
        QString filename("Files/usuarios.json");

        /* ARCHIVO USUARIOS */
        QFile file(filename);
        if (!file.open(QFile::ReadOnly))
        {
            qDebug() << "Archivo 'usuarios.json' no abierto";
            return;
        }
        qDebug() << "Archivo 'usuarios.json' abierto";

        jsd = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (!cargarUsuario())
        {
            qDebug() << "No se pudo cargar los usuarios";
            return;
        }

        /* ARCHIVO ARCHIVOS */
        filename = "Files/archivos.json";
        file.setFileName(filename);
        if (!file.open(QFile::ReadOnly))
        {
            qDebug() << "Archivo 'archivos.json' no abierto";
            return;
        }
        qDebug() << "Archivo 'archivos.json' abierto";

        jsd = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (!cargarArchivo())
        {
            qDebug() << "No se pudo cargar los archivos";
            return;
        }
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

void MainWindow::on_btnMensaje_clicked()
{
//    producer(ui->edtMensaje->text());
//    ui->edtMensaje->clear();

    graficar();
}
