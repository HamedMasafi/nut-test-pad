#ifndef SAMPLETABLE_H
#define SAMPLETABLE_H

#include "database.h"
#include <field2.h>
#include <table.h>
#include "post.h"
#include <QPoint>

//NUT_FORWARD_DECLARE_TABLE(Post)

//class Post;
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

//    ChildTableSet(Post, posts);
//    QList<Post<Nut::Model>> _list;
//    DatabaseTable<Post> posts{this, "posts", RelationName("users")};
    ChildTableSet(Post, posts, RelationName("user"));
    //    Nut_TableSet(Post, posts);

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
