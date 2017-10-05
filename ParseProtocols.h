// ---------------------------------------------------------------------------

#ifndef ParseProtocolsH
#define ParseProtocolsH

#include "InfoCadrFormat.h"
#include <system.hpp>
#include "AddString.h"
#include "SimplePlotter.h"
#include "iki_img.cpp"
#include <DateUtils.hpp>
#include <System.IOUtils.hpp>

#define MAX_STAT 	   16
#define MAX_OBJ_DTMI   15
#define MAX_OBJ_MLOC   32
#define MAX_OBJ_BOKZM  30
#define MAX_WINDOW 	   16

using namespace std;
using namespace add_string;

namespace parse_prot {

	struct SHTMI1 {
		string timeBOKZ;
		unsigned short status1, status2, post;
		int serialNumber;
		float Foc, Xg, Yg;
		unsigned short timeExp, Mean, Sigma, countDefect;
		unsigned short CRC, Date, Version;
	};

	struct SHTMI2 {
		string timeBOKZ;
		unsigned short status1, status2, post;
		int serialNumber, timeExp;
		int cntCommandWord, cntCallNO, cntNOtoSLEZH;
		int cntCallTO, cntTOtoSLEZH, cntSLEZH;
		int cntStatOrient[MAX_STAT];
	};

	struct DTMI {
		string timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, timeExp;
		unsigned short nLocalObj, nDeterObj, nWindows, epsillon;
		float dTimeBOKZ, LocalList[MAX_OBJ_DTMI][4];
		float quatBoard[4], omega[3], centrWindow[MAX_WINDOW][2];
		unsigned short levelWindow[MAX_WINDOW], nObjectWindow[MAX_WINDOW];
		unsigned long timeQuatLast;
		float quatLast[4], Epoch;
		unsigned short nLocal[2], maxHist;
		unsigned short maxHistX, maxHistY;
		unsigned short test_short, Reserved[10];
	};


	struct DTMI_BOKZM {
		float timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber;
		float foc, X0, Y0;
		unsigned short timeExp;
		unsigned short nLocalObj, nDeterObj;
		float LocalList[MAX_OBJ_BOKZM][4];
	};

	struct LOC {
		string timeBOKZ;
		unsigned short status1, status2;
		unsigned short serialNumber, timeExp;
		unsigned short nLocalObj, nFixedObj;
		float MeanC, SigmaC;
		float LocalList[32][4];
		unsigned short Reserved[2];
	};


   static  string arrStatErrorEng[MAX_STAT] = {
		{"EC1"}, {"EC2"}, {"EC3"}, {"EC4"}, {"EC5"}, {"EC6"}, {"EC7"}, {"EC8"},
		{"EC9"}, {"EC10"}, {"EC11"}, {"EC12"}, {"EC13"}, {"EC14"}, {"EC15"},
		{"EC16"}};
	static string arrStatErrorRus[MAX_STAT] = {
		{"ЕС1"}, {"ЕС2"}, {"ЕС3"}, {"ЕС4"}, {"ЕС5"}, {"ЕС6"}, {"ЕС7"}, {"ЕС8"},
		{"ЕС9"}, {"ЕС10"}, {"ЕС11"}, {"ЕС12"}, {"ЕС13"}, {"ЕС14"}, {"ЕС15"},
		{"ЕС16"}};

	unsigned int ReadBinaryString(string binaryString);
	AnsiString GetTimeString(unsigned long time);

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

	void ConvertDataDTMI_BOKZM(struct DTMI_BOKZM tmi, struct CadrInfo &mCadr) ;

	void PrintDTMI_BOKZM(ofstream &file, struct DTMI_BOKZM tmi);

	// проверяем содержит ли протокол режим локализации
	bool checkLocFile(ifstream& in);

	void writeBOKZ1000ProtocolToIKI (CadrInfo& cadrInfo, bool InfoVecEmpty, TDateTime& startDate, double& timeStep, unsigned int& counter);

	void readBOKZ601000MKO(ifstream& in, vector <CadrInfo>& cadrInfoVec, unsigned int& counter);

	CadrInfo convertIKIFormatToInfoCadr(IKI_img* reader, bool CompareIKIRes = false);

