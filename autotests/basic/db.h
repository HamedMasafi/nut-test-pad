#ifndef DB_H
#define DB_H

#include "database.h"
#include "dataset.h"
#include "user.h"
#include "post.h"

NUT_ENTITY
class DB : public NutDatabaseBase
{
public:
    DB() = default;
    NUT_DATABASE

    Nut_TableSet2(User, users);
    Nut_TableSet2(Post, posts);
//    SampleTable<Nut::Type::Model> dddd{this, ""};
};

NUT_DECLARE_DATABASE(DB);

#endif // DB_H
