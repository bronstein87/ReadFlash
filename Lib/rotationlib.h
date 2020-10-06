/*
RotationLib
Version: 3.0
Date:    27.09.2017
*/

#ifndef _ROTATION_LIB_H_
#define _ROTATION_LIB_H_

#include "config.h"
#include "mathlib.h"

typedef struct {
	_double l, m, n;
} TVectLMN;

typedef struct {
	_double x, y, z;
} TVectXYZ;

typedef struct {
	_double w, x, y, z;
} TQuat;

typedef _double _tmatrix[3][3];

typedef struct {
	_double al, dl, Az;
} TAngles;

#ifdef __cplusplus
extern "C" {
#endif
void SingleMatrix(_tmatrix matrix);
void TranspMatrix(_tmatrix matrix, _tmatrix matrixT);
void MultMatrix(const _tmatrix matrix1, const _tmatrix matrix2, _tmatrix matrix);
void MultMatrixVect(const _tmatrix matrix, const TVectXYZ *vect1, TVectXYZ *vect2);
void MatrixToQuat(const _tmatrix matrix, _double quat[4]);
void QuatToMatrix(const TQuat *quat, _tmatrix matrix);
int  CheckQuatNorm(const TQuat *quat, _double deltaNorm);

void AnglesToMatrixOrient(const TAngles *_orient, _tmatrix matrix);
void MatrixOrientToAngles(const _tmatrix matrix, TAngles *orient);
void AnglesToMatrixRotation(const TVectXYZ *ang, _tmatrix matrix);
void MatrixRotationToAngles(const _tmatrix matrix, TVectXYZ *ang);
void GetRotationAngles(const _tmatrix matrix_prev,
					   const _tmatrix matrix_cur, TVectXYZ *omega);

void CorrectMatrix(const TVectLMN *Z1, const TVectLMN *Z2,
				   const _tmatrix matrix1,  _tmatrix matrix2);

void MultQuat(const TQuat *Q1, const TQuat *Q2, TQuat *QR);
void InverseQuat(const TQuat *Q, TQuat *QR);

#ifdef __cplusplus
}
#endif
#endif
