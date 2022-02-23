#ifndef FOREIGNKEYCHECKER_H
#define FOREIGNKEYCHECKER_H


#include "../global.h"
#include "qglobal.h"

namespace Nut {

//class ForeignKeyCheckerBase {
//public:
//    constexpr ForeignKeyCheckerBase(Table<RuntimeChecker> *parent, const char *name);
//};

template <NUT_TABLE_TEMPLATE T, typename KeyType>
class ForeignKeyChecker// : public ForeignKeyCheckerBase
{
    T<TableType::TableTypeMain>* _object{nullptr};
    KeyType _key;

public:
    constexpr ForeignKeyChecker(Table<RuntimeChecker> *parent, const char *name) {
        Q_UNUSED(parent)
        Q_UNUSED(name)
    }
};

} // namespace Nut
#endif // FOREIGNKEYCHECKER_H
