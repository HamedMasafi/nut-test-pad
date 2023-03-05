
#include <QJsonObject>
#include <QJsonValue>
#include <QSqlQuery>
#include <QSqlError>

#include "database/databasedata.h"
#include "generators/abstractsqlgenerator.h"

#include "table.h"
#include "phrases/abstractfieldphrase.h"

namespace Nut {

Table<Type::Model>::Table(Database<Type::Model> *parent, const char *name)
    : _parentDatabase(parent)
    , _name(name) // : AbstractTableModel(parent, name), _name(name)
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
        fieldObject.insert("allowNull", (*i)->allowNull());
        fieldObject.insert("isUnique", (*i)->isUnique());

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
    o.insert("className", className());
    o.insert("tableName", name());

    return o;
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
    for (const auto &f : _fields)
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
        for (const auto &f : std::as_const(_fields))
            if (f->isPrimary()) {
                _primaryField = f;
                break;
            }
    return _primaryField;
}

void Nut::Table<Type::Model>::fromJson(const QJsonObject &json)
{
    auto className = json.value("className").toString();
    auto tableName = json.value("tableName").toString();
    auto fieldsObject  =json.value("fields").toObject();
    for (auto v : fieldsObject) {
        QString name;
        AbstractFieldPhrase *field;
        auto o = v.toObject();

        field->data->isPrimaryKey = o.value("isKey").toBool();
        field->data->len = o.value("len").toInt();
        field->data->maxLen = o.value("maxlen").toInt();
        field->data->fieldName = o.value("columnName").toString().toStdString().data();

        //        fieldObject.insert("autoIncrement",
        //                           QStringLiteral("%1,%2")
        //                               .arg((*i)->autoIncrementStart())
        //                               .arg((*i)->autoIncrementStep()));
        //        fieldObject.insert("len", (*i)->len());
        //        fieldObject.insert("maxlen", (*i)->maxLen());
        //        fieldObject.insert("columnName", (*i)->name());

        _fields.insert(name, field);
    }
}

int Nut::Table<Type::Data>::save(Database<Data> *db, TableModel *model)
{
    //Q_D(Table);
    auto sql = db->generator()->saveRecord(this, this->className());
    QSqlQuery q = db->exec(sql);

    if (q.lastError().type() != QSqlError::NoError)
        qDebug() << "Error: " << q.lastError().text() << sql;
    if(status() == RowStatus::Added && model->isPrimaryKeyAutoIncrement()){
        setFieldValue(model->primaryField()->name(), q.lastInsertId());
    }

//    foreach(AbstractTableSet *ts, d->childTableSets)
//        ts->save(db);
    _status = RowStatus::FetchedFromDB;

    return q.numRowsAffected();
}

bool Nut::Table<Type::Model>::isPrimaryKeyAutoIncrement() const
{
    return std::any_of(_fields.begin(), _fields.end(), [](AbstractFieldPhrase *f) {
        return f->isPrimaryKey() && f->isAutoIncrement();
    });
}

} // namespace Nut
