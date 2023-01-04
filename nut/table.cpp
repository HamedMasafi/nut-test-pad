
#include <QJsonObject>
#include <QJsonValue>

#include "table.h"
#include "phrases/abstractfieldphrase.h"

namespace Nut {

Table<Type::Model>::Table(Database<Type::Model> *parent, const char *name)// : AbstractTableModel(parent, name), _name(name)
{
    qDebug() << "my name is" << name;
}

AbstractFieldPhrase *Nut::Table<Type::Model>::field(const QString &name) const
{
    for (auto i = _fields.begin(); i != _fields.end(); ++i)
        if ((*i)->name() == name)
            return *i;
    return nullptr;
}

void Table<Type::Data>::setFieldValue(const QString &name, const QVariant &value)
{
    if (_fields.contains(name))
        _fields.value(name)->fromVariant(value);
}

QVariant Table<Type::Data>::fieldValue(const QString &name) const
{
    if (!_fields.contains(name))
        return QVariant();
    return _fields.value(name)->toVariant();
}

QJsonObject Table<Type::Model>::toJson() const
{
    QJsonObject fieldsObject;
    for (auto i = _fields.begin(); i != _fields.end(); ++i) {
        QJsonObject fieldObject;
        fieldObject.insert("isKey", (*i)->isPrimaryKey());
        fieldObject.insert("autoIncrement",
                           QStringLiteral("%1,%2")
                               .arg((*i)->autoIncrementStart())
                               .arg((*i)->autoIncrementStep()));
        fieldObject.insert("len", (*i)->len());
        fieldObject.insert("maxlen", (*i)->maxLen());
        fieldObject.insert("columnName", (*i)->name());

        fieldsObject.insert(i.key(), fieldObject);
    }
    QJsonObject foreignKeysObject;
    for (auto i = _foreignKeys.begin(); i != _foreignKeys.end(); ++i) {
        QJsonObject foreignKeyObject;

//        foreignKeyObject.insert("name", (*i)->);
        foreignKeyObject.insert(i.key(), foreignKeyObject);
    }

    QJsonObject o;
    o.insert("fields", fieldsObject);
    o.insert("foreignKeys", foreignKeysObject);
    return o;
}

QVariant Table<Type::Data>::key() const
{
    return _fields.value(keyField)->toVariant();
}

void Table<Type::Data>::setKey(const QVariant &value)
{
    _fields.value(keyField)->fromVariant(value);
}

//const QMap<QString, AbstractFieldPhrase *> &Table<Type::Model>::fields() const
//{
//    return _fields;
//}

const QSet<QString> &Table<Type::Data>::changedFields() const
{
    return _changedFields;
}

const QMap<QString, AbstractFieldPhrase *> &Table<Type::Model>::fields() const
{
    return _fields;
}

const QMap<QString, ForeignKeyModelBase *> &Table<Type::Model>::foreignKeys() const
{
    return _foreignKeys;
}

RowStatus Table<Type::Data>::status() const
{
    return _status;
}

bool operator==(const TableModel &l, const TableModel &r)
{
    //TODO: implement me
    return false;
}

AbstractFieldPhrase *Nut::Table<Type::Model>::primaryField() const
{
    for (const auto &f:_fields)
        if (f->isPrimaryKey())
            return f;
    return nullptr;
}

Nut::Table<Type::Model>::Table(const char *name) : _name{name}
{

}

FieldBase *Table<Type::Data>::primaryField()
{
    if (!_primaryField)
        for (const auto &f: _fields)
            if (f) {
                _primaryField = f;
                break;
            }
    return _primaryField;
}


} // namespace Nut
