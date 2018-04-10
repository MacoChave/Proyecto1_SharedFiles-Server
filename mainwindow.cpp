#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentUserSession = NULL;
    currentFile = NULL;
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
    createJSON();
    matrix->clear();
    delete ui;
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
    else if (message.startsWith("CODER"))
        actionCoderImage(lstMsg);
    else if (message.startsWith("ADDPERMISSION"))
        actionAddPermission(lstMsg);
    else if (message.startsWith("DELETEPERMISSION"))
        actionDeletePermission(lstMsg);
    else if (message.startsWith("LISTUSER"))
        actionListUsers();
    else if (message.startsWith("SHAREDUSER"))
        actionSharedUsers(lstMsg[1]);
}

/***********************************************************************************
 * MANEJO DE SOLICITUDES
 **********************************************************************************/
void MainWindow::actionLogIn(QStringList value)
{
    QString key, request, answer;
    key = "Inicio de sesión";
    request = value[1];

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
            answer = "Correcto";
        }
        else
        {
            producer("LOGIN^INCORRECTO^PASSWORD");
            answer = "Incorrecto";
        }
    }
    else
    {
        producer("LOGIN^INCORRECTO^USUARIO");
        answer = "Incorrecto";
    }

    setLog(key, request, answer);
    delete temporalRow;
    temporalRow = NULL;
}

void MainWindow::actionLogUp(QStringList value)
{
    QString key, request, answer;
    key = "Crear usuario";
    request = value[2];

    /************************************************************************
     * ********************************************************************
     * PROBAR CON UN SIMPLE IF ELSE
     * ********************************************************************
    ************************************************************************/
    TADRow *temporalRow = new TADRow(value[3], value[4], value[1], value[2]);
    if (matrix->getHeaderRow()->get(temporalRow) != NULL)
    {
        producer("LOGUP^INCORRECTO^DUPLICADO");
        answer = "Usuario ya existe";
        delete temporalRow;
        temporalRow = NULL;
    }
    else
    {
        if (matrix->getHeaderRow()->insert(temporalRow) != NULL)
        {
            producer("LOGUP^CORRECTO");
            answer = "Correcto";
        }
        else
        {
            producer("LOGUP^INCORRECTO");
            answer = "Incorrecto";
            delete temporalRow;
            temporalRow = NULL;
        }
    }

    setLog(key, request, answer);
    delete temporalRow;
    temporalRow = NULL;
}

void MainWindow::actionLogOut()
{
    QString key, request, answer;
    key = "Cerrar sesión";
    request = currentUserSession->getNickname();
    currentUserSession = NULL;
    answer = "Correcto";

    setLog(key, request, answer);
}

void MainWindow::actionSession()
{
    QString key, request, answer;
    key = "Usuario en sesión";
    request = "Usuario";

    if (currentUserSession != NULL)
    {
        QString currentUser("SESSION^");
        currentUser.append(currentUserSession->getNickname());
        producer(currentUser);
        answer = currentUserSession->getNickname();
    }
    else
        answer = "Sin usuario activo";

    setLog(key, request, answer);
}

void MainWindow::actionListFiles()
{
    QString key, request, answer;
    key = "Lista de ficheros";
    request = currentUserSession->getNickname();

    if (currentUserSession == NULL)
    {
        request = "Sin usuario activo";
        answer = "Ningún fichero encontrado";

        return;
    }

    /* VALIDAR EXISTENCIA DE NODOS */
    if (matrix->getHeaderColumn()->isEmpty())
    {
        answer = "Ningún fichero encontrado";

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
            result.append(temporalTadColumn->getTipoString());
            result.append("^");
            result.append(temporalTadMatrixNode->getPermisoString());
            result.append("^");

            countFiles++;
            temporalMatrixNode = temporalMatrixNode->getNext();
            temporalTadColumn = NULL;
            temporalTadMatrixNode = NULL;
        }

        temporalNodeColumn = temporalNodeColumn->getNext();
    }

    answer = "Se encontraron ";
    answer.append(QString::number(countFiles));
    answer.append(" archivos");

    producer(result);
    setLog(key, request, answer);
}

void MainWindow::actionInfoFiles(QStringList value)
{
    QString key, request, answer;
    key = "Información de archivo";
    request = value[1];

    /* NODOS TAD TEMPORAL */
    TADColumn *temporalTadColumn = new TADColumn(value[1]);

    currentFile = matrix->getHeaderColumn()->get(temporalTadColumn)->getData();
    QString result("INFOFILE^");
    if (currentFile != NULL)
    {
        QString filepath;
        filepath = currentFile->getFilePath();

        QFile temporalFile(filepath);
        if (temporalFile.open(QFile::ReadOnly))
        {
            QTextStream in(&temporalFile);
            in.setCodec(QTextCodec::codecForName("UTF-8"));
            result.append(in.readAll());

            temporalFile.close();
            answer = "Archivo leido";
        }
        else
            answer = "Error de lectura";

    }

    producer(result);
    setLog(key, request, answer);
    delete temporalTadColumn;
    temporalTadColumn = NULL;
}

