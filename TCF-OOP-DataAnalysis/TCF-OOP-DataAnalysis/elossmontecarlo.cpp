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

#include "elossmontecarlo.h"

//______________________________________________________________________________
ElossMonteCarlo::ElossMonteCarlo()
{
    geant4 = new QProcess();
    calorimeter = new Calorimeter();
}

//______________________________________________________________________________
ElossMonteCarlo::~ElossMonteCarlo()
{
    delete geant4;
    delete hOut;
    delete cOut;
    delete calorimeter;
}

//______________________________________________________________________________
void ElossMonteCarlo::runMacro(QString macro)
{
    qDebug() << "\nAttempting to launch macro\n";
    QString G4ENSDFSTATEDATA;
    QString G4LEDATA;
    QString G4RUN;
    QString Geant4_DIR;
    QString G4BASEF;
    QString G4CALL;
    QString TEMP;

    G4ENSDFSTATEDATA    = QString::fromLocal8Bit(qgetenv("G4ENSDFSTATEDATA"));
    G4LEDATA            = QString::fromLocal8Bit(qgetenv("G4LEDATA"));
    G4RUN               = QString::fromLocal8Bit(qgetenv("G4RUN"));
    Geant4_DIR          = QString::fromLocal8Bit(qgetenv("Geant4_DIR"));
    G4BASEF             = QString::fromLocal8Bit(qgetenv("G4BASEF"));
    TEMP                = QString::fromLocal8Bit(qgetenv("TEMPFOLDER"));
    G4CALL              = G4RUN + QString(" ") + macro;

    setenv("G4ENSDFSTATEDATA", G4ENSDFSTATEDATA.toStdString().c_str(), 1);
    setenv("G4LEDATA", G4LEDATA.toStdString().c_str(), 1);
    setenv("G4RUN", G4RUN.toStdString().c_str(), 1);
    setenv("Geant4_DIR", Geant4_DIR.toStdString().c_str(), 1);

    env.insert("G4ENSDFSTATEDATA", G4ENSDFSTATEDATA);
    env.insert("G4LEDATA", G4LEDATA);
    env.insert("G4RUN", G4RUN);
    env.insert("Geant4_DIR", Geant4_DIR);

    geant4->setProcessEnvironment(env);
    geant4->setWorkingDirectory(TEMP);
    geant4->startDetached(G4CALL);
    geant4->waitForFinished();
}

//______________________________________________________________________________
QString ElossMonteCarlo::parseOutput()
{
    qDebug() << "\nSimulation completed, start parsing output\n";
    QFileInfo qFileInfo;
    QDir qDir;

    //set filenames reading env. vars.
    QString textOutput_ = QString::fromLocal8Bit(qgetenv("RUNBASE")) + textOutput;
    QString rootOutput_ = QString::fromLocal8Bit(qgetenv("RUNBASE")) + rootOutput;
    QString grapOutput_ = QString::fromLocal8Bit(qgetenv("RUNBASE")) + grapOutput;
    QString baseFolder  = QString::fromLocal8Bit(qgetenv("RUNBASE"));
    QString TEMP        = QString::fromLocal8Bit(qgetenv("TEMPFOLDER"));

    qDebug() << "\n" << textOutput_ << "\n";
    qDebug() << "\n" << rootOutput_ << "\n";
    qDebug() << "\n" << grapOutput_ << "\n";

    //Open TH1D in root file
    if (qFileInfo.exists(rootOutput_) == true)
    {
        QString tempFileName2str = TEMP + QString("/c2.png");
        TFile * tfi = new TFile(rootOutput_.toStdString().c_str());
        hOut = (TH1D*)tfi->Get("h1");
        cOut = new TCanvas("cOut", "Geant4 sim output - energy loss", 1000, 1000);
        cOut->cd();
        hOut->SetTitle("Geant4 sim output - energy loss");
        hOut->Draw();
        cOut->Print(tempFileName2str.toStdString().c_str());
        Copy(*hOut);
        delete cOut;
        delete tfi;
        qDir.remove(rootOutput);
        qDebug() << "\n .root file parsed\n";
    }

    //Open drawing
    QString filename = grapOutput_ + QString(".prim");
    QString outputfE = grapOutput_ + QString(".eps");
    QString outputfB = grapOutput_ + QString(".jpg");
    qDebug() << "\n" << filename;
    qDebug() << "\n" << outputfE;
    qDebug() << "\n" << outputfB;
    if (qFileInfo.exists(filename) == true)
    {
        QString dawnExe = QString::fromLocal8Bit(qgetenv("DAWNEXEC"));
        QString convert = QString::fromLocal8Bit(qgetenv("IMGCONVERT"));
        QString dawnCmd = dawnExe + QString(" -d \"") + filename + QString("\"");
        system(dawnCmd.toStdString().c_str());

        //Perform conversion
        QFile cp;
        cp.copy(outputfE, TEMP + QString("g4.eps"));
        //system(convert.toStdString().c_str());


        QProcess qp;
        qp.startDetached(QString("/usr/local/bin/magick ") + TEMP + QString("g4.eps ") + TEMP + QString("g4.jpg"));

        qDir.remove(filename);
        //qDir.remove(TEMP + QString("g4.eps"));
        qDebug() << "\n .prim file " << filename <<  " parsed\n\n";

    }

    return TEMP;
}

