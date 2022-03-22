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

#include "gui.h"
#include "ui_gui.h"

//______________________________________________________________________________
gui::gui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gui)
{
    ui->setupUi(this);

    // File system model for the tree view panel
    QString mPath = "";
    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot |
                        QDir::AllEntries);
    dirModel->setRootPath(mPath);
    ui->fileTV->setModel(dirModel);

    // ROOT event handler
    fRootTimer_ = new QTimer( this );
    QObject::connect(
                fRootTimer_,
                SIGNAL(timeout()),
                this,
                SLOT(handle_root_events())
                );
    fRootTimer_->start( 20 );

    //Single run initialization
    currentMpFit = new FitterClass<MeasPoint>;

    //Energy loss initialization
    currentElossReco = new ElossReco();
    eFit = new FitterClass<ElossReco>;
    currentElossReco->SetTitle("default");
    elossCollection.push_back(currentElossReco);
    refreshElossList();
    ui->g4PathDisplay->setText(QString::fromLocal8Bit(qgetenv("G4RUN")));

    //Initialize materials
    AbsAbsorber::initializeMaterial(globalMaterialRegister);
    PmmaAbsorber::initializeMaterial(globalMaterialRegister);
    ScintAbsorber::initializeMaterial(globalMaterialRegister);
    LiquidCaloAbsorber::initializeMaterial(globalMaterialRegister);

    //Initialize accelerated ions
    Ions::initializeAllIons(globalIonRegister);

    //Populate listViews
    for (int i = 0; i < globalMaterialRegister.size(); ++i)
    {
        ui->T1_mat->addItem(QString::fromStdString(globalMaterialRegister[i].name));
        ui->W1_mat->addItem(QString::fromStdString(globalMaterialRegister[i].name));
        ui->W2_mat->addItem(QString::fromStdString(globalMaterialRegister[i].name));
        ui->CAL_mat->addItem(QString::fromStdString(globalMaterialRegister[i].name));
        ui->S_mat->addItem(QString::fromStdString(globalMaterialRegister[i].name));
    }

    for (int i = 0; i < globalIonRegister.size(); ++i)
        ui->Ions_menu->addItem(QString::fromStdString(globalIonRegister[i].name));


    //Initialize transverse beam viz
    currentTransverse = new Transverse();
    transFit = new FitterClass<Transverse>;

    //Initialize Monte Carlo simulation and detector models
    elmc = new ElossMonteCarlo();

    ui->tabWidgetMain->removeTab(2);

}

//______________________________________________________________________________
gui::~gui()
{
    delete ui;
}

//______________________________________________________________________________
//  At every timer tick check for pending ROOT events from external canvases
//  launched by this thread and process them
void gui::handle_root_events()
{
    gSystem->ProcessEvents();
}

//______________________________________________________________________________
//  Convert a TCanvas into a Pixmap and apply to a QLabel
//  It is used to display plots inside the interface, without user interaction
void gui::displayTCanvas(TCanvas *tc, QLabel *l)
{
    QString tempFileName1str = QString::fromLocal8Bit(qgetenv("TEMPFOLDER")) + QString("c1.png");
    tc->Print(tempFileName1str.toStdString().c_str());
    const QPixmap singleRunPixmap(tempFileName1str);
    l->setPixmap(singleRunPixmap);
}

//______________________________________________________________________________
//  Open a run, given the path to the data file
void gui::openRun(QString temp)
{
    currentMeasPoint = new MeasPoint(temp, ui->invertingAmpl->isChecked());
    if (currentMeasPoint->getRunImported() == false)
    {
        isMeasPointInit = false;
        delete currentMeasPoint;
        return;
    }

    updateMetadataTable();
    ui->displayFname->setText(currentMeasPoint->getFilename());
    ui->displayTimestamp->setText(QString::number(currentMeasPoint->getT()));
    ui->displayEnergyMeV->setText(QString::number(currentMeasPoint->getE()));
    ui->displayX->setText(QString(QString::number(currentMeasPoint->getX())));
    ui->displayY->setText(QString(QString::number(currentMeasPoint->getY())));
    ui->displayZ->setText(QString(QString::number(currentMeasPoint->getZ())));

    TCanvas * C1 = new TCanvas("C1", currentMeasPoint->getFilename().toStdString().c_str(), wc1s, hc1s);
    C1->cd();
    currentMeasPoint->Draw("");
    isMeasPointInit = true;
    displayTCanvas(C1, ui->singleRunDisplay);
    delete C1;

    ui->entriesNum->setText(QString::number(currentMeasPoint->GetEntries()));
}

//______________________________________________________________________________
//  Add new items to the run table
void gui::fillRunGrid(QString Name, QString E, QString Z, QString X, QString Y, QString t, QString src)
{
    if (isMeasPointInit == false) return;
    ui->runTable->insertRow(0);

    QTableWidgetItem * p1Item = new QTableWidgetItem();
    p1Item->setText(Name);
    ui->runTable->setItem(0, 0, p1Item );

    QTableWidgetItem * p2Item = new QTableWidgetItem();
    p2Item->setText(E);
    ui->runTable->setItem(0, 1, p2Item );

    QTableWidgetItem * p3Item = new QTableWidgetItem();
    p3Item->setText(Z);
    ui->runTable->setItem(0, 2, p3Item );

    QTableWidgetItem * p4Item = new QTableWidgetItem();
    p4Item->setText(X);
    ui->runTable->setItem(0, 3, p4Item );

    QTableWidgetItem * p5Item = new QTableWidgetItem();
    p5Item->setText(Y);
    ui->runTable->setItem(0, 4, p5Item );

    QTableWidgetItem * p6Item = new QTableWidgetItem();
    p6Item->setText(t);
    ui->runTable->setItem(0, 5, p6Item );

    QTableWidgetItem * p7Item = new QTableWidgetItem();
    p7Item->setText(src);
    ui->runTable->setItem(0, 6, p7Item );
}

//______________________________________________________________________________
//  Update the run table
void gui::updateRunTable()
{
   ui->runTable->clear();
   ui->runTable->setRowCount(0);

   for (int i = 0; i < currentElossReco->getMp().size(); ++i)
   {
       double T_m = currentElossReco->getMp()[i]->getT();
       double x_m = currentElossReco->getMp()[i]->getX();
       double y_m = currentElossReco->getMp()[i]->getY();
       double z_m = currentElossReco->getMp()[i]->getZ();
       double E_m = currentElossReco->getMp()[i]->getE();


       fillRunGrid(QString("Data take"),
                   QString::number(E_m),
                   QString::number(z_m) ,
                   QString::number(x_m),
                   QString::number(y_m),
                   QString::number(T_m),
                   currentElossReco->getMp()[i]->getFilepath()
                   );
   }

   ui->runTable->setHorizontalHeaderLabels(
               QStringList() << "Name" << "E (MeV)" << "Z (mm)" << "X (mm)" << "Y (mm)" << "t" << "src");
}


//______________________________________________________________________________
//  Reset fit fields
void gui::resetFitFields()
{
    ui->p0_sr->setText("");
    ui->p1_sr->setText("");
    ui->p2_sr->setText("");
    ui->p3_sr->setText("");
    ui->p4_sr->setText("");
    ui->chi2_sr->setText("");
    ui->ndf_sr->setText("");
    ui->chi2rid_sr->setText("");
    ui->srModelExp->setText("");
    ui->res_sr->setText("");
    ui->sr_numpar->setText("");
}

