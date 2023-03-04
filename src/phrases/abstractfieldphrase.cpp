/**************************************************************************
**
** This file is part of Nut project.
** https://github.com/HamedMasafi/Nut
**
** Nut is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Nut is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Nut.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#include "abstractfieldphrase.h"
#include <QDebug>
#include "../table.h"

namespace Nut
{

AbstractFieldPhrase::AbstractFieldPhrase(PhraseData *d) : data(d)
{ }

AbstractFieldPhrase::AbstractFieldPhrase(const QString &tableName, const QJsonObject &json) : data{new PhraseData}
{
    data->tableName = tableName.toUtf8().data();
    data->isPrimaryKey = json.value("isKey").toBool();
    data->len = json.value("len").toInt();
    data->maxLen = json.value("maxlen").toInt();
    data->fieldName = json.value("columnName").toString().toStdString().data();
    auto parts = json.value("autoIncrement").toString().split(",");
    if (parts.size() == 2)
        data->autoIncrement = std::make_pair(parts.at(0).toInt(), parts.at(1).toInt());

}

AbstractFieldPhrase::AbstractFieldPhrase(const char *fieldName)
    : data(new PhraseData("", fieldName))
{

}

AbstractFieldPhrase::AbstractFieldPhrase(const QString &className,
                                         const QString &fieldName)
    :data(new PhraseData(className, fieldName))
{
}

AbstractFieldPhrase::AbstractFieldPhrase(const AbstractFieldPhrase &other)
{
    data = other.data;
    data->ref.ref();
}

AbstractFieldPhrase::AbstractFieldPhrase(AbstractFieldPhrase &&other)
{
    data = other.data;
    other.data = nullptr;
}

QMetaType::Type AbstractFieldPhrase::metaTypeId() const
{
    return QMetaType::Void;
}

AbstractFieldPhrase::~AbstractFieldPhrase()
{
    if (data) {
        if (!data->ref.deref()) {
//            qDebug() << "deleted" << data->className
//                     << data->fieldName;
            delete data;
        }
//        else
//            qDebug() << "more parents for" << data->className
//                     << data->fieldName;
    }
}

PhraseList AbstractFieldPhrase::operator |(const AbstractFieldPhrase &other)
{
    return PhraseList(this, other);
}

ConditionalPhrase AbstractFieldPhrase::isNull()
{
    return ConditionalPhrase(this, PhraseData::Null);
}


ConditionalPhrase AbstractFieldPhrase::operator ==(const ConditionalPhrase
                                                   &other)
{
    return ConditionalPhrase(this, PhraseData::Equal,
                             const_cast<ConditionalPhrase&>(other));
}


AssignmentPhrase AbstractFieldPhrase::operator =(const QVariant &other)
{
    return AssignmentPhrase(this, other);
}

AssignmentPhrase AbstractFieldPhrase::operator =(const ConditionalPhrase &other)
{
    return AssignmentPhrase(new PhraseData(data, PhraseData::Equal, other.data));
}

AssignmentPhrase AbstractFieldPhrase::operator <<(const QVariant &other)
{
    return AssignmentPhrase(this, other);
}

void AbstractFieldPhrase::detach()
{
    auto clone = data->clone();
    if (!data->ref.deref())
        delete data;
    data = clone;
}

void AbstractFieldPhrase::addToParent(const QString &name, Table<Type::Model> *parent)
{
    parent->_fields.insert(name, this);
    data->tableName = parent->name();
}

#define AbstractFieldPhraseOperatorVariant(class, op, cond) \
ConditionalPhrase class::operator op(const QVariant &other) \
{ \
    return ConditionalPhrase(this, cond, other); \
}

AbstractFieldPhraseOperatorVariant(AbstractFieldPhrase, ==, PhraseData::Equal)
AbstractFieldPhraseOperatorVariant(AbstractFieldPhrase, !=, PhraseData::NotEqual)

#define AbstractFieldPhraseOperatorField(op, cond) \
ConditionalPhrase AbstractFieldPhrase::operator op(const AbstractFieldPhrase &other) \
{ \
    return ConditionalPhrase(this, cond, other); \
}

AbstractFieldPhraseOperatorField(==, PhraseData::Equal)
AbstractFieldPhraseOperatorField(!=, PhraseData::NotEqual)
AbstractFieldPhraseOperatorField(< , PhraseData::Less)
AbstractFieldPhraseOperatorField(<=, PhraseData::LessEqual)
AbstractFieldPhraseOperatorField(> , PhraseData::Greater)
AbstractFieldPhraseOperatorField(>=, PhraseData::GreaterEqual)

AbstractFieldPhrase AbstractFieldPhrase::operator ~()
{
    AbstractFieldPhrase f(data->className, data->fieldName);
    f.data->isNot = !data->isNot;
    return f;
}

AbstractFieldPhrase AbstractFieldPhrase::operator !()
{
    AbstractFieldPhrase f(data->clone());
    f.data->isNot = !data->isNot;
    return f;
}


const char *AbstractFieldPhrase::name() const
{
    return data->fieldName.toUtf8().data();
}

QString AbstractFieldPhrase::tableName() const
{
    return data->tableName;
}

int AbstractFieldPhrase::maxLen() const
{
    return data->maxLen;
}

int AbstractFieldPhrase::len() const
{
    return data->len;
}

bool AbstractFieldPhrase::isPrimaryKey() const
{
    return data->isPrimaryKey;
}

bool AbstractFieldPhrase::isAutoIncrement() const
{
    return data->autoIncrement != std::make_pair(-1, -1);
}

int AbstractFieldPhrase::autoIncrementStart() const
{
    return data->autoIncrement.first;
}

int AbstractFieldPhrase::autoIncrementStep() const
{
    return data->autoIncrement.second;
}

bool AbstractFieldPhrase::allowNull() const
{
    return data->allowNull;
}

bool AbstractFieldPhrase::isUnique() const
{
    return data->isUnique;
}

QJsonObject AbstractFieldPhrase::toJson() const
{
    QJsonObject fieldObject;
    fieldObject.insert("isKey", isPrimaryKey());
    fieldObject.insert("autoIncrement",
                       QStringLiteral("%1,%2")
                           .arg(autoIncrementStart())
                           .arg(autoIncrementStep()));
    fieldObject.insert("len", len());
    fieldObject.insert("maxlen", maxLen());
    fieldObject.insert("columnName", name());
    return fieldObject;
}

void AbstractFieldPhrase::fromJson(const QString &tableName, const QJsonObject &json)
{
    data->tableName = tableName.toUtf8().data();
    data->isPrimaryKey = json.value("isKey").toBool();
    data->len = json.value("len").toInt();
    data->maxLen = json.value("maxlen").toInt();
    data->fieldName = json.value("columnName").toString().toStdString().data();
    auto parts = json.value("autoIncrement").toString().split(",");
    if (parts.size() == 2)
        data->autoIncrement = std::make_pair(parts.at(0).toInt(), parts.at(1).toInt());
}

} // namespace Nut
