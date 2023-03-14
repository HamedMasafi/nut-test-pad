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
    QString _relationName;

public:
    template<Type _T>
    ChildDataset(Table<_T> *parent, const char *name)
        : DatasetBase(parent, name, Nut::createModel<T>())
    {}

    template<NUT_TABLE_TEMPLATE _Database, typename... Types>
    constexpr ChildDataset(_Database<Type::Data> *parent, const char *name, Types... args)
        : DatasetBase(parent, &Nut::createModel<_Database>(), name, T<Type::Data>::staticClassName())
        , _parentDatabase{parent}
    {
        using namespace Nut::ModelDeclartion;
        if constexpr (std::is_base_of<Database<Data>, _Database<Data>>::value) {
            _parentDatabase = parent;
        }
        parent->addTableset(name, parent, this);
        //        _parentDatabaseModel = &Nut::createModel<_Database>();
        NamedParam array[] = {args...};
        int size = sizeof...(args);

//        auto rel = pick<RelationName>(args...);
        Picker p{args...};
        const char *tmpName{};
        if (p.template pick<const char *>("RelationName", &tmpName))
            _relationName = tmpName;
        else
            assert("No relation name");

        qDebug() << "Rela=" << tmpName;
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
            i->setFieldValue(_relationName, _parentDatabase->primaryField()->toVariant());
            ret += i->save(database, model);
        }
        return ret;
    }
};
} // namespace Nut
