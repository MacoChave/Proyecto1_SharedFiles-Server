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

    loadJSON();
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
        interpreter((QString) buffer);
    }
}

void MainWindow::producer(QString value)
{
    tcpCliente->write(
                value.toLatin1().data(),
                value.size()
            );
}

void MainWindow::interpreter(QString message)
{
    QStringList lstMsg = message.split("^");

    if (message.startsWith("LOGIN"))
        actionLogIn(lstMsg);
    else if (message.startsWith("LOGUP"))
        actionLogUp(lstMsg);
    else if (message.startsWith("LOGOUT"))
        actionLogOut();
    else if (message.startsWith("SESSION"))
        actionSession();
    else if (message.startsWith("LISTFILES"))
        actionListFiles();
    else if (message.startsWith("INFOFILE"))
        actionInfoFiles(lstMsg);
    else if (message.startsWith("CREATEFILE"))
        actionCreateFile(lstMsg);
    else if (message.startsWith("UPDATEFILE"))
        actionUpdateFile(lstMsg);
    else if (message.startsWith("DELETEFILE"))
        actionDeleteFile(lstMsg);
}

/***********************************************************************************
 * MANEJO DE SOLICITUDES
 **********************************************************************************/
void MainWindow::actionLogIn(QStringList value)
{
    ui->tblLog->insertRow(y);
    ui->tblLog->setItem(
                y, KEY,
                new QTableWidgetItem("Inicio de Sesión")
                );
    ui->tblLog->setItem(
                y, REQUEST,
                new QTableWidgetItem(value[0])
                );

    TADRow *temporalRow = new TADRow();
    temporalRow->setNickname(value[1]);
    temporalRow->setPassword(value[2]);

    Node<TADRow *> *row = matrix->getHeaderRow()->get(temporalRow);
    if (row != NULL)
    {
        if (row->getData()->comparePass(temporalRow) == 0)
        {
            currentUserSession = row->getData();
            producer("LOGIN^CORRECTO");

            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("Correcto")
                        );
        }
        else
        {
            producer("LOGIN^INCORRECTO");

            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("Incorrecto")
                        );
        }
    }
    else
    {
        producer("LOGIN^INCORRECTO");

        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem("Incorrecto")
                    );
    }

    y++;
    delete temporalRow;
    temporalRow = NULL;
}

void MainWindow::actionLogUp(QStringList value)
{
    QString request;
    request.append(value[0]);
    request.append("\n");
    request.append(value[2]);
    request.append("\n");
    request.append(value[3]);

    ui->tblLog->insertRow(y);
    ui->tblLog->setItem(
                y, KEY,
                new QTableWidgetItem("Crear usuario")
                );
    ui->tblLog->setItem(
                y, REQUEST,
                new QTableWidgetItem(request)
                );

    TADRow *temporalRow = new TADRow(value[3], value[4], value[1], value[2]);
    if (matrix->getHeaderRow()->get(temporalRow) != NULL)
    {
        producer("LOGUP^INCORRECTO");

        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem("Usuario ya existe")
                    );

    }
    else
    {
        if (matrix->getHeaderRow()->insert(temporalRow) != NULL)
        {
            producer("LOGUP^CORRECTO");

            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("Usuario creado")
                        );
        }
        else
        {
            producer("LOGUP^INCORRECTO");

            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("Usuario no creado")
                        );
        }
    }

    y++;
    delete temporalRow;
    temporalRow = NULL;
}

void MainWindow::actionLogOut()
{
    ui->tblLog->insertRow(y);
    ui->tblLog->setItem(
                y, KEY,
                new QTableWidgetItem("Cerrar Sesion")
                );
    ui->tblLog->setItem(
                y, REQUEST,
                new QTableWidgetItem(currentUserSession->getNickname())
                );

    currentUserSession = NULL;

    ui->tblLog->setItem(
                y, ANSWER,
                new QTableWidgetItem("Sesion concluida");
                );

    y++;
}

void MainWindow::actionSession()
{
    ui->tblLog->insertRow(y);
    ui->tblLog->setItem(
                y, KEY,
                new QTableWidgetItem("Informe de sesión")
                );
    ui->tblLog->setItem(
                y, REQUEST,
                new QTableWidgetItem("Usuario en sesión")
                );
    if (currentUserSession != NULL)
    {
        QString currentUser("SESSION^");
        currentUser.append(currentUserSession->getNickname());
        producer(currentUser);

        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem(currentUserSession->getNickname())
                    );
    }
    else
        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem("Ningún usuario en sesión")
                    );

    y++;
}