//______________________________________________________________________________
//  Reset energy loss fit fields
void gui::eLossResetFitFields()
{
    ui->p0_e->setText("");
    ui->p1_e->setText("");
    ui->p2_e->setText("");
    ui->p3_e->setText("");
    ui->p4_e->setText("");
    ui->p5_e->setText("");
    ui->p0_val_e->setText("");
    ui->p1_val_e->setText("");
    ui->p2_val_e->setText("");
    ui->p3_val_e->setText("");
    ui->p4_val_e->setText("");
    ui->p5_val_e->setText("");
    ui->p0_min_e->setText("");
    ui->p1_min_e->setText("");
    ui->p2_min_e->setText("");
    ui->p3_min_e->setText("");
    ui->p4_min_e->setText("");
    ui->p5_min_e->setText("");
    ui->p0_max_e->setText("");
    ui->p1_max_e->setText("");
    ui->p2_max_e->setText("");
    ui->p3_max_e->setText("");
    ui->p4_max_e->setText("");
    ui->p5_max_e->setText("");
    ui->chi2_e->setText("");
    ui->ndf_e->setText("");
    ui->chi2rid_e->setText("");
    ui->eLossModelExp->setText("");
    ui->res_e->setText("");
    ui->eLoss_numpar->setText("");
    ui->check_e_val_0->setChecked(false);
    ui->check_e_val_1->setChecked(false);
    ui->check_e_val_2->setChecked(false);
    ui->check_e_val_3->setChecked(false);
    ui->check_e_val_4->setChecked(false);
    ui->check_e_val_5->setChecked(false);
    ui->check_e_range_0->setChecked(false);
    ui->check_e_range_1->setChecked(false);
    ui->check_e_range_2->setChecked(false);
    ui->check_e_range_3->setChecked(false);
    ui->check_e_range_4->setChecked(false);
    ui->check_e_range_5->setChecked(false);
}


//______________________________________________________________________________
//  Update the table with the metadata
void gui::updateMetadataTable()
{
    ui->metadataList->clear();
    /*
    dateTime
    vMon0 = Q
    vMon1 = Q
    temp0 = Q
    temp1 = Q
    tempBoard
    */
    ui->metadataList->addItem(QString("Date/time :  ") + currentMeasPoint->getDateTime());
    ui->metadataList->addItem(QString("Vmon0    V:  ") + currentMeasPoint->getVMon0());
    ui->metadataList->addItem(QString("Vmon1    V:  ") + currentMeasPoint->getVMon1());
    ui->metadataList->addItem(QString("Temp0   °C:  ") + currentMeasPoint->getTemp0());
    ui->metadataList->addItem(QString("Temp1   °C:  ") + currentMeasPoint->getTemp1());
    ui->metadataList->addItem(QString("T.board °C:  ") + currentMeasPoint->getTempBoard());
}

//______________________________________________________________________________
// Refresh current ElossReco
void gui::refreshElossList()
{
    ui->elossList->clear();
    for (int i = 0; i < elossCollection.size(); ++i)
    {
        QString title;
        title = QString::fromLocal8Bit(elossCollection[i]->GetTitle());
        ui->elossList->addItem(title);
    }
}

//______________________________________________________________________________
// Remove current ElossReco
void gui::removeElossList(int index)
{
    if (elossCollection[index] == currentElossReco) return;
    delete elossCollection[index];
    staticToolbox::remove(elossCollection, index);
    refreshElossList();
}

//______________________________________________________________________________
//
void gui::fillElossPointTable()
{
    double Z, Z_err, Mean, Sigma;
    ui->elossPointTable->clear();
    ui->elossPointTable->setRowCount(0);

    for (int i = 0; i < currentElossReco->GetN(); ++i)
    {
        ui->elossPointTable->insertRow(0);

        Z       = currentElossReco->getMp()[i]->getZ();
        Z_err   = currentElossReco->getMp()[i]->getZ_err();
        Mean    = currentElossReco->getMp()[i]->GetMean();
        Sigma   = currentElossReco->getMp()[i]->GetMeanError();

        QTableWidgetItem * p1Item = new QTableWidgetItem();
        QTableWidgetItem * p2Item = new QTableWidgetItem();
        QTableWidgetItem * p3Item = new QTableWidgetItem();
        QTableWidgetItem * p4Item = new QTableWidgetItem();

        p1Item->setText(QString::number(Z));
        ui->elossPointTable->setItem(0, 0, p1Item );

        p2Item->setText(QString::number(Z_err));
        ui->elossPointTable->setItem(0, 1, p2Item );

        p3Item->setText(QString::number(Mean));
        ui->elossPointTable->setItem(0, 2, p3Item );

        p4Item->setText(QString::number(Sigma));
        ui->elossPointTable->setItem(0, 3, p4Item );
    }

    ui->elossPointTable->setHorizontalHeaderLabels(
                QStringList() << "Z mean (mm)" << "Z err (mm)" << "Mean" << "Sigma");

}

//______________________________________________________________________________
//
void gui::refreshTransverse()
{
    ui->transverseList->clear();
    while (ui->transverseList->count() > 0)
        ui->transverseList->removeItem(0);

    for (int i = 0; i < currentTransverse->count(); ++i)
    {
        ui->transverseList->addItem(QString::fromLocal8Bit(currentTransverse->getAt(i)->GetTitle()));
    }
}


//______________________________________________________________________________
//  Plot in the transversal beam canvas the TH2D of the selected slice
void gui::displayCurrentTransverseSlice(bool external)
{
    if (ui->transverseList->currentText().isNull()) return;
    if (ui->transverseList->currentText().isEmpty()) return;

    if (external == false)  // Draw inside the GUI
    {
        TCanvas * TT = new TCanvas("TT", "XY beam slice", 800, 1300);
        TT->cd();
        currentTransverse->getAt(ui->transverseList->currentIndex())->Draw(ui->transverseDrawOption->text().toStdString().c_str());
        displayTCanvas(TT, ui->transverseDisplay);
        ui->transverseDisplay->update();
        ui->transverseDisplay->repaint();
        delete TT;
    }
    else    //Draw in external canvas
    {
        externalTransverseCanvas = new TCanvas("externalTransverseCanvas", "XY beam slice", 800, 600);
        externalTransverseCanvas->cd();
        currentTransverse->getAt(ui->transverseList->currentIndex())->Draw(ui->transverseDrawOption->text().toStdString().c_str());
        externalTransverseCanvas->Draw();
    }
}

//______________________________________________________________________________
//
void gui::simViewportZoomIn()
{
    zoomf = zoomf + 0.2;
    updateViewportZoom();
}

//______________________________________________________________________________
//
void gui::simViewportZoomOut()
{
    zoomf = zoomf - 0.2;
    updateViewportZoom();
}

//______________________________________________________________________________
//
void gui::simViewportSave()
{
    QFile qf;
    QFileDialog qd;
    QString filename = qd.getSaveFileName();
    if ((filename.isNull() == true) || (filename.isEmpty() == true)) return;

    QString TEMP = QString::fromLocal8Bit(qgetenv("TEMPFOLDER"));
    qf.copy(TEMP + QString("g4.jpg"), filename);
}

//______________________________________________________________________________
//
void gui::updateViewportZoom()
{
    if (isG4ViewportInit == false) return;
    QString TEMP = QString::fromLocal8Bit(qgetenv("TEMPFOLDER"));
    QPixmap g4Pixmap(TEMP + QString("g4.jpg"));
    double size = (double)g4Pixmap.height() * zoomf;
    g4Pixmap = g4Pixmap.scaledToHeight((int)size);
    ui->geantViewport->setPixmap(g4Pixmap);
}

//______________________________________________________________________________
//  Initialize material by index
Absorber * gui::getMaterialByIndex(const int &i,  Absorber * ab)
{
    switch(i)
    {
        case 0:
            ab = new AbsAbsorber();
            break;
        case 1:
            ab = new PmmaAbsorber();
            break;
        case 2:
            ab = new ScintAbsorber();
            break;
        case 3:
            ab = new LiquidCaloAbsorber();
            break;
    }
    return ab;
}

//______________________________________________________________________________
//  Navigate the tree view to the inserted path
void gui::on_goToEnteredPath_clicked()
{
    QString pathStr = ui->pathEnter->text();
    QDir pathDir(pathStr);
    if (pathDir.exists() == true)
        dirModel->setRootPath(pathStr);

}

//______________________________________________________________________________
//  If an item in the tree view is clicked once, update the navigation bar
void gui::on_fileTV_clicked(const QModelIndex &index)
{
    ui->pathEnter->setText(dirModel->filePath(index));
}

