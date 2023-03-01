#pragma once

#include "global.h"

#include <QList>

namespace Nut {

class GlobalStorage
{
public:
    static GlobalStorage *instance();
//private:
    struct DatabaseData
    {
        QString className;
        QString databaseName;
        QString host;
        DatabaseData(QString className, QString databaseName, QString host);
    };

    QList<DatabaseData> _initedDatabasesList;

    GlobalStorage();

    int connectionsIndex{0};
    bool isDatabaseInited(QString className, QString databaseName, QString host);
    void setDatabaseInited(QString className, QString databaseName, QString host);

    template<Nut::Type _Type>
    friend class Database;

    friend class DatabaseData;
};

} // namespace Nut
