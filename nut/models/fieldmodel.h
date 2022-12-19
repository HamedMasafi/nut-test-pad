#ifndef FIELDMODEL_H
#define FIELDMODEL_H

#include <QDebug>
#include "../namedtype.h"
#include "../global.h"
#include "../core/fieldbase.h"
#include "phrases/conditionalphrase.h"
#include "expressions/conditionalexpression.h"

namespace Nut {

class FieldModelData : public QSharedData
{
public:
    const char *name;
    int maxLen{0};
    int len{};
    bool isPrimaryKey{false};
    std::pair<int, int> autoIncrement{std::make_pair(0, 0)};
};

class FieldModelBase
{
protected:

    void addToParent(const QString &name, Table<TableTypeModel> *parent);

public:

    const char *name() const;
    int maxLen() const;
    int len() const;
    bool isPrimaryKey() const;
    int autoIncrementStart() const;
    int autoIncrementStep() const;
protected:
    QSharedDataPointer<FieldModelData> d;

};
using namespace Nut::Model;

template<typename T>
class FieldModel : public FieldModelBase
{
public:
    template<typename... Types>
    constexpr FieldModel(TableModel *parent, const char *name, Types... args)
    {
        d = new FieldModelData();
        //        static_assert(containsType<PrimaryKey, Types...>, "no key");
        static_assert(no_unique<PrimaryKey, Types...>::value <= 1, "More than one PrimaryKey()");
//        static_assert(containsType<PrimaryKey, Types...>, "No PrimaryKey()");

//        static_assert(count("MaxLen", args...) <= -100, "max len more than once");
//        static_assert(contains<MaxLen, Types...>::value, "max len more than once");

        if (!pick<const char *>("Name", &d->name, args...))
            d->name = name;

        pick<int>("MaxLen", &d->maxLen, args...);
        pick<int>("Len", &d->len, args...);
        pick<std::pair<int, int> >("AutoIncrement", &d->autoIncrement, args...);
        pick<bool>("PrimaryKey", &d->isPrimaryKey, args...);

        qDebug() << "read values" << d->name << d->maxLen << d->len << d->isPrimaryKey;
        addToParent(name, parent);

    }

};

ConditionalExpression operator ==(const FieldModelBase &f, const QVariant &other);

template<typename T, typename... Types>
struct FieldB<T, TableTypeModel, Types...>
{
    const char *_name;
    int _maxLen{0};
    int _len{};
    bool _isPrimaryKey{false};
    std::pair<int, int> _autoIncrement{std::make_pair(0, 0)};


    constexpr FieldB(TableModel *, const char *name, Types... args)
    {

        //        static_assert(containsType<PrimaryKey, Types...>, "no key");
        static_assert(no_unique<PrimaryKey, Types...>::value <= 1, "More than one PrimaryKey()");

        //        static_assert(count("MaxLen", args...) <= -100, "max len more than once");
        //        static_assert(contains<MaxLen, Types...>::value, "max len more than once");

        if (!pick<const char *>("Name", &_name, args...))
            _name = name;

        pick<int>("MaxLen", &_maxLen, args...);
        pick<int>("Len", &_len, args...);
        pick<std::pair<int, int> >("AutoIncrement", &_autoIncrement, args...);
        pick<bool>("PrimaryKey", &_isPrimaryKey, args...);
    }
};

} // namespace Nut

#endif // FIELDMODEL_H