//______________________________________________________________________________
//  If double click on item in the tree view, preview the file histogram
void gui::on_fileTV_doubleClicked(const QModelIndex &index)
{
    QString temp = dirModel->filePath(index);
    QFileInfo tf(temp);
    currentRunFile = temp;
    openRun(currentRunFile);
}


//______________________________________________________________________________
//  Open the single run plot in an external interactive TCanvas
void gui::on_toolButton_7_clicked()
{
    if(isMeasPointInit == false)
        return;

    TCanvas * C1 = new TCanvas();
    C1->cd();
    currentMeasPoint->Draw();
    C1->Draw();
}

//______________________________________________________________________________
//  Gauss fit of the current plot (WITH REPAINT...are we sure to add it everywhere?)
void gui::on_toolButton_8_clicked()
{
    std::vector<double>          p_i;
    std::vector<bool>            b;
    std::vector<double>          p;
    std::vector<bool>            r;
    std::vector<double>          p_min;
    std::vector<double>          p_max;

    QLineEdit * p_sr[] = {ui->p0_sr, ui->p1_sr, ui->p2_sr, ui->p3_sr, ui->p4_sr};
    QLineEdit * p_val[] = {ui->p0_val_sr, ui->p1_val_sr, ui->p2_val_sr, ui->p3_val_sr, ui->p4_val_sr};
    QLineEdit * pmin[] = {ui->p0_min_sr, ui->p1_min_sr, ui->p2_min_sr, ui->p3_min_sr, ui->p4_min_sr};
    QLineEdit * pmax[] = {ui->p0_max_sr, ui->p1_max_sr, ui->p2_max_sr, ui->p3_max_sr, ui->p4_max_sr};
    QCheckBox * p_val_box[] = {ui->check_sr_val_0, ui->check_sr_val_1, ui->check_sr_val_2, ui->check_sr_val_3, ui->check_sr_val_4};
    QCheckBox * p_range_box[] = {ui->check_sr_range_0, ui->check_sr_range_1, ui->check_sr_range_2, ui->check_sr_range_3, ui->check_sr_range_4};

    ui->res_sr->setText("Running optimizer...");
    ui->sr_numpar->setText(QString::number(3));

    for (int i = 0; i < ui->sr_numpar->text().toInt(); ++i)
    {
        p_i.push_back(p_sr[i]->text().toDouble());
        b.push_back(p_val_box[i]->isChecked());
        p.push_back(p_val[i]->text().toDouble());
        r.push_back(p_range_box[i]->isChecked());
        p_min.push_back(pmin[i]->text().toDouble());
        p_max.push_back(pmax[i]->text().toDouble());
    }

    currentMpFit->gauss1D(currentMeasPoint, p_i, b, p, r, p_min, p_max);
    ui->srModelExp->setText(QString(currentMpFit->getFit1D()->GetExpFormula()));

    for (int i = 0; i < ui->sr_numpar->text().toInt(); ++i)
    {
        p_sr[i]->setText(QString::number(currentMpFit->getFit1D()->GetParameter(i)));
    }

    ui->chi2_sr->setText(QString::number(currentMpFit->getFit1D()->GetChisquare()));
    ui->ndf_sr->setText(QString::number(currentMpFit->getFit1D()->GetNDF()));
    ui->chi2rid_sr->setText(QString::number(currentMpFit->getFit1D()->GetChisquare() / currentMpFit->getFit1D()->GetNDF()));
    ui->res_sr->setText("Fit completed");

    TCanvas * C1 = new TCanvas("C1", currentMpFit->getFitModelName().toStdString().c_str(), wc1s, hc1s);
    C1->cd();
    currentMpFit->getFit1D()->SetLineWidth(15);
    currentMeasPoint->SetLineColor(25);
    gStyle->SetOptFit(0111);
    currentMeasPoint->Draw("");
    isMeasPointInit = true;
    displayTCanvas(C1, ui->singleRunDisplay);
    delete C1;

}


//______________________________________________________________________________
//  Landau fit of the current plot
void gui::on_toolButton_9_clicked()
{
    std::vector<double>          p_i;
    std::vector<bool>            b;
    std::vector<double>          p;
    std::vector<bool>            r;
    std::vector<double>          p_min;
    std::vector<double>          p_max;

    QLineEdit * p_sr[] = {ui->p0_sr, ui->p1_sr, ui->p2_sr, ui->p3_sr, ui->p4_sr};
    QLineEdit * p_val[] = {ui->p0_val_sr, ui->p1_val_sr, ui->p2_val_sr, ui->p3_val_sr, ui->p4_val_sr};
    QLineEdit * pmin[] = {ui->p0_min_sr, ui->p1_min_sr, ui->p2_min_sr, ui->p3_min_sr, ui->p4_min_sr};
    QLineEdit * pmax[] = {ui->p0_max_sr, ui->p1_max_sr, ui->p2_max_sr, ui->p3_max_sr, ui->p4_max_sr};
    QCheckBox * p_val_box[] = {ui->check_sr_val_0, ui->check_sr_val_1, ui->check_sr_val_2, ui->check_sr_val_3, ui->check_sr_val_4};
    QCheckBox * p_range_box[] = {ui->check_sr_range_0, ui->check_sr_range_1, ui->check_sr_range_2, ui->check_sr_range_3, ui->check_sr_range_4};

    ui->res_sr->setText("Running optimizer...");
    ui->sr_numpar->setText(QString::number(3));

    for (int i = 0; i < ui->sr_numpar->text().toInt(); ++i)
    {
        p_i.push_back(p_sr[i]->text().toDouble());
        b.push_back(p_val_box[i]->isChecked());
        p.push_back(p_val[i]->text().toDouble());
        r.push_back(p_range_box[i]->isChecked());
        p_min.push_back(pmin[i]->text().toDouble());
        p_max.push_back(pmax[i]->text().toDouble());
    }

    currentMpFit->landau(currentMeasPoint, p_i, b, p, r, p_min, p_max);
    ui->srModelExp->setText(QString(currentMpFit->getFit1D()->GetExpFormula()));

    for (int i = 0; i < ui->sr_numpar->text().toInt(); ++i)
    {
        p_sr[i]->setText(QString::number(currentMpFit->getFit1D()->GetParameter(i)));
    }

    ui->chi2_sr->setText(QString::number(currentMpFit->getFit1D()->GetChisquare()));
    ui->ndf_sr->setText(QString::number(currentMpFit->getFit1D()->GetNDF()));
    ui->chi2rid_sr->setText(QString::number(currentMpFit->getFit1D()->GetChisquare() / currentMpFit->getFit1D()->GetNDF()));
    ui->res_sr->setText("Fit completed");

    TCanvas * C1 = new TCanvas("C1", currentMpFit->getFitModelName().toStdString().c_str(), wc1s, hc1s);
    C1->cd();
    currentMpFit->getFit1D()->SetLineWidth(15);
    currentMeasPoint->SetLineColor(25);
    gStyle->SetOptFit(0111);
    currentMeasPoint->Draw("");
    isMeasPointInit = true;
    displayTCanvas(C1, ui->singleRunDisplay);
    delete C1;

}

