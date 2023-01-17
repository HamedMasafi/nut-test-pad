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
protected:
    AbstractTableModel *_model;

public:
    DatasetBase() = default;

    DatasetBase(Database<Type::Data> *parent, const char *name, const QString &className);

    DatasetBase(Database<Type::Data> *parentDatabase,
                Database<Type::Model> *parentDatabaseModel,
                const char *name,
                const QString &className);

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
        : DatasetBase(nullptr, &Nut::createModel<_Database>(), name, T<Type::Data>::staticClassName())
        , _parentDatabase{parent}
    {
        Q_UNUSED(name)
        _parentDatabaseModel = &Nut::createModel<_Database>();
    }

    T<Type::Data> *createTable() const { return new T<Type::Data>(); }
    T<Type::Model> &createModel() const
    {
        return Nut::createModel<T>();
    }

    Query<T> query(){
//        _parentDatabaseModel->tableByName(T<Type::Data>::staticClassName());
        auto t = T<Type::Data>::staticClassName();
        return Query<T>(_parentDatabase, _parentDatabaseModel, _model);
    }
    void append(T<Type::Data> *row) {
        _list.append(row);
    }
};

} // namespace Nut

//#define Nut_TableSet(type, name) Nut::Dataset<type> name{this, #name}

