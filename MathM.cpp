#include "mathM.h"

double sqrtm(double xf) {
	if (xf < 1e-20)
		return 0.;
	else
		return sqrt(xf);
}

double acosm(double xf) {
	if (xf > 1.)
		xf = 1.;
	else if (xf < -1.)
		xf = -1.;

	return acos(xf);
}

double asinm(double xf) {
	if (xf > 1.)
		xf = 1.;
	else if (xf < -1.)
		xf = -1.;

	return asin(xf);
}

int CheckQuatNorm(const double quat[4], double deltaNorm)
{
	double norm;
	if ((deltaNorm < 0) || (deltaNorm > 0.1)) return -1;
	norm = quat[0] * quat[0] + quat[1] * quat[1] + quat[2] * quat[2] + quat[3] * quat[3];
	norm = sqrtm(norm);
	if (fabs(norm - 1.) <= deltaNorm) return 0;
	else return 1;
}

void QuatToMatrix(double quat[4], double matrix[3][3])
{
	matrix[0][0]=quat[0]*quat[0]+quat[1]*quat[1]-quat[2]*quat[2]-quat[3]*quat[3];
	matrix[0][1]=2.0*(quat[1]*quat[2]+quat[0]*quat[3]);
	matrix[0][2]=2.0*(quat[1]*quat[3]-quat[0]*quat[2]);
	matrix[1][0]=2.0*(quat[1]*quat[2]-quat[0]*quat[3]);
	matrix[1][1]=quat[0]*quat[0]-quat[1]*quat[1]+quat[2]*quat[2]-quat[3]*quat[3];
	matrix[1][2]=2.0*(quat[2]*quat[3]+quat[0]*quat[1]);
	matrix[2][0]=2.0*(quat[1]*quat[3]+quat[0]*quat[2]);
	matrix[2][1]=2.0*(quat[2]*quat[3]-quat[0]*quat[1]);
	matrix[2][2]=quat[0]*quat[0]-quat[1]*quat[1]-quat[2]*quat[2]+quat[3]*quat[3];
}


double atan2m(double yf, double xf) {
	double ang;
	if (fabs(xf) > 1e-10)
		ang = atan2(yf, xf);
	else {
		if (yf > 0)
			ang = PI / 2.;
		else
			ang = -PI / 2.;
	}
	return ang;
}

void quatToMatr(const double Quat[], double M_ornt[3][3]) {
	M_ornt[0][0] = Quat[0] * Quat[0] + Quat[1] * Quat[1] - Quat[2] * Quat[2] -
		Quat[3] * Quat[3];
	M_ornt[0][1] = 2.0 * (Quat[1] * Quat[2] + Quat[0] * Quat[3]);
	M_ornt[0][2] = 2.0 * (Quat[1] * Quat[3] - Quat[0] * Quat[2]);
	M_ornt[1][0] = 2.0 * (Quat[1] * Quat[2] - Quat[0] * Quat[3]);
	M_ornt[1][1] = Quat[0] * Quat[0] - Quat[1] * Quat[1] + Quat[2] * Quat[2] -
		Quat[3] * Quat[3];
	M_ornt[1][2] = 2.0 * (Quat[2] * Quat[3] + Quat[0] * Quat[1]);
	M_ornt[2][0] = 2.0 * (Quat[1] * Quat[3] + Quat[0] * Quat[2]);
	M_ornt[2][1] = 2.0 * (Quat[2] * Quat[3] - Quat[0] * Quat[1]);
	M_ornt[2][2] = Quat[0] * Quat[0] - Quat[1] * Quat[1] - Quat[2] * Quat[2] +
		Quat[3] * Quat[3];
}

void MatrixToEkvAngles(const double Matrix[3][3], double Angles[3]) {
	Angles[1] = asinm(Matrix[2][2]); // delta
	Angles[0] = atan2m(Matrix[2][1], Matrix[2][0]);
	if (Angles[0] < 0)
		Angles[0] += 2 * PI;
	Angles[2] = atan2m(Matrix[0][2], Matrix[1][2]);
	if (Angles[2] < 0)
		Angles[2] += 2 * PI;
}

void calcTransitionMatrix(double pointAlpha, double pointBeta,
	double pointAzimut, double M_ornt[3][3]) {
	float PS, PC, QS, QC, RS, RC;

	PS = sin(pointAzimut);
	PC = cos(pointAzimut);
	QS = sin(pointBeta);
	QC = cos(pointBeta);
	RS = sin(pointAlpha);
	RC = cos(pointAlpha);
	M_ornt[0][0] = -PC * RS - PS * RC * QS;
	M_ornt[0][1] = PC * RC - PS * RS * QS;
	M_ornt[0][2] = PS * QC;
	M_ornt[1][0] = PS * RS - PC * RC * QS;
	M_ornt[1][1] = -PS * RC - PC * RS * QS;
	M_ornt[1][2] = PC * QC;
	M_ornt[2][0] = QC * RC;
	M_ornt[2][1] = QC * RS;
	M_ornt[2][2] = QS;
}

