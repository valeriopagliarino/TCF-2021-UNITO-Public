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

#ifndef FILEPARSER_H
#define FILEPARSER_H

#include<stdio.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#include<QString>
#include<QFile>
#include<QDir>
#include<QMessageBox>
#include<QDebug>

class FileParser
{

public:
    FileParser();
    ~FileParser();

    void loadFile(const QString path);
    void dumpFile(const QString path) const;
    bool fileExist(const QString path) const;

    QString getFilepath() const;
    QString getFilename() const;
    QString getFoldername() const;
    QString getFileextension() const;

    double getX() const;
    double getY() const;
    double getZ() const;
    double getE() const;
    double getT() const;

    QString getDigitizerChannel0() const;
    QString getDigitizerChannel1() const;
    QString getTriggerMode() const;
    QString getDcOffset0() const;
    QString getDcOffset1() const;
    QString getActCh() const;
    QString getRiseTime() const;
    QString getTrigMean() const;
    QString getGateW() const;
    QString getPreGate() const;
    QString getHoldoff() const;
    QString getBaselineMean() const;
    QString getNoFlatTime() const;
    QString getGPO() const;
    QString getPsauChannel0() const;
    QString getPsauChannel1() const;
    QString getVSet0() const;
    QString getVSet1() const;
    QString getGain0() const;
    QString getGain1() const;
    QString getComp0() const;
    QString getComp1() const;
    QString getThreshold0() const;
    QString getThreshold1() const;
    QString getDateTime() const;
    QString getVMon0() const;
    QString getVMon1() const;
    QString getTemp0() const;
    QString getTemp1() const;
    QString getTempBoard() const;
    std::vector<double> getEvents() const;
    bool getRunImported() const;

    double getX_err() const;
    void setX_err(double value);
    double getY_err() const;
    void setY_err(double value);
    double getZ_err() const;
    void setZ_err(double value);
    double getE_err() const;
    void setE_err(double value);
    bool getInvertingAmplifier() const;
    void setInvertingAmplifier(bool value);

private:
    QString filepath;
    QString filename;
    QString foldername;
    QString fileextension;
    bool    invertingAmplifier;

    std::vector<double> events;

    double  X; // X detector position in m
    double  Y; // Y detector position in m
    double  Z; // Z detector position in m
    double  E; // Beam energy in eV
    double  T; // Timestamp

    double  X_err; // X detector position in m
    double  Y_err; // Y detector position in m
    double  Z_err; // Z detector position in m
    double  E_err; // Beam energy in eV

    // Digitizer
    QString digitizerChannel0;
    QString digitizerChannel1;
    QString triggerMode;
    QString dcOffset0;
    QString dcOffset1;
    QString actCh;
    QString riseTime;
    QString trigMean;
    QString gateW;
    QString preGate;
    QString holdoff;
    QString baselineMean;
    QString noFlatTime;
    QString GPO;

    //PSAU = Power supply & amplification unit
    QString psauChannel0;
    QString psauChannel1;
    QString vSet0;
    QString vSet1;
    QString gain0;
    QString gain1;
    QString comp0;
    QString comp1;
    QString threshold0;
    QString threshold1;

    QString dateTime;
    QString vMon0;
    QString vMon1;
    QString temp0;
    QString temp1;
    QString tempBoard;

    bool runImported = false;
    bool formatMarkerFound = false;

    //Private methods:

};

#endif // FILEPARSER_H
