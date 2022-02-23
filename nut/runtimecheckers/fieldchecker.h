#ifndef FIELDCHECKER_H
#define FIELDCHECKER_H


#include <QDebug>
#include "../namedtype.h"
#include "../global.h"

namespace Nut {

class FieldCheckerBase
{
protected:

    void addToParent(const QString &name, Table<RuntimeChecker> *parent);

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
    constexpr FieldChecker(Table<RuntimeChecker> *parent, const char *name, Types&&... args)
    {
        using namespace Nut::Model;

        if (!pick<const char *>("Name", &_name, args...))
            _name = name;

        pick<int>("MaxLen", &_maxLen, args...);
        pick<int>("Len", &_len, args...);
        pick<std::pair<int, int> >("AutoIncrement", &_autoIncrement, args...);
        pick<bool>("PrimaryKey", &_isPrimaryKey, args...);

        addToParent(name, parent);
    }
};

} // namespace Nut

#endif // FIELDCHECKER_H
