#ifndef StatLib_h
#define StatLib_h

#include "stdio.h"
#include <vector>
using namespace std;

double Maximum (double *massive1, double *massive2, int N);
double Minimum (double *massive1, double *massive2, int N);
double Maximum (double *massive1, int N);
double Minimum (double *massive1, int N);
double noise(double sigma);
void LineInterpolation (double *t, double *t_in, double *x, double *x_in, int N, int N_in);
void SquareInterpolation (double *t, double *t_in, double *x, double *x_in, int N, int N_in);
double IntError(double *x, double *y, int N);
//int FindIntervals(vector<double>& x, vector<double>& y, vector <int>::iterator pN0, vector <int>::iterator pNk, int* Nint) ;
int MNK1pow (double *x, double *y, int n0, int nk, double *a,double *b);
int MNK2pow (double *x, double *y, int n0, int nk, double *a,double *b,double *c);
int MNK2(double *x, double *y, int n, double *a,double *b,double *c);
int MNK(double *x, double *y, int M,int N, double *a);
double VAllon(vector <double>& t, vector <double>& vr,double Tint);
int StatFunc(vector <double>& data, double *mx, double *sigmax, double *minimum,double *maximum);
int StatFunc(double* data, int N,double *mx, double *sigmax, double *minimum,double *maximum);
//double AngOutToFloat(int Ang);
double TempOutToFloat(int Temp);
template<class T> T BareissDeterminant(T **const M, int const N);

#endif

