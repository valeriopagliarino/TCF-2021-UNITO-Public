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

#include "detectormodel.h"

//______________________________________________________________________________
DetectorModel::DetectorModel()
{

}

//______________________________________________________________________________
DetectorModel::~DetectorModel()
{
    clear();
}

//______________________________________________________________________________
void DetectorModel::addLayer(Absorber *layer)
{
    layers.push_back(layer);
}

//______________________________________________________________________________
void DetectorModel::removeLayer(int index)
{
    staticToolbox::remove(layers, index);
}

//______________________________________________________________________________
void DetectorModel::clear()
{
    while(layers.size() > 0)
    {
        staticToolbox::remove(layers, 0);
    }
}

//______________________________________________________________________________
QString DetectorModel::modelSynthesis()
{
    int N =  layers.size();
    synthesisCache = QString("");
    synthesisCache = synthesisCache + QString("\n/calorimeter/det/tallyNumber ")
            + QString::number(N);

    for (int i = 0; i < N; ++i)
    {
        synthesisCache = synthesisCache + QString("\n") +
                layers[i]->renderAll(i);
    }

    return synthesisCache;
}













