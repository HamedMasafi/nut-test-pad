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
    virtual void fromJson(const QJsonObject &json) = 0;
    QString name() const;
    virtual QString className() const = 0;
    AbstractFieldPhrase *primaryField() const;

    bool operator==(const AbstractTableModel &other);
    bool operator!=(const AbstractTableModel &other);
};

class MockTableModel : public AbstractTableModel
{
    QMap<QString, AbstractFieldPhrase *> _fields;
    QMap<QString, ForeignKeyModelBase *> _foreignKeys;
    QString _className;
public:
    const QMap<QString, AbstractFieldPhrase *> &fields() const;
    const QMap<QString, ForeignKeyModelBase *> &foreignKeys() const;
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);
    QString className() const;

    friend class Database<Model>;
};
}
