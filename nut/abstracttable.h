#pragma once

#include "global.h"
#include <QJsonObject>
#include <QString>

namespace Nut {

template<Nut::TableType _Type>
class Database2;
class AbstractFieldPhrase;
class ForeignKeyModelBase;

class AbstractModel
{
    QString _name;
public:
    AbstractModel(Database2<TableTypeModel> *parent, const char *name);
    virtual const QMap<QString, AbstractFieldPhrase *> &fields() = 0;
    virtual const QMap<QString, ForeignKeyModelBase*> &foreignKeys() = 0;
    virtual QJsonObject toJson();
    QString name() const;
};

}
