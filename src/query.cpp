
#include "query.h"
#include "database.h"
#include "generators/abstractsqlgenerator.h"

namespace Nut {
QString QueryData::generateSelectCommand() {
    auto gen = database->generator();
    sql = gen->selectCommand("Post", this);
    return sql;
}
} // namespace Nut
