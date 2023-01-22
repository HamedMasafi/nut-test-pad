#pragma once

#include "global.h"

namespace Nut {

class AbstractSqlGenerator;
class DatabaseData;

template<>
class Database<Type::Data>
{
    QSharedDataPointer<DatabaseData> d;

    AbstractSqlGenerator *_generator{nullptr};

public:
    AbstractSqlGenerator *generator();
};

} // namespace Nut
