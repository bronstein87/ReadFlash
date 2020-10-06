// ---------------------------------------------------------------------------

#ifndef ParseProtocolH
#define ParseProtocolH

#include "InfoCadrFormat.h"
#include "ReadFlashFormat.h"
#include <system.hpp>
#include "AddString.h"
#include "SimplePlotter.h"
#include "iki_img.h"
#include <DateUtils.hpp>
#include <System.IOUtils.hpp>
#include <math.h>
#include <algorithm>
#include "MathM.h"
#include <memory>

#define MAX_STAT 	   16
#define MAX_STAT_M2    15
#define MAX_OBJ_DTMI   15
#define MAX_OBJ_MLOC   32
#define MAX_OBJ_BOKZM  30
#define MAX_OBJ_METEOR 16
#define MAX_WINDOW 	   16

using namespace std;
using namespace add_string;

double clkBokz = 1.;

namespace parse_prot {

	#pragma pack(push, 1)

	struct stLocal {
		float x, y;
		float bright;
		short size;
	};

	struct stLocalFloat {
		float x, y;
		float bright;
		float size;
	};

	struct GYRO {
		unsigned long timeGyro;
		double omegaGyro[3], omegaSat[3];
		double quatGyro[4], quatSat[4];
	};

	struct ERR_BOKZ {
		unsigned int timeBoard;
		unsigned int clkPowerOff;
		unsigned int clkError;
		unsigned int timeFirst;
		float Qfirst[4];
		unsigned int timeLast;
		float Qlast[4];
	};

	struct QBOKZ {
		unsigned long timeBokz1;
		double quatBokz1[4], OZ1[3];
		unsigned long timeBokz2;
		double quatBokz2[4], OZ2[3];
	};

	struct MSHI_M2 {
		unsigned short serialNumber;
		unsigned short status1, status2;
		unsigned long timeBOKZ;
		float OZ[3], Qornt[4];
		float W[3];
		short nLocalObj, nDeterObj;
		unsigned short thMax, m_cur;
	};

	struct TMI1_M2 {
		unsigned long timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, post;
		float Foc, Xg, Yg;
		unsigned short timeExp, Mean, Sigma;
		unsigned short countDefect, countStar;
		unsigned short reserved[8];
		unsigned short CRC, Date, Version, Build;
	};

	struct TMI2_M2 {
		unsigned long timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, post, timeExp;
		unsigned short cntCommandWord, cntCallNO, cntNOtoSLEZH;
		unsigned short cntCallTO, cntTOtoSLEZH;
		unsigned int cntSLEZH;
		unsigned short cntStatOrient[MAX_STAT_M2];
	};

	struct DTMI_M2 {
		unsigned long timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, post, timeExp;
		short nLocalObj[2], nLocalAll[2];
		unsigned short meanCadr[2], maxHist;
		short maxHistX, maxHistY;
		unsigned short nStoreObj, nDeterObj, nWindows, nSec;
		unsigned long timeQuatLast;
		float quatLast[4], m_cur;
		float omega[3], Vline[3];
		float quatBoard[4], Epoch;
		struct stLocalFloat LocalList[2][15], ResultList[12];
		unsigned char nObjectWindow[16];
	};
	struct MLOC_M2 {
		unsigned long timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, timeExp;
		short nLocalObj1, nLocalObj2;
		unsigned short meanCadr;
		struct stLocal LocalList[36];
	};

	struct DTMI_M60K {
		unsigned long timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, post, timeExp;
		short nLocalObj[2], nLocalAll[2];
		unsigned short meanCadr[2], maxHist;
		short maxHistX, maxHistY;
		unsigned short nStoreObj, nDeterObj, nWindows, nSec;
		unsigned long timeMatrixLast;
		float matrixLast[3][3], m_cur;
		float deltaT, omega[3], Vline[3];
		float matrixBoard[3][3], Epoch;
		struct stLocal LocalList1[15];
		struct stLocalFloat LocalList2[15];
		struct stLocal ResultList[14];
		unsigned char nObjectWindow[14];
	};

	struct SINCHRO {
		unsigned int countClk;
        float curEpoch;
	};

	struct MSHI {
		unsigned short status1, status2;
		unsigned int timeBOKZ;
		float OZ[3], Qornt[4];
		float W[3];
	};

	struct SHTMI1 {
		// string timeBOKZ;
		unsigned int timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, post;
		float Foc, Xg, Yg;
		unsigned short timeExp, Mean, Sigma, countDefect;
		unsigned short reserved[11];
		unsigned short CRC, Date, Version;
	};

	struct SHTMI2 {
		// string timeBOKZ;
		unsigned int timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, post, timeExp;
		unsigned short cntCommandWord, cntCallNO, cntNOtoSLEZH;
		unsigned short cntCallTO, cntTOtoSLEZH;
		unsigned int cntSLEZH;
		unsigned short cntStatOrient[MAX_STAT];
	};

	struct DTMI {
		// string timeBOKZ;
		unsigned int timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, timeExp;
		unsigned short nLocalObj, nDeterObj, nWindows, epsillon;
		float dTimeBOKZ, LocalList[MAX_OBJ_DTMI][4];
		float quatBoard[4], omega[3], centrWindow[MAX_WINDOW][2];
		unsigned short levelWindow[MAX_WINDOW], nObjectWindow[MAX_WINDOW];
		unsigned int timeQuatLast;
		float quatLast[4], Epoch;
		unsigned short nLocal[2], maxHist;
		unsigned short maxHistX, maxHistY;
		unsigned short test_short;
		short Reserved[10];
	};

	struct LOC {
		// string timeBOKZ;
		unsigned int timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, timeExp;
		unsigned short nLocalObj, nFixedObj;
		float MeanC, SigmaC;
		float LocalList[32][4];
		unsigned short nPix, Reserved;
	};

	struct MSHI_BOKZM {
		unsigned short status1, status2;
		float timeBOKZ;
		float Mornt[3][3];
	};

	struct MSHIA_BOKZM {
		unsigned short status1, status2;
		float timeBOKZ;
		float Mornt[3][3];
		unsigned short serialNumber;
		unsigned short numSec, nFrag;
		unsigned short nLocalObj, nDeterObj;
		unsigned short thMax, mxy, timeExp;
        float deltaT;
	};

	struct DTMI_BOKZM {
		float timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber;
		float Foc, Xg, Yg;
		unsigned short timeExp;
		unsigned short nLocalObj, nDeterObj;
		float LocalList[MAX_OBJ_BOKZM][4];
	};

	struct SHTMI1_BOKZM {
		float timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, post;
		float Foc, Xg, Yg;
		unsigned short timeExp, Mean, Sigma, countDefect;
		unsigned short reserved[11];
		unsigned short Date, verXCF, verProg;
	};

	struct SHTMI2_BOKZM {
		float timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, post;
		float Foc, Xg, Yg;
		unsigned short timeExp, Mean, Sigma, countDefect;
		unsigned short cntCallNO, cntCallTO;
		unsigned short cntStatOrient[12];
	};

	struct DTMIA_BOKZM {
		float timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber;
		unsigned short timeExp;
		unsigned short nLocalObj, nDeterObj;
		unsigned short nWindow, eps;
		float deltaT;
		stLocal LocalList[15];
		float Mapr[3][3];
		float XYc[15][4];
		unsigned short ThFrag[15], ObjFrag[15];
		float Tpsk, Mpsk[3][3], Epoch;
		short NumL[2];
		unsigned short MaxH;
		short dxmaxH, dymaxH;
		float Wop[3];
		unsigned short NumLocSl, reserved[11];

	};

	struct MSI_BOKZM {
		float timeBOKZ;
		float timeBetwCadrs;
		short timeExp;
		short SKOCadrAfterFilter;
		short thresholdLoc;
		short nLocalObj;
		char  nDeterObj;
		char  nFrag;
		short numSec;
		float omega;
	};

	struct TLOCBOKZM {
		float LocalList[MAX_OBJ_MLOC][4];
	};

	struct TFRAGBOKZM {
		float fragList [MAX_OBJ_MLOC][2];
	};

	struct DTMI_METEOR {
		unsigned long timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, timeExp;
		float Foc, Xg, Yg;
		unsigned short nLocalObj, nDeterObj;
		float LocalList[MAX_OBJ_METEOR][4];
	};

	struct TAistQuat {
		unsigned short status1, status2;
		unsigned int timeFirst;
		float Qfirst[4];
		unsigned int timeLast;
		float Qlast[4], Qssk[4];
		int clkPowerOff;
	};

	struct TAistMshi {
		unsigned short status1, status2;
		unsigned int time_sec;
		float OZ[3], Qornt[4], W[3];
		unsigned short serial, NumSec;
		unsigned short NumStar, NumFrag;
		unsigned short NumLoc, NumDet;
		unsigned short ThMax, mxy;
	};

	struct TAistShtmi1 {
		unsigned int time_sec;
		unsigned short status1, status2;
		unsigned short serial, post;
		float Foc, Xg, Yg;
		unsigned short timeExp;
		unsigned short Mean, Sigma;
		unsigned short Reserved[10];
		unsigned short ConstCS, ProgCS;
		unsigned short X_ident, Date, Version;
	};

	struct TAistShtmi2 {
		unsigned int time_sec;
		unsigned short status1, status2;
		unsigned short serial, post, timeExp;
		unsigned short cntCommandWord, cntCallNO, cntNOtoSLEZH;
		unsigned short cntCallTO, cntTOtoSLEZH;
		unsigned int cntSLEZH;
		unsigned short cntStatOrient[MAX_STAT];
	};

	struct TMshi_2V {
		unsigned short status1, status2;
		unsigned int time_sec;
		unsigned short time_ms;
		float OZ[3], Qornt[4], W[3];
		unsigned short serial, Tcmv;
		unsigned char  NumStar, NumFrag;
		unsigned short NumLoc, NumDet;
		unsigned short ThMax, mxy;
	};

	struct TShtmi1_2V {
		unsigned short nAr;
		unsigned int time_sec;
		unsigned short time_ms;
		unsigned short status1, status2;
		unsigned short serial, post;
		float Foc, Xg, Yg;
		unsigned short timeExp;
		unsigned short Mean, Sigma;
		unsigned short statKP, cntKP;
		unsigned short Tcmv, Reserved[5];
		unsigned short CatCS[2], ConstCS[2], ProgCS[2];
		unsigned short Version;
	};

	struct TShtmi2_2V {
		unsigned short nAr;
		unsigned int time_sec;
		unsigned short time_ms;
		unsigned short status1, status2;
		unsigned short serial, post, timeExp;
		unsigned short cntCommandWord, cntCallNO, cntNOtoSLEZH;
		unsigned short cntCallTO, cntTOtoSLEZH;
		unsigned int cntSLEZH;
		unsigned short cntStatOrient[MAX_STAT];
	};

