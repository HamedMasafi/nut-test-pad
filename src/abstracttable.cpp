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
    auto myFields = fields();
    auto othersFields = other.fields();

    if (myFields.size() != othersFields.size())
        return false;

    for (auto &f : myFields ) {
        auto otherField = other.field(f->name());

        if (!otherField)
            return false;

//        if (*f != *otherField)
//            return false;
#define CHECK(x) \
    if (f->x() != otherField->x()) { \
        qDebug() << #x << "is different"; \
        return false; \
    }
        CHECK(allowNull)
        CHECK(isAutoIncrement)
        CHECK(isPrimaryKey)
        CHECK(isUnique)
        CHECK(len)
        CHECK(maxLen)
    }
    return true;
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

AbstractTableModel::AbstractTableModel(Table<Model> *parent, const char *name)
{

}

AbstractFieldPhrase *AbstractTableModel::field(const QString &name) const
{
    for (auto const &f : fields())
        if (f->name() == name)
            return f;
    return nullptr;
}

ForeignKeyModelBase *AbstractTableModel::foreignKey(const QString &name) const
{
    auto list = foreignKeys();
    auto i = std::find_if(list.begin(), list.end(), [&name](const ForeignKeyModelBase *f) {
        return f->name() == name;
    });
    if (i == list.end())
        return nullptr;
    return *i;
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
    _className = json.value("className").toString();
    _name = _tableName = json.value("tableName").toString();
    auto fieldsObject  =json.value("fields").toObject();
    for (auto v : fieldsObject) {
        auto field = new AbstractFieldPhrase(_tableName, v.toObject());

        _fields.insert(field->name(), field);
    }
}

QString MockTableModel::className() const
{
    return _className;
}

QString MockTableModel::name() const
{
    return _tableName;
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

