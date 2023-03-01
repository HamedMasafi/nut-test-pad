#include "databasedata.h"
#include "core/globalstorage.h"
#include "dataset.h"
#include "databasemigratestorage.h"
#include "generators/sqlitegenerator.h"
#include "databasemodel.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Nut {

DatabaseData::DatabaseData(Database<Data> *parent) : parent{parent}
{

}

bool DatabaseData::open()
{
    if (db.isOpen())
        return true;

    if (GlobalStorage::instance()->isDatabaseInited(parent->className(), databaseName, hostName)){

    }
    bool isDatabaseNew{};
    bool update{};

    connectionName = "";//QString::fromUtf8(q->metaObject()->className())
//                     + QString::number(DatabasePrivate::lastId);

    db = QSqlDatabase::addDatabase(driver, connectionName);
    db.setHostName(hostName);
    if (port)
        db.setPort(port);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(password);

    return db.open();

    if (driver.startsWith(QStringLiteral("qsqlite"), Qt::CaseInsensitive)
        && !QFile::exists(databaseName)) {
        //Force to execute update database
        isDatabaseNew = true;
        update = true;
    }
    bool ok = db.open();

    if (!ok) {
        qWarning("Could not connect to database, error = %s",
                 db.lastError().text().toLocal8Bit().data());

        if (db.lastError().text().contains(QStringLiteral("database \"")
                                           + databaseName
                                           + QStringLiteral("\" does not exist"))
            || db.lastError().text().contains(QStringLiteral("Cannot open database"))
            || db.lastError().text().contains(QStringLiteral("Unknown database '")
                                              + databaseName
                                              + QStringLiteral("'"))) {

            db.close();
            db.setDatabaseName(generator->masterDatabaseName(databaseName));
            ok = db.open();
            qDebug("Creating database");
            if (ok) {
                db.exec(QStringLiteral("CREATE DATABASE ") + databaseName);
                db.close();

                if (db.lastError().type() != QSqlError::NoError) {
                    qWarning("Creating database error: %s",
                             db.lastError().text().toLatin1().data());
                    return false;
                }

                isDatabaseNew = true;
                return open(/*update*/);
            }
            qWarning("Unknown error detecting change logs, %s",
                     db.lastError().text().toLatin1().data());

        }
        return false;
    }

    //    if(update)
    return updateDatabase();
}

bool DatabaseData::updateDatabase()
{
    if (GlobalStorage::instance()->isDatabaseInited(parent->className(), databaseName, hostName))
        return true;

    if (!createDatabase())
        return false;

    auto currentModel = parent->model();
    auto databaseModel = getLastModelFromDatabase();
    auto diff = generator->diffDatabase(databaseModel, currentModel);
    diff << changeLogsCreationSql();
    if (!db.transaction())
        return false;
    for (auto &sql: diff) {
        db.exec(sql);
        if (db.lastError().type() != QSqlError::NoError) {
            qDebug() << "Error updating database" << db.lastError().text();
            if (!db.rollback())
                return false;
            return false;
        }
    }
    storeModelInDatabase(currentModel);
    if (!db.commit())
        return false;
    qDebug() << "Database updated";
    GlobalStorage::instance()->setDatabaseInited(parent->className(), databaseName, hostName);
    return true;
}

void DatabaseData::generateConnectionName()
{
    connectionName = QStringLiteral("nut_connection_%1")
                         .arg(GlobalStorage::instance()->connectionsIndex);
    GlobalStorage::instance()->connectionsIndex++;
}

