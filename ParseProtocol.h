// ---------------------------------------------------------------------------

#ifndef ParseProtocolH
#define ParseProtocolH

#include "InfoCadrFormat.h"
#include <system.hpp>
#include "AddString.h"
#include "SimplePlotter.h"
#include "iki_img.h"
#include <DateUtils.hpp>
#include <System.IOUtils.hpp>
#include <math.h>
#include <algorithm>
#include "MathM.h"

#define MAX_STAT 	   16
#define MAX_OBJ_DTMI   15
#define MAX_OBJ_MLOC   32
#define MAX_OBJ_BOKZM  30
#define MAX_WINDOW 	   16

using namespace std;
using namespace add_string;

namespace parse_prot {
	struct stLocal {
		float x, y;
		float bright, size;
	};

	struct DTMI_M2 {
		unsigned long timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, POST, timeExp;
		unsigned short nLocalObj1, nLocalObj2;
		unsigned short nLocal[2], meanCadr[2], maxHist;
		unsigned short maxHistX, maxHistY;
		unsigned short nStoreObj, nDeterObj, nWindows, nSec;
		unsigned long timeQuatLast;
		float quatLast[4], m_cur;
		float omega[3], Vline[3];
		float quatBoard[4], Epoch;
		stLocal LocalList1[15], LocalList2[15], ResultList[12];
		unsigned char nObjectWindow[16];
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
		unsigned short test_short, Reserved[10];
	};

	struct LOC {
		// string timeBOKZ;
		unsigned int timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, timeExp;
		unsigned short nLocalObj, nFixedObj;
		float MeanC, SigmaC;
		float LocalList[32][4];
		unsigned short Reserved[2];
	};

	struct MSHI_BOKZM {
		unsigned short status1, status2;
		float timeBOKZ;
		float Mornt[3][3];
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

	struct DTMI_BOKZM {
		float timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber;
		float Foc, Xg, Yg;
		unsigned short timeExp;
		unsigned short nLocalObj, nDeterObj;
		float LocalList[MAX_OBJ_BOKZM][4];
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
		float LocalList[MAX_OBJ_BOKZM][4];
	};

	struct TFRAGBOKZM {
		float fragList [MAX_OBJ_BOKZM][2];
    };

	static string arrStatErrorEng[MAX_STAT] = {
		{"EC1"}, {"EC2"}, {"EC3"}, {"EC4"}, {"EC5"}, {"EC6"}, {"EC7"}, {"EC8"},
		{"EC9"}, {"EC10"}, {"EC11"}, {"EC12"}, {"EC13"}, {"EC14"}, {"EC15"},
		{"EC16"}};
	static string arrStatErrorRus[MAX_STAT] = {
		{"��1"}, {"��2"}, {"��3"}, {"��4"}, {"��5"}, {"��6"}, {"��7"}, {"��8"},
		{"��9"}, {"��10"}, {"��11"}, {"��12"}, {"��13"}, {"��14"}, {"��15"},
		{"��16"}};

	void SwapShort(short *word1, short *word2);

	void CheckFileName(AnsiString &fileName);

	unsigned int ReadBinaryString(string binaryString);

	unsigned int StringToDayTime(string _line);

	AnsiString DayTimeToString(unsigned int time);

	int StopReadArray(string line);

	void ClearSHTMI1(struct SHTMI1 &tmi);

	void ClearSHTMI2(struct SHTMI2 &tmi);

	void ClearDTMI(struct DTMI &tmi);

    void ClearLOC(struct LOC &tmi);

	int TryReadSHTMI1(ifstream &finp, struct SHTMI1 &tmi);

	int TryReadSHTMI2(ifstream &finp, struct SHTMI2 &tmi);

	int TryReadDTMI(ifstream &finp, struct DTMI &tmi);

	int TryReadLOC(ifstream &finp, struct LOC &tmi);

	void PrintMSHI(ofstream &file, struct MSHI tmi, TDateTime curDate);
	void PrintLogMSHI(ofstream &file, struct MSHI tmi, TDateTime curDate, bool create);

	void PrintSHTMI1(ofstream &file, struct SHTMI1 tmi);
	void PrintLogSHTMI1(ofstream &file, struct SHTMI1 tmi, bool create);

	void PrintSHTMI2(ofstream &file, struct SHTMI2 tmi, TDateTime curDate);
	void PrintLogSHTMI2(ofstream &file, struct SHTMI2 tmi, TDateTime curDate, bool create);

