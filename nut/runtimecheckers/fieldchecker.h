#ifndef FIELDCHECKER_H
#define FIELDCHECKER_H


#include <QDebug>
#include "../namedtype.h"
#include "../global.h"
#include "../core/tablebase.h"

namespace Nut {

constexpr void addToParent(const char *name, Table<Type::RuntimeChecker> *parent);
class FieldCheckerBase
{
protected:

//    constexpr void addToParent(const char *name, Table<RuntimeChecker> *parent)
//    {
//        parent->k = true;
//    }

public:

    const char *_name;
    int _maxLen{0};
    int _len{};
    bool _isPrimaryKey{false};
    std::pair<int, int> _autoIncrement{std::make_pair(0, 0)};

};

template<typename T>
class FieldChecker : public FieldCheckerBase
{
public:
    template<typename... Types>
    constexpr FieldChecker(Table<Type::RuntimeChecker> *parent, const char *name, Types&&... args)
    {
        using namespace Nut::ModelDeclartion;

        if (!pick<const char *>("Name", &_name, args...))
            _name = name;

        pick<int>("MaxLen", &_maxLen, args...);
        pick<int>("Len", &_len, args...);
        pick<std::pair<int, int> >("AutoIncrement", &_autoIncrement, args...);
        pick<bool>("PrimaryKey", &_isPrimaryKey, args...);

        if (_isPrimaryKey) {
//            static_assert(_isPrimaryKey && parent->k, "fuck");
            parent->k = true;
        }
//            addToParent(name, parent);
    }
};

} // namespace Nut

#endif // FIELDCHECKER_H
