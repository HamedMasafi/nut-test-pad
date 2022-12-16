#ifndef GLOBAL_H
#define GLOBAL_H

#define NUT_TABLE_TEMPLATE template<Nut::TableType _T> class
#define NUT_TABLE_TEMPLATE_MAIN template<Nut::TableType _T = TableTypeMain> class

namespace Nut {

enum TableType {
    TableTypeMain,
    TableTypeModel,
    TableTypeFieldPhrases,
    RuntimeChecker
};

template<template<Nut::TableType> class T>
T<Nut::TableTypeModel> *createModel()
{
    return nullptr;
}

template <TableType _Type>
class Table;
using TableMain = Table<TableTypeMain>;
using TableModel = Table<TableTypeModel>;

template<template<TableType _Type> class T>
using Table_T = T<TableTypeMain>;

} // namespace Nut

#endif // GLOBAL_H