void getAngularDisplacementFromOrientMatr(const double M_ornt_pr[3][3],
	const double M_ornt[3][3], double Wop[3])

{
	double MT_ornt_pr[3][3], dMB[3][3];
	double delta, sdt;
	// считаем матрицу, дающую противоположный поворот
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			MT_ornt_pr[i][j] = M_ornt_pr[j][i];
		}
	}
	// перемножаем, чтобы получить разницу
	multMatrix(M_ornt, MT_ornt_pr, dMB);

	delta = (dMB[0][0] + dMB[1][1] + dMB[2][2] - 1.) / 2.;
	// если угол очень маленький, то считаем дельта равной 0
	if (delta > 1)
		delta = 1;

	if (fabs(delta - 1.0) < 1E-20) {
		Wop[0] = 0;
		Wop[1] = 0;
		Wop[2] = 0;
	}
	else {
		delta = acosm(delta);
		sdt = sin(delta);
		Wop[0] = -delta * (dMB[2][1] - dMB[1][2]) / (2.0 * sdt);
		Wop[1] = -delta * (dMB[0][2] - dMB[2][0]) / (2.0 * sdt);
		Wop[2] = -delta * (dMB[1][0] - dMB[0][1]) / (2.0 * sdt);
	}
}

void multMatrix(const double Matr1[3][3], const double Matr2[3][3],
	double Matr[3][3]) {
	double buf;
	int i, j, k;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			buf = 0;
			for (k = 0; k < 3; k++) {
				buf = buf + Matr1[i][k] * Matr2[k][j];
			}
			Matr[i][j] = buf;
		}
	}
}

double GetAxisAngle(double lmn1[3], double lmn2[3]) {
	double l1, m1, n1, l2, m2, n2;
	double diff_axes, l_or, m_or, n_or;

	l1 = lmn1[0];
	m1 = lmn1[1];
	n1 = lmn1[2];
	l2 = lmn2[0];
	m2 = lmn2[1];
	n2 = lmn2[2];

	l_or = m1 * n2 - m2 * n1;
	m_or = n1 * l2 - n2 * l1;
	n_or = l1 * m2 - l2 * m1;

	diff_axes = sqrtm(l_or * l_or + m_or * m_or + n_or * n_or);
	return asinm(diff_axes);
}

template<class InputIterator, class T, class UnaryOperation>
std::pair<T, T>calculateMeanStDv(InputIterator first, InputIterator last,
	T init, UnaryOperation extractWtC) {
	if (first == last)
		return std::pair<T, T>(extractWtC(first), T());

	T dispersio = 0;
	for (InputIterator i = first; i < last; i++) {
		init += extractWtC(i);
		dispersio += pow(extractWtC(i), 2);
	}
	auto count = std::distance(first, last);
	T mean = init / count;
	dispersio = (dispersio / count) - pow(mean, 2);

	return std::pair<T, T>(mean, sqrt(dispersio));

}

void ToGMS(double gradAngle, int& gradus, int& minutes, int& seconds) {
	gradus = gradAngle;
	minutes = std::abs(gradAngle - gradus) * 60;
	seconds = (std::abs(gradAngle - gradus) -
		(static_cast<double>(minutes) / 60)) * 3600;

	if (gradAngle < 0 && gradAngle > -1) {
		minutes = -minutes;
		seconds = -seconds;
	}
}

