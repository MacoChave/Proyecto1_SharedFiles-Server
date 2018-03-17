#ifndef TADCOLUMN_H
#define TADCOLUMN_H
#include "../Interno/columnlist.h"

class TADColumn
{
    QString nombre;
    int tipo;
    QString fechaCreacion;
    QString nickCreacion;
    QString fechaUltimoCambio;
    QString nickUltimoCambio;
    bool disponible;
    QString filepath;
    ColumnList *internalColumn;
public:
    TADColumn();
    TADColumn(QString _nombre, int _tipo, QString _fecha, QString _nickDuenio, QString _filepath);
    TADColumn(QString _nombre);
    ~TADColumn();

    void setNombre(QString value);
    QString getNombre();
    void setTipo(int value);
    int getTipo();
    void setFechaCreacion(QString value);
    QString getFechaCreacion();
    void setNickCreacion(QString value);
    QString getNickCreacion();
    void setFechaUltimoCambio(QString value);
    QString getFechaUltimoCambio();
    void setNickUltimoCambio(QString value);
    QString getNickUltimoCambio();
    void togleDisponible();
    bool isDisponible();
    void setFilepath(QString value);
    QString getFilePath();

    int compare(TADColumn *column);
    ColumnList *getInternalColumn();
    MatrixNode *addInternalColumn(TADMatrixNode *value);
    MatrixNode *addInternalColumn(MatrixNode *value);
    QString toString();
    QString getNodeName();
    QString createNode();
    QString pointNode(TADColumn *next);
};

#endif // TADCOLUMN_H
