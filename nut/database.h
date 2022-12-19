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
template<TableType _Type>
class Table;
template<NUT_TABLE_TEMPLATE T>
class ModelBase;

template<NUT_TABLE_TEMPLATE C, TableType _Type>
struct TableTypeHelper {
    using type = void;
};

template<NUT_TABLE_TEMPLATE C>
struct TableTypeHelper<C, TableTypeMain> {
    using type = Dataset<C>;
};

template<NUT_TABLE_TEMPLATE C>
struct TableTypeHelper<C, TableTypeModel> {
    using type = ModelBase<C>;
};

template<NUT_TABLE_TEMPLATE C>
struct TableTypeHelper<C, TableTypeFieldPhrases> {
    using type = void;
};

template<NUT_TABLE_TEMPLATE C>
struct TableTypeHelper<C, RuntimeChecker>{
    using type = void;
};


class DatasetBase;

class Database
{

public:
    QMap<QString, DatasetBase*> _tables;

    Q_DECL_DEPRECATED
    Database();

    TableMain *createTable(const QString &name) const;
    QList<TableModel*> model() const;

    QJsonObject jsonModel() const;
};

template<Nut::TableType _Type>
class Database2
{

};

template<>
class Database2<TableTypeMain>
{
};

template<>
class Database2<TableTypeModel>
{
    QList<AbstractModel*> _tables;
public:

    QJsonObject jsonModel() const;

    friend class AbstractModel;
};

#define NutDatabaseBase Nut::Database2<_Type>
#define NUT_DATABASE \
template<template<Nut::TableType _T> class T> \
    using Tableset = typename Nut::TableTypeHelper<T, _Type>::type;

#define NUT_DECLARE_DATABASE(name) \
extern name<Nut::TableTypeModel> name##Model;

#define NUT_DECLARE_IMPLEMENT(name) \
name<Nut::TableTypeModel> name##Model;

#define Nut_TableSet2(type, name) Tableset<type> name{this, #name}
//Nut::Database2<T>::Tableset<type> name{this, #name}

} // namespace Nut

#endif // DATABASE_H
