#if !defined(__atommath_h)
#define __atommath_h

double Sigma(double Detuning, double Gamma, double Lambda, double Clebsch);
void temperature(double x, double a[], double *y, double dyda[], int na);
void number(double x, double a[], double *y, double dyda[], int na, double Clebsch, double Lambda, double Gamma);
double Number(double SigmaX, double SigmaY, double DensOpt, double Detuning, double Gamma, double Lambda, double Clebsch);
void ChargingTime(double x, double a[], double *y, double dyda[], int na);
void Lifetime(double x, double a[], double *y, double dyda[], int na);

#endif
