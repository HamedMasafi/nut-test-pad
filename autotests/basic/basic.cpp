#include "basic.h"
#include <QPoint>
#include <QtTest>
#include "db.h"
#include "user.h"
#include <generators/sqlitegenerator.h>
#include <iostream>
#define print(x) qDebug() << #x "=" << (x)

//void * operator new(size_t size)
//{
//    std::cout << "samap" << std::endl;
//    void * p = malloc(size);
//    return p;
//}

class NoBase{
public:
    void no(){}
};
class PrimaryKeyBase{
public:
    void foo(){}
};

template<bool is>
struct Base{
    using type = NoBase;
};

template<>
struct Base<true>{
    using type = PrimaryKeyBase;
};


template<typename... Types>
    class FieldTest : public Base<Nut::containsType<PrimaryKey, Types...>>::type
{
public:
    FieldTest(Types...args){

    }
    void f2(){}
};
void BasicTest::checkDeclartions()
{
    FieldTest fp{PrimaryKey()};
    fp.foo();
    using namespace Nut;
    using namespace Nut::ModelDeclartion;
    auto row = Nut::createRow<Post>();
    Nut::PropertyTypeHelper<int, Nut::Type::Data, PrimaryKey, AllowNull>::type f(row.data(), nullptr);
    Nut::PropertyTypeHelper<int, Nut::Type::Data>::type f3(row.data(),
                                                          nullptr,
                                                          PrimaryKey(),
                                                          AllowNull(true));

    Nut::Field<int, true, false> f2(row.data(), nullptr);

    auto s = std::is_same<PrimaryKey, decltype(PrimaryKey())>::value;
    QCOMPARE(s, true);
    QCOMPARE(f3.isPrimary(), true);
    QCOMPARE(f2.isPrimary(), true);
    QCOMPARE(f.isPrimary(), true);

    auto n =  no_unique<PrimaryKey, PrimaryKey, AllowNull>::value;
    QCOMPARE(n, 1);
}

void BasicTest::checkPrimary()
{
    QCOMPARE(DBModel.posts.id.isPrimaryKey(), true);
    QCOMPARE(DBModel.posts.content.isPrimaryKey(), false);

    auto post = Nut::createRow<Post>();
    QCOMPARE(post->id.isPrimary(), true);
    QCOMPARE(post->content.isPrimary(), false);
//    QCOMPARE(DBModel.posts.user.isPrimaryKey(), true);
//    QCOMPARE(DBModel.posts.id.isPrimaryKey(), true);
}

void BasicTest::insert()
{
    DBDatabase db;
    db.setDriver("QSQLITE");
    db.setDatabaseName("data.dat");
    QVERIFY(db.open());

    auto user = Nut::createRow<User>();
    user->name = "Hamed";
    user->lastName = "Masafi";
    db.users.append(user);
    db.saveChanges();

//    DBModel.users.posts.primaryField();
}

void BasicTest::insertWithChilds()
{
    DBDatabase db;
    db.setDriver("QSQLITE");
    db.setDatabaseName("data.dat");
    QVERIFY(db.open());

    auto user = Nut::createRow<User>();
    user->name = "Hamed";
    user->lastName = "Masafi";
    db.users.append(user);

    auto post = Nut::createRow<Post>();
    post->content = "salam";
    user->posts.append(post);
    db.saveChanges();
}

void BasicTest::modelTest() {}

void BasicTest::changedTest()
{
//    UserTable t;
//    t.id = 3;
//    t.pn = 4;
//    t.point = QPoint(1, 2);

//    Nut::SqliteGenerator gen{nullptr};
//    gen._database = &DBModel;
//    auto sql = gen.saveRecord(&t, "sample");
//    //    QCOMPARE(sql, "INSERT INTO sample (pn, point) VALUES ('4', '1,2')");
//    QVERIFY(t.changedFields().contains("id"));
//    QVERIFY(t.changedFields().contains("pn"));
//    QVERIFY(t.changedFields().contains("point"));
}

void BasicTest::className()
{
    QCOMPARE(DBModel.users.className(), "User");
}

void BasicTest::checkExpressions()
{
    auto q1 = UserModel2.id == 23;

    auto q2 = UserModel2.id = 4;
//    auto q3 = DBModel.users.id == 4 && DBModel.users.point == QPoint(1, 2);
//    auto order = UserModel2.id | !UserModel2.ps;

    auto q4 = DBModel.users.id == DBModel.users.id;

//    Nut::SqliteGenerator gen;
//    auto where = gen.createConditionalPhrase(q3.data);
//    QCOMPARE(where, "([table20].id = '4' AND [table10].point = '1,2')");
}

void BasicTest::join()
{
    DBDatabase db;
    db.setDriver("QSQLITE");
    db.setDatabaseName("data.dat");
    QVERIFY(db.open());

    auto q = db.posts.query()
                 .join(DBModel.posts.user)
                 .orderBy(!DBModel.posts.id)
                 .skip(10)
                 .take(5)
                 .toList();
}

QTEST_APPLESS_MAIN(BasicTest)
