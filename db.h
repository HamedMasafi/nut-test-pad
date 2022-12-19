#ifndef DB_H
#define DB_H

#include "nut/database.h"
#include "nut/dataset.h"
#include "sampletable.h"
#include "parenttable.h"

class DB : public Nut::Database
{
public:
    DB();

    Nut_TableSet(SampleTable, table1);
    Nut_TableSet(ParentTable, table2);
};


NUT_ENTITY
class DB2 : public NutDatabaseBase
{
public:
    DB2() = default;
    NUT_DATABASE

    Nut_TableSet2(SampleTable, table10);
    Nut_TableSet2(ParentTable, table20);
    Tableset<SampleTable> ddd{this, "new-name"};
//    SampleTable<Nut::TableTypeModel> dddd{this, ""};
};

NUT_DECLARE_DATABASE(DB2);

#endif // DB_H
