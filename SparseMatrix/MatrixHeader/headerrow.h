#ifndef HEADERROW_H
#define HEADERROW_H
#include <Cabecera/list.h>

template <typename T>
class HeaderRow : public List<T>
{
public:
    HeaderRow() {}
    HeaderRow(HeaderRow<T> *value);
    ~HeaderRow() {}

    void graph();
};
#endif // HEADERROW_H

HeaderRow<T>::HeaderRow(HeaderRow<T> *value) : List<T>(value)
{}

void HeaderRow::graph()
{
    QString texto;
    return texto;
}
