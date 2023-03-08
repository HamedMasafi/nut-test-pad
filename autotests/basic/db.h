#pragma once

#include "database.h"
#include "dataset.h"
#include "user.h"
#include "post.h"

NUT_ENTITY
class DB : public NutDatabaseBase
{
    NUT_DATABASE

public:
    DB() = default;

    Nut_TableSet(User, users);
    Nut_TableSet(Post, posts);
//    DatabaseTable dd{Post, ""}
};

NUT_DECLARE_DATABASE(DB);

