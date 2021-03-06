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

#include "fermicalcwindow.h"
#include "ui_fermicalcwindow.h"
#include "fermicalc.cpp"

FermiCalcWindow::FermiCalcWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::FermiCalcWindow)
{
  srand(QDateTime::currentDateTime().toTime_t());
  ui->setupUi(this);

  tracer = new QCPItemTracer(ui->customPlot);
  tracer->setInterpolating(true);
  tracer->setVisible(false);

  ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->customPlot->xAxis->setRange(0.1, 10.044);
  ui->customPlot->yAxis->setRange(0, 20000);
  ui->customPlot->axisRect()->setupFullAxesBox();

  ui->customPlot->plotLayout()->insertRow(0);
  QCPTextElement *title = new QCPTextElement(ui->customPlot, "Fermi integral plots", QFont("sans", 17, QFont::Bold));
  ui->customPlot->plotLayout()->addElement(0, 0, title);

  ui->customPlot->xAxis->setLabel("Q [MeV]");
  ui->customPlot->yAxis->setLabel("f(Z, E)");
  ui->customPlot->legend->setVisible(true);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->customPlot->legend->setFont(legendFont);
  ui->customPlot->legend->setSelectedFont(legendFont);
  ui->customPlot->legend->setSelectableParts(QCPLegend::spItems);

  addFermiGraph(54, true);
  addFermiGraph(42, false);

  ui->customPlot->rescaleAxes();

  ui->graphBMinus->setChecked(true);
  ui->betaMinusRadio->setChecked(true);

  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));

  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // connect some interaction slots:
  connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(ui->customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  connect(ui->semiLogCheckbox, SIGNAL(toggled(bool)), this, SLOT(semiLogScale(bool)));

  // connect slot that shows a message in the status bar when a graph is clicked:
  connect(ui->customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));

  // setup policy and connect slot for context menu popup:
  ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

  connect(ui->graphButton, SIGNAL(pressed()), this, SLOT(addNewGraph()));
  connect(ui->resizeButton, SIGNAL(pressed()), this, SLOT(resizeGraph()));
  connect(ui->graphMinRangeBox, SIGNAL(valueChanged(double)), this, SLOT(valueUpdate()));
  connect(ui->graphMaxRangeBox, SIGNAL(valueChanged(double)), this, SLOT(valueUpdate()));
}

FermiCalcWindow::~FermiCalcWindow()
{
  delete ui;
}

void FermiCalcWindow::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "Plot title", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->customPlot->replot();
    }
  }
}

void FermiCalcWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "Axis label", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->customPlot->replot();
    }
  }
}

void FermiCalcWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "Graph name", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->customPlot->replot();
    }
  }
}

void FermiCalcWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void FermiCalcWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void FermiCalcWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void FermiCalcWindow::addFermiGraph(unsigned int z, bool positron)
{
    //TODO literal numbers in code
    int n = 100; // number of points in graph

    QVector<double> x(n), y(n);

    for (int i=0; i < n; i++) {
        x[i] = (i / (double) n) * 25.034 + 0.01;
        y[i] = f_value(x[i], z, positron);
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(QString("%1, beta %2").arg(QString::number(z), positron ? "plus" : "minus"));
    ui->customPlot->graph()->setData(x, y);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    QPen graphPen;
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    graphPen.setWidthF(0.0);
    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->replot();
}

void FermiCalcWindow::removeSelectedGraph()
{
  if (ui->customPlot->selectedGraphs().size() > 0)
  {
    ui->customPlot->removeGraph(ui->customPlot->selectedGraphs().first());
    ui->customPlot->replot();
  }
}

void FermiCalcWindow::removeAllGraphs()
{
  ui->customPlot->clearGraphs();
  ui->customPlot->replot();
}

void FermiCalcWindow::addNewGraph() {
    // Should never be the case, but just to be safe
    if (!ui->graphBMinus->isChecked() && !ui->grapBPlus->isChecked()) {
        QMessageBox alert;
        alert.setIcon(QMessageBox::Warning);
        alert.setText("Please select beta decay mode!");
        alert.exec();
        return;
    }

    unsigned int z = ui->graphZValue->value();
    bool pos = ui->grapBPlus->isChecked() ? true : false;
    addFermiGraph(z, pos);
}

void FermiCalcWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    // REMOVE menu->addAction("Add random graph", this, SLOT(addNewGraph()));
    if (ui->customPlot->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->customPlot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }

  menu->popup(ui->customPlot->mapToGlobal(pos));
}

