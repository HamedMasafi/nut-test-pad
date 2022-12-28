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
};

using namespace Nut::ModelDeclartion;

template <typename T, bool _AllowNull = true>
class Field : public FieldBase
{
};

template <typename T>
class Field<T, false> : public FieldBase
{
protected:
    T _value;

public:
    template<typename ...Types>
    constexpr inline Field(TableRow *parent, const char *name, ...)
        : FieldBase(parent, name)
    {
    }

    Field<T, false> &operator=(const T &value);

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

template<typename T>
Field<T, false>::operator T()
{
    return _value;
}

template<typename T>
T Nut::Field<T, false>::value() const
{
    return _value;
}

template <typename T>
class Field<T, true> : public Field<T, false>
{
//    T _value;
    bool _isNull;
public:
    template<typename ...Types>
    constexpr inline Field(TableRow *parent, const char *name, ...)
        : Field<T, false>(parent, name)
    {
    }

    Field<T, true> &operator=(const T &value);
    Field<T, true> &operator=(const DBNullType &);
    Field<T, true> &operator=(const std::nullptr_t &);

    bool operator==(const DBNullType &);
    bool operator==(const std::nullptr_t &);
    inline bool isNull() const { return _isNull; }
    void setNull();
};

template<typename T>
bool Field<T, true>::operator==(const std::nullptr_t &)
{
    return _isNull;
}

template<typename T>
Field<T, true> &Nut::Field<T, true>::operator=(const DBNullType &)
{
    this->setChanged();
    setNull();
    return *this;
}

template<typename T>
Field<T, true> &Field<T, true>::operator=(const T &value)
{
    this->setChanged();
    this->_value = value;
    return *this;
}



#define FieldMethod(type)                                                                          \
    template<typename T, bool _AllowNull>                                                          \
    Q_OUTOFLINE_TEMPLATE type Field<T, _AllowNull>

#define FieldMethodSelf                                                                            \
template<typename T, bool _AllowNull>                                                          \
    Q_OUTOFLINE_TEMPLATE Field<T, _AllowNull> &Field<T, _AllowNull>

#define FieldMethodSelfAllowNull \
    template<typename T> \
    Q_OUTOFLINE_TEMPLATE Field<T, true> &Field<T, true>

#define FieldMethodSelfNotNull \
    template<typename T> \
    Q_OUTOFLINE_TEMPLATE Field<T, false> &Field<T, false>

FieldMethodSelfNotNull::operator=(const T &value)
{
    this->setChanged();
    _value = value;
    return *this;
}

FieldMethodSelfAllowNull::operator=(const std::nullptr_t &)
{
    setNull();
    this->setChanged();
    return *this;
}

template<typename T>
bool Field<T, true>::operator==(const DBNullType &)
{
    return _isNull;
}

template<typename T>
void Field<T, true>::setNull()
{
    _isNull = true;
}

template<typename T>
QDebug operator<<(QDebug debug, const Field<T, true> &f)
{
    if (f.isNull())
        return debug << "NULL";
    else
        return debug << f.value();
}

template<typename T>
QDebug operator<<(QDebug debug, const Field<T, false> &f)
{
    return debug << f.value();
}


} // namespace Nut

#endif // FIELD2_H
