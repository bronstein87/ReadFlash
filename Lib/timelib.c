#include "timelib.h"
#include "mathlib.h"

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

double DateTimeToUnixTime(struct _DateTime *stDateTime)
{
	double daysUnix1970;
	daysUnix1970 = DateTimeToDaysJ2000(stDateTime) + 2451545 - 2440587.5;
	return (daysUnix1970 *86400.);
}

long long int DateTimeToTimeAVU(struct _DateTime *stDateTime)
{
	double daysUnix1970;
	long long int timeAVU;

	daysUnix1970 = DateTimeToDaysJ2000(stDateTime) + 2451545 - 2440587.5;
	timeAVU  = (long long int)((double) daysUnix1970 * 86400. * 1000000.);

	return timeAVU;
}

double StarsTime(struct _DateTime *curDateTime, short zone)
{
	double l1, F, D, W, Nf, Ne, e0;
	double d, M, era, era_2, era_3, Ssr_rad, d_05;
	int i, n;

	d  = DateTimeToDaysJ2000(curDateTime);
	d -= (zone/24.);  // юлианская дата на гринвиче - минус 4 часа
	era  = d / 36525;
	era_2 = era   * era;
	era_3 = era_2 * era;
	d_05 = d - 0.5;
	M = d_05 - (int)d_05;
//  M = (date.Hour - 4+date.Min/60.+date.Sec/3600.+date.mSec/3600000.) / 24.;

	Ssr_rad = 1.7533685592 + 0.0172027918051 * d + 6.2831853072 * M
			+ 6.7707139e-6 * era_2 - 4.50876e-10 * era_3;

//l=2.355548393 + 8328.69142288 *tay +1.517952*pow10(-4)*tay*tay +3.103*pow10(-7)*pow(tay,3);
	l1 =  6.24003594 +  628.30195602 * era - 2.7974e-6 *era_2 - 5.82e-8 * era_3;
	F  =  1.62790193 + 8433.46615831 * era - 6.42717e-5*era_2 + 5.33e-8 * era_3;
	D  =  5.19846951 + 7771.37714617 * era - 3.34085e-5*era_2 + 9.21e-8 * era_3;
	W  =  2.182438624 - 33.757045936 * era + 3.61429e-5*era_2 + 3.88e-8 * era_3;
	Nf = -0.83386e-4 * sin(W) + 0.9997e-6 * sin(2 * W) - 0.63932e-5 * sin(2 * (F - D + W))
		 +0.6913e-6 * sin(l1) - 0.11024e-5 * sin(2*(F + W));
	Ne = 0.44615e-4 * cos(W) + 0.27809e-5 * cos(2*(F - D + W)) + 0.474e-6 * cos(2 * (F + W));
	e0 = 0.4090928042 - 0.2269655e-3 * era - 0.29e-8 * era_2 + 0.88e-8 * era_3;

//учитываем нутацию в прямом восхождении
	Ssr_rad += Nf * cos(e0 + Ne);

//приводим звездное время к "нормальному" виду - сделать без цикла
//	for (i = 1; Ssr_rad > 2 * PI; i++) {
//		Ssr_rad = Ssr_rad - 2 * PI;
//	}
	n = (int) Ssr_rad / (2 * PI);
	Ssr_rad = Ssr_rad - 2 * PI * n;

	return Ssr_rad;
}


