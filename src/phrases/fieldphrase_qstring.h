#pragma once

#include "global.h"
#include "abstractfieldphrase.h"
#include "fieldphrase.h"

namespace Nut
{

template<>
class FieldPhrase<QString> : public AbstractFieldPhrase
{
public:
    template<typename... Types>
    constexpr FieldPhrase(TableModel *parent, const char *name, Types... args)
        : AbstractFieldPhrase("", name)
    {
        init(parent, name, args...);
    }

    NUT_DECLARE_METATYPE_ID(QString)
    ConditionalPhrase like(const QString &term)
    {
        return ConditionalPhrase(this, PhraseData::Like, term);
    }

    ConditionalPhrase contains(const QString &term)
    {
        return ConditionalPhrase(this,
                                 PhraseData::Like,
                                 QVariant(QStringLiteral("%") + term
                                          + QStringLiteral("%")));
    }

    AssignmentPhrase operator=(const QVariant &v)
    {
        return AssignmentPhrase(this, v);
    }
};

}
