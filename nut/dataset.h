#pragma once

#include "global.h"

#include <QList>


namespace Nut {

template<Nut::TableType _Type>
class Database2;
class Database;
class DatasetBase
{
public:
    DatasetBase() = default;
    DatasetBase(Database *parent, const char *name);

    template<template<Nut::TableType> class T>
        DatasetBase(Database *parent, const char *name, T<Nut::TableTypeModel> *model) : DatasetBase(parent, name)
    {
        for (auto &f: model->_fields) {
            f->data->className = name;
        }
    }

    virtual TableMain *createTable() const = 0;
    virtual TableModel *createModel() const = 0;
};

template <template<TableType> class T>
class Dataset : public DatasetBase
{
    QList<T<TableTypeMain>> _list;

public:
    template <TableType _T>
    Dataset(Table<_T> *parent, const char *name) : DatasetBase(parent, name, Nut::createModel<T>())
    {

    }

    Dataset(Database *parent, const char *name) : DatasetBase(parent, name, Nut::createModel<T>())
    {

    }

    Dataset(Database2<TableTypeMain> *parent, const char *name)// : DatasetBase(nullptr, name, Nut::createModel<T>())
    {

    }

    T<TableTypeMain> *createTable() const override
    {
        return new T<TableTypeMain>();
    }
    T<TableTypeModel> *createModel() const override
    {
        return Nut::createModel<T>();
    }

    void append(T<TableTypeMain> *row) {
        _list.append(row);
    }
};

} // namespace Nut

#define Nut_TableSet(type, name) Nut::Dataset<type> name{this, #name}

