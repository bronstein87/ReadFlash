
#ifndef ReadFlash
#define ReadFlash

#include <stdio.h>
#include <string>
#include <vcl.h>
#include "MathM.h"
#include "InfoCadrFormat.h"

#define INT8U unsigned char
#define INT16U unsigned short
#define INT32U unsigned long
#define INT8S  char
#define INT16S short
#define INT32S long
#define FP32 float

#define MaxObj 200        // Max.num. objects with noise
#define MaxDetHO 8       // Max.num. objects for Deter in HO/TO
#define MaxOrnt 30       // Max.num. objects for SLEZH
#define MaxBlock 8
#define MaxStarCad  100
#define MaxPix 18432

#define SECTOR_MARKER 		0x55550000
#define HO_MARKER 			0xAAAA0000
#define SL_MARKER 			0xAAAA1111
#define PIX_0_MARKER 		0xAAAA2222
#define PIX_1_MARKER 		0xAAAA3333
#define ALL_REG_MARKER  	0xAAAA4444
#define SINGLE_REG_MARKER 	0xAAAA5555
#define FRAG_MARKER 		0xAAAA6666
#define COUNT_REG  			128

struct DataPixHeader
{
	INT16U SerNum;
	INT16U CntRecord;
	INT32U  T;
	INT32U  Tbshv;
	INT32U  Tpr_sec;
	INT32U  Tpr_msec;
	INT16U  StarExp;
	INT16U  NumPix;
};

struct DataPixFlt
{
  INT16U Y, X, BR;
};

struct DataFragHeader
{
	INT16U SerNum;
	INT16U CntRecord;
	INT32U  Tpr_sec;
	INT32U  Tpr_msec;
	INT32U  NumPix;
};

struct DataSingleReg
{
	INT16U SerNum;
	INT16U CntRecord;
	INT32U  T;
	INT32U  Tbshv;
	INT32U  Tpr_sec;
	INT32U  Tpr_msec;
	INT16U  CntErr;
	INT8U   Nreg;
	INT8U   RegVal;
};

struct DataNO_st
{
	INT16U SerNum; //new
	INT16U CntRecord; //new
	INT32U  T;
	INT32U  Tbshv;
	INT32U  Tpr_sec;  //new
	INT32U  Tpr_msec; //new
	FP32    Temp;
	INT16U  StarExp;
	INT16U  res_stat; //НЕ меняем местами
	INT16U  NumApr;
	INT16U  NumStore; //НЕ меняем местами
	FP32	QuatBoart[4];
	INT16U	NumPixFlt[2]; //!!!!!!!!!!!
	INT16U  NumL[2];
};

struct DataNO_fin
{
	INT16U	NumLoc[2];
	FP32	RsLoc[MaxDetHO][4];
	INT16U	NumDet;
	INT16S	dxmaxH;  //НЕ меняем местами
	INT16S	dymaxH;
	INT16U	maxH;    //НЕ меняем местами


	INT16U	MasRes [MaxDetHO][2];
	FP32	Coord[MaxDetHO][4];
	FP32	D_cos[MaxDetHO][4];

	FP32	Dx[MaxDetHO];
	FP32	Dy[MaxDetHO];

	FP32	m_cur;
	FP32	M_ornt[3][3];
	FP32	Quat[4];
	FP32	TimeExp;
	FP32	Wop[3];
	FP32	EPOCH;
	FP32	Vline[3];
};

struct DataNO
{
	INT16U SerNum; //new
	INT16U CntRecord; //new
	INT32U  T;
	INT32U  Tbshv;
	INT32U  Tpr_sec;  //new
	INT32U  Tpr_msec; //new
	FP32    Temp;
	INT16U  StarExp;
	INT16U  res_stat; //НЕ меняем местами
	INT16U	NumApr;
	INT16U	NumStore; //НЕ меняем местами


	FP32	QuatBoart[4];
	INT16U	NumPixFlt[2]; //!!!!!!!!!!!
	INT16U	NumL[2];
	FP32	RsLocT[2][MaxObj][4];
	INT16U	NumLoc[2];
	FP32	RsLoc[MaxDetHO][4];

	INT16U	NumDet;
	INT16S	dxmaxH;  //НЕ меняем местами
	INT16S	dymaxH;
	INT16U	maxH;    //НЕ меняем местами

	INT16U	MasRes [MaxDetHO][2];
	FP32	Coord[MaxDetHO][4];
	FP32	D_cos[MaxDetHO][4];

	FP32	Dx[MaxDetHO];
	FP32	Dy[MaxDetHO];

	FP32	m_cur;
	FP32	M_ornt[3][3];
	FP32	Quat[4];
	FP32	TimeExp;
	FP32	Wop[3];
	FP32	EPOCH;
	FP32	Vline[3];
};

struct DataSLEZH_st
{
	INT16U SerNum; //new
	INT16U CntRecord; //new
	INT32U  T;
	INT32U  Tbshv;
	INT32U  Tpr_sec;  //new
	INT32U  Tpr_msec; //new
	FP32	Temp;
	FP32    mean, sigma;

