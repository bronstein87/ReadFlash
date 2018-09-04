//---------------------------------------------------------------------------

#pragma hdrstop

#include "ParseProtocol.h"
using namespace add_string;
namespace parse_prot {

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

	unsigned int ReadBinaryString(string binaryString) {
		string test_str;
		double sum = 0;

		for (int k = 0; k < binaryString.length(); k++) {
			test_str = binaryString[binaryString.length() - k - 1];
			sum += pow(2., k) * atoi(test_str.c_str());
		}
		return (unsigned int) sum;
	}

//-------------‘ÛÌÍˆËË ‰Îˇ ˜ÚÂÌËˇ ÔÓÚÓÍÓÎÓ‚ – ÷ "œÓ„ÂÒÒ"-------------------//
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
		int day, hour, min, sec;
		double day_double = time/86400.;///8.;

		day = (int)day_double;
		hour = (day_double - (int)day) * 24 + 1e-7;
		min  = (day_double - day - hour/24.) * 24 * 60 + 1e-7;
		sec  = (day_double - day - hour/24. - min/24./60.) * 86400. + 1e-7;
		if (day > 0) {
			sTime  = IntToStr(day) + "/";
		}
		str.sprintf(" %02d:%02d:%02d", hour, min, sec);
		sTime += str;
		return sTime;
	}

	int StopReadArray(string line) {
		if ( (line.find("----") != string::npos) ||
			 (line.find("ƒ“Ã»") != string::npos) ||
			 (line.find("ÿ“Ã»") != string::npos) ||
			 (line.find("”3-") != string::npos)  ||
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

			if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "—≈–") || (word == "CEP") || (word == "—EP") || (word == "—E–")) {
				finp >> word;
				if ((word == "ÕŒÃ") || (word == "HOM")|| (word == "ÕŒM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "œŒ—“") || (word == "œOCT") || (word == "œO—T")) {
				finp >> word;
				tmi.post = ReadBinaryString(word);
			}
			else if ((word == "T") || (word == "“")) {
				finp >> word;
				if (word == "› —œ")
					finp >> tmi.timeExp;
				else {
					getline(finp, line, '\n');
					word = word + line;
					tmi.timeBOKZ = StringToDayTime(word);
				}
			}
			else if ((word == "‘OK") || (word == "‘O "))
				finp >> tmi.Foc;
			else if ((word == "’0") || (word == "X0"))
				finp >> tmi.Xg;
			else if ((word == "”0") || (word == "Y0"))
				finp >> tmi.Yg;
			else if ((word == "Ã“") || (word == "MT"))
				finp >> tmi.Mean;
			else if ((word == "—“") || (word == "CT") || (word == "—T"))
				finp >> tmi.Sigma;
			else if ((word == "Õ¿Ã") || (word == "HAM")) {
				finp >> word;
				if ((word == "ƒ≈‘") || (word == "ƒE‘"))
					finp >> tmi.countDefect;
			}
			else if (word == "’—‘")
				finp >> tmi.CRC;
			else if (word == "ƒ¿“¿")
				finp >> tmi.Date;
			else if (word == "¬≈–—»ﬂ") {
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
			if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "—≈–") || (word == "CEP") || (word == "—EP") || (word == "—E–")) {
				finp >> word;
				if ((word == "ÕŒÃ") || (word == "HOM")|| (word == "ÕŒM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "œŒ—“") || (word == "œOCT") || (word == "œO—T")) {
				finp >> word;
				tmi.post = ReadBinaryString(word);
			}
			else if ((word == "T") || (word == "“")) {
				finp >> word;
				if (word == "› —œ")
					finp >> tmi.timeExp;
				else {
					getline(finp, line, '\n');
					word = word + line;
					tmi.timeBOKZ = StringToDayTime(word);
				}
			}
			else if (word == "Õ¿Ã") {
				finp >> word;
				if (word == "”—ƒ")
					finp >> tmi.cntCommandWord;
			}
			else if ((word == "◊»—À") || (word == "◊»—ÀŒ") || (word == "◊»CÀ") || (word == "◊»CÀO")) {
				finp >> word;
				if ((word == "ÕŒ") || (word == "HO")) {
					finp >> tmi.cntCallNO;
				}
				else if (word == "ÕŒ—À") {
					finp >> tmi.cntNOtoSLEZH;
				}
				if ((word == "TO") || (word == "“Œ")) {
					finp >> tmi.cntCallTO;
				}
				else if (word == "“Œ—À") {
					finp >> tmi.cntTOtoSLEZH;
				}
				else if (word == "—À≈∆") {
					finp >> tmi.cntSLEZH;
					tmi.cntSLEZH *= 2;
				}
			}
			else if ( (word.find("EC") != string::npos)
					  || (word.find("≈—") != string::npos) ) {
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
			if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "—≈–") || (word == "CEP") || (word == "—EP") || (word == "—E–")) {
				finp >> word;
				if ((word == "ÕŒÃ") || (word == "HOM")|| (word == "ÕŒM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "HAM") || (word == "Õ¿Ã") || (word == "Õ¿M")) {
				finp >> word;
				if ((word == "ÀŒ ") || (word == "ÀO ")) {
					finp >> tmi.nLocalObj;
				}
				else if ((word == "Œ¡∆") || (word == "O¡∆")) {
					finp >> tmi.nDeterObj;
				}
				else if (word == "‘–¿√") {
					finp >> tmi.nWindows;
				}
			}
			else if (word == "›œ—»ÀŒÕ")
				finp >> tmi.epsillon;
			else if (word == "ƒ≈À‹“¿") {
				finp >> word;
				if ((word == "T") || (word == "“"))
					finp >> tmi.dTimeBOKZ;
			}
			else if ((word == "T") || (word == "“")) {
				finp >> word;
				if (word == "› —œ")
					finp >> tmi.timeExp;
				else if (word == "œ’Œ ") {
					getline(finp, line, '\n');
					tmi.timeQuatLast = StringToDayTime(line);
				}
				else {
					getline(finp, line, '\n');
					word = word + line;
					tmi.timeBOKZ = StringToDayTime(word);
				}
			}
			else if (word == "ÀŒ ") {
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
			else if (word == "ŒÃ≈√¿Œ") {
				finp >> indexParam;
				if ((indexParam >= 0) && (indexParam < 3)) {
					finp >> tmi.omega[indexParam];
				}
			}
			else if ((word == "XYC") || (word == "’”—")) {
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
			else if (word == "“Õ‘–¿√") {
				finp >> indexObject;
				if ((indexObject >= 0) && (indexObject < MAX_WINDOW)) {
					finp >> tmi.levelWindow[indexObject];
				}
			}
			else if (word == "Œ¡∆‘–¿√") {
				finp >> indexObject;
				if ((indexObject >= 0) && (indexObject < MAX_WINDOW)) {
					finp >> tmi.nObjectWindow[indexObject];
				}
			}
			else if (word == "œ’Œ ") {
				finp >> indexParam;
				if ((indexParam >= 0) && (indexParam < 4)) {
					finp >> tmi.quatLast[indexParam];
				}
			}
			else if (word == "›œŒ’¿") {
				int test_int;
				float test_float;
				finp >> test_int;
				test_float = *(float*)&test_int;
				tmi.Epoch = test_float;
			}
			else if (word == "Õ”ÃÀ") {
				finp >> indexParam;
				if ((indexParam >= 0) && (indexParam < 2)) {
					finp >> tmi.nLocal[indexParam];
				}
			}
			else if ((word == "MAXH") || (word == "Ã¿’Õ") || (word == "Ã¿’Õ")) {
				finp >> tmi.maxHist;
			}
			else if ((word == "ƒ’Ã¿’Õ") || (word == "ƒ’Ã¿’Õ")) {
				finp >> tmi.maxHistX;
			}
			else if ((word == "ƒ”Ã¿’Õ") || (word == "ƒ”Ã¿’Õ")) {
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
//				while ((line.find("ƒ“Ã»")==string::npos) && (!finp.eof())) {
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
		tmi.Reserved[0] = 0;
		tmi.Reserved[1] = 0;
	}


	int TryReadLOC(ifstream &finp, struct LOC &tmi) {
		string line, word, inpstr, test_word, test_str;
		int indexObject = 0, indexParam = 0, intVal, flLow = 1;
		float fl1, fl2, fl3, sum;
		int Stat1, Stat2;

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "—≈–") || (word == "CEP") || (word == "—EP") || (word == "—E–")) {
				finp >> word;
				if ((word == "ÕŒÃ") || (word == "HOM")|| (word == "ÕŒM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "T") || (word == "“")) {
				finp >> word;
				if (word == "› —œ")
					finp >> tmi.timeExp;
				else {
					getline(finp, line, '\n');
					word = word + line;
					tmi.timeBOKZ = StringToDayTime(word);
				}
			}
			else if ((word == "HAM") || (word == "Õ¿Ã") || (word == "Õ¿M")) {
				finp >> word;
				if ((word == "ÀŒ ") || (word == "ÀO ")) {
					finp >> tmi.nLocalObj;
				}
				else if ((word == "Œ¡∆") || (word == "O¡∆")) {
					finp >> tmi.nFixedObj;
				}
			}
			else if (word == "Ã≈¿Õ") {
				finp >> tmi.MeanC;
			}
			else if (word == "—»√Ã¿") {
				finp >> tmi.SigmaC;
			}
			else if (word == "Õ œ» —") {
				finp >> tmi.Reserved[0];
			}
			else if (word == "ÀŒ ") {
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
//				while ((line.find("ƒ“Ã»")==string::npos) && (!finp.eof())) {
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

	void PrintSHTMI1(ofstream &file, struct SHTMI1 tmi) {
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ÿ“Ã»1" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "«‡‚. π\t" << tmi.serialNumber << "\n";
		file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
		file << "Foc, ÏÏ:\t" << tmi.Foc << "\n";
		file << "X0, ÏÏ:\t" << tmi.Xg << "\n";
		file << "Y0, ÏÏ:\t" << tmi.Yg << "\n";
		file << "—Â‰ÌÂÂ, Â.Ï..:\t" << tmi.Mean << "\n";
		file << "— Œ, Â.Ï..:\t" << tmi.Sigma << "\n";
		file << "◊ËÒÎÓ ‰ÂÙÂÍÚÓ‚:\t" << tmi.countDefect << "\n";
		file << "»‰ÂÌÚËÙËÍ‡ÚÓ:\t" << tmi.CRC << "\n";
		file << "ƒ‡Ú‡:\t" << tmi.Date << "\n";
		file << "¬ÂÒËˇ:\t" << tmi.Version << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintSHTMI2(ofstream &file, struct SHTMI2 tmi) {
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ÿ“Ã»2" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "«‡‚. π\t" << tmi.serialNumber << "\n";
		file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
		file << "◊ËÒÎÓ ”—ƒ:\t" << tmi.cntCommandWord << "\n";
		file << "◊ËÒÎÓ ÕŒ:\t" << tmi.cntCallNO << "\n";
		file << "ÕŒ->—À≈∆:\t" << tmi.cntNOtoSLEZH << "\n";
		file << "◊ËÒÎÓ TŒ:\t" << tmi.cntCallTO << "\n";
		file << "TŒ->—À≈∆:\t" << tmi.cntTOtoSLEZH << "\n";
		file << "◊ËÒÎÓ —À≈∆:\t" << tmi.cntSLEZH << "\n";

		for (int i = 0; i < MAX_STAT; i++) {
			file << "—˜ÂÚ˜ËÍ π " << (i + 1) << ":\t" << tmi.cntStatOrient[i]
				<< "\n";
		}
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintDTMI(ofstream &file, struct DTMI tmi) {
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ƒ“Ã»" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec << setfill(' ');
		file << "«‡‚. π\t" << tmi.serialNumber << "\n";
		file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
		file << "NumLoc: \t" << tmi.nLocalObj << "\n";
		file << "NumDet: \t" << tmi.nDeterObj << "\n";
		file << "NumFrag:\t" << tmi.nWindows << "\n";
		file << "EPSILON:\t" << tmi.epsillon << "\n";
		file << "DeltaT:\t" << tmi.dTimeBOKZ << "\n";
		file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
		for (int i = 0; i < MAX_OBJ_DTMI; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.LocalList[i][0] << "\t" << tmi.LocalList[i][1] << "\t";
			file << tmi.LocalList[i][2] << "\t";
			if ( (tmi.LocalList[i][3] < 0.001) && (tmi.LocalList[i][2] > 0.001) ) {
				short *level, *size;
				size = (short *)(&tmi.LocalList[i][3]);
				level = (short *)(size + 1);
				file << *size << "\t" << *level << "\n";
			}
			else file << tmi.LocalList[i][3] << "\n";
		}
		file << setw(6) << "π" << " X, pix" << " Y, pix" << "\n";
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
		file << "›ÔÓı‡: \t" << tmi.Epoch << "\n";
		file << "MaxHist: \t" << tmi.maxHist << "\n";
		file << "MaxHistX: \t" << tmi.maxHistX << "\n";
		file << "MaxHistY: \t" << tmi.maxHistY << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLOC(ofstream &file, struct LOC tmi) {
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ÃÀŒ " << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec << setfill(' ');
		file << "«‡‚. π\t" << tmi.serialNumber << "\n";
		file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
		file << "NumLoc: \t" << tmi.nLocalObj << "\n";
		file << "NumFix: \t" << tmi.nFixedObj << "\n";
		file << "MeanC:\t" << tmi.MeanC << "\n";
		file << "SigmaC:\t" << tmi.SigmaC << "\n";
		file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
		for (int i = 0; i < 32; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.LocalList[i][0] << "\t" << tmi.LocalList[i][1] << "\t";
			file << tmi.LocalList[i][2] << "\t";
			if ( (tmi.LocalList[i][3] < 0.001) && (tmi.LocalList[i][2] > 0.001) ) {
				short *level, *size;
				size = (short *)(&tmi.LocalList[i][3]);
				level = (short *)(size + 1);
				file << *size << "\t" << *level << "\n";
			}
			else file << tmi.LocalList[i][3] << "\n";
		}

		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLocalDTMI(AnsiString fileDir, TDateTime curTime, struct DTMI tmi) {

		AnsiString stringSerial, localDir, localName;
		TFormatSettings curFormat;

		stringSerial.sprintf("«‡‚. π %02d", tmi.serialNumber);
		localDir = fileDir + "\\" + stringSerial;
		CreateDir(localDir);
//		localDir = localDir + "\\" + AnsiString(DateToStr(curTime)).c_str();
//		CreateDir(localDir);

		curFormat.ShortDateFormat = "yyyy_mm_dd";
		curFormat.LongTimeFormat  = "hh_nn_ss_zzz";
		localName.sprintf("%s_%s_DTMI_LOC.txt",
				AnsiString(DateToStr(curTime, curFormat)).c_str(),
				AnsiString(TimeToStr(curTime, curFormat)).c_str());
		CheckFileName(localName);
		localName = localDir + "\\" + localName;

		ofstream file(localName.c_str());
		file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" <<
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
			if (tmi.LocalList[i][3] < 1) {
				short *level, *size;
				size = (short *)(&tmi.LocalList[i][3]);
				level = (short *)(size + 1);
				file << abs(*size) << "\t" << *level << "\n";
			}
			else file << fabs(tmi.LocalList[i][3]) << "\n";
		}
		file.close();
	}

	void PrintLocalMLOC(AnsiString fileDir, TDateTime curTime, struct LOC tmi) {

		AnsiString stringSerial, localDir, localName;
		TFormatSettings curFormat;

		stringSerial.sprintf("«‡‚. π %02d", tmi.serialNumber);
		localDir = fileDir + "\\" + stringSerial;
		CreateDir(localDir);
//		localDir = localDir + "\\" + AnsiString(DateToStr(curTime)).c_str();
//		CreateDir(localDir);

		curFormat.ShortDateFormat = "yyyy_mm_dd";
		curFormat.LongTimeFormat  = "hh_nn_ss_zzz";
		localName.sprintf("%s_%s_MLOC_LOC.txt",
				AnsiString(DateToStr(curTime, curFormat)).c_str(),
				AnsiString(TimeToStr(curTime, curFormat)).c_str());
		CheckFileName(localName);
		localName = localDir + "\\" + localName;

		ofstream file(localName.c_str());

		file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" <<
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
			if (tmi.LocalList[i][3] < 1) {
				short *level, *size;
				size = (short *)(&tmi.LocalList[i][3]);
				level = (short *)(size + 1);
				file << abs(*size) << "\t" << *level << "\n";
			}
			else file << tmi.LocalList[i][3] << "\n";
		}
		file.close();
	}

	void ConvertDataDTMI(struct DTMI tmi, struct CadrInfo &mCadr) {
		mCadr.IsBinary = true;
		// mCadr.IsReverse=true;
		mCadr.ImageHeight = 256;
		mCadr.ImageWidth = 256;
		// mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;

		mCadr.CountDeterObj = tmi.nDeterObj;
		mCadr.CountWindows = tmi.nWindows;
		mCadr.CountLocalObj = tmi.nLocalObj;

		if (tmi.nLocalObj < MAX_OBJ_DTMI)
			mCadr.SizeObjectsList = tmi.nLocalObj;
		else
			mCadr.SizeObjectsList = MAX_OBJ_DTMI;

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
		for (int i = 0; i < mCadr.SizeWindowsList; i++) {
			winInfo.Level = tmi.levelWindow[i];
			winInfo.CountObj = tmi.nObjectWindow[i];
			winInfo.Width = 17;
			winInfo.Height = 17;
			winInfo.Xstart = tmi.centrWindow[i][0] - (winInfo.Width >> 1);
			winInfo.Ystart = tmi.centrWindow[i][1] - (winInfo.Height >> 1);
			mCadr.WindowsList.push_back(winInfo);

			starList.X = tmi.centrWindow[i][0];
			starList.Y = tmi.centrWindow[i][1];
		}

		mCadr.CountBlock = 0;
		mCadr.CountStars = 0;
	}

	void ConvertDataLOC(struct LOC tmi, struct CadrInfo &mCadr) {
		mCadr.IsBinary = true;
		// mCadr.IsReverse=true;
		mCadr.ImageHeight = 256;
		mCadr.ImageWidth = 256;
		// mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;
		mCadr.CountLocalObj = tmi.nLocalObj;

		if (tmi.nLocalObj < MAX_OBJ_MLOC)
			mCadr.SizeObjectsList = tmi.nLocalObj;
		else
			mCadr.SizeObjectsList = MAX_OBJ_MLOC;

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

//-------------‘ÛÌÍˆËË ‰Îˇ ˜ÚÂÌËˇ ÔÓÚÓÍÓÎÓ‚ –   "›ÌÂ„Ëˇ"--------------------//
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
	//	mCadr.IsReverse=true;
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
		file << "Ã‡ÒÒË‚ Ãÿ»Œ–" << "\n";
		file << "Tpr\t" << tmi.timeBOKZ << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec << setfill(' ');
		file << "Ã‡ÚËˆ‡ ÓËÂÌÚ‡ˆËË:\n";
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
	//	file<<"”„Î˚ ÓËÂÌÚ‡ˆËË:\n";
	//	file<<"Al = "<<ang[0]*RTD<<"\t";
	//	file<<"Dl = "<<ang[1]*RTD<<"\t";
	//	file<<"Az = "<<ang[2]*RTD<<"\n";
		file<<"____________________________________"<<"\n";
		file<<flush;
	}

	void PrintSHTMI1_BOKZM(ofstream &file, struct SHTMI1_BOKZM tmi) {
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ÿ“Ã»1" << "\n";
		file << "Tpr\t" << tmi.timeBOKZ << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "«‡‚. π\t" << tmi.serialNumber << "\n";
		file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
		file << "Foc, ÏÏ:\t" << tmi.Foc << "\n";
		file << "X0, ÏÏ:\t" << tmi.Xg << "\n";
		file << "Y0, ÏÏ:\t" << tmi.Yg << "\n";
		file << "—Â‰ÌÂÂ, Â.Ï..:\t" << tmi.Mean << "\n";
		file << "— Œ, Â.Ï..:\t" << tmi.Sigma << "\n";
		file << "◊ËÒÎÓ ‰ÂÙÂÍÚÓ‚:\t" << tmi.countDefect << "\n";
		file << "ƒ‡Ú‡:\t" << tmi.Date << "\n";
		file << "¬ÂÒËˇ XCF:\t" << tmi.verXCF << "\n";
		file << "¬ÂÒËˇ œŒ:\t" << tmi.verProg << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintSHTMI2_BOKZM(ofstream &file, struct SHTMI2_BOKZM tmi) {
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ÿ“Ã»2" << "\n";
		file << "Tpr\t" << tmi.timeBOKZ << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "«‡‚. π\t" << tmi.serialNumber << "\n";
		file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
		file << "Foc, ÏÏ:\t" << tmi.Foc << "\n";
		file << "X0, ÏÏ:\t" << tmi.Xg << "\n";
		file << "Y0, ÏÏ:\t" << tmi.Yg << "\n";
		file << "—Â‰ÌÂÂ, Â.Ï..:\t" << tmi.Mean << "\n";
		file << "— Œ, Â.Ï..:\t" << tmi.Sigma << "\n";
		file << "◊ËÒÎÓ ‰ÂÙÂÍÚÓ‚:\t" << tmi.countDefect << "\n";
		file << "◊ËÒÎÓ ÕŒ:\t" << tmi.cntCallNO << "\n";
		file << "◊ËÒÎÓ TŒ:\t" << tmi.cntCallTO << "\n";

		for (int i = 0; i < 12; i++) {
			file << "—˜ÂÚ˜ËÍ π " << (i + 1) << ":\t" << tmi.cntStatOrient[i]
				<< "\n";
		}
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintDTMI_BOKZM(ofstream &file, struct DTMI_BOKZM tmi)
	{
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ƒ“Ã»"<<"\n";
		file << "Tpr\t"<<tmi.timeBOKZ<<"\n";
		file << uppercase<<hex<<setfill('0');
		file << " —1\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec<<setfill(' ');
		file << "«‡‚. π\t" << tmi.serialNumber << "\n";
		file << "Foc, ÏÏ:\t" << tmi.Foc << "\n";
		file << "X0, ÏÏ:\t" << tmi.Xg << "\n";
		file << "Y0, ÏÏ:\t" << tmi.Yg << "\n";
		file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
		file << "NumLoc: \t" << tmi.nLocalObj << "\n";
		file << "NumDet: \t" << tmi.nDeterObj << "\n";
		file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" << " Nel" << "\n";
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

		file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" <<
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
	file << "Ã‡ÒÒË‚ ƒ“Ã»" << "\n";
	file << "Tpr\t" << tmi.timeBOKZ << "\n";
	file << uppercase << hex << setfill('0');
	file << " —1\t" << "0x" << setw(4) << tmi.status1 << "\n";
	file << " —2\t" << "0x" << setw(4) << tmi.status2 << "\n";
	file << "POST\t" << "0x" << setw(4) << tmi.POST << "\n";
	file << dec << setfill(' ');
	file << "«‡‚. π\t" << tmi.serialNumber << "\n";
	file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
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

	file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
	for (int i = 0; i < 15; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.LocalList1[i].x << "\t" << tmi.LocalList1[i].y << "\t";
		file << tmi.LocalList1[i].bright << "\t" << tmi.LocalList1[i].size << "\n";
	}

	file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
	for (int i = 0; i < 15; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.LocalList2[i].x << "\t" << tmi.LocalList2[i].y << "\t";
		file << tmi.LocalList2[i].bright << "\t" << tmi.LocalList2[i].size << "\n";
	}

	file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
	for (int i = 0; i < 12; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.ResultList[i].x << "\t" << tmi.ResultList[i].y << "\t";
		file << tmi.ResultList[i].bright << "\t" << tmi.ResultList[i].size << "\n";
	}
	file << setw(6) << "π" << " X, pix" << " Y, pix" << "\n";
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
	file << "›ÔÓı‡: \t" << tmi.Epoch << "\n";
	file << "MaxHist: \t" << tmi.maxHist << "\n";
	file << "MaxHistX: \t" << tmi.maxHistX << "\n";
	file << "MaxHistY: \t" << tmi.maxHistY << "\n";
	file << "____________________________________" << "\n";
	file << flush;
}

// ÔÓ‚ÂˇÂÏ ÒÓ‰ÂÊËÚ ÎË ÔÓÚÓÍÓÎ ÂÊËÏ ÎÓÍ‡ÎËÁ‡ˆËË
bool checkLocFile(ifstream& in)
{
	string line;
	for (int i = 0; i < 3 ; i++)
	{
		getline(in,line);
	}
	// ÔÓ‚ÂË‚, ‚ÓÁ‚‡˘‡ÂÏ ÛÍ‡Á‡ÚÂÎ¸ Ì‡ Ì‡˜‡ÎÓ Ù‡ÈÎ‡
	in.clear();
	in.seekg(0, in.beg);

	if (line.find("ÀÓÍ‡ÎËÁ‡ˆËˇ") == string::npos)
	{
		return false;
	}
	return true;
}

bool checkM2Loc(ifstream& in)
{
	bool isLoc = false;
	if (findLine(in, ";;ÔÓÎÌƒ“Ã»ÀÓÍ;") != string::npos)
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
		if (line.find(" —:	3CA0") != string::npos)
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
			//  ÕŒ
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
				 if (findLine(in," —:	3CA0") != string::npos)
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
			if (abs(diff) > 5)        // ÔÓÚÓÏ Û·‡Ú¸
			{
				diff = (cadrInfo.AnglesOrient[i] + reader->Georeferencing.OrientationAngles[i])
				-	abs(cadrInfo.AnglesOrient[i] - reader->Georeferencing.OrientationAngles[i]);
			}
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


}
//---------------------------------------------------------------------------
#pragma package(smart_init)
