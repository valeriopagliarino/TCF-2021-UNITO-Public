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

#ifndef ABSORBER_H
#define ABSORBER_H

#include<QString>
#include<string>
#include<vector>

struct materialDisplay
{
    std::string name;
    std::string Z;
    std::string A;
    double rho;
};

class Absorber
{

public:
    Absorber();
    ~Absorber();
    QString renderPosition(int j) const;
    QString renderSize(int j) const;
    virtual QString renderTallyMaterial(int j) const = 0;
    virtual QString renderAll(int j) const = 0;

    double getHeigh() const;
    void setHeigh(double value);
    double getWidth() const;
    void setWidth(double value);
    double getTickness() const;
    void setTickness(double value);
    double getXpos() const;
    void setXpos(double value);
    double getYpos() const;
    void setYpos(double value);
    double getZpos() const;
    void setZpos(double value);
    int getTallyIndex() const;
    void setTallyIndex(int value);

private:
    double heigh, width, tickness; // mm
    double xpos, ypos, zpos; // mm
    int tallyIndex;
};

#endif // ABSORBER_H