void MainWindow::actionCreateFile(QStringList value)
{
    QString key, request, answer;
    key = "Crear archivo";
    request = value[1];

    QString filename = value[1];
    QString permission = value[2];
    QString fileType = value[3];
    QString content = value[4];
    QString strCurrentDate;
    QDate currentDate = QDate::currentDate();
    strCurrentDate = currentDate.toString("yyyy-MM-dd");

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

        if (matrix->getColumn(tadColumn) != NULL)
        {
            answer = "Nombre de archivo ocupado";
            producer("CREATEFILE^REPETIDO");
            delete tadColumn;
        }
        else
        {
            matrix->insertMatrixNode(currentUserSession->getNickname(), filename, TADMatrixNode::DUENIO);
            answer = "Archivo creado";
        }
    }
    else
        answer = "Archivo JSON no creado";

    setLog(key, request, answer);
}

void MainWindow::actionUpdateFile(QStringList value)
{
    QString key, request, answer;
    key = "Actualizar archivo";
    request = value[1];

    QString filename = value[1];
    QString contenido = value[4];
    QString strCurrentDate;
    QDate currentDate = QDate::currentDate();
    strCurrentDate = currentDate.toString("yy-MM-dd");

    if (currentFile == NULL)
        return;

    QString filepath(currentFile->getFilePath());
    filepath.append(filename);

    QFile file(filepath);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out << contenido;
        flush(out);

        file.close();

        currentFile->setFechaUltimoCambio(strCurrentDate);
        currentFile->setNickUltimoCambio(currentUserSession->getNickname());

        answer = "Archivo actualizado";
    }
    else
        answer = "No se encontró archivo";
}

void MainWindow::actionDeleteFile(QStringList value)
{
    QString key, request, answer;
    key = "Eliminar archivo";
    request = value[1];

//    TADColumn *tadColumn = new TADColumn(value[1]);
//    if (matrix->eraseColumn(tadColumn))
//        answer = "Correcto";
//    else
//        answer = "Correcto";

//    delete tadColumn;

    setLog(key, request, answer);
}

void MainWindow::actionCoderImage(QStringList value)
{
    QString message("CODER^");
    message.append(coderImage(value[1]));

    producer(message);
}

void MainWindow::actionAddPermission(QStringList value)
{
    QString key, request, answer;
    key = "Agregar Permiso";
    request = value[1] + ", " + value[2];
    matrix->insertMatrixNode(value[1], value[2], value[3]);

    answer = "Permiso agregado";

    setLog(key, request, answer);
}

void MainWindow::actionDeletePermission(QStringList value)
{
    QString key, request, answer;
    key = "Eliminar permiso";
    request = value[1] + ", " + value[2];
    matrix->eraseMatrixNode(value[1], value[2]);

    answer = "Permiso eliminado";

    setLog(key, request, answer);
}

void MainWindow::actionListUsers()
{
    QString message("LISTUSER^");
    message.append(matrix->getListUser());

    producer(message);
    qDebug() << message;
}

void MainWindow::actionSharedUsers(QString filename)
{
    QString message("SHAREDUSER^");
    message.append(matrix->getUserListMatrixNode(filename));

    producer(message);
    qDebug() << message;
}

void MainWindow::setLog(QString key, QString request, QString answer)
{
    int y = ui->tblLog->rowCount();

    ui->tblLog->insertRow(y);
    ui->tblLog->setItem(
                y, KEY,
                new QTableWidgetItem(key)
                );
    ui->tblLog->setItem(
                y, REQUEST,
                new QTableWidgetItem(request)
                );
    ui->tblLog->setItem(
                y, ANSWER,
                new QTableWidgetItem(answer)
                );
}

/***********************************************************************************
 * MANEJO DE CARGA DE ARCHIVOS JSON
 **********************************************************************************/
