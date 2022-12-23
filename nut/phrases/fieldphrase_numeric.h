#pragma once

#include <type_traits>

#include "fieldphrase.h"

namespace Nut
{

#define SPECIALIZATION_NUMERIC_MEMBER(type, op, cond)                          \
    ConditionalPhrase operator op(const QVariant &other)                       \
{                                                                              \
    return ConditionalPhrase(this, cond, other);                               \
}

class NumericFieldPhrase : public AbstractFieldPhrase
{
public:
    PHRASE_CONSTRUCTOR(NumericFieldPhrase)

    NumericFieldPhrase(const char *className, const char *s) :
        AbstractFieldPhrase(className, s)
    {}

    ConditionalPhrase between(const QVariant &min, const QVariant &max)
    {
        return ConditionalPhrase(this, PhraseData::Between,
                                 QVariantList() << min << max);
    }
    ConditionalPhrase operator ++()
    {
        return ConditionalPhrase(this, PhraseData::Add, 1);
    }
    ConditionalPhrase operator --()
    {
        return ConditionalPhrase(this, PhraseData::Minus, 1);
    }
    ConditionalPhrase operator ++(int)
    {
        return ConditionalPhrase(this, PhraseData::Add, 1);
    }
    ConditionalPhrase operator --(int)
    {
        return ConditionalPhrase(this, PhraseData::Minus, 1);
    }

    SPECIALIZATION_NUMERIC_MEMBER(type, <,  PhraseData::Less)
    SPECIALIZATION_NUMERIC_MEMBER(type, <=, PhraseData::LessEqual)
    SPECIALIZATION_NUMERIC_MEMBER(type, >,  PhraseData::Greater)
    SPECIALIZATION_NUMERIC_MEMBER(type, >=, PhraseData::GreaterEqual)
    SPECIALIZATION_NUMERIC_MEMBER(type, %,  PhraseData::Mod)

    SPECIALIZATION_NUMERIC_MEMBER(type, +,  PhraseData::Add)
    SPECIALIZATION_NUMERIC_MEMBER(type, -,  PhraseData::Minus)
    SPECIALIZATION_NUMERIC_MEMBER(type, *,  PhraseData::Multiple)
    SPECIALIZATION_NUMERIC_MEMBER(type, /,  PhraseData::Divide)
};

#define SPECIALIZATION_NUMERIC_TYPE(type) \
    template<> \
    class FieldPhrase<type> : public NumericFieldPhrase \
    { \
    public: \
        template<typename... Types> \
        constexpr FieldPhrase(TableModel *parent, const char *name, Types... args) \
            : NumericFieldPhrase("", name) \
        { \
            init(parent, name, args...); \
        } \
        AssignmentPhrase operator=(const QVariant &other) \
        { \
            return AssignmentPhrase(this, other); \
        } \
        AssignmentPhrase operator=(ConditionalPhrase &&other) \
        { \
            return AssignmentPhrase(new PhraseData(data, PhraseData::Equal, other.data)); \
        } \
    };

SPECIALIZATION_NUMERIC_TYPE(qint8)
SPECIALIZATION_NUMERIC_TYPE(qint16)
SPECIALIZATION_NUMERIC_TYPE(qint32)
SPECIALIZATION_NUMERIC_TYPE(qint64)

SPECIALIZATION_NUMERIC_TYPE(quint8)
SPECIALIZATION_NUMERIC_TYPE(quint16)
SPECIALIZATION_NUMERIC_TYPE(quint32)
SPECIALIZATION_NUMERIC_TYPE(quint64)

SPECIALIZATION_NUMERIC_TYPE(qreal)
SPECIALIZATION_NUMERIC_TYPE(float)

}
