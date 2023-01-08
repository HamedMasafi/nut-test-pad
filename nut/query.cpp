
#include "query.h"
#include "database.h"
#include "generators/abstractsqlgenerator.h"

namespace Nut {
QString QueryData::generateSelectCommand() {
    return database->generator()->selectCommand("SampleTable", fields, where, order, QList<RelationModel *>());
}
} // namespace Nut
