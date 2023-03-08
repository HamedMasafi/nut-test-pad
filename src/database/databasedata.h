#pragma once

#include "core/changelogtable.h"
#include "core/tablesetcontainer.h"
#include "dataset.h"
#include "global.h"

#include <QSqlDatabase>

namespace Nut {

class AbstractSqlGenerator;
class DatasetBase;
class DatabaseData : public QSharedData
{
public:
    DatabaseData(Database<Type::Data> *parent);

    Database<Type::Data> *parent;
    QString hostName;
    QString databaseName;
    int port;
    QString userName;
    QString password;
    QString connectionName;
    QString driver;

    AbstractSqlGenerator *generator{nullptr};
    QSqlDatabase db;

    bool open();
    bool updateDatabase();
    void generateConnectionName();
    void createGenerator();
    Database<Type::Model> getLastModelFromDatabase();
    void storeModelInDatabase(const Database<Type::Model> &model);
    bool createDatabase();
    QString changeLogsCreationSql();
};

template<>
class Database<Type::Data> : public TableSetContainer
{
    QSharedDataPointer<DatabaseData> d;
public:
    Database();
    AbstractSqlGenerator *generator();

    bool open();

    virtual QString className() = 0;

    QString userName() const;
    void setUserName(const QString &newUserName);

    QString password() const;
    void setPassword(const QString &newPassword);

    QString databaseName() const;
    void setDatabaseName(const QString &newDatabaseName);

    QString driver() const;
    void setDriver(const QString &newDriver);

    int saveChanges();

    QSqlQuery exec(const QString &sql);

protected:
    virtual Database<Type::Model> &model() const = 0;

    template <template<Type> class T>
    friend class Dataset;
    friend class DatabaseData;
    friend class QueryData;
    friend class AbstractSqlGenerator;
};

} // namespace Nut
