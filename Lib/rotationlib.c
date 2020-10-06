#include "rotationlib.h"

void SingleMatrix(_tmatrix matrix)
{
	matrix[0][0] = 1.;	matrix[0][1] = 0.;	matrix[0][2] = 0.;
	matrix[1][0] = 0.;	matrix[1][1] = 1.;	matrix[1][2] = 0.;
	matrix[2][0] = 0.;	matrix[2][1] = 0.;	matrix[2][2] = 1.;
}

void TranspMatrix(_tmatrix matrix, _tmatrix matrixT)
{
	int i,j;
	_tmatrix matrixC;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			matrixC[i][j] = matrix[j][i];
		}
	}

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			matrixT[i][j] = matrixC[i][j];
		}
	}
}

void MultMatrix(const _tmatrix matrix1, const _tmatrix matrix2, _tmatrix matrix)
{
	int i, j, k;
	double buf;
	_tmatrix matrixC;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			buf = 0;
			for (k = 0; k < 3; k++) {
				buf += (matrix1[i][k] * matrix2[k][j]);
			}
			matrixC[i][j] = buf;
		}
	}

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			matrix[i][j] = matrixC[i][j];
		}
	}
}

void MultMatrixVect(const _tmatrix matrix, const TVectXYZ *vect1, TVectXYZ *vect2)
{
	vect2->x = matrix[0][0] * vect1->x + matrix[0][1] * vect1->y + matrix[0][2] * vect1->z;
	vect2->y = matrix[1][0] * vect1->x + matrix[1][1] * vect1->y + matrix[1][2] * vect1->z;
	vect2->z = matrix[2][0] * vect1->x + matrix[2][1] * vect1->y + matrix[2][2] * vect1->z;
}

void MatrixToQuat(const _tmatrix matrix, _double quat[4])
{
	double QQ[4], max;
	int i,nP;

	QQ[0] =  matrix[0][0] + matrix[1][1] + matrix[2][2];
	QQ[1] =  matrix[0][0] - matrix[1][1] - matrix[2][2];
	QQ[2] = -matrix[0][0] + matrix[1][1] - matrix[2][2];
	QQ[3] = -matrix[0][0] - matrix[1][1] + matrix[2][2];

	max = -3.0;
	for (i = 0; i < 4; i++) {
		if (QQ[i] > max) {
			max = QQ[i]; nP = i;
		}
	}

	switch (nP)
	{
	case 0:
  // ?ло? P0
		quat[0] = 0.5  * sqrtm(1.0l + QQ[0]);
		quat[1] = 0.25 * (matrix[1][2] - matrix[2][1]) / quat[0];
		quat[2] = 0.25 * (matrix[2][0] - matrix[0][2]) / quat[0];
		quat[3] = 0.25 * (matrix[0][1] - matrix[1][0]) / quat[0];
		break;
	case 1:
  // ?ло? P1
		quat[1] = 0.5  * sqrtm(1.0l + QQ[1]);
		quat[0] = 0.25 * (matrix[1][2] - matrix[2][1]) / quat[1];
		quat[2] = 0.25 * (matrix[0][1] + matrix[1][0]) / quat[1];
		quat[3] = 0.25 * (matrix[2][0] + matrix[0][2]) / quat[1];
		break;
	case 2:
  // ?ло? P2
		quat[2] = 0.5  * sqrtm (1.0l + QQ[2]);
		quat[0] = 0.25 * (matrix[2][0] - matrix[0][2]) / quat[2];
		quat[1] = 0.25 * (matrix[0][1] + matrix[1][0]) / quat[2];
		quat[3] = 0.25 * (matrix[1][2] + matrix[2][1]) / quat[2];
		break;
	case 3:
  // ?ло? P3
		quat[3] = 0.5  * sqrtm(1.0l + QQ[3]);
		quat[0] = 0.25 * (matrix[0][1] - matrix[1][0]) / quat[3];
		quat[1] = 0.25 * (matrix[2][0] + matrix[0][2]) / quat[3];
		quat[2] = 0.25 * (matrix[1][2] + matrix[2][1]) / quat[3];
	}

	if (quat[0] < 0.0)
	{
		quat[0] = -quat[0]; quat[1] = -quat[1];
		quat[2] = -quat[2]; quat[3] = -quat[3];
	}
}

