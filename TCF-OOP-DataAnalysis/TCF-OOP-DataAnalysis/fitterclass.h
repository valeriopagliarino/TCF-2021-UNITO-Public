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

#ifndef FITTERCLASS_H
#define FITTERCLASS_H

#include <iostream>
#include<TCanvas.h>
#include<TSystem.h>
#include<TApplication.h>
#include<TFile.h>
#include<TBrowser.h>
#include<TF1.h>
#include<TF2.h>
#include<TStyle.h>
#include<TAttLine.h>
#include<QMessageBox>
#include<QLineEdit>
#include<QCheckBox>
#include<measpoint.h>
#include<elossreco.h>
#include<transverse.h>


template <class T>
class FitterClass
{
public:
    FitterClass();
    ~FitterClass();

    void custom1D(int numpar, QString formula, T *data,
                  std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p,
                  std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max);

    void gauss1D(T *data,
                 std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p,
                 std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max);
    void landau(T *data,
                std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p,
                std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max);
    void moyal(T *data,
               std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p,
               std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max);

    void custom2D(int index, int numpar, QString formula, T* data,
                  std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p,
                  std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max);
    void gauss2D(int index, T *data,
                 std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p,
                 std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max);

    void Export(FileExport *fe);

    void setTargetObject(T *data);
    void setFitModelName(QString name);

    QString getFitModelName();
    TF1 * getFit1D();
    TF2 * getFit2D();

    QString getFitFormula() const;
    void setFitFormula(const QString &value);

private:
    TF1 *                   fit1D;
    TF2 *                   fit2D;
    T *                     targetObject;
    QString                 fitModelName;
    QString                 fitFormula;
    bool                    isInitialized = false;
    bool                    isInitialized2 = false;
    const int wc1s = 670*4;
    const int hc1s = 400*4;
};


#endif // FITTERCLASS_H
