#ifndef PARENTTABLE_H
#define PARENTTABLE_H

#include "table.h"

NUT_FORWARD_DECLARE_TABLE(User)

using namespace Nut::ModelDeclartion;
NUT_DEFINE_TABLE(Post)
{
    NUT_TABLE

public:
    Post() = default;
    Post(const char *) {}

    Field(int, id, AutoIncrement(10, 10), PrimaryKey());
    Field(QString, content);

    ForeignKey(User, int, user);
};

NUT_DECLARE_TABLE(Post)

#endif // PARENTTABLE_H
