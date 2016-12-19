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