//______________________________________________________________________________
//  Fit of the current plot using custom formula
void gui::on_toolButton_10_clicked()
{
    std::vector<double>          p_i;
    std::vector<bool>            b;
    std::vector<double>          p;
    std::vector<bool>            r;
    std::vector<double>          p_min;
    std::vector<double>          p_max;

    QLineEdit * p_sr[] = {ui->p0_sr, ui->p1_sr, ui->p2_sr, ui->p3_sr, ui->p4_sr};
    QLineEdit * p_val[] = {ui->p0_val_sr, ui->p1_val_sr, ui->p2_val_sr, ui->p3_val_sr, ui->p4_val_sr};
    QLineEdit * pmin[] = {ui->p0_min_sr, ui->p1_min_sr, ui->p2_min_sr, ui->p3_min_sr, ui->p4_min_sr};
    QLineEdit * pmax[] = {ui->p0_max_sr, ui->p1_max_sr, ui->p2_max_sr, ui->p3_max_sr, ui->p4_max_sr};
    QCheckBox * p_val_box[] = {ui->check_sr_val_0, ui->check_sr_val_1, ui->check_sr_val_2, ui->check_sr_val_3, ui->check_sr_val_4};
    QCheckBox * p_range_box[] = {ui->check_sr_range_0, ui->check_sr_range_1, ui->check_sr_range_2, ui->check_sr_range_3, ui->check_sr_range_4};

    ui->res_sr->setText("Running optimizer...");

    if (ui->sr_numpar->text().toInt() > 5)
    {
        QMessageBox msgBox;
        msgBox.setText("Maximum number of parameters allowed is 5");
        msgBox.exec();
    }

    else
    {
        for (int i = 0; i < ui->sr_numpar->text().toInt(); ++i)
        {
            p_i.push_back(p_sr[i]->text().toDouble());
            b.push_back(p_val_box[i]->isChecked());
            p.push_back(p_val[i]->text().toDouble());
            r.push_back(p_range_box[i]->isChecked());
            p_min.push_back(pmin[i]->text().toDouble());
            p_max.push_back(pmax[i]->text().toDouble());
        }

        currentMpFit->setFitFormula(QString::fromStdString(ui->srModelExp->text().toStdString()));

        currentMpFit->custom1D(ui->sr_numpar->text().toInt(), currentMpFit->getFitFormula(), currentMeasPoint, p_i, b, p, r, p_min, p_max);

        for (int i = 0; i < ui->sr_numpar->text().toInt(); ++i)
        {
            p_sr[i]->setText(QString::number(currentMpFit->getFit1D()->GetParameter(i)));
        }

        ui->chi2_sr->setText(QString::number(currentMpFit->getFit1D()->GetChisquare()));
        ui->ndf_sr->setText(QString::number(currentMpFit->getFit1D()->GetNDF()));
        ui->chi2rid_sr->setText(QString::number(currentMpFit->getFit1D()->GetChisquare() / currentMpFit->getFit1D()->GetNDF()));
        ui->res_sr->setText("Fit completed");

        TCanvas * C1 = new TCanvas("C1", "Custom 1D fit", wc1s, hc1s);
        C1->cd();
        currentMpFit->getFit1D()->SetLineWidth(15);
        currentMeasPoint->SetLineColor(25);
        gStyle->SetOptFit(0111);
        currentMeasPoint->Draw("");
        isMeasPointInit = true;
        displayTCanvas(C1, ui->singleRunDisplay);
        delete C1;

        /*currentMeasPoint->getFitp().clear();
        for (int i = 0; i < ui->sr_numpar->text().toInt(); ++i)
        {
            currentMeasPoint->getFitp().push_back(currentMpFit->getFit1D()->GetParameter(i));
            currentMeasPoint->getFitp_err().push_back(currentMpFit->getFit1D()->GetParError(i));
        }

        currentMeasPoint->setModel(currentMeasPoint->getModel());
        */
    }

}


//______________________________________________________________________________
//  Fit autoset of parameters using TH1D stats
void gui::on_pushButton_3_clicked()
{
    if (isMeasPointInit == false) return;

    double mean  = currentMeasPoint->GetMean();
    double sigma = currentMeasPoint->GetStdDev();

    ui->p1_e->setText(QString::number(mean));
    ui->p2_e->setText(QString::number(sigma));
    currentMpFit->getFit1D()->SetParameter(1, mean);
    currentMpFit->getFit1D()->SetParameter(2, sigma);
    currentMpFit->getFit1D()->Draw("same");

    TCanvas * C1 = new TCanvas("C1", currentMeasPoint->getFilename().toStdString().c_str(), wc1s, hc1s);
    C1->cd();
    currentMpFit->getFit1D()->SetLineWidth(15);
    currentMeasPoint->SetLineColor(25);
    currentMeasPoint->Draw("");
    gStyle->SetOptFit(0111);
    isMeasPointInit = true;
    displayTCanvas(C1, ui->singleRunDisplay);
    delete C1;
}

//______________________________________________________________________________
//  Clear fit and reload the last run
void gui::on_pushButton_4_clicked()
{
    if (isMeasPointInit == false) return;
    openRun(currentRunFile);
    resetFitFields();
    currentMeasPoint->getFitp().clear();
    currentMeasPoint->getFitp_err().clear();
    currentMeasPoint->setModel("");
}

//______________________________________________________________________________
//  Navigate the tree view to the inserted path
void gui::on_pushButton_clicked()
{

}

//______________________________________________________________________________
//  Add the current run to the current energy profile
void gui::on_toolButton_5_clicked()
{
    if (isMeasPointInit == false) return;
    currentElossReco->Add(currentMeasPoint);
    updateRunTable();
}

//______________________________________________________________________________
//  Remove items
void gui::on_toolButton_6_clicked()
{
    if (ui->runTable->selectedItems().count() > 0)
    {
        int rowIndex = ui->runTable->selectedItems()[0]->row();
        currentElossReco->RemoveAt(rowIndex);
        updateRunTable();
    }
}

//______________________________________________________________________________
//  Double click on a run in the run table
void gui::on_runTable_cellDoubleClicked(int row, int column)
{
    int rowNum = ui->runTable->selectedItems()[0]->row();
    QString path = ui->runTable->item(rowNum, 6)->text();
    openRun(path);
    resetFitFields();
    ui->srModelExp->setText(currentElossReco->getMp()[rowNum]->getModel());
    ui->srModelExp->setText(currentElossReco->getMp()[rowNum]->getModel());

    if (currentElossReco->getMp()[rowNum]->getFitp().size() > 0)
        ui->p0_sr->setText(QString::number(currentElossReco->getMp()[rowNum]->getFitp()[0]));
    if (currentElossReco->getMp()[rowNum]->getFitp().size() > 1)
        ui->p1_sr->setText(QString::number(currentElossReco->getMp()[rowNum]->getFitp()[1]));
    if (currentElossReco->getMp()[rowNum]->getFitp().size() > 2)
        ui->p2_sr->setText(QString::number(currentElossReco->getMp()[rowNum]->getFitp()[2]));
    if (currentElossReco->getMp()[rowNum]->getFitp().size() > 3)
        ui->p3_sr->setText(QString::number(currentElossReco->getMp()[rowNum]->getFitp()[3]));
    if (currentElossReco->getMp()[rowNum]->getFitp().size() > 4)
        ui->p4_sr->setText(QString::number(currentElossReco->getMp()[rowNum]->getFitp()[4]));
}

//______________________________________________________________________________
//  Clear the elossreco buffer
void gui::on_toolButton_4_clicked()
{
    while(currentElossReco->getMp().size() > 0)
        currentElossReco->RemoveAt(0);
    updateRunTable();
}

//______________________________________________________________________________
//  Process Geant4 Output stdout, stderr (non blocking)
void gui::processGeant4Output()
{
     QString buffO;
     QString buffE;
     buffO.fromLocal8Bit(elmc->getGeant4()->readAllStandardOutput());
     buffE.fromLocal8Bit(elmc->getGeant4()->readAllStandardError());
     qDebug() << buffO << buffE;
}

//______________________________________________________________________________
//
void gui::simCompleted()
{
    ui->status->setText("Status: ready");
    elmc->parseOutput();
}

//______________________________________________________________________________
//
void gui::on_toolButton_12_clicked()
{
    on_toolButton_60_clicked();
}

//______________________________________________________________________________
//
void gui::on_toolButton_13_clicked()
{
    ui->geantViewport->setScaledContents(false);
    simViewportZoomIn();
}

//______________________________________________________________________________
//
void gui::on_T1_mat_currentIndexChanged(int index)
{
    ui->T1_A->setText(QString::fromStdString(globalMaterialRegister[index].A));
    ui->T1_Z->setText(QString::fromStdString(globalMaterialRegister[index].Z));
    ui->T1_rho->setText(QString::number(globalMaterialRegister[index].rho));
}

