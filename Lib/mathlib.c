#include "mathlib.h"

_double sqrtm(_double _x)
{
	if (_x < 1e-20) return 0.;
	else return sqrt(_x);
}

_double acosm(_double _x)
{
	if (_x > 1.) _x = 1.;
	else if (_x < -1.) _x = -1.;

	return acos(_x);
}

_double asinm(_double _x)
{
	if (_x > 1.) _x = 1.;
	else if (_x < -1.) _x = -1.;

	return asin(_x);
}

_double atan2m(_double _y, _double _x)
{
  _double ang;

  if (fabs(_x) > 1e-10) ang = atan2(_y, _x);
  else
  {
	if (_y > 0) ang = PI/2.;
	else ang = -PI/2.;
  }
  return ang;
}
