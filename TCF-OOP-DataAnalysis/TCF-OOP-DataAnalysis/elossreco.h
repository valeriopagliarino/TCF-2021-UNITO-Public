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

#ifndef ELOSSRECO_H
#define ELOSSRECO_H

#include<TGraphErrors.h>
#include<TH1D.h>
#include<TCanvas.h>
#include<measpoint.h>
#include<element.h>


class ElossReco : virtual public Element, public TGraphErrors
{
public:
    ElossReco();
    ElossReco(const std::vector<MeasPoint*> &v);
    ElossReco(const ElossReco &copy);
    ~ElossReco();

    void Add(MeasPoint *a);
    void RemoveAt(const int i);
    void DrawEloss(TCanvas * C);

    virtual void Export(FileExport *fe, std::string opt, int index);

    std::vector<MeasPoint*> getMp() const;
    std::vector<double> getFit(); //This getter is writable to allow push_back and clear
    std::vector<double> getFit_err(); //This getter is writable to allow push_back and clear
    QString getEModel() const;
    void setEModel(const QString &value);

private:

    TH1D * beamEnergy;
    std::vector<MeasPoint*> mp;
    std::vector<double> fit;
    std::vector<double> fit_err;
    QString eModel;
    void updateHistogram();

};


#endif // ELOSSRECO_H
