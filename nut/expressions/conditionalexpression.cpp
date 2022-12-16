#include "conditionalexpression.h"
#include "expressiondata.h"

#include <models/fieldmodel.h>

namespace Nut {

ConditionalExpression::ConditionalExpression()
    : d{nullptr}
{

}

ConditionalExpression::ConditionalExpression(FieldModelBase *f, ExpressionData::Condition cond)
{
    d = new ExpressionData;

//    (l->data, cond);
}

ConditionalExpression::ConditionalExpression(FieldModelBase *f, ExpressionData::Condition, const QVariant &v)
{

}

ConditionalExpression::ConditionalExpression(FieldModelBase *, ExpressionData::Condition, const FieldModelBase &v)
{

}

ConditionalExpression::ConditionalExpression(FieldModelBase *l, ExpressionData::Condition cond, ConditionalExpression &r)
{

}

ConditionalExpression::ConditionalExpression(ConditionalExpression *l, ExpressionData::Condition cond, const FieldModelBase &r)
{

}

ConditionalExpression::ConditionalExpression(ConditionalExpression *l, ExpressionData::Condition cond, const QVariant &r)
{

}

ConditionalExpression::ConditionalExpression(ConditionalExpression *l, ExpressionData::Condition cond, ConditionalExpression &r)
{

}

}
