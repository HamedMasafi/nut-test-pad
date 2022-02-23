#include <QCoreApplication>
#include <QJsonDocument>
#include <nut/table.h>
#include <nut/models/fieldmodel.h>

#include "sampletable.h"

#include <QDebug>
#include "sampletable.h"
#include "db.h"
#include "parenttable.h"
#include "nut/namedtype.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug () << "v2";


    SampleTableModel model;
    SampleTableTable table;

    table.ps = QString("hamed");
    qDebug() << table.ps;

    table.ps = "salam";
    QString ps = table.ps;
    qDebug() << table.ps << table.fieldvalue("ps") << ps;

    table.ps = Nut::DBNull;
    table.ps = nullptr;
    qDebug() << table.ps;

    qDebug() << (table.ps == nullptr);
    table.id = 4;
    qDebug() << "Changed fields" << table.changedFields();


    DB _database;
    DB _database2;

    qDebug() << " ====== MODEL ====== ";
    auto dbmodel = _database.model();
    for (auto const &t: qAsConst(dbmodel)) {
        qDebug() << "*** Table ***";

        for (auto const &f: qAsConst(t->fields())) {
            qDebug() << "name: " << f->_name;
            qDebug() << "   is primary key: " << f->_isPrimaryKey;
            qDebug() << "   max len is: " << f->_maxLen;
            qDebug() << "   len is: " << f->_len;
            qDebug() << "   auto increment: " << f->_autoIncrement.first << f->_autoIncrement.second;
        }
    }

    QJsonDocument doc(_database.jsonModel());
    qDebug().noquote() << doc.toJson(QJsonDocument::Indented);
    auto tbl = reinterpret_cast<SampleTableTable*>(_database.createTable("table1"));
    tbl->setFieldValue("id", 2);
    qDebug() << tbl->id << tbl->fieldvalue("id");

    ParentTableModel mm;
    auto b = mm.t().id;
    Q_UNUSED(b)

    ParentTableTable ttt;
    ttt.t.object();
    Q_UNUSED(ttt)
    return EXIT_SUCCESS;
}
