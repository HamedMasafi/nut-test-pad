#include "abstracttable.h"
#include "database.h"
#include "models/foreignkeymodel.h"
#include "phrases/abstractfieldphrase.h"


namespace Nut {

QString AbstractTableModel::name() const
{
    return _name;
}

AbstractFieldPhrase *AbstractTableModel::primaryField() const
{
    for (auto const &f: fields())
        if (f->isPrimaryKey())
            return f;
    return nullptr;
}

bool AbstractTableModel::operator==(const AbstractTableModel &other)
{
    Q_UNUSED(other)
    return false;
}

bool AbstractTableModel::operator!=(const AbstractTableModel &other)
{
    return !(*this == other);
}

AbstractTableModel::AbstractTableModel(Database<Type::Model> *parent, const char *name)
    : _name{name}
{
    parent->_tables.append(this);
}

AbstractFieldPhrase *AbstractTableModel::field(const QString &name) const
{
    for (auto const &f : fields())
        if (f->name() == name)
            return f;
    return nullptr;
}

const QMap<QString, AbstractFieldPhrase *> &MockTableModel::fields() const
{
    return _fields;
}

const QMap<QString, ForeignKeyModelBase *> &MockTableModel::foreignKeys() const
{
    return _foreignKeys;
}

QJsonObject MockTableModel::toJson() const
{
    return {};
}

void MockTableModel::fromJson(const QJsonObject &json)
{
    auto className = json.value("className").toString();
    auto tableName = json.value("tableName").toString();
    auto fieldsObject  =json.value("fields").toObject();
    for (auto v : fieldsObject) {
        auto field = new AbstractFieldPhrase(tableName, v.toObject());

        _fields.insert(field->name(), field);
    }
}

QString MockTableModel::className() const
{
    return _className;
}

//QJsonObject AbstractModel::toJson()
//{
//    auto _fields = fields();
//    QJsonObject fieldsObject;
//    for (auto i = _fields.begin(); i != _fields.end(); ++i) {
//        QJsonObject fieldObject;
//        fieldObject.insert("isKey", (*i)->isPrimaryKey());
//        fieldObject.insert("autoIncrement",
//                           QStringLiteral("%1,%2")
//                               .arg((*i)->autoIncrementStart())
//                               .arg((*i)->autoIncrementStep()));
//        fieldObject.insert("len", (*i)->len());
//        fieldObject.insert("maxlen", (*i)->maxLen());
//        fieldObject.insert("columnName", (*i)->name());
//        fieldObject.insert("allowNull", (*i)->allowNull());

//        fieldsObject.insert(i.key(), fieldObject);
//    }
//    auto _foreignKeys = foreignKeys();
//    qDebug() << "_foreignKeys " << _foreignKeys.size();
//    QJsonObject foreignKeysObject;
//    for (auto i = _foreignKeys.begin(); i != _foreignKeys.end(); ++i) {
//        QJsonObject foreignKeyObject;

//        foreignKeyObject.insert("table", (*i)->tableName());
//        foreignKeyObject.insert("keytype", (*i)->keyType());

//        foreignKeysObject.insert(i.key(), foreignKeyObject);
//    }

//    QJsonObject o;
//    o.insert("fields", fieldsObject);
//    o.insert("foreignKeys", foreignKeysObject);
//    return o;
//}

}

