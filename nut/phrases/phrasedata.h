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
#include <QString>
#include <QVariant>

namespace Nut
{

class NUT_EXPORT PhraseData
{
public:
    enum Condition {
        NotAssign = 0,
        Equal,
        Less,
        LessEqual,
        Null,
        In,
        Like,

        Not = 10,
        NotEqual,
        GreaterEqual,
        Greater,
        NotNull,
        NotIn,
        NotLike,

        And = 20,
        Or,

        Add,
        Minus,
        Multiple,
        Divide,
        Mod,

        Between,

        //date and time
        AddYears,
        AddMonths,
        AddDays,
        AddHours,
        AddMinutes,
        AddSeconds,

        // sqlite need to know works with qdate, qtime or qdatetime
        AddYearsDateTime,
        AddMonthsDateTime,
        AddDaysDateTime,
        AddHoursDateTime,
        AddMinutesDateTime,
        AddSecondsDateTime,

        DatePartYear,
        DatePartMonth,
        DatePartDay,
        DatePartHour,
        DatePartMinute,
        DatePartSecond,
        DatePartMilisecond
        //        // special types
        //        Distance
    };

    enum Type { Field, WithVariant, WithOther, WithoutOperand };

    QString tableName;
    const char *className;
    const char *fieldName;

    Type type;

    Condition operatorCond;

    PhraseData *left;
    PhraseData *right;

    QVariant operand;
    bool isNot;
    //    quint16 parents;

    int maxLen{};
    int len{};
    bool isPrimaryKey{false};
    bool allowNull;
    bool isUnique;
    std::pair<int, int> autoIncrement{std::make_pair(0, 0)};

    mutable QAtomicInt ref;

    PhraseData();
    PhraseData(const char *className, const char *fieldName);
    PhraseData(PhraseData *l, Condition o);
    PhraseData(PhraseData *l, Condition o, PhraseData *r);
    PhraseData(PhraseData *l, Condition o, QVariant r);

    virtual ~PhraseData();

    PhraseData *operator =(PhraseData *other);
    PhraseData &operator =(PhraseData &other);

    QString toString() const;

    void cleanUp();
    PhraseData *clone() const;
private:
    void cleanUp(PhraseData *d);
};

}
