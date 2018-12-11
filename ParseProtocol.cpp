//---------------------------------------------------------------------------

#pragma hdrstop

#include "ParseProtocol.h"
using namespace add_string;
unsigned short TableCRC[256];
typedef unsigned short uint16_t;

namespace parse_prot {

	void MakeCRC16Table(void)
	{
		unsigned short r;
		for(int i = 0; i < 256; i++)
		{
			r = ((unsigned short)i) << 8;
			for(int j = 0; j < 8; j++)
			{
				if (r&(1<<15)) r = (r << 1) ^ 0x1021;
				else r = r << 1;
			}
			TableCRC[i] = r;
		}
	}

	unsigned short GetCRC16(unsigned char *buf, int size)
	{
		unsigned short crc = 0xFFFF;

		while (size--)
		crc = (crc << 8) ^ TableCRC[(crc >> 8) ^ *buf++];

		return crc;
	}

	unsigned short crc16_ccitt(unsigned char *data_p, int length)
	{
		unsigned char x;
		unsigned short crc = 0xFFFF;
		while (length--){
			x = crc >> 8 ^ *data_p++;
			x ^= x>>4;
			crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
		}
		return crc;
	}

	void SwapShort(short *word1, short *word2)
	{
		short buf;
		buf = *word1;
		*word1 = *word2;
		*word2 = buf;
	}

	void CheckFileName(AnsiString &fileName)
	{
		for (int i = 1; i < fileName.Length() + 1; i++) {
			if ((fileName[i] == ':') || (fileName[i] == ';') ||
				(fileName[i] == '?') || (fileName[i] == '>') ||
				(fileName[i] == '<') || (fileName[i] == '=') ||
				(fileName[i] == '/') || (fileName[i] == '\\')) {
					fileName[i] = '_';
			}
		}
	}

	void SetClkBokz(double clk) {
		clkBokz = clk;
	}

	unsigned int ReadBinaryString(string binaryString) {
		string test_str;
		double sum = 0;

		for (int k = 0; k < binaryString.length(); k++) {
			test_str = binaryString[binaryString.length() - k - 1];
			sum += pow(2., k) * atoi(test_str.c_str());
		}
		return (unsigned int) sum;
	}

//-------------Ôóíêöèè äëÿ ÷òåíèÿ ïðîòîêîëîâ ÐÊÖ "Ïðîãðåññ"-------------------//
	TDateTime GetDateTime(TDateTime _zeroDate, unsigned int _timeBOKZ)
	{
		TDateTime curDate;
		curDate.Val = _zeroDate.Val + _timeBOKZ * clkBokz/86400.;
		return curDate;
	}

	unsigned int StringToDayTime(string _line)
	{
		float mday, mhour, mmin, msec;
		unsigned int dayTime;

		if (sscanf(_line.c_str(), " %f/ %f:%f:%f", &mday, &mhour,
						&mmin, &msec) == 4) {
			dayTime = mday * 86400 + mhour * 3600 +  mmin * 60 + msec;
		}
		else if (sscanf(_line.c_str(), " %f-%f:%f:%f", &mday, &mhour,
						&mmin, &msec) == 4) {
			dayTime = mday * 86400 + mhour * 3600 +  mmin * 60 + msec;
		}
		else if (sscanf(_line.c_str(), "%f:%f:%f", &mhour,
						&mmin, &msec) == 3) {
			dayTime = mhour * 3600 + mmin * 60 + msec;
		}
		else  dayTime = 0;
//		dayTime *= 8;
		return dayTime;
	}

	AnsiString DayTimeToString(unsigned int time)
	{
		AnsiString str, sTime = " ";
		int day, hour, min, sec, msec;
		double day_double = time/86400. * clkBokz;///8.;

		day = (int)day_double;
		hour = (day_double - (int)day) * 24 + 1e-7;
		min  = (day_double - day - hour/24.) * 24 * 60 + 1e-7;
		sec  = (day_double - day - hour/24. - min/24./60.) * 86400. + 1e-7;
		msec  = (day_double - day - hour/24. - min/24./60.- sec/24./3600.) * 86400. * 1000. + 1e-7;
		if (day > 0) {
			sTime  = IntToStr(day) + "/";
		}
		if (msec < 1)
			str.sprintf(" %02d:%02d:%02d", hour, min, sec);
		else str.sprintf(" %02d:%02d:%02d.%03d", hour, min, sec, msec);
		sTime += str;
		return sTime;
	}

	AnsiString OutputDateTime(TDateTime _curDate)
	{
		unsigned short d, ms;
		UnicodeString uFormat;
		DecodeTime(_curDate, d, d, d,  ms);
		if (ms < 1) uFormat = "dd.mm.yyyy hh:mm:ss";
		else uFormat = "dd.mm.yyyy hh:mm:ss.zzz";
		return (AnsiString)FormatDateTime(uFormat, _curDate);
	}

	int StopReadArray(string line) {
		if ( (line.find("#") != string::npos) ||
//		if ( (line.find("----") != string::npos) ||
			 (line.find("ÄÒÌÈ") != string::npos) ||
			 (line.find("ØÒÌÈ") != string::npos) ||
			 (line.find("Ó3-") != string::npos)  ||
			 (line.find("U3-") != string::npos) ) {
			return 1;
		}
		else return 0;
	}

	void ClearSHTMI1(struct SHTMI1 &tmi) {
		tmi.timeBOKZ = 0;
		tmi.status1  = 0;
		tmi.status2  = 0;
		tmi.post     = 0;
		tmi.serialNumber = 0;
		tmi.timeExp  = 0;
		tmi.Foc = 0;
		tmi.Xg  = 0;
		tmi.Mean  = 0;
		tmi.Sigma = 0;
		tmi.countDefect = 0;
		tmi.CRC  = 0;
		tmi.Date = 0;
		tmi.Version = 0;
	}

