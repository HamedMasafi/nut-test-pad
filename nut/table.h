#pragma once

#include "global.h"
#include "field2.h"
#include "models/fieldmodel.h"
#include "foreignkey.h"
#include "models/foreignkeymodel.h"
#include "runtimecheckers/fieldchecker.h"
#include "runtimecheckers/foreignkeychecker.h"
#include <QJsonObject>

#include "design/field.h"
#include "core/tablebase.h"
#include "phrase.h"

#define NUT_FORWARD_DECLARE_TABLE(name)                                                            \
    template<Nut::TableType _Type>                                                                 \
    class name;                                                                                    \
    using name##Main = name<Nut::TableTypeMain>;                                                   \
    using name##Model = name<Nut::TableTypeModel>;

#define NUT_DEFINE_TABLE(name)                                                                     \
    template<Nut::TableType _Type = Nut::TableTypeMain>                                            \
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
struct PropertyTypeHelper<T, TableTypeMain, Types...> {
//    static_assert(Model::count<AllowNull, Types...> ==0 , "is zero");
    using type = ::Nut::Field<T, Model::containsType<AllowNull, Types...>>;
};

template <typename T, typename... Types>
struct PropertyTypeHelper<T, TableTypeModel, Types...> {
    using type = ::Nut::FieldPhrase<T>;
};

template <typename T, typename... Types>
struct PropertyTypeHelper<T, TableTypeFieldPhrases, Types...> {
    using type = void;
};

template <typename T, typename... Types>
struct PropertyTypeHelper<T, RuntimeChecker, Types...> {
    using type = ::Nut::FieldChecker<T>;
};


template <NUT_TABLE_TEMPLATE T, typename KeyType, Nut::TableType _Type>
struct ForeignKeyTypeHelper {
    using type = void;
};

template<NUT_TABLE_TEMPLATE T, typename KeyType>
struct ForeignKeyTypeHelper<T, KeyType, TableTypeMain> {
    using type = Nut::ForeignKey<T, KeyType>;
};

template<NUT_TABLE_TEMPLATE T, typename KeyType>
struct ForeignKeyTypeHelper<T, KeyType, TableTypeModel> {
    using type = Nut::ForeignKeyModel<T, KeyType>;
};

template<NUT_TABLE_TEMPLATE T, typename KeyType>
struct ForeignKeyTypeHelper<T, KeyType, TableTypeFieldPhrases> {
    using type = void;
};

template<NUT_TABLE_TEMPLATE T, typename KeyType>
struct ForeignKeyTypeHelper<T, KeyType, RuntimeChecker> {
    using type = Nut::ForeignKeyChecker<T, KeyType>;
};

template<typename T, TableType _Type, typename... Types>
struct Property2 {
    using type = void;
};

template<typename T, typename... Types>
struct Property2<T, TableTypeMain, Types...> //: public ::Nut::Field<T, Model::containsType<AllowNull, Types...>>
{
    using type = ::Nut::Field<T, Model::containsType<AllowNull, Types...>>;

    Property2(TableMain *parent, const char *name, Types... args)
    // : ::Nut::Field<T, Model::containsType<AllowNull, Types...>>(parent, name, args...)
    {}
};
template<typename T, typename... Types>
struct Property2<T, TableTypeModel, Types...>// : public ::Nut::FieldModel<T>
{
    using type = ::Nut::FieldModel<T>;
    Property2(TableModel *parent, const char *name, Types &&...args)
        //: ::Nut::FieldModel<T>(parent, name, args...)
    {}
};
template<typename T, typename... Types>
struct Property2<T, TableTypeFieldPhrases, Types...> {
    using type = ::Nut::FieldModel<T>;
};
template<typename T, typename... Types>
struct Property2<T, RuntimeChecker, Types...> {
    using type = ::Nut::FieldChecker<T>;
};

#define NUT_TABLE                                                                                  \
    template<typename T, typename... Types>                                                        \
    using Property = typename Nut::PropertyTypeHelper<T, _Type, Types...>::type;                   \
    template<NUT_TABLE_TEMPLATE T, typename KeyType>                                               \
    using ForeignKeyProperty = typename Nut::ForeignKeyTypeHelper<T, KeyType, _Type>::type;

#define NUT_DECLARE_TABLE(name)                                                                    \
    typedef name<Nut::TableTypeMain> name##Table;                                                  \
    typedef name<Nut::TableTypeModel> name##Model;                                                 \
    extern name<Nut::RuntimeChecker> name##Checker;                                                \
    extern name<Nut::TableTypeModel> name##Model2;                                                 \
    namespace Nut {                                                                                \
    template<>                                                                                     \
    name<Nut::TableTypeModel> *createModel<name>();                                                \
    }

//    typedef name<Nut::TableTypeFieldPhrases> name##Fields;

#define NUT_IMPLEMENT_TABLE(name)                                                                  \
    name<Nut::RuntimeChecker> name##Checker;                                                       \
    name<Nut::TableTypeModel> name##Model2;                                                        \
    namespace Nut {                                                                                \
    template<>                                                                                     \
    name<Nut::TableTypeModel> *createModel<name>()                                                 \
    {                                                                                              \
        return &name##Model2;                                                                      \
    }                                                                                              \
    }

#define Field(type, name, ...)  Property<type> name{this, #name, __VA_ARGS__}
#define ForeignKey(type, keyType, name) ForeignKeyProperty<type, keyType> name{this, #name}

}
