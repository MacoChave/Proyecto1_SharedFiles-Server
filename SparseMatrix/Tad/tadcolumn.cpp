#include "tadcolumn.h"

TADColumn::TADColumn()
{
    nombre = "";
    tipo = 0;
    fechaCreacion = "";
    nickCreacion = "";
    fechaUltimoCambio = "";
    nickUltimoCambio = "";
    disponible = true;
    filepath = "";
    internalColumn = new ColumnList();
}

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
    fechaUltimoCambio = _fecha;
    nickUltimoCambio = _nickDuenio;
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

void TADColumn::setTipo(QString tipo)
{
    if (tipo.compare("documento") == 0)
        setTipo(DOCUMENTO);
    else if (tipo.compare("lienzo") == 0)
        setTipo(LIENZO);
    else
        setTipo(PRESENTACION);
}

void TADColumn::setTipo(int value)
{
    tipo = value;
}

int TADColumn::getTipo()
{
    return tipo;
}

QString TADColumn::getTipoString()
{
    if (tipo == DOCUMENTO)
        return "documento";
    else if (tipo == PRESENTACION)
        return "presentacion";
    else
        return "lienzo";
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

void TADColumn::setDisponible()
{
    disponible = true;
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

MatrixNode *TADColumn::addInternalColumn(MatrixNode *value)
{
    return internalColumn->insert(value);
}

MatrixNode *TADColumn::removeInternalColumn(TADMatrixNode *value)
{
    return internalColumn->removeOne(value);
}

bool TADColumn::eraseInternalColumn(TADMatrixNode *value)
{
    MatrixNode *node = removeInternalColumn(value);
    if (node != NULL)
    {
        delete node;
        node = NULL;

        return true;
    }

    return false;
}

QString TADColumn::getList()
{
    return internalColumn->getList();
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