float GetTempSpec(char *sp) {
	short IT = -1, NT;
	float temp;
	float TEMP[8] = {60000, 30000, 10000, 7500, 6000, 5000, 3500, 2000};
	switch (sp[0]) {
	case 'O': {
			IT = 0;
			break;
		}
	case 'B': {
			IT = 1;
			break;
		}
	case 'A': {
			IT = 2;
			break;
		}
	case 'F': {
			IT = 3;
			break;
		}
	case 'G': {
			IT = 4;
			break;
		}
	case 'K': {
			IT = 5;
			break;
		}
	case 'M': {
			IT = 6;
			break;
		}
	default: {
			IT = 0;
			break;
		}
	}
	switch (sp[1]) {
	case '0': {
			NT = 0;
			break;
		}
	case '1': {
			NT = 1;
			break;
		}
	case '2': {
			NT = 2;
			break;
		}
	case '3': {
			NT = 3;
			break;
		}
	case '4': {
			NT = 4;
			break;
		}
	case '5': {
			NT = 5;
			break;
		}
	case '6': {
			NT = 6;
			break;
		}
	case '7': {
			NT = 7;
			break;
		}
	case '8': {
			NT = 8;
			break;
		}
	case '9': {
			NT = 9;
			break;
		}
	case 'A': {
			NT = 5;
			break;
		}
	case 'B': {
			NT = 5;
			break;
		}
	case 'E': {
			NT = 5;
			break;
		}
	default: {
			NT = 0;
			break;
		}
	}
	if ((sp[0] == 'M') && ((sp[1] == '6') || (sp[1] == '7') || (sp[1] == '8') ||
		(sp[1] == '9')))
		NT = 5;

	temp = TEMP[IT] - (TEMP[IT] - TEMP[IT + 1]) * ((double)NT + 0.5) / 10.;
	return temp;
}

double DateTimeToDaysJ2000(struct _DateTime *stDateTime)
{
	int i, mes[12]={31,28,31,30,31,30,31,31,30,31,30,31};
// Юлианская дата 2000 года 12 часов 1 января  = 2451545.0;
	double JD = 0;
// прибавляем число суток за прошедшие  года (не вкл текущий год)
	JD += ((double)stDateTime->Year - 2000.) * 365.;

// считаем  число високосных лет с 2000 года по нынешний (не включительно)
	for (i = 2000; i < stDateTime->Year; i++) {
		if (i % 4 == 0) JD += 1.;
	}
//прибавляем 29 февраля, если оно было в текущем году
	if ((stDateTime->Year % 4 == 0) && (stDateTime->Month >= 3)) JD += 1.;

	//считаем число дней в текущем году, не вкл текущий месяц
	for (i = 1; i < stDateTime->Month; i++) JD += (double)mes[i-1];
	JD += stDateTime->Day - 1;  // прибавляем число дней в текущем месяце
	JD  = JD - 0.5;
	JD += ((double)stDateTime->Hour + (double)stDateTime->Min/60.
			+ (double)stDateTime->Sec/3600. + (double)stDateTime->mSec/3600000.)/24.;

	return JD;
}

void SunVector(double JD_DEV, double mOr[3][3], double *pSunI,  double *pSunD)
{
int i,j;
double Ts, Ls, Ms, eS, Es, Vs, Qs, Ekl, AlphaS, DeltaS, QsVid, Om, JD;

	   Ts=(JD_DEV-2415020.0)/36525.;
	   Ls=279.69668+36000.76892*Ts+0.0003025*Ts*Ts;

	   Ls/=360.;
	   Ls-=(int)Ls;
	   Ls*=360.;

	   Ms=358.47583+35999.04975*Ts-0.000150*Ts*Ts-0.0000033*Ts*Ts*Ts;
	   Ms/=360.;
	   Ms-=(int)Ms;
	   Ms*=360.;

	   Ms*=PI/180.;
	   eS=0.01675104-0.0000418*Ts-0.000000126*Ts*Ts;
	   Es=Ms;
	   for (i = 0; i < 10; i++)  Es=Ms+eS*sin(Es);
	   Vs=2*atan(sqrtm((1+eS)/(1-eS))*tan(Es/2));
	   Qs=Ls*PI/180.+Vs-Ms;

	   Om=259.18-1934.142*Ts;
	   Om/=360.;
	   Om-=(int)Om;
	   Om*=2*PI;

	   QsVid=Qs*180/PI-0.00569-0.00479*sin(Om);
	   QsVid=QsVid*PI/180.;

	   Ekl=23.452294-0.0130125*Ts-0.00000164*Ts*Ts+0.000000503*Ts*Ts*Ts;
	   Ekl+=0.00256*cos(Om);
	   Ekl*=PI/180.;

	   AlphaS=atan2(cos(Ekl)*sin(QsVid),cos(QsVid));
	   DeltaS= asin(sin(Ekl)*sin(QsVid));

	   if (AlphaS<0) AlphaS+=2*PI;

	   pSunI[0]=cos(AlphaS)*cos(DeltaS);
	   pSunI[1]=sin(AlphaS)*cos(DeltaS);
	   pSunI[2]=sin(DeltaS);

	   for (i =0; i<3; i++) {
		   pSunD[i]=0;
		   for (j =0; j< 3; j++){
			   pSunD[i]+=mOr[i][j]*pSunI[j];
		   }
	   }
}
