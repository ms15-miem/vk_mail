#ifndef VECTORREFLECTIONWRAPPER_H
#define VECTORREFLECTIONWRAPPER_H

#include <QObject>

template <class T>
class VectorReflectionWrapper : public QObject
{
    Q_OBJECT
private:
    QVector<T> *data;
public:
    explicit VectorReflectionWrapper(QObject *parent = 0);
    ~VectorReflectionWrapper();
    void append(T elem);
    QVector<T> getData()const;
signals:
    
public slots:
    
};

#endif // VECTORREFLECTIONWRAPPER_H
