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


#ifndef CALORIMETER_H
#define CALORIMETER_H

#include<absabsorber.h>
#include<detectormodel.h>


class Calorimeter : public DetectorModel
{
public:
    Calorimeter();

    void buildBeamDeliveryPMT(double D0mm, double TicknessMm, Absorber *mat, double &zeroRef, int index);
    void buildCalorimeterWall(double D1mm, double TicknessMm, Absorber *mat, double &zeroRef, int index);
    void buildSelaedNacelWall(double TicknessMm, Absorber *mat, double &zeroRef, int index);
    void buildCalorimeterAbso(double TicknessMm, Absorber *mat, double &zeroRef, int index);
    void buildScintillatorXtl(double D3mm, double TicknessMm, Absorber *mat, double &zeroRef, int nx, int ny, double sizeYZ, int index);

    //Acceptor for visitor pattern file saver
};

#endif // CALORIMETER_H