void DatabaseData::createGenerator()
{
    using PostgreSqlGenerator = SqliteGenerator;
    using MySqlGenerator = SqliteGenerator;
    using SqlServerGenerator = SqliteGenerator;
    if (driver == QStringLiteral("QPSQL") || driver == QStringLiteral("QPSQL7"))
        generator = new PostgreSqlGenerator(parent);
    else if (driver == QStringLiteral("QMYSQL") || driver == QStringLiteral("QMYSQL3"))
        generator = new MySqlGenerator(parent);
    else if (driver == QStringLiteral("QSQLITE") || driver == QStringLiteral("QSQLITE3"))
        generator = new SqliteGenerator(parent);
    else if (driver == QStringLiteral("QODBC") || driver == QStringLiteral("QODBC3")) {
        QString driverName = QString();
        QStringList parts = databaseName.toLower().split(QLatin1Char(';'));
        for (auto &p: parts)
            if (p.trimmed().startsWith(QStringLiteral("driver=")))
                driverName = p.split(QLatin1Char('=')).at(1).toLower().trimmed();

        //        if (driverName == "{sql server}")
        generator = new SqlServerGenerator(parent);
        // TODO: add ODBC driver for mysql, postgres, ...
    }

    if (!generator) {
        qFatal("Sql generator for driver %s not found",
               driver.toLatin1().constData());
    }
}

Database<Model> DatabaseData::getLastModelFromDatabase()
{
    if (!db.isOpen())
        return {};

    auto q = db.exec("SELECT * FROM __changeLogs ORDER BY id DESC");

    if (!q.next())
        return {};

    Database<Model> ret;
    QJsonDocument doc = QJsonDocument::fromJson(q.value("data").toString().toUtf8());
    if (!doc.isObject())
        return {};

    ret.fromJsonModel(doc.object());
    return ret;
}

void DatabaseData::storeModelInDatabase(const Database<Model> &model)
{
    QJsonDocument doc = QJsonDocument(model.jsonModel());
    auto sql= QStringLiteral("INSERT INTO __changeLogs (data, version) VALUES ('%1', 0)").arg(doc.toJson());
    db.exec(sql);

    if (db.lastError().type() != QSqlError::NoError) {
        qWarning("Error storing model in database: %s", db.lastError().text().toLatin1().data());
    }
}

bool DatabaseData::createDatabase()
{
    if (driver.startsWith("qsqlite", Qt::CaseInsensitive)) {
        if (db.isOpen())
            return true;
        return db.open();
    }
    db.close();
    db.setDatabaseName(generator->masterDatabaseName(databaseName));
    if (!db.open()) {
        qWarning("Unknown error detecting change logs, %s", db.lastError().text().toLatin1().data());
        return false;
    }
    qDebug("Creating database");
    db.exec(QStringLiteral("CREATE DATABASE ") + databaseName);
    db.close();

    if (db.lastError().type() != QSqlError::NoError) {
        qWarning("Creating database error: %s", db.lastError().text().toLatin1().data());
        return false;
    }

    return true;
}

QString DatabaseData::changeLogsCreationSql()
{
    return generator->createTable(&ChangeLogTableModel2, "__changeLogs");
}

AbstractSqlGenerator *Database<Type::Data>::generator()
{
    return d->generator;
}

bool Nut::Database<Type::Data>::open()
{
    if (d->db.isOpen())
        return true;

    d->createGenerator();

    if (!d->open())
        return false;

    d->updateDatabase();

    return true;
}

Database<Type::Data>::Database()
    : d{new DatabaseData{this}}
{

}

QString Database<Type::Data>::userName() const
{
    return d->userName;
}

void Database<Type::Data>::setUserName(const QString &newUserName)
{
    d->userName = newUserName;
}

QString Database<Type::Data>::password() const
{
    return d->password;
}

void Database<Type::Data>::setPassword(const QString &newPassword)
{
    d->password = newPassword;
}

QString Database<Type::Data>::databaseName() const
{
    return d->databaseName;
}

void Database<Type::Data>::setDatabaseName(const QString &newDatabaseName)
{
    d->databaseName = newDatabaseName;
}

QString Database<Type::Data>::driver() const
{
    return d->driver;
}

void Database<Type::Data>::setDriver(const QString &newDriver)
{
    d->driver = newDriver;
}

int Nut::Database<Type::Data>::saveChanges()
{
    for (auto i = d->tables.begin(); i != d->tables.end(); i++) {
        i.value()->save();
    }
}

void Nut::Database<Type::Data>::addTableset(const QString &name, DatasetBase *table)
{
    d->tables.insert(name, table);
}


} // namespace Nut
