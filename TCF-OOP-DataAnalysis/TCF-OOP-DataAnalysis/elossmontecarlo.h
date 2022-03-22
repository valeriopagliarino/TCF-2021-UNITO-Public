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

#ifndef ELOSSMONTECARLO_H
#define ELOSSMONTECARLO_H

#define _ASYNC_ false //blocking external calls to Geant4

#include<stdlib.h>
#include<iostream>
#include<calorimeter.h>
#include<TGraphErrors.h>
#include<TFile.h>
#include<TCanvas.h>
#include<TH1D.h>

#include<QProcess>
#include<QProcessEnvironment>
#include<QSystemSemaphore>
#include<QString>
#include<QByteArray>
#include<QFile>
#include<QFileInfo>
#include<QDebug>
#include<QStringList>

struct G4ion
{
  int A, Z, Q;
};

class ElossMonteCarlo : public TH1D
{
public:
    ElossMonteCarlo();
    ~ElossMonteCarlo();

    void runMacro(QString macro);
    QString parseOutput();

    QProcess *getGeant4() const;
    QString getSimLog() const;

    QString synthetizeMacro();
    Calorimeter *getCalorimeter() const;

    int getNEvents() const;
    void setNEvents(int value);
    double getBoxX() const;
    void setBoxX(double value);
    double getBoxYZ() const;
    void setBoxYZ(double value);
    double getMaxStep() const;
    void setMaxStep(double value);
    unsigned int getPrintProgress() const;
    void setPrintProgress(unsigned int value);
    signed int getVerbosity() const;
    void setVerbosity(signed int value);
    bool getPrintMaterials() const;
    void setPrintMaterials(bool value);
    double getEnergyMeanMeV() const;
    void setEnergyMeanMeV(double value);
    double getEnergySigmaMeV() const;
    void setEnergySigmaMeV(double value);
    double getBeamFWHM() const;
    void setBeamFWHM(double value);
    G4ion getG4ion() const;
    void setG4ion(const G4ion &value);
    void clearModel();

private:
    QProcess *geant4;
    QProcessEnvironment env;
    Calorimeter * calorimeter;
    void processOutput();
    QString simLog;
    QString textOutput = QString("Calorimeter.out");
    QString rootOutput = QString("Calorimeter.root");
    QString grapOutput = QString("g4_0000");
    TCanvas * cOut;
    TH1D    * hOut;

    int             nEvents;
    double          boxX;
    double          boxYZ;
    double          maxStep;
    unsigned int    printProgress;
    signed int      verbosity;
    bool            printMaterials;

    double          energyMeanMeV;
    double          energySigmaMeV;
    G4ion           g4ion;
    double          beamFWHM;

};

#endif // ELOSSMONTECARLO_H


