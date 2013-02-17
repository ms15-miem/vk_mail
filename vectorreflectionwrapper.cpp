#include "vectorreflectionwrapper.h"

template <class T>
VectorReflectionWrapper<T>::VectorReflectionWrapper<T>(QObject *parent) :
    QObject(parent)
{
    data = new QVector<T>();
}

template <class T>
VectorReflectionWrapper<T>::~VectorReflectionWrapper<T>()
{
    delete data;
}

template <class T>
void VectorReflectionWrapper<T>::append(T elem)
{
    data<<elem;
}

template <class T>
QVector<T> VectorReflectionWrapper<T>::getData() const
{
    return *data;
}
