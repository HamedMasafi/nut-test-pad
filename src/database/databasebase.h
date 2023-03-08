#pragma once

#include "global.h"
#include "core/typehelper.h"

namespace Nut {

template<NUT_TABLE_TEMPLATE T>
class ModelBase;

template<NUT_TABLE_TEMPLATE T>
class Dataset;

template<Nut::Type _Type>
class Database
{};

} // namespace Nut

#define NutDatabaseBase Nut::Database<_Type>
#define NUT_DATABASE \
public: \
    template<template<Nut::Type _T> class T> \
    using DatabaseTable = typename Nut::TableSetTypeHelper<T, _Type>::type; \
    static QString staticClassName(); \
    QString className() override; \
\
protected: \
    Database<Type::Model> &model() const override;

#define NUT_DECLARE_DATABASE(name) \
    extern name<Nut::Type::Model> name##Model; \
    using name##Database = name<Nut::Type::Data>; \
    namespace Nut { \
    template<> \
    name<Type::Model> &createModel<name>(); \
    } \
    template<Nut::Type _T> \
    QString name<_T>::className() \
    { \
        return #name; \
    } \
    template<Nut::Type _T> \
    QString name<_T>::staticClassName() \
    { \
        return #name; \
    } \
    template<Nut::Type _T> \
    Database<Type::Model> &name<_T>::model() const \
    { \
        return name##Model; \
    }

#define NUT_IMPLEMENT_DATABASE(name) \
    namespace Nut { \
    template<> \
    name<Type::Model> &createModel<name>() \
    { \
        return name##Model; \
    } \
    } \
    name<Nut::Type::Model> name##Model;

#define Nut_TableSet(type, name) \
    DatabaseTable<type> name \
    { \
        this, #name \
    }
