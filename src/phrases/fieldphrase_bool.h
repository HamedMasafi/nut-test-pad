#pragma once

#include "global.h"
#include "phrases/fieldphrase.h"
#include "phrases/abstractfieldphrase.h"
//#include "table.h"

namespace Nut
{

template<>
class FieldPhrase<bool> : public AbstractFieldPhrase
{
    TableModel *_parent;
public:
    NUT_DECLARE_METATYPE_ID(bool)

    template<typename... Types>
        constexpr FieldPhrase(TableModel *parent, const char *name, Types... args)
        : AbstractFieldPhrase("", name), _parent(parent)
    {
        init(parent, name, args...);
    }

    AssignmentPhrase operator =(const bool &other) {
        return AssignmentPhrase(this, other);
    }

//    FieldPhrase<bool> operator !()
//    {
////        auto c  = data->clone()
//        FieldPhrase<bool> f(_parent, data->className, data->fieldName);
//        f.data = new PhraseData(data);
//        f.data->isNot = !data->isNot;
//        return f;
//    }

    operator ConditionalPhrase()
    {
        return ConditionalPhrase(this, PhraseData::Equal, !data->isNot);
    }
};

}
