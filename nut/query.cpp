
#include "query.h"
#include "database.h"
#include "generators/abstractsqlgenerator.h"

namespace Nut {
QString QueryData::generateSelectCommand() {
    auto gen = database->generator();
    return gen->selectCommand("SampleTable", fields, where, order);
}
} // namespace Nut