	struct TDtmi_2V {
//ДТМИ1
		unsigned int time_sec;
		unsigned short time_ms;
		unsigned short status1, status2;
		unsigned short serial, post, Tcmv;
		unsigned short cntRegErr, timeExp;
		unsigned short NumAll[2];
		unsigned char  NumL[2];
		unsigned short NumLoc;
		unsigned char  NumStore, NumDet;
		unsigned char  NumStar, NumFrag;
		short MaxH;
		char  dxMaxH, dyMaxH;
		unsigned char  numSec, eps;
		unsigned short deltaT;
		unsigned int timeLast_sec;
		unsigned short timeLast_ms;
		float Qlast[4];
//ДТМИ2
		unsigned short m_cur;
		float Wop[3];
		float Vline[3];
		float Qapr[4], era;
//ДТМИ3-ДТМИ9
		unsigned short pixFilterCnt[2], pixCurrent;
		struct DataLocObj RsLocT[2][15];
//ДТМИ10
		unsigned short XYc[15][2];
//ДТМИ11
		unsigned short MeanFrag[15][2];
//ДТМИ12
		unsigned short ThFrag[15];
		unsigned char ObjFrag[8];
		unsigned char MultXY[8];
		unsigned char ZipXY[14];
	};

	struct TMloc_2V {
		unsigned int time_sec;
		unsigned short time_ms;
		unsigned short status1, status2;
		unsigned short serial, timeExp;
		unsigned short NumLoc, NumObj;
		unsigned short Reserved[4];
		float ArrLoc[32][4];
		unsigned short NumPix;
	};

	struct TBoardArray {
		unsigned short cmdWord, ansWord;
		unsigned short bufMIL[32];
		unsigned short statTmi, crcBoard, crcCalc;
	};

	struct TMshi_2R_old {
		unsigned short status1, status2;
		unsigned int time_sec;
		unsigned short time_ms;
		float OZ[3], Qornt[4], W[3];
		unsigned short serial;
		short Tcmv;
		unsigned char  NumStar, NumFrag;
		unsigned short NumLoc, NumDet;
		unsigned short ThMax, mxy;
	};

	struct TMshi_2R {
		unsigned short status1, status2;
		unsigned int time_sec;
		unsigned short time_ms;
		float OZ[3], Qornt[4], W[3];
		unsigned short serial;
		short Tcmv;
		unsigned char NumFrag, NumLoc;
		unsigned char NumDet, LightMv5;
		unsigned short Mean, ThMax, mxy;
	};

	struct TShtmi1_2R {
		unsigned short nAr;
		unsigned int time_sec;
		unsigned short time_ms;
		unsigned short status1, status2;
		unsigned short serial, post;
		float Foc, Xg, Yg;
		unsigned short timeExp;
		unsigned short Mean, Sigma;
		unsigned short statKP, cntKP, cntUPN;
		unsigned short Tcmv, Reserved[2], optSerial, optConstCS;
		unsigned short CatCS[2], ConstCS[2], ProgCS[2];
		unsigned short Version;
	};

	struct TShtmi2_2R {
		unsigned short nAr;
		unsigned int time_sec;
		unsigned short time_ms;
		unsigned short status1, status2;
		unsigned short serial, post, timeExp;
		unsigned short cntCommandWord, cntCallNO, cntNOtoSLEZH;
		unsigned short cntCallTO, cntTOtoSLEZH;
		unsigned int cntSLEZH;
		unsigned short cntStatOrient[MAX_STAT];
	};

	struct TMloc_2R {
		unsigned int time_sec;
		unsigned short time_ms;
		unsigned short status1, status2;
		unsigned short serial, timeExp;
		unsigned short NumLoc, NumObj;
		unsigned short Reserved[2];
		float sigma;
		stLocal RsLoc[36];
		unsigned short NumPix, Th;
	};

	struct TDtmi_2R_old {
//ДТМИ1
		unsigned int time_sec;
		unsigned short time_ms;
		unsigned short status1, status2;
		unsigned short serial, post;
		short Tcmv;
		unsigned short cntRegErr, timeExp;
		unsigned short NumAll[2];
		unsigned char  NumL[2];
		unsigned short NumLoc;
		unsigned char  NumStore, NumDet;
		unsigned char  NumStar, NumFrag;
		short MaxH;
		char  dxMaxH, dyMaxH;
		unsigned char  numSec, eps;
		unsigned short deltaT;
		unsigned int timeLast_sec;
		unsigned short timeLast_ms;
		float Qlast[4];
//ДТМИ2
		unsigned short m_cur;
		float Wop[3];
		float Vline[3];
		float Qapr[4], era;
		unsigned char brightHist[16];
//ДТМИ3-ДТМИ9
		unsigned short pixFilterCnt[2], pixCurrent;
		stLocal RsLocT[2][15];
//ДТМИ10
		unsigned short XYc[15][2];
//ДТМИ11
		unsigned short MeanFrag[15][2];
//ДТМИ12
		unsigned short ThFrag[15];
		unsigned char ObjFrag[8];
		unsigned char MultXY[8];
		unsigned char ZipXY[14];
	};

	struct TDtmi_2R {
//ДТМИ1
		unsigned int time_sec;
		unsigned short time_ms;
		unsigned short status1, status2;
		unsigned short serial, post;
		short Tcmv;
		unsigned short cntRegErr, timeExp;
//		unsigned short NumAll[2];   //before 30.03.2020
        unsigned short Mean, Sigma; //after 30.03.2020
		unsigned char  NumL[2];
		unsigned short NumLoc;
		unsigned char  NumStore, NumDet;
		unsigned char  NumStar, NumFrag;
		short MaxH;
		char  dxMaxH, dyMaxH;
		unsigned char  numSec, eps;
		unsigned short deltaT;
		unsigned int timeLast_sec;
		unsigned short timeLast_ms;
		float Qlast[4];
//ДТМИ2
		unsigned short m_cur;
		float Wop[3];
		float Vline[3];
		float Qapr[4], era;
		unsigned char brightHist[16];
//ДТМИ3-ДТМИ9
		unsigned short pixFilterCnt[2], pixCurrent;
		stLocal RsLocT[2][15];
//ДТМИ10
		unsigned short XYc[15][2];
//ДТМИ11
		unsigned short MeanFrag[15][2];
//ДТМИ12
		unsigned short ThFrag[15];
		unsigned char ObjFrag[8];
		unsigned char MultXY[8];
		unsigned char ZipXY[14];
	};

	struct TMshi_MR {
		unsigned short status1, status2;
		unsigned int time_sec;
		float OZ[3], Qornt[4], W[3];
		short Tcmv;
		unsigned char NumFrag, NumLoc;
		unsigned char NumDet, LigntMv5;
        unsigned short Mean, Sigma;
//		unsigned short NumStar, NumFrag;
//		unsigned short NumLoc, NumDet;
		unsigned short ThMax, mxy, serial;
	};

	struct TShtmi1_MR {
		unsigned short code1, code2;
		unsigned int time_sec;
		unsigned short status1, status2;
		unsigned short serial, post;
		float Foc, Xg, Yg;
		unsigned short timeExp;
		unsigned short Mean, Sigma;
		unsigned short statKP, cntKP, cntUPN;
		short Tcmv, Tsens[4];
		unsigned short CatCS[2], ConstCS[2], ProgCS[2];
		unsigned short Version;
	};

	struct TShtmi2_MR {
		unsigned short code1, code2;
		unsigned int time_sec;
		unsigned short status1, status2;
		unsigned short serial, post, timeExp;
		unsigned short cntCommandWord, cntCallNO, cntNOtoSLEZH;
		unsigned short cntCallTO, cntTOtoSLEZH;
		unsigned int cntSLEZH;
		unsigned short cntStatOrient[MAX_STAT];
	};

	struct TDtmi_MR {
//ДТМИ1
		unsigned int time_sec;
		unsigned short status1, status2;
		unsigned short serial, post;
		short Tcmv;
		unsigned short Mean;
		float Sigma;
		unsigned short timeExp;
		unsigned char nLocalObj[2];
		unsigned short NumLoc; //PixCount in NO
		unsigned char  NumStore, NumDet;
		unsigned char  NumStar, NumFrag;
		short MaxH;
		char  dxMaxH, dyMaxH;
		unsigned char  numSec, eps;
		unsigned short deltaT, m_cur;
		unsigned int timeLast_sec;
		float Qlast[4];
//ДТМИ2
		float Wop[3];
		float Vline[3];
		float Qapr[4], era;
		unsigned char brightHist[16];
//ДТМИ3-ДТМИ9
		float RsLoc[2][15][3];
		short RsLocSize[2][15];
//ДТМИ10
		unsigned short XYc[15][2];
//ДТМИ11
		unsigned short MeanFrag[15][2];
//ДТМИ12
		unsigned short ThFrag[15];
		unsigned char ObjFrag[8];
		unsigned char MultXY[8];
		unsigned char ZipXY[14];
	};

	struct TTmi_MR {
		unsigned int time_sec;
		unsigned short status1, status2;
		unsigned short serial;
//		unsigned short timeExp, nLocalObj;
//		float LocXY[13][2];
		unsigned char timeExp, nLocalObj;
		float LocXY[18][2];
	};

	struct TMloc_MR {
		unsigned int time_sec;
		unsigned short status1, status2;
		unsigned short serial, timeExp;
		unsigned short nLocalObj, nFixedObj;
		unsigned short nPixFlt, mean;
		float sigma;
		float RsLoc[36][3];
		short RsLocSize[36];
		unsigned short nPixNoise, nPixXXX, Th;
	};

	struct TRtmi_MR {
		unsigned int time_sec;
		unsigned short status1, status2;
		unsigned short serial, timeExp, nLocalObj;
		struct stLocal Local[17];
		unsigned short ThFrag[17];
		short Tcmv, Tsens[4];
		unsigned short Ip;
	};
	struct TRtmiFrag_MR {
		unsigned int time_sec;
		unsigned short timeExp, SwitchBin, NumFrag;
	};

	struct TMshi_MF {
		unsigned int time_sec;
		unsigned short dT;
		unsigned short status1, status2;
		float Tpr;
		float Mornt[3][3];
	};

	struct TMshi_SED {
		unsigned int time_sec;
		unsigned short status;
		unsigned char NumLoc, NumDet;
		unsigned short mean;
		unsigned short dT1, dT2;
		double Qornt[4];
        float W[3];
	};
	#pragma pack (pop)

	static string arrStatErrorEng[MAX_STAT] = {
		{"EC1"}, {"EC2"}, {"EC3"}, {"EC4"}, {"EC5"}, {"EC6"}, {"EC7"}, {"EC8"},
		{"EC9"}, {"EC10"}, {"EC11"}, {"EC12"}, {"EC13"}, {"EC14"}, {"EC15"},
		{"EC16"}};
	static string arrStatErrorRus[MAX_STAT] = {
		{"ЕС1"}, {"ЕС2"}, {"ЕС3"}, {"ЕС4"}, {"ЕС5"}, {"ЕС6"}, {"ЕС7"}, {"ЕС8"},
		{"ЕС9"}, {"ЕС10"}, {"ЕС11"}, {"ЕС12"}, {"ЕС13"}, {"ЕС14"}, {"ЕС15"},
		{"ЕС16"}};