void MainWindow::loadJSON()
{
        QString filename("Files/usuarios.json");

        /* ARCHIVO USUARIOS */
        QString texto;
        QFile file(filename);
        if (!file.open(QFile::ReadOnly))
        {
            qDebug() << "Archivo 'usuarios.json' no abierto";
            return;
        }
        qDebug() << "Archivo 'usuarios.json' abierto";
        QTextStream inUser(&filename);
        inUser.setCodec(QTextCodec::codecForName("UTF-8"));
        texto = inUser.readAll();
        qDebug() << texto;

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
        QTextStream inFile(&filename);
        inFile.setCodec(QTextCodec::codecForName("UTF-8"));
        texto = inFile.readAll();
        qDebug() << texto;

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
    {
        qDebug() << "JSON usuarios vacío";
        return false;
    }

    QJsonArray jsa = jsd.array();

    for (int i = 0; i < jsa.count(); i++)
    {
        QJsonObject jso = jsa.at(i).toObject();
        QString nombre = jso["nombre"].toString();
        QString correo = jso["correo"].toString();
        QString nick = jso["nickname"].toString();
        QString pass = jso["clave"].toString();

        TADRow *row = new TADRow(nombre, correo, nick, pass);
        matrix->insertRow(row);
    }
    return true;
}

bool MainWindow::loadFilesJSON()
{
    if (jsd.isEmpty())
    {
        qDebug() << "JSON archivos vacío";
        return false;
    }

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
        column->setTipo(tipo);

        matrix->insertColumn(column);

        matrix->insertMatrixNode(nickCreacion, nombre, TADMatrixNode::DUENIO);

        for (int j = 0; j < jsaPermiso.count(); j++)
        {
            QJsonObject jsoPermiso = jsaPermiso.at(j).toObject();

            QString usuario = jsoPermiso["usuario"].toString();
            QString permiso = jsoPermiso["permiso"].toString();

            matrix->insertMatrixNode(usuario, nombre, permiso);
        }
    }
    return true;
}

void MainWindow::createJSON()
{
    QString filename("Files/usuarios.json");

    /* ARCHIVO USUARIOS */
    QFile file(filename);
    if (file.open(QFile::WriteOnly))
    {
        QString texto;
        texto = createUserJSON();
        QTextStream out(&file);
        out << texto;
        flush(out);

        file.close();
        qDebug() << "Archivo usuarios.json actualizado";
    }

    /* ARCHIVO ARCHIVOS */
    filename = "Files/archivos.json";
    file.setFileName(filename);
    if (file.open(QFile::WriteOnly))
    {
        QString texto;
        texto = createFilesJSON();
        QTextStream out(&file);
        out << texto;
        flush(out);

        file.close();
        qDebug() << "Archivo archivos.json actualizado";
    }
}

QString MainWindow::createUserJSON()
{
    QJsonArray rootJSA;

    Node<TADRow *> *temporalNode = matrix->getHeaderRow()->first();

    while (temporalNode != NULL)
    {
        QJsonObject jso;
        jso.insert("nombre", temporalNode->getData()->getNombre());
        jso.insert("correo", temporalNode->getData()->getCorreo());
        jso.insert("nickname", temporalNode->getData()->getNickname());
        jso.insert("clave", temporalNode->getData()->getPassword());

        rootJSA.push_back(jso);
        temporalNode = temporalNode->getNext();
    }
    jsd.setArray(rootJSA);
    QString strJSON(jsd.toJson());

    return strJSON;
}

QString MainWindow::createFilesJSON()
{
    QJsonArray rootJSA;

    Node<TADColumn *> *temporalNode = matrix->getHeaderColumn()->first();

    while (temporalNode != NULL)
    {
        QJsonObject jso;
        jso.insert("nombre", temporalNode->getData()->getNombre());
        jso.insert("tipo", temporalNode->getData()->getTipoString());
        jso.insert("creador", temporalNode->getData()->getNickCreacion());
        jso.insert("fecha_creacion", temporalNode->getData()->getFechaCreacion());
        jso.insert("modificador", temporalNode->getData()->getNickUltimoCambio());
        jso.insert("fecha_modificacion", temporalNode->getData()->getFechaUltimoCambio());
        jso.insert("ruta_contenido", temporalNode->getData()->getFilePath());

        QJsonArray jsa;
        MatrixNode *temporalMatrixNode = temporalNode->getData()->getInternalColumn()->first();
        while (temporalMatrixNode != NULL)
        {
            if (temporalMatrixNode->getData()->getPermiso() != TADMatrixNode::DUENIO)
            {
                QJsonObject childJSO;
                childJSO.insert("usuario", temporalMatrixNode->getData()->getNickname());
                childJSO.insert("permiso", temporalMatrixNode->getData()->getPermisoString());

                jsa.push_back(childJSO);
            }

            temporalMatrixNode = temporalMatrixNode->getBottom();
        }

        jso.insert("permisos", jsa);
        rootJSA.push_back(jso);
        temporalNode = temporalNode->getNext();
    }

    jsd.setArray(rootJSA);
    QString strJSON(jsd.toJson());

    return strJSON;
}

QString MainWindow::coderImage(QString filepath)
{
    QFile file(filepath);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray image = file.readAll();
        file.close();

        QString encoded = QString(image.toBase64());

        return encoded;
    }
    else
        return "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";
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
