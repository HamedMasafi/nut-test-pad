#pragma once

#include "global.h"
#include "query.h"
#include <QList>


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

    virtual TableRow *createTable() const = 0;
    virtual TableModel *createModel() const = 0;
};

template <template<Type> class T>
class Dataset : public DatasetBase
{
    QList<T<Type::Data>> _list;

public:
    template <Type _T>
    Dataset(Table<_T> *parent, const char *name) : DatasetBase(parent, name, Nut::createModel<T>())
    {

    }

    Dataset(Database<Type::Data> *parent, const char *name)// : DatasetBase(nullptr, name, Nut::createModel<T>())
    {

    }

    T<Type::Data> *createTable() const override
    {
        return new T<Type::Data>();
    }
    T<Type::Model> *createModel() const override
    {
        return Nut::createModel<T>();
    }

    Query<T> query() const{
        return Query<T>();
    }
    void append(T<Type::Data> *row) {
        _list.append(row);
    }
};

} // namespace Nut

#define Nut_TableSet(type, name) Nut::Dataset<type> name{this, #name}

