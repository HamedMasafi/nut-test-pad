#ifndef FIELDMODEL_H
#define FIELDMODEL_H

#include <QDebug>
#include "../namedtype.h"
#include "../global.h"
#include "../core/fieldbase.h"

namespace Nut {

class FieldModelBase
{
protected:

    void addToParent(const QString &name, Table<TableTypeModel> *parent);

public:

    const char *_name;
    int _maxLen{0};
    int _len{};
    bool _isPrimaryKey{false};
    std::pair<int, int> _autoIncrement{std::make_pair(0, 0)};

};
using namespace Nut::Model;

template<typename T>
class FieldModel : public FieldModelBase
{
public:
    template<typename... Types>
    constexpr FieldModel(TableModel *parent, const char *name, Types... args)
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

        addToParent(name, parent);
    }
};

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
