#pragma once

#include "global.h"
#include "field2.h"
#include "models/fieldmodel.h"
#include "foreignkey.h"
#include "models/foreignkeymodel.h"
#include "runtimecheckers/fieldchecker.h"
#include "runtimecheckers/foreignkeychecker.h"
#include <QJsonObject>

#include "core/tablebase.h"
#include "phrase.h"

#define NUT_FORWARD_DECLARE_TABLE(name)                                                            \
    template<Nut::Type _Type>                                                                 \
    class name;                                                                                    \
    using name##Main = name<Nut::Type::Data>;                                                   \
    using name##Model = name<Nut::Type::Model>;

#define NUT_DEFINE_TABLE(name)                                                                     \
    template<Nut::Type _Type = Nut::Type::Data>                                            \
    class name : public Nut::Table<_Type>

#define Nut__Table Table<_Type>

namespace Nut {

class FieldBase;
class FieldModelBase;
class ForeignKeyModelBase;

template <typename T, int _Type, typename... Types>
struct PropertyTypeHelper {
    using type = void;
};

template <typename T, typename... Types>
struct PropertyTypeHelper<T, Type::Data, Types...> {
//    static_assert(Model::count<AllowNull, Types...> ==0 , "is zero");
    using type
        = ::Nut::Field<T, containsType<PrimaryKey, Types...>, containsType<AllowNull, Types...>>;
};

template <typename T, typename... Types>
struct PropertyTypeHelper<T, Type::Model, Types...> {
    using type = ::Nut::FieldPhrase<T>;
};

template <typename T, typename... Types>
struct PropertyTypeHelper<T, Type::FieldPhrases, Types...> {
    using type = void;
};

template <typename T, typename... Types>
struct PropertyTypeHelper<T, Type::RuntimeChecker, Types...> {
    using type = ::Nut::FieldChecker<T>;
};


template <NUT_TABLE_TEMPLATE T, typename KeyType, Nut::Type _Type>
struct ForeignKeyTypeHelper {
    using type = void;
};

template<NUT_TABLE_TEMPLATE T, typename KeyType>
struct ForeignKeyTypeHelper<T, KeyType, Type::Data> {
    using type = Nut::ForeignKey<T, KeyType>;
};

template<NUT_TABLE_TEMPLATE T, typename KeyType>
struct ForeignKeyTypeHelper<T, KeyType, Type::Model> {
    using type = Nut::ForeignKeyModel<T, KeyType>;
};

template<NUT_TABLE_TEMPLATE T, typename KeyType>
struct ForeignKeyTypeHelper<T, KeyType, Type::FieldPhrases> {
    using type = void;
};

template<NUT_TABLE_TEMPLATE T, typename KeyType>
struct ForeignKeyTypeHelper<T, KeyType, Type::RuntimeChecker> {
    using type = Nut::ForeignKeyChecker<T, KeyType>;
};

template<typename T, Type _Type, typename... Types>
struct Property2 {
    using type = void;
};

template<typename T, typename... Types>
struct Property2<T, Type::Data, Types...> //: public ::Nut::Field<T, Model::containsType<AllowNull, Types...>>
{
    using type = ::Nut::Field<T, containsType<AllowNull, Types...>>;