//______________________________________________________________________________
//
void gui::on_W1_mat_currentIndexChanged(int index)
{
    ui->W1_A->setText(QString::fromStdString(globalMaterialRegister[index].A));
    ui->W1_Z->setText(QString::fromStdString(globalMaterialRegister[index].Z));
    ui->W1_rho->setText(QString::number(globalMaterialRegister[index].rho));
}

//______________________________________________________________________________
//
void gui::on_W2_mat_currentIndexChanged(int index)
{
    ui->W2_A->setText(QString::fromStdString(globalMaterialRegister[index].A));
    ui->W2_Z->setText(QString::fromStdString(globalMaterialRegister[index].Z));
    ui->W2_rho->setText(QString::number(globalMaterialRegister[index].rho));
}

//______________________________________________________________________________
//
void gui::on_CAL_mat_currentIndexChanged(int index)
{
    ui->CAL_A->setText(QString::fromStdString(globalMaterialRegister[index].A));
    ui->CAL_Z->setText(QString::fromStdString(globalMaterialRegister[index].Z));
    ui->CAL_rho->setText(QString::number(globalMaterialRegister[index].rho));
}

//______________________________________________________________________________
//
void gui::on_S_mat_currentIndexChanged(int index)
{
    ui->S_A->setText(QString::fromStdString(globalMaterialRegister[index].A));
    ui->S_Z->setText(QString::fromStdString(globalMaterialRegister[index].Z));
    ui->S_rho->setText(QString::number(globalMaterialRegister[index].rho));
}

//______________________________________________________________________________
//
void gui::on_Ions_menu_currentIndexChanged(int index)
{
    ui->Ion_A->setText(QString::number(globalIonRegister[index].A));
    ui->Ion_Z->setText(QString::number(globalIonRegister[index].Z));
    ui->Ion_Q->setText(QString::number(globalIonRegister[index].Q));
}

//______________________________________________________________________________
//
void gui::on_pushButton_14_clicked()
{
    if (ui->newrunName->text().isEmpty()) return;
    ElossReco *newelossReco = new ElossReco();
    newelossReco->SetTitle(ui->newrunName->text().toStdString().c_str());
    elossCollection.push_back(newelossReco);
    refreshElossList();
    ui->newrunName->clear();
}

//______________________________________________________________________________
//
void gui::on_pushButton_15_clicked()
{
    if (ui->elossList->selectedItems().count() > 0)
        removeElossList(ui->elossList->currentIndex().row());
    updateRunTable();
}

//______________________________________________________________________________
//
void gui::on_pushButton_17_clicked()
{
    while (elossCollection.size() > 1)
        for (int i = 0; i < elossCollection.size(); ++i)
            removeElossList(i);
    updateRunTable();
}

//______________________________________________________________________________
//
void gui::on_pushButton_19_clicked()
{
    if (ui->elossList->selectedItems().count() > 0)
    {
        int j = ui->elossList->currentIndex().row();
        if (ui->renamedTitle->text().isEmpty()) return;
        elossCollection[j]->SetTitle(ui->renamedTitle->text().toStdString().c_str());
        ui->renamedTitle->clear();
        refreshElossList();
    }
}

//______________________________________________________________________________
//
void gui::on_pushButton_16_clicked()
{
    if (ui->elossList->selectedItems().count() > 0)
    {
        currentElossReco = elossCollection[ui->elossList->currentIndex().row()];
        isMeasPointInit = true;
        updateRunTable();
        resetFitFields();
        isMeasPointInit = false;
        ui->singleRunDisplay->clear();
        //Deallocate everything
        ui->currentSeriesName->setText(QString::fromLocal8Bit(currentElossReco->GetTitle()));
        ui->elossPointTable->clear();
        ui->elossPointTable->setRowCount(0);
    }
}

//______________________________________________________________________________
//
void gui::on_elossList_itemDoubleClicked(QListWidgetItem *item)
{
    on_pushButton_16_clicked();
}

//______________________________________________________________________________
//
void gui::on_pushButton_20_clicked()
{
    energyLossCanvas = new TCanvas("energyLossCanvas", "Energy loss canvas", 800, 600);
    currentElossReco->DrawEloss(energyLossCanvas);
    displayTCanvas(energyLossCanvas, ui->eLossDisplay);
    fillElossPointTable();
}

//______________________________________________________________________________
//  Add a new TH2D to the collection from X,Y data coming from a bitmap scan
void gui::on_pushButton_22_clicked()
{
    //data from PNG gafchromic scan
    QFileInfo qfi(ui->filePath2D->text());
    if (qfi.exists() == false) return;
    currentTransverse->addGafchromic(ui->filePath2D->text(),
                                     ui->xMin2D->text().toDouble(),
                                     ui->yMin2D->text().toDouble(),
                                     ui->xMax2D->text().toDouble(),
                                     ui->yMax2D->text().toDouble());
    refreshTransverse();
    displayCurrentTransverseSlice(false);
}

//______________________________________________________________________________
//  Add a new TH2D to the collection from X,Y data coming from the calorimeter elossReco
void gui::on_pushButton_29_clicked()
{
    if (ui->zMin2D->text().toDouble() > ui->zMax2D->text().toDouble())
    {
        QMessageBox msgBox;
        msgBox.setText("Range set for Z is not valid");
        msgBox.exec();
    }

    else {
        currentTransverse->addElossProfile(currentElossReco, ui->zMin2D->text().toDouble(), ui->zMax2D->text().toDouble());
        refreshTransverse();
        displayCurrentTransverseSlice(false);
    }
}

//______________________________________________________________________________
//  Refresh the drawing and the point table when the index of the selected slice changes
void gui::on_transverseList_currentIndexChanged(int index)
{
    displayCurrentTransverseSlice(false);
}

//______________________________________________________________________________
//  Fill the draw option text box with "lego2" for 3D style
void gui::on_pushButton_25_clicked()
{
    ui->transverseDrawOption->setText("lego2");
    ui->transverseDrawOption->update();
    ui->transverseDrawOption->repaint();
}

//______________________________________________________________________________
//  Fill the draw option text box with "COLZ" for 2D contourf style
void gui::on_pushButton_26_clicked()
{
    ui->transverseDrawOption->setText("COLZ");
    ui->transverseDrawOption->update();
    ui->transverseDrawOption->repaint();
}

//______________________________________________________________________________
//  Fill the draw option text box with "COLZPOL" for 2D contourf style and polar coordinates
void gui::on_pushButton_27_clicked()
{
    ui->transverseDrawOption->setText("COLZPOL");
    ui->transverseDrawOption->update();
    ui->transverseDrawOption->repaint();
}

//______________________________________________________________________________
//  Refresh the drawing and the point table when the button "Update Draw" is pressed
void gui::on_pushButton_23_clicked()
{
    refreshTransverse();
    displayCurrentTransverseSlice(false);
}

//______________________________________________________________________________
//  Clear the transversal beam canvas
void gui::on_pushButton_24_clicked()
{
    ui->transverseDisplay->clear();
}

//______________________________________________________________________________
//  Remove the selected slice from the transversal beam TH2Ds collection
void gui::on_toolButton_11_clicked()
{
    currentTransverse->removeAt(ui->transverseList->currentIndex());
    ui->transverseDisplay->clear();
    refreshTransverse();
}

//______________________________________________________________________________
//  Draw in external canvas
void gui::on_pushButton_28_clicked()
{
    displayCurrentTransverseSlice(true);
}

