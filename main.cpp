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

void f() {
    DB2<Nut::TableTypeModel> d;
    d.table10.fields();

//    SampleTableModel2.id
//    Database2<TableTypeMain>::Tableset<SampleTable> ff();
}

void printModel()
{
    DB2<Nut::TableTypeModel> model;
    auto t = model.table10.id == model.table20.id;
    QJsonDocument doc(model.jsonModel());
    qDebug() << "===Model===";
    qDebug().noquote() << doc.toJson(QJsonDocument::Indented);
}

void checkExpressions()
{
    auto q1 = SampleTableModel2.id == 23;

    auto q2 = SampleTableModel2.id = 4;
    auto q3 = SampleTableModel2.id == 4 && SampleTableModel2.point == QPoint(1, 2);
    auto order = SampleTableModel2.id | !SampleTableModel2.ps;

    auto q4 = DB2Model.table10.id == DB2Model.table20.id;
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug () << "v2";


    SampleTableModel model;
    SampleTableTable table;
//    model.pn == model.id;

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

    qDebug() << " ====== MODEL ====== " << SampleTableModel2.id.name() << SampleTableModel2.id.data->className;

    auto dbmodel = _database.model();
    for (auto const &t: qAsConst(dbmodel)) {
        qDebug() << "*** Table ***";

//        for (auto const &f: qAsConst(t->fields())) {
//            qDebug() << "name: " << f->name();
//            qDebug() << "   is primary key: " << f->isPrimaryKey();
//            qDebug() << "   max len is: " << f->maxLen();
//            qDebug() << "   len is: " << f->len();
//            qDebug() << "   auto increment: " << f->autoIncrementStart() << f->autoIncrementStep();
//        }
    }

    QJsonDocument doc(_database.jsonModel());
    qDebug().noquote() << doc.toJson(QJsonDocument::Indented);
    auto tbl = reinterpret_cast<SampleTableTable*>(_database.createTable("table1"));
    tbl->setFieldValue("id", 2);
    qDebug() << tbl->id << tbl->fieldvalue("id");


    ParentTableModel mm;
    mm.t().id;
    auto b = mm.t().id;
    Q_UNUSED(b)

    ParentTableTable ttt;
    ttt.t.object();
    Q_UNUSED(ttt)

    printModel();
    return EXIT_SUCCESS;
}
