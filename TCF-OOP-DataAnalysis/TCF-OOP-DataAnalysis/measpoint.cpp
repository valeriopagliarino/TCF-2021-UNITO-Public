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

#include "measpoint.h"

//______________________________________________________________________________
MeasPoint::MeasPoint()
{
    model = QString("");
}

//______________________________________________________________________________
// This constructor initializes the inherited TH1D filling the bins
// with the data in the events vector.
MeasPoint::MeasPoint(const QString path, bool inverting) : TH1D("Single run", "Single run", 20000, -1.0e5, 1.0e5)
{
    setInvertingAmplifier(inverting);
    loadFile(path);
    int N = getEvents().size();
    for (int i = 0; i < N; ++i)
    {
        Fill(getEvents()[i]);
    }
    GetXaxis()->SetRangeUser(getMinRange(), getMaxRange());
    model = QString("");
}

//______________________________________________________________________________
// Copy constructor
MeasPoint::MeasPoint(const MeasPoint &copy)
{
    loadFile(copy.getFilepath());
    model = QString("");
}

//______________________________________________________________________________
// Accept function for FileExport
void MeasPoint::Export(FileExport *fe, std::string opt, int index)
{
    fe->SetOpt(opt);
    fe->RunExportMp(this);
}

//______________________________________________________________________________
//  This method returns the minimum event
double MeasPoint::getMinRange()
{
    double tm = 1e10;
    for (int i = 0; i < getEvents().size(); ++i)
    {
        if (getEvents()[i] < tm)
                tm = getEvents()[i];
    }
    return tm;
}

//______________________________________________________________________________
//  This method returns the maximum event
double MeasPoint::getMaxRange()
{
    double tM = 1e-10;
    for (int i = 0; i < getEvents().size(); ++i)
    {
        if (getEvents()[i] > tM)
                tM = getEvents()[i];
    }
    return tM;
}

//______________________________________________________________________________
QString MeasPoint::getModel() const
{
    return model;
}

//______________________________________________________________________________
void MeasPoint::setModel(const QString &value)
{
    model = value;
}

//______________________________________________________________________________
std::vector<double> MeasPoint::getFitp_err()
{
    return fitp_err;
}

//______________________________________________________________________________
std::vector<double> MeasPoint::getFitp()
{
    return fitp;
}

