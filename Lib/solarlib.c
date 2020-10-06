#include "solarlib.h"

void SunVector(double JD_DEV, double mOr[3][3], double *pSunI,  double *pSunD)
{
int i,j;
double Ts, Ls, Ms, eS, Es, Vs, Qs, Ekl, AlphaS, DeltaS, QsVid, Om;

//	   Ts=(JD_DEV-2415020.0)/36525.;
	   Ts=JD_DEV+1;
	   Ls=279.69668+36000.76892*Ts+0.0003025*Ts*Ts;
//Ls=Ls-(int)(Ls/360.)*360.;
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

	   AlphaS= atan2m(cos(Ekl)*sin(QsVid), cos(QsVid));
	   DeltaS= asinm(sin(Ekl)*sin(QsVid));

	   if (AlphaS<0) AlphaS+=2*PI;

	   pSunI[0]=cos(AlphaS)*cos(DeltaS);
	   pSunI[1]=sin(AlphaS)*cos(DeltaS);
	   pSunI[2]=sin(DeltaS);

	   for (i =0; i<3; i++) {
		   pSunD[i]=0;
		   for (j =0; j< 3; j++){
			   pSunD[i] += mOr[i][j] * pSunI[j];
		   }
	   }
}

void MoonVector(double JD_DEV, double mOr[3][3], double *pMoonI,  double *pMoonD)
{
int i,j;
double Ts, L1, Ms, M1, D, F, Om, e, Lam, B, Ekl, AlphaM, DeltaM;

//	   Ts=(JD_DEV-2415020.0)/36525.;
	   Ts=JD_DEV+1;
//знак при Ts^2 должен быть минус, а не плюс
	   L1=270.434164+481267.8831*Ts-0.001133*Ts*Ts+0.0000019*Ts*Ts*Ts;
	   L1/=360.; 	   L1-=(int)L1;	   L1*=360.;

	   Ms=358.47583+35999.04975*Ts-0.000150*Ts*Ts-0.0000033*Ts*Ts*Ts;
	   Ms/=360.;	   Ms-=(int)Ms;	   Ms*=2*M_PI;

	   M1=296.104608+477198.8492*Ts+0.009192*Ts*Ts+0.0000144*Ts*Ts*Ts;
	   M1/=360.;	   M1-=(int)M1;	   M1*=2*M_PI;

	   D=350.737486+445267.1142*Ts-0.001436*Ts*Ts+0.0000019*Ts*Ts*Ts;
	   D/=360.;	   D-=(int)D;	   D*=2*M_PI;

	   F=11.250889+483202.0251*Ts-0.003211*Ts*Ts-0.000003*Ts*Ts*Ts;
	   F/=360.;	   F-=(int)F;	   F*=2*M_PI;

	   Om=259.183275-1934.1420*Ts+0.002078*Ts*Ts+0.0000022*Ts*Ts*Ts;
	   Om/=360.;	   Om-=(int)Om;	   Om*=2*M_PI;

	   e=1-0.002495*Ts-0.00000752*Ts*Ts;

	   Lam=L1+6.288750*sin(M1)+1.274018*sin(2*D-M1)+0.658309*sin(2*D)
				 +0.213616*sin(2*M1)-e*0.185596*sin(Ms)-0.114336*sin(2*F)
				 +0.058793*sin(2*D-2*M1)+e*0.057212*sin(2*D-Ms-M1)+0.053320*sin(2*D+M1)
				 +e*0.045874*sin(2*D-Ms)+e*0.041024*sin(M1-Ms)-0.034718*sin(D)-e*0.030465*sin(Ms+M1)
				 +0.015326*sin(2*D-2*F)-0.012528*sin(2*F+M1)-0.01098*sin(2*F-M1)+0.010674*sin(4*D-M1)+0.010034*sin(3*M1);

	   Lam/=360;    Lam-=(int)Lam;	   Lam*=2*M_PI;

	   B=5.128189*sin(F)+0.280606*sin(M1+F)+0.277693*sin(M1-F)+0.173238*sin(2*D-F)+0.055413*sin(2*D+F-M1)+0.046272*sin(2*D-F-M1)
			  +0.032573*sin(2*D+F)+0.017198*sin(2*M1+F)+0.009267*sin(2*D+M1-F)+0.008823*sin(2*M1-F);
	   B/=360;    B-=(int)B;	   B*=2*M_PI;

	   Ekl=23.452294-0.0130125*Ts-0.00000164*Ts*Ts+0.000000503*Ts*Ts*Ts;
	   Ekl*=PI/180.;

	   AlphaM = atan2m((sin(Lam)*cos(Ekl)-tan(B)*sin(Ekl)), cos(Lam));
	   DeltaM = asinm(sin(B)*cos(Ekl)+cos(B)*sin(Ekl)*sin(Lam));

	   pMoonI[0]=cos(AlphaM) * cos(DeltaM);
	   pMoonI[1]=sin(AlphaM) * cos(DeltaM);
	   pMoonI[2]=sin(DeltaM);

	   for (i =0; i<3; i++) {
		   pMoonD[i] = 0;
		   for (j =0; j< 3; j++){
			   pMoonD[i] += mOr[i][j] * pMoonI[i];
		   }
	   }
}
