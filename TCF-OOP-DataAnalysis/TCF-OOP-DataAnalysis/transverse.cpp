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

#include "transverse.h"

//______________________________________________________________________________
Transverse::Transverse()
{

}

//______________________________________________________________________________
Transverse::Transverse(const QString gafchromic, const double &minXmm, const double &minYmm, const double &maxXmm, const double &maxYmm)
{
    addGafchromic(gafchromic, minXmm, minYmm, maxXmm, maxYmm);
}

//______________________________________________________________________________
Transverse::Transverse(const ElossReco *elossProfile, const double &zMin, const double &zMax)
{
    addElossProfile(elossProfile, zMin, zMax);
}

//______________________________________________________________________________
Transverse::~Transverse()
{
    clear();
}

//______________________________________________________________________________
// Accept function for FileExport
void Transverse::Export(FileExport *fe, std::string opt, const int index)
{
    fe->SetOpt(opt);
    fe->setIndex(index);
    fe->RunExportTr(this);
}

//______________________________________________________________________________
void Transverse::addGafchromic(const QString gafchromic, const double &minXmm, const double &minYmm, const double &maxXmm, const double &maxYmm)
{
    if (QFileInfo::exists(gafchromic) == false) return;

    TH2D * h2 = new TH2D("h2", "Transverse Beam Image", 32, minXmm, maxXmm, 32, minYmm, maxYmm);
    QImage * bitmap = new QImage(gafchromic);

    for (int x = 0; x < bitmap->size().width(); ++x)
    {
        for (int y = 0; y < bitmap->size().height(); ++y)
        {
            double x_s = mapFunction(bitmap->size().width(), minXmm, maxXmm, x);
            double y_s = mapFunction(bitmap->size().width(), minYmm, maxYmm, y);
            QColor px = bitmap->pixel(x,y);
            int r,g,b;
            px.getRgb(&r, &g, &b);
            //Here extraction formula
            double w = (double)r;

            h2->Fill(x_s, y_s, w);
        }
    }

    h2->SetTitle(gafchromic.toStdString().c_str());
    beamTransverseImage.push_back(h2);
    currentSlice = h2;
}

//______________________________________________________________________________
void Transverse::addElossProfile(const ElossReco *elossProfile, const double &minZ, const double &maxZ)
{
    TH2D * h2 = new TH2D("h2", "Transverse Beam Image", 32, -5., 5., 32, -5., 5.);
    //Fill h2
    for (int i = 0; i < elossProfile->getMp().size(); ++i)
    {
        if (elossProfile->getMp()[i]->getZ() > minZ  && elossProfile->getMp()[i]->getZ() < maxZ)
        {
            h2->Fill(elossProfile->getMp()[i]->getX(), elossProfile->getMp()[i]->getY(), elossProfile->getMp()[i]->GetMean());
        }
    }
    QString title = QString("ElossProfile: Z = ") + QString::number(minZ) + QString(" mm");
    h2->SetTitle(title.toStdString().c_str());
    beamTransverseImage.push_back(h2);
    currentSlice = h2;
}

//______________________________________________________________________________
void Transverse::removeAt(int index)
{
    delete beamTransverseImage[index];
    staticToolbox::remove(beamTransverseImage, index);
}

//______________________________________________________________________________
void Transverse::clear()
{
    for (int i = 0; i < beamTransverseImage.size(); ++i)
        delete beamTransverseImage[i];

    beamTransverseImage.clear();
}

//______________________________________________________________________________
int Transverse::count()
{
    return getBeamTransverseImage().size();
}

//______________________________________________________________________________
TH2D * Transverse::getAt(int index)
{
    return beamTransverseImage[index];
}

//______________________________________________________________________________
double Transverse::mapFunction(const int &pixN, const double &sizeMin, const double &sizeMax, const int &crd)
{
    return ((double)crd / pixN ) * (sizeMax - sizeMin) + sizeMin;
}

//______________________________________________________________________________
TH2D *Transverse::getCurrentSlice() const
{
    return currentSlice;
}

//______________________________________________________________________________
std::vector<TH2D *> Transverse::getBeamTransverseImage() const
{
    return beamTransverseImage;
}

//______________________________________________________________________________
std::vector<double> Transverse::getFit_t()
{
    return fit_t;
}

//______________________________________________________________________________
std::vector<double> Transverse::getFit_t_err()
{
    return fit_t_err;
}

//______________________________________________________________________________
QString Transverse::getTModel() const
{
    return tModel;
}

//______________________________________________________________________________
void Transverse::setTModel(const QString &value)
{
    tModel = value;
}