	void MakeCRC16Table(void);
	unsigned short GetCRC16(unsigned char *buf, int size);
    unsigned short crc16_ccitt(unsigned char *data_p, int length);

    int FloatToInteger(const float var, int& res);
	void SwapShort(short *word1, short *word2);
    void Swap2Word(short word[2]);
	int GetSizeFrag_2R(char multXY, unsigned short *fragX,  unsigned short *fragY);
    int GetSizeFrag_MR(char multXY, unsigned short *fragX,  unsigned short *fragY);
	void CheckFileName(AnsiString &fileName);

	void SetClkBokz(double clk);

	unsigned int ReadBinaryString(string binaryString);

	unsigned int StringToDayTime(string _line);

	TDateTime GetDateTime(TDateTime _zeroDate, unsigned int _timeBOKZ);
	TDateTime GetDateTime_2R(TDateTime _zeroDate, unsigned int _time_sec,
												  unsigned int _time_msec);

	AnsiString DayTimeToString(unsigned int time);

	AnsiString OutputDateTime(TDateTime _curDate);
	AnsiString OutputDateTime2(TDateTime _curDate);

	int StopReadArray(string line);

	int TryReadErrorBokz(ifstream &finp, struct ERR_BOKZ &tmi);
	void PrintLogError(ofstream &file, struct ERR_BOKZ tmi, TDateTime curDate, bool &create);

	void ClearGyro(struct GYRO &tmi);
	int TryReadGyro(ifstream &finp, struct GYRO &tmi);

	void ClearQuat(struct QBOKZ &tmi);
    int TryReadQuat(ifstream &finp, struct QBOKZ &tmi);

	void ClearSHTMI1(struct SHTMI1 &tmi);
	int TryReadSHTMI1(ifstream &finp, struct SHTMI1 &tmi);

	void ClearSHTMI2(struct SHTMI2 &tmi);
	int TryReadSHTMI2(ifstream &finp, struct SHTMI2 &tmi);

	void ClearDTMI(struct DTMI &tmi);
	int TryReadDTMI(ifstream &finp, struct DTMI &tmi);

	void ClearLOC(struct LOC &tmi);
	int TryReadLOC(ifstream &finp, struct LOC &tmi);

	void PrintLogGyro(ofstream &file, struct GYRO tmi, TDateTime curDate, bool &create);
	void PrintLogQuat(ofstream &file, struct QBOKZ tmi, TDateTime curDate, bool &create);

	void PrintMSHI(ofstream &file, struct MSHI tmi, TDateTime curDate);
	void PrintLogMSHI(ofstream &file, struct MSHI tmi, TDateTime curDate, bool &create);

	void PrintSHTMI1(ofstream &file, struct SHTMI1 tmi);
	void PrintLogSHTMI1(ofstream &file, struct SHTMI1 tmi, bool &create);

	void PrintSHTMI2(ofstream &file, struct SHTMI2 tmi, TDateTime curDate);
	void PrintLogSHTMI2(ofstream &file, struct SHTMI2 tmi, TDateTime curDate, bool &create);

	void PrintDTMI(ofstream &file, TDateTime curDate, struct DTMI tmi, int isM1000);
	void PrintLOC(ofstream &file, TDateTime curDate, struct LOC tmi, int isM1000);

	void PrintLocalDTMI(AnsiString fileDir, TDateTime curDate, struct DTMI tmi, int isM1000);
	void PrintLocalMLOC(AnsiString fileDir, TDateTime curDate, struct LOC tmi, int isM1000);

	void ConvertDataDTMI(struct DTMI tmi, struct CadrInfo &mCadr, int isM1000);
	void ConvertDataLOC(struct LOC tmi, struct CadrInfo &mCadr, int isM1000);

	void SwapShtmi1_2R(TShtmi1_2R *tmi);
    void SwapShtmi2_2R(TShtmi2_2R *tmi);
	void SwapMshi_2R(TMshi_2R *tmi);
    void SwapDtmi_2R(TDtmi_2R *tmi);
    void SwapMloc_2R(TMloc_2R *tmi);
	void PrintLogShtmi1_2R(ofstream &file, TShtmi1_2R *tmi, TDateTime curDate, bool &create);
	void PrintLogShtmi2_2R(ofstream &file, TShtmi2_2R *tmi, TDateTime curDate, bool &create);
	void PrintLogMshi_2R(ofstream &file, TMshi_2R *tmi, TDateTime curDate, bool &create);
	void PrintLogDtmi_2R(ofstream &file, TDtmi_2R tmi, TDateTime curDate, bool &create);
	void PrintLogMloc_2R(ofstream &file, TMloc_2R tmi, TDateTime curDate, bool &create);
	void PrintDtmi_2R(ofstream &file, const TDtmi_2R &tmi, TDateTime curDate, bool &create);
	void PrintMloc_2R(ofstream &file, TMloc_2R tmi, TDateTime curDate);

	void PrintLogMshi_2R_old(ofstream &file, TMshi_2R_old *tmi, TDateTime curDate, bool &create);
	void PrintLogDtmi_2R_old(ofstream &file, TDtmi_2R_old tmi, TDateTime curDate, bool &create);
	void PrintDtmi_2R_old(ofstream &file, const TDtmi_2R_old &tmi, TDateTime curDate, bool &create);

	void PrintLogShtmi1_MR(ofstream &file, TShtmi1_MR tmi, TDateTime curDate, bool &create);
	void PrintLogShtmi2_MR(ofstream &file, TShtmi2_MR tmi, TDateTime curDate, bool &create);
	void PrintLogMshi_MR(ofstream &file, TMshi_MR tmi, TDateTime curDate, bool &create);
	void PrintLogTmi_MR(ofstream &file, TTmi_MR tmi, TDateTime curDate, bool &create);
	void PrintLogRtmi_MR(ofstream &file, TRtmi_MR tmi, TDateTime curDate, bool &create);
	void PrintLogDtmi_MR(ofstream &file, TDtmi_MR tmi, TDateTime curDate, bool &create);
	void PrintLogMloc_MR(ofstream &file, TMloc_MR tmi, TDateTime curDate, bool &create);
	void PrintDtmi_MR(ofstream &file, TDtmi_MR tmi, TDateTime curDate);
	void PrintMloc_MR(ofstream &file, TMloc_MR tmi, TDateTime curDate);

	void ClearAistQuat(struct TAistQuat &tmi);
	int TryReadAistQuat(ifstream &finp, struct TAistQuat &tmi);
	void ClearAistMshi(struct TAistMshi &tmi);
	int TryReadAistMshi(ifstream &finp, struct TAistMshi &tmi);
	void ClearAistShtmi1(struct TAistShtmi1 &tmi);
	int TryReadAistShtmi1(ifstream &finp, struct TAistShtmi1 &tmi);
	void ClearAistShtmi2(struct TAistShtmi2 &tmi);
	int TryReadAistShtmi2(ifstream &finp, struct TAistShtmi2 &tmi);
	void PrintLogAistQuat(ofstream &file, struct TAistQuat tmi, TDateTime curDate, bool &create);
//	void PrintAistMshi(ofstream &file, struct TAistMshi tmi, TDateTime curDate);
	void PrintLogAistMshi(ofstream &file, struct TAistMshi tmi, TDateTime curDate, bool &create);
	void PrintAistShtmi1(ofstream &file, struct TAistShtmi1 tmi);
	void PrintLogAistShtmi1(ofstream &file, struct TAistShtmi1 tmi, bool &create);
	void PrintAistShtmi2(ofstream &file, struct TAistShtmi2 tmi, TDateTime curDate);
	void PrintLogAistShtmi2(ofstream &file, struct TAistShtmi2 tmi, TDateTime curDate, bool &create);

	void PrintMshi_2V(ofstream &file, struct TBoardArray *pack, TDateTime curDate, bool &create);
	void PrintShtmi1_2V(ofstream &file, struct TBoardArray *pack, TDateTime curDate, bool &create);
	void PrintShtmi2_2V(ofstream &file, struct TBoardArray *pack, TDateTime curDate, bool &create);
	void ClearDtmi2V(struct TDtmi_2V &tmi);
	void PrintDtmi_2V(ofstream &file, struct TDtmi_2V tmi, TDateTime curDate, bool &create);
	void ClearMloc2V(struct TMloc_2V &tmi);
    void PrintMloc_2V(ofstream &file, struct TMloc_2V tmi, TDateTime curDate, bool &create);

	int ReadTMIArray(ifstream &_inp, string _keyWord, unsigned short *arrayTMI,
		const int _sizeArray);

	void ConvertDataDTMI_BOKZM(struct DTMI_BOKZM tmi, struct CadrInfo &mCadr);

	void PrintMSHI_BOKZM(ofstream &file, struct MSHI_BOKZM tmi);
	void PrintSHTMI1_BOKZM(ofstream &file, struct SHTMI1_BOKZM tmi);
	void PrintSHTMI2_BOKZM(ofstream &file, struct SHTMI2_BOKZM tmi);
	void PrintDTMI_BOKZM(ofstream &file, struct DTMI_BOKZM tmi);
	void PrintLocalDTMI_BOKZM(AnsiString fileName, struct DTMI_BOKZM tmi);

	void PrintDtmiMeteor(ofstream &file, struct DTMI_METEOR tmi);
    void PrintDtmiM60K(ofstream &file, struct DTMI_M60K tmi);

	void PrintMSHI_M2(ofstream &file, struct MSHI_M2 tmi, TDateTime curDate);
	void PrintTMI1_M2(ofstream &file, struct TMI1_M2 tmi);
	void PrintTMI2_M2(ofstream &file, struct TMI2_M2 tmi, TDateTime curDate);
	void PrintDTMI_M2(ofstream &file, struct DTMI_M2 tmi, TDateTime curDate);
	void PrintMLOC_M2(ofstream &file, struct MLOC_M2 tmi, TDateTime curDate);
	void PrintLogMSHI_M2(ofstream &file, MSHI_M2 tmi, TDateTime curDate, bool &create);
	void PrintLogTMI1_M2(ofstream &file, TMI1_M2 tmi, TDateTime curDate, bool &create);
	void PrintLogTMI2_M2(ofstream &file, TMI2_M2 tmi, TDateTime curDate, bool &create);
	void PrintLogDTMI_M2(ofstream &file, DTMI_M2 tmi, TDateTime curDate, int cntError, bool &create);

	// проверяем содержит ли протокол режим локализации
	bool checkLocFile(ifstream& in);

	bool checkM2Loc(ifstream& in);

	void writeBOKZ1000ProtocolToIKI(CadrInfo& cadrInfo, bool InfoVecEmpty,
		TDateTime& startDate, double timeStep, unsigned int& counter);

	void writeProtocolToIKI(CadrInfo& cadrInfo, int counter);

	void readBOKZ601000MKO(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		unsigned int& counter);

	CadrInfo convertIKIFormatToInfoCadr(IKI_img* reader,
		bool CompareIKIRes = false);

