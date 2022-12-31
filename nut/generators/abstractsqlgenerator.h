/**************************************************************************
**
** This file is part of Nut project.
** https://github.com/HamedMasafi/Nut
**
** Nut is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Nut is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with Nut.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#pragma once

#include <QtCore/qglobal.h>
#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "phrase.h"
#include "global.h"

namespace Nut {


class FieldModelBase;

template<Nut::Type _Type>
class Database;

struct RelationModel;
class SqlSerializer;

class NUT_EXPORT AbstractSqlGenerator : public QObject
{
//    Q_OBJECT

    Database<Type::Model> *_database;
protected:
    SqlSerializer *_serializer;

    bool isNumeric(const QMetaType::Type &type);

public:
    //TODO: remove this enum
    enum CommandType{
        Select,
        Insert,
        Update,
        Delete
    };
    enum AgregateType{
//        SelectAll,
        Count,
        Min,
        Max,
        Average,
        SingleField,
        Sum
    };

    explicit AbstractSqlGenerator(Database<Type::Model> *parent);
    virtual ~AbstractSqlGenerator() = default;

    virtual bool supportPrimaryKey(const QMetaType &type) {
        Q_UNUSED(type)
        return true;
    }
    virtual bool supportAutoIncrement(const QMetaType &type) {
        Q_UNUSED(type)
        return true;
    }

    //fields
    virtual QString fieldType(AbstractFieldPhrase *field) = 0;
    virtual QString fieldDeclare(AbstractFieldPhrase *field);
    virtual QString escapeFieldName(const QString &fieldName) const;
    virtual QStringList constraints(AbstractTableModel *table);
    virtual QString escapeValue(const QVariant &v) const;
    virtual QVariant unescapeValue(const QMetaType &type, const QVariant &dbValue);

    virtual QString masterDatabaseName(QString databaseName);

    virtual QString createTable(TableModel *table);

    virtual QString relationDeclare(const RelationModel *relation);

    virtual QStringList diffDatabase(const DatabaseModel &lastModel, const DatabaseModel &newModel);
    virtual QString diffField(AbstractFieldPhrase *oldField, AbstractFieldPhrase *newField);
    virtual QStringList diffTable(AbstractTableModel *oldTable, AbstractTableModel *newTable);
    virtual QStringList diffRelation(AbstractTableModel *oldTable, AbstractTableModel *newTable);
    virtual QStringList diffRelation2(RelationModel *oldRel, RelationModel *newRel);

    virtual QString join(const QString &mainTable,
                         const QList<RelationModel*> &list,
                         QStringList *order = Q_NULLPTR);
    virtual QString join(const QStringList &list, QStringList *order = Q_NULLPTR);

    virtual QString saveRecord(TableRow *t, QString tableName);

    virtual QString recordsPhrase(TableModel *table);

    virtual QString insertBulk(const QString &tableName, const PhraseList &ph, const QList<QVariantList> &vars);
    virtual QString insertRecord(TableRow *t, QString tableName);
    virtual QString updateRecord(TableRow *t, QString tableName);
    virtual QString deleteRecord(TableRow *t, QString tableName);
    virtual QString deleteRecords(const QString &tableName, const QString &where);

    virtual QString selectCommand(const QString &tableName,
                                  const PhraseList &fields,
                                  const ConditionalPhrase &where,
                                  const PhraseList &order,
                                  const QList<RelationModel *> &joins,
                                  const int skip = -1,
                                  const int take = -1);

    virtual QString selectCommand(const QString &tableName,
                                  const AgregateType &t,
                                  const QString &agregateArg,
                                  const ConditionalPhrase &where,
                                  const QList<RelationModel *> &joins,
                                  const int skip = -1,
                                  const int take = -1);

    virtual QString deleteCommand(const QString &tableName,
                                  const ConditionalPhrase &where);
    virtual QString updateCommand(const QString &tableName,
                                  const AssignmentPhraseList &assigments,
                                  const ConditionalPhrase &where);

    virtual QString insertCommand(const QString &tableName,
                                  const AssignmentPhraseList &assigments);
//    virtual QString selectCommand(AgregateType t,
//                                  QString agregateArg, QString tableName,
//                                  QList<WherePhrase> &wheres,
//                                  QList<WherePhrase> &orders,
//                                  QList<RelationModel*> joins,
//                                  int skip = -1, int take = -1);

//    virtual QString deleteCommand(QList<WherePhrase> &wheres, QString tableName);

//    virtual QString updateCommand(WherePhrase &phrase, QList<WherePhrase> &wheres, QString tableName);

    virtual QString phrase(const PhraseData *d) const;
    virtual QString operatorString(const PhraseData::Condition &cond) const;
    virtual void appendSkipTake(QString &sql, int skip = -1, int take = -1);
    virtual QString primaryKeyConstraint(const AbstractTableModel *table) const;

protected:
    virtual QString createConditionalPhrase(const PhraseData *d) const;
    QString createFieldPhrase(const PhraseList &ph);
    QString createOrderPhrase(const PhraseList &ph);
    void createInsertPhrase(const AssignmentPhraseList &ph, QString &fields, QString &values);

    QString agregateText(const AgregateType &t, const QString &arg = QString()) const;
    QString fromTableText(const QString &tableName, QString &joinClassName, QString &orderBy) const;
//    QString createWhere(QList<WherePhrase> &wheres);

    Q_DECL_DEPRECATED
    virtual void replaceTableNames(QString &command);
    Q_DECL_DEPRECATED
    void removeTableNames(QString &command);
    QString dateTimePartName(const PhraseData::Condition &op) const;
};

}