//______________________________________________________________________________
//  Synthetize macro
void gui::on_pushButton_12_clicked()
{
    //Reset previous allocation
    double zeroRef = 0.;
    elmc->clearModel();
    int index = 0;

    //Build the detector
    T1Mat = getMaterialByIndex(ui->T1_mat->currentIndex(), T1Mat);
    if (true) //ui->checkT1->isChecked() ==
    {
        elmc->getCalorimeter()->buildBeamDeliveryPMT(
                    ui->D0->text().toDouble(),
                    ui->D0Tick->text().toDouble(),
                    T1Mat,
                    zeroRef,
                    index
                    );
    }

    //W1
    W1Mat = getMaterialByIndex(ui->W1_mat->currentIndex(), W1Mat);
    if (true) //ui->enableW1->isChecked() ==
    {
        elmc->getCalorimeter()->buildCalorimeterWall(
                    ui->D1->text().toDouble(),
                    ui->W1Tick->text().toDouble(),
                    W1Mat,
                    zeroRef,
                    index
                    );
    }

    //W2
    W2Mat = getMaterialByIndex(ui->W2_mat->currentIndex(), W2Mat);
    if (true) //ui->enableW2->isChecked() ==
    {
        elmc->getCalorimeter()->buildSelaedNacelWall(
                    ui->W2Tick->text().toDouble(),
                    W2Mat,
                    zeroRef,
                    index
                    );
    }

    //Calorimeter
    CalMat = getMaterialByIndex(ui->CAL_mat->currentIndex(), CalMat);
    if (true) //ui->enableCalo->isChecked() == true
    {
        elmc->getCalorimeter()->buildCalorimeterAbso(
                    ui->D2Tick->text().toDouble(),
                    CalMat,
                    zeroRef,
                    index
                    );
    }

    //Scintillator
    int nx = 1, ny = 1;

    if (ui->detectorType->currentIndex() == 1)
    {
        nx = 8;
        ny = 8;
    }

    if (true) //ui->enableScint->isChecked() ==
    {
        SMat = getMaterialByIndex(ui->S_mat->currentIndex(), SMat);
        elmc->getCalorimeter()->buildScintillatorXtl(
                    ui->D3->text().toDouble(),
                    ui->D3Tick->text().toDouble(),
                    SMat, zeroRef, nx, ny, ui->scintYZsize->text().toDouble(), index
                    );
    }


    //Set the Monte Carlo simulation parameters
    elmc->setNEvents(ui->nEvents->text().toInt());
    elmc->setBoxX(ui->boxX->text().toDouble());
    elmc->setBoxYZ(ui->boxYZ->text().toDouble());
    elmc->setMaxStep(ui->maxStep->text().toInt());
    elmc->setPrintProgress(ui->printProgress->text().toInt());
    elmc->setPrintMaterials(ui->printMaterials->isChecked());
    elmc->setVerbosity(ui->verbosity->text().toInt());

    elmc->setEnergyMeanMeV(ui->beamEnergyMean->text().toDouble() - ui->energyOffset->text().toDouble());
    elmc->setEnergySigmaMeV(ui->beamEnergySigma->text().toDouble());
    elmc->setBeamFWHM(ui->beamFWHM->text().toDouble());
    G4ion g4ion;
    g4ion.Z = ui->Ion_Z->text().toInt();
    g4ion.A = ui->Ion_A->text().toInt();
    g4ion.Q = ui->Ion_Q->text().toInt();
    elmc->setG4ion(g4ion);

    //Synthetize macro
    ui->macroEdit->setPlainText(elmc->synthetizeMacro());
    ui->macroEdit->update();
    ui->macroEdit->repaint();
}

//______________________________________________________________________________
//  Launch Geant4 Simulation
void gui::on_toolButton_59_clicked()
{
    ui->status->setText("Geant4 sim. started, stdout on terminal");
    ui->status->repaint();
    QDir qdir;
    QFileInfo qfileinfo;
    QString filename = QString::fromLocal8Bit(qgetenv("TEMPFOLDER")) + QString("/macro.mac");

    if (qfileinfo.exists(filename) == true) qdir.remove(filename);
    QFile file(filename);
    std::string content = ui->macroEdit->toPlainText().toStdString();
    if ( file.open(QIODevice::ReadWrite) )
        file.write(content.c_str());

    connect (elmc->getGeant4(), SIGNAL(readyReadStandardOutput()), this, SLOT(processGeant4Output()));
    connect (elmc->getGeant4(), SIGNAL(readyReadStandardError()),  this, SLOT(processGeant4Output()));
    connect (elmc->getGeant4(), SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(simCompleted()));
    elmc->runMacro(filename);
    qDebug() << "\nSimulation completed, control returned to the application\n";
    simCompleted();
    ui->status->repaint();
}

//______________________________________________________________________________
//  Parse Geant4 Simulation results
void gui::on_toolButton_60_clicked()
{
    ui->status->setText("Rendering and parsing results");
    ui->status->repaint();
    QString rpath = elmc->parseOutput();
    QString TEMP        = QString::fromLocal8Bit(qgetenv("TEMPFOLDER"));
    const QPixmap g4Pixmap(TEMP + QString("g4.jpg"));
    ui->geantViewport->setPixmap(g4Pixmap);
    const QPixmap rootPixmap(TEMP + QString("/c2.png"));
    ui->geantRootPlots->setPixmap(rootPixmap);
    ui->status->setText("Status: ready");
    ui->status->repaint();
}

//______________________________________________________________________________
//  Clear results
void gui::on_toolButton_62_clicked()
{
    ui->geantRootPlots->clear();
    ui->geantViewport->clear();
    ui->geantRootPlots->repaint();
    ui->geantViewport->repaint();
}

//______________________________________________________________________________
//
void gui::on_nEvents2_editingFinished()
{
    ui->nEvents->setText(ui->nEvents2->text());
}

//______________________________________________________________________________
//
void gui::on_nEvents_editingFinished()
{
    ui->nEvents2->setText(ui->nEvents->text());
}

//______________________________________________________________________________
//
void gui::on_toolButton_61_clicked()
{
    on_pushButton_12_clicked();
}

//______________________________________________________________________________
//  Export macro
void gui::on_pushButton_8_clicked()
{
    if ((ui->soMacroPath->text().isEmpty() == true) || (ui->soMacroPath->text().isNull() == true)) return;
    QString filename = ui->soMacroPath->text();
    QFile file(filename);
    std::string content = ui->macroEdit->toPlainText().toStdString();
    if ( file.open(QIODevice::ReadWrite) )
        file.write(content.c_str());
}

//______________________________________________________________________________
//  Read macro from file
void gui::on_pushButton_9_clicked()
{
    QFileInfo qf(ui->soMacroPath->text());
    if (qf.exists() == false) return;
    QFile file(ui->soMacroPath->text());
    ui->macroEdit->clear();
    QString line;


    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
            QTextStream stream(&file);
            while (!stream.atEnd())
            {
                line = stream.readLine();
                ui->macroEdit->setPlainText(ui->macroEdit->toPlainText()+line+"\n");
            }
    }
    file.close();
    ui->macroEdit->repaint();
}


//______________________________________________________________________________
// Fit with Moyal function
void gui::on_fitMoyal_clicked()
{
    std::vector<double>          p_i;
    std::vector<bool>            b;
    std::vector<double>          p;
    std::vector<bool>            r;
    std::vector<double>          p_min;
    std::vector<double>          p_max;

    QLineEdit * p_e[] = {ui->p0_e, ui->p1_e, ui->p2_e, ui->p3_e, ui->p4_e, ui->p5_e};
    QLineEdit * p_val[] = {ui->p0_val_e, ui->p1_val_e, ui->p2_val_e, ui->p3_val_e, ui->p4_val_e, ui->p5_val_e};
    QLineEdit * pmin[] = {ui->p0_min_e, ui->p1_min_e, ui->p2_min_e, ui->p3_min_e, ui->p4_min_e, ui->p5_min_e};
    QLineEdit * pmax[] = {ui->p0_max_e, ui->p1_max_e, ui->p2_max_e, ui->p3_max_e, ui->p4_max_e, ui->p5_max_e};
    QCheckBox * p_val_box[] = {ui->check_e_val_0, ui->check_e_val_1, ui->check_e_val_2, ui->check_e_val_3, ui->check_e_val_4, ui->check_e_val_5};
    QCheckBox * p_range_box[] = {ui->check_e_range_0, ui->check_e_range_1, ui->check_e_range_2, ui->check_e_range_3, ui->check_e_range_4, ui->check_e_range_5};

    ui->res_e->setText("Running optimizer...");
    ui->eLoss_numpar->setText(QString::number(6));

    for (int i = 0; i < ui->eLoss_numpar->text().toInt(); ++i)
    {
        p_i.push_back(p_e[i]->text().toDouble());
        b.push_back(p_val_box[i]->isChecked());
        p.push_back(p_val[i]->text().toDouble());
        r.push_back(p_range_box[i]->isChecked());
        p_min.push_back(pmin[i]->text().toDouble());
        p_max.push_back(pmax[i]->text().toDouble());
    }

    eFit->moyal(currentElossReco, p_i, b, p, r, p_min, p_max);
    ui->eLossModelExp->setText(QString(eFit->getFit1D()->GetExpFormula()));

    for (int i = 0; i < ui->eLoss_numpar->text().toInt(); ++i)
    {
        p_e[i]->setText(QString::number(eFit->getFit1D()->GetParameter(i)));
    }

    ui->chi2_e->setText(QString::number(eFit->getFit1D()->GetChisquare()));
    ui->ndf_e->setText(QString::number(eFit->getFit1D()->GetNDF()));
    ui->chi2rid_e->setText(QString::number(eFit->getFit1D()->GetChisquare() / eFit->getFit1D()->GetNDF()));
    ui->res_e->setText("Fit completed");

    TCanvas * C2 = new TCanvas("C2", eFit->getFitModelName().toStdString().c_str(), wc1s, hc1s);
    C2->cd();
    eFit->getFit1D()->SetLineWidth(15);
    currentElossReco->SetLineColor(25);
    gStyle->SetOptFit(0111);
    currentElossReco->Draw("");
    displayTCanvas(C2, ui->eLossDisplay);
    delete C2;

}

