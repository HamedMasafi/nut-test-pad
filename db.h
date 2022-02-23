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

#endif // DB_H
