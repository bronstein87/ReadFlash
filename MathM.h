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

#define SUCSESS 0x000
#define SUCSESS_TO 0xFF00
#define HO_first 0x100 // First cadr is processed !
#define HO_Ornt 0x200 // Orient is got after 2-nd cadr in HO!
#define BAD_Light 0x500 // HO/TO/SLEZH < 4 Objects, Lightness
#define BAD_NObj 0x600 // HO/TO/SLEZH < 4 Objects
#define BAD_DetLt 0x700 // Bad deter SL < 4 stars with lightness

#define BAD_DetA 0x800 // Bad deter TO < 4 stars
#define BAD_DetC 0x900 // Bad deter HO < 4 stars
#define BAD_Apr 0xA00 // sum(Quat[i]) != 1, or is absent, or ang. M_Bort^M_ornt>15gr
#define BAD_Per 0xB00 // crash SP, invalid variable!
#define BAD_Ornt 0xC00 // Bad Orient() m_cur >2*Pix
#define BAD_DetSl 0xD00 // Bad deter in SLEZH()
#define BAD_Frag 0xE00 // Error extract fragment from buffer

struct PointXYZ {
	_fastcall PointXYZ(double _X, double _Y, double _Z) : X(_X), Y(_Y), Z(_Z) {
	}

	double X;
	double Y;
	double Z;
};

struct Statistika {
	Statistika() : mean(0), sigma(0), min(0), max(0) {
	}

	double mean, sigma;
	double min, max;
};


double sqrtm(double xf);
double acosm(double xf);
double asinm(double xf);
double atan2m(double yf, double xf);
void quatToMatr(const double Quat[], double M_ornt[3][3]);
void MatrixToEkvAngles(const double Matrix[3][3], double Angles[3]);
int CheckQuatNorm(const double quat[4], double deltaNorm);
void QuatToMatrix(double quat[4], double matrix[3][3]);
double GetAxisAngle(double lmn1[3], double lmn2[3]);
void calcTransitionMatrix(double pointAlpha, double pointBeta,
	double pointAzimut, double M_ornt[3][3]);
void getAngularDisplacementFromOrientMatr(const double M_ornt_pr[3][3],
	const double M_ornt[3][3], double Wop[3]);
void multMatrix(const double Matr1[3][3], const double Matr2[3][3],
	double Matr[3][3]);
void ToGMS(double gradAngle, int& gradus, int& minutes, int& seconds);
float GetTempSpec(char *sp);

template<class InputIterator, class Value, class UnaryOperation>
std::pair<Value, Value>calculateMeanStdDv(InputIterator first,
	InputIterator last, Value init, UnaryOperation extractWtC) {

	bool f = false;
	if (first == last)
		return std::pair<Value, Value>(extractWtC(*first, f), Value());

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
	return std::pair<Value, Value>(mean, sqrtm(dispersio));
}

template<class InputIterator, class Value, class UnaryOperation>
Statistika calculateStatParam(InputIterator first, InputIterator last,
	Value init, UnaryOperation extractWtC) {
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
			else {
				if (temp < min)
					min = temp;
				if (temp > max)
					max = temp;
			}
			init += temp;
			dispersio += pow(temp, 2);
		}
	}
	auto count = std::distance(first, last) - count_offset;
	if (count == 0) {
		return stat;

	}
	double mean = init / count;
	dispersio = (dispersio / count) - pow(mean, 2);

	stat.mean = mean;
	stat.sigma = sqrtm(dispersio);
	stat.min = min;
	stat.max = max;
	return stat;
}

#endif
