#ifndef FERMIPLOT_H
#define FERMIPLOT_H

const double ELECTRON_RMASS_MEV = 0.510998902;

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

double coefficient(unsigned int n, unsigned int z, double energy, bool positron);
double base_factor(double Q);
double correction_factor(double energy, unsigned int z, bool positron);
double f_value(double Q, unsigned int z, bool positron);
double logft(double energy, unsigned int z, bool positron, double time, double intensity);
#endif // FERMIPLOT_H
