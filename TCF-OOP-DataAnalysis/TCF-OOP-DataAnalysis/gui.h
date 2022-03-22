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

#ifndef GUI_H
#define GUI_H

#include<transverse.h>
#include<measpoint.h>
#include<elossreco.h>
#include<elossmontecarlo.h>
#include<ions.h>
#include<fitterclass.cpp>
#include<fileexport.h>
#include<fileexportmp.h>
#include<fileexportel.h>
#include<fileexporttr.h>

#include<QMainWindow>
#include<QDebug>
#include<QString>
#include<QFileSystemModel>
#include<QFile>
#include<QFileInfo>
#include<QTextStream>
#include<QDir>
#include<QTemporaryFile>
#include<QTimer>
#include<QGraphicsItem>
#include<QGraphicsView>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QListWidgetItem>
#include<QFileDialog>
#include<QProcess>
#include<QGraphicsPixmapItem>
#include<QMessageBox>
#include<TCanvas.h>
#include<TSystem.h>
#include<TApplication.h>
#include<TFile.h>
#include<TBrowser.h>
#include<TF1.h>
#include<TF2.h>
#include<TStyle.h>

namespace Ui {
class gui;
}

//______________________________________________________________________________
class gui : public QMainWindow
{
    Q_OBJECT


public:
    explicit gui(QWidget *parent = 0);
    ~gui();

public slots:
    void handle_root_events(void);

private:
    Ui::gui                      *ui;
    QFileSystemModel             *dirModel;
    QFileSystemModel             *fileModel;
    MeasPoint                    *currentMeasPoint;
    ElossReco                    *currentElossReco;
    Transverse                   *currentTransverse;
    FitterClass<MeasPoint>       *currentMpFit;
    FitterClass<ElossReco>       *eFit;
    FitterClass<Transverse>      *transFit;
    bool                         isMeasPointInit = false;
    bool                         isG4ViewportInit = false;
    QString                      currentRunFile;
    Ions                         *ionsTable;
    ElossMonteCarlo              *elmc;
    std::vector<ElossReco*>      elossCollection;
    std::vector<materialDisplay> globalMaterialRegister;
    std::vector<ionDisplay>      globalIonRegister;
    TCanvas                      *energyLossCanvas;
    TCanvas                      *externalTransverseCanvas;

    //Detector materials:
    Absorber                     * T1Mat;
    Absorber                     * W1Mat;
    Absorber                     * W2Mat;
    Absorber                     * CalMat;
    Absorber                     * SMat;

    const int wc1s  = 670*4;
    const int hc1s  = 400*4;
    double    zoomf = 1.;

    void displayTCanvas(TCanvas *tc, QLabel *l);
    void openRun(QString temp);
    void fillRunGrid(QString Name, QString E, QString Z, QString X, QString Y, QString t, QString src);
    void updateRunTable();
    void resetFitFields();
    void eLossResetFitFields();
    void transverseResetFitFields();
    void updateMetadataTable();
    void refreshElossList();
    void removeElossList(int index);
    void fillElossPointTable();
    void refreshTransverse();
    void displayCurrentTransverseSlice(bool external = false);
    void simViewportZoomIn();
    void simViewportZoomOut();
    void simViewportSave();
    void updateViewportZoom();

    static Absorber *getMaterialByIndex(const int &i, Absorber *ab);

private slots:

    void on_goToEnteredPath_clicked();
    void on_fileTV_clicked(const QModelIndex &index);
    void on_fileTV_doubleClicked(const QModelIndex &index);
    void on_toolButton_7_clicked();
    void on_toolButton_8_clicked();
    void on_pushButton_3_clicked();
    void on_toolButton_9_clicked();
    void on_pushButton_4_clicked();
    void on_toolButton_10_clicked();
    void on_pushButton_clicked();
    void on_toolButton_5_clicked();
    void on_toolButton_6_clicked();
    void on_runTable_cellDoubleClicked(int row, int column);
    void on_toolButton_4_clicked();
    void on_toolButton_12_clicked();
    void processGeant4Output();
    void simCompleted();
    void on_toolButton_13_clicked();
    void on_T1_mat_currentIndexChanged(int index);
    void on_W1_mat_currentIndexChanged(int index);
    void on_W2_mat_currentIndexChanged(int index);
    void on_CAL_mat_currentIndexChanged(int index);
    void on_pushButton_14_clicked();
    void on_pushButton_15_clicked();
    void on_S_mat_currentIndexChanged(int index);
    void on_Ions_menu_currentIndexChanged(int index);
    void on_pushButton_17_clicked();
    void on_pushButton_19_clicked();
    void on_pushButton_16_clicked();
    void on_elossList_itemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_20_clicked();
    void on_pushButton_22_clicked();
    void on_pushButton_29_clicked();
    void on_transverseList_currentIndexChanged(int index);
    void on_pushButton_25_clicked();
    void on_pushButton_26_clicked();
    void on_pushButton_27_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_24_clicked();
    void on_toolButton_11_clicked();
    void on_pushButton_28_clicked();
    void on_pushButton_12_clicked();
    void on_toolButton_59_clicked();
    void on_toolButton_60_clicked();
    void on_toolButton_62_clicked();
    void on_nEvents2_editingFinished();
    void on_nEvents_editingFinished();
    void on_toolButton_61_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_fitMoyal_clicked();
    void on_toolButton_14_clicked();
    void on_pushButton_34_clicked();
    void on_toolButton_22_clicked();
    void on_pushButton_35_clicked();
    void on_toolButton_21_clicked();
    void on_toolButton_63_clicked();
    void on_toolButton_15_clicked();
    void on_toolButton_16_clicked();

    void on_toolButton_17_clicked();

    void on_toolButton_67_clicked();

    void on_toolButton_69_clicked();

    void on_toolButton_72_clicked();

    void on_toolButton_70_clicked();

    void on_toolButton_71_clicked();

    void on_toolButton_73_clicked();

    void on_toolButton_74_clicked();

    void on_toolButton_75_clicked();

    void on_toolButton_clicked();

protected:
     QTimer *fRootTimer_;
};

#endif // GUI_H
