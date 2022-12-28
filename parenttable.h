#ifndef PARENTTABLE_H
#define PARENTTABLE_H

#include "nut/table.h"
#include "nut/foreignkey.h"
#include "phrases/fieldphrase_numeric.h"

NUT_FORWARD_DECLARE_TABLE(SampleTable)

using namespace Nut::ModelDeclartion;
NUT_DEFINE_TABLE(ParentTable)
{
    NUT_TABLE

public:
    ParentTable(){

    }

    Field(int, id, AutoIncrement(10, 10), PrimaryKey());

    ForeignKey(SampleTable, int, t);
};

NUT_DECLARE_TABLE(ParentTable)

#endif // PARENTTABLE_H
