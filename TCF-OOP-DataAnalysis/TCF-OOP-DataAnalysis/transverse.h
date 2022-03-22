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

#ifndef Transverse_H
#define Transverse_H

#include<QString>
#include<QImage>
#include<QPixmap>
#include<QRgb>
#include<QPixelFormat>
#include<QFileInfo>
#include<QFile>

#include<TH2D.h>
#include<TProfile2D.h>
#include<TGraph2DErrors.h>

#include<elossreco.h>
#include<element.h>
#include<statictoolbox.h>


class Transverse :  public Element, public TH2D
{
public:
    Transverse();
    Transverse(const QString gafchromic, const double &minXmm, const double &minYmm, const double &maxXmm, const double &maxYmm);
    Transverse(const ElossReco *elossProfile, const double &zMin, const double &zMax);
    ~Transverse();

    void Export(FileExport *fe, std::string opt, const int index);

    void addGafchromic(QString const gafchromic, const double &minXmm, const double &minYmm, const double &maxXmm, const double &maxYmm);
    void addElossProfile(ElossReco const *elossProfile, const double &minZ, const double &maxZ);
    void removeAt(int index);
    void clear();
    int count();
    TH2D *getAt(int index);
    TH2D *getCurrentSlice() const;
    std::vector<TH2D *> getBeamTransverseImage() const;

    std::vector<double> getFit_t(); //This getter is writable to allow push_back and clear
    std::vector<double> getFit_t_err(); //This getter is writable to allow push_back and clear
    QString getTModel() const;
    void setTModel(const QString &value);

private:
    std::vector<TH2D*> beamTransverseImage;
    double mapFunction(const int &pixN, const double &sizeMin, const double &sizeMax, const int &crd);
    TH2D * currentSlice;
    std::vector<double> fit_t;
    std::vector<double> fit_t_err;
    QString tModel;

};

#endif // Transverse_H
