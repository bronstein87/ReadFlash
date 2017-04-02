
#include "mathM.h"
double sqrtm(double xf)
{
  if (xf<1e-20)
    return 0.;
  else
	return sqrt(xf);
}

double acosm(double xf)
{
  if (xf>1.) xf=1.;
  else if (xf<-1.) xf=-1.;

  return acos(xf);
}

double asinm(double xf)
{
  if (xf>1.) xf=1.;
  else if (xf<-1.) xf=-1.;

  return asin(xf);
}

double atan2m(double yf, double xf)
{
  double ang;
  if (fabs(xf)>1e-10) ang=atan2(yf,xf);
  else
  {
	if (yf>0) ang=PI/2.;
	else ang=-PI/2.;
  }
  return ang;
}
