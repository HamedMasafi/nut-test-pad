#include "basic.h"
#include <QPoint>
#include <QtTest>
#include "db.h"
#include "sampletable.h"
#include <generators/sqlitegenerator.h>

void BasicTest::modelTest() {}

void BasicTest::changedTest()
{
    SampleTableTable t;
    t.id = 3;
    t.pn = 4;
    t.point = QPoint(1, 2);

    Nut::SqliteGenerator gen;
    gen._database = &DBModel;
    auto sql = gen.saveRecord(&t, "sample");
//    print(sql);

    QVERIFY(t.changedFields().contains("id"));
    QVERIFY(t.changedFields().contains("pn"));
    QVERIFY(t.changedFields().contains("point"));
    QVERIFY(t.changedFields().size() == 4);
}

QTEST_APPLESS_MAIN(BasicTest)
