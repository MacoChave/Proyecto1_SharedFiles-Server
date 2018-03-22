#ifndef TADROW_H
#define TADROW_H
#include "../Interno/rowlist.h"

class TADRow
{
    QString nombre;
    QString correo;
    QString nickname;
    QString password;
    RowList *internalRow;
public:
    TADRow();
    TADRow(QString _nombre, QString _correo, QString _nickname, QString _password);
    TADRow(QString _nombre);
    ~TADRow();

    void setNombre(QString value);
    QString getNombre();
    void setCorreo(QString value);
    QString getCorreo();
    void setNickname(QString value);
    QString getNickname();
    void setPassword(QString value);
    QString getPassword();

    int compare(TADRow *row);
    int comparePass(TADRow *row);
    RowList *getInternalRow();
    MatrixNode *addInternalRow(MatrixNode *value);
    QString toString();
    QString getNodeName();
    QString createNode();
    QString pointNode(TADRow *next);
};

#endif // TADROW_H
