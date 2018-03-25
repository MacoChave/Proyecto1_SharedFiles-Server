#include "matrixnode.h"

MatrixNode::MatrixNode()
{
    data = NULL;
    preview = NULL;
    next = NULL;
    top = NULL;
    bottom = NULL;
}

MatrixNode::MatrixNode(TADMatrixNode *value)
{
    data = value;
    preview = NULL;
    next = NULL;
    top = NULL;
    bottom = NULL;
}

MatrixNode::~MatrixNode()
{
    if (data != NULL)
        delete data;

    data = NULL;
    preview = NULL;
    next = NULL;
    top = NULL;
    bottom = NULL;
}

TADMatrixNode *MatrixNode::getData()
{
    return data;
}

void MatrixNode::setData(TADMatrixNode *value)
{
    data = value;
}

MatrixNode *MatrixNode::getPreview()
{
    return preview;
}

void MatrixNode::setPreview(MatrixNode *value)
{
    preview = value;
}

MatrixNode *MatrixNode::getNext()
{
    return next;
}

void MatrixNode::setNext(MatrixNode *value)
{
    next = value;
}

MatrixNode *MatrixNode::getTop()
{
    return top;
}

void MatrixNode::setTop(MatrixNode *value)
{
    top = value;
}

MatrixNode *MatrixNode::getBottom()
{
    return bottom;
}

void MatrixNode::setBottom(MatrixNode *value)
{
    bottom = value;
}

int MatrixNode::compareColumn(TADMatrixNode *value)
{
    return data->compareColumn(value);
}

int MatrixNode::compareRow(TADMatrixNode *value)
{
    return data->compareRow(value);
}

QString MatrixNode::getNodeName()
{
    return data->getNodeName();
}

QString MatrixNode::toString()
{
    return data->toString();
}

QString MatrixNode::createNode()
{
    return data->createNode();
}

QString MatrixNode::pointNode(int orientation)
{
    if (orientation == VERTICAL)
    {
        if (bottom != NULL)
            return data->pointNode(bottom->getData());
        else
            return "";
    }
    else
    {
        if (next != NULL)
            return data->pointNode(next->getData());
        else
            return "";
    }
}
