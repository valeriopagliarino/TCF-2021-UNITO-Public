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

#ifndef FILEEXPORT_H
#define FILEEXPORT_H

#include<TFile.h>
#include<fileparser.h>
#include<statictoolbox.h>
#include<TCanvas.h>
#include<QString>

class Transverse;
class MeasPoint;
class ElossReco;



class FileExport
{
public:
    //FileExport();
    virtual ~FileExport() {};
    virtual void RunExportMp(MeasPoint *mp) = 0;
    virtual void RunExportEl(ElossReco *er) = 0;
    virtual void RunExportTr(Transverse *tr) = 0;
    virtual void SetOpt(const std::string opt)= 0;
    virtual void setIndex(const int index) = 0;
    //void runExportF(FitterClass<MeasPoint> *ft);


    //do be done

    //void runExport(ElossSim *es);
    //void runExport(DetectorModel *es);
    const QString exportPath = QString::fromLocal8Bit(qgetenv("EXPORT"));
    const int wc1s = 670*4;
    const int hc1s = 400*4;
};


#endif // FILEEXPORT_H

#include<measpoint.h>
#include<transverse.h>
#include<elossreco.h>

