#include "fitmath.h"
#include "const.h"
#include <stdio.h>
#include <window.h>
#define NRANSI
#include <math.h>
#include <stdlib.h >
#define FREE_ARG char*
#define NR_END 1
#include "leakdetect.h"

bool matherror=false;
bool TrapMathError=false;
bool MathErrorMessage=false;

void SetMathErrorMessage( bool b) {
  MathErrorMessage=b;
}

int _matherr (struct exception *a)
{
  if (!TrapMathError) return 0;
  matherror=true;
  if (MathErrorMessage) return 0; else return 1;
}

int _matherrl(struct exceptionl *a)
{
  if (!TrapMathError) return 0;
  matherror=true;
  if (MathErrorMessage) return 0; else return 1;
}

void nrerror(char error_text[])
{
  if (MathErrorMessage) {
    char buf[300];
    sprintf(buf,"Numerical Recipes run-time error...\n%s\n",error_text);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
  }
  matherror=true;
}

double **matrix(long nrl, long nrh, long ncl, long nch,long Info)
{
// allocate a double matrix with subscript range m[0..nrh][0..nch]
	double **m;
   m=NULL;
   try {
   	m=new double*[nrh+1];
      #if defined(DetectLeaks)
        LeakDetect.New(m,11,1,Info);
      #endif
		for (int i=0;i<nrh+1;i++) {
        m[i]=new double[nch+1];
        #if defined(DetectLeaks)
          LeakDetect.New(m[i],11,2,Info);
        #endif
      }
   }
   catch (xalloc) {
  		MessageBox(GetActiveWindow(),"fit.h matrix : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
   }
   return m;
// allocate a double matrix with subscript range m[nrl..nrh][ncl..nch]
/*	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;

	// allocate pointers to rows
	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	// allocate rows and set pointers to them
	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	// return pointer to array of pointers to rows
	return m;*/
}

void free_matrix(double **m, long nrl, long nrh, long ncl, long nch)
/* free a double matrix allocated by matrix() */
{
	for (int i=0;i<nrh+1;i++) {
     #if defined(DetectLeaks)
       LeakDetect.Delete(m[i]);
     #endif
     delete[] m[i];
   }
   #if defined(DetectLeaks)
     LeakDetect.Delete(m);
   #endif
	delete[] m;
/*	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));*/
}

double *vector(long nl, long nh,long Info)
/* allocate a double vector with subscript range v[nl..nh] */
{
   double *v;
   v=NULL;
   try {
   	v=new double[nh+1];
   }
    catch (xalloc) {
  		MessageBox(GetActiveWindow(),"fit.h matrix : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
   }
   #if defined(DetectLeaks)
      LeakDetect.New(v,11,3,Info);
   #endif
   return v;

/*	double *v;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
   */
}

void free_vector(double *v, long nl, long nh)
/* free a double vector allocated with vector() */
{
   #if defined(DetectLeaks)
      LeakDetect.Delete(v);
   #endif
	delete[] v;
	//free((FREE_ARG) (v+nl-NR_END));
}

int *ivector(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
	int *v;
   v=NULL;
   try {
   	v=new int[nh+1];
   }
    catch (xalloc) {
  		MessageBox(GetActiveWindow(),"fit.h matrix : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
   }
   #if defined(DetectLeaks)
      LeakDetect.New(v,11,4);
   #endif
   return v;
	/*int *v;

	v=(int *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(int)));
	if (!v) nrerror("allocation failure in ivector()");
	return v-nl+NR_END;
   */
}

void free_ivector(int *v, long nl, long nh)
/* free an int vector allocated with ivector() */
{
	//free((FREE_ARG) (v+nl-NR_END));
   #if defined(DetectLeaks)
      LeakDetect.Delete(v);
   #endif
   delete[] v;
}

#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

#define CON 1.4
#define CON2 (CON*CON)
#define BIG 1.0e30
#define NTAB 10
#define SAFE 2.0
static float maxarg1,maxarg2;
#define FMAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
        (maxarg1) : (maxarg2))

double dfridr(double (*func)(double), double x, double h, double *err)
{
	int i,j;
	double errt,fac,hh,**a,ans;
   a=NULL;
   ans=0;
	if (h == 0.0) nrerror("h must be nonzero in dfridr.");
	a=matrix(1,NTAB,1,NTAB,0);
	hh=h;
	a[1][1]=((*func)(x+hh)-(*func)(x-hh))/(2.0*hh);
   if (matherror) {
     free_matrix(a,1,NTAB,1,NTAB);
     return ans;
   }
	*err=BIG;
	for (i=2;i<=NTAB;i++) {
		hh /= CON;
		a[1][i]=((*func)(x+hh)-(*func)(x-hh))/(2.0*hh);
      if (matherror) {
        free_matrix(a,1,NTAB,1,NTAB);
        return ans;
      }
		fac=CON2;
		for (j=2;j<=i;j++) {
			a[j][i]=(a[j-1][i]*fac-a[j-1][i-1])/(fac-1.0);
			fac=CON2*fac;
			errt=FMAX(fabs(a[j][i]-a[j-1][i]),fabs(a[j][i]-a[j-1][i-1]));
			if (errt <= *err) {
				*err=errt;
				ans=a[j][i];
			}
		}
		if (fabs(a[i][i]-a[i-1][i-1]) >= SAFE*(*err)) break;
	}
	free_matrix(a,1,NTAB,1,NTAB);
	return ans;
}
#undef CON
#undef CON2
#undef BIG
#undef NTAB
#undef SAFE
#undef NRANSI

double bessi1(double x)
{
	double ax,ans;
	double y;

	if ((ax=fabs(x)) < 3.75) {
		y=x/3.75;
		y*=y;
		ans=ax*(0.5+y*(0.87890594+y*(0.51498869+y*(0.15084934
			+y*(0.2658733e-1+y*(0.301532e-2+y*0.32411e-3))))));
	} else {
		y=3.75/ax;
		ans=0.2282967e-1+y*(-0.2895312e-1+y*(0.1787654e-1
			-y*0.420059e-2));
		ans=0.39894228+y*(-0.3988024e-1+y*(-0.362018e-2
			+y*(0.163801e-2+y*(-0.1031555e-1+y*ans))));
		ans *= (exp(ax)/sqrt(fabs(ax)));
	}
	return x < 0.0 ? -ans : ans;
}

double bessk1(double x)
{
	double y,ans;

	if (x <= 2.0) {
		y=x*x/4.0;
		ans=(log(fabs(x/2.0))*bessi1(x))+(1.0/x)*(1.0+y*(0.15443144
			+y*(-0.67278579+y*(-0.18156897+y*(-0.1919402e-1
			+y*(-0.110404e-2+y*(-0.4686e-4)))))));
	} else {
		y=2.0/x;
		ans=(exp(-x)/sqrt(x))*(1.25331414+y*(0.23498619
			+y*(-0.3655620e-1+y*(0.1504268e-1+y*(-0.780353e-2
			+y*(0.325614e-2+y*(-0.68245e-3)))))));
	}
	return ans;
}

double mfgfmyb=0;
double sx=0;
double sy=0;
double dx=0;
double dy=0;
int nx=0;
int ny=0;
double U0=0;
double A=0;
double B=0;
double Aiq=0;
double Bias=0;
double Gradient=0;
double Curvature=0;
double Masse=0;
double aktSigma=0;
double SigmaEl=0;
double (*FitFunc)(double, double, double []);

void SetIoffeTrapParameters(double aBias, double aGradient, double aCurvature,
  double amfgfmyb,double asigma, double aMasse, double aSigmaEl) {
  Masse=aMasse;
  aktSigma=asigma;
  SigmaEl=aSigmaEl;
  Bias=aBias*1E-4;           //conversion G      -> T
  Gradient=aGradient*1E-2;   //           G/cm   -> T/m
  Curvature=aCurvature;      //           G/cm^2 =  T/m^2
  mfgfmyb=amfgfmyb;
  U0=amfgfmyb*Bias;
  B=amfgfmyb*Curvature*0.5;
  A=amfgfmyb*sqrt(Gradient*Gradient-0.5*Bias*Curvature);
  Aiq=pow(2.0*Masse*M_PI*M_PI,1.5)/(_h*_h*_h*2.0*A*A*sqrt(B));
}

void Set2DFitParameters(double asx, double asy, double adx, double ady,
  int anx, int any, double (*func)(double, double, double [])) {
  sx=asx;
  sy=asy;
  dx=adx;
  dy=ady;
  nx=anx;
  ny=any;
  FitFunc=func;
}

double IoffeOptDens(double x, double y, double a[]) {
  if (a[9]<0.01) a[9]=0.01;
  B=mfgfmyb*Curvature*a[9]*0.5;
  A=mfgfmyb*sqrt(Gradient*Gradient-0.5*Bias*Curvature*a[9]);
  Aiq=pow(2.0*Masse*M_PI*M_PI,1.5)/(_h*_h*_h*2.0*A*A*sqrt(B));

  double cosphi=cos(a[8]);
  double sinphi=sin(a[8]);
  double x2=x-a[1];
  double y2=y-a[2];
  x= cosphi*x2+sinphi*y2;
  y=-sinphi*x2+cosphi*y2;
  double h1=U0+B*x*x;
  double H=sqrt(A*A*y*y+h1*h1);
  if (a[3]<1E-12) {
    matherror=true;
    a[3]=1E-3;
  }
  double kBT=(_kB)*a[3];
  double Beta=1/kBT;
  double help=(Aiq*(6*pow(kBT,4)+4*U0*pow(kBT,3)));
  if (help<=0) {
    matherror=true;
    help=1E-30;
  }
  double rho=a[4]/help;
  double lambda=_h/(sqrt(2*M_PI*Masse*kBT));
  double n0=rho/pow(lambda,3);
  return 2*aktSigma*n0/A*exp(Beta*U0)*H*bessk1(Beta*H)+a[5]+a[6]*x+a[7]*y;
}

void Ioffen0rho0(double N, double T, double zcorr, double &n0, double &rho0) {
  if (zcorr<0.01) zcorr=0.01;
  B=mfgfmyb*Curvature*zcorr*0.5;
  A=mfgfmyb*sqrt(Gradient*Gradient-0.5*Bias*Curvature*zcorr);
  Aiq=pow(2.0*Masse*M_PI*M_PI,1.5)/(_h*_h*_h*2.0*A*A*sqrt(B));
  if (T<=1E-9) {
    matherror=true;
    T=1E-9;
  }
  double kBT=(_kB)*T;
  double Beta=1/kBT;
  double help=(Aiq*(6*pow(kBT,4)+4*U0*pow(kBT,3)));
  if (help<1E-10) {
    matherror=true;
    help=1E-10;
  }
  if (help>1) {
    matherror=true;
    help=1;
  }
  rho0=N/help;
  double lambda=_h/(sqrt(2*M_PI*Masse*kBT));
  n0=rho0/pow(lambda,3);
}

double IoffeGamma(double N, double T,double zcorr) {
  if (zcorr<0.01) zcorr=0.01;
  B=mfgfmyb*Curvature*zcorr*0.5;
  A=mfgfmyb*sqrt(Gradient*Gradient-0.5*Bias*Curvature*zcorr);
  Aiq=pow(2.0*Masse*M_PI*M_PI,1.5)/(_h*_h*_h*2.0*A*A*sqrt(B));
  double kBT=(_kB)*T;

  double n0T,n0T_2,dummy;
  Ioffen0rho0(N,T,zcorr,n0T,dummy);
  Ioffen0rho0(N,T*0.5,zcorr,n0T_2,dummy);
  double v_=sqrt(16*kBT/(M_PI*Masse));
 if (n0T_2==0) n0T_2=1;
  return v_*SigmaEl*n0T*n0T/n0T_2;
}

double SumX(double x1, double x2, double dx, double y, double a[],
  double (*funcs)(double, double ,double [])) {
  double h=0;
  if (x2<x1) { double swap=x2;x2=x1; x1=swap; }
  double x=x1;
  int n=floor((x2-x1)/dx)+1;
  for (int i=0;i<n;i++) {
    h+=funcs(x,y,a);
    x+=dx;
  }
  return h/n;
}

double SumY(double x,double y1, double y2, double dy, double a[],
  double (*funcs)(double, double, double [])) {
  double h=0;
  if (y2<y1) { double swap=y2;y2=y1; y1=swap; }
  double y=y1;
  int n=floor((y2-y1)/dy)+1;
  for (int i=0;i<n;i++) {
    h+=funcs(x,y,a);
    y+=dy;
  }
  return h/n;
}

double aktParams[10];
double aktX;
double aktY;

double DOptDensX0(double X0) {
  aktParams[1]=X0;
  return FitFunc(aktX,aktY,aktParams);
}

double DOptDensY0(double Y0) {
  aktParams[2]=Y0;
  return FitFunc(aktX,aktY,aktParams);
}

double DOptDensT(double T) {
  aktParams[3]=T;
  return FitFunc(aktX,aktY,aktParams);
}

double DOptDensn(double n) {
  aktParams[4]=n;
  return FitFunc(aktX,aktY,aktParams);
}

double DOptDensphi(double phi) {
  aktParams[8]=phi;
  return FitFunc(aktX,aktY,aktParams);
}

double DOptDensFlight(double Tf) {
  aktParams[9]=Tf;
  return FitFunc(aktX,aktY,aktParams);
}

void FitIoffeOptDens(double ax, double a[], double *y, double dyda[], int na) {
  aktX=floor(ax/ny)*dx+sx;
  aktY=fmod(ax,ny)*dy+sy;
  *y=FitFunc(aktX,aktY,a);
  double err;
  if (matherror) return;
  for (int i=1;i<=na;i++) aktParams[i]=a[i];
  dyda[1]=dfridr(DOptDensX0,a[1],1E-4,&err);
  if (matherror) return;
  for (int i=1;i<=na;i++) aktParams[i]=a[i];
  dyda[2]=dfridr(DOptDensY0,a[2],1E-4,&err);
  if (matherror) return;
  for (int i=1;i<=na;i++) aktParams[i]=a[i];
  dyda[3]=dfridr(DOptDensT,a[3],a[3]*0.1,&err);
  if (matherror) return;
  //for (int i=1;i<na;i++) aktParams[i]=a[i];
  dyda[4]=(*y)/a[4];
  //dyda[4]=dfridr(DOptDensn,a[4],a[4]*0.1,&err);
  dyda[5]=1;
  dyda[6]=aktX;
  dyda[7]=aktY;
  for (int i=1;i<=na;i++) aktParams[i]=a[i];
  dyda[8]=dfridr(DOptDensphi,a[8],a[8]*0.01+0.001,&err);
  if (matherror) return;
  for (int i=1;i<=na;i++) aktParams[i]=a[i];
  dyda[9]=dfridr(DOptDensFlight,a[9],a[9]*0.01+0.001,&err);
}

double Gauss2D(double x, double y, double a[]) {
  double fac1,fac2,ex,arg1,arg2;
  if (a[3]==0) a[3]=0.000000001;
  if (a[4]==0) a[4]=0.000000001;
  arg1=(x-a[1])/a[3];
  arg2=(y-a[2])/a[4];
  ex=exp(-0.5*(arg1*arg1+arg2*arg2));
  return  a[8]*ex+a[5]+a[6]*x+a[7]*y;
}

double Fermi2D(double x, double y, double a[]) {
  double fac1,fac2,ex,arg1,arg2;
  if (a[3]==0) a[3]=0.000000001;
  if (a[4]==0) a[4]=0.000000001;
  arg1=(x-a[1])/a[3];
  arg2=(y-a[2])/a[4];
  ex=(1-arg1*arg1-arg2*arg2);
  if (ex<0) ex=0;
  ex*=ex;
  return  a[8]*ex+a[5]+a[6]*x+a[7]*y;
}

void FitGauss2D(double ax, double a[], double *y, double dyda[], int na) {
  double px=floor(ax/ny)*dx+sx;
  double py=fmod(ax,ny)*dy+sy;
  double fac1,fac2,ex,arg1,arg2;
  if (a[3]==0) a[3]=0.000000001;
  if (a[4]==0) a[4]=0.000000001;
  arg1=(px-a[1])/a[3];
  arg2=(py-a[2])/a[4];
  ex=exp(-0.5*(arg1*arg1+arg2*arg2));
  *y = a[8]*ex+a[5]+a[6]*px+a[7]*py;
  fac1=a[8]*ex*arg1*2.0;
  fac2=a[8]*ex*arg1*2.0;
  dyda[1]=fac1/a[3];
  dyda[2]=fac2/a[4];
  dyda[3]=fac1*arg1*2.0/a[3];
  dyda[4]=fac2*arg2*2.0/a[4];
  dyda[5]=1;
  dyda[6]=px;
  dyda[7]=py;
  dyda[8]=ex;
}

double FermiZeroTempFunc(double x, double a[]) {
  double A=a[1];
  double x0=a[2];
  double sig=a[3];
  double off=a[4];
  double slo=a[5];
  double L=atan(a[6])/M_PI+0.5;
  if (sig<=0.000000001) sig=0.000000001;
  double rho=(x-x0)/sig;
  double help=rho*rho;
  return (help<1) ? A*pow(1-help,2.5)+off+slo*x : off+slo*x;
}

double PartialFermiFunc(double x, double a[]) {
  double A=a[1];
  double x0=a[2];
  double sig=a[3];
  double off=a[4];
  double slo=a[5];
  double L=atan(a[6])/M_PI+0.5;
  if (sig<=0.000000001) sig=0.000000001;
  double rho=(x-x0)/sig;

  if (rho*rho<=5*(1-L)/L) {
    double help=rho*rho*L/5.0;
    return (help<1) ? A*pow(1-help,2.5)/(pow(L,2.5)*exp(2.5*(1-L)/L))+off+slo*x : off+slo*x;
  } else {
  	 return A*exp(-0.5*rho*rho)+off+slo*x;
  }
}

double GaussFunc(double x, double a[]) {
  double A=a[1];
  double x0=a[2];
  double sig=a[3];
  double off=a[4];
  double slo=a[5];
  if (sig<=0.000000001) sig=0.000000001;
  double rho=(x-x0)/sig;
  if (fabs(rho)>10) rho=10;
  return A*exp(-0.5*rho*rho)+off+slo*x;
}

double (*FitFunc1D)(double, double []);
void SetFitFunc1D(double (*func)(double, double [])) {
  FitFunc1D=func;
}

int aktDeriveParam;
//double SetAktDeriveParam(int i) { aktDeriveParam=i;}
double DFitFunc1D(double P) {
  aktParams[aktDeriveParam]=P;
  return FitFunc1D(aktX,aktParams);
}

void GeneralFitFunc1D(double x, double a[], double *y, double dyda[], int na)
{
  aktX=x;
  *y=FitFunc1D(x,a);
  if (matherror) return;
  for (int j=1;j<=na;j++) {
    double err;
    for (int i=1;i<=na;i++) aktParams[i]=a[i];
//    SetAktDeriveParam(j);
    aktDeriveParam=j;
    dyda[j]=dfridr(DFitFunc1D,a[j],1E-4,&err);
    if (matherror) return;
  }
}

void gauss(double x, double a[], double *y, double dyda[], int na)
{
	int i;
	double fac,ex,arg;
	*y=0.0;
	for (i=1;i<=na-1;i+=4) {
		if (a[i+2]==0) a[i+2]=0.000000001;
		arg=(x-a[i+1])/a[i+2];
		ex=exp(-0.5*arg*arg);
		fac=a[i]*ex*arg*2.0;
		*y += a[i]*ex+a[i+3]+a[i+4]*x;
		dyda[i]=ex;
		dyda[i+1]=fac/a[i+2];
		dyda[i+2]=fac*arg*2.0/a[i+2];
		dyda[i+3]=1;
      dyda[i+4]=x;
	}
}

void gaussWithoutOffset(double x, double a[], double *y, double dyda[], int na)
{
	int i;
	double fac,ex,arg;
	*y=0.0;
	for (i=1;i<=na-1;i+=4) {
		if (a[i+2]==0) a[i+2]=0.000000001;
		arg=(x-a[i+1])/a[i+2];
		ex=exp(-0.5*arg*arg);
		fac=a[i]*ex*arg*2.0;
		*y += a[i]*ex;
		dyda[i]=ex;
		dyda[i+1]=fac/a[i+2];
		dyda[i+2]=fac*arg*2.0/a[i+2];
		dyda[i+3]=0;
	}
}

void exponential(double x, double a[], double *y, double dyda[], int na)
{
	int i;
	double ex,arg;
	*y=0.0;
	for (i=1;i<=na-1;i+=4) {
		if (a[i+2]==0) a[i+2]=0.000000001;
		arg=fabs(x-a[i+1])/a[i+2];
		ex=exp(-arg);
		*y += a[i]*ex;//+a[i+3]+a[i+4]*x;
		dyda[i]=ex;
		dyda[i+1]=a[i]*ex/a[i+2];
		dyda[i+2]=-a[i]*arg*ex;
		dyda[i+3]=0;//1;
      dyda[i+4]=0;//1;
	}
}

void linear(double x, double a[], double *y, double dyda[], int na)
{
	*y=a[1];
	dyda[1]=1;
}

void gaussj(double **a, int n, double **b, int m)
{
	int *indxc,*indxr,*ipiv;
   indxc=NULL;
   indxr=NULL;
   ipiv=NULL;
	int i,icol,irow,j,k,l,ll;
	double big,dum,pivinv,temp;

	indxc=ivector(1,n);
	indxr=ivector(1,n);
	ipiv=ivector(1,n);
	for (j=1;j<=n;j++) ipiv[j]=0;
	for (i=1;i<=n;i++) {
		big=0.0;
		for (j=1;j<=n;j++)
			if (ipiv[j] != 1)
				for (k=1;k<=n;k++) {
					if (ipiv[k] == 0) {
						if (fabs(a[j][k]) >= big) {
							big=fabs(a[j][k]);
							irow=j;
							icol=k;
						}
					} else if (ipiv[k] > 1) nrerror("gaussj: Singular Matrix-1");
				}
		++(ipiv[icol]);
		if (irow != icol) {
			for (l=1;l<=n;l++) SWAP(a[irow][l],a[icol][l])
			for (l=1;l<=m;l++) SWAP(b[irow][l],b[icol][l])
		}
		indxr[i]=irow;
		indxc[i]=icol;
		//if (a[icol][icol] ==0) ;//nrerror("gaussj: Singular Matrix-2");
		if (fabs(a[icol][icol])>1E-10) pivinv=1.0/a[icol][icol]; else pivinv=1E10;
		a[icol][icol]=1.0;
      //floating point error here 2x  17Mar06
      if (pivinv>1E10) pivinv=1E10;
      if (pivinv<-1E10) pivinv=-1E10;
      //error occured here  4 times
		for (l=1;l<=n;l++) a[icol][l] *= pivinv;
		for (l=1;l<=m;l++) b[icol][l] *= pivinv;
		for (ll=1;ll<=n;ll++)
			if (ll != icol) {
				dum=a[ll][icol];
				a[ll][icol]=0.0;
				for (l=1;l<=n;l++) a[ll][l] -= a[icol][l]*dum;
				for (l=1;l<=m;l++) b[ll][l] -= b[icol][l]*dum;
			}
	}
	for (l=n;l>=1;l--) {
		if (indxr[l] != indxc[l])
			for (k=1;k<=n;k++)
				SWAP(a[k][indxr[l]],a[k][indxc[l]]);
	}
	free_ivector(ipiv,1,n);
	free_ivector(indxr,1,n);
	free_ivector(indxc,1,n);
}
#undef SWAP

#define SWAP(a,b) {swap=(a);(a)=(b);(b)=swap;}

void covsrt(double **covar, int ma, int ia[], int mfit)
{
	int i,j,k;
	double swap;

	for (i=mfit+1;i<=ma;i++)
		for (j=1;j<=i;j++) covar[i][j]=covar[j][i]=0.0;
	k=mfit;
	for (j=ma;j>=1;j--) {
		if (ia[j]) {
			for (i=1;i<=ma;i++) SWAP(covar[i][k],covar[i][j])
			for (i=1;i<=ma;i++) SWAP(covar[k][i],covar[j][i])
			k--;
		}
	}
}
#undef SWAP


void mrqcof(double x[], double y[], double sig[], int ndata, double a[], int ia[],
	int ma, double **alpha, double beta[], double *chisq,
	void (*funcs)(double, double [], double *, double [], int))
{
	int i,j,k,l,m,mfit=0;
	double ymod,wt,sig2i,dy,*dyda;

	dyda=vector(1,ma,0);
	for (j=1;j<=ma;j++)
		if (ia[j]) mfit++;
	for (j=1;j<=mfit;j++) {
		for (k=1;k<=j;k++) alpha[j][k]=0.0;
		beta[j]=0.0;
	}
	*chisq=0.0;
	for (i=1;i<=ndata;i++) {
		if (!matherror) (*funcs)(x[i],a,&ymod,dyda,ma);
      if (matherror) {
        free_vector(dyda,1,ma);
        return;
      }
		if (sig[i]!=0) sig2i=1.0/(sig[i]*sig[i]); else sig2i=1E10;
		dy=y[i]-ymod;
		for (j=0,l=1;l<=ma;l++) {
			if (ia[l]) {
				wt=dyda[l]*sig2i;
				for (j++,k=0,m=1;m<=l;m++)
					if (ia[m]) alpha[j][++k] += wt*dyda[m];
				beta[j] += dy*wt;
			}
		}
		*chisq += dy*dy*sig2i;
	}
	for (j=2;j<=mfit;j++)
		for (k=1;k<j;k++) alpha[k][j]=alpha[j][k];
	free_vector(dyda,1,ma);
}

double ochisq,*atry,*beta,*da,**oneda;
int mfit;

void Prepare_mrqmin(double x[], double y[], double sig[], int ndata, double a[], int ia[],
	int ma, double **covar, double **alpha, double *chisq,
	void (*funcs)(double, double [], double *, double [], int), double *alamda) {
   int j;
	atry=vector(1,ma,1);
	beta=vector(1,ma,2);
	da=vector(1,ma,3);
	for (mfit=0,j=1;j<=ma;j++)
		if (ia[j]) mfit++;
	oneda=matrix(1,mfit,1,1,1);
	*alamda=0.001;
	mrqcof(x,y,sig,ndata,a,ia,ma,alpha,beta,chisq,funcs);
	ochisq=(*chisq);
	for (j=1;j<=ma;j++) atry[j]=a[j];
}

void End_mrqmin(int ma) {
   if (da!=NULL) free_vector(da,1,ma);
   da=NULL;
   if (beta) free_vector(beta,1,ma);
   beta=NULL;
   if (atry) free_vector(atry,1,ma);
   atry=NULL;
 	if (oneda) free_matrix(oneda,1,mfit,1,1);
   oneda=NULL;
}

void mrqmin(double x[], double y[], double sig[], int ndata, double a[], int ia[],
	int ma, double **covar, double **alpha, double *chisq,
	void (*funcs)(double, double [], double *, double [], int), double *alamda)
{
	void covsrt(double **covar, int ma, int ia[], int mfit);
	void gaussj(double **a, int n, double **b, int m);
	void mrqcof(double x[], double y[], double sig[], int ndata, double a[],
		int ia[], int ma, double **alpha, double beta[], double *chisq,
		void (*funcs)(double, double [], double *, double [], int));
	int j,k,l,m;

	for (j=0,l=1;l<=ma;l++) {
		if (ia[l]) {
			for (j++,k=0,m=1;m<=ma;m++) {
				if (ia[m]) {
					k++;
					covar[j][k]=alpha[j][k];
				}
			}
			covar[j][j]=alpha[j][j]*(1.0+(*alamda));
			oneda[j][1]=beta[j];
		}
	}
	if (!matherror) {
      gaussj(covar,mfit,oneda,1);
      for (j=1;j<=mfit;j++) da[j]=oneda[j][1];
      if (*alamda == 0.0) {
         if (!matherror) covsrt(covar,ma,ia,mfit);
//         End_mrqmin(ma);
         return;
      }
      for (j=0,l=1;l<=ma;l++)
         if (ia[l]) atry[l]=a[l]+da[++j];
      if (!matherror) mrqcof(x,y,sig,ndata,atry,ia,ma,covar,da,chisq,funcs);
      if (matherror) {
	     // End_mrqmin(ma);
         return;
      }
      if (*chisq < ochisq) {
         *alamda *= 0.1;
         ochisq=(*chisq);
         for (j=0,l=1;l<=ma;l++) {
            if (ia[l]) {
               for (j++,k=0,m=1;m<=ma;m++) {
                  if (ia[m]) {
                     k++;
                     alpha[j][k]=covar[j][k];
                  }
               }
               beta[j]=da[j];
               a[l]=atry[l];
            }
         }
      } else {
         *alamda *= 10.0;
         *chisq=ochisq;
      }
   }
}

bool fit(double x[], double y[], double sig[], int ndata, double a[], double v[],
  	int ma, double *chisq, int maxn, int minn, double minchange,  double minchisq,double maxchange, int &nend,
     double &changeend,void (*funcs)(double, double [], double *, double [], int)) {
	double ** covar=matrix(1,ma,1,ma,3);
	double **alpha=matrix(1,ma,1,ma,4);
	int *ia=ivector(1,ma);
	for (int i=1;i<=ma;i++) ia[i]=v[i-1];
	double *nx=vector(1,ndata,4);
	for (int i=1;i<=ndata;i++) nx[i]=x[i-1];
	double *ny=vector(1,ndata,5);
	for (int i=1;i<=ndata;i++) ny[i]=y[i-1];
	double *nsig=vector(1,ndata,6);
	for (int i=1;i<=ndata;i++) nsig[i]=sig[i-1];
	double *na=vector(1,ma,7);
	for (int i=1;i<=ma;i++) na[i]=a[i-1];
	double lamda=-1;
	double *alamda=&lamda;
	int n=0;
   matherror=false;
   TrapMathError=true;
   double *alta=NULL;
   try {
	   alta=new double[ma+1];
   }
   catch (xalloc) {
  		MessageBox(GetActiveWindow(),"fit : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
   }
   #if defined(DetectLeaks)
      LeakDetect.New(alta,11,5);
   #endif
   double change;
   for (int i=1;i<=ma;i++) alta[i]=na[i];
   int lastchange=0;
   Prepare_mrqmin(nx,ny,nsig,ndata,na,ia,ma,covar,alpha,chisq,funcs,alamda);
	do {
	  mrqmin(nx,ny,nsig,ndata,na,ia,ma,covar,alpha,chisq,funcs,alamda);
     change=0;
     if (!matherror) {
       for (int j=1;j<=ma;j++) {
         if (na[j]!=0) change+=((alta[j]-na[j])*(alta[j]-na[j])/(na[j]*na[j]));
         alta[j]=na[j];
       }
       if (change>0) change=sqrt(change)/ma;
       if (change==0) lastchange++; else lastchange=0;
     }
	  n++;
	} while ((n<maxn) && (*chisq>minchisq) && (!matherror) && (lastchange<=minn));
   changeend=change;
   nend=n;
   #if defined(DetectLeaks)
      LeakDetect.Delete(alta);
   #endif
   delete[] alta;
	lamda=0;
	if (!matherror) {
     mrqmin(nx,ny,nsig,ndata,na,ia,ma,covar,alpha,chisq,funcs,alamda);
     for (int i=1;i<=ma;i++) a[i-1]=na[i];
   }
   TrapMathError=false;
   End_mrqmin(ma);
	free_matrix(covar,1,ma,1,ma);
	free_matrix(alpha,1,ma,1,ma);
	free_ivector(ia,1,ma);
	free_vector(na,1,ma);
	free_vector(nx,1,ndata);
	free_vector(ny,1,ndata);
	free_vector(nsig,1,ndata);
   return (matherror);
}

#undef NRANSI

