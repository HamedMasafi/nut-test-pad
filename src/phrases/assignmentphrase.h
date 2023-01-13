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

#include "assignmentphraselist.h"
#include "global.h"

#include <QVariant>

QT_BEGIN_NAMESPACE

namespace Nut
{

class PhraseData;
class AbstractFieldPhrase;
class NUT_EXPORT AssignmentPhrase
{
public:
    PhraseData *data;
    explicit AssignmentPhrase(PhraseData *d);
    explicit AssignmentPhrase(AbstractFieldPhrase *l, const QVariant r);
    explicit AssignmentPhrase(AbstractFieldPhrase *l, const AssignmentPhrase *r);
    explicit AssignmentPhrase(AssignmentPhrase *ph, const QVariant &v);
//    explicit AssignmentPhrase(AssignmentPhrase &other);
    ~AssignmentPhrase();
//    AssignmentPhrase(AssignmentPhrase *l, const AssignmentPhrase *r);

    AssignmentPhraseList operator &(const AssignmentPhrase &other);

};

} // namespace Nut
