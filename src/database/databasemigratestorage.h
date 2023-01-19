#pragma once

#include <QMap>
#include <QString>

class DatabaseMigrateStorage
{
public:
    DatabaseMigrateStorage();
    static DatabaseMigrateStorage *instance();

private:
    QMap<QString, int> _data;
};

