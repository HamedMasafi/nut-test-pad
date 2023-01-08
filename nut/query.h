#pragma once

//#include <QtNut/query.h>
#include "global.h"
#include "phrase.h"
#include <functional>

#include <QList>

namespace Nut {

template<Nut::Type T>
class Database;

struct NUT_EXPORT QueryData : public QSharedData {
    Database<Type::Data> *database;
    ConditionalPhrase where;
    PhraseList order;
    QList<const ForeignKeyModelBase*> joins;
    PhraseList fields;
    QString generateSelectCommand();
};

template<template<typename _T> class Container, typename ElementType>
struct appender {
    static void append();
};

template<typename ElementType>
struct appender<QList, ElementType> {
    static void append(QList<ElementType> &list, const ElementType &el) {
        list << el;
    }
};

template<typename ElementType>
struct appender<QVector, ElementType> {
    static void append(QVector<ElementType> &list, const ElementType &el) {
        list << el;
    }
};

template<typename ElementType>
struct appender<QSet, ElementType> {
    static void append(QSet<ElementType> &list, const ElementType &el) {
        list << el;
    }
};

template<template<Type> class T,
         class ListElement = T<Type::Data>,
         template<typename _T> class ListType = QList,
         typename Appender = appender<ListType, ListElement>>
class Query2
{
    std::list<int> f;
    using DataType = T<Type::Data>;
    using ModelType = T<Type::Model>;
    QSharedDataPointer<QueryData> d;
    std::function<ListElement()> _builderCallBack{};

public:
    ListType<ListElement> toList() {}
    ListElement firstOrDefault() {}

    template<typename ReturnType>
    Query2<T, ReturnType, ListType> select(const std::function<ReturnType(const ListElement &)> &cb);

protected:
    void allocate();
};

template<template<Type> class T,
         class ListElement = T<Type::Data>,
         template<typename _T> class ListType = QList>
class QuerySelector2 : public Query2<T, ListElement, ListType>
{

};

template<template<Type> class T>
class Query
{
    using DataType = T<Type::Data>;
    using ModelType = T<Type::Model>;
    QSharedDataPointer<QueryData> d;

public:
    Query<T>()
        : d{new QueryData}
    {}

    inline Query<T> &where(const ConditionalPhrase &ph);;
    inline Query<T> &orderBy(const PhraseList &ph);

    template<NUT_TABLE_TEMPLATE Table, typename KeyType>
    inline Query<T> &join(const ForeignKeyModel<Table, KeyType> &ph);

    QList<DataType> toList();
};

#define QueryMethodSelf \
    template<template<Type> class T> \
    Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>

#define QueryMethod(type) \
template<template<Type> class T> \
    Q_OUTOFLINE_TEMPLATE type Query<T>

QueryMethodSelf::where(const ConditionalPhrase &ph)
{
    if (d->where.data)
        d->where = d->where && ph;
    else
        d->where = ph;
    return *this;
}

QueryMethodSelf::orderBy(const PhraseList &ph)
{
    d->order = ph;
    return *this;
}

QueryMethod(QList<typename Query<T>::DataType>)::toList() {
    qDebug() << "select cmd=" << d->generateSelectCommand();
    return QList<DataType>();
}

template<template<Type> class T, class ListElement, template<typename _T> class ListType, typename Appender>
template<typename ReturnType>
Query2<T, ReturnType, ListType> Query2<T, ListElement, ListType, Appender>::select(
    const std::function<ReturnType(const ListElement &)> &cb)
{
    Query2<T, ReturnType, ListType> ret;
    ret._builderCallBack = cb;
    return ret;
}

template<template<Type> class T>
template<NUT_TABLE_TEMPLATE Table, typename KeyType>
Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>::join(const ForeignKeyModel<Table, KeyType> &foreignKey)
{
    d->joins << &foreignKey;
    return *this;
}



};
