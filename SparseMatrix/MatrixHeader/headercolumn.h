#ifndef HEADERCOLUMN_H
#define HEADERCOLUMN_H
#include <Cabecera/list.h>

template <typename T>
class HeaderColumn : public List<T>
{
public:
    HeaderColumn() {}
    HeaderColumn(HeaderColumn<T> *value);
    ~HeaderColumn() {}

    void graph();
};
#endif // HEADERCOLUMN_H

template <typename T>
HeaderColumn<T>::HeaderColumn(HeaderColumn<T> *value) : List<T>(value)
{}

template <typename T>
void HeaderColumn<T>::graph()
{

    QString texto;
    return texto;
}
