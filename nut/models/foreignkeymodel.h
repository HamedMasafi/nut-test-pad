#ifndef FOREIGNKEYMODEL_H
#define FOREIGNKEYMODEL_H

#include "../global.h"
#include "qglobal.h"

#include <QMetaType>

namespace Nut {

class ForeignKeyModelBase {
public:
    ForeignKeyModelBase(Table<TableTypeModel> *parent, const char *name);
    virtual QString keyType() const = 0;
    virtual QString tableNae() const = 0;
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

    QString keyType() const override {
        return QMetaType::fromType<KeyType>().name();
    }
    virtual QString tableNae() const override {
        return T<TableTypeModel>().name();
    }
    T<TableTypeModel> operator()() { return T<TableTypeModel>{}; }
};

} // namespace Nut

#endif // FOREIGNKEYMODEL_H
