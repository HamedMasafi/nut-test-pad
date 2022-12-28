#include "abstracttable.h"
#include "database.h"
#include "models/foreignkeymodel.h"
#include "phrases/abstractfieldphrase.h"


namespace Nut {

QString AbstractModel::name() const
{
    return _name;
}

AbstractModel::AbstractModel(Database<Type::Model> *parent, const char *name)
    : _name{name}
{
    parent->_tables.append(this);
}

QJsonObject AbstractModel::toJson()
{
    auto _fields = fields();
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

        fieldsObject.insert(i.key(), fieldObject);
    }
    auto _foreignKeys = foreignKeys();
    qDebug() << "_foreignKeys " << _foreignKeys.size();
    QJsonObject foreignKeysObject;
    for (auto i = _foreignKeys.begin(); i != _foreignKeys.end(); ++i) {
        QJsonObject foreignKeyObject;

        foreignKeyObject.insert("table", (*i)->tableName());
        foreignKeyObject.insert("keytype", (*i)->keyType());

        foreignKeysObject.insert(i.key(), foreignKeyObject);
    }

    QJsonObject o;
    o.insert("fields", fieldsObject);
    o.insert("foreignKeys", foreignKeysObject);
    return o;
}

}

