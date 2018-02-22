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



	static string arrStatErrorEng[MAX_STAT] = {
		{"EC1"}, {"EC2"}, {"EC3"}, {"EC4"}, {"EC5"}, {"EC6"}, {"EC7"}, {"EC8"},
		{"EC9"}, {"EC10"}, {"EC11"}, {"EC12"}, {"EC13"}, {"EC14"}, {"EC15"},
		{"EC16"}};
	static string arrStatErrorRus[MAX_STAT] = {
		{"ЕС1"}, {"ЕС2"}, {"ЕС3"}, {"ЕС4"}, {"ЕС5"}, {"ЕС6"}, {"ЕС7"}, {"ЕС8"},
		{"ЕС9"}, {"ЕС10"}, {"ЕС11"}, {"ЕС12"}, {"ЕС13"}, {"ЕС14"}, {"ЕС15"},
		{"ЕС16"}};

	void SwapShort(short *word1, short *word2);

	unsigned int ReadBinaryString(string binaryString);

	unsigned int StringToDayTime(string _line);

	AnsiString DayTimeToString(unsigned int time);

	int TryReadSHTMI1(ifstream &finp, struct SHTMI1 &tmi);

	int TryReadSHTMI2(ifstream &finp, struct SHTMI2 &tmi);

	int TryReadDTMI(ifstream &finp, struct DTMI &tmi);

	void PrintSHTMI1(ofstream &file, struct SHTMI1 tmi);

	void PrintSHTMI2(ofstream &file, struct SHTMI2 tmi);

	void PrintDTMI(ofstream &file, struct DTMI tmi);

	void PrintLOC(ofstream &file, struct LOC tmi);

	void PrintLocalDTMI(struct DTMI tmi);

	void PrintLocalMLOC(struct LOC tmi);

	void ConvertDataDTMI(struct DTMI tmi, struct CadrInfo &mCadr);

	void ConvertDataLOC(struct LOC tmi, struct CadrInfo &mCadr);

	int ReadTMIArray(ifstream &_inp, string _keyWord, unsigned short *arrayTMI,
		const int _sizeArray);

	void ConvertDataDTMI_BOKZM(struct DTMI_BOKZM tmi, struct CadrInfo &mCadr);

	void PrintMSHI_BOKZM(ofstream &file, struct MSHI_BOKZM tmi);

	void PrintSHTMI1_BOKZM(ofstream &file, struct SHTMI1_BOKZM tmi);

	void PrintSHTMI2_BOKZM(ofstream &file, struct SHTMI2_BOKZM tmi);

	void PrintDTMI_BOKZM(ofstream &file, struct DTMI_BOKZM tmi);

	void PrintLocalDTMI_BOKZM(struct DTMI_BOKZM tmi);

	void PrintDTMI_M2(ofstream &file, struct DTMI_M2 tmi);

	// проверяем содержит ли протокол режим локализации
	bool checkLocFile(ifstream& in);

	bool checkM2Loc(ifstream& in);

	void writeBOKZ1000ProtocolToIKI(CadrInfo& cadrInfo, bool InfoVecEmpty,
		TDateTime& startDate, double& timeStep, unsigned int& counter);

	void writeProtocolToIKI(CadrInfo& cadrInfo, int counter);

	void readBOKZ601000MKO(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		unsigned int& counter);

	CadrInfo convertIKIFormatToInfoCadr(IKI_img* reader,
		bool CompareIKIRes = false);

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
	void readBOKZ60Protocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handle, TDateTime& startDate) {
		try {
			string line;
			string errorMessage =
				string("Cчитывание протокола завершено необычным образом. "
				"Возможно работа прибора была остановлена.");
			while (getline(in, line)) {
				TColor PointColor = clBlue;
				if (line.find("Состав ДТМИ:") != string::npos) {
					CadrInfo cadrInfo;
					cadrInfo.ImageHeight = 512;
					cadrInfo.ImageWidth = 512;

					// ищем время привязки
					if (findWord(in, "информации") != string::npos) {
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

					if (findLine(in, "4) Код состояния 1") != string::npos) {
						getline(in, line);
						vector<string>splitted = split(line, "\t\t\t");
						cadrInfo.StatOrient =
							strtoul(string(splitted[1].substr(2, 3) + "0").c_str
							(), NULL, 16);
						// проверяем, что последний УСД не 0

						// if(splitted[1][3] == '0') continue;
						if (!contains(splitted[1], "010") && !contains
							(splitted[1], "020") && !contains(splitted[1],
							"000")) {
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
						"	Х			Y			I			N")
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

					handle(cadrInfo);
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

			const string errorMessage =
				string("Cчитывание протокола завершено необычным образом.");
			string line;
			static double timeStep = 0.25;
			static bool NeedNextFile = false;

			if (NeedNextFile) {
				if (findWord(in, "такта:") != string::npos) {
					int TickNumber = 0;
					in >> TickNumber;
					if (TickNumber != cadrInfoVec.back().FrameNumber) {
						throw logic_error(errorMessage);
					}

				}
				else
					throw logic_error(errorMessage);

				if (findLine(in, "5) Кватернион ориентации, Qо")
					!= string::npos) {

					for (int i = 0; i < 4; i++) {
						getline(in, line);
						vector<string>splittedStr = split(line, "\t\t\t\t");
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
						vector<string>splittedStr = split(line, "\t\t\t\t");
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

						// время привязки в секундах
						if (findWord(in, "информации") != string::npos) {
							in >> cadrInfo.Time;
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
									if (status2.substr(0, 2) != "00") {
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
									else if (status2.substr(0, 2) != "00") {
										pointColor = clRed;
									}
								}
								else if (status == "0000" && status2 ==
									"0000" || status == "c400") {
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

						if (findWord(in, "объектов") != string::npos) {
							in >> cadrInfo.CountLocalObj;
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

						if (findLine(in,
							"	Х			Y			I			N")
							!= string::npos) {
							vector<string>splittedLocData;
							const int сountLocObj = cadrInfo.CountLocalObj;
							ObjectsInfo objInfo;
							for (int i = 0; i < сountLocObj; i++) {
								getline(in, line);
								// см. эту строку в протоколе, чтобы понять почему так
								splittedLocData = split(line, ")\t");
								splittedLocData =
									split(splittedLocData[1], "\t");

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

						// if(findLine(in,"16) Значение порогов во фрагментах") != string::npos)
						// {
						// for(int i = 0; i < cadrInfo.CountWindows; i++)
						// {
						// WindowsInfo winInfo;
						// getline(in,line);
						// vector<string> splittedStr = split(line,"\t");
						// winInfo.Level =  atoi(splittedStr[1].c_str());
						// if (winInfo.Level == 0) {
						// break;
						// }
						// cadrInfo.WindowsList.push_back(winInfo);
						// }
						//
						// }
						// else throw logic_error(errorMessage);
						//
						//
						// if(findLine(in,"17) Количество объектов во фрагментах") != string::npos)
						// {
						// for(int i = 0; i < cadrInfo.WindowsList.size(); i++)
						// {
						// getline(in,line);
						// vector <string> splittedStr = split(line,"\t");
						// cadrInfo.WindowsList[i].CountObj = atoi(splittedStr[1].c_str());
						// }
						//
						// }
						// else throw logic_error(errorMessage);

						if (findWord(in, "такта:") != string::npos) {
							int TickNumberSecond = 0;
							in >> TickNumberSecond;
							// если была рассинхронизация
							if (cadrInfo.FrameNumber != TickNumberSecond) {
								timeStep += timeStep;
								continue;
							}
						}
						else {
							NeedNextFile = true;
							cadrInfoVec.push_back(move(cadrInfo));
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

	template<class ProtHandler>
	void readBOKZMFProtocol(ifstream& in, vector<CadrInfo>& cadrInfoVec,
		ProtHandler handler, TDateTime& dt) {
		try {
			const string errorMessage =
				string("Cчитывание протокола завершено необычным образом.");
			string line;
			const string dtmi1 =
				"\\par \\b C\\'ee\\'f1\\'f2\\'e0\\'e2 \\'c4\\'d2\\'cc\\'c81:";
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
				"\\par \\b0  \\f1\\u8470?     X       Y       \\f0\\'df\\'f0\\'ea\\'ee\\'f1\\'f2\\'fc \\'d7\\'e8\\'f1\\'eb\\'ee \\'fd\\'eb.";
			const string fragTable = "\\par \\b0  \\f1\\u8470?     X       Y  ";
			const string datetime =
				"\\viewkind4\\uc1\\pard\\b\\f0\\fs20\\'d3\\'f7\\'e0\\'f1\\'f2\\'ee\\'ea";
			const string timePrHMS = "\\'ef\\'f0\\'e8\\'e2\\'ff\\'e7\\'ea\\'e8";

			if (dt.Val == 0 && findLine(in, datetime) != string::npos) {
				getline(in, line);
				getline(in, line);
				vector<string>splitted = split(line, " ");
				dt = StrToDateTime(toUString(splitted[2] + " " + splitted[3]));
				in.seekg(ios_base::beg);
			}
			else
				throw logic_error("Не найдено стартовое время привязки");

			while (getline(in, line)) {
				CadrInfo cadrInfo;
				cadrInfo.ImageHeight = 512;
				cadrInfo.ImageWidth = 512;
				if (line.find(dtmi1) != string::npos) {
					if (findWord(in, timePrDTMI) != string::npos) {
						string hms;
						in >> hms >> hms;
						dt = StrToDateTime
							(dt.DateString() + " " + toUString(hms));
						cadrInfo.Time = dt.Val;
					}
					else
						throw logic_error(errorMessage);

					if (findWord(in, objects) != string::npos) // лок
					{
						in >> cadrInfo.CountLocalObj;
					}
					else
						throw logic_error(errorMessage);

					if (findWord(in, objects) != string::npos) // расп.
					{
						in >> cadrInfo.CountDeterObj;
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
							objInfo.Square = atoi(splitted[5].c_str());
							cadrInfo.ObjectsList.push_back(objInfo);
						}
						cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
					}
					else
						throw logic_error(errorMessage);
					handler(cadrInfo, clBlue);
					cadrInfoVec.push_back(cadrInfo);
					writeProtocolToIKI(cadrInfo, cadrInfoVec.size());

				}
				else if (line.find(mshior) != string::npos) {
					TColor pointColor = clBlue;
					if (findLine(in, CS) != string::npos) {
						string status1, status2;
						getline(in, status1);
						getline(in, status2);
						if (!(contains(status1, "e000H")
						&& contains(status2,"0005H"))) {
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
							objInfo.Square = atoi(splitted[5].c_str());
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
			if (contains(line, "Расшифровка;МПИ;;ДТМИ Лок;")
			&& !contains(line, "Таблица")) {
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
				if (!contains(splitted[6], "000")
				|| !contains(splitted[6],"010")
				|| !contains(splitted[6], "020")) {
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