//______________________________________________________________________________
// Fit energy loss with custom function
void gui::on_toolButton_14_clicked()
{
    std::vector<double>          p_i;
    std::vector<bool>            b;
    std::vector<double>          p;
    std::vector<bool>            r;
    std::vector<double>          p_min;
    std::vector<double>          p_max;

    QLineEdit * p_e[] = {ui->p0_e, ui->p1_e, ui->p2_e, ui->p3_e, ui->p4_e, ui->p5_e};
    QLineEdit * p_val[] = {ui->p0_val_e, ui->p1_val_e, ui->p2_val_e, ui->p3_val_e, ui->p4_val_e, ui->p5_val_e};
    QLineEdit * pmin[] = {ui->p0_min_e, ui->p1_min_e, ui->p2_min_e, ui->p3_min_e, ui->p4_min_e, ui->p5_min_e};
    QLineEdit * pmax[] = {ui->p0_max_e, ui->p1_max_e, ui->p2_max_e, ui->p3_max_e, ui->p4_max_e, ui->p5_max_e};
    QCheckBox * p_val_box[] = {ui->check_e_val_0, ui->check_e_val_1, ui->check_e_val_2, ui->check_e_val_3, ui->check_e_val_4, ui->check_e_val_5};
    QCheckBox * p_range_box[] = {ui->check_e_range_0, ui->check_e_range_1, ui->check_e_range_2, ui->check_e_range_3, ui->check_e_range_4, ui->check_e_range_5};

    ui->res_e->setText("Running optimizer...");

    if (ui->eLoss_numpar->text().toInt() > 6)
    {
        QMessageBox msgBox;
        msgBox.setText("Maximum number of parameters allowed is 6");
        msgBox.exec();
    }

    else
    {
        for (int i = 0; i < ui->eLoss_numpar->text().toInt(); ++i)
        {
            p_i.push_back(p_e[i]->text().toDouble());
            b.push_back(p_val_box[i]->isChecked());
            p.push_back(p_val[i]->text().toDouble());
            r.push_back(p_range_box[i]->isChecked());
            p_min.push_back(pmin[i]->text().toDouble());
            p_max.push_back(pmax[i]->text().toDouble());
        }

        eFit->setFitFormula(QString::fromStdString(ui->eLossModelExp->text().toStdString()));

        eFit->custom1D(ui->eLoss_numpar->text().toInt(), eFit->getFitFormula(), currentElossReco, p_i, b, p, r, p_min, p_max);

        for (int i = 0; i < ui->eLoss_numpar->text().toInt(); ++i)
        {
            p_e[i]->setText(QString::number(eFit->getFit1D()->GetParameter(i)));
        }

        ui->chi2_e->setText(QString::number(eFit->getFit1D()->GetChisquare()));
        ui->ndf_e->setText(QString::number(eFit->getFit1D()->GetNDF()));
        ui->chi2rid_e->setText(QString::number(eFit->getFit1D()->GetChisquare() / eFit->getFit1D()->GetNDF()));
        ui->res_e->setText("Fit completed");

        TCanvas * C2 = new TCanvas("C2", "Custom 1D fit", wc1s, hc1s);
        C2->cd();
        eFit->getFit1D()->SetLineWidth(15);
        currentElossReco->SetLineColor(25);
        gStyle->SetOptFit(0111);
        currentElossReco->Draw("");
        displayTCanvas(C2, ui->eLossDisplay);
        delete C2;
    }

}

//______________________________________________________________________________
// Clear energy loss fit
void gui::on_pushButton_34_clicked()
{
    eLossResetFitFields();
}

//______________________________________________________________________________
// 2D Gaussian fit to beam spreadout
void gui::on_toolButton_22_clicked()
{
    std::vector<double>          p_i;
    std::vector<bool>            b;
    std::vector<double>          p;
    std::vector<bool>            r;
    std::vector<double>          p_min;
    std::vector<double>          p_max;

    QLineEdit * p_t[] = {ui->p0_t, ui->p1_t, ui->p2_t, ui->p3_t, ui->p4_t, ui->p5_t};
    QLineEdit * p_val[] = {ui->p0_val_t, ui->p1_val_t, ui->p2_val_t, ui->p3_val_t, ui->p4_val_t, ui->p5_val_t};
    QLineEdit * pmin[] = {ui->p0_min_t, ui->p1_min_t, ui->p2_min_t, ui->p3_min_t, ui->p4_min_t, ui->p5_min_t};
    QLineEdit * pmax[] = {ui->p0_max_t, ui->p1_max_t, ui->p2_max_t, ui->p3_max_t, ui->p4_max_t, ui->p5_max_t};
    QCheckBox * p_val_box[] = {ui->check_t_val_0, ui->check_t_val_1, ui->check_t_val_2, ui->check_t_val_3, ui->check_t_val_4, ui->check_t_val_5};
    QCheckBox * p_range_box[] = {ui->check_t_range_0, ui->check_t_range_1, ui->check_t_range_2, ui->check_t_range_3, ui->check_t_range_4, ui->check_t_range_5};

    //ui->transverseDisplay->clear();
    //transverseResetFitFields();

    ui->res_t->setText("Running optimizer...");
    ui->t_numpar->setText(QString::number(6));

    for (int i = 0; i < ui->t_numpar->text().toInt(); ++i)
    {
        p_i.push_back(p_t[i]->text().toDouble());
        b.push_back(p_val_box[i]->isChecked());
        p.push_back(p_val[i]->text().toDouble());
        r.push_back(p_range_box[i]->isChecked());
        p_min.push_back(pmin[i]->text().toDouble());
        p_max.push_back(pmax[i]->text().toDouble());
    }

    transFit->gauss2D(ui->transverseList->currentIndex(), currentTransverse, p_i, b, p, r, p_min, p_max);
    ui->modelExp_t->setText(QString(transFit->getFit2D()->GetExpFormula()));

    for (int i = 0; i < ui->t_numpar->text().toInt(); ++i)
    {
        p_t[i]->setText(QString::number(transFit->getFit2D()->GetParameter(i)));
    }

    ui->chi2_t->setText(QString::number(transFit->getFit2D()->GetChisquare()));
    ui->ndf_t->setText(QString::number(transFit->getFit2D()->GetNDF()));
    ui->chi2rid_t->setText(QString::number(transFit->getFit2D()->GetChisquare() / transFit->getFit2D()->GetNDF()));
    ui->res_t->setText("Fit completed");

    gStyle->SetOptFit(0111);
    displayCurrentTransverseSlice(false);

}

