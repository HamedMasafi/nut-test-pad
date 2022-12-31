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

#include "global.h"

#include "abstractsqlgenerator.h"

namespace Nut {


class NUT_EXPORT SqliteGenerator : public AbstractSqlGenerator
{
public:
    explicit SqliteGenerator(Database<Type::Model> *parent = nullptr);

    QString fieldType(AbstractFieldPhrase *field) override;
    QString fieldDeclare(AbstractFieldPhrase *field) override;

    bool supportAutoIncrement(const QMetaType &type) override;

    void appendSkipTake(QString &sql, int skip, int take) override;

    QString primaryKeyConstraint(const AbstractTableModel *table) const override;
    QStringList diffTable(AbstractTableModel *oldTable, AbstractTableModel *newTable) override;

    QString createConditionalPhrase(const PhraseData *d) const override;

    QString escapeValue(const QVariant &v) const override;
    QVariant unescapeValue(const QMetaType &type, const QVariant &dbValue) override;
};

} // namespace Nut
