#ifndef TABLEBASE_H
#define TABLEBASE_H

#include "../global.h"

#include "../runtimecheckers/tablechecker.h"
#include "abstracttable.h"

#include <QMap>
#include <QSet>
#include <QString>
#include <QVariant>

namespace Nut {
class FieldBase;
class ForeignKeyModelBase;
class FieldCheckerBase;
class AbstractFieldPhrase;
template<Nut::TableType _Type>
class Database;


using TableClassBase = Table<TableTypeMain>;
using ModelClassBase = Table<TableTypeModel>;


}
#endif // TABLEBASE_H