//______________________________________________________________________________
// 2D custom fit to beam spreadout
void gui::on_toolButton_21_clicked()
{
    std::vector<double>          p_i;
    std::vector<bool>            b;
    std::vector<double>          p;
    std::vector<bool>            r;
    std::vector<double>          p_min;
    std::vector<double>          p_max;

    QLineEdit * p_t[] = {ui->p0_t, ui->p1_t, ui->p2_t, ui->p3_t, ui->p4_t, ui->p5_t};
    QLineEdit * p_val[] = {ui->p0_val_t, ui->p1_val_t, ui->p2_val_t, ui->p3_val_t, ui->p4_val_t, ui->p5_val_t};
    QLineEdit * pmin[] = {ui->p0_min_t, ui->p1_min_t, ui->p2_min_t, ui->p3_min_t, ui->p4_min_t, ui->p5_min_t};
    QLineEdit * pmax[] = {ui->p0_max_t, ui->p1_max_t, ui->p2_max_t, ui->p3_max_t, ui->p4_max_t, ui->p5_max_t};
    QCheckBox * p_val_box[] = {ui->check_t_val_0, ui->check_t_val_1, ui->check_t_val_2, ui->check_t_val_3, ui->check_t_val_4, ui->check_t_val_5};
    QCheckBox * p_range_box[] = {ui->check_t_range_0, ui->check_t_range_1, ui->check_t_range_2, ui->check_t_range_3, ui->check_t_range_4, ui->check_t_range_5};

    ui->transverseDisplay->clear();

    ui->res_t->setText("Running optimizer...");

    if (ui->t_numpar->text().toInt() > 6)
    {
        QMessageBox msgBox;
        msgBox.setText("Maximum number of parameters allowed is 6");
        msgBox.exec();
    }

    else
    {
        for (int i = 0; i < ui->t_numpar->text().toInt(); ++i)
        {
            p_i.push_back(p_t[i]->text().toDouble());
            b.push_back(p_val_box[i]->isChecked());
            p.push_back(p_val[i]->text().toDouble());
            r.push_back(p_range_box[i]->isChecked());
            p_min.push_back(pmin[i]->text().toDouble());
            p_max.push_back(pmax[i]->text().toDouble());
        }

        transFit->setFitFormula(QString::fromStdString(ui->modelExp_t->text().toStdString()));

        transFit->custom2D(ui->transverseList->currentIndex(), ui->t_numpar->text().toInt(), transFit->getFitFormula(), currentTransverse, p_i, b, p, r, p_min, p_max);

        for (int i = 0; i < ui->t_numpar->text().toInt(); ++i)
        {
            p_t[i]->setText(QString::number(transFit->getFit2D()->GetParameter(i)));
        }

        ui->chi2_t->setText(QString::number(transFit->getFit2D()->GetChisquare()));
        ui->ndf_t->setText(QString::number(transFit->getFit2D()->GetNDF()));
        ui->chi2rid_t->setText(QString::number(transFit->getFit2D()->GetChisquare() / transFit->getFit2D()->GetNDF()));
        ui->res_t->setText("Fit completed");

        gStyle->SetOptFit(0111);
        displayCurrentTransverseSlice(false);
    }

}

//______________________________________________________________________________
// Clear transverse beam spreadout fit
void gui::on_pushButton_35_clicked()
{
    transverseResetFitFields();
}

//______________________________________________________________________________
//  Reset transverse beam spreadout fit fields
void gui::transverseResetFitFields()
{
    ui->p0_t->setText("");
    ui->p1_t->setText("");
    ui->p2_t->setText("");
    ui->p3_t->setText("");
    ui->p4_t->setText("");
    ui->p5_t->setText("");
    ui->p0_val_t->setText("");
    ui->p1_val_t->setText("");
    ui->p2_val_t->setText("");
    ui->p3_val_t->setText("");
    ui->p4_val_t->setText("");
    ui->p5_val_t->setText("");
    ui->p0_min_t->setText("");
    ui->p1_min_t->setText("");
    ui->p2_min_t->setText("");
    ui->p3_min_t->setText("");
    ui->p4_min_t->setText("");
    ui->p5_min_t->setText("");
    ui->p0_max_t->setText("");
    ui->p1_max_t->setText("");
    ui->p2_max_t->setText("");
    ui->p3_max_t->setText("");
    ui->p4_max_t->setText("");
    ui->p5_max_t->setText("");
    ui->chi2_t->setText("");
    ui->ndf_t->setText("");
    ui->chi2rid_t->setText("");
    ui->modelExp_t->setText("");
    ui->res_t->setText("");
    ui->t_numpar->setText("");
    ui->check_t_val_0->setChecked(false);
    ui->check_t_val_1->setChecked(false);
    ui->check_t_val_2->setChecked(false);
    ui->check_t_val_3->setChecked(false);
    ui->check_t_val_4->setChecked(false);
    ui->check_t_val_5->setChecked(false);
    ui->check_t_range_0->setChecked(false);
    ui->check_t_range_1->setChecked(false);
    ui->check_t_range_2->setChecked(false);
    ui->check_t_range_3->setChecked(false);
    ui->check_t_range_4->setChecked(false);
    ui->check_t_range_5->setChecked(false);
}


//______________________________________________________________________________
//
void gui::on_toolButton_63_clicked()
{

}

//______________________________________________________________________________
//
void gui::on_toolButton_15_clicked()
{
    ui->geantViewport->setScaledContents(false);
    simViewportZoomOut();
}

//______________________________________________________________________________
//
void gui::on_toolButton_16_clicked()
{
    if (isG4ViewportInit == false) return;
    simViewportSave();
}

void gui::on_toolButton_17_clicked()
{
    zoomf = 1.;
    ui->geantViewport->setScaledContents(true);
    updateViewportZoom();
}

//______________________________________________________________________________
//  Export single run root file
void gui::on_toolButton_67_clicked()
{
    FileExportMp *fe = new FileExportMp;
    currentMeasPoint->Export(fe, "root", 0);

    delete fe;
}


//______________________________________________________________________________
//  Export single run canvas on pdf
void gui::on_toolButton_69_clicked()
{
    FileExportMp *fe= new FileExportMp;
    currentMeasPoint->Export(fe, "pdf", 0);

    delete fe;
}

//______________________________________________________________________________
//  Export single run canvas on png
void gui::on_toolButton_72_clicked()
{
    FileExportMp *fe = new FileExportMp;
    currentMeasPoint->Export(fe, "png", 0);

    delete fe;
}

//______________________________________________________________________________
//  Export energy loss root file
void gui::on_toolButton_70_clicked()
{
    FileExportEl *fe = new FileExportEl;
    currentElossReco->Export(fe, "root", 0);

    delete fe;
}


//______________________________________________________________________________
//  Export energy loss canvas on pdf
void gui::on_toolButton_71_clicked()
{
    FileExportEl *fe = new FileExportEl;
    currentElossReco->Export(fe, "pdf", 0);

    delete fe;
}

//______________________________________________________________________________
//  Export energy loss canvas on png
void gui::on_toolButton_73_clicked()
{
    FileExportEl *fe = new FileExportEl;
    currentElossReco->Export(fe, "png", 0);

    delete fe;
}

//______________________________________________________________________________
//  Export transverse beam root file
void gui::on_toolButton_74_clicked()
{
   FileExportTr *fe = new FileExportTr;
   currentTransverse->Export(fe, "root", ui->transverseList->currentIndex());

    delete fe;
}

//______________________________________________________________________________
//  Export transverse beam canvas on pdf
void gui::on_toolButton_75_clicked()
{
    FileExportTr *fe = new FileExportTr;
    currentTransverse->Export(fe, "pdf", ui->transverseList->currentIndex());

    delete fe;
}

//______________________________________________________________________________
//  Export transverse beam canvas on png
void gui::on_toolButton_clicked()
{
    FileExportTr *fe = new FileExportTr;
    currentTransverse->Export(fe, "png", ui->transverseList->currentIndex());

    delete fe;
}
