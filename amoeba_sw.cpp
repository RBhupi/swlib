#include "amoeba_sw.h"
#include "gasrv_2.h"

#define NVALS 10000
#define NRANSI
#define NMAX 100
#define GET_PSUM \
		for (j=1;j<=ndim;j++) {\
		for (sum=0.0,i=1;i<=mpts;i++) sum += p[i][j];\
					psum[j]=sum;}
#define SWAP(a,b) {swap=(a);(a)=(b);(b)=swap;}

void get_normMeanStd(float trMean, float trStd, float LTPoint, float normMean, float normStd)
{
  //make first guess based on given mean and std
  float randval[NVALS];
  gen_nrand(normMean, normStd, randval);  //get the normal distribution for the given values.

  for(int ia=0; ia<NVALS; ia++) //truncate the distribution
  {
      if (randval[ia]<LTPoint)
          randval[ia]=0;
  }
  get_meanstd(randval, normMean, normStd); //get truncated mean and std of new distribution

};

void gen_nrand(float mean, float std, float *rvalues)
{
  gasRV nrand;                  //to generate random values
  nrand.generate (NVALS, rvalues);
  for (int ia=0; ia<NVALS; ia++)
    {
      rvalues[ia]=(rvalues[ia]*std)+mean;
    }
};

void get_meanstd(float values[], float mean, float std)
{
    int n = 0;
    mean = 0.0;
    float M2 = 0.0;

    for (int ia=0; ia<NVALS; ia++)
    {
        n = n + 1;
        float delta = values[ia] - mean;
        mean = mean + delta/n;
        M2 = M2 + delta*(values[ia] - mean);
    }
    std = sqrt(M2/(n - 1));
};



void amoeba(float **p, float *y, int ndim, float ftol, float (*funk)(float *), int *nfunk)
{
	float amotry(float **p, float *y, float *psum, int ndim,
								float (*funk)(float *), int ihi, float fac);
	int i,ihi,ilo,inhi,j,mpts=ndim+1;
	float rtol,sum,swap,ysave,ytry,*psum;

 psum = new float [ndim+1] ;
	*nfunk=0;
	GET_PSUM
	for (;;) {
		ilo=1;
		ihi = y[1]>y[2] ? (inhi=2,1) : (inhi=1,2);
		for (i=1;i<=mpts;i++) {
			if (y[i] <= y[ilo]) ilo=i;
			if (y[i] > y[ihi]) {
				inhi=ihi;
				ihi=i;
			} else if (y[i] > y[inhi] && i != ihi) inhi=i;
		}

	 if ( fabs(y[ihi]) + fabs(y[ilo]) == 0 )
		rtol = 0 ;
	 else
			rtol=2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo]));

	char message[256] ;
	sprintf(message, "rtol = %f", rtol) ;
		if (rtol < ftol) {
			SWAP(y[1],y[ilo])
			for (i=1;i<=ndim;i++) SWAP(p[1][i],p[ilo][i])
			break;
		}
		if (*nfunk >= NMAX) return ;
		*nfunk += 2;
		ytry=amotry(p,y,psum,ndim,funk,ihi,-1.0);
		if (ytry <= y[ilo])
			ytry=amotry(p,y,psum,ndim,funk,ihi,2.0);
		else if (ytry >= y[inhi]) {
			ysave=y[ihi];
			ytry=amotry(p,y,psum,ndim,funk,ihi,0.5);
			if (ytry >= ysave) {
				for (i=1;i<=mpts;i++) {
					if (i != ilo) {
						for (j=1;j<=ndim;j++)
							p[i][j]=psum[j]=0.5*(p[i][j]+p[ilo][j]);
						y[i]=(*funk)(psum);
					}
				}
				*nfunk += ndim;
				GET_PSUM
			}
		} else --(*nfunk);
	}
	delete psum ;

}
#undef SWAP
#undef GET_PSUM
#undef NMAX

float  amotry(float **p, float y[], float psum[], int ndim,
	float (*funk)(float []), int ihi, float fac)
{
	int j;
	float fac1,fac2,ytry,*ptry;

	ptry=new float [1+ndim];
	fac1=(1.0-fac)/ndim;
	fac2=fac1-fac;
	for (j=1;j<=ndim;j++) ptry[j]=psum[j]*fac1-p[ihi][j]*fac2;
	ytry=(*funk)(ptry);
	if (ytry < y[ihi]) {
		y[ihi]=ytry;
		for (j=1;j<=ndim;j++) {
			psum[j] += ptry[j]-p[ihi][j];
			p[ihi][j]=ptry[j];
		}
	}
	delete ptry ;
	return ytry;
}


#undef NRANSI


/* (C) Copr. 1986-92 Numerical Recipes Software <=nH. */
