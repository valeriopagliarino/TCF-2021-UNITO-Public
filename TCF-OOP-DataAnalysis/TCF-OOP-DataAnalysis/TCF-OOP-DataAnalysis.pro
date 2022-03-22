#/*===============================================================================
# * Università degli Studi di Torino - Dipartimento di Fisica
# * Corso di Tecniche di Calcolo per la Fisica A.A. 2021-2022
# *
# * [PROJECTNAME] an object-oriented data analysis tool
# *               for SiPM-based sampling calorimeter
# *
# * Copyright (C) 2022
# *
# * Authors:
# *
# * Francesco Calisto
# * Lorenzo Cane
# * Valerio Pagliarino
# *
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
# ================================================================================*/



QT       += core gui
QT       += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCF-OOP-DataAnalysis
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH  += ./include		   \
                ./uiFiles                  \
                -pthread $(ROOTINC)

LIBS        += -L$(ROOTLIB)               \
               -lCore                     \
               -lRIO                      \
               -lNet                      \
               -lHist                     \
               -lGraf                     \
               -lGraf3d                   \
               -lGpad                     \
               -lTree                     \
               -lRint                     \
               -lPostscript               \
               -lMatrix                   \
               -lPhysics                  \
               -lMathCore                 \
               -lThread

SOURCES += \
        main.cpp \
        gui.cpp \
    fitterclass.cpp \
    absorber.cpp \
    scintabsorber.cpp \
    pmmaabsorber.cpp \
    absabsorber.cpp \
    liquidcaloabsorber.cpp \
    detectormodel.cpp \
    elossmontecarlo.cpp \
    elossreco.cpp \
    measpoint.cpp \
    fileparser.cpp \
    qrootcanvas.cpp \
    statictoolbox.cpp \
    transverse.cpp \
    codeeditor.cpp \
    ions.cpp \
    calorimeter.cpp \
    fileexportel.cpp \
    fileexportmp.cpp \
    fileexporttr.cpp

HEADERS += \
        gui.h \
    fitterclass.h \
    absorber.h \
    scintabsorber.h \
    pmmaabsorber.h \
    absabsorber.h \
    liquidcaloabsorber.h \
    detectormodel.h \
    elossmontecarlo.h \
    elossreco.h \
    measpoint.h \
    fileparser.h \
    fileexport.h \
    qrootcanvas.h \
    statictoolbox.h \
    transverse.h \
    codeeditor.h \
    ions.h \
    calorimeter.h \
    fileexportel.h \
    fileexportmp.h \
    fileexporttr.h \
    element.h

FORMS += \
        gui.ui

STATECHARTS +=

DISTFILES   += \
    sampleData.txt \
    assets/icons/landau.jpg \
    assets/icons/gaussian.jpg

QMAKE_CXXFLAGS += -std=c++14

RESOURCES += \
    resources.qrc
