#ifndef CUSTOMDOUBLESPINBOX_H
#define CUSTOMDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QWidget>
#include <QtGui>
#include <iostream>
#include <math.h>
#include <float.h>
#include <limits>

#define NUMBER_OF_DECIMALS 15

class CustomDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    CustomDoubleSpinBox(QWidget *parent = 0);
    virtual QString textFromValue(double value) const;
};

#endif // CUSTOMDOUBLESPINBOX_H
