/***************************************************************************
**                                                                        **
**  Fermi_calc, a log ft calculator based on results from                 **
**  Wilkinson, D.H. et al, "A parametrization of                          **
**  the phase space factor for allowed $\beta$-decay"                     **
**  Copyright (C) 2016 Kasjan Siwek                                       **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Kasjan Siwek                                         **
**  Website/Contact: kasjan.siwek@gmail.com                               **
**             Date: 19.12.16                                             **
****************************************************************************/

#ifndef FERMICALCWINDOW_H
#define FERMICALCWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "qcustomplot.h"

namespace Ui {
class FermiCalcWindow;
}

class FermiCalcWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit FermiCalcWindow(QWidget *parent = 0);
  ~FermiCalcWindow();

private slots:
  void titleDoubleClick(QMouseEvent *event);
  void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
  void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
  void selectionChanged();
  void mousePress();
  void mouseWheel();
  void addNewGraph();
  void addFermiGraph(unsigned int z, bool positron);
  void removeSelectedGraph();
  void removeAllGraphs();
  void contextMenuRequest(QPoint pos);
  void moveLegend();
  void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);
  void semiLogScale(bool state);
  void resizeGraph();
  void on_logftButton_clicked();
  void valueUpdate();

private:
  Ui::FermiCalcWindow *ui;
  double getValueByKey(QCPGraph *graph,double key);
  QCPItemTracer *tracer;
};

#endif // FERMICALCWINDOW_H
