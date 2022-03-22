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
 * MERCHANTABILITY or fit1DNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ================================================================================*/

#include "fitterclass.h"


//______________________________________________________________________________
template <class T>
FitterClass<T>::FitterClass()
{
    targetObject = new T;
}

//______________________________________________________________________________
template <class T>
FitterClass<T>::~FitterClass()
{
    delete fit1D;
    delete fit2D;
    delete targetObject;
}

//______________________________________________________________________________
// 1D fit with custom function
template <class T>
void FitterClass<T>::custom1D(int numpar, const QString formula, T *data, std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p, std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max)
{
    if (isInitialized == false) isInitialized = true;
    else delete fit1D;

    setTargetObject(data);
    setFitFormula(formula);

    fit1D = new TF1("fit1D", fitFormula.toStdString().c_str(), -1.e5, 1.e5);

    for (int i = 0; i < numpar; ++i)
    {
        fit1D->SetParameter(i, p_i[i]);
        if (b[i]) fit1D->FixParameter(i, p[i]);

        if (r[i])
        {
            if (p_min[i] < p_max[i]) fit1D->SetParLimits(i, p_min[i], p_max[i]);
            else
            {
                QMessageBox msgBox;
                msgBox.setText(QString("Range set for p") + QString::number(i) + QString(" is not valid"));
                msgBox.exec();
            }
        }
    }

    targetObject->Fit(fit1D, "MR");

    /*data->getfit().clear();
    data->getfit().push_back(fit1D->GetParameter(0));
    data->getfit().push_back(fit1D->GetParameter(1));
    data->getfit().push_back(fit1D->GetParameter(2));
    data->getfit().push_back(fit1D->GetParameter(3));
    data->getfit().push_back(fit1D->GetParameter(4));
    data->getfit().push_back(fit1D->GetParameter(5));
    data->getfit_err().push_back(fit1D->GetParError(0));
    data->getfit_err().push_back(fit1D->GetParError(1));
    data->getfit_err().push_back(fit1D->GetParError(2));
    data->getfit_err().push_back(fit1D->GetParError(3));
    data->getfit_err().push_back(fit1D->GetParError(4));
    data->getfit_err().push_back(fit1D->GetParError(5));
    data->setEModel(data->getEModel()); */

}

//______________________________________________________________________________
// 1D Gaussian fit
template<class T>
void FitterClass<T>::gauss1D(T *data, std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p, std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max)
{
    setFitFormula("gaus");
    setFitModelName("Gaussian fit");
    custom1D(3, fitFormula, data, p_i, b, p, r, p_min, p_max);
}

//______________________________________________________________________________
// 1D Landau fit
template<class T>
void FitterClass<T>::landau(T *data, std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p, std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max)
{
    setFitFormula("landau");
    setFitModelName("Landau fit");
    custom1D(3, fitFormula, data, p_i, b, p, r, p_min, p_max);
}

//______________________________________________________________________________
// 1D Moyal fit
template<class T>
void FitterClass<T>::moyal(T *data, std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p, std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max)
{
    setFitFormula("( exp ( - ( ( - [p4] * x - [p0] ) / [p0] + exp ( - [p2] * ( - [p4] * x - [p0] ) / [p0] ) ) / 2 ) + [p3] ) / [p1] + [p5]");
    setFitModelName("Moyal fit");
    custom1D(6, fitFormula, data, p_i, b, p, r, p_min, p_max);
}

//______________________________________________________________________________
// 2D fit with custom function
template<class T>
void FitterClass<T>::custom2D(int index, int numpar, QString formula, T *data, std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p, std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max)
{
    if (isInitialized2 == false) isInitialized2 = true;
    else delete fit2D;

    setTargetObject(data);
    setFitFormula(formula);

    fit2D = new TF2("fit2D", fitFormula.toStdString().c_str(), -1.e5, 1.e5, -1.e5, 1.e5);

    for (int i = 0; i < numpar; ++i)
    {
        fit2D->SetParameter(i, p_i[i]);
        if (b[i]) fit2D->FixParameter(i, p[i]);
        if (r[i])
        {
            if (p_min[i] < p_max[i]) fit2D->SetParLimits(i, p_min[i], p_max[i]);
            else
            {
                QMessageBox msgBox;
                msgBox.setText(QString("Range set for p") + QString::number(i) + QString(" is not valid"));
                msgBox.exec();
            }
        }
    }

    targetObject->getAt(index)->Fit(fit2D, "MR");
}

//______________________________________________________________________________
// 2D gaussian fit
template<class T>
void FitterClass<T>::gauss2D(int index, T *data, std::vector<double> &p_i, std::vector<bool> &b, std::vector<double> &p, std::vector<bool> &r, std::vector<double> &p_min, std::vector<double> &p_max)
{
    setFitFormula("bigaus");
    setFitModelName("2D gaussian fit");
    custom2D(index, 6, fitFormula, data, p_i, b, p, r, p_min, p_max);
}

//______________________________________________________________________________
// Accept function for FileExport
template<class T>
void FitterClass<T>::Export(FileExport *fe)
{

}

//______________________________________________________________________________
// Set data to fit
template<class T>
void FitterClass<T>::setTargetObject(T *data)
{
    targetObject = data;
}

//______________________________________________________________________________
// Set fit model name
template<class T>
void FitterClass<T>::setFitModelName(QString name)
{
    fitModelName = name;
}


//______________________________________________________________________________
// Get fit model name
template<class T>
QString FitterClass<T>::getFitModelName()
{
    return fitModelName;
}

//______________________________________________________________________________
// Get fit1D object
template<class T>
TF1 * FitterClass<T>::getFit1D()
{
    return fit1D;
}

//______________________________________________________________________________
// Get fit2D object
template<class T>
TF2 * FitterClass<T>::getFit2D()
{
    return fit2D;
}

//______________________________________________________________________________
// Get fit formula
template<class T>
QString FitterClass<T>::getFitFormula() const
{
    return fitFormula;
}

//______________________________________________________________________________
// Set fit formula
template<class T>
void FitterClass<T>::setFitFormula(const QString &value)
{
    fitFormula = value;
}






