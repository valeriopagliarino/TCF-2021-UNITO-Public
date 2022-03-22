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

#include "fileparser.h"

//______________________________________________________________________________
FileParser::FileParser()
{

}

//______________________________________________________________________________
FileParser::~FileParser()
{

}

//______________________________________________________________________________
//  Parse the file at the specified path, fill the fields and the events vector
void FileParser::loadFile(const QString path)
{
    QFileInfo f(path);
    if (f.exists() == false)
    {
        qDebug() << "\nFile not found: " << path;
        QMessageBox msgBox;
        msgBox.setText("File not found!");
        msgBox.exec();
        runImported = false;
        return;
    }

    filename = f.fileName();
    filepath = path;
    foldername = f.absoluteDir().absolutePath();

    std::ifstream infile(path.toStdString().c_str());
    std::string line;
    events.clear();
    int dh_c = 0;
    X = 0;
    Y = 0;
    Z = 0;
    E = 0;
    T = 0;

    runImported = false;
    signed int sgn = 1;
    if (invertingAmplifier == true) sgn = -1;

    while (std::getline(infile, line))
    {
        if ((line.c_str()[1] == 'P') && (line.c_str()[2] == '.') && (line.c_str()[3] == 'R'))
            formatMarkerFound = true;

        if (line.c_str()[0] == '#')
        {
           //Parse header!!

           if (line.c_str()[1] == 'X')
                X = QString::fromStdString(line).mid(2).toDouble();

           if (line.c_str()[1] == 'Y')
                Y = QString::fromStdString(line).mid(2).toDouble();

           if (line.c_str()[1] == 'Z')
                Z = QString::fromStdString(line).mid(2).toDouble();

           if (line.c_str()[1] == 'E')
                E = QString::fromStdString(line).mid(2).toDouble();

           if (line.c_str()[1] == 'T')
                T = QString::fromStdString(line).mid(2).toDouble();

           if (line.c_str()[1] == 'D')
           {
               //debug qDebug() << "\n\n" << dh_c << "\n" << line << "\n";

               //FROM HERE
                if (dh_c == 0) digitizerChannel0 = "";
                if (dh_c == 0) digitizerChannel1 = "";
                if (dh_c == 0) triggerMode = "";
                if (dh_c == 0) dcOffset0 = "";
                if (dh_c == 0) dcOffset1 = "";
                if (dh_c == 0) actCh = "";
                if (dh_c == 0) riseTime = "";
                if (dh_c == 0) trigMean = "";
                if (dh_c == 0) gateW = "";
                if (dh_c == 0) preGate = "";
                if (dh_c == 0) holdoff = "";
                if (dh_c == 0) baselineMean = "";
                if (dh_c == 0) noFlatTime = "";
                if (dh_c == 0) GPO = "";
                if (dh_c == 0) psauChannel0 = "";
                if (dh_c == 0) psauChannel1 = "";
                if (dh_c == 0) vSet0 = "";
                if (dh_c == 0) vSet1 = "";
                if (dh_c == 0) gain0 = "";
                if (dh_c == 0) gain1 = "";
                if (dh_c == 0) comp0 = "";
                if (dh_c == 0) comp1 = "";
                if (dh_c == 0) threshold0 = "";
                if (dh_c == 0) threshold1 = "";
               //TO HERE - THIS PARSING SECTION IS TO BE COMPLETED

                if (dh_c == 6) dateTime = QString::fromStdString(line).right(13);
                if (dh_c == 5) vMon0 = QString::fromStdString(line).right(96).left(9);
                if (dh_c == 5) vMon1 = QString::fromStdString(line).right(75).left(8);
                if (dh_c == 5) temp0 = QString::fromStdString(line).right(55).left(9);
                if (dh_c == 5) temp1 = QString::fromStdString(line).right(34).left(9);
                if (dh_c == 5) tempBoard = QString::fromStdString(line).right(9);
                dh_c++;
                //.left and .right sintax: how many chars you want to keep starting from left or right.
           }

        }
        else
        {
            //Parse data
            events.push_back(sgn * atof(line.c_str()));
        }
    }
    if (formatMarkerFound == false)
        return;

    runImported = true;
}

//______________________________________________________________________________
void FileParser::dumpFile(const QString path) const
{

}

//______________________________________________________________________________
//Check if file exists
bool FileParser::fileExist(const QString path) const
{
    QFileInfo checkExist(path);
    return checkExist.exists();
}

//______________________________________________________________________________
QString FileParser::getDigitizerChannel0() const
{
    return digitizerChannel0;
}

//______________________________________________________________________________
QString FileParser::getDigitizerChannel1() const
{
    return digitizerChannel1;
}

//______________________________________________________________________________
QString FileParser::getTriggerMode() const
{
    return triggerMode;
}

