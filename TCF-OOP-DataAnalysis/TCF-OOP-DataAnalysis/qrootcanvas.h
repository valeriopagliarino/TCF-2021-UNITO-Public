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

#ifndef CANVAS_H
#define CANVAS_H

#include <iostream>
#include <string>
#include <QWidget>

//class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class QPushButton;
class QTimer;
class TCanvas;

using namespace std;

//=======================================================
class QRootCanvas : public QWidget {
  Q_OBJECT

public:
  QRootCanvas(QWidget *parent = 0, string title = "");
  ~QRootCanvas(void) {
    if (fCanvas) {
      // delete fCanvas;
    }
  }
  TCanvas *GetCanvas(void) { return fCanvas; }

protected:
  TCanvas *fCanvas;

  virtual void mouseMoveEvent(QMouseEvent *e);
  virtual void mousePressEvent(QMouseEvent *e);
  virtual void mouseReleaseEvent(QMouseEvent *e);
  virtual void paintEvent(QPaintEvent *e);
  virtual void resizeEvent(QResizeEvent *e);
};

#endif
