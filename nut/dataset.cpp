#include "database.h"
#include "dataset.h"

namespace Nut {

DatasetBase::DatasetBase(Database *parent, const char *name)
{
    parent->_tables.insert(name, this);
}


} // namespace Nut
