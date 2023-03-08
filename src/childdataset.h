#pragma once

#include "dataset.h"
#include "global.h"

namespace Nut {
template<template<Type> class T>
class ChildDataset : public DatasetBase
{
    QList<Nut::RowPointer<T>> _list;
    Table<Type::Data> *_parentDatabase;
    Database<Type::Model> *_parentDatabaseModel;

public:
    template<Type _T>
    ChildDataset(Table<_T> *parent, const char *name)
        : DatasetBase(parent, name, Nut::createModel<T>())
    {}

    template<NUT_TABLE_TEMPLATE _Database>
    ChildDataset(_Database<Type::Data> *parent, const char *name, ...)
        : DatasetBase(parent, &Nut::createModel<_Database>(), name, T<Type::Data>::staticClassName())
        , _parentDatabase{parent}
    {
        if constexpr (std::is_base_of<Database<Data>, _Database<Data>>::value) {
            _parentDatabase = parent;
        }
        parent->addTableset(name, parent, this);
        //        _parentDatabaseModel = &Nut::createModel<_Database>();
    }

    T<Type::Data> *createTable() const { return new T<Type::Data>(); }
    T<Type::Model> &createModel() const { return Nut::createModel<T>(); }

    Query<T> query()
    {
        //        _parentDatabaseModel->tableByName(T<Type::Data>::staticClassName());
        auto t = T<Type::Data>::staticClassName();
        return Query<T>(_parentDatabase, _parentDatabaseModel, _model);
    }
    //    void append(T<Type::Data> *row) {
    //        _list.append(row);
    //    }
    void append(const Nut::RowPointer<T> &row) { _list.append(row); }

    int save(Database<Type::Data> *database) override
    {
        int ret{0};

        auto model = static_cast<TableModel *>(&createModel());
        for (auto &i : _list) {
            ret += i->save(database, model);
        }
        return ret;
    }
};
} // namespace Nut
