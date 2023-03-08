#pragma once

#include "global.h"
namespace Nut {

template<Nut::Type _Type>
class Database;

class AbstractTableModel;

class DatasetBase
{
protected:
    AbstractTableModel *_model;

public:
    DatasetBase() = default;

    DatasetBase(Table<Type::Data> *parent,
                Table<Type::Model> *model,
                const char *name,
                const QString &className);
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

    virtual int save(Database<Type::Data> *database)
    {
        Q_UNUSED(database);
        return 0;
    }
};

} // namespace Nut
