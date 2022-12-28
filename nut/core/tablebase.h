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
template<Nut::Type _Type>
class Database;


using TableClassBase = Table<Type::Data>;
using ModelClassBase = Table<Type::Model>;


}
#endif // TABLEBASE_H
