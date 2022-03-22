/*===============================================================================
 * Università degli Studi di Torino - Dipartimento di Fisica
 * Corso di Tecniche di Calcolo per la Fisica A.A. 2021-2022
 *
 * [PROJECTNAME] an object-oriented data analysis tool
 *               for SiPM-based sampling calorimeter
 *
 * Copyright (C) 2022
 *
 * Authors:
 *
 * Francesco Calisto
 * Lorenzo Cane
 * Valerio Pagliarino
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ================================================================================*/

#include "liquidcaloabsorber.h"

//______________________________________________________________________________
LiquidCaloAbsorber::LiquidCaloAbsorber()
{

}

//______________________________________________________________________________
void LiquidCaloAbsorber::initializeMaterial(std::vector<materialDisplay> &reg)
{
    materialDisplay regItem;
    regItem.name = MAT_NAME;
    regItem.Z = MAT_Z;
    regItem.A = MAT_A;
    regItem.rho = MAT_RHO;
    reg.push_back(regItem);
}

//______________________________________________________________________________
QString LiquidCaloAbsorber::renderTallyMaterial(int j) const
{
    return QString("\n") + QString("/calorimeter/det/tallyMat ")
            + QString::number(j) + QString(" ") + QString::fromStdString("Water");
}

//______________________________________________________________________________
QString LiquidCaloAbsorber::renderAll(int j) const
{
    QString output;
    output = renderSize(j) + renderPosition(j) + renderTallyMaterial(j);
    return output;
}
