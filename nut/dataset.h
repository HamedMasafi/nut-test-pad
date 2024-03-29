#pragma once

#include "global.h"
#include "query.h"
#include <QList>
#include <QObject>


namespace Nut {

template<Nut::Type _Type>
class Database;

class DatasetBase
{
public:
    DatasetBase() = default;
//    DatasetBase(Database *parent, const char *name);

//    template<template<Nut::Type> class T>
//        DatasetBase(Database *parent, const char *name, T<Nut::Type::Model> *model) : DatasetBase(parent, name)
//    {
//        for (auto &f: model->_fields) {
//            f->data->className = name;
//        }
//    }

//    virtual TableRow *createTable() const = 0;
//    virtual TableModel *createModel() const = 0;
};

template <template<Type> class T>
class Dataset : public DatasetBase
{
    QList<T<Type::Data>> _list;
    Database<Type::Data> *_parentDatabase;
    Database<Type::Model> *_parentDatabaseModel;

public:
    template <Type _T>
    Dataset(Table<_T> *parent, const char *name) : DatasetBase(parent, name, Nut::createModel<T>())
    {

    }

    template<NUT_TABLE_TEMPLATE _Database>
    Dataset(_Database<Type::Data> *parent, const char *name)
        : _parentDatabase{parent} // : DatasetBase(nullptr, name, Nut::createModel<T>())
    {
        Q_UNUSED(name)
        _parentDatabaseModel = &Nut::createModel<_Database>();
    }

    T<Type::Data> *createTable() const { return new T<Type::Data>(); }
    T<Type::Model> &createModel() const
    {
        return Nut::createModel<T>();
    }

    Query<T> query() const{
        return Query<T>(_parentDatabase, _parentDatabaseModel, Nut::createModel<T>());
    }
    void append(T<Type::Data> *row) {
        _list.append(row);
    }
};

} // namespace Nut

#define Nut_TableSet(type, name) Nut::Dataset<type> name{this, #name}

