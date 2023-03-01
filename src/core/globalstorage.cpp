#include "globalstorage.h"

namespace Nut {

GlobalStorage::GlobalStorage() {}

GlobalStorage *GlobalStorage::instance()
{
    static GlobalStorage instance;
    return &instance;
}

bool GlobalStorage::isDatabaseInited(QString className, QString databaseName, QString host)
{
    return std::any_of(_initedDatabasesList.begin(), _initedDatabasesList.end(), [&className, &databaseName, &host](const DatabaseData &d) {
        return d.databaseName == databaseName && d.className == className && d.host == host;
    });
}

void GlobalStorage::setDatabaseInited(QString className, QString databaseName, QString host)
{
    _initedDatabasesList.emplaceBack(className, databaseName, host);
}

GlobalStorage::DatabaseData::DatabaseData(QString className, QString databaseName, QString host)
    : className(std::move(className))
    , databaseName(std::move(databaseName))
    , host(std::move(host))
{}

} // namespace Nut
