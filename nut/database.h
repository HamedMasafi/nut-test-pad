#ifndef DATABASE_H
#define DATABASE_H

#include <QJsonObject>
#include <QList>
#include <QMap>
#include "global.h"


namespace Nut {

class DatasetBase;
class Database
{

public:
    QMap<QString, DatasetBase*> _tables;
    Database();

    QList<TableModel*> model() const;

    TableMain *createTable(const QString &name) const;

    QJsonObject jsonModel() const;
};

} // namespace Nut

#endif // DATABASE_H
