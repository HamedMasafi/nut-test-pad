
#include "query.h"
#include "database.h"
#include "generators/abstractsqlgenerator.h"

namespace Nut {
QString QueryData::generateSelectCommand() {
    auto gen = database->generator();
    sql = gen->selectCommand(this);
    return sql;
}


template<typename ElementType>
struct appender<QList, ElementType> {
    static void append(QList<ElementType> &list, const ElementType &el);
};

//template<typename ElementType>
//struct appender<QVector, ElementType> {
//    static void append(QVector<ElementType> &list, const ElementType &el);
//};

template<typename ElementType>
struct appender<QSet, ElementType> {
    static void append(QSet<ElementType> &list, const ElementType &el);
};

template<typename ElementType>
void appender<QList, ElementType>::append(QList<ElementType> &list, const ElementType &el) {
    list << el;
}


template<typename ElementType>
void appender<QSet, ElementType>::append(QSet<ElementType> &list, const ElementType &el) {
    list << el;
}

//template<typename ElementType>
//void appender<QVector, ElementType>::append(QVector<ElementType> &list, const ElementType &el) {
//    list << el;
//}

} // namespace Nut
