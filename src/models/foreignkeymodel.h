#ifndef FOREIGNKEYMODEL_H
#define FOREIGNKEYMODEL_H

#include "../global.h"

#include <QMetaType>

namespace Nut {

class AbstractFieldPhrase;
class ForeignKeyModelBase {
    QString _name;
    Table<Type::Model> *_localModel;
    Table<Type::Model> *_remoteModel;
public:
    ForeignKeyModelBase(Table<Type::Model> *parent, Table<Type::Model> *remoteModel, const char *name);
    virtual QString keyType() const = 0;
    virtual QString remoteTableName() const ;
    virtual QString localTableName() const ;
    QString name() const;

    Nut::AbstractFieldPhrase *remoteTablePrimaryField() const;
    Table<Type::Model> *localModel() const;
    Table<Type::Model> *remoteModel() const;
};

template <NUT_TABLE_TEMPLATE T, typename KeyType>
class ForeignKeyModel : public ForeignKeyModelBase
{
    KeyType _key;

public:
    template<NUT_TABLE_TEMPLATE _LocalTable>
    ForeignKeyModel(_LocalTable<Type::Model> *parent, const char *name)
        : ForeignKeyModelBase(parent, &Nut::createModel<T>(), name)
    {

    }

    QString keyType() const override {
        return QMetaType::fromType<KeyType>().name();
    }

    T<Type::Model> operator()() { return Nut::createModel<T>(); }

    /*auto remoteTablePrimaryField() -> QString
    {
        auto m = Nut::createModel<T>();
        return m;
    }*/

};

} // namespace Nut

#endif // FOREIGNKEYMODEL_H
