#include "databasedata.h"
#include "generators/sqlitegenerator.h"

namespace Nut {

AbstractSqlGenerator *Database<Type::Data>::generator()
{
    if (!_generator) {
        _generator = new SqliteGenerator();
        //        _generator->_database = this;
    }
    return _generator;
}

} // namespace Nut
