#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
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
    TADRow *user;
    Matrix *matrix;

    QJsonDocument jsd;

    QTcpServer *tcpServer;
    QTcpSocket *tcpCliente;

    void interpretarMensaje(QString mensaje);
    bool cargarUsuario();
    bool cargarArchivo();
    void graficar();
};

#endif // MAINWINDOW_H