	template <class ProtHandler>
	void readBOKZ60LocProtocol(ifstream& in, vector <CadrInfo>& cadrInfoVec, ProtHandler handle)
{
	string line;
	string errorMessage = string("Cчитывание протокола завершено необычным образом. "
				"Возможно работа прибора была остановлена.");
	while(getline(in,line))
	{
		   if(line.find("Состав ДТМИ ЛОК:") != string::npos)
		   {
				CadrInfo cadrInfo;
				cadrInfo.CountBlock = 0;
				cadrInfo.CountStars = 0;
				// считываем время привязки
				if(findWord(in,"информации") != string::npos)
				{
					in >> cadrInfo.Time;
				}
				else throw logic_error(errorMessage);

				// ищем число локализованных объектов
				if(findWord(in, "объектов") != string::npos)
				{
					in >> cadrInfo.CountLocalObj;
				}
				else throw logic_error(errorMessage);

				  //ищем число распознанных объектов
				if(findWord(in,"объектов") != string::npos)
				{
					in >> cadrInfo.CountDeterObj;
				}
				else throw logic_error(errorMessage);

				// ищем начало массива лок
				if(findLine(in,"	Х			Y			I			N") != string::npos)
				{
					vector<string> splittedLocData;
					const int сountLocObj = cadrInfo.CountLocalObj;
					ObjectsInfo objInfo;
					for(int i = 0 ; i < сountLocObj; i ++)
					{
						getline(in,line);
						// см. эту строку в протоколе, чтобы понять почему так
						splittedLocData = split(line, ")\t");
						splittedLocData = split(splittedLocData[1], "\t");

						objInfo.X = atof (splittedLocData[0].c_str());
						objInfo.Y = atof (splittedLocData[1].c_str());
						objInfo.Bright = atof (splittedLocData[2].c_str());
						objInfo.Square = atoi(splittedLocData[3].c_str());
						objInfo.StarID = 0;
						objInfo.Mv = 0;
						objInfo.Sp[0]='_';
						objInfo.Sp[1]='_';
						objInfo.Dx = 0;
						objInfo.Dy = 0;

						cadrInfo.ObjectsList.push_back(objInfo);
					}


				}
				else throw logic_error(errorMessage);

				cadrInfo.CountBlock = 0;
				cadrInfo.CountWindows = 0;
				cadrInfo.CountStars = 0;
				cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
				cadrInfo.SizeStarsList = 0;


				handle(cadrInfo);
				cadrInfoVec.push_back(cadrInfo);
		   }

	}
}

	template <class ProtHandler>
	void readBOKZ60Protocol(ifstream& in, vector <CadrInfo>& cadrInfoVec,ProtHandler handle)
{
	string line;
	string errorMessage = string("Cчитывание протокола завершено необычным образом. "
				"Возможно работа прибора была остановлена.");
	while(getline(in,line))
	{

		if(line.find("и) Ориентация				определена") != string::npos)
		{
		   CadrInfo cadrInfo;
		   // ищем время привязки
			if(findWord(in,"информации") != string::npos)
			{
				in >> cadrInfo.Time;
			}
		   else throw logic_error(errorMessage);


		   if(findLine(in,"4) Кватернион ориентации, Qо") != string::npos)
		   {

				for(int i = 0; i < 4; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t\t\t\t\t");
					cadrInfo.QuatOrient[i] = atof(splittedStr[1].c_str());
				}

				double matrixOfOrientation [3][3];
				quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
				MatrixToEkvAngles(matrixOfOrientation, cadrInfo.AnglesOrient);

		   }
		   else throw logic_error(errorMessage);


		   if(findWord(in,"объектов") != string::npos)
		   {
				in >> cadrInfo.CountLocalObj;
		   }
		   else throw logic_error(errorMessage);


		   //ищем число распознанных объектов
		   if(findWord(in,"объектов") != string::npos)
		   {
				in >> cadrInfo.CountDeterObj;
		   }
		   else throw logic_error(errorMessage);

		   // ищем начало массива лок
		   if(findLine(in,"	Х			Y			I			N") != string::npos)
		   {
				vector<string> splittedLocData;
				const int сountLocObj = cadrInfo.CountLocalObj;
				ObjectsInfo objInfo;

				for(int i = 0 ; i < сountLocObj; i ++)
				{
						getline(in,line);
						splittedLocData = split(line,")\t");
						splittedLocData = split(splittedLocData[1],"\t\t");
						objInfo.X = atof (splittedLocData[0].c_str());
						objInfo.Y = atof (splittedLocData[1].c_str());
						objInfo.Bright = atof(splittedLocData[2].c_str());
						objInfo.Square = atoi(splittedLocData[3].c_str());
						objInfo.StarID = 0;
						objInfo.Mv = 0;
						objInfo.Sp[0]='_';
						objInfo.Sp[1]='_';
						objInfo.Dx = 0;
						objInfo.Dy = 0;
						cadrInfo.ObjectsList.push_back(objInfo);
				}
		   }
			else throw logic_error(errorMessage);

		   if(findLine(in,"14) Проекции угловой скорости на оси ПСК") != string::npos)
		   {
				for(int i = 0; i < 3; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.OmegaOrient[i] = atof(splittedStr[1].c_str());
				}

		   }
		   else throw logic_error(errorMessage);


		   if(findLine(in,"15) Координаты центров фрагментов") != string::npos)
		   {
				WindowsInfo winInfo;
				const int maxCountOfObjects = 16;
				cadrInfo.CountWindows = maxCountOfObjects;
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t");

					if(atoi(splittedStr[1].c_str()) == 0)
					{
						cadrInfo.CountWindows = i;
						break;
					}

					winInfo.Xstart = (atof(splittedStr[1].c_str()));
					winInfo.Ystart = (atof(splittedStr[2].c_str()));
					winInfo.Mean = 0;
					winInfo.Sigma = 0;
					winInfo.Mv = 0;
					cadrInfo.WindowsList.push_back(winInfo);
				}

		   }
		  else throw logic_error(errorMessage);


		   if(findLine(in,"16) Значение порогов во фрагментах") != string::npos)
		   {
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.WindowsList[i].Level = atoi(splittedStr[1].c_str());
				}

		   }
		  else throw logic_error(errorMessage);


		   if(findLine(in,"17) Количество объектов во фрагментах") != string::npos)
		   {
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.WindowsList[i].CountObj = atoi(splittedStr[1].c_str());
				}

		   }
		   else throw logic_error(errorMessage);

