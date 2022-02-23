#ifndef FOREIGNKEYMODEL_H
#define FOREIGNKEYMODEL_H

#include "../global.h"
#include "qglobal.h"

namespace Nut {

class ForeignKeyModelBase {
public:
    ForeignKeyModelBase(Table<TableTypeModel> *parent, const char *name);
};

template <NUT_TABLE_TEMPLATE T, typename KeyType>
class ForeignKeyModel : public ForeignKeyModelBase
{
    T<TableType::TableTypeMain>* _object{nullptr};
    KeyType _key;

public:
    ForeignKeyModel(Table<TableTypeModel> *parent, const char *name)
        : ForeignKeyModelBase(parent, name)
    {}

    T<TableTypeModel> operator()() { return T<TableTypeModel>{}; }
};

} // namespace Nut

#endif // FOREIGNKEYMODEL_H
