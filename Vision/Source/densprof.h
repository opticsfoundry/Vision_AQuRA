#if !defined(__densprof_h)
#define __densprof_h

#include "densprof.h"

void SetFermiFuncParams(double afax,double afrad, double asigma, double amasse,double atof,double ax0,
  double aoff,double aslo, bool aFitFermi, bool aFitHorizontal);
double FermiFunc(double x, double a[]);

#endif
