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

#include "customdoublespinbox.h"

CustomDoubleSpinBox::CustomDoubleSpinBox(QWidget *parent) : QDoubleSpinBox(parent)
{
}

QString CustomDoubleSpinBox::textFromValue(double value) const {
    return QString::number(value, 'g', NUMBER_OF_DECIMALS);
}
