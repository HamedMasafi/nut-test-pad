#ifndef DB_H
#define DB_H

#include "database.h"
#include "dataset.h"
#include "sampletable.h"
#include "parenttable.h"

NUT_ENTITY
class DB : public NutDatabaseBase
{
public:
    DB() = default;
    NUT_DATABASE

    Nut_TableSet2(SampleTable, table10);
    Nut_TableSet2(ParentTable, table20);
    Tableset<SampleTable> sampleTable{this, "sampleTable"};
//    SampleTable<Nut::Type::Model> dddd{this, ""};
};

NUT_DECLARE_DATABASE(DB);

#endif // DB_H