	void PrintDTMI(ofstream &file, struct DTMI tmi);

	void PrintLOC(ofstream &file, struct LOC tmi);

	void PrintLocalDTMI(AnsiString fileDir, TDateTime curTime, struct DTMI tmi);

	void PrintLocalMLOC(AnsiString fileDir, TDateTime curTime, struct LOC tmi);

	void ConvertDataDTMI(struct DTMI tmi, struct CadrInfo &mCadr);

	void ConvertDataLOC(struct LOC tmi, struct CadrInfo &mCadr);

	int ReadTMIArray(ifstream &_inp, string _keyWord, unsigned short *arrayTMI,
		const int _sizeArray);

	void ConvertDataDTMI_BOKZM(struct DTMI_BOKZM tmi, struct CadrInfo &mCadr);

	void PrintMSHI_BOKZM(ofstream &file, struct MSHI_BOKZM tmi);

	void PrintSHTMI1_BOKZM(ofstream &file, struct SHTMI1_BOKZM tmi);

	void PrintSHTMI2_BOKZM(ofstream &file, struct SHTMI2_BOKZM tmi);

	void PrintDTMI_BOKZM(ofstream &file, struct DTMI_BOKZM tmi);

	void PrintLocalDTMI_BOKZM(AnsiString fileName, struct DTMI_BOKZM tmi);

	void PrintDTMI_M2(ofstream &file, struct DTMI_M2 tmi);

