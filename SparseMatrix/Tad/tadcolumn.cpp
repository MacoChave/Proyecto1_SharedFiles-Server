#include "tadcolumn.h"

TADColumn::TADColumn(QString _nombre, int _tipo, QString _fechaCreacion, QString _nickDuenio, QString _fechaCambio, QString _nickCambio, QString _filepath)
{
    nombre = _nombre;
    tipo = _tipo;
    fechaCreacion = _fechaCreacion;
    nickCreacion = _nickDuenio;
    fechaUltimoCambio = _fechaCambio;
    nickUltimoCambio = _nickCambio;
    disponible = true;
    filepath = _filepath;
    internalColumn = new ColumnList();
}

TADColumn::TADColumn(QString _nombre, int _tipo, QString _fecha, QString _nickDuenio, QString _filepath)
{
    nombre = _nombre;
    tipo = _tipo;
    fechaCreacion = _fecha;
    nickCreacion = _nickDuenio;
    fechaUltimoCambio = "";
    nickUltimoCambio ="";
    disponible = true;
    filepath = _filepath;
    internalColumn = new ColumnList();
}

TADColumn::TADColumn(QString _nombre)
{
    nombre = _nombre;
    tipo = -1;
    fechaCreacion = "";
    nickCreacion = "";
    fechaUltimoCambio = "";
    nickUltimoCambio = "";
    disponible = true;
    filepath = "";
    internalColumn = new ColumnList();
}

TADColumn::~TADColumn()
{
    nombre.clear();
    tipo = 0;
    fechaCreacion.clear();
    nickCreacion.clear();
    fechaUltimoCambio.clear();
    nickUltimoCambio.clear();
    disponible = false;
    filepath.clear();
    delete internalColumn;
    internalColumn = NULL;
}

void TADColumn::setNombre(QString value)
{
    nombre = value;
}

QString TADColumn::getNombre()
{
    return nombre;
}

void TADColumn::setTipo(int value)
{
    tipo = value;
}

int TADColumn::getTipo()
{
    return tipo;
}

void TADColumn::setFechaCreacion(QString value)
{
    fechaCreacion = value;
}

QString TADColumn::getFechaCreacion()
{
    return fechaCreacion;
}

void TADColumn::setNickCreacion(QString value)
{
    nickCreacion = value;
}

QString TADColumn::getNickCreacion()
{
    return nickCreacion;
}

void TADColumn::setFechaUltimoCambio(QString value)
{
    fechaUltimoCambio = value;
}

QString TADColumn::getFechaUltimoCambio()
{
    return fechaUltimoCambio;
}

void TADColumn::setNickUltimoCambio(QString value)
{
    nickUltimoCambio = value;
}

QString TADColumn::getNickUltimoCambio()
{
    return nickUltimoCambio;
}

void TADColumn::togleDisponible()
{
    disponible = !disponible;
}

bool TADColumn::isDisponible()
{
    return disponible;
}

void TADColumn::setFilepath(QString value)
{
    filepath = value;
}

QString TADColumn::getFilePath()
{
    return filepath;
}

int TADColumn::compare(TADColumn *column)
{
    return nombre.compare(column->getNombre());
}

ColumnList *TADColumn::getInternalColumn()
{
    return internalColumn;
}

MatrixNode *TADColumn::addInternalColumn(TADMatrixNode *value)
{
    MatrixNode *node = new MatrixNode(value);
    return internalColumn->insert(node);
}

MatrixNode *TADColumn::addInternalColumn(MatrixNode *value)
{
    return internalColumn->insert(value);
}

QString TADColumn::toString()
{
    QString text;
    text.append(nombre);

    return text;
}

QString TADColumn::getNodeName()
{
    QString name("Column");
    name.append(nombre);

    return name;
}

QString TADColumn::createNode()
{
    QString str;
    QTextStream createdNode(&str);
    createdNode << "\t" << getNodeName();
    createdNode << " [label = \"" << toString() << "\"];\n";

    return str;
}

QString TADColumn::pointNode(TADColumn *next)
{
    QString str;
    QTextStream nodePointers(&str);
    if (next != NULL)
    {
        nodePointers << "\t" << getNodeName() << " -> " << next->getNodeName() << "  [constraint = true];\n";
        nodePointers << "\t" << next->getNodeName() << " -> " << getNodeName() << "  [constraint = true];\n";
    }

    if (! internalColumn->isEmpty())
    {
        nodePointers << "\t" << getNodeName() << " -> " << internalColumn->front()->getNodeName() << "  [constraint = true];\n";
        nodePointers << internalColumn->graph() << "\n";
    }

    return str;
}
