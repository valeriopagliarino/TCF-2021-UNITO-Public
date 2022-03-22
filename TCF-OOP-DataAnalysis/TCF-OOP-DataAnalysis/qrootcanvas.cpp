/*===============================================================================
 * Monicelli: the FERMILAB MTEST geometry builder and track reconstruction tool
 *
 * Copyright (C) 2014
 *
 * Authors:
 *
 * Dario Menasce      (INFN)
 * Luigi Moroni       (INFN)
 * Jennifer Ngadiuba  (INFN)
 * Stefano Terzo      (INFN)
 * Lorenzo Uplegger   (FNAL)
 * Luigi Vigani       (INFN)
 *
 * INFN: Piazza della Scienza 3, Edificio U2, Milano, Italy 20126
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

#include <QPushButton>
#include <QLayout>
#include <QtCore/QTimer>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QMouseEvent>
#include <QMessageBox>

#include <stdlib.h>
#include <iostream>
#include <sstream>

#include <TCanvas.h>
#include <TVirtualX.h>
#include <TSystem.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1.h>
#include <TFrame.h>
#include <TTimer.h>
#include "qrootcanvas.h"

using namespace std ;

//------------------------------------------------------------------------------

//______________________________________________________________________________
QRootCanvas::QRootCanvas(QWidget *parent, string title) : QWidget(parent, 0), fCanvas(0)
{
   setAttribute     (Qt::WA_PaintOnScreen,    true);
   setAttribute     (Qt::WA_OpaquePaintEvent, true);
   setAttribute     (Qt::WA_NativeWindow,     true);
   setMinimumSize   (50, 50);
   setUpdatesEnabled(kFALSE);
   setMouseTracking (kTRUE );

   int wid = gVirtualX->AddWindow((ULong_t)winId(), width(), height());
   this->setGeometry(
                     0,
                     0,
                     parent->width(),
                     parent->height()
                    );
   fCanvas = new TCanvas(title.c_str(), width(), height(), wid);
   TQObject::Connect("TGPopMenu", "PoppedDown()", "TCanvas", fCanvas, "Update()") ;
}

//______________________________________________________________________________
void QRootCanvas::mouseMoveEvent(QMouseEvent *e)
{
   // Handle mouse move events.

   if (fCanvas)
   {
      if (e->buttons() & Qt::LeftButton)
      {
         fCanvas->HandleInput(kButton1Motion, e->x(), e->y());
      }
      else if (e->buttons() & Qt::MidButton)
      {
         fCanvas->HandleInput(kButton2Motion, e->x(), e->y());
      }
      else if (e->buttons() & Qt::RightButton)
      {
         fCanvas->HandleInput(kButton3Motion, e->x(), e->y());
      }
      else
      {
         fCanvas->HandleInput(kMouseMotion,   e->x(), e->y());
      }
   }
}

//______________________________________________________________________________
void QRootCanvas::mousePressEvent( QMouseEvent *e )
{
   // Handle mouse button press events.

   if (fCanvas)
   {
      switch (e->button())
      {
         case Qt::LeftButton :
            fCanvas->HandleInput(kButton1Down, e->x(), e->y());
            break;
         case Qt::MidButton :
            fCanvas->HandleInput(kButton2Down, e->x(), e->y());
            break;
         case Qt::RightButton :
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true)
            // seems to cure the problem
            fCanvas->HandleInput(kButton3Down, e->x(), e->y());
            break;
         default:
            break;
      }
   }
}

//______________________________________________________________________________
void QRootCanvas::mouseReleaseEvent( QMouseEvent *e )
{
   // Handle mouse button release events.

   if (fCanvas)
   {
      switch (e->button())
      {
         case Qt::LeftButton :
            fCanvas->HandleInput(kButton1Up, e->x(), e->y());
            break;
         case Qt::MidButton :
            fCanvas->HandleInput(kButton2Up, e->x(), e->y());
            break;
         case Qt::RightButton :
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true)
            // seems to cure the problem
            fCanvas->HandleInput(kButton3Up, e->x(), e->y());
            break;
         default:
            break;
      }
   }
}
//______________________________________________________________________________
void QRootCanvas::resizeEvent( QResizeEvent * event)
{
   // Handle resize events.

   if (fCanvas)
   {
      fCanvas->SetCanvasSize(event->size().width(), event->size().height());
      fCanvas->Resize();
      fCanvas->Update();
   }
}

//______________________________________________________________________________
void QRootCanvas::paintEvent( QPaintEvent * )
{
   // Handle paint events.

   if (fCanvas)
   {
      fCanvas->Resize();
      fCanvas->Update();
   }
}