	// ��������� �������� �� �������� ����� �����������
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
			string("C��������� ��������� ��������� ��������� �������. "
			"�������� ������ ������� ���� �����������.");
		while (getline(in, line)) {
			if (line.find("������ ���� ���:") != string::npos) {
				CadrInfo cadrInfo;
				cadrInfo.CountBlock = 0;
				cadrInfo.CountStars = 0;
				cadrInfo.ImageHeight = 512;
				cadrInfo.ImageWidth = 512;
				// ��������� ����� ��������
				if (findWord(in, "����������") != string::npos) {

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

				// ���� ����� �������������� ��������
				if (findWord(in, "��������") != string::npos) {
					in >> cadrInfo.CountLocalObj;
				}
				else
					throw logic_error(errorMessage);

				// ���� ����� ������������ ��������
				if (findWord(in, "��������") != string::npos) {
					in >> cadrInfo.CountDeterObj;
				}
				else
					throw logic_error(errorMessage);

				// ���� ������ ������� ���
				if (findLine(in,
					"	�			Y			I			N") != string::npos)
				{
					vector<string>splittedLocData;
					const int �ountLocObj = cadrInfo.CountLocalObj;
					ObjectsInfo objInfo;
					for (int i = 0; i < �ountLocObj; i++) {
						getline(in, line);
						// ��. ��� ������ � ���������, ����� ������ ������ ���
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
				string("C��������� ��������� ��������� ��������� �������. "
				"�������� ������ ������� ���� �����������.");
			while (getline(in, line)) {
				bool error = false;
				TColor PointColor = clGreen;
				if (line.find("������ ��� ��:") != string::npos) {
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

					if (findLine(in, "������� ���������") != string::npos) {
						getline(in, line);
						splitted = split(line, "\t\t\t");
						if (find(err.begin(), err.end(), splitted[1].substr(0,
							2)) != err.end()) {
							error = true;
							PointColor = clRed;
						}

					}

					// ���� ����� ��������
					if (findWord(in, "����������") != string::npos) {
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

					if (findLine(in, "4) ���������� ����������, Q�")
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

					if (findLine(in, "5) ������� �������� �� ����������")
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
				string("C��������� ��������� ��������� ��������� �������. "
				"�������� ������ ������� ���� �����������.");
			while (getline(in, line)) {
				TColor PointColor = clGreen;
				if (line.find("������ ����:") != string::npos) {
					CadrInfo cadrInfo;
					cadrInfo.ImageHeight = 512;
					cadrInfo.ImageWidth = 512;

					// ���� ����� ��������
					if (findWord(in, "����������") != string::npos) {
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

					if (findWord(in, "��������") != string::npos) {
						in >> cadrInfo.CountLocalObj;
					}
					else
						throw logic_error(errorMessage);

					// ���� ����� ������������ ��������
					if (findWord(in, "��������") != string::npos) {
						in >> cadrInfo.CountDeterObj;
					}
					else
						throw logic_error(errorMessage);

					if (findWord(in, "�������������") != string::npos) {
						in >> cadrInfo.Epsilon;
					}
					else
						throw logic_error(errorMessage);

					// ���� ������ ������� ���
					if (findLine(in,
						"	�			Y			I			nPix		ThFrag")
						!= string::npos) {
						vector<string>splittedLocData;
						const int �ountLocObj = 16;
						ObjectsInfo objInfo;

						for (int i = 0; i < �ountLocObj; i++) {
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

					if (findLine(in, "14) �������� ������� �������� �� ��� ���")
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

					if (findLine(in, "15) ���������� ������� ����������")
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

					if (findLine(in, "16) �������� ������� �� ����������")
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

					if (findLine(in, "17) ���������� �������� �� ����������")
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
						"19) ��������� ������� ���������� ����������")
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
					if (findLine(in, "������ ��� ��") != string::npos) {
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
							// ���� 12 ��� �� ����������, �� ��� ��
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
			string("C��������� ��������� ��������� ��������� �������. "
			"�������� ������ ������� ���� �����������.");

		while (getline(in, line)) {
			TColor PointColor = clBlue;
			if (line.find("2,3	����� �������� ����������	[�]")
				!= string::npos) {

				CadrInfo cadrInfo;
				cadrInfo.ImageHeight = 1024;
				cadrInfo.ImageWidth = 1024;

				if (findWord(in, "���������") != string::npos) {
					string status1;
					in >> status1 >> status1;
					if (findWord(in, "���������") != string::npos) {
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

				// ���� ����� ������ � ������ �������� ��������, ���������, ��� �� ����� �� ����
				// ���� ����, ���������� ����
				if (findLine(in, "20 ����� ������� (0..7 ����)")
					!= string::npos) {
					getline(in, line);
					vector<string>splitted = split(line, "\t");
					cadrInfo.Epsilon = atoi(splitted[1].c_str());
				}
				else
					throw logic_error(errorMessage);

				if (findWord(in, "�����") != string::npos) {
					int number;
					in >> number;
					if (number == 0)
						continue;
				}
				else
					throw logic_error(errorMessage);

				// ���� ������ ������� ��� � ����������
				if (findLine(in,
					"18, 19	������ �������������� �������� �� 1-�� �����")
					!= string::npos) {
					vector<string>splittedStr;
					const int maxCountLocObj = 15;
					ObjectsInfo objInfo;
					WindowsInfo winInfo;

					for (int i = 0; i < maxCountLocObj; i++) {
						getline(in, line);
						splittedStr = split(line, "\t");

						// ���� ��-���� �������� ������
						if (atof(splittedStr[0].c_str()) == 0) {
							break;
						}
						// ��������� ��� � ���
						objInfo.X = atof(splittedStr[0].c_str());
						objInfo.Y = atof(splittedStr[1].c_str());
						objInfo.Bright = atof(splittedStr[2].c_str());
						objInfo.Square = atoi(splittedStr[3].c_str());
						cadrInfo.ObjectsList.push_back(objInfo);

						// ��������� �� � ����������
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
						"18, 19	������ �������������� �������� �� 2-�� �����")
						!= string::npos) {
						for (int i = 0; i < maxCountLocObj; i++) {
							getline(in, line);
							splittedStr = split(line, "\t");

							// ���� ��-���� �������� ������
							if (atof(splittedStr[0].c_str()) == 0) {
								break;
							}
							// ��������� ��� � ���
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

				// ���� ����� �������� � ��������
				if (findWord(in, "����������") != string::npos) {
					int secs = 0;
					int msecs = 0;
					int prevPos = in.tellg();

					in >> line;
					if (line == "�") {
						in >> secs;

						getline(in, line);
						getline(in, line);

						vector<string>splittedStr = split(line, "\t");
						msecs = atoi(splittedStr[1].c_str());
					}

					else {
						// ���� ��� �� "c", ������������ �����
						in.seekg(prevPos);
						in >> secs;
						// ���� ������������
						findWord(in, "����������");
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

				if (findLine(in, "6) ���������� ����������, Q�")
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
					"������� �������� �� ���������� ���������� � ��������� �� ��� ���")
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

				// ���� ����� ���������������� �����
				if (findWord(in, "NumProgFrag") != string::npos) {
					in >> cadrInfo.CountStars;
					if (cadrInfo.CountStars <= 0)
						continue;
				}
				else
					throw logic_error(errorMessage);

				// ���� ����� ����������
				if (findWord(in, "NumFrag") != string::npos) {
					// ����� ����� ����������
					in >> cadrInfo.CountWindows;
					if (cadrInfo.CountWindows <= 0)
						continue;
				}
				else
					throw logic_error(errorMessage);

				// ����� �������. ��������
				if (findWord(in, "NumLoc[0]") != string::npos) {
					in >> cadrInfo.CountLocalObj;
					if (cadrInfo.CountLocalObj <= 0)
						continue;
				}
				else
					throw logic_error(errorMessage);

				// ���� ����� ������������ ��������
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
				string("C��������� ��������� ��������� ��������� �������.");
			const maxTimePr = 100000;
			double timeStep = 0.25;
			string line;

			static bool NeedNextFile = false;
			// ��� ��
			if (!cadrInfoVec.empty()) {
				bool readLastData = true;
				if (NeedNextFile) {
					if (findWord(in, "�����:") != string::npos) {
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
						if (findLine(in, "5) ���������� ����������, Q�")
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
						// ��������� ��������� �� ����

						if (findLine(in,
							"������� �������� �� ���������� ���������� � ��������� �� ��� ���")
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
				if (line.find("����� �����:") != string::npos) {
					// ����� ����� � ����
					int TickNumber = 0;
					vector<string>splitTickNumber = split(line, " ");
					TickNumber = atoi(splitTickNumber[2].c_str());

					if (findLine(in, "������ ����:") != string::npos) {
						CadrInfo cadrInfo;
						cadrInfo.FrameNumber = TickNumber;
						cadrInfo.ImageHeight = 1024;
						cadrInfo.ImageWidth = 1024;

						if (findLine(in, "������ ���� 1:") != string::npos) {

						   bool skipDTMI = false;
						   for (int i = 0; i < 2; i++) {
								getline(in, line);
								if (contains(line, "(������)")) {
									skipDTMI = true;
								}
						   }
						   if (skipDTMI) {
							   continue;
						   }

                        }

						// ����� �������� � ��������
						if (findWord(in, "����������") != string::npos) {
							in >> cadrInfo.Time;
							if (cadrInfo.Time > maxTimePr)
								continue;
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "���������") != string::npos) {
							string status;
							in >> status >> status;
							if (findWord(in, "���������") != string::npos) {
								string status2;
								in >> status2 >> status2;
								// ��
								cadrInfo.StatOrient =
									strtoul(status2.c_str(), NULL, 16);
								if (status == "ec00") {
									if (find(err.begin(), err.end(),
										status2.substr(0, 2)) != err.end()) {
										pointColor = clRed;
									}
								}
								// ��
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
								// � ��� ���������� �����
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
						// ��������������
						string fLine;
						if (findLineBack(in, "�������������� ��������", fLine) != string::npos) {
							vector <string> v = split(fLine,"\t");
							cadrInfo.CountLocalObj = atoi(v[1].c_str());
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "��������") != string::npos) {
							in >> cadrInfo.CountDeterObj;
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "����������") != string::npos) {
							in >> cadrInfo.CountWindows;
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "�������������") != string::npos) {
							in >> cadrInfo.Epsilon;
						}
						else
							throw logic_error(errorMessage);

						if (findWord(in, "�������") != string::npos) {
							in >> timeStep;
						}

						if (findLine(in,
							"	�			Y			I			N")
							!= string::npos) {
							vector <string> splittedLocData;
							const int �ountLocObj = cadrInfo.CountLocalObj;
							ObjectsInfo objInfo;
							for (int i = 0; i < �ountLocObj; i++) {
								getline(in, line);
								// ��. ��� ������ � ���������, ����� ������ ������ ���
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

								// ����� ���������� ������ ��� �����������
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

						if (findWord(in, "�����:") != string::npos) {
							// ��� ��
							int TickNumberSecond = 0;
							in >> TickNumberSecond;
							// ���� ���� ����������������
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

						if (findLine(in, "5) ���������� ����������, Q�")
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
						// ��������� ��������� �� ����

						if (findLine(in,
							"������� �������� �� ���������� ���������� � ��������� �� ��� ���")
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
//				string("C��������� ��������� ��������� ��������� �������.");
//			string line;
//			const string dtmi1 =
//				"\\par \\b C\\'ee\\'f1\\'f2\\'e0\\'e2 \\'c4\\'d2\\'cc\\'c81:";
//			const string mshior = "Alfa"; // ������� ������ ��� �� ��������
//			const string matrixOrient =
//				"\\'cc\\'e0\\'f2\\'f0\\'e8\\'f6\\'e0 \\'ee\\'f0\\'e8\\'e5\\'ed\\'f2\\'e0\\'f6\\'e8\\'e8";
//			const string codeStatus =
//				"\\'ca\\'ee\\'e4 \\'f1\\'ee\\'f1\\'f2\\'ee\\'ff\\'ed\\'e8\\'ff";
//			// �� ����
//			const string CS = "Az"; // ����� ����� ���������
//			const string timePrMSHIOR = "(\\'d2\\'ef\\'f0";
//			// ����� ��������   ��� ��
//			const string timePrDTMI =
//				"\\'ef\\'f0\\'e8\\'e2\\'ff\\'e7\\'ea\\'e8";
//			// ����� �������� ����
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
//				throw logic_error("�� ������� ��������� ����� ��������");
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
//					if (findWord(in, objects) != string::npos) // ���
//					{
//						in >> cadrInfo.CountLocalObj;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					if (findWord(in, objects) != string::npos) // ����.
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
//					// ����� �������. ��������
//					if (findWord(in, "NumLoc") != string::npos) {
//						in >> cadrInfo.CountLocalObj;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					// ���� ����� ����������
//					if (findWord(in, "NumObj") != string::npos) {
//						in >> cadrInfo.CountDeterObj;
//					}
//					else
//						throw logic_error(errorMessage);
//
//					// ���� ����� ������������ ��������
//					if (findWord(in, "NumFrag") != string::npos) {
//						// ����� ����� ����������
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
				string("C��������� ��������� ��������� ��������� �������.");
			string line;
			const string dtmi1 =
				"\\b\\'c4\\'ee\\'ef. \\'e8\\'ed\\'f4\\'ee\\'f0\\'ec\\'e0\\'f6\\'e8\\'ff:";
			const string mshior = "Alfa"; // ������� ������ ��� �� ��������
			const string matrixOrient =
				"\\'cc\\'e0\\'f2\\'f0\\'e8\\'f6\\'e0 \\'ee\\'f0\\'e8\\'e5\\'ed\\'f2\\'e0\\'f6\\'e8\\'e8";
			const string codeStatus =
				"\\'ca\\'ee\\'e4 \\'f1\\'ee\\'f1\\'f2\\'ee\\'ff\\'ed\\'e8\\'ff";
			// �� ����
			const string CS = "Az"; // ����� ����� ���������
			const string timePrMSHIOR = "(\\'d2\\'ef\\'f0";
			// ����� ��������   ��� ��
			const string timePrDTMI =
				"\\'ef\\'f0\\'e8\\'e2\\'ff\\'e7\\'ea\\'e8";
			// ����� �������� ����
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
				throw logic_error("�� ������� ��������� ����� ��������");

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
						//��� ���������
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

					// ����� �������. ��������
					if (findWord(in, "NumLoc") != string::npos) {
						in >> cadrInfo.CountLocalObj;
					}
					else
						throw logic_error(errorMessage);

					// ���� ����� ����������
					if (findWord(in, "NumObj") != string::npos) {
						in >> cadrInfo.CountDeterObj;
					}
					else
						throw logic_error(errorMessage);

					// ���� ����� ������������ ��������
					if (findWord(in, "NumFrag") != string::npos) {
						// ����� ����� ����������
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
			if (contains(line, ";;����;;;")) {
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
					// ���� 16 ��� �� ����������, �� ��� ��
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

					// ��� ��������� ������, � ����������� ������ (�� ���??)
					if (contains(splittedObj[2], "�")) {
						delStr =
							remove(splittedObj[2].begin(),
							splittedObj[2].end(), '�');
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
			if (contains(line, "�����������;���;;���� ���;") && !contains(line,
				"�������")) {
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

					// ��� ��������� ������, � ����������� ������ (�� ���??)
					if (contains(splittedObj[4], "�")) {
						string::iterator delStr =
							remove(splittedObj[4].begin(),
							splittedObj[4].end(), '�');
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