    Property2(TableRow *parent, const char *name, Types... args)
    // : ::Nut::Field<T, Model::containsType<AllowNull, Types...>>(parent, name, args...)
    {}
};
template<typename T, typename... Types>
struct Property2<T, Type::Model, Types...>// : public ::Nut::FieldModel<T>
{
    using type = ::Nut::FieldModel<T>;
    Property2(TableModel *parent, const char *name, Types &&...args)
        //: ::Nut::FieldModel<T>(parent, name, args...)
    {}
};
template<typename T, typename... Types>
struct Property2<T, Type::FieldPhrases, Types...> {
    using type = ::Nut::FieldModel<T>;
};
template<typename T, typename... Types>
struct Property2<T, Type::RuntimeChecker, Types...> {
    using type = ::Nut::FieldChecker<T>;
};

#define NUT_TABLE \
    template<typename T, typename... Types> \
    using Property = typename Nut::PropertyTypeHelper<T, _Type, Types...>::type; \
    template<NUT_TABLE_TEMPLATE T, typename KeyType> \
    using ForeignKeyProperty = typename Nut::ForeignKeyTypeHelper<T, KeyType, _Type>::type; \
\
public: \
    Nut::Table<Nut::Type::Model> *model() const; \
    QString className() const override; \
    static QString staticClassName(); \
\
private:

#define NUT_DECLARE_TABLE(table) \
    typedef table<Nut::Type::Data> table##Table; \
    typedef table<Nut::Type::Model> table##Model; \
    extern table<Nut::Type::Model> table##Model2; \
    namespace Nut { \
    template<> \
    table<Type::Model> &createModel<table>(); \
    } \
    template<Nut::Type _T> \
    QString table<_T>::className() const \
    { \
        return #table; \
    } \
    template<Nut::Type _T> \
    QString table<_T>::staticClassName() \
    { \
        return #table; \
    }

//    typedef name<Nut::Type::FieldPhrases> name##Fields;

#define NUT_IMPLEMENT_TABLE(name) \
    name<Nut::Type::Model> name##Model2; \
    namespace Nut { \
    template<> \
    name<Type::Model> &createModel<name>() \
    { \
        return name##Model2; \
    } \
    }

#define Field(type, name, ...)  Property<type> name{this, #name, __VA_ARGS__}
#define ForeignKey(type, keyType, name) ForeignKeyProperty<type, keyType> name{this, #name}


template<Type _Type>
class Table
{
public:
    Table() = default;
    virtual QString className() const = 0;
    friend class FieldBase;
};

template<>
class Table<Type::Data>
{
protected:
    QString keyField;
    QMap<QString, FieldBase*> _fields;
    QSet<QString> _changedFields;
    RowStatus _status{RowStatus::Added};
    FieldBase *_primaryField;
public:

    Table() = default;

    friend class FieldBase;
    virtual QString className() const = 0;

    void setFieldValue(const QString &name, const QVariant &value);
    QVariant fieldValue(const QString &name) const;

    QVariant key() const;
    void setKey(const QVariant &value);

    const QSet<QString> &changedFields() const;
    RowStatus status() const;
    FieldBase *primaryField();
};

template <>
class Table<Type::Model> //: public AbstractTableModel
{
protected:
    Database<Model> *_parentDatabase;
    QMap<QString, AbstractFieldPhrase*> _fields;
    QMap<QString, ForeignKeyModelBase*> _foreignKeys;
    QString _name;
public:
    Table() = default;
    Table(const char *name);
    Table(Database<Type::Model> *parent, const char *name);

    virtual QString className() const = 0;
    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    AbstractFieldPhrase *field(const QString &name) const;

    friend class DatasetBase;
    friend class AbstractFieldPhrase;
    friend class ForeignKeyModelBase;
    virtual QString name() const{
        return _name;
    }
    AbstractFieldPhrase *primaryField() const;
    const QMap<QString, AbstractFieldPhrase *> &fields() const ;
    const QMap<QString, ForeignKeyModelBase *> &foreignKeys() const ;

};

bool operator==(const TableModel &l, const TableModel &r);

template<NUT_TABLE_TEMPLATE T>
class ModelBase : public AbstractTableModel, public T<Type::Model>
{
    QString _name;
public:
    ModelBase(Database<Type::Model> *parent, const char *name)
        : AbstractTableModel(parent, name), T<Type::Model>(name), _name{name}
    {
        T<Type::Model>::_parentDatabase = parent;
        for (auto &f: T<Type::Model>::_fields) {
            qDebug() << "field" << f->data->fieldName<<f->data->tableName;
            f->data->tableName = name;
            f->data->className = name;
        }
    }
    virtual QString className() const override { return T<Type::Data>::staticClassName(); }
    const QMap<QString, AbstractFieldPhrase *> &fields() const override{
        return T<Type::Model>::_fields;
    }
    const QMap<QString, ForeignKeyModelBase*> &foreignKeys() const override{
        return T<Type::Model>::_foreignKeys;
    }
    QJsonObject toJson() const override
    {
        return T<Type::Model>::toJson();
    }
    void fromJson(const QJsonObject &json) override { return T<Type::Model>::fromJson(json); }
    virtual QString name() const override{
        return _name;
    }
};

} // namespace Nut