void FermiCalcWindow::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->customPlot->replot();
    }
  }
}

void FermiCalcWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
  // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
  // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
  double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
  QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
  ui->statusBar->showMessage(message, 2500);
}

void FermiCalcWindow::on_logftButton_clicked()
{
    double Q = ui->qMeVDoubleSpinBox->value();

    unsigned int z = ui->zSpinBox->value();

    double time = ui->t12SDoubleSpinBox->value();
    QString timeUnit = ui->timeUnit->currentText();

    if (timeUnit == "years")
        time *= 3600 * 24 * 365;
    else if (timeUnit == "days")
        time *= 3600 * 24;
    else if (timeUnit == "hours")
        time *= 3600;
    else if (timeUnit == "minutes")
        time *= 60;
    else if (timeUnit == "miliseconds")
        time /= 1000;

    double intensity = ui->intensityDoubleSpinBox->value() / 100.0;

    bool positron = false;
    if (ui->betaMinusRadio->isChecked()) {
        positron = false;
    } else if (ui->betaPlusRadio->isChecked()) {
        positron = true;
    }

    double lnft = logft(Q, z, positron, time, intensity);

    ui->logftValueBox->setText(QString::number(lnft));
}


void FermiCalcWindow::semiLogScale(bool state) {
    ui->customPlot->yAxis->setScaleType(state ? QCPAxis::stLogarithmic : QCPAxis::stLinear);
    ui->customPlot->replot();
}

double FermiCalcWindow::getValueByKey(QCPGraph *graph,double key) {
    // Not perfect solution, but good enough for our needs, as we don't use tracer
    tracer->setGraph(graph);
    tracer->setGraphKey(key);
    tracer->updatePosition();

    return tracer->position->value();
}

void FermiCalcWindow::resizeGraph() {
    double minRange = ui->graphMinRangeBox->value();
    double maxRange = ui->graphMaxRangeBox->value();

    ui->customPlot->xAxis->setRange(minRange, maxRange);

    if (ui->customPlot->graphCount() == 0) {
        return;
    }

    double minValue = 0.0;
    double maxValue = 0.0;

    bool firstVal = true;

    for (int i = 0; i < ui->customPlot->graphCount(); ++i) {
        QCPGraph *graph = ui->customPlot->graph(i);

        double currentMinVal = getValueByKey(graph, minRange);
        double currentMaxVal = getValueByKey(graph, maxRange);


        if (firstVal) {
            minValue = currentMinVal;
            maxValue = currentMaxVal;
            firstVal = false;
        } else {
            minValue = currentMinVal < minValue ? currentMinVal : minValue;
            maxValue = currentMaxVal > maxValue ? currentMaxVal : maxValue;
        }
    }

    ui->customPlot->yAxis->setRange(minValue, maxValue);
    ui->customPlot->replot();
}

void FermiCalcWindow::valueUpdate() {
    double minRange = ui->graphMinRangeBox->value();
    double maxRange = ui->graphMaxRangeBox->value();

    if (minRange >= maxRange) {
        ui->resizeButton->setText("Incorrect range");
        ui->resizeButton->setDisabled(true);
    } else {
        ui->resizeButton->setText("Resize");
        ui->resizeButton->setDisabled(false);
    }
}
