#include "matrix.h"

QString Matrix::getRankRow(MatrixNode *currentNode)
{
    QString texto;
    while (currentNode != NULL)
    {
        texto.append("; ");
        texto.append(currentNode->getNodeName());

        currentNode = currentNode->getNext();
    }

    return texto;
}

Matrix::Matrix()
{
    headerColumns = new List<TADColumn *>();
    headerRows = new List<TADRow *>();
}

Matrix::~Matrix()
{
    clear();
}

List<TADColumn *> *Matrix::getHeaderColumn()
{
    return headerColumns;
}

List<TADRow *> *Matrix::getHeaderRow()
{
    return headerRows;
}

/***********************************************************************************
 * MODIFICADORES DE COLUMNA
 **********************************************************************************/
bool Matrix::insertColumn(TADColumn *value)
{
    if (getColumn(value) == NULL)
    {
        headerColumns->insert(value);
        return true;
    }
    else
        return false;
}

bool Matrix::eraseColumn(TADColumn *value)
{
    Node<TADColumn *> *node = NULL;
    node = headerColumns->removeOne(value);
    node->getData()->getInternalColumn()->clear();


}

Node<TADColumn *> *Matrix::getColumn(TADColumn *value)
{
    Node<TADColumn *> node = NULL;
    node = headerColumns->get(value);
    return node;
}

/***********************************************************************************
 * MODIFICADORES DE FILA
 **********************************************************************************/
bool Matrix::insertRow(TADRow *value)
{
    if (getRow(value) == NULL)
    {
        headerRows->insert(value);
        return true;
    }
    else
        return false;
}

Node<TADRow *> *Matrix::getRow(TADRow *value)
{
    Node<TADRow *> node = NULL;
    node = headerRows->get(value);
    return node;
}

/***********************************************************************************
 * MODIFICADORES DE NODO MATRIZ
 **********************************************************************************/
bool Matrix::insertMatrixNode(QString user, QString filename, QString permiso)
{
    TADRow *tadRow = new TADRow(user);
    TADColumn *tadColumn = new TADColumn(filename);

    Node<TADRow *> row = getRow(tadRow);
    Node<TADColumn *> column = getColumn(tadColumn);

    delete tadRow;
    delete tadColumn;

    if (row == NULL || column == NULL)
        return false;

    TADMatrixNode *tadMatrixNode = new TADMatrixNode(user, filename, permiso);
    MatrixNode *matrixNode = new MatrixNode(tadMatrixNode);
    tadRow = row.getData();
    tadColumn = column.getData();

    bool sobreescritura = false;

    sobreescritura = tadRow->addInternalRow(matrixNode);
    sobreescritura = tadColumn->addInternalColumn(matrixNode);

    if (sobreescritura)
        delete matrixNode;

    tadRow = NULL;
    tadColumn = NULL;
    matrixNode = NULL;

    return true;
}

bool Matrix::eraseMatrixNode(QString user, QString filename)
{
    TADRow *tadRow = new TADRow(user);
    TADColumn *tadColumn = new TADColumn(filename);

    Node<TADRow *> row = getRow(tadRow);
    Node<TADColumn *> column = getColumn(tadColumn);

    delete tadRow;
    delete tadColumn;

    if (row == NULL || column = NULL)
        return false;

    tadRow = row.getData();
    tadColumn = column.getData();
    TADMatrixNode *tadMatrixNode = new TADMatrixNode(user, filename, permiso);
    MatrixNode *matrixNode = NULL;

    tadRow->removeInternalRow(tadMatrixNode);
    matrixNode = tadColumn->removeInternalColumn(tadMatrixNode);

    if (matrixNode != NULL)
        delete matrixNode;

    tadRow = NULL;
    tadColumn = NULL;
    matrixNode = NULL;

    return true;
}

QString Matrix::getUserListMatrixNode(QString filename)
{
    QString listUserName;
    TADColumn *tadColumn = new TADColumn(filename);
    Node<TADColumn *> column = NULL;

    column = getColumn(tadColumn);

    delete tadColumn;

    if (column != NULL)
        return listUserName;

    tadColumn = column.getData();

    listUserName = tadColumn->getList();

    return listUserName;
}

/***********************************************************************************
 * OPERADORES
 **********************************************************************************/
void Matrix::graph(QString filename)
{
    if (headerColumns->isEmpty() || headerRows->isEmpty())
        return;

    QFile file(filename + ".dot");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << "digraph " << filename << " {\n";
        out << "\tnode[shape = box]\n";
        flush(out);

        out << "\tnodeMatrix [label=\"COLUMNAS\\nFILAS\"];\n";
        out << "\tnodeMatrix -> " << headerColumns->front()->getNodeName() << ";\n";
        out << "\tnodeMatrix -> " << headerRows->front()->getNodeName() << " [constraint = true];\n";

        /* GRAFICAR CABECERAS CON SUS LISTAS INTERNAS */
        out << headerColumns->graph();
        flush(out);

        /* GRAFICAR FILAS CON SUS LISTAS INTERNAS */
        out << headerRows->graph();
        flush(out);

        /* GRAFICAR RANK DE FILAS */
        Node<TADRow *> *currentRow = headerRows->first();
        while (currentRow != NULL)
        {
            out << "\t{ rank = same; " << currentRow->getNameNode();
            flush(out);
            out << getRankRow(currentRow->getData()->getInternalRow()->first());
            flush(out);
            out << "}\n";
            flush(out);

            currentRow = currentRow->getNext();
        }

        Node<TADColumn *> *currentColumn = headerColumns->first();
        out << "\t{ rank = same; nodeMatrix";
        flush(out);
        while (currentColumn != NULL)
        {
            out << " ; " << currentColumn->getNameNode();
            currentColumn = currentColumn->getNext();
        }
        out << " }\n";

        out << "}";
        flush(out);

        file.close();

        QString compilarDot;
        QTextStream outCompile(&compilarDot);
        outCompile << "dot -Tpng " << filename << ".dot -o " << filename << ".png";
        flush(outCompile);

        qDebug() << compilarDot;
        system(compilarDot.toLatin1().data());
        compilarDot.clear();
    }
}

void Matrix::clear()
{
    qInfo() << "Limpiando memoria de matriz :)";
    delete headerColumns;
    delete headerRows;
    qInfo() << "Memoria de matriz limpia :D";
}
