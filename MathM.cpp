
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

double GetAxisAngle(double lmn1[3], double lmn2[3])
{
	double l1, m1, n1, l2, m2, n2;
	double diff_axes, l_or, m_or, n_or;

	l1 = lmn1[0];  m1 = lmn1[1];   n1 = lmn1[2];
	l2 = lmn2[0];  m2 = lmn2[1];   n2 = lmn2[2];

	l_or=m1*n2-m2*n1;
	m_or=n1*l2-n2*l1;
	n_or=l1*m2-l2*m1;

	diff_axes=sqrtm(l_or*l_or+m_or*m_or+n_or*n_or);
	return asinm(diff_axes);
}

template <class InputIterator, class T, class UnaryOperation>
std::pair<T,T> calculateMeanStDv (InputIterator first, InputIterator last, T init, UnaryOperation extractWtC)
{
    if (first == last) return std::pair<T,T>(extractWtC(first), T());

    T dispersio = 0;
    for (InputIterator i = first;i < last;i ++)
    {
        init += extractWtC(i);
        dispersio += pow(extractWtC(i), 2);
    }
    auto count = std::distance(first,last);
    T mean = init / count;
    dispersio = (dispersio / count) - pow(mean, 2);

    return std::pair <T,T> (mean, sqrt(dispersio));

}

void ToGMS(double gradAngle, int& gradus, int& minutes, int& seconds)
{
	gradus = gradAngle;
    minutes = std::abs(gradAngle - gradus) * 60;
    seconds = (std::abs(gradAngle - gradus) - (static_cast <double> (minutes) / 60)) * 3600;

    if (gradAngle < 0 && gradAngle > -1)
    {
        minutes = -minutes;
        seconds = -seconds;
	}
}

float GetTempSpec(char *sp)
{
	short IT = -1, NT;
	float temp;
	float TEMP[8] = {60000, 30000, 10000, 7500, 6000, 5000, 3500, 2000};
	switch (sp[0])
	{
		case 'O':  { IT = 0; break;}
		case 'B':  { IT = 1; break;}
		case 'A':  { IT = 2; break;}
		case 'F':  { IT = 3; break;}
		case 'G':  { IT = 4; break;}
		case 'K':  { IT = 5; break;}
		case 'M':  { IT = 6; break;}
		default:   { IT = 0; break;}
	}
	switch (sp[1])
	{
		case '0': { NT = 0; break;}
		case '1': { NT = 1; break;}
		case '2': { NT = 2; break;}
		case '3': { NT = 3; break;}
		case '4': { NT = 4; break;}
		case '5': { NT = 5; break;}
		case '6': { NT = 6; break;}
		case '7': { NT = 7; break;}
		case '8': { NT = 8; break;}
		case '9': { NT = 9; break;}
		case 'A': { NT = 5; break;}
		case 'B': { NT = 5; break;}
		case 'E': { NT = 5; break;}
		default : { NT = 0; break;}
	}
	if ( (sp[0] == 'M')&&((sp[1] == '6')||(sp[1] == '7')
	   ||(sp[1] == '8')||(sp[1] == '9'))) NT = 5;

  temp = TEMP[IT] - (TEMP[IT] - TEMP[IT+1]) * ((double)NT + 0.5) / 10.;
  return temp;
}
