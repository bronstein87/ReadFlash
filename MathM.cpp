
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
    M_ornt[0][0] = Quat[0]*Quat[0]+Quat[1]*Quat[1]-Quat[2]*Quat[2]-Quat[3]*Quat[3];
	M_ornt[0][1] = 2.0*(Quat[1]*Quat[2]+Quat[0]*Quat[3]);
	M_ornt[0][2] = 2.0*(Quat[1]*Quat[3]-Quat[0]*Quat[2]);
	M_ornt[1][0] = 2.0*(Quat[1]*Quat[2]-Quat[0]*Quat[3]);
	M_ornt[1][1] = Quat[0]*Quat[0]-Quat[1]*Quat[1]+Quat[2]*Quat[2]-Quat[3]*Quat[3];
	M_ornt[1][2] = 2.0*(Quat[2]*Quat[3]+Quat[0]*Quat[1]);
	M_ornt[2][0] = 2.0*(Quat[1]*Quat[3]+Quat[0]*Quat[2]);
	M_ornt[2][1] = 2.0*(Quat[2]*Quat[3]-Quat[0]*Quat[1]);
	M_ornt[2][2] = Quat[0]*Quat[0]-Quat[1]*Quat[1]-Quat[2]*Quat[2]+Quat[3]*Quat[3];
}

void MatrixToEkvAngles(const double Matrix[3][3], double Angles[3])
{
	Angles[1] = asinm(Matrix[2][2]); //delta
	Angles[0] = atan2m(Matrix[2][1], Matrix[2][0]);   if (Angles[0] < 0)  Angles[0] += 2 * PI;
	Angles[2] = atan2m(Matrix[0][2], Matrix[1][2]);   if (Angles[2] < 0)  Angles[2] += 2 * PI;
}

void calcTransitionMatrix(double pointAlpha, double pointBeta, double pointAzimut, double M_ornt[3][3])
{
	float PS,PC,QS,QC,RS,RC;

	PS = sin(pointAzimut); PC = cos(pointAzimut);
	QS = sin(pointBeta); QC = cos(pointBeta);
	RS = sin(pointAlpha); RC = cos(pointAlpha);
    M_ornt[0][0] = -PC*RS-PS*RC*QS;
    M_ornt[0][1] = PC*RC-PS*RS*QS;
    M_ornt[0][2] = PS*QC;
    M_ornt[1][0] = PS*RS-PC*RC*QS;
    M_ornt[1][1] = -PS*RC-PC*RS*QS;
    M_ornt[1][2] = PC*QC;
    M_ornt[2][0] = QC*RC;
    M_ornt[2][1] = QC*RS;
	M_ornt[2][2] = QS;
}

void getAngularDisplacementFromOrientMatr(const double M_ornt_pr[3][3],const double M_ornt[3][3], double Wop [3])

{
    double MT_ornt_pr[3][3], dMB[3][3];
    double delta, sdt;
    // считаем матрицу, дающую противоположный поворот
    for (int i = 0;i < 3;i ++)
    {
        for (int j = 0;j < 3;j ++)
        {
            MT_ornt_pr[i][j] = M_ornt_pr[j][i];
        }
    }
    // перемножаем, чтобы получить разницу
	multMatrix(M_ornt, MT_ornt_pr, dMB);


	delta=(dMB[0][0] + dMB[1][1] + dMB[2][2] - 1.)/2.;
    // если угол очень маленький, то считаем дельта равной 0
	if (delta > 1) delta = 1;

    if (fabs(delta - 1.0) < 1E-20)
    {
		Wop[0] = 0;
		Wop[1] = 0;
        Wop[2] = 0;
    }
    else
    {
        delta = acosm(delta);
        sdt = sin(delta);
        Wop[0]= -delta*(dMB[2][1] - dMB[1][2]) / (2.0 * sdt);
        Wop[1]= -delta*(dMB[0][2] - dMB[2][0]) / (2.0 * sdt);
        Wop[2]= -delta*(dMB[1][0] - dMB[0][1]) / (2.0 * sdt);
    }
}

void multMatrix(const double Matr1[3][3],const double Matr2[3][3], double Matr[3][3])
{
    double buf;
    int i,j,k;

    for(i = 0;i < 3;i ++)
    {
        for(j = 0;j < 3;j ++)
        {
            buf = 0;
            for (k = 0;k < 3;k ++)
            {
                buf = buf + Matr1[i][k] * Matr2[k][j];
            }
            Matr[i][j] = buf;
        }
    }
}
