#pragma once

#include "global.h"
#include <QJsonObject>
#include <QString>

namespace Nut {

template<Nut::Type _Type>
class Database;
class AbstractFieldPhrase;
class ForeignKeyModelBase;

class AbstractTableModel
{
    QString _name;
public:
    AbstractTableModel() = default;
    AbstractTableModel(Database<Type::Model> *parent, const char *name);
    AbstractFieldPhrase *field(const QString &name) const;
    virtual const QMap<QString, AbstractFieldPhrase *> &fields() const = 0;
    virtual const QMap<QString, ForeignKeyModelBase*> &foreignKeys() const = 0;
    virtual QJsonObject toJson() const = 0;
    QString name() const;
    virtual QString className() const = 0;
    AbstractFieldPhrase *primaryField() const;

    bool operator==(const AbstractTableModel &other);
    bool operator!=(const AbstractTableModel &other);
};

}
