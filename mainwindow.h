#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
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
    void leerSocketCliente();

private slots:
    void on_btnMensaje_clicked();

private:
    Ui::MainWindow *ui;
    Matrix *matrix;

    QTcpServer *tcpServer;
    QTcpSocket *tcpCliente;

    void interpretarMensaje(QString mensaje);
};

#endif // MAINWINDOW_H
