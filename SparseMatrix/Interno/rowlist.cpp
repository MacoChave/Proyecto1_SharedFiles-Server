#include "rowlist.h"

MatrixNode *RowList::insert(MatrixNode *current, MatrixNode *value)
{
    if (current == NULL)
        return NULL;

    int compare = current->getData()->compareColumn(value->getData());

    if (compare == 0)
        return current;
    else
    {
        if (compare > 0)
        {
            value->setNext(current);
            value->setPreview(current->getPreview());
            current->getPreview()->setNext(value);
            current->setPreview(value);
            count++;

            return value;
        }
        else
            return insert(current->getNext(), value);
    }
}

MatrixNode *RowList::get(MatrixNode *current, TADMatrixNode *value)
{
    if (current == NULL)
        return NULL;

    int compare = current->getData()->compareColumn(value);

    if (compare == 0)
        return current;
    else
    {
        if (compare < 0)
            return get(current->getNext(), value);
        else
            return NULL;
    }
}

RowList::RowList()
{
    count = 0;
    head = NULL;
    tail = NULL;
}

RowList::~RowList()
{
    clear();
}

bool RowList::isEmpty()
{
    return (head == NULL);
}

int RowList::size()
{
    return count;
}

void RowList::clear()
{
    while (!isEmpty())
    {
        MatrixNode *temporal = NULL;
        temporal = removeFirst();
        delete temporal;
    }
}

MatrixNode *RowList::push_front(MatrixNode *value)
{
    if (isEmpty())
        head = tail = value;
    else
    {
        value->setNext(head);
        head->setPreview(value);
        head = value;
    }

    count++;

    return value;
}

MatrixNode *RowList::push_back(MatrixNode *value)
{
    if (isEmpty())
        head = tail = value;
    else
    {
        value->setPreview(tail);
        tail->setNext(value);
        tail = value;
    }

    count++;

    return value;
}

MatrixNode *RowList::insert(MatrixNode *value)
{
    if (isEmpty())
    {
        head = tail = value;
        count++;

        return value;
    }
    else
    {
        if (head->getData()->compareColumn(value->getData()) > 0)
            return push_front(value);
        else if (tail->getData()->compareColumn(value->getData()) < 0)
            return push_back(value);
        else
            return insert(head, value);
    }
}

MatrixNode *RowList::removeFirst()
{
    if (isEmpty())
        return NULL;

    MatrixNode *temporal = head;
    head = temporal->getNext();
    if (size() > 1)
        head->setPreview(NULL);
    else
        tail = head;
    count--;

    return temporal;
}

MatrixNode *RowList::removeLast()
{
    if (isEmpty())
        return NULL;

    MatrixNode *temporal = tail;
    tail = temporal->getPreview();
    if (size() > 1)
        tail->setNext(NULL);
    else
        head = tail;
    count--;

    return temporal;
}

MatrixNode *RowList::removeOne(TADMatrixNode *value)
{
    if (head->getData()->compareColumn(value) == 0)
        return removeFirst();
    if (tail->getData()->compareColumn(value) == 0)
        return removeLast();

    if (head->getData()->compareColumn(value) < 0
            && tail->getData()->compareColumn(value) > 0)
    {
        MatrixNode *temporal = get(head->getNext(), value);

        if (temporal == NULL)
            return NULL;

        if (temporal->getNext() != NULL)
            temporal->getNext()->setPreview(temporal->getPreview());
        if (temporal->getPreview() != NULL)
            temporal->getPreview()->setNext(temporal->getNext());

        temporal->setNext(NULL);
        temporal->setPreview(NULL);
        count--;

        return temporal;
    }
    else
        return NULL;
}

MatrixNode *RowList::first()
{
    return head;
}

MatrixNode *RowList::last()
{
    return tail;
}

TADMatrixNode *RowList::front()
{
    return head->getData();
}

TADMatrixNode *RowList::back()
{
    return tail->getData();
}

MatrixNode *RowList::get(TADMatrixNode *value)
{
    if (isEmpty())
            return NULL;

    if (head->getData()->compareColumn(value) == 0)
        return head;
    if (tail->getData()->compareColumn(value) == 0)
        return tail;

    if (head->getData()->compareColumn(value) < 0
            && tail->getData()->compareColumn(value) > 0)
        return get(head->getNext(), value);
    else
        return NULL;
}

QString RowList::graph()
{
    QString listGraph;

    MatrixNode *temporal = head;
    while (temporal != NULL)
    {
        listGraph.append(temporal->pointNode(temporal->HORIZONTAL));

        temporal = temporal->getNext();
    }

    return listGraph;
}
