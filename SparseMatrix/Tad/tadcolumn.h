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
    TADColumn(QString _nombre, int _tipo, QString _fechaCreacion, QString _nickDuenio, QString _fechaCambio, QString _nickCambio, QString _filepath);
    TADColumn(QString _nombre, int _tipo, QString _fecha, QString _nickDuenio, QString _filepath);
    TADColumn(QString _nombre);
    ~TADColumn();

    enum
    {
        DOCUMENTO, PRESENTACION, LIENZO
    };

    void setNombre(QString value);
    QString getNombre();
    void setTipo(QString tipo);
    void setTipo(int value);
    int getTipo();
    QString getTipoString();
    void setFechaCreacion(QString value);
    QString getFechaCreacion();
    void setNickCreacion(QString value);
    QString getNickCreacion();
    void setFechaUltimoCambio(QString value);
    QString getFechaUltimoCambio();
    void setNickUltimoCambio(QString value);
    QString getNickUltimoCambio();
    void setDisponible();
    void togleDisponible();
    bool isDisponible();
    void setFilepath(QString value);
    QString getFilePath();

    int compare(TADColumn *column);
    ColumnList *getInternalColumn();
    MatrixNode *addInternalColumn(MatrixNode *value);
    MatrixNode *removeInternalColumn(TADMatrixNode *value);
    bool eraseInternalColumn(TADMatrixNode *value);
    QString getList();
    QString toString();
    QString getNodeName();
    QString createNode();
    QString pointNode(TADColumn *next);
};

#endif // TADCOLUMN_H
