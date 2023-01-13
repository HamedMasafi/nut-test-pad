#ifndef FOREIGNKEY_H
#define FOREIGNKEY_H

#include "qglobal.h"
#include "global.h"

namespace Nut {

template <NUT_TABLE_TEMPLATE T, typename KeyType>
class ForeignKey
{
    T<Type::Data>* _object{nullptr};
    KeyType _key;

public:
    ForeignKey(Table<Type::Data> *parent, const char*)
    {
        Q_UNUSED(parent)
    }

    ForeignKey<T, KeyType> &operator=(T<Type::Data> *p);
    ForeignKey<T, KeyType> &operator=(const KeyType &id);
    T<Type::Data> *object() const {
        return _object;
    }
};


template <NUT_TABLE_TEMPLATE T, typename KeyType>
class ForeignKeyPhrase
{
public:
    T<Type::FieldPhrases>* _object{nullptr};
    KeyType _key;

    ForeignKeyPhrase(const char*)
    {}

};


template <NUT_TABLE_TEMPLATE T, typename KeyType>
Q_OUTOFLINE_TEMPLATE ForeignKey<T, KeyType> &ForeignKey<T, KeyType>::operator=(T<Type::Data> *p)
{
    _object = p;
    return *this;
}

template <NUT_TABLE_TEMPLATE T, typename KeyType>
Q_OUTOFLINE_TEMPLATE ForeignKey<T, KeyType> &ForeignKey<T, KeyType>::operator=(const KeyType &id)
{
    _object = nullptr;
    _key = id;
    return *this;
}

}

#endif // FOREIGNKEY_H
