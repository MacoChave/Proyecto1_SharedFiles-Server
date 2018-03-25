#include "tadmatrixnode.h"

TADMatrixNode::TADMatrixNode()
{
    nickname = "";
    archivo = "";
    permiso = -1;
}

TADMatrixNode::TADMatrixNode(QString _nickname, QString _archivo, int _permiso)
{
    nickname = _nickname;
    archivo = _archivo;
    permiso = _permiso;
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
    QString str(permiso);

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
