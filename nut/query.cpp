
#include "query.h"
#include "database.h"
#include "generators/abstractsqlgenerator.h"

namespace Nut {
QString QueryData::generateSelectCommand() {
    auto gen = database->generator();
    return gen->selectCommand("SampleTable", this);
}
} // namespace Nut
