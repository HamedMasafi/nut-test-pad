#ifndef GLOBAL_H
#define GLOBAL_H

#define NUT_TABLE_TEMPLATE template<Nut::Type _T> class
#define NUT_TABLE_TEMPLATE_MAIN template<Nut::Type _T = Type::Data> class

#include <QSharedPointer>

namespace Nut {

enum Type { Data, Model, FieldPhrases, RuntimeChecker };

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
T<Model> &createModel()
{
    static_assert(sizeof(T<Model>) == std::size_t(-1), "Create model is not registered for this class");
    return T<Model>();
}

template<template<Type _Type> class T>
T<Model> &modelForRow(T<Type::Data> *) {
    return createModel<Table>();
}


template<template<Type _Type> class T>
QSharedPointer<T<Data>> createRow() {
    return QSharedPointer<T<Data>>(new T<Data>);
}

} // namespace Nut

#define NUT_EXPORT

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#   define VARIANT_TYPE_COMPARE(v, t)  v.typeId() == QMetaType::Q##t
#   define VARIANT_TYPE_COMPARE_X(v, vt, mt)  v.typeId() == QMetaType::mt
#   define METATYPE_TO_NAME(type) QMetaType(type).name()
#   define METATYPE_ID(v) static_cast<QMetaType::Type>(v.typeId())
#else
#   define VARIANT_TYPE_COMPARE(v, t)  v.type() == QVariant::t
#   define VARIANT_TYPE_COMPARE_X(v, vt, mt)  v.type() == QVariant::vt
#   define METATYPE_TO_NAME(type) QMetaType::typeName(type)
#   define METATYPE_ID(v) static_cast<QMetaType::Type>(v.type())
#endif

#endif // GLOBAL_H