//______________________________________________________________________________
QString FileParser::getDcOffset0() const
{
    return dcOffset0;
}

//______________________________________________________________________________
QString FileParser::getDcOffset1() const
{
    return dcOffset1;
}

//______________________________________________________________________________
QString FileParser::getActCh() const
{
    return actCh;
}

//______________________________________________________________________________
QString FileParser::getRiseTime() const
{
    return riseTime;
}

//______________________________________________________________________________
QString FileParser::getTrigMean() const
{
    return trigMean;
}

//______________________________________________________________________________
QString FileParser::getGateW() const
{
    return gateW;
}

//______________________________________________________________________________
QString FileParser::getPreGate() const
{
    return preGate;
}

//______________________________________________________________________________
QString FileParser::getHoldoff() const
{
    return holdoff;
}

//______________________________________________________________________________
QString FileParser::getBaselineMean() const
{
    return baselineMean;
}

//______________________________________________________________________________
QString FileParser::getNoFlatTime() const
{
    return noFlatTime;
}

//______________________________________________________________________________
QString FileParser::getGPO() const
{
    return GPO;
}

//______________________________________________________________________________
QString FileParser::getPsauChannel0() const
{
    return psauChannel0;
}

//______________________________________________________________________________
QString FileParser::getPsauChannel1() const
{
    return psauChannel1;
}

//______________________________________________________________________________
QString FileParser::getVSet0() const
{
    return vSet0;
}

//______________________________________________________________________________
QString FileParser::getVSet1() const
{
    return vSet1;
}

//______________________________________________________________________________
QString FileParser::getGain0() const
{
    return gain0;
}

//______________________________________________________________________________
QString FileParser::getGain1() const
{
    return gain1;
}

//______________________________________________________________________________
QString FileParser::getComp0() const
{
    return comp0;
}

//______________________________________________________________________________
QString FileParser::getComp1() const
{
    return comp1;
}

//______________________________________________________________________________
QString FileParser::getThreshold0() const
{
    return threshold0;
}

//______________________________________________________________________________
QString FileParser::getThreshold1() const
{
    return threshold1;
}

//______________________________________________________________________________
QString FileParser::getDateTime() const
{
    return dateTime;
}

//______________________________________________________________________________
QString FileParser::getVMon0() const
{
    return vMon0;
}

//______________________________________________________________________________
QString FileParser::getVMon1() const
{
    return vMon1;
}

//______________________________________________________________________________
QString FileParser::getTemp0() const
{
    return temp0;
}

//______________________________________________________________________________
QString FileParser::getTemp1() const
{
    return temp1;
}

//______________________________________________________________________________
QString FileParser::getTempBoard() const
{
    return tempBoard;
}

//______________________________________________________________________________
std::vector<double> FileParser::getEvents() const
{
    return events;
}

//______________________________________________________________________________
bool FileParser::getRunImported() const
{
    return runImported;
}

//______________________________________________________________________________
double FileParser::getX_err() const
{
    return X_err;
}

//______________________________________________________________________________
void FileParser::setX_err(double value)
{
    if (value < 0)
        return;

    X_err = value;
}

//______________________________________________________________________________
double FileParser::getY_err() const
{
    return Y_err;
}

//______________________________________________________________________________
void FileParser::setY_err(double value)
{
    if (value < 0)
        return;

    Y_err = value;
}

//______________________________________________________________________________
double FileParser::getZ_err() const
{
    if (Z_err > 0)
        return Z_err;
    else
        return Z_err * -1.;
}

//______________________________________________________________________________
void FileParser::setZ_err(double value)
{
    if (value < 0)
        return;

    Z_err = value;
}

//______________________________________________________________________________
double FileParser::getE_err() const
{
    return E_err;
}

//______________________________________________________________________________
void FileParser::setE_err(double value)
{
    if (value < 0)
        return;

    E_err = value;
}

bool FileParser::getInvertingAmplifier() const
{
    return invertingAmplifier;
}

void FileParser::setInvertingAmplifier(bool value)
{
    invertingAmplifier = value;
}

//______________________________________________________________________________
QString FileParser::getFilepath() const
{
    return filepath;
}

//______________________________________________________________________________
QString FileParser::getFilename() const
{
    return filename;
}

//______________________________________________________________________________
QString FileParser::getFoldername() const
{
    return foldername;
}

//______________________________________________________________________________
QString FileParser::getFileextension() const
{
    return fileextension;
}

//______________________________________________________________________________
double FileParser::getX() const
{
    return X;
}

//______________________________________________________________________________
double FileParser::getY() const
{
    return Y;
}

//______________________________________________________________________________
double FileParser::getZ() const
{
    return Z;
}

//______________________________________________________________________________
double FileParser::getE() const
{
    return E;
}

//______________________________________________________________________________
double FileParser::getT() const
{
    return T;
}

