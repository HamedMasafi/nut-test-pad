#pragma once

#include <QSharedData>
#include <QVariant>

namespace Nut {

class ExpressionData : public QSharedData
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

    const char *className;
    const char *fieldName;

    Type type;

    Condition operatorCond;

    ExpressionData *left;
    ExpressionData *right;

    QVariant operand;
    bool isNot;

    ExpressionData();
};

} // namespace Nut
