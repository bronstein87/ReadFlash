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

namespace parse_prot
{

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
		{"��1"}, {"��2"}, {"��3"}, {"��4"}, {"��5"}, {"��6"}, {"��7"}, {"��8"},
		{"��9"}, {"��10"}, {"��11"}, {"��12"}, {"��13"}, {"��14"}, {"��15"},
		{"��16"}};

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

	// ��������� �������� �� �������� ����� �����������
	bool checkLocFile(ifstream& in);

	void writeBOKZ1000ProtocolToIKI (CadrInfo& cadrInfo, bool InfoVecEmpty, TDateTime& startDate, double& timeStep, unsigned int& counter);

	void writeProtocolToIKI(CadrInfo& cadrInfo, int counter);

	void readBOKZ601000MKO(ifstream& in, vector <CadrInfo>& cadrInfoVec, unsigned int& counter);

	CadrInfo convertIKIFormatToInfoCadr(IKI_img* reader, bool CompareIKIRes = false);

	template <class ProtHandler>
	void readBOKZ60LocProtocol(ifstream& in, vector <CadrInfo>& cadrInfoVec, ProtHandler handle)
{
	string line;
	string errorMessage = string("C��������� ��������� ��������� ��������� �������. "
				"�������� ������ ������� ���� �����������.");
	while(getline(in,line))
	{
		   if(line.find("������ ���� ���:") != string::npos)
		   {
				CadrInfo cadrInfo;
				cadrInfo.CountBlock = 0;
				cadrInfo.CountStars = 0;
				// ��������� ����� ��������
				if(findWord(in,"����������") != string::npos)
				{
					in >> cadrInfo.Time;
				}
				else throw logic_error(errorMessage);

				// ���� ����� �������������� ��������
				if(findWord(in, "��������") != string::npos)
				{
					in >> cadrInfo.CountLocalObj;
				}
				else throw logic_error(errorMessage);

				  //���� ����� ������������ ��������
				if(findWord(in,"��������") != string::npos)
				{
					in >> cadrInfo.CountDeterObj;
				}
				else throw logic_error(errorMessage);

				// ���� ������ ������� ���
				if(findLine(in,"	�			Y			I			N") != string::npos)
				{
					vector<string> splittedLocData;
					const int �ountLocObj = cadrInfo.CountLocalObj;
					ObjectsInfo objInfo;
					for(int i = 0 ; i < �ountLocObj; i ++)
					{
						getline(in,line);
						// ��. ��� ������ � ���������, ����� ������ ������ ���
						splittedLocData = split(line, ")\t");
						splittedLocData = split(splittedLocData[1], "\t");

						objInfo.X = atof (splittedLocData[0].c_str());
						objInfo.Y = atof (splittedLocData[1].c_str());
						objInfo.Bright = atof (splittedLocData[2].c_str());
						objInfo.Square = atoi(splittedLocData[3].c_str());

						cadrInfo.ObjectsList.push_back(objInfo);
					}


				}
				else throw logic_error(errorMessage);

				cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
				handle(cadrInfo);
				cadrInfoVec.push_back(cadrInfo);
		   }

	}
}

	template <class ProtHandler>
	void readBOKZ60Protocol(ifstream& in, vector <CadrInfo>& cadrInfoVec, ProtHandler handle, TDateTime& startDate)
{

	try
	{
	string line;
	string errorMessage = string("C��������� ��������� ��������� ��������� �������. "
				"�������� ������ ������� ���� �����������.");
	while(getline(in,line))
	{
		TColor PointColor = clBlue;
		if(line.find("������ ��� ��:") != string::npos)
		{
		   CadrInfo cadrInfo;
		   cadrInfo.ImageHeight = 512;
		   cadrInfo.ImageWidth = 512;

		   if(findLine(in, "1) ��� ��������� 1") != string::npos)
		   {
				getline(in, line);
				vector <string> splitted = split(line, "\t\t\t");
				// ���������, ��� ��������� ��� �� 0
				if(splitted[1][3] == '0') continue;
				if(!contains(splitted[1], "010")
				&& !contains(splitted[1], "020")
				&& !contains(splitted[1], "000"))
				{
					   PointColor = clRed;
				}


		   }

		   // ���� ����� ��������
			if(findWord(in, "����������") != string::npos)
			{
				in >> cadrInfo.Time;
				startDate = IncMilliSecond(DateOf(startDate), cadrInfo.Time * 1000);
				if (cadrInfoVec.size() == 0)
				{
					cadrInfo.Time =  startDate.Val;
				}
				else
				{
					cadrInfo.Time = cadrInfoVec.back().Time + (startDate.Val - cadrInfoVec.back().Time);
				}
			}
		   else throw logic_error(errorMessage);


		   if(findLine(in,"4) ���������� ����������, Q�") != string::npos)
		   {

				for(int i = 0; i < 4; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line, "\t\t\t\t\t");
					cadrInfo.QuatOrient[i] = atof(splittedStr[1].c_str());
				}

				double matrixOfOrientation [3][3];
				quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
				MatrixToEkvAngles(matrixOfOrientation, cadrInfo.AnglesOrient);

		   }
		   else throw logic_error(errorMessage);


		   if(findWord(in,"��������") != string::npos)
		   {
				in >> cadrInfo.CountLocalObj;
				if (cadrInfo.CountLocalObj == 0 || cadrInfo.CountLocalObj > 100) {
					continue;
				}
		   }
		   else throw logic_error(errorMessage);


		   //���� ����� ������������ ��������
		   if(findWord(in,"��������") != string::npos)
		   {
				in >> cadrInfo.CountDeterObj;
		   }
		   else throw logic_error(errorMessage);

		   if (findWord(in, "�������������") != string::npos)
			{
				in >> cadrInfo.Epsilon;
			}
			else throw logic_error(errorMessage);

		   // ���� ������ ������� ���
		   if(findLine(in, "	�			Y			I			N") != string::npos)
		   {
				vector <string> splittedLocData;
				const int �ountLocObj = cadrInfo.CountLocalObj;
				ObjectsInfo objInfo;

				for(int i = 0 ; i < �ountLocObj; i ++)
				{
						getline(in,line);
						splittedLocData = split(line,")\t");
						splittedLocData = split(splittedLocData[1],"\t\t");

						if(atof(splittedLocData[0].c_str()) == 0)
						{
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
			else throw logic_error(errorMessage);


		   if(findLine(in,"14) �������� ������� �������� �� ��� ���") != string::npos)
		   {
				for(int i = 0; i < 3; i++)
				{
					getline(in,line);
					vector <string> splittedStr = split(line,"\t");
					cadrInfo.OmegaOrient[i] = atof(splittedStr[1].c_str());
				}

		   }
		   else throw logic_error(errorMessage);


		   if(findLine(in,"15) ���������� ������� ����������") != string::npos)
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

					winInfo.Xstart = atof(splittedStr[1].c_str());
					winInfo.Ystart = atof(splittedStr[2].c_str());
					winInfo.Mean = 0;
					winInfo.Sigma = 0;
					winInfo.Mv = 0;
					cadrInfo.WindowsList.push_back(winInfo);
				}

		   }
		  else throw logic_error(errorMessage);


		   if(findLine(in,"16) �������� ������� �� ����������") != string::npos)
		   {
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.WindowsList[i].Level = atoi(splittedStr[1].c_str());
				}

		   }
		  else throw logic_error(errorMessage);


		   if(findLine(in,"17) ���������� �������� �� ����������") != string::npos)
		   {
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.WindowsList[i].CountObj = atoi(splittedStr[1].c_str());
				}

		   }
		   else throw logic_error(errorMessage);

		   cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
		   cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
		  // cadrInfo.CountLocalObj = cadrInfo.ObjectsList.size();

		   handle (cadrInfo);
		   cadrInfoVec.push_back(cadrInfo);
		   writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
		}

	}
	}
	catch (exception &e)
	{
		ShowMessage(e.what());
	}

}

	template <class ProtHandler>
	void readmBOKZ2VProtocol(ifstream& in,vector <CadrInfo>& cadrInfoVec, ProtHandler handle, TDateTime& startDate)
{
	string line;
	string errorMessage = string("C��������� ��������� ��������� ��������� �������. "
				"�������� ������ ������� ���� �����������.");


	while(getline(in,line))
	{
		TColor PointColor = clBlue;
		if(line.find("2,3	����� �������� ����������	[�]") != string::npos)
		{

		   CadrInfo cadrInfo;
		   cadrInfo.ImageHeight = 1024;
		   cadrInfo.ImageWidth = 1024;

			if (findWord(in, "���������") != string::npos)
			{
				string status1;
				in >> status1 >> status1;
				if (findWord(in, "���������") != string::npos)
				{
					string status2;
					in >> status2 >> status2;
					if (status1 != "EC00" && status2 != "00C3")
					{
						PointColor = clRed;
					} 
				}
				else throw logic_error(errorMessage);

			}
			else throw logic_error(errorMessage);

			
			// ���� ����� ������ � ������ �������� ��������, ���������, ��� �� ����� �� ����
			// ���� ����, ���������� ����
			if (findLine(in, "20 ����� ������� (0..7 ����)") != string::npos)
			{
				getline(in, line);
				vector <string> splitted = split(line, "\t");
				cadrInfo.Epsilon =  atoi(splitted[1].c_str());
			}
			else throw logic_error(errorMessage);
			
			if (findWord(in, "�����") != string::npos)
			{
				int number;
				in >> number;
				if (number == 0) continue;
			}
			else throw logic_error(errorMessage);

			// ���� ������ ������� ��� � ����������
		   if(findLine(in,"18, 19	������ �������������� �������� �� 1-�� �����") != string::npos)
		   {
				vector <string> splittedStr;
				const int maxCountLocObj = 15;
				ObjectsInfo objInfo;
				WindowsInfo winInfo;

				for(int i = 0 ; i < maxCountLocObj; i++)
				{
						getline(in,line);
						splittedStr = split(line,"\t");

						// ���� ��-���� �������� ������
						if(atof (splittedStr[0].c_str()) == 0)
						{
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
						winInfo.Xstart = (atof(splittedStr[4].c_str())) - winInfo.Width / 2;
						winInfo.Ystart = (atof(splittedStr[5].c_str())) - winInfo.Height / 2;
						cadrInfo.WindowsList.push_back(winInfo);

						if (objInfo.Square < 0)  {
							objInfo.StarID = 1;
						}


				}

			if(findLine(in,"18, 19	������ �������������� �������� �� 2-�� �����") != string::npos)
		   {
				for(int i = 0 ; i < maxCountLocObj; i ++)
				{
					getline(in,line);
					splittedStr = split(line,"\t");

					// ���� ��-���� �������� ������
					if(atof (splittedStr[0].c_str()) == 0)
					{
						break;
					}
						// ��������� ��� � ���
					objInfo.X = atof (splittedStr[0].c_str());
					objInfo.Y = atof (splittedStr[1].c_str());
					objInfo.Bright = atof(splittedStr[2].c_str());
					objInfo.Square = atoi (splittedStr[3].c_str());
					cadrInfo.ObjectsList.push_back(objInfo);

					if (objInfo.Square < 0)  {
						objInfo.StarID = 1;
					}
				}
		   }


		   }
			else throw logic_error(errorMessage);

			cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
			cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
			GetImageBright(cadrInfo);

		   // ���� ����� �������� � ��������
			if(findWord(in, "����������") != string::npos)
			{
				int secs = 0;
				int msecs = 0;
				int prevPos = in.tellg();

				in>>line;
				if(line == "�")
				{
					in >> secs;

					getline(in, line);
					getline(in, line);

					vector <string> splittedStr = split(line, "\t");
					msecs =  atoi(splittedStr[1].c_str());
				}

				else
				{
					// ���� ��� �� "c", ������������ �����
					in.seekg(prevPos);
					in >> secs;
					// ���� ������������
					findWord(in, "����������");
					in >> msecs;

				}

				int ms = secs * 1000  + msecs;
				startDate = IncMilliSecond(DateOf(startDate), ms); 	
				if (cadrInfoVec.size() == 0)
				{
					cadrInfo.Time =  startDate.Val;
				}
				else
				{
					cadrInfo.Time = cadrInfoVec.back().Time + (startDate.Val - cadrInfoVec.back().Time);
				}
			}
		   else throw logic_error(errorMessage);

		   if(findLine(in,"6) ���������� ����������, Q�") != string::npos)
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



		   if(findLine(in,"������� �������� �� ���������� ���������� � ��������� �� ��� ���") != string::npos)
		   {
				for(int i = 0; i < 3; i++)
				{
					getline(in,line);
					vector <string> splittedStr = split(line,"\t\t\t\t");
					cadrInfo.OmegaOrient[i] = atof(splittedStr[i].c_str());
				}

		   }
		   else throw logic_error(errorMessage);


		   if(findWord(in, "Tcmv") != string::npos)
		   {
				in >> cadrInfo.MatrixTemp;
		   }
		   else throw logic_error(errorMessage);

		   // ���� ����� ���������������� �����
		   if(findWord(in,"NumProgFrag") != string::npos)
		   {
				in >> cadrInfo.CountStars;
				if(cadrInfo.CountStars <= 0) continue;
		   }
		   else throw logic_error(errorMessage);

		   // ���� ����� ����������
		   if(findWord(in,"NumFrag") != string::npos)
		   {
				// ����� ����� ����������
				in >> cadrInfo.CountWindows;
				if(cadrInfo.CountWindows <= 0) continue;
		   }
		   else throw logic_error(errorMessage);


		   // ����� �������. ��������
		   if(findWord(in, "NumLoc[0]") != string::npos)
		   {
				in >> cadrInfo.CountLocalObj;
				if(cadrInfo.CountLocalObj <= 0) continue;
		   }
		   else throw logic_error(errorMessage);


		   //���� ����� ������������ ��������
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


		   handle (cadrInfo);
		   cadrInfoVec.push_back(cadrInfo);
		   writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
		}

	}

}

	template <class ProtHandler>
	void readBOKZ601000Protocol(ifstream& in, vector <CadrInfo>& cadrInfoVec, unsigned int& counter, TDateTime& startDate, ProtHandler handler)
{

	try
	{

	const string errorMessage = string("C��������� ��������� ��������� ��������� �������.");
	string line;
	static double timeStep = 0.25;
	static bool NeedNextFile = false;

	if (NeedNextFile)
	{
		if (findWord(in, "�����:") != string::npos)
		{
			int TickNumber = 0;
			in >> TickNumber;
			if (TickNumber != cadrInfoVec.back().FrameNumber)
			{
				throw logic_error(errorMessage);
			}

		}
		else throw logic_error(errorMessage);

		if(findLine(in,"5) ���������� ����������, Q�") != string::npos)
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
		else throw logic_error(errorMessage); // ��������� ��������� �� ����

		if(findLine(in, "������� �������� �� ���������� ���������� � ��������� �� ��� ���") != string::npos)
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
	if (line.find("����� �����:") != string::npos)
	{
		// ����� ����� � ����
		int TickNumber = 0;
		vector <string> splitTickNumber = split(line, " ");
		TickNumber = atoi(splitTickNumber[2].c_str());

		if (findLine (in, "������ ����:") != string::npos)
		{
			CadrInfo cadrInfo;
			cadrInfo.FrameNumber = TickNumber;
			cadrInfo.ImageHeight = 1024;
			cadrInfo.ImageWidth = 1024;

			//  ����� �������� � ��������
			if(findWord(in, "����������") != string::npos)
			{
				in >> cadrInfo.Time;
			}
			else throw logic_error(errorMessage);


			if (findWord(in, "���������") != string::npos)
			{
				string status;
				in >> status >> status;

				if (findWord(in, "���������") != string::npos)
				{
					string status2;
					in >> status2 >> status2;

					 // ��
					if (status == "ec00")
					{
						if (status2.substr(0, 2) != "00")
						{
						   pointColor = clRed;
						}
					}
					//  ��
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
			// ��������������

			if (findWord(in, "��������") != string::npos)
			{
				in >> cadrInfo.CountLocalObj;
			}
			else throw logic_error(errorMessage);


			if (findWord(in, "��������") != string::npos)
			{
				in >> cadrInfo.CountDeterObj;
			}
			else throw logic_error(errorMessage);


			if (findWord(in, "����������") != string::npos)
			{
				in >> cadrInfo.CountWindows;
			}
			else throw logic_error(errorMessage);

			if (findWord(in, "�������������") != string::npos)
			{
				in >> cadrInfo.Epsilon;
			}
			else throw logic_error(errorMessage);


			if(findLine(in,"	�			Y			I			N") != string::npos)
			{
				vector<string> splittedLocData;
				const int �ountLocObj = cadrInfo.CountLocalObj;
				ObjectsInfo objInfo;
				for(int i = 0 ; i < �ountLocObj; i ++)
				{
					getline(in,line);
					// ��. ��� ������ � ���������, ����� ������ ������ ���
					splittedLocData = split(line, ")\t");
					splittedLocData = split(splittedLocData[1], "\t");

					objInfo.X = atof (splittedLocData[0].c_str());
					objInfo.Y = atof (splittedLocData[1].c_str());

					if (objInfo.X == 0 && objInfo.Y == 0) {
						break;
					}

					objInfo.Bright = atof (splittedLocData[2].c_str());
					objInfo.Square = atoi(splittedLocData[3].c_str());

					// ����� ���������� ������ ��� �����������
					if (objInfo.Square < 0)  {
						objInfo.StarID = 1;
					}

					cadrInfo.ObjectsList.push_back(objInfo);
				}
				cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
			}
			else throw logic_error(errorMessage);


//			if(findLine(in,"16) �������� ������� �� ����������") != string::npos)
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
//		   if(findLine(in,"17) ���������� �������� �� ����������") != string::npos)
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


			if (findWord(in, "�����:") != string::npos)
			{
				int TickNumberSecond = 0;
				in >> TickNumberSecond;
				// ���� ���� ����������������
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



		   if(findLine(in,"5) ���������� ����������, Q�") != string::npos)
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
		   else throw logic_error(errorMessage);// ��������� ��������� �� ����


		   if(findLine(in, "������� �������� �� ���������� ���������� � ��������� �� ��� ���") != string::npos)
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

	template <class ProtHandler>
	void readBOKZMFProtocol(ifstream& in, vector <CadrInfo>& cadrInfoVec, ProtHandler handler, TDateTime& dt)
{
	try
	{
		const string errorMessage = string("C��������� ��������� ��������� ��������� �������.");
		string line;
		const string dtmi1 = "\\par \\b C\\'ee\\'f1\\'f2\\'e0\\'e2 \\'c4\\'d2\\'cc\\'c81:";
		const string mshior = "Alfa"; // ������� ������ ��� �� ��������
		const string matrixOrient = "\\'cc\\'e0\\'f2\\'f0\\'e8\\'f6\\'e0 \\'ee\\'f0\\'e8\\'e5\\'ed\\'f2\\'e0\\'f6\\'e8\\'e8";
		const string codeStatus = "\\'ca\\'ee\\'e4 \\'f1\\'ee\\'f1\\'f2\\'ee\\'ff\\'ed\\'e8\\'ff"; // �� ����
		const string CS = "Az";  // ����� ����� ���������
		const string timePrMSHIOR = "(\\'d2\\'ef\\'f0"; // ����� ��������   ��� ��
		const string timePrDTMI = "\\'ef\\'f0\\'e8\\'e2\\'ff\\'e7\\'ea\\'e8"; // ����� �������� ����
		const string objects = "\\'ee\\'e1\\'fa\\'e5\\'ea\\'f2\\'ee\\'e2";
		const string locTable = "\\par \\b0  \\f1\\u8470?     X       Y       \\f0\\'df\\'f0\\'ea\\'ee\\'f1\\'f2\\'fc \\'d7\\'e8\\'f1\\'eb\\'ee \\'fd\\'eb.";
		const string fragTable = "\\par \\b0  \\f1\\u8470?     X       Y  ";
		const string datetime = "\\viewkind4\\uc1\\pard\\b\\f0\\fs20\\'d3\\'f7\\'e0\\'f1\\'f2\\'ee\\'ea";
		const string timePrHMS = "\\'ef\\'f0\\'e8\\'e2\\'ff\\'e7\\'ea\\'e8";

		if (dt.Val == 0 && findLine(in, datetime) != string::npos)
		{
			 getline(in,line);
			 getline(in,line);
			 vector <string> splitted = split(line, " ");
			 dt = StrToDateTime(toUString(splitted[2] + " " + splitted[3]));
			 in.seekg(ios_base::beg);
		}
		else throw logic_error("�� ������� ��������� ����� ��������");

		while (getline(in,line))
		{
			CadrInfo cadrInfo;
			cadrInfo.ImageHeight = 512;
		    cadrInfo.ImageWidth = 512;
			if (line.find(dtmi1) != string::npos)
			{
				if (findWord(in, timePrDTMI) != string::npos)
				{
					  string hms;
					  in >> hms >> hms;
					  dt = StrToDateTime(dt.DateString() + " " + toUString(hms));
					  cadrInfo.Time = dt.Val;
				}
				else throw logic_error(errorMessage);
				
				if (findWord(in, objects) != string::npos)  // ���
				{
					  in >> cadrInfo.CountLocalObj;
				}
				else throw logic_error(errorMessage);

				if (findWord(in, objects) != string::npos)  // ����.
				{
					  in >> cadrInfo.CountDeterObj;
				}
				else throw logic_error(errorMessage);

				if(findLine(in, locTable) != string::npos)
				{
					ObjectsInfo objInfo;
					while (getline(in,line) && line != "\\par ")
					{
						vector <string> splitted = split(line, " ");
						objInfo.X = atof (splitted[2].c_str());
						objInfo.Y = atof (splitted[3].c_str());
						if (objInfo.X == 0 && objInfo.Y == 0) 
							break;
						objInfo.Bright = atof(splitted[4].c_str());
						objInfo.Square = atoi (splitted[5].c_str());
						cadrInfo.ObjectsList.push_back(objInfo);
					}
					cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
				} 
				else throw logic_error(errorMessage);
				handler(cadrInfo, clBlue);
				cadrInfoVec.push_back(cadrInfo);
				writeProtocolToIKI(cadrInfo, cadrInfoVec.size());

			}
			else if (line.find(mshior) != string::npos)
			{
				TColor pointColor = clBlue;
				if (findLine(in, CS) != string::npos)
				{
					string status1, status2;
					getline(in, status1);
					getline(in, status2);
					if (!(contains(status1, "e000H") && contains(status2,"0005H")))
					{
						pointColor = clRed;
					}
				}
				else throw logic_error(errorMessage);

				if (findWord(in, timePrMSHIOR) != string::npos) 
				{
					string hms;
					in >> hms >> hms;
					dt = StrToDateTime(dt.DateString() + " " + toUString(hms));
					cadrInfo.Time = dt.Val;
				}
				else throw logic_error(errorMessage);

				if (findLine(in, matrixOrient) != string::npos)
				{
					for (int i = 0; i < 3; i++)
					{
						getline(in, line);
						vector <string> splitted = split(line, " ");
						cadrInfo.MatrixOrient[i][0] = atof(splitted[1].c_str());
						cadrInfo.MatrixOrient[i][1] = atof(splitted[2].c_str());
						cadrInfo.MatrixOrient[i][2] = atof(splitted[3].c_str());
					}
					MatrixToEkvAngles(cadrInfo.MatrixOrient, cadrInfo.AnglesOrient);
				}

				 // ����� �������. ��������
				if(findWord(in, "NumLoc") != string::npos)
				{
					in >> cadrInfo.CountLocalObj;
				}
				else throw logic_error(errorMessage);

		   		   // ���� ����� ����������
				if(findWord(in,"NumObj") != string::npos)
				{
					in >> cadrInfo.CountDeterObj;
				}
				else throw logic_error(errorMessage);


				//���� ����� ������������ ��������
				if(findWord(in,"NumFrag") != string::npos)
				{
					// ����� ����� ����������
					in >> cadrInfo.CountWindows;
				}
				 else throw logic_error(errorMessage);

				if (findLine(in, "NumSec") != string::npos)
				{
					for (int i = 0; i < 3; i++)
					{
						getline(in, line);
						vector <string> splitted = split(line, " ");
						cadrInfo.OmegaOrient[i] = atof(splitted.back().c_str());
					}
				}
				else throw logic_error(errorMessage);

				if(findLine(in, locTable) != string::npos)
				{
					ObjectsInfo objInfo;
					while (getline(in,line) && line != "\\par ")
					{
						vector <string> splitted = split(line, " ");
						objInfo.X = atof (splitted[2].c_str());
						objInfo.Y = atof (splitted[3].c_str());
						if (objInfo.X == 0 && objInfo.Y == 0) 
							break;
						objInfo.Bright = atof(splitted[4].c_str());
						objInfo.Square = atoi (splitted[5].c_str());
						cadrInfo.ObjectsList.push_back(objInfo);
					}
					cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
				}
				else throw logic_error(errorMessage);

				if(findLine(in, fragTable) != string::npos)
				{
					WindowsInfo winInfo;
					bool checkFirstRow = false;
					while (getline(in,line) && line != "\\par ")
					{
						vector <string> splitted = split(line, " ");
						if (!checkFirstRow)
						{
							winInfo.Xstart = atof (splitted[3].c_str());
							winInfo.Ystart = atof (splitted[4].c_str());
							winInfo.CountObj = 1;
							checkFirstRow = true;
						}
						else
						{
							winInfo.Xstart = atof (splitted[2].c_str());
							winInfo.Ystart = atof (splitted[3].c_str());
							winInfo.CountObj = 1;
							cadrInfo.WindowsList.push_back(winInfo);
						}
					}
					cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
				}
				else throw logic_error(errorMessage);
				handler(cadrInfo, pointColor);
				cadrInfoVec.push_back(cadrInfo);
				writeProtocolToIKI(cadrInfo, cadrInfoVec.size());
				
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
