#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QDate>
#include <QTextCodec>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "SparseMatrix/matrix.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void nuevaConexion();
    void consumer();
    void producer(QString value);

private slots:
    void on_btnMensaje_clicked();

private:
    Ui::MainWindow *ui;

    int rowCountTable;
    TADRow *currentUserSession;
    TADColumn *currentFile;
    Matrix *matrix;

    QJsonDocument jsd;

    QTcpServer *tcpServer;
    QTcpSocket *tcpCliente;

    enum
    {
        KEY, REQUEST, ANSWER
    };

    void interpreter(QString message);

    void actionLogIn(QStringList value);
    void actionLogUp(QStringList value);
    void actionLogOut();
    void actionSession();
    void actionListFiles();
    void actionInfoFiles(QStringList value);
    void actionCreateFile(QStringList value);
    void actionUpdateFile(QStringList value);
    void actionDeleteFile(QStringList value);
    void actionCoderImage(QStringList value);
    void actionAddPermission(QStringList value);
    void actionDeletePermission(QStringList value);
    void actionListUsers();
    void actionSharedUsers(QString filename);
    void setLog(QString key, QString request, QString answer);
    void loadJSON();
    bool loadUserJSON();
    bool loadFilesJSON();
    void createJSON();
    QString createUserJSON();
    QString createFilesJSON();
    QString coderImage(QString filepath);
    void graphMatrix();
};

#endif // MAINWINDOW_H
