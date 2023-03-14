#pragma once

#include "global.h"
#include "abstractfieldphrase.h"
#include "fieldphrase.h"

namespace Nut
{

//template<typename... Types>
//class FieldPhrase<QString, Types...> : public AbstractFieldPhrase
//{
//public:
//    constexpr FieldPhrase(TableModel *parent,  ModelDeclartion::ColumnType<QString>&&, Types... args)
//        : AbstractFieldPhrase("", "name")
//    {
//        init(parent, "name", args...);
//    }

//    NUT_DECLARE_METATYPE_ID(QString)
//    ConditionalPhrase like(const QString &term)
//    {
//        return ConditionalPhrase(this, PhraseData::Like, term);
//    }

//    ConditionalPhrase contains(const QString &term)
//    {
//        return ConditionalPhrase(this,
//                                 PhraseData::Like,
//                                 QVariant(QStringLiteral("%") + term
//                                          + QStringLiteral("%")));
//    }

//    AssignmentPhrase operator=(const QVariant &v)
//    {
//        return AssignmentPhrase(this, v);
//    }
//};

template<>
class FieldPhraseSpecialMethods<QString>
{
public:
//    using Self = AbstractFieldPhrase;
//    Self *self() { return static_cast<Self *>(this); }
//    const Self *self() const { return static_cast<const Self *>(this); }

    ConditionalPhrase like(const QString &term)
    {
        auto self = reinterpret_cast<AbstractFieldPhrase*>(this);

        return ConditionalPhrase(self, PhraseData::Like, term);
    }

    ConditionalPhrase contains(const QString &term)
    {
        auto self = reinterpret_cast<AbstractFieldPhrase*>(this);
        return ConditionalPhrase(self,
                                 PhraseData::Like,
                                 QVariant(QStringLiteral("%") + term
                                          + QStringLiteral("%")));
    }

    AssignmentPhrase operator=(const QVariant &v)
    {
        auto self = reinterpret_cast<AbstractFieldPhrase*>(this);
        return AssignmentPhrase(self, v);
    }
};
}
