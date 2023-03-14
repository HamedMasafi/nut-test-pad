#pragma once

#include <QDebug>
#include <QObject>
#include "global.h"
#include "namedtype.h"

#define ALLOW_NULL_METHODS(type) typename std::enable_if<_AllowNull, type >::type

namespace Nut {

struct DBNullType {};

extern DBNullType DBNull;

template<typename T, typename... Types>
class Field;

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


class PrimaryKeyFieldBase {
protected:
    static constexpr bool _isPrimary{true};
public:
    constexpr bool isPrimary() { return true; }
};

class NoPrimaryKeyFieldBase {
protected:
    static constexpr bool _isPrimary{false};
public:
    constexpr bool isPrimary() { return false; }
};

template<typename T, typename... Types>
class AllowNullFieldBaseBase {
public:
    operator T() { return _value; }
    T value() const { return _value; }
    bool operator==(const T &value) { return _value == value; }
    void setValue(const T &value) { _value = value; }

protected:
    T _value;
};

template<typename T, typename... Types>
class NoAllowNullFieldBase : public AllowNullFieldBaseBase<T, Types...>
{
public:
    constexpr bool isPrimaryImpl() { return false; }
};

template<typename T, typename... Types>
class AllowNullFieldBase : public AllowNullFieldBaseBase<T, Types...>
{
public:
    constexpr bool isPrimaryImpl() { return true; }

    Field<T, Types...> &operator=(const DBNullType &);
    Field<T, Types...> &operator=(const std::nullptr_t &);

    bool operator==(const DBNullType &) { return _isNull; }
    bool operator==(const std::nullptr_t &) { return _isNull; }
    inline bool isNull() const { return _isNull; }
    void setNull() { _isNull = true; }

protected:
    bool _isNull;
};

template<bool is, typename... Types>
struct PrimaryKeyBaseChooser{
    using type = void;
};

template<typename... Types>
struct PrimaryKeyBaseChooser<false, Types...>
{
    using type = NoPrimaryKeyFieldBase;
};

template<typename... Types>
struct PrimaryKeyBaseChooser<true, Types...>
{
    using type = PrimaryKeyFieldBase;
};

template<bool is, typename T, typename... Types>
struct AllowNullBaseChooser
{
    using type = void;
};

template<typename T, typename... Types>
struct AllowNullBaseChooser<false, T, Types...>
{
    using type = NoAllowNullFieldBase<T, Types...>;
};

template<typename T, typename... Types>
struct AllowNullBaseChooser<true, T, Types...>
{
    using type = AllowNullFieldBase<T, Types...>;
};

template<typename T, typename... Types>
class Field : public FieldBase
    , public PrimaryKeyBaseChooser<ModelDeclartion::containsType<ModelDeclartion::PrimaryKey, Types...>, Types...>::type
    , public AllowNullBaseChooser<ModelDeclartion::containsType<ModelDeclartion::AllowNull, Types...>, T, Types...>::type
{
public:

    template<NUT_TABLE_TEMPLATE _ParentClass>
    Field(_ParentClass<Data> *parent, ModelDeclartion::ColumnType<T>&&, Types... args)
        : FieldBase(parent, "name")
    {}

    Field(const T &t, Types... args)
        : FieldBase(nullptr, "")
    {}

    Field<T, Types...> &operator=(const T &value)
    {
        this->setChanged();
//        _value = value;
        this->setValue(value);
        return *this;
    }
    QVariant toVariant() const override {
        return QVariant(this->value());
    }
    void fromVariant(const QVariant& value) override {
//        _value = value.value<T>();
        this->setValue(value.value<T>());
    }
    virtual bool isPrimary() override{
        return this->isPrimaryImpl();
    }

};
/*
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
    const int IsPrimary{true};

public:
    template<typename... Types>
    constexpr inline Field(TableRow *parent, const char *name, Types... args)
        : FieldBase(parent, name)
        , IsPrimary{Nut::containsType<PrimaryKey, Types...>}
    {
        qDebug() << "Is primary" << name << _IsPrimary << Nut::containsType<PrimaryKey, Types...>;
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
    template<typename... Types>
    constexpr inline Field(TableRow *parent, const char *name, ...)
        : Field<T, _IsPrimary, false>(parent, name)
        , _isNull{true}
    {
        qDebug() << "Is primary" << name << _IsPrimary;
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
*/
template<typename T, typename...Types>
QDebug operator<<(QDebug debug, const Field<T, Types...> &f)
{
    if (f.isNull())
        return debug << "NULL";
    else
        return debug << f.value();
}

} // namespace Nut

