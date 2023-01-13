#ifndef FOREIGNKEYMODEL_H
#define FOREIGNKEYMODEL_H

#include "../global.h"
#include "qglobal.h"

#include <QMetaType>

namespace Nut {

class ForeignKeyModelBase {
    QString _name;
public:
    ForeignKeyModelBase(Table<Type::Model> *parent, const char *name);
    virtual QString keyType() const = 0;
    virtual QString tableName() const = 0;
    QString name() const;
};

template <NUT_TABLE_TEMPLATE T, typename KeyType>
class ForeignKeyModel : public ForeignKeyModelBase
{
    T<Type::Data>* _object{nullptr};
    KeyType _key;

public:
    ForeignKeyModel(Table<Type::Model> *parent, const char *name)
        : ForeignKeyModelBase(parent, name)
    {}

    QString keyType() const override {
        return QMetaType::fromType<KeyType>().name();
    }
    virtual QString tableName() const override {
        return T<Type::Model>().name();
    }
    T<Type::Model> operator()() { return T<Type::Model>{}; }
};

} // namespace Nut

#endif // FOREIGNKEYMODEL_H
