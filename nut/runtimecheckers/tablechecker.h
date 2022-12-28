#ifndef TABLECHECKER_H
#define TABLECHECKER_H

class FieldCheckerBase;

#include "../global.h"
#include <array>

namespace Nut {

template <>
class Table<Type::RuntimeChecker>
{
    std::array<FieldCheckerBase*, 0> _fields;
    FieldCheckerBase *d{nullptr};

    struct Link {
        FieldCheckerBase *data;
        Link *next {nullptr};
    };
    Link *first{nullptr};
    Link *last{nullptr};

public:
    bool k{};
    constexpr Table() {
//        if (first != nullptr)
//            throw std::logic_error( "is not null");
    }

    friend class FieldCheckerBase;
};
}


#endif // TABLECHECKER_H
