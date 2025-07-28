#include "densprof.h"
#include <math.h>
#include "const.h"
#define INTEGRATION_ENERGY 500
#define INTEGRATION_P 100
#define TRANCATION 200
double fax,frad,sigma,masse,oldN,oldT,chp,tof,x0,off,slo;
bool FitHorizontal;
bool FitFermi;

void SetFermiFuncParams(double afax,double afrad, double asigma, double amasse,
  double atof, double ax0, double aoff, double aslo,bool aFitFermi, bool aFitHorizontal) {
  fax=afax;
  frad=afrad;
  masse=amasse;
  sigma=asigma;
  oldN=0;
  oldT=0;
  tof=atof;
  x0=ax0;
  off=aoff;
  slo=aslo;
  FitFermi=aFitFermi;
  FitHorizontal=aFitHorizontal;
}

//////////////////////////////////////////////////////////////
double IntegrateEnergyDistribution(double a, double b, double chp, double betha)
{
//a and b are limits of integration
   long double func;
   double lv_x;
   double step = (b-a)/INTEGRATION_ENERGY;
   int stat; //statistic: Fermi=1, Bose=-1

   FitFermi ? stat=1 : stat=-1;

   func = 1./2*(a*a)/(expl(betha*(a-chp))+stat*1);
   lv_x = a;
   for (int i=0; i<(INTEGRATION_ENERGY-2); i++)
   {  lv_x = lv_x + step;
      func = func + (lv_x*lv_x)/(expl(betha*(lv_x-chp))+stat*1);
   }
   func = func + 1./2*(b*b)/(expl(betha*(b-chp))+stat*1);

   return double(step*func);
}