void QuatToMatrix(const TQuat *quat, _tmatrix matrix)
{
	matrix[0][0] = quat->w * quat->w + quat->x * quat->x
				 - quat->y * quat->y - quat->z * quat->z;
	matrix[0][1] = 2.0 * (quat->x * quat->y + quat->w * quat->z);
	matrix[0][2] = 2.0 * (quat->x * quat->z - quat->w * quat->y);

	matrix[1][0] = 2.0 * (quat->x * quat->y - quat->w * quat->z);
	matrix[1][1] = quat->w * quat->w - quat->x * quat->x
				 + quat->y * quat->y - quat->z * quat->z;
	matrix[1][2] = 2.0 * (quat->y * quat->z + quat->w * quat->x);

	matrix[2][0] = 2.0 * (quat->x * quat->z + quat->w * quat->y);
	matrix[2][1] = 2.0 * (quat->y * quat->z - quat->w * quat->x);
	matrix[2][2] = quat->w * quat->w - quat->x * quat->x
				 - quat->y * quat->y + quat->z * quat->z;
}

int CheckQuatNorm(const TQuat *quat, _double deltaNorm)
{
	_double norm;
	if ((deltaNorm < 0) || (deltaNorm > 0.1)) return -1;
	norm = quat->w * quat->w + quat->x * quat->x + quat->y * quat->y + quat->z * quat->z;
	norm = sqrtm(norm);
	if (fabs(norm - 1.) <= deltaNorm) return 0;
	else return 1;
}

void AnglesToMatrixOrient(const TAngles *_orient, _tmatrix matrix)
{
	_double cos_al, cos_dl, cos_Az;
	_double sin_al, sin_dl, sin_Az;

	cos_al = cos(_orient->al);   sin_al = sin(_orient->al);
	cos_dl = cos(_orient->dl);   sin_dl = sin(_orient->dl);
	cos_Az = cos(_orient->Az);   sin_Az = sin(_orient->Az);

	matrix[0][0] = -cos_Az * sin_al - sin_Az * cos_al * sin_dl;
	matrix[0][1] =  cos_Az * cos_al - sin_Az * sin_al * sin_dl;
	matrix[0][2] =  sin_Az * cos_dl;

	matrix[1][0] =  sin_Az * sin_al - cos_Az * cos_al * sin_dl;
	matrix[1][1] = -sin_Az * cos_al - cos_Az * sin_al * sin_dl;
	matrix[1][2] =  cos_Az * cos_dl;

	matrix[2][0] =  cos_dl * cos_al;
	matrix[2][1] =  cos_dl * sin_al;
	matrix[2][2] =  sin_dl;
}

void MatrixOrientToAngles(const _tmatrix matrix, TAngles *orient)
{
	orient->dl = asin(matrix[2][2]);
	orient->al = atan2m(matrix[2][1], matrix[2][0]);
	orient->Az = atan2m(matrix[0][2], matrix[1][2]);

	if (orient->al < 0) orient->al += 2*PI;
	if (orient->Az < 0) orient->Az += 2*PI;
}

void AnglesToMatrixRotation(const TVectXYZ *ang, _tmatrix matrix)
{
//	double ax, ay, az;
	_double c1, c2, c3, delta;
	_double cdt, sdt, cdt_1;

// ax=VR[0]*dt_prog;      ay=VR[1]*dt_prog;     az=VR[2]*dt_prog;
//	ax = (double)ang[0]; ay = (double)ang[1]; az = (double)ang[2];
	delta = sqrtm(ang->x * ang->x + ang->y * ang->y + ang->z * ang->z);

	if (delta < 1.0e-7)
	{
		SingleMatrix(matrix);
		return;
	}

// 26.02.09   (c1,c2,c3)  right rotate vector (delta>0)
	c1  = - ang->x / delta;
	c2  = - ang->y / delta;
	c3  = - ang->z / delta;

	cdt = cos(delta);
	sdt = sin(delta);
	cdt_1 = 1.0 - cdt;

	matrix[0][0] = (cdt + cdt_1 * c1 * c1);
	matrix[0][1] =       (cdt_1 * c1 * c2 - sdt * c3);
	matrix[0][2] =       (cdt_1 * c1 * c3 + sdt * c2);

	matrix[1][0] =       (cdt_1 * c2 * c1 + sdt * c3);
	matrix[1][1] = (cdt + cdt_1 * c2 * c2);
	matrix[1][2] =       (cdt_1 * c2 * c3 - sdt * c1);

	matrix[2][0] =       (cdt_1 * c3 * c1 - sdt * c2);
	matrix[2][1] =       (cdt_1 * c3 * c2 + sdt * c1);
	matrix[2][2] = (cdt + cdt_1 * c3 * c3);
}

