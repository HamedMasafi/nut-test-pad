#pragma once

#include "global.h"
#include "query.h"
#include "database/databasedata.h"
#include "core/datasetbase.h"
#include "childdataset.h"

#include <QList>
#include <QObject>

namespace Nut {

template<Nut::Type _Type>
class Database;

template <template<Type> class T>
class Dataset : public DatasetBase
{
    QList<Nut::RowPointer<T>> _list;
    Database<Type::Data> *_parentDatabase;
    Database<Type::Model> *_parentDatabaseModel;

public:
    template <Type _T>
    Dataset(Table<_T> *parent, const char *name) : DatasetBase(parent, name, Nut::createModel<T>())
    {

    }

    template<NUT_TABLE_TEMPLATE _Database>
    Dataset(_Database<Type::Data> *parent, const char *name)
        : DatasetBase(nullptr,
                      &Nut::createModel<_Database>(),
                      name,
                      T<Type::Data>::staticClassName())
    //        , _parentDatabase{parent}
    {
        if constexpr (std::is_base_of<Database<Data>, _Database<Data>>::value) {
            _parentDatabase = parent;
        }
        parent->addTableset(name, this);
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
    //    void append(T<Type::Data> *row) {
    //        _list.append(row);
    //    }
    void append(const Nut::RowPointer<T> &row) {
        _list.append(row);
    }

    int save(Database<Type::Data> *database) override
    {
        int ret{0};

        auto model = static_cast<TableModel *>(&createModel());
        for (auto &i: _list) {
            ret += i->save(database, model);
        }
        return ret;
    }
};

} // namespace Nut

//#define Nut_TableSet(type, name) Nut::Dataset<type> name{this, #name}