//______________________________________________________________________________
QProcess *ElossMonteCarlo::getGeant4() const
{
    return geant4;
}

//______________________________________________________________________________
QString ElossMonteCarlo::getSimLog() const
{
    return simLog;
}

//______________________________________________________________________________
QString ElossMonteCarlo::synthetizeMacro()
{
    QString stdviz = QString("/vis/open DAWNFILE") +
            QString("\n/vis/drawVolume") +
            QString("\n/vis/viewer/zoom 3.4") +
            QString("\n/vis/scene/add/trajectories smooth") +
            QString("\n/vis/modeling/trajectories/create/drawByCharge") +
            QString("\n/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true") +
            QString("\n/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 1") +
            QString("\n/vis/scene/add/hits") +
            QString("\n/vis/scene/endOfEventAction accumulate");

    QString macro = "";

    macro = macro
            + QString("/control/verbose ") + QString::number(getVerbosity())
            + QString("\n/control/saveHistory")
            + QString("\n/run/verbose ") + QString::number(getVerbosity())
            + QString("\n\n")
            + QString("\n/calorimeter/det/setMat Air")
            + QString("\n/calorimeter/det/setSizeX ") + QString::number(getBoxX() / 10.) + QString(" cm")
            + QString("\n/calorimeter/det/setSizeYZ ") + QString::number(getBoxYZ() / 10.) + QString(" cm")
            + QString("\n\n");

    macro = macro + calorimeter->modelSynthesis() + QString("\n/run/initialize\n\n");

    macro = macro
            + QString("\n/gun/particle ion")
            + QString("\n/gun/ion ") + QString::number(getG4ion().Z) + QString(" ")
            + QString::number(getG4ion().A) + QString(" ")
            + QString::number(getG4ion().Q)
            + QString("\n/gun/energy ") + QString::number(getEnergyMeanMeV()) + QString(" MeV")
            + QString("\n#/calorimeter/gun/rndm ") + QString::number(getBeamFWHM()) + QString(" mm")
            + QString("\n#/calorimeter/stepMax ") + QString::number(getMaxStep()) + QString(" mm")
            + QString("/run/print/progress ") + QString::number(getPrintProgress());

    macro = macro + QString("\n\n") + stdviz + QString("\n\n/run/beamOn ") + QString::number(getNEvents());

    return macro;
}

//______________________________________________________________________________
Calorimeter *ElossMonteCarlo::getCalorimeter() const
{
    return calorimeter;
}

int ElossMonteCarlo::getNEvents() const
{
    return nEvents;
}

void ElossMonteCarlo::setNEvents(int value)
{
    nEvents = value;
}

double ElossMonteCarlo::getBoxX() const
{
    return boxX;
}

void ElossMonteCarlo::setBoxX(double value)
{
    boxX = value;
}

double ElossMonteCarlo::getBoxYZ() const
{
    return boxYZ;
}

void ElossMonteCarlo::setBoxYZ(double value)
{
    boxYZ = value;
}

double ElossMonteCarlo::getMaxStep() const
{
    return maxStep;
}

void ElossMonteCarlo::setMaxStep(double value)
{
    maxStep = value;
}

unsigned int ElossMonteCarlo::getPrintProgress() const
{
    return printProgress;
}

void ElossMonteCarlo::setPrintProgress(unsigned int value)
{
    printProgress = value;
}

signed int ElossMonteCarlo::getVerbosity() const
{
    return verbosity;
}

void ElossMonteCarlo::setVerbosity(signed int value)
{
    verbosity = value;
}

bool ElossMonteCarlo::getPrintMaterials() const
{
    return printMaterials;
}

void ElossMonteCarlo::setPrintMaterials(bool value)
{
    printMaterials = value;
}

double ElossMonteCarlo::getEnergyMeanMeV() const
{
    return energyMeanMeV;
}

void ElossMonteCarlo::setEnergyMeanMeV(double value)
{
    energyMeanMeV = value;
}

double ElossMonteCarlo::getEnergySigmaMeV() const
{
    return energySigmaMeV;
}

void ElossMonteCarlo::setEnergySigmaMeV(double value)
{
    energySigmaMeV = value;
}

double ElossMonteCarlo::getBeamFWHM() const
{
    return beamFWHM;
}

void ElossMonteCarlo::setBeamFWHM(double value)
{
    beamFWHM = value;
}

G4ion ElossMonteCarlo::getG4ion() const
{
    return g4ion;
}

void ElossMonteCarlo::setG4ion(const G4ion &value)
{
    g4ion = value;
}

void ElossMonteCarlo::clearModel()
{
    Clear();
    getCalorimeter()->clear();
}
