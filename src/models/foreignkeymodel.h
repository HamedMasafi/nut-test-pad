#ifndef FOREIGNKEYMODEL_H
#define FOREIGNKEYMODEL_H

#include "../global.h"
#include "qdebug.h"

#include <QMetaType>

namespace Nut {

class AbstractTableModel;
class AbstractFieldPhrase;
class ForeignKeyModelBase {
    QString _name;
    Table<Type::Model> *_localModel;
    AbstractTableModel *_remoteModel{nullptr};
protected:
    QString _remoteClassName;
public:
    ForeignKeyModelBase(Table<Type::Model> *parent, Table<Type::Model> *remoteModel, const char *name);
    virtual QString keyType() const = 0;
    virtual QString remoteTableName() const ;
    virtual QString localTableName() const ;
    QString name() const;

    Nut::AbstractFieldPhrase *remoteTablePrimaryField() const;
    Table<Type::Model> *localModel() const;
    AbstractTableModel *remoteModel();
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
        _remoteClassName = T<Model>::staticClassName();
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