	void parseMILHex(vector <string>& parseTo, int rowCount, ifstream& in, int offset); 

	  	template<class ProtHandler>
	void readBOKZMMil(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handle) {

		const int arraySize = 32;
		const int sizeMSHI = 22;
		const int sizeTLOC = 256;
		const int sizeTFRAG = 128;
		const int sizeMSI = 12;
		const int TLOCCount = 8;
		const int TFRAGCount = 4;

		const string mshior = "\\'cc\\'d8\\'c8 \\'ce\\'d0";
		const string mloc = "\\'cc\\'cb\\'ce\\'ca";
		const string msi = "\\'c4\\'c8\\'cd\\'d4";
		const string xyc = "XYc";
		const int skipBeforeData = 3;
		const int firstDataBlockFirstRow = 2;

		unsigned short arrayMSHI[sizeMSHI];
		unsigned short arrayTLOC[sizeTLOC];
		unsigned short arrayTFRAG[sizeTFRAG];
		unsigned short arrayMSI[sizeMSI];

		CadrInfo cadrInfo;
		string needToFind = mshior;
		int tLocReaded = 0;
		int tFragReaded = 0;

		string line;
		while (getline(in, line)) {

		  if (line.find(mshior) != string::npos) {

			if (needToFind != mshior) {
			   needToFind = mshior;	
			}

			cadrInfo.CountBlock = 0;
			cadrInfo.CountStars = 0;
			cadrInfo.ImageHeight = 512;
			cadrInfo.ImageWidth = 512;

			for (int i = 0; i < skipBeforeData; i++) {
				 getline(in, line);
			}

			vector <string> mshiStr;
			parseMILHex(mshiStr, 3, in, firstDataBlockFirstRow);

			for (int i = 0; i < sizeMSHI; i++) {
				 arrayMSHI[i] = strtoul(mshiStr[i].c_str(), NULL, 16);
			}
			MSHI_BOKZM mshi;
			memcpy(&mshi, arrayMSHI, sizeof(short) * sizeMSHI);
			if (mshi.Mornt[0][0] == 0
			&& mshi.Mornt[0][1] == 0
			&& mshi.Mornt[0][2] == 0) {

				continue;
			}
			cadrInfo.Time = mshi.timeBOKZ;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
				   cadrInfo.MatrixOrient[i][j] = mshi.Mornt[j][i];
				}
			}

			MatrixToEkvAngles(cadrInfo.MatrixOrient, cadrInfo.AnglesOrient);
            cadrInfo.IsOrient = true;
			needToFind = msi;
		  }
		  else if (line.find(msi) != string::npos) {

			if (needToFind != msi) {
				needToFind = mshior;
				cadrInfo = CadrInfo();
				continue;
			}

			for (int i = 0; i < skipBeforeData; i++) {
				 getline(in, line);
			}

			vector <string> msiStr;
			parseMILHex(msiStr, 2, in, firstDataBlockFirstRow);
			for (int i = 0; i < sizeMSI; i++) {
				 arrayMSI[i] = strtoul(msiStr[i].c_str(), NULL, 16);
			}
			MSI_BOKZM msi;
			memcpy(&msi, arrayMSI, sizeof(short) * sizeMSI);
			cadrInfo.CountDeterObj = msi.nDeterObj;
			cadrInfo.CountWindows = msi.nFrag;
			needToFind = mloc;
		  }
		  else if (line.find(mloc) != string::npos) {
			 if (needToFind != mloc) {
				needToFind = mshior;
				cadrInfo = CadrInfo();
				continue;	
			} 

			for (int i = 0; i < skipBeforeData; i++) {
				 getline(in, line);
			}

			vector <string> tLocStr;
			parseMILHex(tLocStr, 4, in, firstDataBlockFirstRow);
			++tLocReaded;

			int startFrom =  arraySize * (tLocReaded - 1);
			int to = arraySize * tLocReaded;
			for (int i = startFrom, j = 0; i < to; i++, j++) {
				 arrayTLOC[i] = strtoul(tLocStr[j].c_str(), NULL, 16);
			}


			if (tLocReaded == TLOCCount) {
				TLOCBOKZM tloc;
				memcpy(&tloc, arrayTLOC,  sizeof(short) * sizeTLOC);
				for (int i = 0; i < MAX_OBJ_BOKZM; i++) {
					ObjectsInfo info;
					info.X = tloc.LocalList[i][0];
					info.Y = tloc.LocalList[i][1];
					info.Bright = tloc.LocalList[i][2];
					info.Square = tloc.LocalList[i][3];
					if (info.X == 0 && info.Y == 0) {		
						break;
					}
					cadrInfo.ObjectsList.push_back(info);
				}
				cadrInfo.SizeObjectsList = cadrInfo.CountLocalObj = cadrInfo.ObjectsList.size();
				tLocReaded = 0;
				needToFind = xyc; 
			}

		  }
		  else if (line.find(xyc) != string::npos) {
			 if (needToFind != xyc) {
				needToFind = mshior;
				cadrInfo = CadrInfo();
				continue;	
			} 


			for (int i = 0; i < skipBeforeData; i++) {
				 getline(in, line);
			}

			vector <string> tFragStr;
			parseMILHex(tFragStr, 4, in, firstDataBlockFirstRow);
			++tFragReaded;
			
			int startFrom =  arraySize * (tFragReaded - 1);
			int to = arraySize * tFragReaded;
			for (int i = startFrom, j = 0; i < to; i++, j++) {
				 arrayTFRAG[i] = strtoul(tFragStr[j].c_str(), NULL, 16);
			}


			if (tFragReaded == TFRAGCount) {
				needToFind = xyc; 
				TFRAGBOKZM tfrag;
				memcpy(&tfrag, arrayTFRAG,  sizeof(short) * sizeTFRAG);
				tFragReaded = 0;
				cadrInfoVec.push_back(cadrInfo);
				handle(cadrInfo);
				cadrInfo = CadrInfo();
			}

		  }
	}
}

	template<class ProtHandler>
	void readBOKZ60LocProtocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handle, TDateTime& startDate) {
		string line;
		string errorMessage =
			string("Cчитывание протокола завершено необычным образом. "
			"Возможно работа прибора была остановлена.");
		while (getline(in, line)) {
			if (line.find("Состав ДТМИ ЛОК:") != string::npos) {
				CadrInfo cadrInfo;
				cadrInfo.CountBlock = 0;
				cadrInfo.CountStars = 0;
				cadrInfo.ImageHeight = 512;
				cadrInfo.ImageWidth = 512;
				// считываем время привязки
				if (findWord(in, "информации") != string::npos) {

					in >> cadrInfo.Time;
					startDate = IncMilliSecond(DateOf(startDate),
						cadrInfo.Time * 1000);
					if (cadrInfoVec.size() == 0) {
						cadrInfo.Time = startDate.Val;
					}
					else {
						cadrInfo.Time = cadrInfoVec.back().Time +
							(startDate.Val - cadrInfoVec.back().Time);
					}

				}
				else
					throw logic_error(errorMessage);

				// ищем число локализованных объектов
				if (findWord(in, "объектов") != string::npos) {
					in >> cadrInfo.CountLocalObj;
				}
				else
					throw logic_error(errorMessage);

				// ищем число распознанных объектов
				if (findWord(in, "объектов") != string::npos) {
					in >> cadrInfo.CountDeterObj;
				}
				else
					throw logic_error(errorMessage);

				// ищем начало массива лок
				if (findLine(in,
					"	Х			Y			I			N") != string::npos)
				{
					vector<string>splittedLocData;
					const int сountLocObj = cadrInfo.CountLocalObj;
					ObjectsInfo objInfo;
					for (int i = 0; i < сountLocObj; i++) {
						getline(in, line);
						// см. эту строку в протоколе, чтобы понять почему так
						splittedLocData = split(line, ")\t");
						splittedLocData = split(splittedLocData[1], "\t");

						objInfo.X = atof(splittedLocData[0].c_str());
						if (objInfo.X == 0)
							break;
						objInfo.Y = atof(splittedLocData[1].c_str());
						objInfo.Bright = atof(splittedLocData[2].c_str());
						objInfo.Square = atoi(splittedLocData[3].c_str());

						cadrInfo.ObjectsList.push_back(objInfo);
					}

				}
				else
					throw logic_error(errorMessage);

				cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
				handle(cadrInfo);
				cadrInfoVec.push_back(cadrInfo);
				writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
			}

		}
	}

	template<class ProtHandler>
	void readBOKZ60MSHIOR(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handle, TDateTime& startDate) {
		try {

			const char *args[] = {
				"05", "06", "07", "08", "09", "0a", "0b", "0c", "0d"};
			static vector<string>err(args, args + 9);
			string line;
			string errorMessage =
				string("Cчитывание протокола завершено необычным образом. "
				"Возможно работа прибора была остановлена.");
			while (getline(in, line)) {
				bool error = false;
				TColor PointColor = clGreen;
				if (line.find("Состав МШИ ОР:") != string::npos) {
					CadrInfo cadrInfo;
					cadrInfo.ImageHeight = 512;
					cadrInfo.ImageWidth = 512;
					getline(in, line);
					vector<string>splitted = split(line, "\t\t\t");
					cadrInfo.StatOrient =
						strtoul(string(splitted[1].substr(2, 3) + "0").c_str(),
						NULL, 16);
					if (contains(splitted[1], "e8")) {
						PointColor = clBlue;
					}

					if (findLine(in, "Команда выполнена") != string::npos) {
						getline(in, line);
						splitted = split(line, "\t\t\t");
						if (find(err.begin(), err.end(), splitted[1].substr(0,
							2)) != err.end()) {
							error = true;
							PointColor = clRed;
						}

					}

					// ищем время привязки
					if (findWord(in, "информации") != string::npos) {
						cadrInfo.FrameNumber = cadrInfo.Time;
						in >> cadrInfo.Time;

						if (cadrInfo.Time == 0 || cadrInfo.Time > 10000)
							cadrInfo.Time = 0;
						startDate =
							IncMilliSecond(DateOf(startDate),
							cadrInfo.Time * 1000);
						if (cadrInfoVec.size() == 0) {
							cadrInfo.Time = startDate.Val;
						}
						else {
							cadrInfo.Time = cadrInfoVec.back().Time +
								(startDate.Val - cadrInfoVec.back().Time);
						}
					}
					else
						throw logic_error(errorMessage);

					if (findLine(in, "4) Кватернион ориентации, Qо")
						!= string::npos) {

						for (int i = 0; i < 4; i++) {
							getline(in, line);
							vector<string>splittedStr =
								split(line, "\t\t\t\t\t");
							cadrInfo.QuatOrient[i] =
								atof(splittedStr[1].c_str());
						}
						if (cadrInfo.QuatOrient[0] == 0 && !error)
							continue;
						double matrixOfOrientation[3][3];
						quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
						MatrixToEkvAngles(matrixOfOrientation,
							cadrInfo.AnglesOrient);

					}
					else
						throw logic_error(errorMessage);

					if (findLine(in, "5) Угловая скорость по оптическим")
						!= string::npos) {
						for (int i = 0; i < 3; i++) {
							getline(in, line);
							vector<string>splittedStr =
								split(line, "\t\t\t\t\t");
							cadrInfo.OmegaOrient[i] =
								atof(splittedStr[1].c_str());
						}
					}
					else
						throw logic_error(errorMessage);

					handle(cadrInfo, PointColor);
					cadrInfoVec.push_back(cadrInfo);
					writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
				}

			}
		}
		catch (exception &e) {
			ShowMessage(e.what());
		}

	}

	template<class ProtHandler>
	void readBOKZ60Protocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handle, TDateTime& startDate) {
		try {

			const char *args[] = {
				"05", "06", "07", "08", "09", "0a", "0b", "0c", "0d"};
			static vector<string>err(args, args + 11);
			string line;
			string errorMessage =
				string("Cчитывание протокола завершено необычным образом. "
				"Возможно работа прибора была остановлена.");
			while (getline(in, line)) {
				TColor PointColor = clGreen;
				if (line.find("Состав ДТМИ:") != string::npos) {
					CadrInfo cadrInfo;
					cadrInfo.ImageHeight = 512;
					cadrInfo.ImageWidth = 512;

					// ищем время привязки
					if (findWord(in, "информации") != string::npos) {
						cadrInfo.FrameNumber = cadrInfo.Time;
						in >> cadrInfo.Time;
						if (cadrInfo.Time == 0 || cadrInfo.Time > 10000)
							cadrInfo.Time = 0;
						startDate =
							IncMilliSecond(DateOf(startDate),
							cadrInfo.Time * 1000);
						if (cadrInfoVec.size() == 0) {
							cadrInfo.Time = startDate.Val;
						}
						else {
							cadrInfo.Time = cadrInfoVec.back().Time +
								(startDate.Val - cadrInfoVec.back().Time);
						}
						getline(in, line);
						getline(in, line);
						vector<string>splitted = split(line, "\t\t\t");
						cadrInfo.StatOrient =
							strtoul(string(splitted[1].substr(2, 3) + "0").c_str
							(), NULL, 16);
						if (contains(splitted[1], "e0")) {
							PointColor = clBlue;
						}

						getline(in, line);
						splitted = split(line, "\t\t\t");
						if (find(err.begin(), err.end(), splitted[1].substr(0,
							2)) != err.end()) {
							PointColor = clRed;
						}
					}
					else
						throw logic_error(errorMessage);

					if (findWord(in, "объектов") != string::npos) {
						in >> cadrInfo.CountLocalObj;
					}
					else
						throw logic_error(errorMessage);

					// ищем число распознанных объектов
					if (findWord(in, "объектов") != string::npos) {
						in >> cadrInfo.CountDeterObj;
					}
					else
						throw logic_error(errorMessage);

					if (findWord(in, "распознавания") != string::npos) {
						in >> cadrInfo.Epsilon;
					}
					else
						throw logic_error(errorMessage);

					// ищем начало массива лок
					if (findLine(in,
						"	Х			Y			I			nPix		ThFrag")
						!= string::npos) {
						vector<string>splittedLocData;
						const int сountLocObj = 16;
						ObjectsInfo objInfo;

						for (int i = 0; i < сountLocObj; i++) {
							getline(in, line);
							splittedLocData = split(line, ")\t");
							splittedLocData = split(splittedLocData[1], "\t\t");

							if (atof(splittedLocData[0].c_str()) == 0) {
								cadrInfo.CountLocalObj = i;
								break;
							}
							objInfo.X = atof(splittedLocData[0].c_str());
							objInfo.Y = atof(splittedLocData[1].c_str());
							objInfo.Bright = atof(splittedLocData[2].c_str());
							objInfo.Square = atoi(splittedLocData[3].c_str());
							cadrInfo.ObjectsList.push_back(objInfo);
						}

					}
					else
						throw logic_error(errorMessage);

					if (findLine(in, "14) Проекции угловой скорости на оси ПСК")
						!= string::npos) {
						for (int i = 0; i < 3; i++) {
							getline(in, line);
							vector<string>splittedStr = split(line, "\t");
							cadrInfo.OmegaOrient[i] =
								atof(splittedStr[1].c_str());
						}

					}
					else
						throw logic_error(errorMessage);

					if (findLine(in, "15) Координаты центров фрагментов")
						!= string::npos) {
						WindowsInfo winInfo;
						const int maxCountOfObjects = 16;
						cadrInfo.CountWindows = maxCountOfObjects;
						for (int i = 0; i < cadrInfo.CountWindows; i++) {
							getline(in, line);
							vector<string>splittedStr = split(line, "\t");

							if (atoi(splittedStr[1].c_str()) == 0) {
								cadrInfo.CountWindows = i;
								break;
							}

							winInfo.Xstart = atof(splittedStr[1].c_str());
							winInfo.Ystart = atof(splittedStr[2].c_str());
							winInfo.Mean = 0;
							winInfo.Sigma = 0;
							winInfo.Mv = 0;
							cadrInfo.WindowsList.push_back(winInfo);
						}

					}
					else
						throw logic_error(errorMessage);

					if (findLine(in, "16) Значение порогов во фрагментах")
						!= string::npos) {
						for (int i = 0; i < cadrInfo.CountWindows; i++) {
							getline(in, line);
							vector<string>splittedStr = split(line, "\t");
							cadrInfo.WindowsList[i].Level =
								atoi(splittedStr[1].c_str());
						}

					}
					else
						throw logic_error(errorMessage);

					if (findLine(in, "17) Количество объектов во фрагментах")
						!= string::npos) {
						for (int i = 0; i < cadrInfo.CountWindows; i++) {
							getline(in, line);
							vector<string>splittedStr = split(line, "\t");
							cadrInfo.WindowsList[i].CountObj =
								atoi(splittedStr[1].c_str());
						}

					}
					else
						throw logic_error(errorMessage);

					if (findLine(in,
						"19) Последний хороший оптический кватернион")
						!= string::npos) {

						for (int i = 0; i < 4; i++) {
							getline(in, line);
							vector<string>splittedStr = split(line, "\t");
							cadrInfo.QuatOrient[i] =
								atof(splittedStr[1].c_str());
						}

						double matrixOfOrientation[3][3];
						quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
						MatrixToEkvAngles(matrixOfOrientation,
							cadrInfo.AnglesOrient);

					}
					else
						throw logic_error(errorMessage);

					int pos = in.tellg();
					if (findLine(in, "Состав МШИ ОР") != string::npos) {
						getline(in, line);
						vector<string>splitted = split(line, "\t\t\t");
						int status1 =
							strtoul(string(string("0x") + splitted[1].c_str())
							.c_str(), NULL, 16);
						getline(in, line);
						splitted = split(line, "\t\t\t");
						cadrInfo.StatOrient =
							strtoul(string(string("0x") + splitted[1].substr(0,
							3) + "0").c_str(), NULL, 16);
						if (!((status1 >> 11) & 1))
							// если 12 бит не установлен, то это НО
						{
							if (contains(splitted[1], "000"))
								cadrInfo.DeviceInfo = "HO 4";
							else if (contains(splitted[1], "010"))
								cadrInfo.DeviceInfo = "HO 1";
							else if (contains(splitted[1], "020")) {
								if (cadrInfoVec.back().DeviceInfo == "HO 1")
									cadrInfo.DeviceInfo = "HO 2";
								else
									cadrInfo.DeviceInfo = "HO 3";
							}
						}
						else {
							cadrInfo.DeviceInfo = "SLEZH";
						}
					}
					else
						in.seekg(pos);

					cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
					cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();

					handle(cadrInfo, PointColor);
					cadrInfoVec.push_back(cadrInfo);
					writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
				}

			}
		}
		catch (exception &e) {
			ShowMessage(e.what());
		}

	}

	template<class ProtHandler>
	void readmBOKZ2VProtocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handle, TDateTime& startDate) {
		string line;
		string errorMessage =
			string("Cчитывание протокола завершено необычным образом. "
			"Возможно работа прибора была остановлена.");

		while (getline(in, line)) {
			TColor PointColor = clBlue;
			if (line.find("2,3	Время привязки информации	[с]")
				!= string::npos) {

				CadrInfo cadrInfo;
				cadrInfo.ImageHeight = 1024;
				cadrInfo.ImageWidth = 1024;

				if (findWord(in, "состояния") != string::npos) {
					string status1;
					in >> status1 >> status1;
					if (findWord(in, "состояния") != string::npos) {
						string status2;
						in >> status2 >> status2;
						if (status1 != "EC00" && status2 != "00C3") {
							PointColor = clRed;
						}
					}
					else
						throw logic_error(errorMessage);

				}
				else
					throw logic_error(errorMessage);

				// если нашли строку с числом принятых пикселей, проверяем, что их число не ноль
				// если ноль, пропускаем такт
				if (findLine(in, "20 Номер сектора (0..7 биты)")
					!= string::npos) {
					getline(in, line);
					vector<string>splitted = split(line, "\t");
					cadrInfo.Epsilon = atoi(splitted[1].c_str());
				}
				else
					throw logic_error(errorMessage);

				if (findWord(in, "кадре") != string::npos) {
					int number;
					in >> number;
					if (number == 0)
						continue;
				}
				else
					throw logic_error(errorMessage);

				// ищем начало массива лок и фрагментов
				if (findLine(in,
					"18, 19	Массив локализованных объектов на 1-ом кадре")
					!= string::npos) {
					vector<string>splittedStr;
					const int maxCountLocObj = 15;
					ObjectsInfo objInfo;
					WindowsInfo winInfo;

					for (int i = 0; i < maxCountLocObj; i++) {
						getline(in, line);
						splittedStr = split(line, "\t");

						// если всё-таки объектов меньше
						if (atof(splittedStr[0].c_str()) == 0) {
							break;
						}
						// заполняем все о лок
						objInfo.X = atof(splittedStr[0].c_str());
						objInfo.Y = atof(splittedStr[1].c_str());
						objInfo.Bright = atof(splittedStr[2].c_str());
						objInfo.Square = atoi(splittedStr[3].c_str());
						cadrInfo.ObjectsList.push_back(objInfo);

						// заполняем всё о фрагментах
						winInfo.Mv = 0;
						winInfo.Mean = atof(splittedStr[6].c_str());
						winInfo.Sigma = atof(splittedStr[7].c_str());
						winInfo.Level = atof(splittedStr[8].c_str());
						winInfo.CountObj = atoi(splittedStr[9].c_str());
						unsigned short windowSize =
							atoi(splittedStr[10].c_str());

						switch (windowSize) {
						case 15:
							winInfo.Width = 24;
							winInfo.Height = 24;
							break;
						case 14:
							winInfo.Width = 24;
							winInfo.Height = 48;
							break;
						case 11:
							winInfo.Width = 48;
							winInfo.Height = 24;
							break;
						case 10:
							winInfo.Width = 48;
							winInfo.Height = 48;
							break;
						}
						winInfo.Xstart = (atof(splittedStr[4].c_str())) -
							winInfo.Width / 2;
						winInfo.Ystart = (atof(splittedStr[5].c_str())) -
							winInfo.Height / 2;
						cadrInfo.WindowsList.push_back(winInfo);

						if (objInfo.Square < 0) {
							objInfo.StarID = 1;
						}

					}

					if (findLine(in,
						"18, 19	Массив локализованных объектов на 2-ом кадре")
						!= string::npos) {
						for (int i = 0; i < maxCountLocObj; i++) {
							getline(in, line);
							splittedStr = split(line, "\t");

							// если всё-таки объектов меньше
							if (atof(splittedStr[0].c_str()) == 0) {
								break;
							}
							// заполняем все о лок
							objInfo.X = atof(splittedStr[0].c_str());
							objInfo.Y = atof(splittedStr[1].c_str());
							objInfo.Bright = atof(splittedStr[2].c_str());
							objInfo.Square = atoi(splittedStr[3].c_str());
							cadrInfo.ObjectsList.push_back(objInfo);

							if (objInfo.Square < 0) {
								objInfo.StarID = 1;
							}
						}
					}

				}
				else
					throw logic_error(errorMessage);

				cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
				cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
				GetImageBright(cadrInfo);

				// ищем время привязки в секундах
				if (findWord(in, "информации") != string::npos) {
					int secs = 0;
					int msecs = 0;
					int prevPos = in.tellg();

					in >> line;
					if (line == "с") {
						in >> secs;

						getline(in, line);
						getline(in, line);

						vector<string>splittedStr = split(line, "\t");
						msecs = atoi(splittedStr[1].c_str());
					}

					else {
						// если там не "c", возвращаемся назад
						in.seekg(prevPos);
						in >> secs;
						// ищем миллисекунды
						findWord(in, "информации");
						in >> msecs;

					}

					int ms = secs * 1000 + msecs;
					startDate = IncMilliSecond(DateOf(startDate), ms);
					if (cadrInfoVec.size() == 0) {
						cadrInfo.Time = startDate.Val;
					}
					else {
						cadrInfo.Time = cadrInfoVec.back().Time +
							(startDate.Val - cadrInfoVec.back().Time);
					}
				}
				else
					throw logic_error(errorMessage);

				if (findLine(in, "6) Кватернион ориентации, Qо")
					!= string::npos) {

					for (int i = 0; i < 4; i++) {
						getline(in, line);
						vector<string>splittedStr = split(line, "\t\t\t\t");
						cadrInfo.QuatOrient[i] = atof(splittedStr[1].c_str());
					}

					double matrixOfOrientation[3][3];
					quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
					MatrixToEkvAngles(matrixOfOrientation,
						cadrInfo.AnglesOrient);

				}
				else
					throw logic_error(errorMessage);

				if (findLine(in,
					"Угловая скорость по оптическим измерениям в проекциях на оси ПСК")
					!= string::npos) {
					for (int i = 0; i < 3; i++) {
						getline(in, line);
						vector<string>splittedStr = split(line, "\t\t\t\t");
						cadrInfo.OmegaOrient[i] = atof(splittedStr[i].c_str());
					}

				}
				else
					throw logic_error(errorMessage);

				if (findWord(in, "Tcmv") != string::npos) {
					in >> cadrInfo.MatrixTemp;
				}
				else
					throw logic_error(errorMessage);

				// ищем число спроектированных звезд
				if (findWord(in, "NumProgFrag") != string::npos) {
					in >> cadrInfo.CountStars;
					if (cadrInfo.CountStars <= 0)
						continue;
				}
				else
					throw logic_error(errorMessage);

				// ищем число фрагментов
				if (findWord(in, "NumFrag") != string::npos) {
					// общее число фрагментов
					in >> cadrInfo.CountWindows;
					if (cadrInfo.CountWindows <= 0)
						continue;
				}
				else
					throw logic_error(errorMessage);

				// число локализ. объектов
				if (findWord(in, "NumLoc[0]") != string::npos) {
					in >> cadrInfo.CountLocalObj;
					if (cadrInfo.CountLocalObj <= 0)
						continue;
				}
				else
					throw logic_error(errorMessage);

				// ищем число распознанных объектов
				if (findWord(in, "NumDet") != string::npos) {
					in >> cadrInfo.CountDeterObj;
					if (cadrInfo.CountDeterObj <= 0)
						continue;
				}
				else
					throw logic_error(errorMessage);

				if (findWord(in, "m_cur") != string::npos) {
					in >> cadrInfo.MeanErrorXY;
				}
				else
					throw logic_error(errorMessage);

				handle(cadrInfo);
				cadrInfoVec.push_back(cadrInfo);
				writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
			}

		}

	}

	template<class ProtHandler>
	void readBOKZ601000Protocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		unsigned int& counter, TDateTime& startDate, ProtHandler handler) {

		try {

			const char *args[] = {
				"04", "05", "06", "07", "08", "09", "0a", "0b", "0d", "0e"};
			static vector<string>err(args, args + 10);
			const string errorMessage =
				string("Cчитывание протокола завершено необычным образом.");
			const int maxTimePr = 100000;      //C++11
			double timeStep = 0.25;
			string line;

			static bool NeedNextFile = false;
			// для ВГ
			if (!cadrInfoVec.empty()) {
				bool readLastData = true;
				if (NeedNextFile) {
					if (findWord(in, "такта:") != string::npos) {
						int TickNumber = 0;
						in >> TickNumber;
						if (cadrInfoVec.empty()) {
							NeedNextFile = false;
						}
						else {
							if (TickNumber != cadrInfoVec.back().FrameNumber) {
								cadrInfoVec.erase(cadrInfoVec.end() - 1);
								readLastData = false;
							}
						}

					}
					else
						readLastData = false;
					if (readLastData) {
						if (findLine(in, "5) Кватернион ориентации, Qо")
							!= string::npos) {

							for (int i = 0; i < 4; i++) {
								getline(in, line);
								vector<string>splittedStr =
									split(line, "\t\t\t\t");
								cadrInfoVec.back().QuatOrient[i] =
									atof(splittedStr[1].c_str());
							}

							double matrixOfOrientation[3][3];
							quatToMatr(cadrInfoVec.back().QuatOrient,
								matrixOfOrientation);
							MatrixToEkvAngles(matrixOfOrientation,
								cadrInfoVec.back().AnglesOrient);

						}
						else
							throw logic_error(errorMessage);
						// протоколы кончаются на ДТМИ

						if (findLine(in,
							"Угловая скорость по оптическим измерениям в проекциях на оси ПСК")
							!= string::npos) {
							for (int i = 0; i < 3; i++) {
								getline(in, line);
								vector<string>splittedStr =
									split(line, "\t\t\t\t");
								cadrInfoVec.back().OmegaOrient[i] =
									atof(splittedStr[1].c_str());
							}

						}
						else
							throw logic_error(errorMessage);
						writeBOKZ1000ProtocolToIKI(cadrInfoVec.back(),
							cadrInfoVec.empty(), startDate, timeStep, counter);
						NeedNextFile = false;
					}

				}
			}

			while (getline(in, line)) {
				TColor pointColor = clBlue;
				if (line.find("Номер такта:") != string::npos) {
					// номер такта в ДТМИ
					int TickNumber = 0;
					vector<string>splitTickNumber = split(line, " ");
					TickNumber = atoi(splitTickNumber[2].c_str());

					if (findLine(in, "Состав ДТМИ:") != string::npos) {
						CadrInfo cadrInfo;
						cadrInfo.FrameNumber = TickNumber;
						cadrInfo.ImageHeight = 1024;
						cadrInfo.ImageWidth = 1024;

						if (findLine(in, "Состав ДТМИ 1:") != string::npos) {

						   bool skipDTMI = false;
						   for (int i = 0; i < 2; i++) {
								getline(in, line);
								if (contains(line, "(ошибка)")) {
									skipDTMI = true;
								}
						   }
						   if (skipDTMI) {
							   continue;
						   }

                        }

						// время привязки в секундах
						if (findWord(in, "информации") != string::npos) {
							in >> cadrInfo.Time;
							if (cadrInfo.Time > maxTimePr)
								continue;
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "состояния") != string::npos) {
							string status;
							in >> status >> status;
							if (findWord(in, "состояния") != string::npos) {
								string status2;
								in >> status2 >> status2;
								// ТО
								cadrInfo.StatOrient =
									strtoul(status2.c_str(), NULL, 16);
								if (status == "ec00") {
									if (find(err.begin(), err.end(),
										status2.substr(0, 2)) != err.end()) {
										pointColor = clRed;
									}
								}
								// НО
								else if (status == "2400") {
									pointColor = clGreen;
									if (status2.substr(0, 2) == "0c" ||
										status2.substr(0, 2) == "01") {
										continue;
									}

									else if (find(err.begin(), err.end(),
										status2.substr(0, 2)) != err.end()) {
										pointColor = clRed;
									}
								}
								// и тут пропускаем мусор
								else if (status == "0000" && status2 ==
									"0000" || status == "c400" ||
									status == "4456") {
									continue;
								}
								else {
									pointColor = clRed;
								}
							}
							else
								throw logic_error(errorMessage);
						}
						else
							throw logic_error(errorMessage);
						// локализованные
						string fLine;
						if (findLineBack(in, "локализованных объектов", fLine) != string::npos) {
							vector <string> v = split(fLine,"\t");
							cadrInfo.CountLocalObj = atoi(v[1].c_str());
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "объектов") != string::npos) {
							in >> cadrInfo.CountDeterObj;
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "фрагментов") != string::npos) {
							in >> cadrInfo.CountWindows;
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "распознавания") != string::npos) {
							in >> cadrInfo.Epsilon;
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "времени") != string::npos) {
							in >> timeStep;
						}

						if (findLine(in,
							"	Х			Y			I			N")
							!= string::npos) {
							vector <string> splittedLocData;
							const int сountLocObj = cadrInfo.CountLocalObj;
							ObjectsInfo objInfo;
							for (int i = 0; i < сountLocObj; i++) {
								getline(in, line);
								// см. эту строку в протоколе, чтобы понять почему так
								splittedLocData = split(line, ")\t");
								splittedLocData = split(splittedLocData[1], "\t");

								objInfo.X = atof(splittedLocData[0].c_str());
								objInfo.Y = atof(splittedLocData[1].c_str());

								if (objInfo.X == 0 && objInfo.Y == 0) {
									break;
								}

								objInfo.Bright =
									atof(splittedLocData[2].c_str());
								objInfo.Square =
									atoi(splittedLocData[3].c_str());

								// чтобы отобразить объект как распознаный
								if (objInfo.Square < 0) {
									objInfo.StarID = 1;
								}

								cadrInfo.ObjectsList.push_back(objInfo);
							}
							cadrInfo.SizeObjectsList =
								cadrInfo.ObjectsList.size();
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "такта:") != string::npos) {
							// для ВГ
							int TickNumberSecond = 0;
							in >> TickNumberSecond;
							// если была рассинхронизация
							if (cadrInfo.FrameNumber != TickNumberSecond) {
								timeStep += timeStep;
								// continue;
							}
						}
						else {
							NeedNextFile = true;
							cadrInfoVec.push_back(cadrInfo);
							handler(cadrInfo, pointColor);
							break;
						}

						if (findLine(in, "5) Кватернион ориентации, Qо")
							!= string::npos) {

							for (int i = 0; i < 4; i++) {
								getline(in, line);
								vector<string>splittedStr =
									split(line, "\t\t\t\t");
								cadrInfo.QuatOrient[i] =
									atof(splittedStr[1].c_str());
							}

							quatToMatr(cadrInfo.QuatOrient,
								cadrInfo.MatrixOrient);
							MatrixToEkvAngles(cadrInfo.MatrixOrient,
								cadrInfo.AnglesOrient);
						}
						else
							throw logic_error(errorMessage);
						// протоколы кончаются на ДТМИ

						if (findLine(in,
							"Угловая скорость по оптическим измерениям в проекциях на оси ПСК")
							!= string::npos) {
							for (int i = 0; i < 3; i++) {
								getline(in, line);
								vector<string>splittedStr =
									split(line, "\t\t\t\t");
								cadrInfo.OmegaOrient[i] =
									atof(splittedStr[1].c_str());
							}

						}
						else
							throw logic_error(errorMessage);

						writeBOKZ1000ProtocolToIKI(cadrInfo,
							cadrInfoVec.empty(), startDate, timeStep, counter);
						handler(cadrInfo, pointColor);
						cadrInfoVec.push_back(cadrInfo);

					}

				}
			}

		}
		catch (exception &e) {
			ShowMessage(e.what());
		}

	}

