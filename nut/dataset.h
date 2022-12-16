#ifndef DATASET_H
#define DATASET_H

#include "global.h"

namespace Nut {

class Database;
class DatasetBase
{
public:
    DatasetBase(Database *parent, const char *name);

    virtual TableMain *createTable() const = 0;
    virtual TableModel *createModel() const = 0;
};

template <template<TableType> class T>
class Dataset : public DatasetBase
{
public:
    template <TableType _T>
    Dataset(Table<_T> *parent, const char *name) : DatasetBase(parent, name)
    {}

    Dataset(Database *parent, const char *name) : DatasetBase(parent, name)
    {}

    T<TableTypeMain> *createTable() const override
    {
        return new T<TableTypeMain>();
    }
    T<TableTypeModel> *createModel() const override
    {
        return Nut::createModel<T>();
    }
};

} // namespace Nut

#define Nut_TableSet(type, name) Nut::Dataset<type> name{this, #name}

#endif // DATASET_H
