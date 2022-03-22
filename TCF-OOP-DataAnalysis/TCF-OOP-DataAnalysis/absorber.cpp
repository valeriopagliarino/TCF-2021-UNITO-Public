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

#include "absorber.h"


//______________________________________________________________________________
Absorber::Absorber()
{

}

//______________________________________________________________________________
Absorber::~Absorber()
{

}

//______________________________________________________________________________
QString Absorber::renderPosition(int j) const
{
    return QString("\n/calorimeter/det/tallyPosition ") + QString::number(j)
           + QString(" ") + QString::number(zpos)
           + QString(" ") + QString::number(ypos)
           + QString(" ") + QString::number(xpos)
           + QString(" mm");
}

//______________________________________________________________________________
QString Absorber::renderSize(int j) const
{
    return QString("\n/calorimeter/det/tallyDefinition ") + QString::number(j)
           + QString(" ") + QString::number(tickness)
           + QString(" ") + QString::number(heigh)
           + QString(" ") + QString::number(width)
           + QString(" mm");
}

double Absorber::getHeigh() const
{
    return heigh;
}

void Absorber::setHeigh(double value)
{
    heigh = value;
}

double Absorber::getWidth() const
{
    return width;
}

void Absorber::setWidth(double value)
{
    width = value;
}

double Absorber::getTickness() const
{
    return tickness;
}

void Absorber::setTickness(double value)
{
    tickness = value;
}

double Absorber::getXpos() const
{
    return xpos;
}

void Absorber::setXpos(double value)
{
    xpos = value;
}

double Absorber::getYpos() const
{
    return ypos;
}

void Absorber::setYpos(double value)
{
    ypos = value;
}

double Absorber::getZpos() const
{
    return zpos;
}

void Absorber::setZpos(double value)
{
    zpos = value;
}

int Absorber::getTallyIndex() const
{
    return tallyIndex;
}

void Absorber::setTallyIndex(int value)
{
    tallyIndex = value;
}
