#include <QMetaObject>
#include <QMetaProperty>

#include "jsonparser.h"


void JsonParser::ParseObject(QString &jsonObject, QObject *obj, bool *ok)
{
    *ok = true;

    QTextStream stream(&jsonObject);
    QChar c;

    const QMetaObject *meta = obj->metaObject();

    QString propertyName;
    QString propertyType;
    QMetaProperty property;
    qint32 propertyNumber = -1;

    stream >> c;
    while(!stream.atEnd())
    {
        propertyName = getPropertyName(stream, ok);

        propertyNumber= meta->indexOfProperty(propertyName.toLatin1().data());
        if (propertyNumber == -1)
        {
            *ok = false;
            return;
        }

        property = meta->property(propertyNumber);
        propertyType = property.typeName();



        stream.skipWhiteSpace();
    }
}

QString JsonParser::getPropertyName(QTextStream &s, bool *ok)
{
    QString propertyName = QString();
    QChar c;
    s >> c;

    while(c != '\"' && !s.atEnd())
    {
        s >> c;
    }
    if (s.atEnd())
    {
        *ok = false;
        return propertyName;
    }

    s>>c;
    while(c != '\"' && !s.atEnd())
    {
        propertyName.append(c);
        s>>c;
    }
    if (s.atEnd())
    {
        *ok = false;
        return propertyName;
    }
    return propertyName;
}

QVariant JsonParser::getPropertyValue(QString propertyType, QTextStream &s)
{
    qint32 propertyTypeNumber = QMetaType::type(propertyType.toLatin1().data());
    bool ok;

    if (propertyTypeNumber == 0)
        return QVariant();


    if (propertyType == "QString")
    {
        s.skipWhiteSpace();
        QString value = getStringValue(s, &ok);
        if (!(ok))
            return QVariant();

        return QVariant(value);

    }
    else if (propertyType.contains("int"))
    {
        qint64 value;
        s >> value;
        return QVariant(value);
    }
    else if (propertyType.startsWith("QList<"))
    {
        QString wrapperType(propertyType);
        wrapperType = "VectorReflectionWrapper<" + wrapperType.remove(0,8);
        int wrapperTypeNumber = QMetaType::type(wrapperType.toLatin1().data());

        QObject *wrapper= (QObject*)QMetaType::construct(wrapperTypeNumber);


        QString itemType(propertyType);
        itemType = wrapperType.remove(0,8).trimmed();
        itemType.chop(1);
        itemType = itemType.trimmed();
        int itemTypeNumber = QMetaType::type(itemType.toLatin1().data());

        QChar c;
        s.skipWhiteSpace();
        s>>c;
        QVariant v;
        while(c != '[' && !s.atEnd())
        {            s.skipWhiteSpace();
            v = getPropertyValue(itemType, s);
            if(!v.isValid())
                return QVariant();

            qint32 methodNumber = wrapper->metaObject()->indexOfMethod("append");
            wrapper->metaObject()->method(methodNumber).invoke(wrapper,Q_ARG(v.value);

            s>>c;
        }
        if(s.atEnd())
            return QVariant();
        qint32 methodNumber = wrapper->metaObject()->indexOfMethod("data");
        wrapper->metaObject()->method(methodNumber).invoke(wrapper,v.value);

    }
}
