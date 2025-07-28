//	program camera
#include <iostream.h>
#include <fstream.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#define FREE_ARG char*
#define NR_END 1


   int i,index,q,j,xi,xf,npixel,ipixel,nobs;
   const nparam=1,npixx=130,npixy=90;
   double tabx[npixx+1],y[3],dydx[3],param[nparam];
   double x,spix,pixx,pixy,t,dt,Texp;
   double cal1,cal2;
   double delta,Gamma,lambda,sigma,pi,tobs,Isat,g;
   double dcol,natint,nat,I_f,I_i,signe;
   bool fin;
   FILE *fileabs,*fileprobe,*fileI1,*fileI2,
        *filedcol,*filenat,*filenatintx,*filenatinty;



void nrerror(char error_text[])
/* Numerical Recipes standard error handler */
{
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}

double *vector(long nl, long nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
	double *v;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

void free_vector(double *v, long nl, long nh)
/* free a double vector allocated with vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void rk4(double y[], int n, double x, double h, double delta,
void (*derivs)(double, double [], double [], int n, double delta))
{
	int i;
	double xh,hh,h6,*dym,*dyt,*yt,*dydx;

   dydx=vector(1,n);
   (*derivs)(x,y,dydx,n,delta);
	dym=vector(1,n);
	dyt=vector(1,n);
	yt=vector(1,n);
	hh=h*0.5;
	h6=h/6.0;
	xh=x+hh;
	for (i=1;i<=n;i++) yt[i]=y[i]+hh*dydx[i];
	(*derivs)(xh,yt,dyt,n,delta);
	for (i=1;i<=n;i++) yt[i]=y[i]+hh*dyt[i];
	(*derivs)(xh,yt,dym,n,delta);
	for (i=1;i<=n;i++) {
		yt[i]=y[i]+h*dym[i];
		dym[i] += dyt[i];
	}
	(*derivs)(x+h,yt,dyt,n,delta);
	for (i=1;i<=n;i++)
		y[i]=y[i]+h6*(dydx[i]+dyt[i]+2.0*dym[i]);
	free_vector(yt,1,n);
	free_vector(dyt,1,n);
	free_vector(dym,1,n);
   free_vector(dydx,1,n);
}

void derivs(double t, double y[], double dydx[],int n, double delta) {
  double deno=1+delta*delta+0.5*(y[1]*y[1]+y[2]*y[2]);
  double eps=y[1]*y[2]/deno;
  double I_0=1/sqrt(1-eps*eps);
  double I_1=(1-I_0)/eps;
  dydx[1]=-(I_1*y[2]+I_0*y[1])/deno;
  dydx[2]=(I_1*y[1]+I_0*y[2])/deno;
}


main()
{

    pi=acos(-1.);

    // parametres physiques et pas d'integration
    delta=0;       // en MHz
    lambda=1.083e-6; // en m
    Gamma=1.6;      // en MHz
    Isat=1.6;       // W/m^2
    npixel=130*90;       // nombre de pixel
    g=0.2;
    pixx=17.0e-6/g;      // taille pixel x
    pixy=26.0e-6/g;      // taille pixel y
    Isat=1.6;
    dt=1.0e-2;      // pas pour l'integration
    Texp=200e-6;
    spix=pixx*pixy;
    cal1=1/(4.02e13*spix*Texp);
    cal2=1/(3.359e13*spix*Texp);

     DWORD start=GetTickCount();
   do Sleep(10); while (GetTickCount()-start<2000);

    param[1]=2.*delta/Gamma;
    sigma=3.*(lambda*lambda)/2./pi;
    nat=0.;
    j=0;
    natint=0;

    for (ipixel=1; ipixel<npixel+1; ipixel++)

    // lit les conditions initiales a z=-infini
    // y[1]=|A_+|;  |A_+|=I_i
    // y[2]=|A_-|;  |A_-|=I_fin
    // A_+ et A_- sont en unite de Gamma/2

    {
//       fscanf(fileprobe,"%i",&xi);
//       fscanf(fileabs,"%i",&xf);

       I_i=xi*cal1;
       I_f=xf*cal2;

       y[1]=sqrt(2.*I_i/Isat);
       y[2]=sqrt(2.*I_f/Isat);

       t=0.;
       dcol=0.;
       int iter=0;

       // boucle temporelle
      // ici t=0.5*sigma*Int(n(x,y,z')dz') pour z'=(-infiny , z)
           signe=y[1]-y[2];
           if (signe>0)
              {
              do {
                  rk4(y,2,t,dt,param[1],derivs);
                  t=t+dt;
                  iter=iter+1;
                  fin=(signe*(y[1]-y[2]))<=0;
                  } while (!fin);
              dcol=dcol+4*t/sigma;
              };
      //   nat=nat+dcol*spix;
    };

}















