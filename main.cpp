#include <QCoreApplication>
#include <QJsonDocument>
#include <nut/table.h>
#include <nut/models/fieldmodel.h>
#include <generators/sqlitegenerator.h>

#include "sampletable.h"

#include <QDebug>
#include "sampletable.h"
#include "db.h"
#include "parenttable.h"
#include "nut/namedtype.h"

#define print(x) qDebug() << #x "=" << (x)

void ff(TableModel *t)
{
    print(typeid(t).name());
    print(t->className());
}
void f() {
    DBModel.table20.t().id;
    DBDatabase d;
    auto q = d.table20.query().where(DBModel.table20.id == 4 || DBModel.table20.id == 3).toList();

    auto f = SampleTableModel2.field("id");
    qDebug() << "f.name" << f->name() << f->tableName();

    ff(&DBModel.table10);
    //    Database<Type::Data>::Tableset<SampleTable> ff();
}

void testJoin()
{
    DBDatabase db;
    db.table10.query()
        .join(DBModel.table20.t)
        .toList();
}
void printModel()
{
    DB<Nut::Type::Model> model;
    auto t = model.table10.id == model.table20.id;
    QJsonDocument doc(model.jsonModel());
    qDebug() << "===Model===";
    qDebug().noquote() << doc.toJson(QJsonDocument::Indented);
}

void changedTest()
{
    SampleTableTable t;
    t.id = 3;
    t.pn = 4;
    t.point = QPoint(1, 2);

    Nut::SqliteGenerator gen;
    gen._database = &DBModel;
    auto sql = gen.saveRecord(&t, "sample");
    print(sql);

    print(t.changedFields().contains("id"));
    print(t.changedFields().contains("pn"));
    print(t.changedFields().contains("point"));
    print(t.changedFields());
}

void checkQuery()
{
    DBDatabase db;
    Nut::Query2<SampleTable> q2;
    auto list = db.table10.query()
        .toList();
}
void checkExpressions()
{
    auto q1 = SampleTableModel2.id == 23;

    auto q2 = SampleTableModel2.id = 4;
    auto q3 = DBModel.table20.id == 4 && DBModel.table10.point == QPoint(1, 2);
    auto order = SampleTableModel2.id | !SampleTableModel2.ps;

    auto q4 = DBModel.table10.id == DBModel.table20.id;

    Nut::SqliteGenerator gen;
    auto where = gen.createConditionalPhrase(q3.data);
    print(where);
    print(DBModel.table10.id.data->tableName);
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
    qDebug() << table.ps << table.fieldValue("ps") << ps;

    table.ps = Nut::DBNull;
    table.ps = nullptr;
    qDebug() << table.ps;

    qDebug() << (table.ps == nullptr);
    table.id = 4;
    qDebug() << "Changed fields" << table.changedFields();


    ParentTableModel mm;

    mm.t().id;
    auto b = mm.t().id;
    Q_UNUSED(b)

    ParentTableTable ttt;
    ttt.t.object();
    Q_UNUSED(ttt)

    printModel();
    f();
    changedTest();
    ff(&mm);
    testJoin();
    checkExpressions();
    return EXIT_SUCCESS;
}
