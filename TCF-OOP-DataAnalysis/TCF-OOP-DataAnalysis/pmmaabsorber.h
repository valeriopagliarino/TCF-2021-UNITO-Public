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

#ifndef PMMAABSORBER_H
#define PMMAABSORBER_H
#define MAT_NAME std::string("PMMA")
#define MAT_A std::string("---")
#define MAT_Z std::string("5x6, 2x2, 8x1")
#define MAT_RHO 1180

#include<absorber.h>


class PmmaAbsorber : public Absorber
{
public:
    PmmaAbsorber();

    QString renderTallyMaterial(int j) const;
    QString renderAll(int j) const;
    static void initializeMaterial(std::vector<materialDisplay> &reg);

};

#endif // PMMAABSORBER_H
