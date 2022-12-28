#pragma once

#include "global.h"
#include <QJsonObject>
#include <QString>

namespace Nut {

template<Nut::Type _Type>
class Database;
class AbstractFieldPhrase;
class ForeignKeyModelBase;

class AbstractModel
{
    QString _name;
public:
    AbstractModel(Database<Type::Model> *parent, const char *name);
    virtual const QMap<QString, AbstractFieldPhrase *> &fields() = 0;
    virtual const QMap<QString, ForeignKeyModelBase*> &foreignKeys() = 0;
    virtual QJsonObject toJson();
    QString name() const;
};

}
