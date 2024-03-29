#ifndef FIELD2_H
#define FIELD2_H

#include <QDebug>
#include <QObject>
#include "global.h"
#include "namedtype.h"

#define ALLOW_NULL_METHODS(type) typename std::enable_if<_AllowNull, type >::type

namespace Nut {

struct DBNullType {};

extern DBNullType DBNull;

class FieldBase {
    TableRow *_parent;
    QString _name;

protected:
    FieldBase(TableRow *parent, const char *name);
    void setChanged();

public:
    virtual QVariant toVariant() const = 0;
    virtual void fromVariant(const QVariant& value) = 0;
    virtual bool isPrimary() = 0;
};

using namespace Nut::ModelDeclartion;

template <typename T, bool _IsPrimary, bool _AllowNull = true>
class Field : public FieldBase
{
};

template <typename T, bool _IsPrimary>
class Field<T, _IsPrimary, false> : public FieldBase
{
protected:
    T _value;

public:
    template<typename ...Types>
    constexpr inline Field(TableRow *parent, const char *name, ...)
        : FieldBase(parent, name)
    {
    }

    bool isPrimary() override { return _IsPrimary; }

    Field<T, _IsPrimary, false> &operator=(const T &value);

    operator T ();
    T value() const;

    bool operator==(const T &);

    QVariant toVariant() const override {
        return QVariant(_value);
    }
    void fromVariant(const QVariant& value) override {
        _value = value.value<T>();
    }
};

#define FieldMethod(type, allowNull) \
    template<typename T, bool _IsPrimary> \
    Q_OUTOFLINE_TEMPLATE type Field<T, _IsPrimary, allowNull>

#define FieldMethodSelf(_AllowNull) \
    template<typename T, bool _IsPrimary> \
    Q_OUTOFLINE_TEMPLATE Field<T, _IsPrimary, _AllowNull> &Field<T, _IsPrimary, _AllowNull>

#define FieldMethodSelfAllowNull \
    template <typename T, bool _IsPrimary> \
    Q_OUTOFLINE_TEMPLATE Field<T, true> &Field<T, true>

#define FieldMethodSelfNotNull \
    template <typename T, bool _IsPrimary> \
    Q_OUTOFLINE_TEMPLATE Field<T, false> &Field<T, false>


template <typename T, bool _IsPrimary>
Field<T, _IsPrimary, false>::operator T()
{
    return _value;
}

template <typename T, bool _IsPrimary>
T Nut::Field<T, _IsPrimary, false>::value() const
{
    return _value;
}

template<typename T, bool _IsPrimary>
class Field<T, _IsPrimary, true> : public Field<T, _IsPrimary, false>
{
//    T _value;
    bool _isNull;
public:
    template<typename ...Types>
    constexpr inline Field(TableRow *parent, const char *name, ...)
        : Field<T, _IsPrimary, false>(parent, name)
    {
    }

    bool isPrimary() override { return _IsPrimary; }

    Field<T, _IsPrimary, true> &operator=(const T &value);
    Field<T, _IsPrimary, true> &operator=(const DBNullType &);
    Field<T, _IsPrimary, true> &operator=(const std::nullptr_t &);

    bool operator==(const DBNullType &);
    bool operator==(const std::nullptr_t &);
    inline bool isNull() const { return _isNull; }
    void setNull();
};

FieldMethod(bool, true)::operator==(const std::nullptr_t &)
{
    return _isNull;
}

FieldMethodSelf(true)::operator=(const DBNullType &)
{
    this->setChanged();
    setNull();
    return *this;
}

FieldMethodSelf(true)::operator=(const T &value)
{
    this->setChanged();
    this->_value = value;
    return *this;
}



FieldMethodSelf(false)::operator=(const T &value)
{
    this->setChanged();
    _value = value;
    return *this;
}

FieldMethodSelf(true)::operator=(const std::nullptr_t &)
{
    setNull();
    this->setChanged();
    return *this;
}

FieldMethod(bool, true)::operator==(const DBNullType &)
{
    return _isNull;
}

FieldMethod(void, true)::setNull()
{
    _isNull = true;
}

template <typename T, bool _IsPrimary>
QDebug operator<<(QDebug debug, const Field<T, true> &f)
{
    if (f.isNull())
        return debug << "NULL";
    else
        return debug << f.value();
}

template <typename T, bool _IsPrimary>
QDebug operator<<(QDebug debug, const Field<T, false> &f)
{
    return debug << f.value();
}


} // namespace Nut

#endif // FIELD2_H
