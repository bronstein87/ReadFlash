#ifndef MathM
#define MathM

#include <math.h>
#include <vector>
#include <utility>
#define PI    3.141592653589793
#define PI_2  6.283185307179587
#define RTD   57.29577951308232  /* RTD degries in 1 radian: 180/pi */
#define RTM	  3437.746770784939  /* RTM minuts in 1 radian:  60*180/pi */
#define RTS   206264.8062470964  /* RTS seconds in 1 radian: 3600*180/pi */
#define DTR   0.0174532925
#define BOKZ1000ConvCoef       0.098174770424681


double sqrtm(double xf);
double acosm(double xf);
double asinm(double xf);
double atan2m(double yf, double xf);
void quatToMatr(const double Quat[], double M_ornt[3][3]);
void MatrixToEkvAngles(const double Matrix[3][3], double Angles[3]);
double GetAxisAngle(double lmn1[3], double lmn2[3]);
void calcTransitionMatrix(double pointAlpha, double pointBeta, double pointAzimut, double M_ornt[3][3]);
void getAngularDisplacementFromOrientMatr(const double M_ornt_pr[3][3],const double M_ornt[3][3], double Wop [3]);
void multMatrix(const double Matr1[3][3],const double Matr2[3][3], double Matr[3][3]);
void ToGMS (double gradAngle, int& gradus, int& minutes, int& seconds);

template <class InputIterator, class Value, class UnaryOperation>
std::pair<Value, Value> calculateMeanStdDv (InputIterator first, InputIterator last, Value init, UnaryOperation extractWtC)
{
	if (first == last) return std::pair <Value, Value> (extractWtC(*first), Value());

	Value dispersio = 0;
	for (InputIterator i = first;i < last; i++)
	{
		init += extractWtC(*i);
		dispersio += pow(extractWtC(*i), 2);
	}
	auto count = std::distance(first,last);
	Value mean = init / count;
	dispersio = (dispersio / count) - pow(mean, 2);
    if(abs(dispersio) < 0.0000000001) dispersio = 0;

	return std::pair <Value,Value> (mean, sqrt(dispersio));
}

#endif
