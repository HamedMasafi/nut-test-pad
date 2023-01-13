#include "basic.h"
#include <QPoint>
#include <QtTest>
#include "db.h"
#include "sampletable.h"
#include <generators/sqlitegenerator.h>
#include <iostream>
#define print(x) qDebug() << #x "=" << (x)

//void * operator new(size_t size)
//{
//    std::cout << "samap" << std::endl;
//    void * p = malloc(size);
//    return p;
//}

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
//    QCOMPARE(sql, "INSERT INTO sample (pn, point) VALUES ('4', '1,2')");

    QCOMPARE(t.changedFields().size(), 3);
    QVERIFY(t.changedFields().contains("id"));
    QVERIFY(t.changedFields().contains("pn"));
    QVERIFY(t.changedFields().contains("point"));
}

void BasicTest::className()
{
    QCOMPARE(DBModel.sampleTable.className(), "SampleTable");
}

void BasicTest::checkExpressions()
{
    auto q1 = SampleTableModel2.id == 23;

    auto q2 = SampleTableModel2.id = 4;
    auto q3 = DBModel.table20.id == 4 && DBModel.table10.point == QPoint(1, 2);
    auto order = SampleTableModel2.id | !SampleTableModel2.ps;

    auto q4 = DBModel.table10.id == DBModel.table20.id;

    Nut::SqliteGenerator gen;
    auto where = gen.createConditionalPhrase(q3.data);
    QCOMPARE(where, "([table20].id = '4' AND [table10].point = '1,2')");
}

QTEST_APPLESS_MAIN(BasicTest)
