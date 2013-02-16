#include <QMetaObject>
#include <QMetaProperty>

#include "jsonparser.h"

JsonParser::JsonParser()
{
}


void JsonParser::ParseObject(QString &jsonObject, QObject *obj, bool *ok)
{
    *ok = true;

    QTextStream stream(jsonObject);
    QChar c;

    QMetaObject *meta = obj->metaObject();

    QString propertyName;
    QString propertyType;
    qint32 propertyTypeNumber;
    QMetaProperty *property;
    int propertyIndex = -1;

    stream >> c;
    while(!stream.atEnd())
    {
        propertyName = getPropertyName(stream);



        stream.skipWhiteSpace();
    }
}

QString JsonParser::getPropertyName(QTextStream &s, bool *ok)
{
    QString propertyName;
    while(c != '\"')
    {
        s >> c;
    }
    s>>c;
    while(c != '\"')
    {
        propertyName.append(c);
        s>>c;
    }
    return propertyName;
}

QVariant JsonParser::getPropertyValue(QString propertyType, QTextStream &s)
{
    propertyNumber= meta->indexOfProperty(QByteArray(propertyName).data());

    if (propertyNumber = -1)
    {
        *ok = false;
        return;
    }

    property = meta->property(propertyNumber);
    propertyType = property->typeName();

    if (propertyType = "QString")
    {
        stream.skipWhiteSpace();
        QString value = getStringValue(stream, ok);
        if (!(*ok))
            return;

        obj->setProperty(QByteArray(propertyName).data(), QVariant(value));
    }
    else if (propertyType.contains("int"))
    {
        qint64 value;
        stream >> value;
        obj->setProperty(QByteArray(propertyName).data(), QVariant(value));
    }
    else if (propertyType == "QVector<QString>")
    {
        QVector<QString> value = getStringVector(stream, ok);
        if (!(*ok))
            return;
        propertyTypeNumber = QMetaType::type(QByteArray(propertyType).data());

        obj->setProperty(QByteArray(propertyName).data(), QVariant(propertyTypeNumber,value));
    }
    else if (propertyType == "QVector<qint64>")
    {
        QVector<QString> value = getStringVector(stream, ok);
        if (!(*ok))
            return;
        propertyTypeNumber = QMetaType::type(QByteArray(propertyType).data());

        obj->setProperty(QByteArray(propertyName).data(), QVariant(propertyTypeNumber,value));
    }
}