	INT16U	StarExp;
	INT16U	res_stat; //НЕ меняем местами

	FP32	M_prog[3][3];

	INT16U	NumSec;
	INT16U	NumProgFrag; //НЕ меняем местами

	FP32	XYcProg[MaxStarCad][2];
	FP32	XYc [MaxOrnt][4];

	FP32	MeanFrag[MaxOrnt][2];
	INT16U	ThFrag[MaxOrnt];

	INT16U	NumFrag;
	INT16U	ThMax;  //НЕ меняем местами

	INT16U	CountBlock;
	INT16U	EndBufFrag;   //НЕ меняем местами

	INT16U	TabTakeAway[8][2];
	INT16U	PfragXY0[MaxOrnt][2];
	INT16U	PfragXY[MaxOrnt][2];
	INT16U	ZipXY[MaxOrnt][2];
	INT16U	FragYX[MaxOrnt][2];
	INT16U	YendLoc[MaxOrnt];
	INT16U	ObjFrag[MaxOrnt];

	INT16U	NumLoc;
	INT16U	NumStore; //НЕ меняем местами
};

struct DataSLEZH_fin
{
	FP32	RsLoc[MaxOrnt][4];

	INT16U	NumDet;
	INT16S	dxmaxH; //НЕ меняем местами

	INT16S	dymaxH;
	INT16U	maxH;  //НЕ меняем местами

	INT16U	MasRes [MaxOrnt][2];
	FP32	Coord[MaxOrnt][4];
	FP32	D_cos[MaxOrnt][4];
	FP32	Dx[MaxOrnt];
	FP32	Dy[MaxOrnt];
	FP32	m_cur;
	FP32	M_ornt[3][3];
	FP32	Quat[4];
	FP32	TimeExp;
	FP32	Wop[3];
	FP32	EPOCH;
	FP32	Vline[3];
};

struct DataSLEZH
{
	INT16U SerNum; //new
	INT16U CntRecord; //new
	INT32U  T;
	INT32U  Tbshv;
	INT32U  Tpr_sec;  //new
	INT32U  Tpr_msec; //new
	FP32	Temp;
	FP32    mean, sigma;

	INT16U	StarExp;
	INT16U	res_stat; //НЕ меняем местами

	FP32	M_prog[3][3];

	INT16U	NumSec;
	INT16U	NumProgFrag; //НЕ меняем местами

	FP32	XYcProg[MaxStarCad][2];
	FP32	XYc [MaxOrnt][4];

	FP32	MeanFrag[MaxOrnt][2];
	INT16U	ThFrag[MaxOrnt];

	INT16U	NumFrag;
	INT16U	ThMax;  //НЕ меняем местами

	INT16U	CountBlock;
	INT16U	EndBufFrag;   //НЕ меняем местами

	INT16U	TabTakeAway[8][2];
	INT16U	PfragXY0[MaxOrnt][2];
	INT16U	PfragXY[MaxOrnt][2];
	INT16U	ZipXY[MaxOrnt][2];
	INT16U	FragYX[MaxOrnt][2];
	INT16U	YendLoc[MaxOrnt];
	INT16U	ObjFrag[MaxOrnt];

	INT16U	NumLoc;
	INT16U	NumStore; //НЕ меняем местами

	FP32	RsLocT[MaxObj][4];
	FP32	RsLoc[MaxOrnt][4];

	INT16U	NumDet;
	INT16S	dxmaxH; //НЕ меняем местами

	INT16S	dymaxH;
	INT16U	maxH;  //НЕ меняем местами

	INT16U	MasRes [MaxOrnt][2];
	FP32	Coord[MaxOrnt][4];
	FP32	D_cos[MaxOrnt][4];
	FP32	Dx[MaxOrnt];
	FP32	Dy[MaxOrnt];
	FP32	m_cur;
	FP32	M_ornt[3][3];
	FP32	Quat[4];
	FP32	TimeExp;
	FP32	Wop[3];
	FP32	EPOCH;
	FP32	Vline[3];
};

float GetFloat(float ptr);
int   GetInt(int ptr);

void ChangeWordSingleReg(struct DataSingleReg &data);
void ChangeWordPix(struct DataPixHeader &data);
void ChangeWordFrag(struct DataFragHeader &data);
void ChangeWordNO(struct DataNO &data);
void ChangeWordSLEZH(struct DataSLEZH &data);

void ConvertDataSLEZH(struct DataSLEZH data, struct CadrInfo &mCadr);
void ConvertDataNO(struct DataNO data, struct CadrInfo &mCadr, int NC);

void PrintReg(FILE *ftxt, unsigned char *mas);
void PrintSingleReg(FILE *ftxt, struct DataSingleReg data);
void PrintDataNO(FILE *ftxt,struct DataNO data);
void PrintDataSLEZH(FILE *ftxt,struct DataSLEZH data);
void PrintDataPix(FILE *ftxt,struct DataPixHeader data, int NumCadr);
void PrintDataFrag(FILE *ftxt,struct DataFragHeader data);



#endif
