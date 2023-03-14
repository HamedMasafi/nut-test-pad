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

#pragma once

#include "global.h"
#include "abstractfieldphrase.h"
#include "../global.h"
#include "../namedtype.h"

namespace Nut
{

template<class T>
class FieldPhraseSpecialMethods
{

};

template<typename T, typename... Types>
class FieldPhrase : public AbstractFieldPhrase, public FieldPhraseSpecialMethods<T>
{
public:
    NUT_DECLARE_METATYPE_ID(T)

    FieldPhrase(const char *className, const char *s) :
        AbstractFieldPhrase(className, s)
    {}

    constexpr FieldPhrase(TableModel *parent, ModelDeclartion::ColumnType<T>&&, Types... args)
        : AbstractFieldPhrase("", "name")
    {
        using namespace ModelDeclartion;
//        d = new FieldModelData();
        //        static_assert(containsType<PrimaryKey, Types...>, "no key");
        static_assert(no_unique<PrimaryKey, Types...>::value <= 1, "More than one PrimaryKey()");
        //        static_assert(containsType<PrimaryKey, Types...>, "No PrimaryKey()");

        //        static_assert(count("MaxLen", args...) <= -100, "max len more than once");
        //        static_assert(contains<MaxLen, Types...>::value, "max len more than once");

        char *tmpName;
        if (pick<char *>("Name", &tmpName, args...))
            data->fieldName = tmpName;
//        else
//            data->fieldName = name;

        pick<int>("MaxLen", &data->maxLen, args...);
        pick<int>("Len", &data->len, args...);
        pick<std::pair<int, int> >("AutoIncrement", &data->autoIncrement, args...);
        pick<bool>("PrimaryKey", &data->isPrimaryKey, args...);

        addToParent(tmpName, parent);

    }

    virtual ~FieldPhrase() {}

    AssignmentPhrase operator =(const QVariant &other);
    ConditionalPhrase operator ==(const QVariant &other);
};

template<typename T, typename... Types>
Q_OUTOFLINE_TEMPLATE AssignmentPhrase FieldPhrase<T, Types...>::operator =(const QVariant &other) {
    return AssignmentPhrase(this, other);
}

template<typename T, typename... Types>
Q_OUTOFLINE_TEMPLATE ConditionalPhrase FieldPhrase<T, Types...>::operator ==(const QVariant &other) {
    return ConditionalPhrase(this, PhraseData::Equal, other);
}

//Date and time
#define CONDITIONAL_VARIANT_METHOD(name, cond) \
    ConditionalPhrase name(int val) \
    { \
        return ConditionalPhrase(this, cond, val); \
    }


}
