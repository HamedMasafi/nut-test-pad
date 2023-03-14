#pragma once

#include "global.h"
#include "phrase.h"
#include "field2.h"
#include "namedtype.h"

namespace Nut {

template<NUT_TABLE_TEMPLATE T, typename KeyType>
class ForeignKey;

template<NUT_TABLE_TEMPLATE T, typename KeyType>
class ForeignKeyModel;

template<NUT_TABLE_TEMPLATE T>
class ModelBase;

template<NUT_TABLE_TEMPLATE T>
class Dataset;

template<NUT_TABLE_TEMPLATE C, Type _Type>
struct TableSetTypeHelper
{
    using type = void;
};

template<NUT_TABLE_TEMPLATE C>
struct TableSetTypeHelper<C, Type::Data>
{
    using type = Dataset<C>;
};

template<NUT_TABLE_TEMPLATE C>
struct TableSetTypeHelper<C, Type::Model>
{
    using type = ModelBase<C>;
};


template <typename T, int _Type, typename... Types>
struct PropertyTypeHelper {
    using type = void;
};

template <typename T, typename... Types>
struct PropertyTypeHelper<T, Type::Data, Types...> {
    //    static_assert(Model::count<AllowNull, Types...> ==0 , "is zero");
    using type = Nut::Field<T, Types...>;
    //        = Nut::Field<T, no_unique<PrimaryKey, Types...>::value, no_unique<AllowNull, Types...>::value>;
};

template <typename T, typename... Types>
struct PropertyTypeHelper<T, Type::Model, Types...> {
    using type = ::Nut::FieldPhrase<T>;
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

}
