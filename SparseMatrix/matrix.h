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

    /* MODIFICADORES */
    void insert(TADMatrixNode *value);
    void erase(QString archivo, QString nickname);
    MatrixNode *get(QString archivo, QString nickname);
    void edit(QString archivo, QString nickname, TADMatrixNode *value);
    MatrixNode *remove(QString archivo, QString nickname);

    /* OPERACIONES */
    void graph(QString filename);
    /*
    clear()
    isEmpty()
     */
};

#endif // MATRIZ_H
