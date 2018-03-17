#include "columnlist.h"

MatrixNode *ColumnList::insert(MatrixNode *current, MatrixNode *value)
{
    if (current == NULL)
        return NULL;

    int compare = current->getData()->compareRow(value->getData());

    if (compare == 0)
        return NULL;
    else
    {
        if (compare > 0)
        {
            value->setBottom(current);
            value->setTop(current->getTop());
            current->getTop()->setBottom(value);
            current->setTop(value);
            count++;

            return value;
        }
        else
            return insert(current->getBottom(), value);
    }
}

MatrixNode *ColumnList::get(MatrixNode *current, TADMatrixNode *value)
{
    if (current == NULL)
        return NULL;

    int compare = current->getData()->compareRow(value);

    if (compare == 0)
    {
        /* COMPARAR NIVELES */
        return current;
    }
    else
    {
        if (compare < 0)
            return get(current->getBottom(), value);
        else
            return NULL;
    }
}

ColumnList::ColumnList()
{
    count = 0;
    head = NULL;
    tail = NULL;
}

ColumnList::~ColumnList()
{
    clear();
}

bool ColumnList::isEmpty()
{
    return (head == NULL);
}

int ColumnList::size()
{
    return count;
}

void ColumnList::clear()
{
    while (!isEmpty())
        removeFirst();
}

MatrixNode *ColumnList::push_front(MatrixNode *value)
{
    if (isEmpty())
        head = tail = value;
    else
    {
        value->setBottom(head);
        head->setTop(value);
        head = value;
    }

    count++;

    return value;
}

MatrixNode *ColumnList::push_back(MatrixNode *value)
{
    if (isEmpty())
        head = tail = value;
    else
    {
        value->setTop(tail);
        tail->setBottom(value);
        tail = value;
    }

    count++;

    return value;
}

MatrixNode *ColumnList::insert(MatrixNode *value)
{
    if (isEmpty())
    {
        head = tail = value;
        count++;

        return value;
    }
    else
    {
        if (head->getData()->compareRow(value->getData()) > 0)
            return push_front(value);
        else if (tail->getData()->compareRow(value->getData()) < 0)
            return push_back(value);
        else
            return insert(head, value);
    }
}

MatrixNode *ColumnList::removeFirst()
{
    if (isEmpty())
        return NULL;

    MatrixNode *temporal = head;
    head = temporal->getBottom();
    if (size() > 1)
        head->setTop(NULL);
    else
        tail = head;
    count--;

    return temporal;
}

MatrixNode *ColumnList::removeLast()
{
    if (isEmpty())
        return NULL;

    MatrixNode *temporal = tail;
    tail = temporal->getTop();
    if (size() > 1)
        tail->setBottom(NULL);
    else
        head = tail;
    count--;

    return temporal;
}

MatrixNode *ColumnList::removeOne(TADMatrixNode *value)
{
    if (head->getData()->compareRow(value) == 0)
        return removeFirst();
    if (tail->getData()->compareRow(value) == 0)
        return removeLast();

    if (head->getData()->compareRow(value) < 0
            && tail->getData()->compareRow(value) > 0)
    {
        MatrixNode *temporal = get(head->getBottom(), value);

        if (temporal == NULL)
            return NULL;

        if (temporal->getBottom() != NULL)
            temporal->getBottom()->setTop(temporal->getTop());
        if (temporal->getTop() != NULL)
            temporal->getTop()->setBottom(temporal->getBottom());

        temporal->setBottom(NULL);
        temporal->setTop(NULL);
        count--;

        return temporal;
    }
    else
        return NULL;
}

MatrixNode *ColumnList::first()
{
    return head;
}

MatrixNode *ColumnList::last()
{
    return tail;
}

TADMatrixNode *ColumnList::front()
{
    return head->getData();
}

TADMatrixNode *ColumnList::back()
{
    return tail->getData();
}

MatrixNode *ColumnList::get(TADMatrixNode *value)
{
    if (isEmpty())
        return NULL;

    if (head->getData()->compareRow(value) == 0)
        return head;
    if (tail->getData()->compareRow(value) == 0)
        return tail;

    if (head->getData()->compareRow(value) < 0
            && tail->getData()->compareRow(value) > 0)
        return get(head->getBottom(), value);
    else
        return NULL;
}

QString ColumnList::graph()
{
    QString listGraph("");

    MatrixNode *temporal = head;
    while (temporal != NULL)
    {
        listGraph.append(temporal->createNode());
        listGraph.append(temporal->pointNode(temporal->VERTICAL));

        temporal = temporal->getBottom();
    }

    return listGraph;
}