//	template<class ProtHandler>
//	void readBOKZMFProtocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
//		ProtHandler handler, TDateTime& dt) {
//		try {
//			const string errorMessage =
//				string("Cчитывание протокола завершено необычным образом.");
//			string line;
//			const string dtmi1 =
//				"\\par \\b C\\'ee\\'f1\\'f2\\'e0\\'e2 \\'c4\\'d2\\'cc\\'c81:";
//			const string mshior = "Alfa"; // признак начала МШИ ОР слежения
//			const string matrixOrient =
//				"\\'cc\\'e0\\'f2\\'f0\\'e8\\'f6\\'e0 \\'ee\\'f0\\'e8\\'e5\\'ed\\'f2\\'e0\\'f6\\'e8\\'e8";
//			const string codeStatus =
//				"\\'ca\\'ee\\'e4 \\'f1\\'ee\\'f1\\'f2\\'ee\\'ff\\'ed\\'e8\\'ff";
//			// КС ДМТИ
//			const string CS = "Az"; // перед кодом состояния
//			const string timePrMSHIOR = "(\\'d2\\'ef\\'f0";
//			// время привязки   МШИ ОР
//			const string timePrDTMI =
//				"\\'ef\\'f0\\'e8\\'e2\\'ff\\'e7\\'ea\\'e8";
//			// время привязки ДТМИ
//			const string objects = "\\'ee\\'e1\\'fa\\'e5\\'ea\\'f2\\'ee\\'e2";
//			const string locTable =
//				"\\par \\b0  \\f1\\u8470?     X       Y       \\f0\\'df\\'f0\\'ea\\'ee\\'f1\\'f2\\'fc \\'d7\\'e8\\'f1\\'eb\\'ee \\'fd\\'eb.";
//			const string fragTable = "\\par \\b0  \\f1\\u8470?     X       Y  ";
//			const string datetime =
//				"\\viewkind4\\uc1\\pard\\b\\f0\\fs20\\'d3\\'f7\\'e0\\'f1\\'f2\\'ee\\'ea";
//			const string timePrHMS = "\\'ef\\'f0\\'e8\\'e2\\'ff\\'e7\\'ea\\'e8";
//
//			if (dt.Val == 0 && findLine(in, datetime) != string::npos) {
//				getline(in, line);
//				getline(in, line);
//				vector<string>splitted = split(line, " ");
//				dt = StrToDateTime(toUString(splitted[2] + " " + splitted[3]));
//				in.seekg(ios_base::beg);
//			}
//			else
//				throw logic_error("Не найдено стартовое время привязки");
//
//			while (getline(in, line)) {
//				CadrInfo cadrInfo;
//				cadrInfo.ImageHeight = 512;
//				cadrInfo.ImageWidth = 512;
//				if (line.find(dtmi1) != string::npos) {
//					if (findWord(in, timePrDTMI) != string::npos) {
//						string hms;
//						in >> hms >> hms;
//						dt = StrToDateTime
//							(dt.DateString() + " " + toUString(hms));
//						cadrInfo.Time = dt.Val;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					if (findWord(in, objects) != string::npos) // лок
//					{
//						in >> cadrInfo.CountLocalObj;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					if (findWord(in, objects) != string::npos) // расп.
//					{
//						in >> cadrInfo.CountDeterObj;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					if (findLine(in, locTable) != string::npos) {
//						ObjectsInfo objInfo;
//						while (getline(in, line) && line != "\\par ") {
//							vector<string>splitted = split(line, " ");
//							objInfo.X = atof(splitted[2].c_str());
//							objInfo.Y = atof(splitted[3].c_str());
//							if (objInfo.X == 0 && objInfo.Y == 0)
//								break;
//							objInfo.Bright = atof(splitted[4].c_str());
//							objInfo.Square = atoi(splitted[5].c_str());
//							cadrInfo.ObjectsList.push_back(objInfo);
//						}
//						cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
//					}
//					else
//						throw logic_error(errorMessage);
//					handler(cadrInfo, clBlue);
//					cadrInfoVec.push_back(cadrInfo);
//					writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
//
//				}
//				else if (line.find(mshior) != string::npos) {
//					TColor pointColor = clBlue;
//					if (findLine(in, CS) != string::npos) {
//						string status1, status2;
//						getline(in, status1);
//						getline(in, status2);
//						if (!(contains(status1, "e000H") && contains(status2,
//							"0005H"))) {
//							pointColor = clRed;
//						}
//					}
//					else
//						throw logic_error(errorMessage);
//
//					if (findWord(in, timePrMSHIOR) != string::npos) {
//						string hms;
//						in >> hms >> hms;
//						dt = StrToDateTime
//							(dt.DateString() + " " + toUString(hms));
//						cadrInfo.Time = dt.Val;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					if (findLine(in, matrixOrient) != string::npos) {
//						for (int i = 0; i < 3; i++) {
//							getline(in, line);
//							vector<string>splitted = split(line, " ");
//							cadrInfo.MatrixOrient[i][0] =
//								atof(splitted[1].c_str());
//							cadrInfo.MatrixOrient[i][1] =
//								atof(splitted[2].c_str());
//							cadrInfo.MatrixOrient[i][2] =
//								atof(splitted[3].c_str());
//						}
//						MatrixToEkvAngles(cadrInfo.MatrixOrient,
//							cadrInfo.AnglesOrient);
//					}
//
//					// число локализ. объектов
//					if (findWord(in, "NumLoc") != string::npos) {
//						in >> cadrInfo.CountLocalObj;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					// ищем число фрагментов
//					if (findWord(in, "NumObj") != string::npos) {
//						in >> cadrInfo.CountDeterObj;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					// ищем число распознанных объектов
//					if (findWord(in, "NumFrag") != string::npos) {
//						// общее число фрагментов
//						in >> cadrInfo.CountWindows;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					if (findLine(in, "NumSec") != string::npos) {
//						for (int i = 0; i < 3; i++) {
//							getline(in, line);
//							vector<string>splitted = split(line, " ");
//							cadrInfo.OmegaOrient[i] =
//								atof(splitted.back().c_str());
//						}
//					}
//					else
//						throw logic_error(errorMessage);
//
//					if (findLine(in, locTable) != string::npos) {
//						ObjectsInfo objInfo;
//						while (getline(in, line) && line != "\\par ") {
//							vector<string>splitted = split(line, " ");
//							objInfo.X = atof(splitted[2].c_str());
//							objInfo.Y = atof(splitted[3].c_str());
//							if (objInfo.X == 0 && objInfo.Y == 0)
//								break;
//							objInfo.Bright = atof(splitted[4].c_str());
//							objInfo.Square = atoi(splitted[5].c_str());
//							cadrInfo.ObjectsList.push_back(objInfo);
//						}
//						cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
//					}
//					else
//						throw logic_error(errorMessage);
//
//					if (findLine(in, fragTable) != string::npos) {
//						WindowsInfo winInfo;
//						bool checkFirstRow = false;
//						while (getline(in, line) && line != "\\par ") {
//							vector<string>splitted = split(line, " ");
//							if (!checkFirstRow) {
//								winInfo.Xstart = atof(splitted[3].c_str());
//								winInfo.Ystart = atof(splitted[4].c_str());
//								winInfo.CountObj = 1;
//								checkFirstRow = true;
//							}
//							else {
//								winInfo.Xstart = atof(splitted[2].c_str());
//								winInfo.Ystart = atof(splitted[3].c_str());
//								winInfo.CountObj = 1;
//								cadrInfo.WindowsList.push_back(winInfo);
//							}
//						}
//						cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
//					}
//					else
//						throw logic_error(errorMessage);
//					handler(cadrInfo, pointColor);
//					cadrInfoVec.push_back(cadrInfo);
//					writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
//
//				}
//			}
//		}
//
//		catch (exception &e) {
//			ShowMessage(e.what());
//		}
//
//	}

    	template<class ProtHandler>
	void readBOKZMFProtocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handler, TDateTime& dt) {
		try {
			const string errorMessage =
				string("Cчитывание протокола завершено необычным образом.");
			string line;
			const string dtmi1 =
				"\\b\\'c4\\'ee\\'ef. \\'e8\\'ed\\'f4\\'ee\\'f0\\'ec\\'e0\\'f6\\'e8\\'ff:";
			const string mshior = "Alfa"; // признак начала МШИ ОР слежения
			const string matrixOrient =
				"\\'cc\\'e0\\'f2\\'f0\\'e8\\'f6\\'e0 \\'ee\\'f0\\'e8\\'e5\\'ed\\'f2\\'e0\\'f6\\'e8\\'e8";
			const string codeStatus =
				"\\'ca\\'ee\\'e4 \\'f1\\'ee\\'f1\\'f2\\'ee\\'ff\\'ed\\'e8\\'ff";
			// КС ДМТИ
			const string CS = "Az"; // перед кодом состояния
			const string timePrMSHIOR = "(\\'d2\\'ef\\'f0";
			// время привязки   МШИ ОР
			const string timePrDTMI =
				"\\'ef\\'f0\\'e8\\'e2\\'ff\\'e7\\'ea\\'e8";
			// время привязки ДТМИ
			const string objects = "\\'ee\\'e1\\'fa\\'e5\\'ea\\'f2\\'ee\\'e2";
			const string locTable =
				"\\b0  \\f1\\u8470?     X       Y       \\f0\\'df\\'f0\\'ea\\'ee\\'f1\\'f2\\'fc \\'d7\\'e8\\'f1\\'eb\\'ee \\'fd\\'eb.";
			const string fragTable = "\\par \\b0  \\f1\\u8470?     X       Y  ";
			const string datetime =
				"\\'c1\\'d8\\'c2:";
			const string timePrHMS = "\\'ef\\'f0\\'e8\\'e2\\'ff\\'e7\\'ea\\'e8";

			if (dt.Val == 0 && findWord(in, datetime) != string::npos) {
				string date, time;
				in >> date >> time;
				dt = StrToDateTime(toUString(date + " " + time));
				in.seekg(ios_base::beg);
			}
			else
				throw logic_error("Не найдено стартовое время привязки");

			while (getline(in, line)) {
				CadrInfo cadrInfo;
				cadrInfo.ImageHeight = 512;
				cadrInfo.ImageWidth = 512;

			if (line.find(mshior) != string::npos) {
					TColor pointColor = clBlue;
					if (findLine(in, CS) != string::npos) {
						string status1, status2;
						getline(in, status1);
						getline(in, status2);
						if (!(contains(status1, "e000H") && contains(status2,
							"0005H"))) {
							pointColor = clRed;
						}
					}
					else
						throw logic_error(errorMessage);

					if (findWord(in, timePrMSHIOR) != string::npos) {
						string hms;
						in >> hms >> hms;
						dt = StrToDateTime
							(dt.DateString() + " " + toUString(hms));
						cadrInfo.Time = dt.Val;
					}
					else
						throw logic_error(errorMessage);
						//тут исправить
					if (findLine(in, matrixOrient) != string::npos) {
						for (int i = 0; i < 3; i++) {
							getline(in, line);
							vector<string>splitted = split(line, " ");
							cadrInfo.MatrixOrient[i][0] =
								atof(splitted[1].c_str());
							cadrInfo.MatrixOrient[i][1] =
								atof(splitted[2].c_str());
							cadrInfo.MatrixOrient[i][2] =
								atof(splitted[3].c_str());
						}
						MatrixToEkvAngles(cadrInfo.MatrixOrient,
							cadrInfo.AnglesOrient);
					}

					// число локализ. объектов
					if (findWord(in, "NumLoc") != string::npos) {
						in >> cadrInfo.CountLocalObj;
					}
					else
						throw logic_error(errorMessage);

					// ищем число фрагментов
					if (findWord(in, "NumObj") != string::npos) {
						in >> cadrInfo.CountDeterObj;
					}
					else
						throw logic_error(errorMessage);

					// ищем число распознанных объектов
					if (findWord(in, "NumFrag") != string::npos) {
						// общее число фрагментов
						in >> cadrInfo.CountWindows;
					}
					else
						throw logic_error(errorMessage);

					if (findLine(in, "NumSec") != string::npos) {
						for (int i = 0; i < 3; i++) {
							getline(in, line);
							vector<string>splitted = split(line, " ");
							cadrInfo.OmegaOrient[i] =
								atof(splitted.back().c_str());
						}
					}
					else
						throw logic_error(errorMessage);

					if (findLine(in, locTable) != string::npos) {
						ObjectsInfo objInfo;
						while (getline(in, line) && line != "\\par ") {
							vector<string>splitted = split(line, " ");
							objInfo.X = atof(splitted[2].c_str());
							objInfo.Y = atof(splitted[3].c_str());
							if (objInfo.X == 0 && objInfo.Y == 0)
								break;
							objInfo.Bright = atof(splitted[4].c_str());
							int square = atoi(splitted[5].c_str());
							objInfo.Square = square < 0 ? -square : square;
							cadrInfo.ObjectsList.push_back(objInfo);
						}
						cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
					}
					else
						throw logic_error(errorMessage);

					if (findLine(in, fragTable) != string::npos) {
						WindowsInfo winInfo;
						bool checkFirstRow = false;
						while (getline(in, line) && line != "\\par ") {
							vector<string>splitted = split(line, " ");
							if (!checkFirstRow) {
								winInfo.Xstart = atof(splitted[3].c_str());
								winInfo.Ystart = atof(splitted[4].c_str());
								winInfo.CountObj = 1;
								checkFirstRow = true;
							}
							else {
								winInfo.Xstart = atof(splitted[2].c_str());
								winInfo.Ystart = atof(splitted[3].c_str());
								winInfo.CountObj = 1;
								cadrInfo.WindowsList.push_back(winInfo);
							}
						}
						cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
					}
					else
						throw logic_error(errorMessage);
					handler(cadrInfo, pointColor);
					cadrInfoVec.push_back(cadrInfo);
					writeProtocolToIKI(cadrInfo, cadrInfoVec.size());

				}
			}
		}

		catch (exception &e) {
			ShowMessage(e.what());
		}

	}

	template<class ProtHandler>
	void readBOKZM2Protocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handler, TDateTime& dt) {
		string line;
		while (getline(in, line)) {
			TColor pointColor = clBlue;
			if (contains(line, ";;ДТМИ;;;")) {
				CadrInfo cadrInfo;
				cadrInfo.ImageHeight = 512;
				cadrInfo.ImageWidth = 512;
				vector<string>splitted = split(line, ";");
				vector<string>::iterator delFrom =
					remove(splitted.begin(), splitted.end(), ";");
				splitted.erase(delFrom, splitted.end());
				UnicodeString date = toUString(splitted[0]);
				UnicodeString time = toUString(splitted[1]);
				cadrInfo.Time = StrToDateTime(date + " " + time).Val;
				int status1 = strtoul(string(splitted[6]).c_str(), NULL, 16);
				if (!((status1 >> 15) & 1))
					// если 16 бит не установлен, то это НО
				{
					if (contains(splitted[7], "000"))
						cadrInfo.DeviceInfo = "HO 4";
					else if (contains(splitted[7], "010"))
						cadrInfo.DeviceInfo = "HO 1";
					else if (contains(splitted[7], "020")) {
						if (cadrInfoVec.back().DeviceInfo == "HO 1")
							cadrInfo.DeviceInfo = "HO 2";
						else
							cadrInfo.DeviceInfo = "HO 3";
					}
				}
				else {
					cadrInfo.DeviceInfo = "SLEZH";
				}

				cadrInfo.StatOrient =
					strtoul(string(splitted[7].substr(2, 3) + "0").c_str(),
					NULL, 16);
				if (!contains(splitted[7], "000") || !contains(splitted[7],
					"010") || !contains(splitted[7], "020")) {
					TColor pointColor = clRed;
				}
				cadrInfo.CountLocalObj = atoi(splitted[20].c_str());
				cadrInfo.CountDeterObj = atoi(splitted[21].c_str());
				cadrInfo.CountWindows = atoi(splitted[22].c_str());
				for (int i = 25; i < 29; i++) {
					cadrInfo.QuatOrient[i - 25] = atof(splitted[i].c_str());
				}

				quatToMatr(cadrInfo.QuatOrient, cadrInfo.MatrixOrient);
				MatrixToEkvAngles(cadrInfo.MatrixOrient, cadrInfo.AnglesOrient);

				// cadrInfo.MeanErrorXY = atof(splitted[29].c_str());
				for (int i = 30; i < 34; i++) {
					vector<string>splittedOmega = split(splitted[i], "'");
					cadrInfo.OmegaOrient[i - 30] =
						atof(splittedOmega.back().c_str());
				}

				const int objCount = 14;
				for (int i = 0; i < objCount; i++) {
					ObjectsInfo objInfo;
					getline(in, line);
					splitted = split(line, ";;");
					string::iterator delStr =
						remove(splitted[3].begin(), splitted[3].end(), ';');
					splitted[3].erase(delStr, splitted[3].end());
					vector<string>splittedObj = split(splitted[3], " ");
					if (splittedObj.empty())
						break;
					objInfo.X = atof(splittedObj[0].c_str());
					if (objInfo.X == 0)
						break;
					objInfo.Y = atof(splittedObj[1].c_str());

					// тут непростой пробел, А НЕРАЗРЫВНЫЙ ПРОБЕЛ (ЗА ЧТО??)
					if (contains(splittedObj[2], " ")) {
						delStr =
							remove(splittedObj[2].begin(),
							splittedObj[2].end(), ' ');
						splittedObj[2].erase(delStr, splittedObj[2].end());
					}
					objInfo.Bright = atoi(splittedObj[2].c_str());
					objInfo.Square = atoi(splittedObj[3].c_str());
					cadrInfo.ObjectsList.push_back(objInfo);
				}
				cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
				handler(cadrInfo);
				cadrInfoVec.push_back(cadrInfo);
				writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
			}
		}
	}

	template<class ProtHandler>
	void readBOKZM2LocProtocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handler, TDateTime& dt) {
		string line;
		while (getline(in, line)) {
			TColor pointColor = clBlue;
			if (contains(line, "Расшифровка;МПИ;;ДТМИ Лок;") && !contains(line,
				"Таблица")) {
				CadrInfo cadrInfo;
				cadrInfo.ImageHeight = 512;
				cadrInfo.ImageWidth = 512;
				vector<string>splitted = split(line, ";");
				vector<string>::iterator delFrom =
					remove(splitted.begin(), splitted.end(), ";");
				splitted.erase(delFrom, splitted.end());
				UnicodeString date = toUString(splitted[0]);
				UnicodeString time = toUString(splitted[1]);
				cadrInfo.Time = StrToDateTime(date + " " + time).Val;
				if (!contains(splitted[6], "000") || !contains(splitted[6],
					"010") || !contains(splitted[6], "020")) {
					TColor pointColor = clRed;
				}
				cadrInfo.CountLocalObj = atoi(splitted[10].c_str());

				const int objCount = 20;
				for (int i = 0; i < objCount; i++) {
					ObjectsInfo objInfo;
					getline(in, line);
					splitted = split(line, ";;");
					vector<string>splittedObj = split(splitted[1], ";");
					if (splittedObj.empty())
						break;
					objInfo.X = atof(splittedObj[2].c_str());
					if (objInfo.X == 0)
						break;
					objInfo.Y = atof(splittedObj[3].c_str());

					// тут непростой пробел, А НЕРАЗРЫВНЫЙ ПРОБЕЛ (ЗА ЧТО??)
					if (contains(splittedObj[4], " ")) {
						string::iterator delStr =
							remove(splittedObj[4].begin(),
							splittedObj[4].end(), ' ');
						splittedObj[4].erase(delStr, splittedObj[4].end());
					}
					objInfo.Bright = atof(splittedObj[4].c_str());
					objInfo.Square = atoi(splittedObj[5].c_str());
					cadrInfo.ObjectsList.push_back(objInfo);
				}
				if (cadrInfo.ObjectsList.size() < 4) {
					continue;
				}
				cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();

				handler(cadrInfo);
				cadrInfoVec.push_back(cadrInfo);
				writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
			}
		}
	}

}

// ---------------------------------------------------------------------------
#endif






