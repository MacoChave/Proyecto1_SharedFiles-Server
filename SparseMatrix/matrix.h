#ifndef MATRIZ_H
#define MATRIZ_H
#include "Lista/list.h"
#include "Tad/tadcolumn.h"
#include "Tad/tadrow.h"
#include <QFile>
#include <QTextStream>
#include <QtDebug>

class Matrix
{
    List<TADColumn *> *headerColumns;
    List<TADRow *> *headerRows;

    QString getRankRow(MatrixNode *currentNode);
public:
    Matrix();
    ~Matrix();

    List<TADColumn *> *getHeaderColumn();
    List<TADRow *> *getHeaderRow();

    /* MODIFICADORES DE COLUMNA */
    void insertColumn(TADColumn *value);
    bool eraseColumn(TADColumn *value);
    Node<TADColumn *> *getColumn(TADColumn *value);

    /* MODIFICADORES DE FILA */
    void insertRow(TADRow *value);
    Node<TADRow *> *getRow(TADRow *value);
    QString getListUser();

    /* MODIFICADORES DE NODO MATRIZ */
    bool insertMatrixNode(QString user, QString filename, QString permiso);
    bool insertMatrixNode(QString user, QString filename, int permiso);
    bool eraseMatrixNode(QString user, QString filename);
    QString getUserListMatrixNode(QString filename);
    QString getFileListMatrixNode(QString user);

    /* OPERACIONES */
    void graph(QString filename);
    void clear();
};

#endif // MATRIZ_H