void MainWindow::actionListFiles()
{
    ui->tblLog->insertRow(y);
    ui->tblLog->setItem(
                y, KEY,
                new QTableWidgetItem("Lista de archivos")
                );

    if (currentUserSession == NULL)
    {
        ui->tblLog->setItem(
                    y, REQUEST,
                    new QTableWidgetItem("Ningún usuario en sesión")
                    );
        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem("No hay archivos para mostrar")
                    );

        producer("LISTFILES^");
        y++;
        return;
    }

    ui->tblLog->setItem(
                y, REQUEST,
                new QTableWidgetItem(currentUserSession->getNickname())
                );

    /* VALIDAR EXISTENCIA DE NODOS */
    if (matrix->getHeaderColumn()->isEmpty())
    {
        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem("No hay archivos para mostar")
                    );

        producer("LISTFILES^");
        y++;
        return;
    }

    /* TEMPORALES */
    Node<TADColumn *> *temporalNodeColumn = NULL;
    TADColumn *temporalTadColumn = NULL;
    MatrixNode *temporalMatrixNode = NULL;
    TADMatrixNode *temporalTadMatrixNode = NULL;

    /* IGUALAR NODOS TAD TEMPORALES */
    temporalNodeColumn = matrix->getHeaderColumn()->first();
    temporalMatrixNode = currentUserSession->getInternalRow()->first();

    /* RECORRER NODOS, VALIDANDO PRESENCIA DE USUARIO ACTUAL */
    int countFiles = 0;
    QString result("LISTFILES^");

    while (temporalNodeColumn != NULL || temporalMatrixNode != NULL)
    {
        temporalTadColumn = temporalNodeColumn->getData();
        temporalTadMatrixNode = temporalMatrixNode->getData();

        if (temporalTadColumn->getNombre().compare(temporalTadMatrixNode->getArchivo()) == 0)
        {
            result.append(temporalTadColumn->getNombre());
            result.append("^");

            if (temporalTadColumn->getTipo() == temporalTadColumn->DOCUMENTO)
                result.append("Documento");
            else if (temporalTadColumn->getTipo() == temporalTadColumn->LIENZO)
                result.append("Lienzo");
            else if (temporalTadColumn->getTipo() == temporalTadColumn->PRESENTACION)
                result.append("Presentacion");

            result.append("^");

            if (temporalTadMatrixNode->getPermiso() == temporalTadMatrixNode->DUENIO)
                result.append("Propietario");
            else if (temporalTadMatrixNode->getPermiso() == temporalTadMatrixNode->EDITAR)
                result.append("Editar");
            else if (temporalTadMatrixNode->getPermiso() == temporalTadMatrixNode->VER)
                result.append("Ver");

            result.append("^");
            temporalNodeColumn = temporalNodeColumn->getNext();
            temporalMatrixNode = temporalMatrixNode->getNext();

            countFiles++;
            temporalTadColumn = NULL;
            temporalTadMatrixNode = NULL;
        }
        else
            temporalNodeColumn = temporalNodeColumn->getNext();
    }

    QString answer("Se encontraron ");
    answer.append(QString::number(countFiles));
    answer.append(" archivos");
    ui->tblLog->setItem(
                y, ANSWER,
                new QTableWidgetItem(answer)
                );
    producer(result);
    y++;
}

void MainWindow::actionInfoFiles(QStringList value)
{
    ui->tblLog->insertRow(y);
    ui->tblLog->setItem(
                y, KEY,
                new QTableWidgetItem("Información de archivo")
                );
    ui->tblLog->setItem(
                y, REQUEST,
                new QTableWidgetItem(value[1]);
                );
    /* VALIDAR EXISTENCIA DE ARCHIVOS */
    if (matrix->getHeaderColumn()->isEmpty())
    {
        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem("No hay archivos para mostrar");
                    );

        producer("INFOFILE^");

        y++;
        return;
    }

    /* NODOS TAD TEMPORAL */
    TADColumn *temporalTadColumn = new TADColumn(value[1]);

    /* NODOS RESULTADO BUSQUEDA */
    TADColumn *tadColumn = matrix->getHeaderColumn()->get(temporalTadColumn)->getData();
    if (tadColumn != NULL)
    {
        QString filepath;
        QString result("INFOFILE^");
        filepath = tadColumn->getFilePath();

        QFile temporalFile(filepath);
        if (temporalFile.open(QFile::ReadOnly))
        {
            QTextStream in(&temporalFile);
            result.append(in.readAll());

            temporalFile.close();
            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("Archivo leido")
                        );
        }
        else
            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("Error de lectura")
                        );

        producer(result);
    }

    y++;
    delete temporalTadColumn;
    temporalTadColumn = NULL;
}