//////////////////////////////////////////////////////////////
double CalculateChemicalPotential(double Natoms, double ef, double betha, double lambda, double frad)
{
  int flag = 1;
  double chp; //starting point for iteration
  double ln;     //local number of atoms to iterate chemical potential
  double a;  //low limit for iteration
  double b;  //high limit for iteration
  double Tc;
//Intagration over energy
  if ( 1./(_kB*betha) < 10E-9)
    return ef;

  if (FitFermi)    //initial parameters for Bose distribution
  { chp = 0.5*ef; a = -10*ef; b = ef; }
  else
  { chp = -0.5*ef; a = -20*ef; b = 0;
    Tc = _h/_kB*frad*pow(lambda*Natoms/1.202,1./3);  //critical temperature
    if ( 1./(_kB*betha) < 1.01*Tc )
      return 0;
  }

  while ( flag != 0 )
  { //prefactor is from density of state expression
    ln = 1/(2*lambda*pow(frad*_h,3))*IntegrateEnergyDistribution(0,10*ef,chp,betha);
    if( ln > Natoms )
    { b = chp; chp = (b + a)/2; }
    else
    { a = chp; chp = (b + a)/2; }

    if (fabs(Natoms-ln) < 0.001)
      flag = 0;
  }

  return chp;
}
//////////////////////////////////////////////////////////////
double IntegrateOverCoordinateSpace(double a, double b, double betha, double chp,
                                    double mass, double frad, double lambda, double pr, double pz)
{
   long double func;
   int i;
   double lv_x;
   double step = (b-a)/INTEGRATION_P;
   double pot = mass*pow(2*M_PI*frad,2)/2;      //coefficient for the potential energy
   double kin = 1/(2*mass);              //coefficient for the kinetic energy
   int stat;
//integration over momentum space is provided in spherical coordinates (spherical symmetry)
   FitFermi ? stat=1 : stat=-1;

   func = 1./2*(a*a)/(expl(betha*(kin*(pz*pz + pr*pr) + pot*a*a - chp))+stat*1);
   lv_x = a;
   for (i=0; i<(INTEGRATION_P-2); i++)
   {  lv_x = lv_x + step;
      func += (lv_x*lv_x)/(expl(betha*(kin*(pz*pz + pr*pr) + pot*lv_x*lv_x - chp))+stat*1);
   }
   func = func + 1./2*(b*b)/(expl(betha*(kin*(pz*pz + pr*pr) + pot*b*b - chp))+stat*1);

   return double(2*M_PI*2*step*(1./lambda)*func);
}
//////////////////////////////////////////////////////////////
double IntegrateOverMomentumSpace(double a, double b, double betha, double chp,
                                    double mass, double frad, double lambda, double r, double z)
{
   long double func;
   int i;
   double lv_v;
   double step = (b-a)/INTEGRATION_P;
   double pot = mass*pow(2*M_PI*frad,2)/2;      //coefficient for the potential energy
   double kin = 1/(2*mass);              //coefficient for the kinetic energy
   int stat;    //statistic: Fermi=1, Bose=-1
//integration over momentum space is provided in spherical coordinates (spherical symmetry)
   FitFermi ? stat=1 : stat=-1;

   func = 1./2*(a*a)/(expl(betha*(kin*(a*a) + pot*(r*r + lambda*lambda*z*z) - chp))+stat*1);
   lv_v = a;
   for (i=0; i<(INTEGRATION_P-2); i++)
   {  lv_v = lv_v + step;
      func += (lv_v*lv_v)/(expl(betha*(kin*(lv_v*lv_v) + pot*(r*r + lambda*lambda*z*z) - chp))+stat*1);
   }
   func = func + 1./2*(b*b)/(expl(betha*(kin*(b*b) + pot*(r*r + lambda*lambda*z*z) - chp))+stat*1);

   return double(2*M_PI*2*step*func);
}
//////////////////////////////////////////////////////////////
double BoseDistributionWithPolyLog(double betha, double chp, double mass, double fax, double frad, double z)
{  //Calculation of the PolyLog function 5/2 by summation of series up to trancation parameter
   long double dens = 0;
   long double lhelp = expl(betha*chp)*expl(-betha*mass*pow(2*M_PI*fax,2)/2*z*z);

   for(int i=1; i<TRANCATION+1; i++)
     dens += powl(lhelp,i)/pow(i,5./2);

   return double(dens);
}
//////////////////////////////////////////////////////////////
double FermiDistributionWithPolinom(double chp, double betha, double mass, double fax, double frad, double z)
{
    long double dens, x;

    x = (-1)*expl(betha*(chp - 0.5*mass*pow(2*M_PI*fax,2)*z*z));

    //for chemical_potential/kT = ln(500) = 3.9 (T=0.25Tfermi)
    dens = -0.16157605312601253 + 0.7346169398655529*x +
            0.03282896082321644*x*x + 0.0010658215136565307*x*x*x +
            0.000017992516617156413*x*x*x*x + 1.1887189849372155E-7*x*x*x*x*x;

   return double((-1)*dens);
}
//////////////////////////////////////////////////////////////
double FermiFunc(double x, double a[]) {
  double Nratio=a[3];
  double N=Nratio*a[1];       //a[1] contains number from Nint
  //double x0=a[2];
  double T=1E-6*a[2];       //in Kelvin

  if (T < 1E-9) T = 1E-9;
  //double off=a[4];
  //double slo=a[5];
  //fax = 39;
  //frad = 433;

  double dens = 0, pr = 0, pr_step, px;
  double r = 0, r_step, z;
  double lambda=fax/frad;
  double ef = _h*frad*pow(6*N*lambda,1./3);   //Fermi energy
  double rf = pow(2*ef/(pow(2*M_PI*frad,2)*masse),1./2);
  double pf = pow(2*masse*ef,1./2);
  double betha = 1./(_kB*T);

  if ((oldN!=N) || (oldT!=T)) {
    oldN=N;
    oldT=T;
    chp = CalculateChemicalPotential(N, ef, betha, lambda, frad);
  }

  if (!FitHorizontal)
  { z = 1E-3*(x-x0);   //in units of m
    if (FitFermi)
    { r_step = 4*rf/INTEGRATION_P;
      for(int i=0; i<INTEGRATION_P; i++)
      { dens += 2*M_PI*r*IntegrateOverMomentumSpace(0,8*pf,betha,chp,masse,frad,lambda,r,z);
        r += r_step;
      }
      dens = 1./pow(_h,3)*(sigma*dens*r_step)*1E3; //back to the m
    }
    //  dens = 1./pow(_h,3)*pow(2*M_PI/betha,5./2)*pow(masse,1./2)/pow(2*M_PI*frad,2)*
    //         FermiDistributionWithPolinom(chp,betha,masse,fax,frad,z)*sigma*1E3;
    else
      dens = 1./pow(_h,3)*pow(2*M_PI/betha,5./2)*pow(masse,1./2)/pow(2*M_PI*frad,2)*
             BoseDistributionWithPolyLog(betha,chp,masse,fax,frad,z)*sigma*1E3;
  }
  else
  { pr_step = 4*pf/INTEGRATION_P;
    px = masse*1E-3*(x-x0)/(1E-3*tof);   //in units of m/sec
    for(int i=0; i<INTEGRATION_P; i++)
    { dens += 2*M_PI*pr*IntegrateOverCoordinateSpace(0,10*rf,betha,chp,masse,frad,lambda,pr,px);
      pr += pr_step;
    }
    dens = 1./pow(_h,3)*(sigma*dens*pr_step)*(masse/(1E-3*tof))*1E3; //back to the mm
  }

  dens =dens/Nratio + slo*x + off;

  return dens;
}