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
    TableMain *_parent;
    QString _name;

protected:
    FieldBase(TableMain *parent, const char *name);
    void setChanged();

public:
    virtual QVariant toVariant() const = 0;
    virtual void fromVariant(const QVariant& value) = 0;
};

using namespace Nut::Model;
template <typename T, bool _AllowNull = true>
class Field : public FieldBase
{
    bool _isNull;
    T _value;

public:
    template<typename ...Types>
    constexpr inline Field(TableMain *parent, const char *name, ...)
        : FieldBase(parent, name)
    {
    }

    bool isNull() const;
    void setNull();
    void set(const T& newValue);
    T value() const;
    T operator()();

    Field<T, _AllowNull> &operator=(const T &value);
    Field<T, _AllowNull> &operator=(const DBNullType &);
    constexpr Field<T, _AllowNull> &operator=(const std::nullptr_t &);

    bool operator==(const std::nullptr_t &);

    operator T ();

    QVariant toVariant() const override {
        return QVariant(_value);
    }
    void fromVariant(const QVariant& value) override {
        _value = value.value<T>();
    }
};

#define FieldMethod(type)                                                                          \
    template<typename T, bool _AllowNull>                                                          \
    Q_OUTOFLINE_TEMPLATE type Field<T, _AllowNull>

#define FieldMethodSelf                                                                            \
    template<typename T, bool _AllowNull>                                                          \
    Q_OUTOFLINE_TEMPLATE Field<T, _AllowNull> &Field<T, _AllowNull>

FieldMethod(bool)::isNull() const {
    return _isNull;
}

FieldMethod(void)::setNull() {
    _value = T{};
    _isNull = true;
}

FieldMethod(void)::set(const T &newValue) {
    setChanged();
    _value = newValue;
    _isNull = false;
}

FieldMethod(T)::value() const {
    return _value;
}

FieldMethod(T)::operator()() {
    return _value;
}

FieldMethodSelf::operator=(const T &value)
{
    set(value);
    return *this;
}

FieldMethodSelf::operator=(const DBNullType &)
{
    setNull();
    return *this;
}
template<typename T, bool _AllowNull>
Q_OUTOFLINE_TEMPLATE constexpr Field<T, _AllowNull> &Field<T, _AllowNull>::operator=(const std::nullptr_t &)
{
    static_assert(_AllowNull, "Field dont allow null values");
    setNull();
    return *this;
}

template<typename T, bool _AllowNull>
Q_OUTOFLINE_TEMPLATE Field<T, _AllowNull>::operator T()
{
    return _value;
}

FieldMethod(bool)::operator==(const std::nullptr_t &)
{
    return _isNull;
}

template<typename T, bool _AllowNull = true>
QDebug operator<<(QDebug debug, const Field<T, _AllowNull> &f)
{
    if (f.isNull())
        return debug << "NULL";
    else
        return debug << f.value();
}

} // namespace Nut

#endif // FIELD2_H
