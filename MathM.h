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

struct PointXYZ
{
	_fastcall PointXYZ(double _X, double _Y, double _Z): X(_X), Y(_Y), Z(_Z){}
	double X;
	double Y;
	double Z;
};

struct Statistika
{
	double mean, sigma;
	double min, max;
};

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
float GetTempSpec(char *sp);

template <class InputIterator, class Value, class UnaryOperation>
std::pair <Value, Value> calculateMeanStdDv(InputIterator first, InputIterator last, Value init, UnaryOperation extractWtC) {

	bool f = false;
	if (first == last) return std::pair <Value, Value> (extractWtC(*first, f), Value());

	Value dispersio = 0;
	unsigned int count_offset = 0;
	for (InputIterator i = first; i < last; i++) {
		Value temp = extractWtC(*i, f);
		if (f) {
			++count_offset;
			f = !f;
			continue;
		}
		else {
			init += temp;
			dispersio += pow(temp, 2);
		}
	}
	auto count = std::distance(first, last) - count_offset;
	Value mean = init / count;
	dispersio = (dispersio / count) - pow(mean, 2);
	return std::pair <Value, Value> (mean, sqrtm(dispersio));
}

template <class InputIterator, class Value, class UnaryOperation>
Statistika calculateStatParam(InputIterator first, InputIterator last, Value init, UnaryOperation extractWtC) {
Statistika stat;
	bool f = false;

	double dispersio = 0;
	double min, max;
	unsigned int count_offset = 0;
	bool flStart = true;


	for (InputIterator i = first; i < last; i++) {
		Value temp = extractWtC(*i, f);
		if (f) {
			++count_offset;
			f = !f;
			continue;
		}
		else {
			if (flStart) {
				min = temp;
				max = temp;
				flStart = false;
			}
			else
			{
				if (temp < min) min = temp;
				if (temp > max) max = temp;
			}
			init += temp;
			dispersio += pow(temp, 2);
		}
	}
	auto count = std::distance(first, last) - count_offset;
	double mean = init / count;
	dispersio  = (dispersio / count) - pow(mean, 2);

	stat.mean  = mean;
	stat.sigma = sqrtm(dispersio);
	stat.min   = min;
	stat.max   = max;
	return stat;
}

#endif