bool MainWindow::actionCreateFile(QStringList value)
{
    QString filename = value[1];
    QString permission = value[2];
    QString fileType = value[3];
    QString content = value[4];
    QString strCurrentDate;
    QDate currentDate = QDate::currentDate();
    strCurrentDate = currentDate.toString("yy-MM-dd");

    ui->tblLog->insertRow(y);
    ui->tblLog->setItem(
                y, KEY,
                new QTableWidgetItem("Crear archivo")
                );
    ui->tblLog->setItem(
                y, REQUEST,
                new QTableWidgetItem(filename);
                );

    TADColumn *temporalTadColumn = new TADColumn(filename);
    if (matrix->getHeaderColumn()->get(temporalTadColumn) != NULL)
    {
        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem("Archivo ya existe");
                    );

        y++;
        return;
    }

    QString filepath("Files/");
    filepath.append(filename);
    filepath.append(".json");

    QFile file(filepath);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << content;
        flush(out);

        file.close();

        TADColumn *tadColumn = new TADColumn();
        tadColumn->setNombre(filename);
        tadColumn->setTipo(fileType);
        tadColumn->setFechaCreacion(strCurrentDate);
        tadColumn->setNickCreacion(currentUserSession->getNickname());
        tadColumn->setFechaUltimoCambio(strCurrentDate);
        tadColumn->setNickUltimoCambio(currentUserSession->getNickname());
        tadColumn->setFilepath(filepath);

        TADMatrixNode *tadMatrixNode = new TADMatrixNode();
        tadMatrixNode->setArchivo(filename);
        tadMatrixNode->setNickname(currentUserSession->getNickname());
        tadMatrixNode->setPermiso(permission);

        if (matrix->getHeaderColumn()->insert(tadColumn) != NULL)
        {
            MatrixNode *matrixNode = NULL;
            matrixNode = tadColumn->addInternalColumn(tadMatrixNode);
            currentUserSession->addInternalRow(matrixNode);

            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("Archivo creado");
                        );
        }
        else
            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("Archivo no creado");
                        );
    }
    else
        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem("Archivo JSON no creado");
                    );

    y++;
}

bool MainWindow::actionUpdateFile(QStringList value)
{
    QString filename = lstMsg[1];
    QString contenido = lstMsg[4];
    QString strCurrentDate;
    QDate currentDate = QDate::currentDate();
    strCurrentDate = currentDate.toString("yy-MM-dd");

    ui->tblLog->insertRow(y);
    ui->tblLog->setItem(
                y, KEY,
                new QTableWidgetItem("Actualizar archivo")
                );
    ui->tblLog->setItem(
                y, REQUEST,
                new QTableWidgetItem(filename);
                );

    QString filepath("Files/");
    filepath.append(filename);
    filepath.append(".json");

    QFile file(filepath);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << contenido;
        flush(out);

        file.close();

        TADColumn *temporalTadColumn = new TADColumn(filename);
        TADColumn *tadColumn = NULL;

        tadColumn = matrix->getHeaderColumn()->get(temporalTadColumn)->getData();
        if (tadColumn != NULL)
        {
            tadColumn->setFechaUltimoCambio(strCurrentDate);
            tadColumn->setNickUltimoCambio(currentUserSession->getNickname());

            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("Cambios guardados");
                        );
        }
        else
            ui->tblLog->setItem(
                        y, ANSWER,
                        new QTableWidgetItem("No se encontró el archivo");
                        );
        delete temporalTC;
    }
    else
        ui->tblLog->setItem(
                    y, ANSWER,
                    new QTableWidgetItem("No se encontró el archivo JSON");
                    );

    y++;
}

bool MainWindow::actionDeleteFile(QStringList value)
{

}

/***********************************************************************************
 * MANEJO DE CARGA DE ARCHIVOS JSON
 **********************************************************************************/
void MainWindow::loadJSON()
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
        if (!loadUserJSON())
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
        if (!loadFilesJSON())
        {
            qDebug() << "No se pudo cargar los archivos";
            return;
        }
}

bool MainWindow::loadUserJSON()
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

bool MainWindow::loadFilesJSON()
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

/***********************************************************************************
 * MANEJO DE MATRIZ
 **********************************************************************************/
void MainWindow::graphMatrix()
{
    matrix->graph("Matriz");
}

/***********************************************************************************
 * METODOS DE ACCIÓN DE BOTÓN
 **********************************************************************************/
void MainWindow::on_btnMensaje_clicked()
{
    graphMatrix();
}