	int TryReadSHTMI1(ifstream &finp, struct SHTMI1 &tmi) {
		string line, word = "";

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;

			if ((word == "ÊÑ1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == "ÊÑ2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "ÑÅÐ") || (word == "CEP") || (word == "ÑEP") || (word == "ÑEÐ")) {
				finp >> word;
				if ((word == "ÍÎÌ") || (word == "HOM")|| (word == "ÍÎM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "ÏÎÑÒ") || (word == "ÏOCT") || (word == "ÏOÑT")) {
				finp >> word;
				tmi.post = ReadBinaryString(word);
			}
			else if ((word == "T") || (word == "Ò")) {
				finp >> word;
				if (word == "ÝÊÑÏ")
					finp >> tmi.timeExp;
				else {
					getline(finp, line, '\n');
					word = word + line;
					tmi.timeBOKZ = StringToDayTime(word);
				}
			}
			else if ((word == "ÔOK") || (word == "ÔOÊ"))
				finp >> tmi.Foc;
			else if ((word == "Õ0") || (word == "X0"))
				finp >> tmi.Xg;
			else if ((word == "Ó0") || (word == "Y0"))
				finp >> tmi.Yg;
			else if ((word == "ÌÒ") || (word == "MT"))
				finp >> tmi.Mean;
			else if ((word == "ÑÒ") || (word == "CT") || (word == "ÑT"))
				finp >> tmi.Sigma;
			else if ((word == "ÍÀÌ") || (word == "HAM")) {
				finp >> word;
				if ((word == "ÄÅÔ") || (word == "ÄEÔ"))
					finp >> tmi.countDefect;
			}
			else if (word == "ÕÑÔ")
				finp >> tmi.CRC;
			else if (word == "ÄÀÒÀ")
				finp >> tmi.Date;
			else if (word == "ÂÅÐÑÈß") {
				finp >> tmi.Version;
				return 1;
			}
//			else
//			{
//				unsigned short testArr[32];
//				int cntWord = 0;
//				while ((cntWord < 32) && (word.find("-----") == string::npos)) {
//					int highByte = ReadBinaryString(word);
//					finp >> word;
//					int lowByte = ReadBinaryString(word);
//					finp >> word;
//					testArr[cntWord] = (highByte << 8) | (lowByte);
//					cntWord++;
//				}
//				if ( (cntWord == 32) && (testArr[0] == 0x42C3) && (testArr[1] == 0x5AAA) ){
//					SwapShort((short *)&testArr[2],  (short *)&testArr[3]);
//					SwapShort((short *)&testArr[8],  (short *)&testArr[9]);
//					SwapShort((short *)&testArr[10], (short *)&testArr[11]);
//					SwapShort((short *)&testArr[12], (short *)&testArr[13]);
//					memcpy(&tmi, &testArr[2], sizeof(tmi));
//					return 1;
//				}
//			}
//			getline(finp, line, '\n');
		}
		return 0;
	}

	void ClearSHTMI2(struct SHTMI2 &tmi) {
		tmi.timeBOKZ = 0;
		tmi.status1  = 0;
		tmi.status2  = 0;
		tmi.post     = 0;
		tmi.serialNumber = 0;
		tmi.timeExp  = 0;
		tmi.cntCommandWord = 0;
		tmi.cntCallNO    = 0;
		tmi.cntNOtoSLEZH = 0;
		tmi.cntCallTO    = 0;
		tmi.cntTOtoSLEZH = 0;
		tmi.cntSLEZH     = 0;

		for (int i = 0; i < MAX_STAT; i++) {
			tmi.cntStatOrient[i] = 0;
		}
	}

	int TryReadSHTMI2(ifstream &finp, struct SHTMI2 &tmi) {
		string line, word = "";

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if ((word == "ÊÑ1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == "ÊÑ2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "ÑÅÐ") || (word == "CEP") || (word == "ÑEP") || (word == "ÑEÐ")) {
				finp >> word;
				if ((word == "ÍÎÌ") || (word == "HOM")|| (word == "ÍÎM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "ÏÎÑÒ") || (word == "ÏOCT") || (word == "ÏOÑT")) {
				finp >> word;
				tmi.post = ReadBinaryString(word);
			}
			else if ((word == "T") || (word == "Ò")) {
				finp >> word;
				if (word == "ÝÊÑÏ")
					finp >> tmi.timeExp;
				else {
					getline(finp, line, '\n');
					word = word + line;
					tmi.timeBOKZ = StringToDayTime(word);
				}
			}
			else if (word == "ÍÀÌ") {
				finp >> word;
				if (word == "ÓÑÄ")
					finp >> tmi.cntCommandWord;
			}
			else if ((word == "×ÈÑË") || (word == "×ÈÑËÎ") || (word == "×ÈCË") || (word == "×ÈCËO")) {
				finp >> word;
				if ((word == "ÍÎ") || (word == "HO")) {
					finp >> tmi.cntCallNO;
				}
				else if (word == "ÍÎÑË") {
					finp >> tmi.cntNOtoSLEZH;
				}
				if ((word == "TO") || (word == "ÒÎ")) {
					finp >> tmi.cntCallTO;
				}
				else if (word == "ÒÎÑË") {
					finp >> tmi.cntTOtoSLEZH;
				}
				else if (word == "ÑËÅÆ") {
					finp >> tmi.cntSLEZH;
					tmi.cntSLEZH *= 2;
				}
			}
			else if ( (word.find("EC") != string::npos)
					  || (word.find("ÅÑ") != string::npos) ) {
					int i = 0, fl_find = 0;
					while ((i < MAX_STAT) && (!fl_find)) {
						if ((word == arrStatErrorRus[i]) ||
							(word == arrStatErrorEng[i])) {
							finp >> tmi.cntStatOrient[i];
							fl_find = 1;
						}
						i++;
					}
					if ((word == arrStatErrorRus[MAX_STAT - 1]) ||
						(word == arrStatErrorEng[MAX_STAT - 1])) {
						return 1;
					}
			}
//			else {
//				unsigned short testArr[32];
//				int cntWord = 0;
//				while ((cntWord < 32) && (word.find("-----") == string::npos)) {
//					int highByte = ReadBinaryString(word);
//					finp >> word;
//					int lowByte = ReadBinaryString(word);
//					finp >> word;
//					testArr[cntWord] = (highByte << 8) | (lowByte);
//					cntWord++;
//				}
//				if ( (cntWord == 32) && (testArr[0] == 0x42C3) && (testArr[1] == 0x6AAA) ){
//					SwapShort((short *)&testArr[2],  (short *)&testArr[3]);
//					SwapShort((short *)&testArr[14], (short *)&testArr[15]);
//					memcpy(&tmi, &testArr[2], sizeof(tmi));
//					return 1;
//				}
//			}
//			getline(finp, line, '\n');
		}
		return 0;
	}

	void ClearDTMI(struct DTMI &tmi) {
		tmi.timeBOKZ = 0;
		tmi.status1  = 0;
		tmi.status2  = 0;
		tmi.serialNumber = 0;
		tmi.timeExp   = 0;
		tmi.nLocalObj = 0;
		tmi.nDeterObj = 0;
		tmi.nWindows  = 0;
		tmi.epsillon  = 0;
		tmi.dTimeBOKZ = 0;

		for (int i = 0; i < MAX_OBJ_DTMI; i++) {
			for (int j = 0; j < 4; j++) {
				tmi.LocalList[i][j] = 0;
			}
		}

		for (int i = 0; i < 3; i++) {
			tmi.omega[i] = 0;
		}

		for (int i = 0; i < MAX_WINDOW; i++) {
			tmi.levelWindow[i]    = 0 ;
			tmi.nObjectWindow[i]  = 0;
			tmi.centrWindow[i][0] = 0;
			tmi.centrWindow[i][1] = 0;
		}

		tmi.timeQuatLast = 0;
		for (int i = 0; i < 4; i++) {
        	tmi.quatBoard[i] = 0;
			tmi.quatLast[i]  = 0;
		}

		tmi.Epoch = 0;
		tmi.nLocal[0] = 0;
		tmi.nLocal[1] = 0;
		tmi.maxHist   = 0;
		tmi.maxHistX  = 0;
		tmi.maxHistY  = 0;
        tmi.test_short = 0;
		for (int i = 0; i < 10; i++) {
			tmi.Reserved[i] = 0;
		}
	}

	int TryReadDTMI(ifstream &finp, struct DTMI &tmi) {
		string line, word, inpstr, test_word, test_str;
		int indexObject = 0, indexParam = 0, intVal, flLow = 1;
		float fl1, fl2, fl3, sum;
		int Stat1, Stat2;

		word = "";
		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if ((word == "ÊÑ1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == "ÊÑ2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "ÑÅÐ") || (word == "CEP") || (word == "ÑEP") || (word == "ÑEÐ")) {
				finp >> word;
				if ((word == "ÍÎÌ") || (word == "HOM")|| (word == "ÍÎM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "HAM") || (word == "ÍÀÌ") || (word == "ÍÀM")) {
				finp >> word;
				if ((word == "ËÎÊ") || (word == "ËOÊ")) {
					finp >> tmi.nLocalObj;
				}
				else if ((word == "ÎÁÆ") || (word == "OÁÆ")) {
					finp >> tmi.nDeterObj;
				}
				else if (word == "ÔÐÀÃ") {
					finp >> tmi.nWindows;
				}
			}
			else if (word == "ÝÏÑÈËÎÍ")
				finp >> tmi.epsillon;
			else if (word == "ÄÅËÜÒÀ") {
				finp >> word;
				if ((word == "T") || (word == "Ò"))
					finp >> tmi.dTimeBOKZ;
			}
			else if ((word == "T") || (word == "Ò")) {
				finp >> word;
				if (word == "ÝÊÑÏ")
					finp >> tmi.timeExp;
				else if (word == "ÏÕÎÊ") {
					getline(finp, line, '\n');
					tmi.timeQuatLast = StringToDayTime(line);
				}
				else {
					getline(finp, line, '\n');
					word = word + line;
					tmi.timeBOKZ = StringToDayTime(word);
				}
			}
			else if (word == "ËÎÊ") {
				getline(finp, line, '\n');
				int nread = sscanf(line.c_str(), "%f%f%f", &fl1, &fl2, &fl3);
				if (nread == 3) {
					indexObject = (short)fl1;
					indexParam  = (short)fl2;
					tmi.LocalList[indexObject][indexParam] = fl3;
				}
				else if (nread == 2) {
					indexObject = (short)(fl1 / 10.);
					indexParam = (short)fl1 % 10;
					tmi.LocalList[indexObject][indexParam] = fl2;
				}
			}
			else if (word == "ÎÌÅÃÀÎ") {
				finp >> indexParam;
				if ((indexParam >= 0) && (indexParam < 3)) {
					finp >> tmi.omega[indexParam];
				}
			}
			else if ((word == "XYC") || (word == "ÕÓÑ")) {
				getline(finp, line, '\n');

				int nread = sscanf(line.c_str(), "%f%f%f", &fl1, &fl2, &fl3);
				if (nread == 3) {
					indexObject = (short)fl1;
					indexParam = (short)fl2;
					tmi.centrWindow[indexObject][indexParam] = fl3;
				}
				else if (nread == 2) {
					indexObject = (short)(fl1 / 10.);
					indexParam = (short)fl1 % 10;
					tmi.centrWindow[indexObject][indexParam] = fl2;
				}

			}
			else if (word == "ÒÍÔÐÀÃ") {
				finp >> indexObject;
				if ((indexObject >= 0) && (indexObject < MAX_WINDOW)) {
					finp >> tmi.levelWindow[indexObject];
				}
			}
			else if (word == "ÎÁÆÔÐÀÃ") {
				finp >> indexObject;
				if ((indexObject >= 0) && (indexObject < MAX_WINDOW)) {
					finp >> tmi.nObjectWindow[indexObject];
				}
			}
			else if (word == "ÏÕÎÊ") {
				finp >> indexParam;
				if ((indexParam >= 0) && (indexParam < 4)) {
					finp >> tmi.quatLast[indexParam];
				}
			}
			else if (word == "ÝÏÎÕÀ") {
				int test_int;
				float test_float;
				finp >> test_int;
				test_float = *(float*)&test_int;
				tmi.Epoch = test_float;
			}
			else if (word == "ÍÓÌË") {
				finp >> indexParam;
				if ((indexParam >= 0) && (indexParam < 2)) {
					finp >> tmi.nLocal[indexParam];
				}
			}
			else if ((word == "MAXH") || (word == "ÌÀÕÍ") || (word == "ÌÀÕÍ")) {
				finp >> tmi.maxHist;
			}
			else if ((word == "ÄÕÌÀÕÍ") || (word == "ÄÕÌÀÕÍ")) {
				finp >> tmi.maxHistX;
			}
			else if ((word == "ÄÓÌÀÕÍ") || (word == "ÄÓÌÀÕÍ")) {
				finp >> tmi.maxHistY;
				return 1;
			}
//			else {
//				unsigned short testArr[32];
//				int cntWord = 0;
//				while ((cntWord < 32) && (word.find("-----") == string::npos)) {
//					int highByte = ReadBinaryString(word);
//					finp >> word;
//					int lowByte = ReadBinaryString(word);
//                    finp >> word;
//					testArr[cntWord] = (highByte << 8) | (lowByte);
//					cntWord++;
//				}
//				if ( (cntWord == 32) && (testArr[0] == 0x42C3) && (testArr[1] == 0x7AAA) ){
//					for (int iWord = 12; iWord <= 30; iWord = iWord + 2) {
//						SwapShort((short *)&testArr[iWord],  (short *)&testArr[iWord+1]);
//					}
//					memcpy(&tmi, &testArr[2], 30 * sizeof(short));
//				}
//
//				while ((line.find("ÄÒÌÈ")==string::npos) && (!finp.eof())) {
//					getline(finp, line, '\n' );
//				}
//				finp >> word;
//					if ( (cntWord == 32) && (testArr[0] == 0x42C3) && (testArr[1] == 0x8AAA) ){
//						for (int iWord = 2; iWord <= 30; iWord = iWord + 2) {
//							SwapShort((short *)&testArr[iWord],  (short *)&testArr[iWord+1]);
//						}
//						memcpy(&tmi.LocalList[2][1], &testArr[2], 30 * sizeof(short));
//						return 1;
//					}
//
//            }
//			getline(finp, line, '\n');
		}
		return 0;
	}

	void ClearLOC(struct LOC &tmi) {
		tmi.timeBOKZ = 0;
		tmi.status1  = 0;
		tmi.status2  = 0;
		tmi.serialNumber = 0;
		tmi.timeExp   = 0;
		tmi.nLocalObj = 0;
		tmi.nFixedObj = 0;
		tmi.MeanC  = 0;
		tmi.SigmaC = 0;

		for (int i = 0; i < 32; i++) {
			for (int j = 0; j < 4; j++) {
				tmi.LocalList[i][j] = 0;
			}
		}
		tmi.Reserved = 0;
	}

	int TryReadLOC(ifstream &finp, struct LOC &tmi) {
		string line, word, inpstr, test_word, test_str;
		int indexObject = 0, indexParam = 0, intVal, flLow = 1;
		float fl1, fl2, fl3, sum;
		int Stat1, Stat2;

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if ((word == "ÊÑ1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == "ÊÑ2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "ÑÅÐ") || (word == "CEP") || (word == "ÑEP") || (word == "ÑEÐ")) {
				finp >> word;
				if ((word == "ÍÎÌ") || (word == "HOM")|| (word == "ÍÎM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "T") || (word == "Ò")) {
				finp >> word;
				if (word == "ÝÊÑÏ")
					finp >> tmi.timeExp;
				else {
					getline(finp, line, '\n');
					word = word + line;
					tmi.timeBOKZ = StringToDayTime(word);
				}
			}
			else if ((word == "HAM") || (word == "ÍÀÌ") || (word == "ÍÀM")) {
				finp >> word;
				if ((word == "ËÎÊ") || (word == "ËOÊ")) {
					finp >> tmi.nLocalObj;
				}
				else if ((word == "ÎÁÆ") || (word == "OÁÆ")) {
					finp >> tmi.nFixedObj;
				}
			}
			else if (word == "ÌÅÀÍ") {
				finp >> tmi.MeanC;
			}
			else if (word == "ÑÈÃÌÀ") {
				finp >> tmi.SigmaC;
			}
			else if (word == "Í ÏÈÊÑ") {
				finp >> tmi.Reserved;
			}
			else if (word == "ËÎÊ") {
				getline(finp, line, '\n');
				int nread = sscanf(line.c_str(), "%f%f%f", &fl1, &fl2, &fl3);
				if (nread == 3) {
					indexObject = (short)fl1;
					indexParam  = (short)fl2;
					tmi.LocalList[indexObject][indexParam] = fl3;
				}
				else if (nread == 2) {
					indexObject = (short)(fl1 / 10.);
					indexParam = (short)fl1 % 10;
					tmi.LocalList[indexObject][indexParam] = fl2;
				}
			}
//			else {
//				unsigned short testArr[32];
//				int cntWord = 0;
//				while ((cntWord < 32) && (word.find("-----") == string::npos)) {
//					int highByte = ReadBinaryString(word);
//					finp >> word;
//					int lowByte = ReadBinaryString(word);
//                    finp >> word;
//					testArr[cntWord] = (highByte << 8) | (lowByte);
//					cntWord++;
//				}
//				if ( (cntWord == 32) && (testArr[0] == 0x42C3) && (testArr[1] == 0x7AAA) ){
//					for (int iWord = 12; iWord <= 30; iWord = iWord + 2) {
//						SwapShort((short *)&testArr[iWord],  (short *)&testArr[iWord+1]);
//					}
//					memcpy(&tmi, &testArr[2], 30 * sizeof(short));
//				}
//
//				while ((line.find("ÄÒÌÈ")==string::npos) && (!finp.eof())) {
//					getline(finp, line, '\n' );
//				}
//				finp >> word;
//					if ( (cntWord == 32) && (testArr[0] == 0x42C3) && (testArr[1] == 0x8AAA) ){
//						for (int iWord = 2; iWord <= 30; iWord = iWord + 2) {
//							SwapShort((short *)&testArr[iWord],  (short *)&testArr[iWord+1]);
//						}
//						memcpy(&tmi.LocalList[2][1], &testArr[2], 30 * sizeof(short));
//						return 1;
//					}
//
//            }
//			getline(finp, line, '\n');
		}
		return 0;
	}

	void PrintMSHI(ofstream &file, struct MSHI tmi, TDateTime curDate) {
		file << OutputDateTime(curDate).c_str() << "\n";
		file << "____________________________________" << "\n";
		file << "Ìàññèâ ÌØÈÎÐ" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << "ÊÑ1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "ÊÑ2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec << setfill(' ');
		for (int i = 0; i < 3; i++) {
			file << "OZ[" << i << "]:\t" << tmi.OZ[i] << "\n";
		}
		for (int i = 0; i < 4; i++) {
			file << "Qornt[" << i << "]:\t" << tmi.Qornt[i] << "\n";
		}
		for (int i = 0; i < 3; i++) {
			file << "W[" << i << "], ''/c:\t" << tmi.W[i] * RTS << "\n";
		}
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLogMSHI(ofstream &file, struct MSHI tmi, TDateTime curDate, bool &create) {
		if (!create) {
			file << "Date&Time\t";
			file << "Day/Time\t";
			file << "KC1\t" << "KC2\t";
			file << "OZ[0]\t" << "OZ[1]\t" << "OZ[2]\t";
			file << "Q[0]\t" << "Q[1]\t" << "Q[2]\t" << "Q[3]\t";
			file << "Wx,''/c\t" << "Wy,''/c\t" << "Wz,''/c\t";
			file << "Al, deg\t" << "Dl, deg\t" << "Az, deg\t";
			file << "\n";
            create = true;
		}

		file << OutputDateTime(curDate).c_str() << "\t";
		file << DayTimeToString(tmi.timeBOKZ).c_str() << "\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << dec << setfill(' ');
		for (int i = 0; i < 3; i++) {
			file << tmi.OZ[i] << "\t";
		}
		for (int i = 0; i < 4; i++) {
			file << tmi.Qornt[i] << "\t";
		}
		for (int i = 0; i < 3; i++) {
			file << tmi.W[i] * RTS << "\t";
		}

		double Qdbl[4], Mornt[3][3], ang[3] = {0, 0, 0};
		for (int i = 0; i < 4; i++) {
			 Qdbl[i] = (double)tmi.Qornt[i];
		}
		if ( !CheckQuatNorm(Qdbl, 0.001) ) {
			QuatToMatrix(Qdbl, Mornt);
			MatrixToEkvAngles(Mornt, ang);
		}
		for (int i = 0; i < 3; i++) {
			file << ang[i] * RTD << "\t";
		}
		file << "\n";
	}

	void PrintSHTMI1(ofstream &file, struct SHTMI1 tmi) {
		file << "____________________________________" << "\n";
		file << "Ìàññèâ ØÒÌÈ1" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << "ÊÑ1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "ÊÑ2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "Çàâ. ¹\t" << tmi.serialNumber << "\n";
		file << "Texp, ìñ:\t" << tmi.timeExp << "\n";
		file << "Foc, ìì:\t" << tmi.Foc << "\n";
		file << "X0, ìì:\t" << tmi.Xg << "\n";
		file << "Y0, ìì:\t" << tmi.Yg << "\n";
		file << "Ñðåäíåå, å.ì.ð.:\t" << tmi.Mean << "\n";
		file << "ÑÊÎ, å.ì.ð.:\t" << tmi.Sigma << "\n";
		file << "×èñëî äåôåêòîâ:\t" << tmi.countDefect << "\n";
		file << "Èäåíòèôèêàòîð:\t" << tmi.CRC << "\n";
		file << "Äàòà:\t" << tmi.Date << "\n";
		file << "Âåðñèÿ:\t" << tmi.Version << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLogSHTMI1(ofstream &file, struct SHTMI1 tmi, bool &create) {
		if (!create) {
			file << "Day/Time\t";
			file << "KC1\t" << "KC2\t" << "POST\t" << "¹\t" << "Texp\t";
			file << "Foc\t" << "Xg\t" << "Yg\t";
			file << "Mean\t" << "Sigma\t";
			file << "Ndef\t" << "CRC\t";
			file << "Date\t" << "Version\t";
			file << "\n";
			create = true;
		}
		file << DayTimeToString(tmi.timeBOKZ).c_str() << "\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";
		file << dec << setfill(' ');
		file << tmi.serialNumber << "\t";
		file << tmi.timeExp << "\t";
		file << tmi.Foc << "\t";
		file << tmi.Xg << "\t";
		file << tmi.Yg << "\t";
		file << tmi.Mean << "\t";
		file << tmi.Sigma << "\t";
		file << tmi.countDefect << "\t";
		file << tmi.CRC << "\t";
		file << tmi.Date << "\t";
		file << tmi.Version << "\t";
		file << "\n";
		file << flush;
	}

	void PrintSHTMI2(ofstream &file, struct SHTMI2 tmi, TDateTime curDate) {
		file << OutputDateTime(curDate).c_str() << "\n";
		file << "____________________________________" << "\n";
		file << "Ìàññèâ ØÒÌÈ2" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << "ÊÑ1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "ÊÑ2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "Çàâ. ¹\t" << tmi.serialNumber << "\n";
		file << "Texp, ìñ:\t" << tmi.timeExp << "\n";
		file << "×èñëî ÓÑÄ:\t" << tmi.cntCommandWord << "\n";
		file << "×èñëî ÍÎ:\t" << tmi.cntCallNO << "\n";
		file << "ÍÎ->ÑËÅÆ:\t" << tmi.cntNOtoSLEZH << "\n";
		file << "×èñëî TÎ:\t" << tmi.cntCallTO << "\n";
		file << "TÎ->ÑËÅÆ:\t" << tmi.cntTOtoSLEZH << "\n";
		file << "×èñëî ÑËÅÆ:\t" << tmi.cntSLEZH << "\n";

		for (int i = 0; i < MAX_STAT; i++) {
			file << "Ñ÷åò÷èê ¹ " << (i + 1) << ":\t" << tmi.cntStatOrient[i]
				<< "\n";
		}
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLogSHTMI2(ofstream &file, struct SHTMI2 tmi, TDateTime curDate, bool &create) {
		if (!create) {
			file << "Date&Time\t";
			file << "Day/Time\t";
			file << "KC1\t" << "KC2\t" << "POST\t" << "¹\t" << "Texp\t";
			file << "ÓÑÄ\t" << "ÍÎ\t" << "ÍÎÑË\t";
			file << "TÎ\t" << "TÎÑË\t" << "ÑËÅÆ\t";
			for (int i = 0; i < MAX_STAT; i++) {
				file << "EC" << (i+1) << "\t";
			}
			file << "\n";
			create = true;
		}
		file << OutputDateTime(curDate).c_str() << "\t";
		file << DayTimeToString(tmi.timeBOKZ).c_str()<<"\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";
		file << dec << setfill(' ');
		file << setw(6) << tmi.serialNumber << "\t";
		file << setw(6) << tmi.timeExp << "\t";
		file << setw(6) << tmi.cntCommandWord << "\t";
		file << setw(6) << tmi.cntCallNO << "\t";
		file << setw(6) << tmi.cntNOtoSLEZH << "\t";
		file << setw(6) << tmi.cntCallTO << "\t";
		file << setw(6) << tmi.cntTOtoSLEZH << "\t";
		file << setw(8) << tmi.cntSLEZH << "\t";
		for (int i = 0; i < MAX_STAT; i++) {
			file << tmi.cntStatOrient[i] << "\t";
		}
		file << "\n";
        file << flush;
	}

	int CheckNewDtmiM60(unsigned short serial)
	{
		if ( ((serial>=10) && (serial<=13)) || (serial >=49) ) {
			return 1;
		}
        return 0;
	}

	void PrintDTMI(ofstream &file, TDateTime curDate, struct DTMI tmi, int isM1000) {
		file << OutputDateTime(curDate).c_str() << "\n";
		file << "____________________________________" << "\n";
		file << "Ìàññèâ ÄÒÌÈ" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << "ÊÑ1\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "ÊÑ2\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec << setfill(' ');
		file << "Çàâ. ¹\t" << tmi.serialNumber << "\n";
		file << "Texp, ìñ:\t" << tmi.timeExp << "\n";
		file << "NumLoc: \t" << tmi.nLocalObj << "\n";
		file << "NumDet: \t" << tmi.nDeterObj << "\n";
		file << "NumFrag:\t" << tmi.nWindows << "\n";
		file << "EPSILON:\t" << tmi.epsillon << "\n";
		file << "DeltaT:\t" << tmi.dTimeBOKZ << "\n";
		file << setw(6) << "¹" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
		for (int i = 0; i < MAX_OBJ_DTMI; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.LocalList[i][0] << "\t" << tmi.LocalList[i][1] << "\t";
			file << tmi.LocalList[i][2] << "\t";
			if (CheckNewDtmiM60(tmi.serialNumber) && !isM1000) {
				short *level, *size;
				size = (short *)(&tmi.LocalList[i][3]);
				level = (short *)(size + 1);
				file << *size << "\t" << *level << "\n";
			}
			else file << tmi.LocalList[i][3] << "\n";
		}
		file << setw(6) << "¹" << " X, pix" << " Y, pix" << "\n";
		for (int i = 0; i < MAX_WINDOW; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.centrWindow[i][0] << "\t" << tmi.centrWindow[i][1]
				<< "\t";
			file << tmi.levelWindow[i] << "\t" << tmi.nObjectWindow[i] << "\n";
		}

		for (int i = 0; i < 4; i++) {
			file << "Qapr[" << i << "]:\t" << tmi.quatBoard[i] << "\n";
		}

		for (int i = 0; i < 3; i++) {
			file << "Wop[" << i << "]:\t" << tmi.omega[i] << "\n";
		}

		file << "Tlst:\t" << DayTimeToString(tmi.timeQuatLast).c_str() << "\n";
		for (int i = 0; i < 4; i++) {
			file << "Qlst[" << i << "]:\t" << tmi.quatLast[i] << "\n";
		}
		file << "NumLoc[0]: \t" << tmi.nLocal[0] << "\n";
		file << "NumLoc[1]: \t" << tmi.nLocal[1] << "\n";
		file << "Ýïîõà: \t" << tmi.Epoch << "\n";
		file << "MaxHist: \t" << tmi.maxHist << "\n";
		file << "MaxHistX: \t" << (short)tmi.maxHistX << "\n";
		file << "MaxHistY: \t" << (short)tmi.maxHistY << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLOC(ofstream &file, TDateTime curDate, struct LOC tmi, int isM1000) {
		file << OutputDateTime(curDate).c_str() << "\n";
		file << "____________________________________" << "\n";
		file << "Ìàññèâ ÌËÎÊ" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << "ÊÑ1\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "ÊÑ2\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec << setfill(' ');
		file << "Çàâ. ¹\t" << tmi.serialNumber << "\n";
		file << "Texp, ìñ:\t" << tmi.timeExp << "\n";
		file << "NumLoc: \t" << tmi.nLocalObj << "\n";
		file << "NumFix: \t" << tmi.nFixedObj << "\n";
		file << "MeanC:\t" << tmi.MeanC << "\n";
		file << "SigmaC:\t" << tmi.SigmaC << "\n";
		file << setw(6) << "¹" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
		for (int i = 0; i < 32; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.LocalList[i][0] << "\t" << tmi.LocalList[i][1] << "\t";
			file << tmi.LocalList[i][2] << "\t";
			if (CheckNewDtmiM60(tmi.serialNumber) && !isM1000) {
				short *level, *size;
				size = (short *)(&tmi.LocalList[i][3]);
				level = (short *)(size + 1);
				file << *size << "\t" << *level << "\n";
			}
			else  file << tmi.LocalList[i][3] << "\n";
		}

		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLocalDTMI(AnsiString fileDir, TDateTime curDate, struct DTMI tmi, int isM1000) {

		AnsiString stringSerial, localDir, localName;
		TFormatSettings curFormat;

		stringSerial.sprintf("Çàâ. ¹ %02d", tmi.serialNumber);
		localDir = fileDir + "\\" + stringSerial;
		CreateDir(localDir);
//		localDir = localDir + "\\" + AnsiString(DateToStr(curTime)).c_str();
//		CreateDir(localDir);

		curFormat.ShortDateFormat = "yyyy_mm_dd";
		curFormat.LongTimeFormat  = "hh_nn_ss_zzz";
		localName.sprintf("%s_%s_DTMI_LOC.txt",
				AnsiString(DateToStr(curDate, curFormat)).c_str(),
				AnsiString(TimeToStr(curDate, curFormat)).c_str());
		CheckFileName(localName);
		localName = localDir + "\\" + localName;

		ofstream file(localName.c_str());
		file << setw(6) << "¹" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";

		int cntLocal;
		if (tmi.nLocalObj < MAX_OBJ_DTMI)
			cntLocal = tmi.nLocalObj;
		else
			cntLocal = MAX_OBJ_DTMI;
		for (int i = 0; i < cntLocal; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.LocalList[i][0] << "\t" << tmi.LocalList[i][1] << "\t";
			file << tmi.LocalList[i][2] << "\t";
			if (CheckNewDtmiM60(tmi.serialNumber) && !isM1000) {
				short *level, *size;
				size = (short *)(&tmi.LocalList[i][3]);
				level = (short *)(size + 1);
				file << abs(*size) << "\t" << *level << "\n";
			}
			else file << fabs(tmi.LocalList[i][3]) << "\n";
		}
		file.close();
	}

	void PrintLocalMLOC(AnsiString fileDir, TDateTime curDate, struct LOC tmi, int isM1000) {

		AnsiString stringSerial, localDir, localName;
		TFormatSettings curFormat;

		stringSerial.sprintf("Çàâ. ¹ %02d", tmi.serialNumber);
		localDir = fileDir + "\\" + stringSerial;
		CreateDir(localDir);
//		localDir = localDir + "\\" + AnsiString(DateToStr(curTime)).c_str();
//		CreateDir(localDir);

		curFormat.ShortDateFormat = "yyyy_mm_dd";
		curFormat.LongTimeFormat  = "hh_nn_ss_zzz";
		localName.sprintf("%s_%s_MLOC_LOC.txt",
				AnsiString(DateToStr(curDate, curFormat)).c_str(),
				AnsiString(TimeToStr(curDate, curFormat)).c_str());
		CheckFileName(localName);
		localName = localDir + "\\" + localName;

		ofstream file(localName.c_str());

		file << setw(6) << "¹" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
		int cntLocal;
		if (tmi.nFixedObj < MAX_OBJ_MLOC)
			cntLocal = tmi.nFixedObj;
		else
			cntLocal = MAX_OBJ_MLOC;
		for (int i = 0; i < cntLocal; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.LocalList[i][0] << "\t" << tmi.LocalList[i][1] << "\t";
			file << tmi.LocalList[i][2] << "\t";
			if (CheckNewDtmiM60(tmi.serialNumber) && !isM1000) {
				short *level, *size;
				size = (short *)(&tmi.LocalList[i][3]);
				level = (short *)(size + 1);
				file << abs(*size) << "\t" << *level << "\n";
			}
			else file << tmi.LocalList[i][3] << "\n";
		}
		file.close();
	}

	void ConvertDataDTMI(struct DTMI tmi, struct CadrInfo &mCadr, int isM1000) {

		// mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;
        int WinWidth, WinHeight;
		if (isM1000) {
			mCadr.ImageHeight = mCadr.ImageWidth = 1024;
			WinWidth = WinHeight = 13;
		}
		else {
			mCadr.ImageHeight = mCadr.ImageWidth = 512;
			WinWidth = WinHeight = 17;
		}

		mCadr.CountDeterObj = tmi.nDeterObj;
		mCadr.CountWindows  = tmi.nWindows;
		mCadr.CountLocalObj = tmi.nLocalObj;

		if (tmi.nLocalObj < MAX_OBJ_DTMI)
			mCadr.SizeObjectsList = tmi.nLocalObj;
		else
			mCadr.SizeObjectsList = MAX_OBJ_DTMI;

		ObjectsInfo objInfo;
		mCadr.IsBinary = true;
		for (int i = 0; i < mCadr.SizeObjectsList; i++) {
			if ( (objInfo.X > (mCadr.ImageWidth >> 1))
				|| (objInfo.Y > (mCadr.ImageHeight >> 1)) ) {
				mCadr.IsBinary = false;
			}
			objInfo.X = tmi.LocalList[i][0];
			objInfo.Y = tmi.LocalList[i][1];
			objInfo.Bright = tmi.LocalList[i][2];
			if (CheckNewDtmiM60(tmi.serialNumber) && !isM1000) {
				objInfo.Square = *(short *)(&tmi.LocalList[i][3]);
			}
			else objInfo.Square = tmi.LocalList[i][3];
			objInfo.Dx = 0;
			objInfo.Dy = 0;
			objInfo.StarID = 0;
			objInfo.Mv = 0;
			objInfo.Sp[0] = '_';
			objInfo.Sp[1] = '_';
			mCadr.ObjectsList.push_back(objInfo);
		}

		if (tmi.nWindows < MAX_WINDOW) {
			mCadr.SizeWindowsList = tmi.nWindows;
			mCadr.SizeStarsList = tmi.nWindows;
		}
		else {
			mCadr.SizeWindowsList = MAX_WINDOW;
			mCadr.SizeStarsList = MAX_WINDOW;
		}

		WindowsInfo winInfo;
		StarsInfo starList;
		mCadr.Level = 0;
		for (int i = 0; i < mCadr.SizeWindowsList; i++) {
			winInfo.Level = tmi.levelWindow[i];
			winInfo.CountObj = tmi.nObjectWindow[i];
			winInfo.Width  = WinWidth;
			winInfo.Height = WinHeight;
			winInfo.Xstart = tmi.centrWindow[i][0] - (winInfo.Width >> 1);
			winInfo.Ystart = tmi.centrWindow[i][1] - (winInfo.Height >> 1);
			mCadr.WindowsList.push_back(winInfo);

			starList.X = tmi.centrWindow[i][0];
			starList.Y = tmi.centrWindow[i][1];

			if ( (starList.X > (mCadr.ImageWidth >> 1))
				|| (starList.Y > (mCadr.ImageHeight >> 1)) ) {
				mCadr.IsBinary = false;
			}
			mCadr.Level += mCadr.WindowsList[i].Level;
		}
		if (mCadr.SizeWindowsList) {
			mCadr.Level /= mCadr.SizeWindowsList;
		}

		if (mCadr.IsBinary) {
			mCadr.ImageHeight >>= 1;
			mCadr.ImageWidth  >>= 1;
		}

		mCadr.CountBlock = 0;
		mCadr.CountStars = 0;
		mCadr.SizeStarsList = 0;
	}

	void ConvertDataLOC(struct LOC tmi, struct CadrInfo &mCadr, int isM1000) {

		// mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;
		if (isM1000) {
			mCadr.ImageHeight = mCadr.ImageWidth = 1024;
		}
		else {
			mCadr.ImageHeight = mCadr.ImageWidth = 512;
		}

		mCadr.MeanBright = tmi.MeanC;
        mCadr.SigmaBright = tmi.SigmaC;
		mCadr.CountLocalObj = tmi.nLocalObj;
		mCadr.SizeObjectsList = tmi.nFixedObj;

		if (tmi.nLocalObj < MAX_OBJ_MLOC)
			mCadr.SizeObjectsList = tmi.nLocalObj;
		else
			mCadr.SizeObjectsList = MAX_OBJ_MLOC;

		ObjectsInfo objInfo;
		mCadr.IsBinary = true;
		for (int i = 0; i < mCadr.SizeObjectsList; i++) {
			if ( (objInfo.X > (mCadr.ImageWidth >> 1))
				|| (objInfo.Y > (mCadr.ImageHeight >> 1)) ) {
				mCadr.IsBinary = false;
			}
			objInfo.X = tmi.LocalList[i][0];
			objInfo.Y = tmi.LocalList[i][1];
			objInfo.Bright = tmi.LocalList[i][2];
			if (CheckNewDtmiM60(tmi.serialNumber) && !isM1000) {
				objInfo.Square = *(short *)(&tmi.LocalList[i][3]);
			}
			else objInfo.Square = tmi.LocalList[i][3];
			objInfo.Dx = 0;
			objInfo.Dy = 0;
			objInfo.StarID = 0;
			objInfo.Mv = 0;
			objInfo.Sp[0] = '_';
			objInfo.Sp[1] = '_';
			mCadr.ObjectsList.push_back(objInfo);
		}

		if (mCadr.IsBinary) {
			mCadr.ImageHeight >>= 1;
			mCadr.ImageWidth  >>= 1;
		}

		mCadr.CountBlock = 0;
		mCadr.CountWindows = 0;
		mCadr.SizeWindowsList = 0;
		mCadr.CountDeterObj = 0;
		mCadr.CountStars = 0;
		mCadr.SizeStarsList = 0;
	}

//-------------ôóíêöèè äëÿ ÷òåíèÿ ïðîòîêîëîâ ÝÌÊÈ-----------------------------//
	void PrintMshi_2V(ofstream &file, struct TBoardArray *pack, TDateTime curDate, bool &create)
	{
		struct TMshi_2V *tmi;
		if (!create) {
			file << "Date&Time\t";
			file << "Tbshv\t";
			file << "Q[0]\t" << "Q[1]\t" << "Q[2]\t" << "Q[3]\t";
			file << "Serial\t" << "KC1\t" << "KC2\t";
			file << "NumStar\t" << "NumFrag\t";
			file << "NumLoc\t" << "NumDet\t";
			file << "ThMax\t" << "Mxy,mkm\t" << "Tcmv\t";
			file << "OZ[0]\t" << "OZ[1]\t" << "OZ[2]\t";
			file << "Wx,''/c\t" << "Wy,''/c\t" << "Wz,''/c\t";
			file << "Al, deg\t" << "Dl, deg\t" << "Az, deg\t";
			file << "StatTmi\t" << "BoardCRC\t" << "CalcCRC\n";
			create = true;
		}

		tmi = (TMshi_2V *)&pack->bufMIL;
		file << dec << setfill(' ');
//        file << OutputDateTime(curDate).c_str() << "\t";
		file << AnsiString(DateTimeToStr(curDate)).c_str() << "\t";
		file << tmi->time_sec << "." << setfill('0');
		file << setw(3) << tmi->time_ms << "\t";
		for (int i = 0; i < 4; i++) {
			file << tmi->Qornt[i] << "\t";
		}
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi->serial << "\t";
		file << "0x" << setw(4) << tmi->status1 << "\t";
		file << "0x" << setw(4) << tmi->status2 << "\t";
		file << dec << setfill(' ');

		file << (unsigned short)tmi->NumStar << "\t";
		file << (unsigned short)tmi->NumFrag << "\t";
		file << tmi->NumLoc  << "\t" << tmi->NumDet << "\t";
		file << tmi->ThMax  << "\t" << tmi->mxy/(1e+4) * 1000. << "\t";
		file << tmi->Tcmv/10. << "\t";

		for (int i = 0; i < 3; i++) {
			file << tmi->OZ[i] << "\t";
		}
		for (int i = 0; i < 3; i++) {
			file << tmi->W[i] * RTS << "\t";
		}
		double Qdbl[4], Mornt[3][3], ang[3] = {0, 0, 0};
		for (int i = 0; i < 4; i++) {
			 Qdbl[i] = (double)tmi->Qornt[i];
		}
		if ( !CheckQuatNorm(Qdbl, 0.001) ) {
			QuatToMatrix(Qdbl, Mornt);
			MatrixToEkvAngles(Mornt, ang);
		}
		for (int i = 0; i < 3; i++) {
			file << ang[i] * RTD << "\t";
		}
		file << (unsigned short) pack->statTmi << "\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << pack->crcBoard << "\t";
		file << "0x" << setw(4) << pack->crcCalc  << "\n";
	}

	void PrintShtmi1_2V(ofstream &file, struct TBoardArray *pack, TDateTime curDate, bool &create)
	{
		struct TShtmi1_2V *tmi;
		if (!create) {
			file << "Date&Time\t";
			file << "Tbshv\t";
			file << "Serial\t" << "KC1\t" << "KC2\t" << "POST\t";
			file << "Foc\t" << "Xg\t" << "Yg\t" ;
			file << "Texp\t" << "Mean\t" << "Sigma\t";
			file << "Tcmv\t" << "StatKP\t" << "CntKP\t" ;
			file << "CatCS0\t" << "ConstCS0\t" << "ProgCS0\t";
			file << "CatCS1\t" << "ConstCS1\t" << "ProgCS1\t";
			file << "Version\t" << "StatTmi\t" << "BoardCRC\t" << "CalcCRC\n";
			create = true;
		}

		tmi = (TShtmi1_2V *)&pack->bufMIL;
		file << dec << setfill(' ');
//        file << OutputDateTime(curDate).c_str() << "\t";
		file << AnsiString(DateTimeToStr(curDate)).c_str() << "\t";
		file << tmi->time_sec << "." << setfill('0');
		file << setw(3) << tmi->time_ms << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi->serial << "\t";
		file << "0x" << setw(4) << tmi->status1 << "\t";
		file << "0x" << setw(4) << tmi->status2 << "\t";
		file << "0x" << setw(4) << tmi->post << "\t";

		file << dec << setfill(' ');
		file << tmi->Foc << "\t" << tmi->Xg << "\t" << tmi->Yg << "\t";
		file << tmi->timeExp  << "\t" << tmi->Mean   << "\t" << tmi->Sigma << "\t";
		file << tmi->Tcmv/10. << "\t" << tmi->statKP << "\t" << tmi->cntKP << "\t";
		file << tmi->CatCS[0] << "\t" << tmi->ConstCS[0] << "\t" << tmi->ProgCS[0] << "\t";
		file << tmi->CatCS[1] << "\t" << tmi->ConstCS[1] << "\t" << tmi->ProgCS[1] << "\t";
		file << tmi->Version << "\t";
		file << (unsigned short) pack->statTmi << "\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << pack->crcBoard << "\t";
		file << "0x" << setw(4) << pack->crcCalc  << "\n";
	}

	void PrintShtmi2_2V(ofstream &file,  struct TBoardArray *pack, TDateTime curDate, bool &create)
	{
		struct TShtmi2_2V *tmi;
		if (!create) {
			file << "Date&Time\t";
			file << "Tbshv\t";
			file << "Serial\t" << "KC1\t" << "KC2\t" << "POST\t";
			file << "ÓÑÄ\t" << "ÍÎ\t" << "ÍÎÑË\t";
			file << "Texp\t" << "TÎ\t" << "TÎÑË\t" << "ÑËÅÆ\t";
			for (int i = 0; i < MAX_STAT; i++) {
				file << "EC" << (i+1) << "\t";
			}
			file << "StatTmi\t" << "BoardCRC\t" << "CalcCRC\n";
			create = true;
		}

		tmi = (TShtmi2_2V *)&pack->bufMIL;
		file << dec << setfill(' ');
//        file << OutputDateTime(curDate).c_str() << "\t";
		file << AnsiString(DateTimeToStr(curDate)).c_str() << "\t";
		file << tmi->time_sec << "." << setfill('0');
		file << setw(3) << tmi->time_ms << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi->serial << "\t";
		file << "0x" << setw(4) << tmi->status1 << "\t";
		file << "0x" << setw(4) << tmi->status2 << "\t";
		file << "0x" << setw(4) << tmi->post << "\t";

		file << dec << setfill(' ');
		file << setw(6) << tmi->timeExp << "\t";
		file << setw(6) << tmi->cntCommandWord << "\t";
		file << setw(6) << tmi->cntCallNO << "\t";
		file << setw(6) << tmi->cntNOtoSLEZH << "\t";
		file << setw(6) << tmi->cntCallTO << "\t";
		file << setw(6) << tmi->cntTOtoSLEZH << "\t";
		file << setw(8) << tmi->cntSLEZH << "\t";
		for (int i = 0; i < MAX_STAT; i++) {
			file << tmi->cntStatOrient[i] << "\t";
		}
		file << (unsigned short) pack->statTmi << "\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << pack->crcBoard << "\t";
		file << "0x" << setw(4) << pack->crcCalc  << "\n";
	}

	void PrintDtmi_2V(ofstream &file, struct TDtmi_2V tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Date&Time\t" << "Tbshv\t";
			file << "Serial\t" << "KC1\t" << "KC2\t" << "POST\t";
			file << "Tcmv\t" << "Texp\t";
			file << "NumAll[0]\t" << "NumAll[1]\t";
			file << "NumL[0]\t" << "NumL[1]\t";
			file << "NumStore\t" << "NumDet\t";
			file << "NumStar\t" << "NumFrag\t";
			file << "MaxH" << "dxMaxH\t" << "dyMaxH\t";
			file << "\n";
            create = true;
		}

//        file << OutputDateTime(curDate).c_str() << "\t";
		file << AnsiString(DateTimeToStr(curDate)).c_str() << "\t";
		file << tmi.time_sec << "." << setfill('0');
		file << setw(3) << tmi.time_ms << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.serial << "\t";
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";

		file << dec << setfill(' ');
		file << tmi.Tcmv/10. << "\t" << tmi.timeExp << "\t";
		file << tmi.NumAll[0] << "\t" <<tmi.NumAll[1] << "\t";
		file << (unsigned short)tmi.NumL[0] << "\t";
		file << (unsigned short)tmi.NumL[1] << "\t";
		file << tmi.NumLoc << "\t" ;
		file << (unsigned short)tmi.NumStore << "\t";
		file << (unsigned short)tmi.NumDet << "\t";
		file << (unsigned short)tmi.NumStar << "\t";
		file << (unsigned short)tmi.NumFrag << "\t";
		file << tmi.MaxH << "\t" ;
		file << (short)tmi.dxMaxH << "\t";
		file << (short)tmi.dyMaxH << "\n";
	}


//-------------Ôóíêöèè äëÿ ÷òåíèÿ ïðîòîêîëîâ ÐÊÊ "Ýíåðãèÿ"--------------------//
	int ReadTMIArray(ifstream &_inp, string _keyWord, unsigned short *arrayTMI, const int _sizeArray)
	{
		string line, readWord;
		string format = _keyWord + "[%ld].W[%d] %d";
		unsigned short nWord = 0;
		do
		{
			getline(_inp, line, '\n' );
			if (line.find(_keyWord) != std::string::npos) {
				int nBOKZ = -1;
				unsigned short word, buf;
				if (sscanf(line.c_str(), format.c_str(), &nBOKZ, &buf, &word) == 3) {
					if ((nWord >= 0) && (nWord < _sizeArray)) {
						arrayTMI[nWord] =  word;
						nWord++;
					}
				}
			}
		} while (line.find("CIS1_DSP.TMOS") == std::string::npos);
		if (nWord == _sizeArray) return 0;
		else return -1;
	}

	void ConvertDataDTMI_BOKZM(struct DTMI_BOKZM tmi, struct CadrInfo &mCadr)
	{
		mCadr.IsBinary = true;
		mCadr.ImageHeight = 512;
		mCadr.ImageWidth = 512;
	//	mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;
//    	mCadr.Time = tmi.timeBOKZ;
		mCadr.CountLocalObj = tmi.nLocalObj;

		if (tmi.nLocalObj < MAX_OBJ_BOKZM) mCadr.SizeObjectsList = tmi.nLocalObj;
		else mCadr.SizeObjectsList = MAX_OBJ_BOKZM;

		ObjectsInfo objInfo;
		for (int i = 0; i < mCadr.SizeObjectsList; i++) {
			objInfo.X = tmi.LocalList[i][0];
			objInfo.Y = tmi.LocalList[i][1];
			objInfo.Bright = tmi.LocalList[i][2];
			objInfo.Square = tmi.LocalList[i][3];
			objInfo.Dx = 0;
			objInfo.Dy = 0;
			objInfo.StarID = 0;
			objInfo.Mv = 0;
			objInfo.Sp[0] = '_';
			objInfo.Sp[1] = '_';
			mCadr.ObjectsList.push_back(objInfo);
		}

		mCadr.CountBlock = 0;
		mCadr.CountWindows = 0;
		mCadr.SizeWindowsList = 0;
		mCadr.CountDeterObj = 0;
		mCadr.CountStars = 0;
		mCadr.SizeStarsList = 0;
	}

	void PrintMSHI_BOKZM(ofstream &file, struct MSHI_BOKZM tmi)
	{
		file << "____________________________________" << "\n";
		file << "Ìàññèâ ÌØÈÎÐ" << "\n";
		file << "Tpr\t" << tmi.timeBOKZ << "\n";
		file << uppercase << hex << setfill('0');
		file << "ÊÑ1\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "ÊÑ2\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec << setfill(' ');
		file << "Ìàòðèöà îðèåíòàöèè:\n";
		file << std::setprecision(8);
		for (int i = 0; i < 3; i++) {
			file << std::setw(12) << tmi.Mornt[i][0];
			file << std::setw(12) << tmi.Mornt[i][1];
			file << std::setw(12) << tmi.Mornt[i][2] << "\n";
		}

	//	double ang[3], MorntT[3][3];
	//
	//	for (int i = 0; i < 3; i++) {
	//		for (int j = 0; j < 3; j++) {
	//            MorntT[i][j] =  tmi.Mornt[j][i];
	//		}
	//	}
	//	MatrixToEkvAngles(MorntT, ang);
	//	file<<"Óãëû îðèåíòàöèè:\n";
	//	file<<"Al = "<<ang[0]*RTD<<"\t";
	//	file<<"Dl = "<<ang[1]*RTD<<"\t";
	//	file<<"Az = "<<ang[2]*RTD<<"\n";
		file<<"____________________________________"<<"\n";
		file<<flush;
	}

	void PrintSHTMI1_BOKZM(ofstream &file, struct SHTMI1_BOKZM tmi) {
		file << "____________________________________" << "\n";
		file << "Ìàññèâ ØÒÌÈ1" << "\n";
		file << "Tpr\t" << tmi.timeBOKZ << "\n";
		file << uppercase << hex << setfill('0');
		file << "ÊÑ1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "ÊÑ2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "Çàâ. ¹\t" << tmi.serialNumber << "\n";
		file << "Texp, ìñ:\t" << tmi.timeExp << "\n";
		file << "Foc, ìì:\t" << tmi.Foc << "\n";
		file << "X0, ìì:\t" << tmi.Xg << "\n";
		file << "Y0, ìì:\t" << tmi.Yg << "\n";
		file << "Ñðåäíåå, å.ì.ð.:\t" << tmi.Mean << "\n";
		file << "ÑÊÎ, å.ì.ð.:\t" << tmi.Sigma << "\n";
		file << "×èñëî äåôåêòîâ:\t" << tmi.countDefect << "\n";
		file << "Äàòà:\t" << tmi.Date << "\n";
		file << "Âåðñèÿ XCF:\t" << tmi.verXCF << "\n";
		file << "Âåðñèÿ ÏÎ:\t" << tmi.verProg << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintSHTMI2_BOKZM(ofstream &file, struct SHTMI2_BOKZM tmi) {
		file << "____________________________________" << "\n";
		file << "Ìàññèâ ØÒÌÈ2" << "\n";
		file << "Tpr\t" << tmi.timeBOKZ << "\n";
		file << uppercase << hex << setfill('0');
		file << "ÊÑ1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "ÊÑ2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "Çàâ. ¹\t" << tmi.serialNumber << "\n";
		file << "Texp, ìñ:\t" << tmi.timeExp << "\n";
		file << "Foc, ìì:\t" << tmi.Foc << "\n";
		file << "X0, ìì:\t" << tmi.Xg << "\n";
		file << "Y0, ìì:\t" << tmi.Yg << "\n";
		file << "Ñðåäíåå, å.ì.ð.:\t" << tmi.Mean << "\n";
		file << "ÑÊÎ, å.ì.ð.:\t" << tmi.Sigma << "\n";
		file << "×èñëî äåôåêòîâ:\t" << tmi.countDefect << "\n";
		file << "×èñëî ÍÎ:\t" << tmi.cntCallNO << "\n";
		file << "×èñëî TÎ:\t" << tmi.cntCallTO << "\n";

		for (int i = 0; i < 12; i++) {
			file << "Ñ÷åò÷èê ¹ " << (i + 1) << ":\t" << tmi.cntStatOrient[i]
				<< "\n";
		}
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintDTMI_BOKZM(ofstream &file, struct DTMI_BOKZM tmi)
	{
		file << "____________________________________" << "\n";
		file << "Ìàññèâ ÄÒÌÈ"<<"\n";
		file << "Tpr\t"<<tmi.timeBOKZ<<"\n";
		file << uppercase<<hex<<setfill('0');
		file << "ÊÑ1\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "ÊÑ2\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec<<setfill(' ');
		file << "Çàâ. ¹\t" << tmi.serialNumber << "\n";
		file << "Foc, ìì:\t" << tmi.Foc << "\n";
		file << "X0, ìì:\t" << tmi.Xg << "\n";
		file << "Y0, ìì:\t" << tmi.Yg << "\n";
		file << "Texp, ìñ:\t" << tmi.timeExp << "\n";
		file << "NumLoc: \t" << tmi.nLocalObj << "\n";
		file << "NumDet: \t" << tmi.nDeterObj << "\n";
		file << setw(6) << "¹" << " X, pix" << " Y, pix" << " Bright" << " Nel" << "\n";
		for (int i = 0; i < MAX_OBJ_BOKZM; i++) {
			file << setw(6) << (i+1) << "\t";
			file << tmi.LocalList[i][0] << "\t" << tmi.LocalList[i][1] << "\t";
			file << tmi.LocalList[i][2] << "\t" << tmi.LocalList[i][3] << "\n";
		}
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLocalDTMI_BOKZM(AnsiString fileName, struct DTMI_BOKZM tmi)
	{
		ofstream file(fileName.c_str());

		file << setw(6) << "¹" << " X, pix" << " Y, pix" << " Bright" <<
				" Nel" << "\n";
		int cntLocal;
		if (tmi.nLocalObj < MAX_OBJ_BOKZM)
			cntLocal = tmi.nLocalObj;
		else
			cntLocal = MAX_OBJ_BOKZM;
		for (int i = 0; i < cntLocal; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.LocalList[i][0] << "\t" << tmi.LocalList[i][1] << "\t";
			file << tmi.LocalList[i][2] << "\t" << tmi.LocalList[i][3] << "\n";
		}
		file.close();
	}
//----------------------------------------------------------------------------//

void PrintDTMI_M2(ofstream &file, struct DTMI_M2 tmi) {
	file << "____________________________________" << "\n";
	file << "Ìàññèâ ÄÒÌÈ" << "\n";
	file << "Tpr\t" << tmi.timeBOKZ << "\n";
	file << uppercase << hex << setfill('0');
	file << "ÊÑ1\t" << "0x" << setw(4) << tmi.status1 << "\n";
	file << "ÊÑ2\t" << "0x" << setw(4) << tmi.status2 << "\n";
	file << "POST\t" << "0x" << setw(4) << tmi.POST << "\n";
	file << dec << setfill(' ');
	file << "Çàâ. ¹\t" << tmi.serialNumber << "\n";
	file << "Texp, ìñ:\t" << tmi.timeExp << "\n";
	file << "Mean: \t" << tmi.meanCadr[0] << "\n";
	file << "Sigma: \t" << tmi.meanCadr[1] << "\n";
	file << "NumLoc1: \t" << tmi.nLocalObj1 << "\n";
	file << "NumLoc2: \t" << tmi.nLocalObj2 << "\n";
	file << "NumAll1: \t" << tmi.nLocal[0] << "\n";
	file << "NumAll2: \t" << tmi.nLocal[1] << "\n";
	file << "NumStore: \t" << tmi.nStoreObj << "\n";
	file << "NumDet: \t" << tmi.nDeterObj << "\n";
	file << "NumFrag:\t" << tmi.nWindows << "\n";
	file << "Sector:\t" << tmi.nSec << "\n";

	file << setw(6) << "¹" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
	for (int i = 0; i < 15; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.LocalList1[i].x << "\t" << tmi.LocalList1[i].y << "\t";
		file << tmi.LocalList1[i].bright << "\t" << tmi.LocalList1[i].size << "\n";
	}

	file << setw(6) << "¹" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
	for (int i = 0; i < 15; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.LocalList2[i].x << "\t" << tmi.LocalList2[i].y << "\t";
		file << tmi.LocalList2[i].bright << "\t" << tmi.LocalList2[i].size << "\n";
	}

	file << setw(6) << "¹" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
	for (int i = 0; i < 12; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.ResultList[i].x << "\t" << tmi.ResultList[i].y << "\t";
		file << tmi.ResultList[i].bright << "\t" << tmi.ResultList[i].size << "\n";
	}
	file << setw(6) << "¹" << " X, pix" << " Y, pix" << "\n";
	for (int i = 0; i < 16; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.nObjectWindow[i] << "\n";
	}
	for (int i = 0; i < 3; i++) {
		file << "Wop[" << i << "]:\t" << tmi.omega[i] << "\n";
	}
	for (int i = 0; i < 3; i++) {
		file << "Vline[" << i << "]:\t" << tmi.Vline[i] << "\n";
	}
	file << "Tlst:\t" << DayTimeToString(tmi.timeQuatLast).c_str() << "\n";
	for (int i = 0; i < 4; i++) {
		file << "Qlst[" << i << "]:\t" << tmi.quatLast[i] << "\n";
	}
	file << "Ýïîõà: \t" << tmi.Epoch << "\n";
	file << "MaxHist: \t" << tmi.maxHist << "\n";
	file << "MaxHistX: \t" << (short)tmi.maxHistX << "\n";
	file << "MaxHistY: \t" << (short)tmi.maxHistY << "\n";
	file << "____________________________________" << "\n";
	file << flush;
}

// ïðîâåðÿåì ñîäåðæèò ëè ïðîòîêîë ðåæèì ëîêàëèçàöèè
bool checkLocFile(ifstream& in)
{
	string line;
	for (int i = 0; i < 3 ; i++)
	{
		getline(in,line);
	}
	// ïðîâåðèâ, âîçâðàùàåì óêàçàòåëü íà íà÷àëî ôàéëà
	in.clear();
	in.seekg(0, in.beg);

	if (line.find("Ëîêàëèçàöèÿ") == string::npos)
	{
		return false;
	}
	return true;
}

bool checkM2Loc(ifstream& in)
{
	bool isLoc = false;
	if (findLine(in, ";;ïîëíÄÒÌÈËîê;") != string::npos)
	{
	   isLoc = true;
	}
	in.clear();
	in.seekg(0, in.beg);
    return isLoc;
}

void writeProtocolToIKI(CadrInfo& cadrInfo, int counter)
{
	unique_ptr <IKI_img> writer (new IKI_img());
	TDateTime dt = cadrInfo.Time;
	UnicodeString dtStr;
	DateTimeToString(dtStr, UnicodeString("dd.mm.yyyy hh:nn:ss.zzz"), dt);
	writer->Georeferencing.DateTime = toStdString(dtStr);
	writer->Georeferencing.FrameNumber = ++counter;
	writer->CameraSettings.ResolutionACP = 12;

	writer->ImageData.FrameData.FrameHeight = cadrInfo.ImageHeight;
	writer->ImageData.FrameData.FrameWidth = cadrInfo.ImageWidth;
	writer->ImageData.WindowsData.SizeWindowList = cadrInfo.SizeWindowsList;
	writer->StarsData.LocalizedCount = cadrInfo.CountLocalObj;
	writer->StarsData.RecognizedCount = cadrInfo.CountDeterObj;
	writer->ImageData.WindowsData.WindowCount = cadrInfo.SizeWindowsList;
	writer->StarsData.Epsilon = cadrInfo.Epsilon;
	writer->StarsData.m_Cur = cadrInfo.MeanErrorXY;

	writer->ImageData.WindowsData.Info = new datawindow[cadrInfo.SizeWindowsList];
	for (int i = 0; i < cadrInfo.SizeWindowsList; i++)
	{
		writer->ImageData.WindowsData.Info[i].WindowWidth = cadrInfo.WindowsList[i].Width;
		writer->ImageData.WindowsData.Info[i].WindowHeight = cadrInfo.WindowsList[i].Height;
		writer->ImageData.WindowsData.Info[i].X = cadrInfo.WindowsList[i].Xstart;
		writer->ImageData.WindowsData.Info[i].Y = cadrInfo.WindowsList[i].Ystart;
		writer->ImageData.WindowsData.Info[i].Limit = cadrInfo.WindowsList[i].Level;
		writer->ImageData.WindowsData.Info[i].ObjCount = cadrInfo.WindowsList[i].CountObj;
	}

//	writer->StarsData.SimulatedFrame.SizeStarList = cadrInfo.SizeObjectsList;
//	writer->StarsData.SimulatedFrame.StarRec = new STARREC [cadrInfo.SizeObjectsList];
//	for (int i = 0; i < cadrInfo.SizeObjectsList; i++)
//	{
//		writer->StarsData.SimulatedFrame.StarRec[i].Xs = cadrInfo.ObjectsList[i].X;
//		writer->StarsData.SimulatedFrame.StarRec[i].Ys = cadrInfo.ObjectsList[i].Y;
//		writer->StarsData.SimulatedFrame.StarRec[i].Is = cadrInfo.ObjectsList[i].Bright;
//		writer->StarsData.SimulatedFrame.StarRec[i].Ns = cadrInfo.ObjectsList[i].StarID;
//		writer->StarsData.SimulatedFrame.StarRec[i].Mv = cadrInfo.ObjectsList[i].Mv;
//		writer->StarsData.SimulatedFrame.StarRec[i].Sp[0] = cadrInfo.ObjectsList[i].Sp[0];
//		writer->StarsData.SimulatedFrame.StarRec[i].Sp[1] = cadrInfo.ObjectsList[i].Sp[1];
//	}


	if (cadrInfo.SizeObjectsList != 0)
	{
		writer->StarsData.SizeLocalList = cadrInfo.SizeObjectsList;
		writer->StarsData.StarsList = new starinfo [cadrInfo.SizeObjectsList];
		for (int i = 0; i < cadrInfo.SizeObjectsList; i++)
		{
			writer->StarsData.StarsList[i].X_coordinate = cadrInfo.ObjectsList[i].X;
			writer->StarsData.StarsList[i].Y_coordinate = cadrInfo.ObjectsList[i].Y;
			writer->StarsData.StarsList[i].BrightnessObject = cadrInfo.ObjectsList[i].Bright;
			writer->StarsData.StarsList[i].NumberStar = cadrInfo.ObjectsList[i].StarID;
			writer->StarsData.StarsList[i].StellarMagnitude = cadrInfo.ObjectsList[i].Mv;
			writer->StarsData.StarsList[i].PixelsCount = cadrInfo.ObjectsList[i].Square;
			writer->StarsData.StarsList[i].DX = cadrInfo.ObjectsList[i].Dx;
			writer->StarsData.StarsList[i].DY = cadrInfo.ObjectsList[i].Dy;
			writer->StarsData.StarsList[i].SpectralClass[0] = cadrInfo.ObjectsList[i].Sp[0];
			writer->StarsData.StarsList[i].SpectralClass[1] = cadrInfo.ObjectsList[i].Sp[1];
		}
	}

	for (int i = 0; i < 3; i++)
	{
		writer->Georeferencing.OrientationAngles[i] = cadrInfo.AnglesOrient[i];
		writer->Georeferencing.DeviceAngularVelocity[i] = cadrInfo.OmegaOrient[i];
	}

	AnsiString curDate = TDateTime::CurrentDate().DateString();
	AnsiString FileName = GetCurrentDir() + "/" + "IKI_" + curDate + "/";
	TDirectory::CreateDirectory(FileName);
	char fileNumber [2];
	sprintf (fileNumber, "%02u", counter);
	FileName = FileName + IntToStr((int)counter) + "_" + TDateTime::CurrentDate().DateString() + "_00-00-" + fileNumber + ".iki";
	writer->WriteFormat(FileName.c_str());
}




void writeBOKZ1000ProtocolToIKI (CadrInfo& cadrInfo, bool InfoVecEmpty, TDateTime& startDate, double timeStep, unsigned int& counter)
{
	unique_ptr <IKI_img> writer (new IKI_img());
	UnicodeString dt;
	if (InfoVecEmpty)
	{
		DateTimeToString(dt, UnicodeString("yyyy-MM-dd hh:mm:ss.z"), startDate);
		writer->Georeferencing.DateTime = toStdString(dt);
	}
	else
	{
		startDate = IncMilliSecond(startDate, timeStep * 1000);
		DateTimeToString(dt, UnicodeString("yyyy-MM-dd hh:mm:ss.z"), startDate);
		writer->Georeferencing.DateTime = toStdString(dt);
	}

	writer->Georeferencing.FrameNumber = ++counter;
	writer->StarsData.RezStat = 0;
	writer->ImageData.FrameData.FrameHeight = 1024;
	writer->ImageData.FrameData.FrameWidth = 1024;
	writer->StarsData.SizeLocalList = cadrInfo.SizeObjectsList;
	writer->StarsData.LocalizedCount = cadrInfo.CountLocalObj;
	writer->StarsData.RecognizedCount = cadrInfo.CountDeterObj;
	writer->StarsData.Epsilon = cadrInfo.Epsilon;
	writer->ImageData.WindowsData.WindowCount = cadrInfo.CountWindows;

	writer->StarsData.StarsList = new starinfo [cadrInfo.SizeObjectsList];
	for (int i = 0; i < cadrInfo.SizeObjectsList; i++)
	{
		writer->StarsData.StarsList[i].X_coordinate = cadrInfo.ObjectsList[i].X;
		writer->StarsData.StarsList[i].Y_coordinate = cadrInfo.ObjectsList[i].Y;
		writer->StarsData.StarsList[i].BrightnessObject = cadrInfo.ObjectsList[i].Bright;
		writer->StarsData.StarsList[i].NumberStar = cadrInfo.ObjectsList[i].StarID;
		writer->StarsData.StarsList[i].StellarMagnitude = cadrInfo.ObjectsList[i].Mv;
		writer->StarsData.StarsList[i].PixelsCount = cadrInfo.ObjectsList[i].Square;
		writer->StarsData.StarsList[i].DX = cadrInfo.ObjectsList[i].Dx;
		writer->StarsData.StarsList[i].DY = cadrInfo.ObjectsList[i].Dy;
		writer->StarsData.StarsList[i].SpectralClass[0] = cadrInfo.ObjectsList[i].Sp[0];
		writer->StarsData.StarsList[i].SpectralClass[1] = cadrInfo.ObjectsList[i].Sp[1];
	}

	double matrixOfOrientation [3][3];
	quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
	double Angles[3];
	MatrixToEkvAngles(matrixOfOrientation, Angles);
	for (int i = 0; i < 3; i ++)
	{
		writer->Georeferencing.OrientationAngles[i] = Angles[i];
		writer->Georeferencing.DeviceAngularVelocity[i] = cadrInfo.OmegaOrient[i] * BOKZ1000ConvCoef;
	}

	AnsiString FileName = GetCurrentDir() + "/" + "IKI_" + TDateTime::CurrentDate().DateString() + "/";
	TDirectory::CreateDirectory(FileName);
	char fileNumber [2];
	sprintf (fileNumber, "%02u", counter);
	FileName = FileName + IntToStr((int)counter) + "_" + TDateTime::CurrentDate().DateString() + "_00-00-" + fileNumber + ".iki";
	writer->WriteFormat(FileName.c_str());

}



void readBOKZ601000MKO(ifstream& in, vector <CadrInfo>& cadrInfoVec, unsigned int& counter)
{
	string line;
	unsigned int toNextDTMI = 0;

	while (getline(in,line))
	{
		TColor pointColor = clBlue;
		if (line.find("ÊÑ:	3CA0") != string::npos)
		{
			if (toNextDTMI != 0)
			{
				--toNextDTMI;
				continue;
			}
			getline(in,line);
			getline(in,line);
			vector <string> splitted = split(line, "\t");
			string status = splitted[5];
			string status2 = splitted[6];

			if (status == "EC00")
			{
				if (status2.substr(0, 2) != "00")
				{
					pointColor = clRed;
				}
			}
			//  ÍÎ
			else if (status == "2400")
			{
				pointColor = clGreen;
				if (status2.substr(0, 2) == "0C"  || status2.substr(0, 2) == "01")
				{
					toNextDTMI = 8;
					continue;
				}
				else if (status2.substr(0, 2) != "00")
				{
					pointColor = clRed;
				}
			}
			else if (status == "0000" && status2 == "0000"
			|| status == "C400")
			{
				toNextDTMI  = 8;
				continue;
			}
			else
			{
				pointColor = clRed;
			}

			for (int i = 0; i < 8; i++)
			{
				 if (findLine(in,"ÊÑ:	3CA0") != string::npos)
				 {

				 }
			}


		}
	}

}

CadrInfo convertIKIFormatToInfoCadr(IKI_img* reader, bool CompareIKIRes)
{
	CadrInfo cadrInfo;
	cadrInfo.Time = StrToDateTime(toUString(reader->Georeferencing.DateTime)).Val;
	cadrInfo.DeviceInfo = reader->CameraSettings.DataSource;
	cadrInfo.FrameNumber = reader->Georeferencing.FrameNumber;
	cadrInfo.IsBinary = reader->ImageData.FrameData.DegreeBinning;
	cadrInfo.DataType = reader->ImageData.FrameData.DataType;
	cadrInfo.IsReverse = false;
	cadrInfo.IsOrient = !reader->StarsData.RezStat;//(reader->StarsData.RezStat == 0xFF00 || reader->StarsData.RezStat == 0x0000) ? 1 : 0;//
	cadrInfo.CountPixFilter = reader->FilterData.FilteredPixelsCount;
	cadrInfo.ImageHeight = reader->ImageData.FrameData.FrameHeight;
	cadrInfo.ImageWidth = reader->ImageData.FrameData.FrameWidth;
	cadrInfo.SizePixel = reader->CameraSettings.PixelSize;
	cadrInfo.ResolutionACP = pow(2., reader->CameraSettings.ResolutionACP) - 1;
	cadrInfo.StatOrient = reader->StarsData.RezStat;
	cadrInfo.CountStars = reader->StarsData.SimulatedFrame.strrec;
	cadrInfo.CountWindows = reader->ImageData.WindowsData.SizeWindowList;
	cadrInfo.CountLocalObj = reader->StarsData.LocalizedCount;  //.SizeLocalList;
	cadrInfo.CountDeterObj = reader->StarsData.RecognizedCount;
	cadrInfo.SizeStarsList = reader->StarsData.SimulatedFrame.SizeStarList;
	cadrInfo.SizeObjectsList = reader->StarsData.SizeLocalList; //cadrInfo.CountLocalObj;
	cadrInfo.SizeWindowsList = cadrInfo.CountWindows;

	for (int i = 0; i < cadrInfo.SizeStarsList; i++) {

		if (reader->StarsData.SimulatedFrame.StarRec[i].Xs == 0
		&&  reader->StarsData.SimulatedFrame.StarRec[i].Ys == 0)
		{
		   cadrInfo.SizeStarsList = i + 1;
		   break;
		}
		StarsInfo starInfo;
		starInfo.X = reader->StarsData.SimulatedFrame.StarRec[i].Xs;
		starInfo.Y = reader->StarsData.SimulatedFrame.StarRec[i].Ys;
		starInfo.Bright = reader->StarsData.SimulatedFrame.StarRec[i].Is;
		starInfo.StarID = reader->StarsData.SimulatedFrame.StarRec[i].Ns;
		starInfo.Mv = reader->StarsData.SimulatedFrame.StarRec[i].Mv;
		starInfo.Sp[0] = reader->StarsData.SimulatedFrame.StarRec[i].Sp[0];
		starInfo.Sp[1] = reader->StarsData.SimulatedFrame.StarRec[i].Sp[1];
		cadrInfo.StarsList.push_back(starInfo);
	}

	for (int i = 0; i < cadrInfo.SizeObjectsList; i ++)
	{
		if(reader->StarsData.StarsList[i].X_coordinate == 0
		&& reader->StarsData.StarsList[i].Y_coordinate == 0)
		{
		   cadrInfo.SizeObjectsList = i + 1;
		   break;
		}

		ObjectsInfo objInfo;
		objInfo.X = reader->StarsData.StarsList[i].X_coordinate;
		objInfo.Y = reader->StarsData.StarsList[i].Y_coordinate;
		objInfo.Square = reader->StarsData.StarsList[i].PixelsCount;
		objInfo.Bright = reader->StarsData.StarsList[i].BrightnessObject;
		objInfo.StarID = reader->StarsData.StarsList[i].NumberStar;
		objInfo.Mv = reader->StarsData.StarsList[i].StellarMagnitude;
		objInfo.Sp[0] = reader->StarsData.StarsList[i].SpectralClass[0];
		objInfo.Sp[1] = reader->StarsData.StarsList[i].SpectralClass[1];
		objInfo.Dx = reader->StarsData.StarsList[i].DX;
		objInfo.Dy = reader->StarsData.StarsList[i].DY;
		cadrInfo.ObjectsList.push_back(objInfo);
	}

	for (int i = 0; i < cadrInfo.SizeWindowsList; i ++)
	{

		if (reader->ImageData.WindowsData.Info[i].X == 0
		&& reader->ImageData.WindowsData.Info[i].Y == 0)
		{
		   cadrInfo.SizeWindowsList = i + 1;
		   break;
		}
		 WindowsInfo winInfo;
		 winInfo.Xstart = reader->ImageData.WindowsData.Info[i].X;
		 winInfo.Ystart = reader->ImageData.WindowsData.Info[i].Y;
		 winInfo.Width = reader->ImageData.WindowsData.Info[i].WindowWidth;
		 winInfo.Height = reader->ImageData.WindowsData.Info[i].WindowHeight;
		 winInfo.CountObj = reader->ImageData.WindowsData.Info[i].ObjCount;
		 winInfo.Mean =  reader->ImageData.WindowsData.Info[i].Average;
		 winInfo.Sigma =  reader->ImageData.WindowsData.Info[i].SKO;
		 winInfo.Level =  reader->ImageData.WindowsData.Info[i].Limit;
		 winInfo.ZipX = reader->ImageData.WindowsData.Info[i].ZipX;
		 winInfo.ZipY = reader->ImageData.WindowsData.Info[i].ZipY;
		 cadrInfo.WindowsList.push_back(winInfo);
	}

	int count = cadrInfo.SizeWindowsList < cadrInfo.SizeObjectsList ? cadrInfo.SizeWindowsList : cadrInfo.SizeObjectsList;
	for (int i = 0; i < count; i++)
	{
		if (cadrInfo.ObjectsList[i].StarID == 0
		&& cadrInfo.WindowsList[i].CountObj > 1)
		{
			cadrInfo.WindowsList[i].StarID = cadrInfo.ObjectsList[i + 1].StarID;
			cadrInfo.WindowsList[i].Bright = cadrInfo.ObjectsList[i + 1].Bright;
			cadrInfo.WindowsList[i].xCenter = cadrInfo.ObjectsList[i + 1].X - cadrInfo.WindowsList[i].Xstart;
			cadrInfo.WindowsList[i].yCenter = cadrInfo.ObjectsList[i + 1].Y - cadrInfo.WindowsList[i].Ystart;
			i++;
		}
		else
		{
		   cadrInfo.WindowsList[i].StarID = cadrInfo.ObjectsList[i].StarID;
		   cadrInfo.WindowsList[i].Bright = cadrInfo.ObjectsList[i].Bright;
		   cadrInfo.WindowsList[i].xCenter = cadrInfo.ObjectsList[i].X - cadrInfo.WindowsList[i].Xstart;
		   cadrInfo.WindowsList[i].yCenter = cadrInfo.ObjectsList[i].Y - cadrInfo.WindowsList[i].Ystart;
		}
		
	}

	if (reader->ImageData.WindowsData.SizeData != 0) {

		string dirName = AnsiString(GetCurrentDir()).c_str() + string("/") + "Frag" + "_" + "IKI";
		TDirectory::CreateDirectory(dirName.c_str());
		string time = AnsiString(FloatToStr(cadrInfo.Time)).c_str();
		string fileName = dirName + "/" + "Frag" + "_" + time + ".bin";
		ofstream fragmentFile(fileName.c_str(), ios::binary | ios::trunc);

		if(fragmentFile.is_open())
		{
			char bytesInFormat = 1;
			switch (cadrInfo.DataType)
			{
				case 0: break;
				case 1: bytesInFormat = 2; break;
				case 2: bytesInFormat = 4; break;
			}

		  fragmentFile.write(
		  (char*)(reader->ImageData.WindowsData.Data),
		   reader->ImageData.WindowsData.SizeData * bytesInFormat);
		  fragmentFile.close();
		}
	}

	cadrInfo.CountLines = reader->ImageData.LinesData.LinesHeight;
	cadrInfo.CountBlock = reader->ImageData.LinesData.LinesCount;

	for (int i = 0; i < cadrInfo.CountBlock; i ++)
	{
		LinesInfo lineInfo;
		lineInfo.Start = reader->ImageData.LinesData.Info[i].Y_FirstString;
		lineInfo.Height = reader->ImageData.LinesData.Info[i].CountString;
		cadrInfo.LinesList.push_back(lineInfo);
	}

	cadrInfo.MeanErrorX = reader->StarsData.m_X;
	cadrInfo.MeanErrorY = reader->StarsData.m_Y;
	cadrInfo.MeanErrorXY = reader->StarsData.m_Cur;

	for (int i = 0; i < 4; i ++)
	{
		cadrInfo.QuatOrient[i] = reader->StarsData.RecognizedOrientationQuaternion[i];
	}

	for (int i = 0; i < 3; i ++)
	{
		cadrInfo.AnglesOrient[i] = reader->StarsData.RecognizedOrientationAngles[i];
		cadrInfo.AnglesModel[i] = reader->Georeferencing.OrientationAngles[i];
		cadrInfo.OmegaOrient[i] = reader->StarsData.RecognizedAngularVelocity[i];
		cadrInfo.OmegaModel[i] = reader->Georeferencing.DeviceAngularVelocity[i];

		for (int j = 0; j < 3; j ++)
		{
			cadrInfo.MatrixOrient[i][j] = reader->StarsData.RecognizedOrientationMatrix[i][j];
		}
	}

	if ((CompareIKIRes) && (cadrInfo.IsOrient))
	{
		for (int i = 0; i < 3; i++)
		{
			cadrInfo.OmegaDiff[i] = reader->StarsData.RecognizedAngularVelocity[i]
			- reader->Georeferencing.DeviceAngularVelocity[i];
		}

		for (int i = 0;  i < 3; i++)
		{
			double diff = cadrInfo.AnglesOrient[i] - reader->Georeferencing.OrientationAngles[i];
//			if (abs(diff) > 5)        // ïîòîì óáðàòü
//			{
//				diff = (cadrInfo.AnglesOrient[i] + reader->Georeferencing.OrientationAngles[i])
//				-	abs(cadrInfo.AnglesOrient[i] - reader->Georeferencing.OrientationAngles[i]);
//			}
			if (abs(diff) >  PI) diff -= 2*PI;
			if (abs(diff) < -PI) diff += 2*PI;
			cadrInfo.AnglesDiff[i] = diff;
		}

		for (int i = 0; i < 3; i++)
		{
			cadrInfo.AxesDiff[i] = GetAxisAngle(&reader->Georeferencing.OrientationMatrix[i][0],
												&cadrInfo.MatrixOrient[i][0]);
		}
	}

	GetImageBright(cadrInfo);
	return cadrInfo;
}

	void parseMILHex(vector <string>& parseTo, int rowCount, ifstream& in, int offset)
	{
			string line;
			for (int i = 0 ; i < rowCount; i++) {
                getline(in, line);
				vector <string> row =  split(line, " ");
				if (i == 0) {
					for (int j = 0; j < offset; j++) {
						  row.erase(row.begin());
					}
				}
				else
				{
                    row.erase(row.begin());
                }
			   parseTo.insert(parseTo.end(), row.begin(), row.end());
			}
	}

}
//---------------------------------------------------------------------------
#pragma package(smart_init)
