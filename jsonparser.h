#ifndef JSONPARSER_H
#define JSONPARSER_H
#include <QString>
#include <QObject>
#include <QVariant>
#include <QTextStream>

class JsonParser
{
public:
    static void ParseObject(QString &jsonObject, QObject* obj, bool* ok);
private:
    static QString getPropertyName(QTextStream& s);
    static QVariant getPropertyValue(QString propertyType, QTextStream &s);
  //  static void Parse
};

#endif // JSONPARSER_H
