#ifndef GLOBAL_H
#define GLOBAL_H

#define NUT_TABLE_TEMPLATE template<Nut::Type _T> class
#define NUT_TABLE_TEMPLATE_MAIN template<Nut::Type _T = Type::Data> class

namespace Nut {

enum Type { Data, Model, FieldPhrases, RuntimeChecker };

template<template<Nut::Type> class T>
T<Nut::Type::Model> *createModel()
{
    return nullptr;
}

template <Type _Type>
class Table;
using TableRow = Table<Type::Data>;
using TableModel = Table<Type::Model>;

template <Type _Type>
class Database;
using DatabaseMain = Database<Type::Data>;
using DatabaseModel = Database<Type::Model>;

template<template<Type _Type> class T>
using Table_T = T<Type::Data>;

#define NUT_ENTITY template<Type _Type>

enum class RowStatus {
    Added,
    Deleted,
    Modified,
    NewCreated,
    FetchedFromDB
};

template<template<Type _Type> class T>
T<Type::Model> &createModel()
{
    return {};
}

} // namespace Nut

#define NUT_EXPORT

#endif // GLOBAL_H
