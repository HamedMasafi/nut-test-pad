#pragma once

#include "global.h"
#include "phrases/conditionalphrase.h"
#include "phrases/phraselist.h"

namespace Nut {

template<Nut::Type T>
class Database;

class ForeignKeyModelBase;

class AbstractTableModel;
struct NUT_EXPORT QueryData : public QSharedData
{
    AbstractTableModel *model;
    Database<Type::Data> *database;
    Database<Type::Model> *databaseModel;
    ConditionalPhrase where;
    PhraseList order;
    QList<const ForeignKeyModelBase *> joins;
    PhraseList fields;
    QString generateSelectCommand();

    QString sql;
    int take{-1};
    int skip{-1};
};

} // namespace Nut
