#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "qcustomplot.h"
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

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
  Ui::MainWindow *ui;
  std::vector<int> graphIndexes;
  double getValueByKey(QCPGraph *graph,double key);
  QCPItemTracer *tracer;
};

#endif // MAINWINDOW_H
