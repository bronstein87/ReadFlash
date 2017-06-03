
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

void quatToMatr(const double Quat[], double M_ornt[3][3])
{
    M_ornt[0][0]=Quat[0]*Quat[0]+Quat[1]*Quat[1]-Quat[2]*Quat[2]-Quat[3]*Quat[3];
    M_ornt[0][1]=2.0*(Quat[1]*Quat[2]+Quat[0]*Quat[3]);
    M_ornt[0][2]=2.0*(Quat[1]*Quat[3]-Quat[0]*Quat[2]);
    M_ornt[1][0]=2.0*(Quat[1]*Quat[2]-Quat[0]*Quat[3]);
    M_ornt[1][1]=Quat[0]*Quat[0]-Quat[1]*Quat[1]+Quat[2]*Quat[2]-Quat[3]*Quat[3];
    M_ornt[1][2]=2.0*(Quat[2]*Quat[3]+Quat[0]*Quat[1]);
    M_ornt[2][0]=2.0*(Quat[1]*Quat[3]+Quat[0]*Quat[2]);
    M_ornt[2][1]=2.0*(Quat[2]*Quat[3]-Quat[0]*Quat[1]);
    M_ornt[2][2]=Quat[0]*Quat[0]-Quat[1]*Quat[1]-Quat[2]*Quat[2]+Quat[3]*Quat[3];
}
