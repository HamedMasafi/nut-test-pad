#pragma once

#include "expressiondata.h"

#include <QSharedDataPointer>
#include <QVariant>

class ExpressionData;

namespace Nut {

class FieldModelBase;
class ConditionalExpression
{
    QSharedDataPointer<ExpressionData> d;
public:
    ConditionalExpression();
    ConditionalExpression(FieldModelBase *f, ExpressionData::Condition cond);
    ConditionalExpression(FieldModelBase *f, ExpressionData::Condition, const QVariant &v);
    ConditionalExpression(FieldModelBase *, ExpressionData::Condition, const FieldModelBase &v);
    ConditionalExpression(FieldModelBase *l, ExpressionData::Condition cond, ConditionalExpression &r);
    ConditionalExpression(ConditionalExpression *l, ExpressionData::Condition cond, const FieldModelBase  &r);
    ConditionalExpression(ConditionalExpression *l, ExpressionData::Condition cond, const QVariant &r);
    ConditionalExpression(ConditionalExpression *l, ExpressionData::Condition cond, ConditionalExpression &r);
};

} // namespace Nut

