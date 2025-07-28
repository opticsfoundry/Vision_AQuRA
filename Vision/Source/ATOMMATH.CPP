#include "atommath.h"
#include "const.h"
//#include <cmath>
#include <math.h>
#include "setup.h"

double Sigma(double Detuning, double Gamma, double Lambda, double Clebsch) {
  double h=Detuning/((Gamma)/4/M_PI);
  return (Clebsch)*3*(Lambda)*(Lambda)/(2*M_PI*(1+h*h));
}

void temperature(double x, double a[], double *y, double dyda[], int na)
{
	*y=sqrt(a[1]*a[1]+a[2]*a[2]*x*x);
	dyda[1]=a[1]/(*y);
	dyda[2]=x*x*a[2]/(*y);
}

void number(double x, double a[], double *y, double dyda[], int na, double Clebsch, double Lambda, double Gamma)
{
  double h=(x+a[2])/((Gamma)/4/M_PI);
  double h2=(Clebsch)*(Lambda)*(Lambda)/4/M_PI/M_PI;
  double h3=1+h*h;
  if (abs(h3)<1E-10) h3=1E-10;
  dyda[1]=h2/h3;
  *y=a[1]*dyda[1];
  double h4=x+a[2];
  if (abs(h4)<1E-10) h4=1E-10;
  dyda[2]=(-2)*(*y)/h3*h*h/h4;
}

void ChargingTime(double x, double a[], double *y, double dyda[], int na)
{
	double h2;
	if (abs(a[2])<1E-10) h2=1; else h2=a[2];
	double h=exp(-x/h2);
	*y=a[1]*(1-h);
	dyda[1]=1-h;
	dyda[2]=-a[1]*h*x/(h2*h2);
}

void Lifetime(double x, double a[], double *y, double dyda[], int na)
{
	double h2;
	if (abs(a[2])<1E-5) h2=1; else h2=a[2];
	double h=exp(-x/h2);
	*y=a[1]*h;
	dyda[1]=h;
	dyda[2]=(*y)*x/(h2*h2);
}

double Number(double SigmaX, double SigmaY, double DensOpt, double Detuning, double Gamma, double Lambda, double Clebsch) {
  return 2*M_PI*SigmaX*SigmaY*DensOpt/Sigma(Detuning, Gamma, Lambda, Clebsch);
}



