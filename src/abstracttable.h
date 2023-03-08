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
protected:
    QString _name;
public:
    AbstractTableModel() = default;
    AbstractTableModel(Database<Type::Model> *parent, const char *name);
    AbstractTableModel(Table<Type::Model> *parent, const char *name);
    AbstractFieldPhrase *field(const QString &name) const;
    ForeignKeyModelBase* foreignKey(const QString &name) const;

    virtual const QMap<QString, AbstractFieldPhrase *> &fields() const = 0;
    virtual const QMap<QString, ForeignKeyModelBase*> &foreignKeys() const = 0;
    virtual QJsonObject toJson() const = 0;
    virtual void fromJson(const QJsonObject &json) = 0;
    virtual QString name() const;
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
    QString _tableName;
public:
    const QMap<QString, AbstractFieldPhrase *> &fields() const override;
    const QMap<QString, ForeignKeyModelBase *> &foreignKeys() const override;
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject &json) override;
    QString className() const override;
    QString name() const override;

    friend class Database<Model>;
};
}
