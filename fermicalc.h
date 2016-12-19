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

#ifndef FERMIPLOT_H
#define FERMIPLOT_H


// CONSTANTS
const double ELECTRON_RMASS_MEV = 0.510998902;

// FUNCTIONS
double coefficient(unsigned int n, unsigned int z, double energy, bool positron);
double base_factor(double Q);
double correction_factor(double energy, unsigned int z, bool positron);
double f_value(double Q, unsigned int z, bool positron);
double logft(double energy, unsigned int z, bool positron, double time, double intensity);
double convert_to_seconds(double time, std::string unit);
bool validate_data(unsigned int z, double energy, double time, double intensity, std::string mode);

// DATA TABLES
extern const double NA0_0[];
extern const double NA0_1[];
extern const double NA0_2[];
extern const double NA0_3[];
extern const double NA0_4[];

extern const double* NA0[];

extern const double NA1_0[];
extern const double NA1_1[];
extern const double NA1_2[];
extern const double NA1_3[];
extern const double NA1_4[];

extern const double* NA1[];

extern const double NA2_0[];
extern const double NA2_1[];
extern const double NA2_2[];
extern const double NA2_3[];
extern const double NA2_4[];

extern const double* NA2[];

extern const double NA3_0[];
extern const double NA3_1[];
extern const double NA3_2[];
extern const double NA3_3[];
extern const double NA3_4[];

extern const double* NA3[];

extern const double PA0_0[];
extern const double PA0_1[];
extern const double PA0_2[];
extern const double PA0_3[];
extern const double PA0_4[];

extern const double* PA0[];

extern const double PA1_0[];
extern const double PA1_1[];
extern const double PA1_2[];
extern const double PA1_3[];
extern const double PA1_4[];

extern const double* PA1[];

extern const double PA2_0[];
extern const double PA2_1[];
extern const double PA2_2[];
extern const double PA2_3[];
extern const double PA2_4[];

extern const double* PA2[];

extern const double PA3_0[];
extern const double PA3_1[];
extern const double PA3_2[];
extern const double PA3_3[];
extern const double PA3_4[];

extern const double* PA3[];

extern const double** NA[];
extern const double** PA[];

#endif // FERMIPLOT_H
