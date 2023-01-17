#ifndef DATABASE_H
#define DATABASE_H

#include <QJsonObject>
#include <QList>
#include <QMap>
#include "dataset.h"
#include "global.h"
#include "dataset.h"

namespace Nut {

class AbstractSqlGenerator;
class AbstractTableModel;
template<Type _Type>
class Table;
template<NUT_TABLE_TEMPLATE T>
class ModelBase;

template<NUT_TABLE_TEMPLATE C, Type _Type>
struct TypeHelper {
    using type = void;
};

template<NUT_TABLE_TEMPLATE C>
struct TypeHelper<C, Type::Data> {
    using type = Dataset<C>;
};

template<NUT_TABLE_TEMPLATE C>
struct TypeHelper<C, Type::Model> {
    using type = ModelBase<C>;
};

template<NUT_TABLE_TEMPLATE C>
struct TypeHelper<C, Type::FieldPhrases> {
    using type = void;
};

template<NUT_TABLE_TEMPLATE C>
struct TypeHelper<C, Type::RuntimeChecker>{
    using type = void;
};


class DatasetBase;

template<Nut::Type _Type>
class Database
{

};

template<>
class Database<Type::Data>
{
    AbstractSqlGenerator *_generator{nullptr};

public:
    AbstractSqlGenerator *generator();
};

template<>
class Database<Type::Model>
{
    QList<AbstractTableModel*> _tables;
public:

    QJsonObject jsonModel() const;

    friend class AbstractTableModel;
    Database<Type::Model> operator|(const Database<Type::Model> &other);

    AbstractTableModel *tableByName(const QString &name) const;
    QList<AbstractTableModel *> tables() const;
};

#define NutDatabaseBase Nut::Database<_Type>
#define NUT_DATABASE \
    template<template<Nut::Type _T> class T> \
    using Tableset = typename Nut::TypeHelper<T, _Type>::type;

#define NUT_DECLARE_DATABASE(name) \
    namespace Nut { \
    template<> \
    name<Type::Model> &createModel<name>(); \
    } \
    extern name<Nut::Type::Model> name##Model; \
    using name##Database = name<Nut::Type::Data>;

#define NUT_IMPLEMENT_DATABASE(name) \
    namespace Nut { \
    template<> \
    name<Type::Model> &createModel<name>() \
    { \
        return name##Model; \
    } \
    } \
    name<Nut::Type::Model> name##Model;

#define Nut_TableSet(type, name) Tableset<type> name{this, #name}
//Nut::Database<T>::Tableset<type> name{this, #name}

} // namespace Nut

#endif // DATABASE_H
