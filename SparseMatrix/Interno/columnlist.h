#ifndef COLUMNLIST_H
#define COLUMNLIST_H
#include "matrixnode.h"

class ColumnList
{
    int count;
    MatrixNode *head;
    MatrixNode *tail;

    MatrixNode *insert(MatrixNode *current, MatrixNode *value);
    MatrixNode *get(MatrixNode *current, TADMatrixNode *value);

public:
    ColumnList();
    ~ColumnList();

    /* CAPACIDAD */
    bool isEmpty();
    int size();
    void clear();

    /* MODIFICADORES */
    MatrixNode *push_front(MatrixNode *value);
    MatrixNode *push_back(MatrixNode *value);
    MatrixNode *insert(MatrixNode *value);
    MatrixNode *removeFirst();
    MatrixNode *removeLast();
    MatrixNode *removeOne(TADMatrixNode *value);

    /* OPERACIONES */
    MatrixNode *first();
    MatrixNode *last();
    TADMatrixNode *front();
    TADMatrixNode *back();
    MatrixNode *get(TADMatrixNode *value);
    QString graph();
};

#endif // COLUMNAMATRIZ_H
