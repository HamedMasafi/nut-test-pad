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

#define print(x) qDebug() << #x "=" << (x)

void ff(TableModel *t)
{
    print(typeid(t).name());
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


    print(t.changedFields().contains("id"));
    print(t.changedFields().contains("pn"));
    print(t.changedFields().contains("point"));
    print(t.changedFields());
}

void checkExpressions()
{
    auto q1 = SampleTableModel2.id == 23;

    auto q2 = SampleTableModel2.id = 4;
    auto q3 = SampleTableModel2.id == 4 && SampleTableModel2.point == QPoint(1, 2);
    auto order = SampleTableModel2.id | !SampleTableModel2.ps;

    auto q4 = DBModel.table10.id == DBModel.table20.id;
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
    return EXIT_SUCCESS;
}
