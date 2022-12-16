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

#ifndef ABSTRACTFIELDPHRASE_H
#define ABSTRACTFIELDPHRASE_H

#include <QtNut/nut_global.h>
#include "namedtype.h"
#include "assignmentphrase.h"
#include "conditionalphrase.h"
#include "phraselist.h"
#include "../global.h"

QT_BEGIN_NAMESPACE

NUT_BEGIN_NAMESPACE

#define PHRASE_CONSTRUCTOR(cls) \
    template<typename... Types> \
    constexpr cls(TableModel *parent, const char *name, Types... args) \
        : AbstractFieldPhrase("", name) \
    { \
        using namespace Model; \
        static_assert(Model::no_unique<PrimaryKey, Types...>::value <= 1, \
                      "More than one PrimaryKey()"); \
        if (!pick<const char *>("Name", &data->fieldName, args...)) \
            data->fieldName = name; \
        pick<int>("MaxLen", &data->maxLen, args...); \
        pick<int>("Len", &data->len, args...); \
        pick<std::pair<int, int>>("AutoIncrement", &data->autoIncrement, args...); \
        pick<bool>("PrimaryKey", &data->isPrimaryKey, args...); \
        addToParent(name, parent); \
    }

class PhraseData;
class NUT_EXPORT AbstractFieldPhrase
{
public:
    PhraseData *data;
    explicit AbstractFieldPhrase(PhraseData *d);
    AbstractFieldPhrase(const char *fieldName);
    AbstractFieldPhrase(const char *className, const char *fieldName);
    AbstractFieldPhrase(const AbstractFieldPhrase &other);
    AbstractFieldPhrase(AbstractFieldPhrase &&other);

    const char *name() const;
    int maxLen() const;
    int len() const;
    bool isPrimaryKey() const;
    int autoIncrementStart() const;
    int autoIncrementStep() const;

    virtual ~AbstractFieldPhrase();

    PhraseList operator |(const AbstractFieldPhrase &other);

    template<typename T>
    ConditionalPhrase in(QList<T> list)
    {
        QVariantList vlist;
        for (auto &t: list)
            vlist.append(QVariant::fromValue(t));

        return ConditionalPhrase(this, PhraseData::In, vlist);
    }
#ifdef Q_COMPILER_INITIALIZER_LISTS
    ConditionalPhrase in(std::initializer_list<int> list) {
        QVariantList vlist;
        std::initializer_list<int>::iterator it;
        for (it = list.begin(); it != list.end(); ++it)
            vlist.append(*it);
        return ConditionalPhrase(this, PhraseData::In, vlist);
    }
#endif

    ConditionalPhrase isNull();

    ConditionalPhrase operator ==(const QVariant &other);
    ConditionalPhrase operator ==(const ConditionalPhrase &other);
    //why?
    ConditionalPhrase operator !=(const QVariant &other);

    ConditionalPhrase operator ==(const AbstractFieldPhrase &other);
    ConditionalPhrase operator !=(const AbstractFieldPhrase &other);
    ConditionalPhrase operator <(const AbstractFieldPhrase &other);
    ConditionalPhrase operator >(const AbstractFieldPhrase &other);
    ConditionalPhrase operator <=(const AbstractFieldPhrase &other);
    ConditionalPhrase operator >=(const AbstractFieldPhrase &other);

    AbstractFieldPhrase operator ~();
    AbstractFieldPhrase operator !();
    AssignmentPhrase operator =(const QVariant &other);
    AssignmentPhrase operator =(const ConditionalPhrase &other);
    AssignmentPhrase operator <<(const QVariant &other);

protected:
    void detach();
    template<typename... Types>
    constexpr void init(TableModel *parent, const char *name, Types... args)
    {
        using namespace Model;
        //        d = new FieldModelData();
        //        static_assert(containsType<PrimaryKey, Types...>, "no key");
        static_assert(Model::no_unique<PrimaryKey, Types...>::value <= 1, "More than one PrimaryKey()");
        //        static_assert(containsType<PrimaryKey, Types...>, "No PrimaryKey()");

        //        static_assert(count("MaxLen", args...) <= -100, "max len more than once");
        //        static_assert(contains<MaxLen, Types...>::value, "max len more than once");

        if (!pick<const char *>("Name", &data->fieldName, args...))
            data->fieldName = name;

        pick<int>("MaxLen", &data->maxLen, args...);
        pick<int>("Len", &data->len, args...);
        pick<std::pair<int, int> >("AutoIncrement", &data->autoIncrement, args...);
        pick<bool>("PrimaryKey", &data->isPrimaryKey, args...);

        addToParent(name, parent);

    }
    void addToParent(const QString &name, Table<TableTypeModel> *parent);
};

NUT_END_NAMESPACE

QT_END_NAMESPACE

#endif // ABSTRACTFIELDPHRASE_H
