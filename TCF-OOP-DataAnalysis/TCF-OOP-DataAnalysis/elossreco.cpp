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

#include "elossreco.h"

//______________________________________________________________________________
ElossReco::ElossReco()
{
    beamEnergy = new TH1D("beamEnergy", "Beam Energy", 1000, 1e6, 1e9);
}

//______________________________________________________________________________
// This constructor initializes the inherited TGraphErrors reading the TH1Ds
ElossReco::ElossReco(const std::vector<MeasPoint*> &v)
{
    beamEnergy = new TH1D("beamEnergy", "Beam Energy", 1000, 1e6, 1e9);

    for (int i = 0; i < v.size(); ++i)
    {
        SetPoint(i, v[i]->getZ(), v[i]->GetMean());
        SetPointError(i, v[i]->getZ_err(), v[i]->GetMeanError());
        beamEnergy->Fill(v[i]->getE());
    }

    Sort();
}

//______________________________________________________________________________
// Copy constructor
ElossReco::ElossReco(const ElossReco &copy)
{

}

//______________________________________________________________________________
ElossReco::~ElossReco()
{
    delete beamEnergy;
    staticToolbox::deallocateVec(mp);
}

//______________________________________________________________________________
void ElossReco::Add(MeasPoint *a)
{
    int i = GetN();
    SetPoint(i, a->getZ(), a->GetMean());
    SetPointError(i, a->getZ_err(), a->GetMeanError());
    beamEnergy->Fill(a->getE());
    mp.push_back(a);
    Sort();
}

//______________________________________________________________________________
void ElossReco::RemoveAt(const int k)
{
    RemovePoint(k);
    delete mp[k];
    staticToolbox::remove(mp, k);
    updateHistogram();
    Sort();
}

//______________________________________________________________________________
void ElossReco::DrawEloss(TCanvas *C3)
{
    C3->cd();
    Sort();
    SetMarkerColor(4);
    SetMarkerStyle(23);
    SetMarkerSize(1.3);
    C3->SetGrid();
    GetXaxis()->SetTitle("Z depth [mm]");
    Draw("AP");
}

//______________________________________________________________________________
// Accept function for FileExport
void ElossReco::Export(FileExport *fe, std::string opt, int index)
{
    fe->SetOpt(opt);
    fe->RunExportEl(this);
}

//______________________________________________________________________________
std::vector<MeasPoint *> ElossReco::getMp() const
{
    return mp;
}

//______________________________________________________________________________
std::vector<double> ElossReco::getFit()
{
    return fit;
}

//______________________________________________________________________________
std::vector<double> ElossReco::getFit_err()
{
    return fit_err;
}

//______________________________________________________________________________
QString ElossReco::getEModel() const
{
    return eModel;
}

//______________________________________________________________________________
void ElossReco::setEModel(const QString &value)
{
    eModel = value;
}

//______________________________________________________________________________
void ElossReco::updateHistogram()
{
    beamEnergy->Clear();
    for (int j = 0; j < mp.size(); ++j)
    {
        beamEnergy->Fill(mp[j]->getE());
    }
}


