#ifndef FOREIGNKEY_H
#define FOREIGNKEY_H

#include <QVariant>
#include "qglobal.h"
#include "global.h"

namespace Nut {

class ForeignKeyBase
{
protected:
    TableRow *_parent;
    const char *_name;
    void setChanged();
public:
    ForeignKeyBase(TableRow *parent, const char *name);
    virtual void fromVariant(const QVariant& value) = 0;
    virtual QVariant toVariant() const = 0;
};

template <NUT_TABLE_TEMPLATE T, typename KeyType>
class ForeignKey : public ForeignKeyBase
{
    T<Type::Data>* _object{nullptr};
    KeyType _key;

public:
    ForeignKey(Table<Type::Data> *parent, const char *name)
        : ForeignKeyBase(parent, name)
    {
    }

    ForeignKey<T, KeyType> &operator=(T<Type::Data> *p);
    ForeignKey<T, KeyType> &operator=(const KeyType &id);
    T<Type::Data> *object() const {
        return _object;
    }

public:
    void fromVariant(const QVariant &value) override {
        setChanged();
        _key = value.value<KeyType>();
    }
    QVariant toVariant() const override
    {
        return QVariant::fromValue(_key);
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
    setChanged();
    _object = p;
    return *this;
}

template <NUT_TABLE_TEMPLATE T, typename KeyType>
Q_OUTOFLINE_TEMPLATE ForeignKey<T, KeyType> &ForeignKey<T, KeyType>::operator=(const KeyType &id)
{
    setChanged();
    _object = nullptr;
    _key = id;
    return *this;
}

}

#endif // FOREIGNKEY_H
