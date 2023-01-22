#include "databasedata.h"
#include "databasemigratestorage.h"
#include "generators/sqlitegenerator.h"

#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace Nut {

class DatabaseData : public QSharedData
{
public:
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
};


bool DatabaseData::open()
{
    if (db.isOpen())
        return true;

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

AbstractSqlGenerator *Database<Type::Data>::generator()
{
    return d->generator;
}

bool Nut::Database<Type::Data>::open()
{
    if (d->db.isOpen())
        return true;
    d->generator = new SqliteGenerator;

    return true;
}

Nut::Database<Type::Data>::Database()
    : d{new DatabaseData}
{

}


} // namespace Nut