void MatrixRotationToAngles(const _tmatrix matrix, TVectXYZ *ang)
{
	double delta, sin_delta_2;
	delta = (matrix[0][0] + matrix[1][1] + matrix[2][2] - 1) / 2.;
	if (delta > 1.) delta = 1.;
	else if ((delta < -1.)) delta = -1.;

	if (fabs(delta - 1.0) < 1E-20)
	{
		ang->x = 0.; ang->y = 0.; ang->z = 0.;
	}
	else
	{
		delta = acosm(delta);
		sin_delta_2 = 2.0 * sin(delta);
		//08.07.09  (-delta !!!)
		ang->x = -delta * (matrix[2][1] - matrix[1][2]) / sin_delta_2;
		ang->y = -delta * (matrix[0][2] - matrix[2][0]) / sin_delta_2;
		ang->z = -delta * (matrix[1][0] - matrix[0][1]) / sin_delta_2;
	}
}

void GetRotationAngles(const _tmatrix matrix_prev,
					   const _tmatrix matrix_cur, TVectXYZ *omega)
{
		_tmatrix matrix_prevT, matrix_diff;
		double delta, sin_delta_2;
		int  i, j;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				matrix_prevT[i][j]=matrix_prev[j][i];
			}
		}

		MultMatrix(matrix_cur, matrix_prevT, matrix_diff);

		delta = (matrix_diff[0][0] + matrix_diff[1][1] + matrix_diff[2][2] - 1)/2.;
		if (delta > 1.) delta = 1.;

		if (fabs(delta - 1.0) < 1E-20)
		{
			omega->x = 0.;
			omega->y = 0.;
			omega->z = 0.;
        }
        else
		{
			delta = acos(delta);
			sin_delta_2 = 2.0 * sin(delta);
			//08.07.09  (-delta !!!)
			omega->x = -delta *(matrix_diff[2][1] - matrix_diff[1][2]) / sin_delta_2;
			omega->y = -delta *(matrix_diff[0][2] - matrix_diff[2][0]) / sin_delta_2;
			omega->z = -delta *(matrix_diff[1][0] - matrix_diff[0][1]) / sin_delta_2;
		}
}

void CorrectMatrix(const TVectLMN *Z1, const TVectLMN *Z2,
				   const _tmatrix matrix1,  _tmatrix matrix2)
{
	int i, j;
	TVectLMN norm;
	TVectXYZ rotate;
	_tmatrix  matrix12;
	_double ang, mod, ar[3], MD[3][3];

	norm.l = Z1->m * Z2->n - Z2->m * Z1->n;
	norm.m = Z1->n * Z2->l - Z2->n * Z1->l;
	norm.n = Z1->l * Z2->m - Z2->l * Z1->m;

	mod = sqrtm(norm.l * norm.l+norm.m * norm.m + norm.n * norm.n);
	ang = asinm(mod);

	if (ang < 1e-10)
	{
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				matrix2[i][j] = matrix1[i][j];
			}
		}
	}
	else
	{
		norm.l /= mod;
		norm.m /= mod;
		norm.n /= mod;

		rotate.x = ang * norm.l;
		rotate.y = ang * norm.m;
		rotate.z = ang * norm.n;

		AnglesToMatrixRotation(&rotate, matrix12);
		MultMatrix(matrix1, matrix12, matrix2);
	}
}

void MultQuat(const TQuat *Q1, const TQuat *Q2, TQuat *QR)
{
	QR->w = Q1->w * Q2->w - Q1->x * Q2->x - Q1->y * Q2->y - Q1->z * Q2->z;
	QR->x = Q1->w * Q2->x + Q1->x * Q2->w + Q1->y * Q2->z - Q1->z * Q2->y;
	QR->y = Q1->w * Q2->y + Q1->y * Q2->w + Q1->z * Q2->x - Q1->x * Q2->z;
	QR->z = Q1->w * Q2->z + Q1->z * Q2->w + Q1->x * Q2->y - Q1->y * Q2->x;
}

void InverseQuat(const TQuat *Q, TQuat *QR)
{
	QR->w =   Q->w;
	QR->x = - Q->x;
	QR->y = - Q->y;
	QR->z = - Q->z;
}

