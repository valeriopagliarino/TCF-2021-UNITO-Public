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

#ifndef FILEEXPORTEL_H
#define FILEEXPORTEL_H

#include <fileexport.h>


class FileExportEl : public FileExport
{
public:
    FileExportEl();
    ~FileExportEl() {};
    virtual void RunExportEl(ElossReco *er);
    virtual void RunExportTr(Transverse *tr) {};
    virtual void RunExportMp(MeasPoint *mp) {};

    virtual void SetOpt(const std::string opt);
    virtual void setIndex(const int index) {};

    std::string GetOpt();

private:
    std::string Option;

};

#endif // FILEEXPORTEL_H
