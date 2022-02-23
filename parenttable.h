#ifndef PARENTTABLE_H
#define PARENTTABLE_H

#include "nut/table.h"
#include "nut/foreignkey.h"

NUT_FORWARD_DECLARE_TABLE(SampleTable)

using namespace Nut::Model;
NUT_DEFINE_TABLE(ParentTable)
{
    NUT_TABLE

public:
    ParentTable(){

    }

    Field(int, id, AutoIncrement(10, 10), PrimaryKey());

    ForeignKeyC(SampleTable, int, t);
};

NUT_DECLARE_TABLE(ParentTable);

#endif // PARENTTABLE_H
