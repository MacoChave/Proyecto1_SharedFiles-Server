#ifndef TADMATRIXNODE_H
#define TADMATRIXNODE_H
#include <QString>
#include <QTextStream>

class TADMatrixNode
{
    QString nickname;
    QString archivo;
    int permiso;

    enum
    {
        DUENIO, EDITAR, VER
    };
public:
    TADMatrixNode();
    TADMatrixNode(QString _nickname, QString _archivo, int _permiso);
    ~TADMatrixNode();

    QString getNickname();
    void setNickname(QString value);
    QString getArchivo();
    void setArchivo(QString value);
    int getPermiso();
    void setPermiso(int value);

    int compareColumn(TADMatrixNode *value);
    int compareRow(TADMatrixNode *value);
    QString getNodeName();
    QString toString();
    QString createNode();
    QString pointNode(TADMatrixNode *next);
};

#endif // TADMATRIXNODE_H
