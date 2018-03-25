#include "tadrow.h"

TADRow::TADRow()
{
    nombre = "";
    correo = "";
    nickname = "";
    password = "";
    internalRow = new RowList();
}

TADRow::TADRow(QString _nombre, QString _correo, QString _nickname, QString _password)
{
    nombre = _nombre;
    correo = _correo;
    nickname = _nickname;
    password = _password;
    internalRow = new RowList();
}

TADRow::TADRow(QString _nombre)
{
    nombre = _nombre;
    correo = "";
    nickname = "";
    password = "";
    internalRow = new RowList();
}

TADRow::~TADRow()
{
    nombre.clear();
    correo.clear();
    nickname.clear();
    password.clear();
    delete internalRow;
    internalRow = NULL;
}

void TADRow::setNombre(QString value)
{
    nombre = value;
}

QString TADRow::getNombre()
{
    return nombre;
}

void TADRow::setCorreo(QString value)
{
    correo = value;
}

QString TADRow::getCorreo()
{
    return correo;
}

void TADRow::setNickname(QString value)
{
    nickname = value;
}

QString TADRow::getNickname()
{
    return nickname;
}

void TADRow::setPassword(QString value)
{
    password = value;
}

QString TADRow::getPassword()
{
    return password;
}

int TADRow::compare(TADRow *row)
{
    return nickname.compare(row->getNickname());
}

int TADRow::comparePass(TADRow *row)
{
    return password.compare(row->getPassword());
}

RowList *TADRow::getInternalRow()
{
    return internalRow;
}

MatrixNode *TADRow::addInternalRow(MatrixNode *value)
{
    return internalRow->insert(value);
}

QString TADRow::toString()
{
    QString text;
    text.append(nickname);

    return text;
}

QString TADRow::getNodeName()
{
    QString name("Row");
    name.append(nickname);

    return name;
}

QString TADRow::createNode()
{
    QString str;
    QTextStream createdNode(&str);
    createdNode << "\t" << getNodeName();
    createdNode << " [label = \"" << toString() << "\"];\n";

    return str;
}

QString TADRow::pointNode(TADRow *next)
{
    QString str;
    QTextStream nodePointers(&str);
    if (next != NULL)
    {
        nodePointers << "\t" << getNodeName() << " -> " << next->getNodeName() << ";\n";
        nodePointers << "\t" << next->getNodeName() << " -> " << getNodeName() << ";\n";
    }

    if (! internalRow->isEmpty())
    {
        nodePointers << "\t" << getNodeName() << " -> " << internalRow->front()->getNodeName() << ";\n";
        nodePointers << internalRow->graph() << "\n";
    }

    return str;
}
