#pragma once

#include "global.h"

namespace Nut {

class AbstractSqlGenerator;
template<>
class Database<Type::Data>
{
    AbstractSqlGenerator *_generator{nullptr};

public:
    AbstractSqlGenerator *generator();
};

} // namespace Nut
