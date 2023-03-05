#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include <field2.h>
#include <table.h>

#include <QPoint>

using namespace Nut;
using namespace Nut::ModelDeclartion;
NUT_DEFINE_TABLE(User)
{
    NUT_TABLE

public:
    User() = default;
    User(const char *) {}

    Field(int, id, AutoIncrement(10, 10), PrimaryKey());

    Field(QString, name);
    Field(QString, lastName, AllowNull(true));

//    Field(int, pn, ColumnName("pn"), Len(8));
//    Field(QPoint, point);
//    Field(QPoint, pt3, AllowNull(true));
////    Field(QString, ps, ColumnName("salam"), MaxLen(100), AllowNull(true));
//    Property<QString, AllowNull> ps{this, "ps", AllowNull(true)};

//    Property2<QString, _Type> ps2{this, "ps2"};
//    FieldB<QString, _Type> salam{this, "D", 2};
};

NUT_DECLARE_TABLE(User)

#endif // SAMPLETABLE_H
