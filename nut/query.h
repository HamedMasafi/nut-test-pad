#pragma once

//#include <QtNut/query.h>
#include "global.h"
#include "phrase.h"

#include <QList>

namespace Nut {

struct NUT_EXPORT QueryData : public QSharedData {
    ConditionalPhrase where;
    PhraseList order;
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

    Query<T> &where(const ConditionalPhrase &ph);;
    Query<T> &orderBy(const PhraseList &ph);

    QList<DataType> toList();
};

#define QueryMethodSelf \
    template<template<Type> class T> \
    Q_OUTOFLINE_TEMPLATE Query<T> &Query<T>

#define QueryMethod(type) \
template<template<Type> class T> \
    Q_OUTOFLINE_TEMPLATE type Query<T>

QueryMethodSelf::where(const ConditionalPhrase &ph){
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
    return QList<DataType>();
}

};
