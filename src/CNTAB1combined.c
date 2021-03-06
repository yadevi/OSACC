#include <math.h>
#define NRANSI
#include "nrutil.h"
#define TINY 1.0e-30

float gammln(float xx);
float gammq(float a, float x);
void gcf(float *gammcf, float a, float x, float *gln);
void gser(float *gamser, float a, float x, float *gln);
 
void cntab1(int **nn, int ni, int nj, float *chisq, float *df, float *prob,float *cramrv, float *ccc)
{
	float gammq(float a, float x);
	int nnj,nni,j,i,minij;
	float sum=0.0,expctd,*sumi,*sumj,temp;

	sumi=vector(1,ni);
	sumj=vector(1,nj);
	nni=ni;
	nnj=nj;
	for (i=1;i<=ni;i++) {
		sumi[i]=0.0;
		for (j=1;j<=nj;j++) {
			sumi[i] += nn[i][j];
			sum += nn[i][j];
		}
		if (sumi[i] == 0.0) --nni;
	}
	for (j=1;j<=nj;j++) {
		sumj[j]=0.0;
		for (i=1;i<=ni;i++) sumj[j] += nn[i][j];
		if (sumj[j] == 0.0) --nnj;
	}
	*df=nni*nnj-nni-nnj+1;
	*chisq=0.0;
	for (i=1;i<=ni;i++) {
		for (j=1;j<=nj;j++) {
			expctd=sumj[j]*sumi[i]/sum;
			temp=nn[i][j]-expctd;
			*chisq += temp*temp/(expctd+TINY);
		}
	}
	*prob=gammq(0.5*(*df),0.5*(*chisq));
	minij = nni < nnj ? nni-1 : nnj-1;
	*cramrv=sqrt(*chisq/(sum*minij));
	*ccc=sqrt(*chisq/(*chisq+sum));
	free_vector(sumj,1,nj);
	free_vector(sumi,1,ni);
}
#undef TINY
#undef NRANSI
/* (C) Copr. 1986-92 Numerical Recipes Software "25Bi..<!. */
#include <math.h>

float gammln(float xx)
{
	double x,y,tmp,ser;
	static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
	int j;

	y=x=xx;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.000000000190015;
	for (j=0;j<=5;j++) ser += cof[j]/++y;
	return -tmp+log(2.5066282746310005*ser/x);
}
/* (C) Copr. 1986-92 Numerical Recipes Software "25Bi..<!. */
float gammq(float a, float x)
{
	void gcf(float *gammcf, float a, float x, float *gln);
	void gser(float *gamser, float a, float x, float *gln);
	void nrerror(char error_text[]);
	float gamser,gammcf,gln;

	if (x < 0.0 || a <= 0.0) nrerror("Invalid arguments in routine gammq");
	if (x < (a+1.0)) {
		gser(&gamser,a,x,&gln);
		return 1.0-gamser;
	} else {
		gcf(&gammcf,a,x,&gln);
		return gammcf;
	}
}
/* (C) Copr. 1986-92 Numerical Recipes Software "25Bi..<!. */
#include <math.h>
#define ITMAX 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30

void gcf(float *gammcf, float a, float x, float *gln)
{
	float gammln(float xx);
	void nrerror(char error_text[]);
	int i;
	float an,b,c,d,del,h;

	*gln=gammln(a);
	b=x+1.0-a;
	c=1.0/FPMIN;
	d=1.0/b;
	h=d;
	for (i=1;i<=ITMAX;i++) {
		an = -i*(i-a);
		b += 2.0;
		d=an*d+b;
		if (fabs(d) < FPMIN) d=FPMIN;
		c=b+an/c;
		if (fabs(c) < FPMIN) c=FPMIN;
		d=1.0/d;
		del=d*c;
		h *= del;
		if (fabs(del-1.0) < EPS) break;
	}
	if (i > ITMAX) nrerror("a too large, ITMAX too small in gcf");
	*gammcf=exp(-x+a*log(x)-(*gln))*h;
}
#undef ITMAX
#undef EPS
#undef FPMIN
/* (C) Copr. 1986-92 Numerical Recipes Software "25Bi..<!. */
#include <math.h>
#define ITMAX 100
#define EPS 3.0e-7

void gser(float *gamser, float a, float x, float *gln)
{
	float gammln(float xx);
	void nrerror(char error_text[]);
	int n;
	float sum,del,ap;

	*gln=gammln(a);
	if (x <= 0.0) {
		if (x < 0.0) nrerror("x less than 0 in routine gser");
		*gamser=0.0;
		return;
	} else {
		ap=a;
		del=sum=1.0/a;
		for (n=1;n<=ITMAX;n++) {
			++ap;
			del *= x/ap;
			sum += del;
			if (fabs(del) < fabs(sum)*EPS) {
				*gamser=sum*exp(-x+a*log(x)-(*gln));
				return;
			}
		}
		nrerror("a too large, ITMAX too small in routine gser");
		return;
	}
}
#undef ITMAX
#undef EPS
/* (C) Copr. 1986-92 Numerical Recipes Software "25Bi..<!. */
