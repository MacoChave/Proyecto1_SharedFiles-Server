#include "tadmatrixnode.h"

TADMatrixNode::TADMatrixNode()
{
    nickname = "";
    archivo = "";
    permiso = -1;
}

TADMatrixNode::TADMatrixNode(QString _nickname, QString _archivo, QString _permiso)
{
    nickname = _nickname;
    archivo = _archivo;
    permiso = _permiso;
}

TADMatrixNode::TADMatrixNode(QString _nickname, QString _archivo, QString _permiso)
{
    nickname = _nickname;
    archivo = _archivo;
    setArchivo(_permiso);
}

TADMatrixNode::~TADMatrixNode()
{
    nickname.clear();
    archivo.clear();
    permiso = -1;
}

QString TADMatrixNode::getNickname()
{
    return nickname;
}

void TADMatrixNode::setNickname(QString value)
{
    nickname = value;
}

QString TADMatrixNode::getArchivo()
{
    return archivo;
}

void TADMatrixNode::setArchivo(QString value)
{
    archivo = value;
}

int TADMatrixNode::getPermiso()
{
    return permiso;
}

QString TADMatrixNode::getPermisoString()
{
    if (permiso == DUENIO)
        return "Dueño";
    else if (permiso == EDITAR)
        return "Editar";
    else
        return "Ver";
}

void TADMatrixNode::setPermiso(QString value)
{
    if (value.compare("dueño") == 0)
        setPermiso(DUENIO);
    else if (value.compare("editar") == 0)
        setPermiso(EDITAR);
    else
        setPermiso(VER);
}

void TADMatrixNode::setPermiso(int value)
{
    permiso = value;
}

int TADMatrixNode::compareColumn(TADMatrixNode *value)
{
    return archivo.compare(value->getArchivo());
}

int TADMatrixNode::compareRow(TADMatrixNode *value)
{
    return nickname.compare(value->getNickname());
}

QString TADMatrixNode::getNodeName()
{
    QString str = "";
    QTextStream name(&str);
    name << "node_";
    name << archivo << "_";
    name << nickname;

    return str;
}

QString TADMatrixNode::toString()
{
    QString str;

    switch (permiso) {
    case DUENIO:
        str = "Dueño";
        break;
    case VER:
        str = "Ver";
        break;
    case EDITAR:
        str = "Editar";
        break;
    }

    return str;
}

QString TADMatrixNode::createNode()
{
    QString str = "";
    QTextStream createdNode(&str);
    createdNode << "\t" << getNodeName();
    createdNode << " [label = \"" << toString() << "\"];\n";

    return str;
}

QString TADMatrixNode::pointNode(TADMatrixNode *next)
{
    QString str = "";
    QTextStream nodePointers(&str);
    if (next != NULL)
    {
        nodePointers << "\t" << getNodeName() << " -> " << next->getNodeName() << "  [constraint = true];\n";
        nodePointers << "\t" << next->getNodeName() << " -> " << getNodeName() << "  [constraint = true];\n";
    }

    return str;
}
