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

#ifndef SCINTABSORBER_H
#define SCINTABSORBER_H
#define MAT_NAME std::string("CAF2")
#define MAT_A std::string("---")
#define MAT_Z std::string("1x20, 2x9")
#define MAT_RHO 3180

#include<absorber.h>


class ScintAbsorber : public Absorber
{
public:
    ScintAbsorber();

    QString renderTallyMaterial(int j) const;
    QString renderAll(int j) const;
    static void initializeMaterial(std::vector<materialDisplay> &reg);


    int getNrows() const;
    void setNrows(int value);
    int getNcols() const;
    void setNcols(int value);
    double getCellHsize() const;
    void setCellHsize(double value);
    double getCellVsize() const;
    void setCellVsize(double value);

private:
    int nrows = 1;
    int ncols = 1;

    double cellHsize = 3; // mm
    double cellVsize = 3; // mm
};

#endif // SCINTABSORBER_H
