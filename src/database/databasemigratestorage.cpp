#include "databasemigratestorage.h"

DatabaseMigrateStorage::DatabaseMigrateStorage()
{

}

DatabaseMigrateStorage *DatabaseMigrateStorage::instance()
{
    static DatabaseMigrateStorage instance;
    return &instance;
}
