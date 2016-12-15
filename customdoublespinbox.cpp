#include "customdoublespinbox.h"

CustomDoubleSpinBox::CustomDoubleSpinBox(QWidget *parent) : QDoubleSpinBox(parent)
{
}

QString CustomDoubleSpinBox::textFromValue(double value) const {
    return QString::number(value, 'g', NUMBER_OF_DECIMALS);
}
