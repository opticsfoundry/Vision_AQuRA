#if !defined(__fitmath_h)
#define __fitmath_h

void SetMathErrorMessage( bool b);
void gauss(double x, double a[], double *y, double dyda[], int na);
void gaussWithoutOffset(double x, double a[], double *y, double dyda[], int na);
void exponential(double x, double a[], double *y, double dyda[], int na);
bool fit(double x[], double y[], double sig[], int ndata, double a[], double v[],
  	int ma, double *chisq, int maxn, int minn, double minchange,  double minchisq,double maxchange, int &nend,
     double &changeend,void (*funcs)(double, double [], double *, double [], int));
void linear(double x, double a[], double *y, double dyda[], int na);
double SumX(double x1, double x2, double dx, double y, double a[],
    double (*funcs)(double, double ,double []));
double SumY(double x,double y1, double y2, double yx, double a[],
    double (*funcs)(double, double, double []));
void SetIoffeTrapParameters(double aBias, double aGradient, double aCurvature,
  double amy,double asigma, double aMasse, double aSigmaEl);
double IoffeOptDens(double x, double y, double a[]);
void FitIoffeOptDens(double ax, double a[], double *y, double dyda[], int na);
void Set2DFitParameters(double asx, double asy, double adx, double ady,
  int anx, int any, double (*func)(double, double, double []));
void Ioffen0rho0(double N, double T, double zcorr, double &n0, double &rho0);
double IoffeGamma(double N, double T, double zcorr);
double Gauss2D(double x, double y, double a[]);
double Fermi2D(double x, double y, double a[]);
void FitGauss2D(double ax, double a[], double *y, double dyda[], int na);
double PartialFermiFunc(double x, double a[]);
double FermiZeroTempFunc(double x, double a[]);
double GaussFunc(double x, double a[]);
void SetFitFunc1D(double (*func)(double, double []));
void GeneralFitFunc1D(double x, double a[], double *y, double dyda[], int na);

#endif
