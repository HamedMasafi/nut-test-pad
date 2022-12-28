#ifndef DATABASE_H
#define DATABASE_H

#include <QJsonObject>
#include <QList>
#include <QMap>
#include "dataset.h"
#include "global.h"
#include "dataset.h"

namespace Nut {

class AbstractModel;
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

class Database3
{

public:
    QMap<QString, DatasetBase*> _tables;

    Q_DECL_DEPRECATED
    Database3();

    TableRow *createTable(const QString &name) const;
    QList<TableModel*> model() const;

    QJsonObject jsonModel() const;
};

template<Nut::Type _Type>
class Database
{

};

template<>
class Database<Type::Data>
{
};

template<>
class Database<Type::Model>
{
    QList<AbstractModel*> _tables;
public:

    QJsonObject jsonModel() const;

    friend class AbstractModel;
    Database<Type::Model> operator|(const Database<Type::Model> &other);

    AbstractModel *tableByName(const QString &name) const;
    QList<AbstractModel *> tables() const;
};

#define NutDatabaseBase Nut::Database<_Type>
#define NUT_DATABASE \
template<template<Nut::Type _T> class T> \
    using Tableset = typename Nut::TypeHelper<T, _Type>::type;

#define NUT_DECLARE_DATABASE(name) \
    extern name<Nut::Type::Model> name##Model; \
    using name##Database = name<Nut::Type::Data>;

#define NUT_DECLARE_IMPLEMENT(name) \
    name<Nut::Type::Model> name##Model;

#define Nut_TableSet2(type, name) Tableset<type> name{this, #name}
//Nut::Database<T>::Tableset<type> name{this, #name}

} // namespace Nut

#endif // DATABASE_H