		   cadrInfo.CountBlock = 0;
		   cadrInfo.CountStars = 0;
		   cadrInfo.SizeStarsList = 0;
		   cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
		   cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();

		   handle (cadrInfo);
		   cadrInfoVec.push_back(cadrInfo);
		}

	}

}

	template <class ProtHandler>
	void readmBOKZ2VProtocol(ifstream& in,vector <CadrInfo>& cadrInfoVec, ProtHandler handle)
{
	string line;
	string errorMessage = string("Cчитывание протокола завершено необычным образом. "
				"Возможно работа прибора была остановлена.");


	while(getline(in,line))
	{

		if(line.find("2 Число принятых пикселей на первом кадре") != string::npos)
		{
			// если нашли строку с числом принятых пикселей, проверяем, что их число не ноль
			// если ноль, пропускаем такт
			if(split(line, "\t").back() == "0")
			{
				continue;
			}
		   CadrInfo cadrInfo;
		   cadrInfo.ImageHeight = 1024;
		   cadrInfo.ImageWidth = 1024;

			// ищем начало массива лок и фрагментов
		   if(findLine(in,"18, 19	Массив локализованных объектов на 1-ом кадре") != string::npos)
		   {
				vector <string> splittedStr;
				const int maxCountLocObj = 15;
				ObjectsInfo objInfo;
				WindowsInfo winInfo;
				StarsInfo starsInfo;

				for(int i = 0 ; i < maxCountLocObj; i ++)
				{
						getline(in,line);
						splittedStr = split(line,"\t");

						// если всё-таки объектов меньше
						if(atof (splittedStr[0].c_str()) == 0)
						{
							  break;
						}
						// заполняем все о лок
						objInfo.X = atof (splittedStr[0].c_str());
						objInfo.Y = atof (splittedStr[1].c_str());
						objInfo.Bright = atof(splittedStr[2].c_str());
						objInfo.Square = atoi (splittedStr[3].c_str());
						objInfo.StarID = 0;
						objInfo.Mv = 0;
						objInfo.Sp[0]='_';
						objInfo.Sp[1]='_';
						objInfo.Dx = 0;
						objInfo.Dy = 0;
						cadrInfo.ObjectsList.push_back(objInfo);

						// заполняем всё о фрагментах
						winInfo.Mv = 0;
						winInfo.Mean = (atof(splittedStr[6].c_str()));
						winInfo.Sigma = (atof(splittedStr[7].c_str()));
						winInfo.Level = (atof(splittedStr[8].c_str()));
						winInfo.CountObj = atoi(splittedStr[9].c_str());
						unsigned short windowSize = atoi(splittedStr[10].c_str());

						switch(windowSize)
						{
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
						winInfo.Xstart = (atof(splittedStr[4].c_str())) - winInfo.Width/2;
						winInfo.Ystart = (atof(splittedStr[5].c_str())) - winInfo.Height/2;
						cadrInfo.WindowsList.push_back(winInfo);

						starsInfo.X = (atof(splittedStr[4].c_str()));
						starsInfo.Y = (atof(splittedStr[5].c_str()));
						cadrInfo.StarsList.push_back(starsInfo);

				}

			if(findLine(in,"18, 19	Массив локализованных объектов на 2-ом кадре") != string::npos)
		   {
				for(int i = 0 ; i < maxCountLocObj; i ++)
				{
					getline(in,line);
					splittedStr = split(line,"\t");

					// если всё-таки объектов меньше
					if(atof (splittedStr[0].c_str()) == 0)
					{
						break;
					}
						// заполняем все о лок
					objInfo.X = atof (splittedStr[0].c_str());
					objInfo.Y = atof (splittedStr[1].c_str());
					objInfo.Bright = atof(splittedStr[2].c_str());
					objInfo.Square = atoi (splittedStr[3].c_str());
					objInfo.StarID = 0;
					objInfo.Mv = 0;
					objInfo.Sp[0] = '_';
					objInfo.Sp[1] = '_';
					objInfo.Dx = 0;
					objInfo.Dy = 0;
					cadrInfo.ObjectsList.push_back(objInfo);
				}
		   }


		   }
			else throw logic_error(errorMessage);

			cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
			cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
			cadrInfo.SizeStarsList = cadrInfo.StarsList.size();
			GetImageBright(cadrInfo);

		   // ищем время привязки в секундах
			if(findWord(in, "информации") != string::npos)
			{
				int secs = 0;
				int msecs = 0;
				int prevPos = in.tellg();

				in>>line;
				if(line == "с")
				{
					in >> secs;

					getline(in, line);
					getline(in, line);

					vector <string> splittedStr = split(line, "\t");
					msecs =  atoi (splittedStr[1].c_str());
				}

				else
				{
					// если там не "c", возвращаемся назад
					in.seekg(prevPos);
					in >> secs;
					// ищем миллисекунды
					findWord(in, "информации");
					in >> msecs;

				}
				cadrInfo.Time =  static_cast <double> (secs) + static_cast <double> (msecs) / 1000;
			}
		   else throw logic_error(errorMessage);

		   if(findLine(in,"6) Кватернион ориентации, Qо") != string::npos)
		   {

				for(int i = 0; i < 4; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t\t\t\t");
					cadrInfo.QuatOrient[i] = atof(splittedStr[1].c_str());
				}

				double matrixOfOrientation [3][3];
				quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
				MatrixToEkvAngles(matrixOfOrientation, cadrInfo.AnglesOrient);


		   }
		   else throw logic_error(errorMessage);



		   if(findLine(in,"Угловая скорость по оптическим измерениям в проекциях на оси ПСК") != string::npos)
		   {
				for(int i = 0; i < 3; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t\t\t\t");
					cadrInfo.OmegaOrient[i] = atof(splittedStr[1].c_str());
				}

		   }
		   else throw logic_error(errorMessage);


		   if(findWord(in, "Tcmv") != string::npos)
		   {
				in >> cadrInfo.MatrixTemp;
		   }
		   else throw logic_error(errorMessage);

		   // ищем число спроектированных звезд
		   if(findWord(in,"NumProgFrag") != string::npos)
		   {
				in >> cadrInfo.CountStars;
				if(cadrInfo.CountStars <= 0) continue;
		   }
		   else throw logic_error(errorMessage);

		   // ищем число фрагментов
		   if(findWord(in,"NumFrag") != string::npos)
		   {
				// общее число фрагментов
				in >> cadrInfo.CountWindows;
				if(cadrInfo.CountWindows <= 0) continue;
		   }
		   else throw logic_error(errorMessage);


		   // число локализ. объектов
		   if(findWord(in,"NumLoc[0]") != string::npos)
		   {
				in >> cadrInfo.CountLocalObj;
				if(cadrInfo.CountLocalObj <= 0) continue;
		   }
		   else throw logic_error(errorMessage);


		   //ищем число распознанных объектов
		   if(findWord(in,"NumDet") != string::npos)
		   {
				in >> cadrInfo.CountDeterObj;
				if(cadrInfo.CountDeterObj <= 0) continue;
		   }
		   else throw logic_error(errorMessage);


		   if(findWord(in,"m_cur") != string::npos)
		   {
				in >> cadrInfo.MeanErrorXY;
		   }
		   else throw logic_error(errorMessage);

		   cadrInfo.CountBlock = 0;
		   cadrInfo.CountLines = 0;

		   handle (cadrInfo);
		   cadrInfoVec.push_back(cadrInfo);
		}

	}

}

	template <class ProtHandler>
	void readBOKZ601000Protocol(ifstream& in, vector <CadrInfo>& cadrInfoVec, unsigned int& counter, TDateTime& startDate, ProtHandler handler)
{

	try
	{

	const string errorMessage = string("Cчитывание протокола завершено необычным образом.");
	string line;
	static double timeStep = 0.25;
	static bool NeedNextFile = false;

	if (NeedNextFile)
	{
		if (findWord(in, "такта:") != string::npos)
		{
			int TickNumber = 0;
			in >> TickNumber;
			if (TickNumber != cadrInfoVec.back().FrameNumber)
			{
				throw logic_error(errorMessage);
			}

		}
		else throw logic_error(errorMessage);

		if(findLine(in,"5) Кватернион ориентации, Qо") != string::npos)
		{

			for(int i = 0; i < 4; i++)
			{
				getline(in,line);
				vector<string> splittedStr = split(line,"\t\t\t\t");
				cadrInfoVec.back().QuatOrient[i] = atof(splittedStr[1].c_str());
			}

			double matrixOfOrientation [3][3];
			quatToMatr(cadrInfoVec.back().QuatOrient, matrixOfOrientation);
			MatrixToEkvAngles(matrixOfOrientation, cadrInfoVec.back().AnglesOrient);

		}
		else throw logic_error(errorMessage); // протоколы кончаются на ДТМИ

		if(findLine(in, "Угловая скорость по оптическим измерениям в проекциях на оси ПСК") != string::npos)
		{
			for(int i = 0; i < 3; i++)
			{
				getline(in,line);
				vector <string> splittedStr = split(line,"\t\t\t\t");
				cadrInfoVec.back().OmegaOrient[i] = atof(splittedStr[1].c_str());
			}

		}
		else throw logic_error(errorMessage);
		writeBOKZ1000ProtocolToIKI (cadrInfoVec.back(), cadrInfoVec.empty(), startDate, timeStep, counter);
		NeedNextFile = false;
	}



	while (getline(in,line))
	{
		TColor pointColor = clBlue;
	if (line.find("Номер такта:") != string::npos)
	{
		// номер такта в ДТМИ
		int TickNumber = 0;
		vector <string> splitTickNumber = split(line, " ");
		TickNumber = atoi(splitTickNumber[2].c_str());

		if (findLine (in, "Состав ДТМИ:") != string::npos)
		{
			CadrInfo cadrInfo;
			cadrInfo.FrameNumber = TickNumber;
			cadrInfo.ImageHeight = 1024;
			cadrInfo.ImageWidth = 1024;
			cadrInfo.CountBlock = 0;
			cadrInfo.CountLines = 0;
			cadrInfo.CountStars = 0;
			cadrInfo.SizeStarsList = 0;
			cadrInfo.SizeWindowsList = 0;

			//  время привязки в секундах
			if(findWord(in, "информации") != string::npos)
			{
				in >> cadrInfo.Time;
			}
			else throw logic_error(errorMessage);


			if (findWord(in, "состояния") != string::npos)
			{
				string status;
				in >> status >> status;

				if (findWord(in, "состояния") != string::npos)
				{
					string status2;
					in >> status2 >> status2;

					 // ТО
					if (status == "ec00")
					{
						if (status2.substr(0, 2) != "00")
						{
						   pointColor = clRed;
						}
					}
					//  НО
					else if (status == "2400")
					{
						pointColor = clGreen;
						if (status2.substr(0, 2) == "0c"  || status2.substr(0, 2) == "01")
						{
							continue;
						}
						else if (status2.substr(0, 2) != "00")
						{
						   pointColor = clRed;
						}
					}
					else if (status == "0000" && status2 == "0000" || status == "c400")
					{
						continue;
					}
					else
					{
						pointColor = clRed;
					}
				}
				else throw logic_error(errorMessage);
			}
			else throw logic_error(errorMessage);
			// локализованные

			if (findWord(in, "объектов") != string::npos)
			{
				in >> cadrInfo.CountLocalObj;
			}
			else throw logic_error(errorMessage);


			if (findWord(in, "объектов") != string::npos)
			{
				in >> cadrInfo.CountDeterObj;
			}
			else throw logic_error(errorMessage);


			if (findWord(in, "фрагментов") != string::npos)
			{
				in >> cadrInfo.CountWindows;
			}
			else throw logic_error(errorMessage);

			if (findWord(in, "распознавания") != string::npos)
			{
				in >> cadrInfo.Epsilon;
			}
			else throw logic_error(errorMessage);


			if(findLine(in,"	Х			Y			I			N") != string::npos)
			{
				vector<string> splittedLocData;
				const int сountLocObj = cadrInfo.CountLocalObj;
				ObjectsInfo objInfo;
				for(int i = 0 ; i < сountLocObj; i ++)
				{
					getline(in,line);
					// см. эту строку в протоколе, чтобы понять почему так
					splittedLocData = split(line, ")\t");
					splittedLocData = split(splittedLocData[1], "\t");

					objInfo.X = atof (splittedLocData[0].c_str());
					objInfo.Y = atof (splittedLocData[1].c_str());

					if (objInfo.X == 0 && objInfo.Y == 0) {
						break;
					}

					objInfo.Bright = atof (splittedLocData[2].c_str());
					objInfo.Square = atoi(splittedLocData[3].c_str());

					// чтобы отобразить объект как распознаный
					if (objInfo.Square < 0)  {
						objInfo.StarID = 1;
					}

					objInfo.StarID = 0;
					objInfo.Mv = 0;
					objInfo.Sp[0] = '_';
					objInfo.Sp[1] =  '_';
					objInfo.Dx = 0;
					objInfo.Dy = 0;
					cadrInfo.ObjectsList.push_back(objInfo);
				}
				cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
			}
			else throw logic_error(errorMessage);


//			if(findLine(in,"16) Значение порогов во фрагментах") != string::npos)
//		   {
//				for(int i = 0; i < cadrInfo.CountWindows; i++)
//				{
//					WindowsInfo winInfo;
//					getline(in,line);
//					vector<string> splittedStr = split(line,"\t");
//					winInfo.Level =  atoi(splittedStr[1].c_str());
//					if (winInfo.Level == 0) {
//						break;
//					}
//					cadrInfo.WindowsList.push_back(winInfo);
//				}
//
//		   }
//		  else throw logic_error(errorMessage);
//
//
//		   if(findLine(in,"17) Количество объектов во фрагментах") != string::npos)
//		   {
//				for(int i = 0; i < cadrInfo.WindowsList.size(); i++)
//				{
//					getline(in,line);
//					vector <string> splittedStr = split(line,"\t");
//					cadrInfo.WindowsList[i].CountObj = atoi(splittedStr[1].c_str());
//				}
//
//		   }
//		   else throw logic_error(errorMessage);


			if (findWord(in, "такта:") != string::npos)
			{
				int TickNumberSecond = 0;
				in >> TickNumberSecond;
				// если была рассинхронизация
				if (cadrInfo.FrameNumber != TickNumberSecond)
				{
					timeStep += timeStep;
					continue;
				}
			}
			else
			{
				NeedNextFile = true;
				cadrInfoVec.push_back(move(cadrInfo));
				break;
			}



		   if(findLine(in,"5) Кватернион ориентации, Qо") != string::npos)
		   {

				for(int i = 0; i < 4; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t\t\t\t");
					cadrInfo.QuatOrient[i] = atof(splittedStr[1].c_str());
				}

				double matrixOfOrientation [3][3];
				quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
				MatrixToEkvAngles(matrixOfOrientation, cadrInfo.AnglesOrient);
		   }
		   else throw logic_error(errorMessage);// протоколы кончаются на ДТМИ


		   if(findLine(in, "Угловая скорость по оптическим измерениям в проекциях на оси ПСК") != string::npos)
		   {
				for(int i = 0; i < 3; i++)
				{
					getline(in,line);
					vector <string> splittedStr = split(line,"\t\t\t\t");
					cadrInfo.OmegaOrient[i] = atof(splittedStr[1].c_str());
				}

		   }
		   else throw logic_error(errorMessage);

		   writeBOKZ1000ProtocolToIKI (cadrInfo, cadrInfoVec.empty(), startDate, timeStep, counter);

		   handler(cadrInfo, pointColor);

		   cadrInfoVec.push_back(cadrInfo);

		}

	}
	}
	}

	catch (exception &e)
	{
		ShowMessage(e.what());
	}

}


}
// ---------------------------------------------------------------------------
#endif
