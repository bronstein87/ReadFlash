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

	int FloatToInteger(const float var, int& res)
	{
		if ( (var > INT_MAX) || (var < INT_MIN) ) return -1;

		res = (int)var;
		return 0;
	}

	void SwapShort(short *word1, short *word2)
	{
		short buf;
		buf = *word1;
		*word1 = *word2;
		*word2 = buf;
	}

    void Swap2Word(short word[2])
	{
		short buf;
		buf = word[0];
		word[0] = word[1];
		word[1] = buf;
	}

	int GetSizeFrag_2R(char multXY, unsigned short *fragX,  unsigned short *fragY)     //4 bits
	{
		int fragXY[2];
		fragXY[0] = (multXY >> 2) & 0x3;
		fragXY[1] =  multXY & 0x3;
		for (int i = 0; i < 2; i++) {
			switch (fragXY[i]) {
				case 0: { fragXY[i] = 96; break;}     //00
				case 1: { fragXY[i] = 72; break;}     //01
				case 2: { fragXY[i] = 48; break;}    //10
				case 3: { fragXY[i] = 24; break;}     //11
				default: return -1;
			}
		}
		*fragX = fragXY[0];
		*fragY = fragXY[1];
		return 0;
	}

	int GetSizeFrag_MR(char multXY, unsigned short *fragX,  unsigned short *fragY)     //4 bits
	{
		int fragXY[2];
		fragXY[0] = (multXY >> 2) & 0x3;
		fragXY[1] =  multXY & 0x3;
		for (int i = 0; i < 2; i++) {
			switch (fragXY[i]) {
				case 0: { fragXY[i] = 18; break;}     //00
				case 1: { fragXY[i] = 24; break;}     //01
				case 2: { fragXY[i] = 48; break;}    //10
				default: return -1;
			}
		}
		*fragX = fragXY[0];
		*fragY = fragXY[1];
		return 0;
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

	void PrintProgDate(ofstream &file, unsigned short date, unsigned short year0)
	{
		file << setfill('0');
		file << ((date & 0xF000) >> 12) + year0 << ".";
		file << setw(2) << ((date & 0x0F00) >> 8) << ".";
		file << setw(2) << (date & 0xFF);
		file << setfill(' ');
	}

	void PrintProgVersion(ofstream &file, unsigned short version)
	{
		file << ((version & 0xF000) >> 12) << ".";
		file << ((version & 0x0F00) >> 8)  << ".";
		file << (version & 0xFF);
	}

//-------------‘ÛÌÍˆËË ‰Îˇ ˜ÚÂÌËˇ ÔÓÚÓÍÓÎÓ‚ – ÷ "œÓ„ÂÒÒ"-------------------//
	TDateTime GetDateTime(TDateTime _zeroDate, unsigned int _timeBOKZ)
	{
		TDateTime curDate;
		curDate.Val = _zeroDate.Val + _timeBOKZ * clkBokz/86400.;
		return curDate;
	}

	TDateTime GetDateTime_2R(TDateTime _zeroDate, unsigned int _time_sec,
												  unsigned int _time_msec)
	{
		TDateTime curDate;
		curDate.Val = _zeroDate.Val + (_time_sec + _time_msec/1000.)/86400.;
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

	AnsiString OutputDateTime2(TDateTime _curDate)
	{
		unsigned short d, ms;
		UnicodeString uFormat;
		DecodeTime(_curDate, d, d, d,  ms);
		if (ms < 1) uFormat = "yyyy-mm-dd hh:mm:ss";
		else uFormat = "yyyy-mm-dd hh:mm:ss.zzz";
		return (AnsiString)FormatDateTime(uFormat, _curDate);
	}

	int StopReadArray(string line) {
		if ( (line.find("#") != string::npos) ||
//		if ( (line.find("----") != string::npos) ||
			 (line.find("ƒ“Ã»") != string::npos) ||
			 (line.find("ÿ“Ã»") != string::npos) ||
			 (line.find("”3-") != string::npos)  ||
			 (line.find("U3-") != string::npos) ) {
			return 1;
		}
		else return 0;
	}


	void ClearGyro(struct GYRO &tmi) {
		tmi.timeGyro = 0;
		for (int i = 0; i < 3; i++) {
			tmi.omegaSat[i] = 0;
			tmi.omegaGyro[i] = 0;
		}
		for (int i = 0; i < 4; i++) {
			tmi.quatSat[i] = 0;
			tmi.quatGyro[i] = 0;
		}
	}

	int TryReadGyro(ifstream &finp, struct GYRO &tmi) {

        int indexParam;
		string line, word = "";

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if (word == "¬–≈Ãﬂ") {
				finp >> word;
				if (word == "¿3") {
					getline(finp, line, '\n');
					tmi.timeGyro = StringToDayTime(line);
				}
			}
			else if (word == "— ") {
				finp >> word;
				if (word == "«¿ƒ") {
					finp >> word;
					if ((word == "X") || (word == "’") ) finp >> tmi.omegaSat[0];
					else if (word == "Y")  finp >> tmi.omegaSat[1];
					else if	(word == "Z")  finp >> tmi.omegaSat[2];
				}
			}
			else if (word == "— Œ–") {
				finp >> word;
				if ((word == "X") || (word == "’") ) finp >> tmi.omegaGyro[0];
				else if (word == "Y")  finp >> tmi.omegaGyro[1];
				else if	(word == "Z")  finp >> tmi.omegaGyro[2];
			}
			else if (word == " ¬") {
				finp >> word;
				if (word == "¿3") {
					finp >> indexParam;
					if ((indexParam >= 0) && (indexParam < 4)) {
						finp >> tmi.quatGyro[indexParam];
					}
				}
				else if (word == "«¿ƒ") {
					finp >> indexParam;
					if ((indexParam >= 0) && (indexParam < 4)) {
						finp >> tmi.quatSat[indexParam];
					}
					if (indexParam == 3) return 1;
				}
			}
		}
        return 0;
	}

	int TryReadErrorBokz(ifstream &finp, struct ERR_BOKZ &tmi) {

        int indexParam;
		string line, word = "";

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if ( (word == "“¡ÿ¬") || (word == "T¡ÿB") ) {
				getline(finp, line, '\n');
				tmi.timeBoard= StringToDayTime(line);
			}
			else if ( (word == "—◊¬€ À1") || (word == "—◊¬€ À2") ) {
				finp >> tmi.clkPowerOff;
			}
			else if ( (word == "—◊ÿ“1") || (word == "—◊ÿ“2") ) {
				finp >> tmi.clkError;
			}
			else if (word == "“Õ ¬Œ–") {
				getline(finp, line, '\n');
				tmi.timeFirst = StringToDayTime(line);
			}
			else if (word == "Õ ¬Œ–0") {
				finp >> tmi.Qfirst[0];
			}
			else if (word == "Õ ¬Œ–1") {
				finp >> tmi.Qfirst[1];
			}
			else if (word == "Õ ¬Œ–2") {
				finp >> tmi.Qfirst[2];
			}
			else if (word == "Õ ¬Œ–3") {
				finp >> tmi.Qfirst[3];
//				return 1;
			}
			else if (word == "“ ¬Œ–") {
				getline(finp, line, '\n');
				tmi.timeLast = StringToDayTime(line);
			}
			else if (word == " ¬Œ–0") {
				finp >> tmi.Qlast[0];
			}
			else if (word == " ¬Œ–1") {
				finp >> tmi.Qlast[1];
			}
			else if (word == " ¬Œ–2") {
				finp >> tmi.Qlast[2];
			}
			else if (word == " ¬Œ–3") {
				finp >> tmi.Qlast[3];
				return 1;
			}
		}
        return 0;
	}

	void ClearQuat(struct QBOKZ &tmi) {
		tmi.timeBokz1 = 0;
		tmi.timeBokz2 = 0;
		for (int i = 0; i < 4; i++) {
			tmi.quatBokz1[i] = 0;
			tmi.quatBokz2[i] = 0;
		}
		for (int i = 0; i < 3; i++) {
			tmi.OZ1[i] = 0;
			tmi.OZ2[i] = 0;
		}
	}

	int TryReadQuat(ifstream &finp, struct QBOKZ &tmi) {

        int indexParam;
		string line, word = "";

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if (word == "“1") {
				finp >> word;
				if (word == " ¬") {
					finp >> word;
					if (word == "Œ–") {
						getline(finp, line, '\n');
						tmi.timeBokz1 = StringToDayTime(line);
					}
				}
			}
			else if (word == " ¬1") {
				finp >> word;
				if (word == "Œ–") {
					finp >> indexParam;
					if ((indexParam >= 0) && (indexParam < 4)) {
						finp >> tmi.quatBokz1[indexParam];
					}
				}
			}
			else if (word == "«≈“1") {
				finp >> word;
				if ((word == "X") || (word == "’") ) finp >> tmi.OZ1[0];
				else if (word == "Y")  finp >> tmi.OZ1[1];
				else if	(word == "Z")  finp >> tmi.OZ1[2];
			}
			else if (word == "“2") {
				finp >> word;
				if (word == " ¬") {
					finp >> word;
					if (word == "Œ–") {
						getline(finp, line, '\n');
						tmi.timeBokz2 = StringToDayTime(line);
					}
				}
			}
			else if (word == " ¬2") {
				finp >> word;
				if (word == "Œ–") {
					finp >> indexParam;
					if ((indexParam >= 0) && (indexParam < 4)) {
						finp >> tmi.quatBokz2[indexParam];
					}
				}
			}
			else if (word == "«≈“2") {
				finp >> word;
				if ((word == "X") || (word == "’") ) finp >> tmi.OZ2[0];
				else if (word == "Y")  finp >> tmi.OZ2[1];
				else if	(word == "Z")  {
					finp >> tmi.OZ2[2];
					return 1;
				}
			}
		}
        return 0;
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
		tmi.Reserved = 0;
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
				finp >> tmi.Reserved;
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

	void PrintLogGyro(ofstream &file, struct GYRO tmi, TDateTime curDate, bool &create) {
		if (!create) {
			file << "Date&Time\t";
			file << "Day/Time\t";
			file << "Wx_sat,''/c\t" << "Wy_sat,''/c\t" << "Wz_sat,''/c\t";
			file << "Wx_gyro,''/c\t" << "Wy_gyro,''/c\t" << "Wz_gyro,''/c\t";
			file << "Q_sat[0]\t" << "Q_sat[1]\t" << "Q_sat[2]\t" << "Q_sat[3]\t";
			file << "Q_gyro[0]\t" << "Q_gyro[1]\t" << "Q_gyro[2]\t" << "Q_gyro[3]\t";
			file << "\n";
            create = true;
		}

		file << OutputDateTime(curDate).c_str() << "\t";
		file << DayTimeToString(tmi.timeGyro).c_str() << "\t";
		file << dec << setfill(' ');
		file << std::setprecision(9);
		for (int i = 0; i < 3; i++) {
			file << tmi.omegaSat[i] << "\t";
		}
		for (int i = 0; i < 3; i++) {
			file << tmi.omegaGyro[i] << "\t";
		}
		for (int i = 0; i < 4; i++) {
			file << tmi.quatSat[i] << "\t";
		}
		for (int i = 0; i < 4; i++) {
			file << tmi.quatGyro[i] << "\t";
		}
//		double Qdbl[4], Mornt[3][3], ang[3] = {0, 0, 0};
//		for (int i = 0; i < 4; i++) {
//			 Qdbl[i] = (double)tmi.Qornt[i];
//		}
//		if ( !CheckQuatNorm(Qdbl, 0.001) ) {
//			QuatToMatrix(Qdbl, Mornt);
//			MatrixToEkvAngles(Mornt, ang);
//		}
//		for (int i = 0; i < 3; i++) {
//			file << ang[i] * RTD << "\t";
//		}
		file << "\n";
	}

	void PrintLogError(ofstream &file, struct ERR_BOKZ tmi, TDateTime curDate, bool &create) {
		if (!create) {
			file << "Date&Time\t";
			file << "Tfind\t";
			file << "Qfind[0]\t" << "Qfind[1]\t" << "Qfind[2]\t" << "Qfind[3]\t";
			file << "Tlost\t";
			file << "Qlost[0]\t" << "Qlost[1]\t" << "Qlost[2]\t" << "Qlost[3]\t";
			file << "\n";
			create = true;
		}

		file << OutputDateTime(curDate).c_str() << "\t";
		file << DayTimeToString(tmi.timeBoard).c_str() << "\t";
		file << DayTimeToString(tmi.timeFirst).c_str() << "\t";
		file << dec << setfill(' ');
		for (int i = 0; i < 4; i++) {
			file << tmi.Qfirst[i] << "\t";
		}
		file << DayTimeToString(tmi.timeLast).c_str() << "\t";
		file << dec << setfill(' ');
		for (int i = 0; i < 4; i++) {
			file << tmi.Qlast[i] << "\t";
		}
		file << tmi.clkPowerOff << "\t";
        file << tmi.clkError << "\t";
		file << "\n";
	}


	void PrintLogQuat(ofstream &file, struct QBOKZ tmi, TDateTime curDate, bool &create) {
		if (!create) {
			file << "Date&Time\t";
			file << "Day/Time-1\t";
			file << "Day/Time-2\t";
			file << "Qbokz-1[0]\t" << "Qbokz-1[1]\t" << "Qbokz-1[2]\t" << "Qbokz-1[3]\t";
			file << "Qbokz-2[0]\t" << "Qbokz-2[1]\t" << "Qbokz-2[2]\t" << "Qbokz-2[3]\t";
			file << "\n";
            create = true;
		}

		file << OutputDateTime(curDate).c_str() << "\t";
		file << DayTimeToString(tmi.timeBokz1).c_str() << "\t";
		file << DayTimeToString(tmi.timeBokz2).c_str() << "\t";
		file << dec << setfill(' ');
		for (int i = 0; i < 4; i++) {
			file << tmi.quatBokz1[i] << "\t";
		}
		for (int i = 0; i < 4; i++) {
			file << tmi.quatBokz2[i] << "\t";
		}
		for (int i = 0; i < 3; i++) {
			file << tmi.OZ1[i] << "\t";
		}
		for (int i = 0; i < 3; i++) {
			file << tmi.OZ2[i] << "\t";
		}
//		double Qdbl[4], Mornt[3][3], ang[3] = {0, 0, 0};
//		for (int i = 0; i < 4; i++) {
//			 Qdbl[i] = (double)tmi.Qornt[i];
//		}
//		if ( !CheckQuatNorm(Qdbl, 0.001) ) {
//			QuatToMatrix(Qdbl, Mornt);
//			MatrixToEkvAngles(Mornt, ang);
//		}
//		for (int i = 0; i < 3; i++) {
//			file << ang[i] * RTD << "\t";
//		}
		file << "\n";
	}

	void PrintMSHI(ofstream &file, struct MSHI tmi, TDateTime curDate) {
		file << OutputDateTime(curDate).c_str() << "\n";
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ Ãÿ»Œ–" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.timeBOKZ).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
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
		file << "ƒ‡Ú‡:\t";   PrintProgDate(file, tmi.Date, 2000); file << "\n";
		file << "¬ÂÒËˇ:\t"; PrintProgVersion(file, tmi.Version); file << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLogSHTMI1(ofstream &file, struct SHTMI1 tmi, bool &create) {
		if (!create) {
			file << "Day/Time\t";
			file << "KC1\t" << "KC2\t" << "POST\t" << "π\t" << "Texp\t";
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
		PrintProgDate(file, tmi.Date, 2000); file << "\t";
		PrintProgVersion(file, tmi.Version); file << "\t";
		file << "\n";
		file << flush;
	}

	void PrintSHTMI2(ofstream &file, struct SHTMI2 tmi, TDateTime curDate) {
		file << OutputDateTime(curDate).c_str() << "\n";
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

	void PrintLogSHTMI2(ofstream &file, struct SHTMI2 tmi, TDateTime curDate, bool &create) {
		if (!create) {
			file << "Date&Time\t";
			file << "Day/Time\t";
			file << "KC1\t" << "KC2\t" << "POST\t" << "π\t" << "Texp\t";
			file << "”—ƒ\t" << "ÕŒ\t" << "ÕŒ—À\t";
			file << "TŒ\t" << "TŒ—À\t" << "—À≈∆\t";
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
			if (CheckNewDtmiM60(tmi.serialNumber) && !isM1000) {
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
		file << "MaxHistX: \t" << (short)tmi.maxHistX << "\n";
		file << "MaxHistY: \t" << (short)tmi.maxHistY << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLOC(ofstream &file, TDateTime curDate, struct LOC tmi, int isM1000) {
		file << OutputDateTime(curDate).c_str() << "\n";
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

		stringSerial.sprintf("«‡‚. π %02d", tmi.serialNumber);
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

		stringSerial.sprintf("«‡‚. π %02d", tmi.serialNumber);
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

//-------------ÙÛÌÍˆËË ‰Îˇ ˜ÚÂÌËˇ ÔÓÚÓÍÓÎÓ‚ Ï¡Œ «-2–------------------------//
	void SwapMshi_2R(TMshi_2R *tmi)
	{
		Swap2Word((short*)&tmi->time_sec);
		for (int i = 0; i < 3; i++) {
			Swap2Word((short*)&tmi->OZ[i]);
			Swap2Word((short*)&tmi->W[i]);
		}
		for (int i = 0; i < 4; i++) {
			Swap2Word((short*)&tmi->Qornt[i]);
		}
	}

	void SwapShtmi1_2R(TShtmi1_2R *tmi)
	{
		Swap2Word((short*)&tmi->time_sec);
		Swap2Word((short*)&tmi->Foc);
		Swap2Word((short*)&tmi->Xg);
		Swap2Word((short*)&tmi->Yg);
	}

	void SwapShtmi2_2R(TShtmi2_2R *tmi)
	{
		Swap2Word((short*)&tmi->time_sec);
		Swap2Word((short*)&tmi->cntSLEZH);
	}

	void SwapDtmi_2R(TDtmi_2R *tmi)
	{
		Swap2Word((short*)&tmi->time_sec);
		Swap2Word((short*)&tmi->timeLast_sec);
		Swap2Word((short*)&tmi->era);

		for (int i = 0; i < 3; i++) {
			Swap2Word((short*)&tmi->Wop[i]);
			Swap2Word((short*)&tmi->Vline[i]);
		}

		for (int i = 0; i < 4; i++) {
			Swap2Word((short*)&tmi->Qapr[i]);
			Swap2Word((short*)&tmi->Qlast[i]);
		}

		for (int i = 0; i < 15; i++) {
			for (int k = 0; k < 2; k++) {
				Swap2Word((short*)&tmi->RsLocT[k][i].x);
				Swap2Word((short*)&tmi->RsLocT[k][i].y);
				Swap2Word((short*)&tmi->RsLocT[k][i].bright);
			}
		}
	}

	void SwapMloc_2R(TMloc_2R *tmi)
	{
		Swap2Word((short*)&tmi->time_sec);
		Swap2Word((short*)&tmi->sigma);
		for (int i = 0; i < 36; i++) {
			Swap2Word((short*)&tmi->RsLoc[i].x);
			Swap2Word((short*)&tmi->RsLoc[i].y);
			Swap2Word((short*)&tmi->RsLoc[i].bright);
		}

	}

	void PrintLogMshi_2R_old(ofstream &file, TMshi_2R_old *tmi, TDateTime curDate, bool &create)
	{
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
			file << "\n";
			create = true;
		}

		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
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
		file << tmi->ThMax  << "\t" << tmi->mxy/(1e+5) * 1000. << "\t"; //(1e+4)
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
		file << "\n" << flush;
	}

	void PrintLogMshi_2R(ofstream &file, TMshi_2R *tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Date&Time\t";
			file << "Tbshv\t";
			file << "Q[0]\t" << "Q[1]\t" << "Q[2]\t" << "Q[3]\t";
			file << "Serial\t" << "KC1\t" << "KC2\t";
			file << "NumFrag\t" << "NumLoc\t" ;
			file << "NumDet\t" << "LightMv5\t";
			file << "Mean\t" << "ThMax\t" << "Mxy,mkm\t" << "Tcmv\t";
			file << "OZ[0]\t" << "OZ[1]\t" << "OZ[2]\t";
			file << "Wx,''/c\t" << "Wy,''/c\t" << "Wz,''/c\t";
			file << "Al, deg\t" << "Dl, deg\t" << "Az, deg\t";
			file << "\n";
			create = true;
		}

		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
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

		file << (unsigned short)tmi->NumFrag << "\t";
		file << (unsigned short)tmi->NumLoc  << "\t";
		file << (unsigned short)tmi->NumDet << "\t";
		file << (unsigned short)tmi->LightMv5 * 50. << "\t";  //50???
		file << tmi->Mean << "\t";
		file << tmi->ThMax  << "\t" << tmi->mxy/(1e+5) * 1000. << "\t"; //(1e+4)
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
		file << "\n" << flush;
	}

	void PrintLogShtmi1_2R(ofstream &file, TShtmi1_2R *tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Date&Time\t";
			file << "Tbshv\t";
			file << "Serial\t" << "KC1\t" << "KC2\t" << "POST\t";
			file << "Foc\t" << "Xg\t" << "Yg\t" ;
			file << "Texp\t" << "Mean\t" << "Sigma\t";
			file << "Tcmv\t" << "StatKP\t" << "CntKP\t";
			file << "CatCS0\t"   << "CatCS1\t";
			file << "ConstCS0\t" << "ConstCS1\t";
			file << "ProgCS0\t"  << "ProgCS1\t";
			file << "Version\t";
			file << "\n";
			create = true;
		}

		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
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
		file << tmi->cntUPN << "\t" << tmi->optSerial << "\t" << tmi->optConstCS << "\t";
		file << tmi->CatCS[0]   << "\t" << tmi->CatCS[1]   << "\t";
		file << tmi->ConstCS[0] << "\t" << tmi->ConstCS[1] << "\t";
		file << tmi->ProgCS[0]  << "\t" << tmi->ProgCS[1]  << "\t";
		PrintProgVersion(file, tmi->Version); file << "\t";
		file << "\n" << flush;
	}

	void PrintLogShtmi2_2R(ofstream &file,  TShtmi2_2R *tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Date&Time\t";
			file << "Tbshv\t";
			file << "Serial\t" << "KC1\t" << "KC2\t" << "POST\t";
			file << "”—ƒ\t" << "ÕŒ\t" << "ÕŒ—À\t";
			file << "Texp\t" << "TŒ\t" << "TŒ—À\t" << "—À≈∆\t";
			for (int i = 0; i < MAX_STAT; i++) {
				file << "EC" << (i+1) << "\t";
			}
			file << "\n";
			create = true;
		}

		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
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
		file << "\n" << flush;
	}

	void PrintLogDtmi_2R_old(ofstream &file, TDtmi_2R_old tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t" << "POST\t";
			file << "Serial\t" << "Texp,ms\t"<< "Tcmv\t";
			file << "RegErr\t";// << "Sigma\t";
			file << "NumAll[0,1]\t" << "NumL[0,1]\t";
			file << "NumLoc[NumPix]\t";
			file << "NumStore\t" << "NumDet\t";
			file << "NumStar\t"  << "NumFrag\t";
			file << "deltaT,ms\n";
			create = true;
		}

		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec << "." << setfill('0');
		file << setw(3) << tmi.time_ms << "\t";

		file << uppercase << hex << setfill('0');
		file << setw(6) << tmi.serial << "\t";
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";

		file << dec << setfill(' ');
		file << setw(6) << tmi.timeExp << "\t";
		file << setw(6) << tmi.Tcmv/10. << "\t";

		file << tmi.cntRegErr << "\t";
		file << tmi.NumAll[0] << "\t" << tmi.NumAll[1] << "\t";
		file << (int)tmi.NumL[0] << "\t" << (int)tmi.NumL[1] << "\t";
		file << tmi.NumLoc << "\t";
		file << (int)tmi.NumStore << "\t" << (int)tmi.NumDet << "\t";
		file << (int)tmi.NumStar  << "\t" << (int)tmi.NumFrag << "\t";
        file << tmi.deltaT << "\t";
		file << "\n" << flush;
	}
	void PrintLogDtmi_2R(ofstream &file, TDtmi_2R tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t" << "POST\t";
			file << "Serial\t" << "Texp,ms\t"<< "Tcmv\t";
			file << "RegErr\t";
			file << "Mean\t" << "Sigma\t";
			file << "NumLoc[NumPix]\t";
			file << "NumStore\t" << "NumDet\t";
			file << "NumStar\t"  << "NumFrag\t";
			file << "deltaT,ms\n";
			create = true;
		}

		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec << "." << setfill('0');
		file << setw(3) << tmi.time_ms << "\t";

		file << uppercase << hex << setfill('0');
		file << setw(6) << tmi.serial << "\t";
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";

		file << dec << setfill(' ');
		file << setw(6) << tmi.timeExp << "\t";
		file << setw(6) << tmi.Tcmv/10. << "\t";

		file << tmi.cntRegErr << "\t";
		file << tmi.Mean << "\t" << tmi.Sigma/10. << "\t";
		file << (int)tmi.NumL[0] << "\t" << (int)tmi.NumL[1] << "\t";
		file << tmi.NumLoc << "\t";
		file << (int)tmi.NumStore << "\t" << (int)tmi.NumDet << "\t";
		file << (int)tmi.NumStar  << "\t" << (int)tmi.NumFrag << "\t";
        file << tmi.deltaT << "\t";
		file << "\n" << flush;
	}

	void PrintDtmi_2R_old(ofstream &file, const TDtmi_2R_old &tmi, TDateTime curDate, bool &create)
	{
		file << dec << setfill(' ');
		file << "Date&Time: \t" << OutputDateTime(curDate).c_str() << "\n";
		file << "Time: \t" << tmi.time_sec << "." << setfill('0');
		file << setw(3) << tmi.time_ms << "\n";

		file << uppercase << hex << setfill('0');
		file << "Serial: \t" << "0x" << setw(4) << tmi.serial << "\n";
		file << "KC1: \t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "KC2: \t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST: \t" << "0x" << setw(4) << tmi.post << "\n";

		file << dec << setfill(' ');
		file << "Tcmv: \t" << tmi.Tcmv/10. << "\n";
		file << "CntRegErr: \t" << tmi.cntRegErr << "\n";
		file << "Texp: \t" << tmi.timeExp << "\n";
		file << "NumFltPix: \t" << tmi.pixFilterCnt[0] << ", " << tmi.pixFilterCnt[1] << "\n";
		file << "NumCurPix: \t" << tmi.pixCurrent << "\n";
		file << "NumAll: \t" << tmi.NumAll[0] << ", " << tmi.NumAll[1] << "\n";
		file << "NumL: \t" << (int)tmi.NumL[0] << ", " << (int)tmi.NumL[1] << "\n";

		file << "NumLoc: \t" << tmi.NumLoc << "\n";
		file << "NumStore: \t" << (int)tmi.NumStore << "\n";
		file << "NumDet: \t" << (int)tmi.NumDet << "\n";
		file << "NumStar: \t" << (int)tmi.NumStar << "\n";
		file << "NumFrag: \t" << (int)tmi.NumFrag << "\n";

		file << "MaxH: \t" << tmi.MaxH << "\n";
		file << "dxdyMaxH: \t" << (int)tmi.dxMaxH << ", " << (int)tmi.dyMaxH << "\n";
		file << "NumSec: \t" << (int)tmi.numSec << "\n";
		file << "Eps: \t" << (int)tmi.eps << "\n";
		file << "deltaT: \t" << tmi.deltaT << "\n";
		file << "Mxy: \t" << tmi.m_cur/(1e+5) << "\n";  //(1e+4)

		file << "TimeLast: \t" << tmi.timeLast_sec << "." << setfill('0');
		file << setw(3) << tmi.timeLast_ms << "\n";
		file << "QuatLast: \t" << "{ " << tmi.Qlast[0] << ", "<< tmi.Qlast[1] << ", ";
		file << tmi.Qlast[2] << ", "<< tmi.Qlast[3] << " }\n";
		file << "QuatApr: \t" << "{ " << tmi.Qapr[0] << ", "<< tmi.Qapr[1] << ", ";
		file << tmi.Qapr[2] << ", "<< tmi.Qapr[3] << " }\n";
		file << "Wop: \t" << "{ " << tmi.Wop[0] << ", "<< tmi.Wop[1] << ", " << tmi.Wop[2] << " }\n";
		file << "Vline: \t" << "{ " << tmi.Vline[0] << ", "<< tmi.Vline[1] << ", " << tmi.Vline[2] << " }\n";
		file << "Era: \t" << tmi.era << "\n";
		file << "HistBright:\n";
		for (int i = 0; i < 16; i++)
			file << (i+1) << "\t" << (int)tmi.brightHist[i] << "\n";
		for (int k = 0; k < 2; k++) {
			file << "Frame π " << (k+1) << ":\n";
			file << "π\t" << "X, pix\t" << "Y, pix\t" << "B, EMP\t" << "S\n";
			for (int i = 0; i < 15; i++) {
				file << (i+1) << "\t" << tmi.RsLocT[k][i].x << "\t" << tmi.RsLocT[k][i].y << "\t";
				file << tmi.RsLocT[k][i].bright << "\t" << tmi.RsLocT[k][i].size << "\n";
			}
		}

		file << "π\t" << "X, pix\t" << "Y, pix\n";
		float cft = 1/pow(2., 5);
		for (int i = 0; i < 15; i++) {
			file << (i+1) << "\t" << tmi.XYc[i][0] * cft << "\t" << tmi.XYc[i][1] * cft << "\n";
		}

		file << "π\t" << "Mean, EMP\t" << "Sigma, EMP\t" << "Th, EMP\n";
		cft = 1/pow(2., 4);
		for (int i = 0; i < 15; i++) {
			file << (i+1) << "\t" << tmi.MeanFrag[i][0] * cft << "\t" << tmi.MeanFrag[i][1] * cft << "\t";
			file << tmi.ThFrag[i] << "\n";
		}

		file << "π\t" << "ObjFrag, EMP\t" << "FragXY\n";
		unsigned short fragX, fragY;
		for (int i = 0; i < 8; i++) {
			file << (i*2+1) << "\t" << (int)(tmi.ObjFrag[i] & 0x0F) << "\t";
			GetSizeFrag_2R( (tmi.MultXY[i] & 0x0F), &fragX, &fragY);
			file << fragX << "\t" << fragY << "\n";
			file << (i*2+2) << "\t" << (int)((tmi.ObjFrag[i] >> 4) & 0x0F) << "\t";
			GetSizeFrag_2R( ((tmi.MultXY[i] >> 4) & 0x0F), &fragX, &fragY);
			file << fragX << "\t" << fragY << "\n";
		}

		file << "π\t" << "ZipXY\n";
		for (int i = 0; i < 14; i++) {
			file << (i+1) << "\t" << (int)(tmi.ZipXY[i] & 0x0F);
			file << "\t" << (int)(tmi.ZipXY[i] >> 4) << "\n" ;
		}
		file << "\n";
		file << flush;
	}

	void PrintDtmi_2R(ofstream &file, const TDtmi_2R &tmi, TDateTime curDate, bool &create)
	{
		file << dec << setfill(' ');
		file << "Date&Time: \t" << OutputDateTime(curDate).c_str() << "\n";
		file << "Time: \t" << tmi.time_sec << "." << setfill('0');
		file << setw(3) << tmi.time_ms << "\n";

		file << uppercase << hex << setfill('0');
		file << "Serial: \t" << "0x" << setw(4) << tmi.serial << "\n";
		file << "KC1: \t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "KC2: \t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST: \t" << "0x" << setw(4) << tmi.post << "\n";

		file << dec << setfill(' ');
		file << "Tcmv: \t" << tmi.Tcmv/10. << "\n";
		file << "CntRegErr: \t" << tmi.cntRegErr << "\n";
		file << "Texp: \t" << tmi.timeExp << "\n";
		file << "NumFltPix: \t" << tmi.pixFilterCnt[0] << ", " << tmi.pixFilterCnt[1] << "\n";
		file << "NumCurPix: \t" << tmi.pixCurrent << "\n";
		file << "Mean: \t" << tmi.Mean << "\n";
		file << "Sigma: \t" << tmi.Sigma/10. << "\n";
		file << "NumL: \t" << (int)tmi.NumL[0] << ", " << (int)tmi.NumL[1] << "\n";

		file << "NumLoc: \t" << tmi.NumLoc << "\n";
		file << "NumStore: \t" << (int)tmi.NumStore << "\n";
		file << "NumDet: \t" << (int)tmi.NumDet << "\n";
		file << "NumStar: \t" << (int)tmi.NumStar << "\n";
		file << "NumFrag: \t" << (int)tmi.NumFrag << "\n";

		file << "MaxH: \t" << tmi.MaxH << "\n";
		file << "dxdyMaxH: \t" << (int)tmi.dxMaxH << ", " << (int)tmi.dyMaxH << "\n";
		file << "NumSec: \t" << (int)tmi.numSec << "\n";
		file << "Eps: \t" << (int)tmi.eps << "\n";
		file << "deltaT: \t" << tmi.deltaT << "\n";
		file << "Mxy: \t" << tmi.m_cur/(1e+5) << "\n";  //(1e+4)

		file << "TimeLast: \t" << tmi.timeLast_sec << "." << setfill('0');
		file << setw(3) << tmi.timeLast_ms << "\n";
		file << "QuatLast: \t" << "{ " << tmi.Qlast[0] << ", "<< tmi.Qlast[1] << ", ";
		file << tmi.Qlast[2] << ", "<< tmi.Qlast[3] << " }\n";
		file << "QuatApr: \t" << "{ " << tmi.Qapr[0] << ", "<< tmi.Qapr[1] << ", ";
		file << tmi.Qapr[2] << ", "<< tmi.Qapr[3] << " }\n";
		file << "Wop: \t" << "{ " << tmi.Wop[0] << ", "<< tmi.Wop[1] << ", " << tmi.Wop[2] << " }\n";
		file << "Vline: \t" << "{ " << tmi.Vline[0] << ", "<< tmi.Vline[1] << ", " << tmi.Vline[2] << " }\n";
		file << "Era: \t" << tmi.era << "\n";
		file << "HistBright:\n";
		for (int i = 0; i < 16; i++)
			file << (i+1) << "\t" << (int)tmi.brightHist[i] << "\n";
		for (int k = 0; k < 2; k++) {
			file << "Frame π " << (k+1) << ":\n";
			file << "π\t" << "X, pix\t" << "Y, pix\t" << "B, EMP\t" << "S\n";
			for (int i = 0; i < 15; i++) {
				file << (i+1) << "\t" << tmi.RsLocT[k][i].x << "\t" << tmi.RsLocT[k][i].y << "\t";
				file << tmi.RsLocT[k][i].bright << "\t" << tmi.RsLocT[k][i].size << "\n";
			}
		}

		file << "π\t" << "X, pix\t" << "Y, pix\n";
		float cft = 1/pow(2., 5);
		for (int i = 0; i < 15; i++) {
			file << (i+1) << "\t" << tmi.XYc[i][0] * cft << "\t" << tmi.XYc[i][1] * cft << "\n";
		}

		file << "π\t" << "Mean, EMP\t" << "Sigma, EMP\t" << "Th, EMP\n";
		cft = 1/pow(2., 4);
		for (int i = 0; i < 15; i++) {
			file << (i+1) << "\t" << tmi.MeanFrag[i][0] * cft << "\t" << tmi.MeanFrag[i][1] * cft << "\t";
			file << tmi.ThFrag[i] << "\n";
		}

		file << "π\t" << "ObjFrag, EMP\t" << "FragXY\n";
		unsigned short fragX, fragY;
		for (int i = 0; i < 8; i++) {
			file << (i*2+1) << "\t" << (int)(tmi.ObjFrag[i] & 0x0F) << "\t";
			GetSizeFrag_2R( (tmi.MultXY[i] & 0x0F), &fragX, &fragY);
			file << fragX << "\t" << fragY << "\n";
			file << (i*2+2) << "\t" << (int)((tmi.ObjFrag[i] >> 4) & 0x0F) << "\t";
			GetSizeFrag_2R( ((tmi.MultXY[i] >> 4) & 0x0F), &fragX, &fragY);
			file << fragX << "\t" << fragY << "\n";
		}

		file << "π\t" << "ZipXY\n";
		for (int i = 0; i < 14; i++) {
			file << (i+1) << "\t" << (int)(tmi.ZipXY[i] & 0x0F);
			file << "\t" << (int)(tmi.ZipXY[i] >> 4) << "\n" ;
		}
		file << "\n";
		file << flush;
	}
	void PrintLogMloc_2R(ofstream &file, TMloc_2R tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "Serial\t" << "KC1\t" << "KC2\t";
			file <<  "Texp\t" << "NumPix\t";
			file << "Sigma\t" << "Th\t";
			file << "NumLoc\t" << "NumObj\t";
			file << "\n";
			create = true;
		}
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec << "." << setfill('0');
		file << setw(3) << tmi.time_ms << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.serial << "\t";
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";

		file << dec << setfill(' ');
		file << setw(6) << tmi.timeExp << "\t" << tmi.NumPix << "\t";
		file << tmi.sigma << "\t" << tmi.Th << "\t" ;
		file << setw(6) << tmi.NumLoc << "\t";
		file << setw(6) << tmi.NumObj << "\t";
		file << "\n" << flush;
	}

	void PrintMloc_2R(ofstream &file, TMloc_2R tmi, TDateTime curDate)
	{
		file << dec << setfill(' ');
        file << "Ã‡ÒÒË‚ ÃÀŒ :\n";
		file << "Date&Time: \t" << OutputDateTime(curDate).c_str() << "\n";
		file << "Time: \t" << tmi.time_sec << "." << setfill('0');
		file << setw(3) << tmi.time_ms << "\n";

		file << uppercase << hex << setfill('0');
		file << "Serial: \t" << "0x" << setw(4) << tmi.serial << "\n";
		file << "KC1: \t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "KC2: \t" << "0x" << setw(4) << tmi.status2 << "\n";

		file << dec << setfill(' ');
		file << "Texp: \t" << tmi.timeExp << "\n";
		file << "NumPix: \t" << tmi.NumPix << "\n";
		file << "Sigma: \t" << tmi.sigma << "\n";
		file << "Th: \t" << tmi.Th << "\n";
		file << "NumLoc: \t" << tmi.NumLoc << "\n";
		file << "NumObj: \t" << tmi.NumObj << "\n";
		file << "π\t" << "X, pix\t" << "Y, pix\t" << "B, EMP\t" << "S\n";
		for (int i = 0; i < 32; i++) {
			file << (i+1) << "\t" << tmi.RsLoc[i].x << "\t" << tmi.RsLoc[i].y << "\t";
			file << tmi.RsLoc[i].bright << "\t" << tmi.RsLoc[i].size << "\n";
		}
		file << "\n" << flush;
	}

//void PrintDtmi_MR(ofstream &file, TDtmi_MR tmi, TDateTime curDate)
//{
////		if (!create) create = true;
//
//		file << dec << setfill(' ');
//		file << "Date&Time: \t" << AnsiString(DateTimeToStr(curDate)).c_str() << "\n";
//		file << "Time: \t" << tmi.time_sec << "\n";
//
//		file << uppercase << hex << setfill('0');
//		file << "KC1: \t" << "0x" << setw(4) << tmi.status1 << "\n";
//		file << "KC2: \t" << "0x" << setw(4) << tmi.status2 << "\n";
//		file << "POST: \t" << "0x" << setw(4) << tmi.post << "\n";
//
//		file << dec << setfill(' ');
//		file << "Serial: \t" << tmi.serial << "\n";
//		file << "Tcmv: \t" << tmi.Tcmv/10. << "\n";
//		file << "Mean: \t" << tmi.Mean << "\n";
////        file << "Sigma: \t" << tmi.Sigma << "\n";
//		file << "Texp: \t" << tmi.timeExp << "\n";
//		file << "NumAll: \t" << (int)tmi.nLocalAll[0] << ", " << (int)tmi.nLocalAll[1] << "\n";
//		file << "NumObj: \t" << (int)tmi.nLocalObj[0] << ", " << (int)tmi.nLocalObj[1] << "\n";
//		file << "NumLoc: \t" << tmi.NumLoc << "\n";
//		file << "NumStore: \t" << (int)tmi.NumStore << "\n";
//		file << "NumDet: \t" << (int)tmi.NumDet << "\n";
//		file << "NumStar: \t" << (int)tmi.NumStar << "\n";
//		file << "NumFrag: \t" << (int)tmi.NumFrag << "\n";
//
//		file << "MaxH: \t" << tmi.MaxH << "\n";
//		file << "dxdyMaxH: \t" << (int)tmi.dxMaxH << ", " << (int)tmi.dyMaxH << "\n";
//		file << "NumSec: \t" << (int)tmi.numSec << "\n";
//		file << "Eps: \t" << (int)tmi.eps << "\n";
//		file << "deltaT: \t" << tmi.deltaT << "\n";
//		file << "Mxy: \t" << tmi.m_cur/(1e+4) << "\n";
//
//		file << "TimeLast: \t" << tmi.timeLast_sec << "\n";
//		file << "QuatLast: \t" << "{ " << tmi.Qlast[0] << ", "<< tmi.Qlast[1] << ", ";
//		file << tmi.Qlast[2] << ", "<< tmi.Qlast[3] << " }\n";
//		file << "QuatApr: \t" << "{ " << tmi.Qapr[0] << ", "<< tmi.Qapr[1] << ", ";
//		file << tmi.Qapr[2] << ", "<< tmi.Qapr[3] << " }\n";
//		file << "Wop: \t" << "{ " << tmi.Wop[0] << ", "<< tmi.Wop[1] << ", " << tmi.Wop[2] << " }\n";
//		file << "Vline: \t" << "{ " << tmi.Vline[0] << ", "<< tmi.Vline[1] << ", " << tmi.Vline[2] << " }\n";
//		file << "Era: \t" << tmi.era << "\n";
//		file << "HistBright:\n";
//		for (int i = 0; i <16; i++)
//			file << (i+1) << "\t" << (int)tmi.brightHist[i] << "\n";
//
//		for (int k = 0; k < 2; k++) {
//			file << "Frame π " << (k+1) << ":\n";
//			file << "π\t" << "X, pix\t" << "Y, pix\t" << "B, EMP\t" << "S\n";
//			for (int i = 0; i < 15; i++) {
//				file << (i+1) << "\t" << tmi.RsLoc[k][i][0] << "\t" << tmi.RsLoc[k][i][1] << "\t";
//				file << tmi.RsLoc[k][i][2] << "\t" << tmi.RsLocSize[k][i] << "\n";
//			}
//		}
//
//		file << "π\t" << "X, pix\t" << "Y, pix\n";
//		float cft = 1/pow(2., 5);
//		for (int i = 0; i < 15; i++) {
//			file << (i+1) << "\t" << tmi.XYc[i][0] * cft << "\t" << tmi.XYc[i][1] * cft << "\n";
//		}
//
//		file << "π\t" << "Mean, EMP\t" << "Sigma, EMP\t" << "Th, EMP\n";
//		cft = 1/pow(2., 4);
//		for (int i = 0; i < 15; i++) {
//			file << (i+1) << "\t" << tmi.MeanFrag[i][0] * cft << "\t" << tmi.MeanFrag[i][1] * cft << "\t";
//			file << tmi.ThFrag[i] << "\n";
//		}
//
//		file << "π\t" << "ObjFrag, EMP\n";
//		for (int i = 0; i < 8; i++) {
//			file << (i*2+1) << "\t" << (int)(tmi.ObjFrag[i] & 0x0F) << "\n";
//			file << (i*2+2) << "\t" << (int)((tmi.ObjFrag[i] >> 4) & 0x0F) << "\n";
//		}
//		file << "\n";
//		file << flush;
//}


//-------------ÙÛÌÍˆËË ‰Îˇ ˜ÚÂÌËˇ ÔÓÚÓÍÓÎÓ‚ ¡Œ «-Ã–------------------------//

	void PrintLogShtmi1_MR(ofstream &file, TShtmi1_MR tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t" << "POST\t";
			file << "Serial\t" << "Texp\t";
			file << "Foc\t" << "Xg\t" << "Yg\t";
			file << "Mean\t" << "Sigma\t";
			file << "StatKP\t" << "CntKP\t" << "CntUPN\t";
			file << "Tcmv\t" << "T1\t" << "T2\t" << "T3\t" << "T4\t";
			file << "CatCS0\t"   << "CatCS1\t";
			file << "ConstCS0\t" << "ConstCS1\t";
			file << "ProgCS0\t"  << "ProgCS1\t";
			file << "Version\t";
			file << "\n";
			create = true;
		}
		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec * 0.125 << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";

		file << dec << setfill(' ');
		file << tmi.serial << "\t" << tmi.timeExp << "\t";
		file << tmi.Foc << "\t" << tmi.Xg << "\t" << tmi.Yg << "\t";
		file << tmi.Mean << "\t" << tmi.Sigma << "\t";
		file << tmi.statKP << "\t" << tmi.cntKP << "\t" << tmi.cntUPN << "\t";
		file << tmi.Tcmv << "\t" << tmi.Tsens[0] << "\t" << tmi.Tsens[1] << "\t";
		file << tmi.Tsens[2] << "\t" << tmi.Tsens[3] << "\t";
		file << tmi.CatCS[0] << "\t" << tmi.CatCS[1] << "\t";
		file << tmi.ConstCS[0] << "\t" << tmi.ConstCS[1] << "\t";
		file << tmi.ProgCS[0] << "\t" << tmi.ProgCS[1] << "\t";
		PrintProgVersion(file, tmi.Version); file << "\t";
		file << "\n" << flush;
	}

	void PrintLogShtmi2_MR(ofstream &file, TShtmi2_MR tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t" << "POST\t";
			file << "Serial\t" << "Texp\t";
			file << "”—ƒ\t" << "ÕŒ\t" << "ÕŒ—À\t";
			file << "TŒ\t" << "TŒ—À\t" << "—À≈∆\t";
			for (int i = 0; i < 19; i++) {
				file << "EC" << (i+1) << "\t";
			}
			file << "\n";
			create = true;
		}
		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec * 0.125 << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";

		file << dec << setfill(' ');
		file << setw(6) << tmi.serial << "\t";
		file << setw(6) << tmi.timeExp << "\t";
		file << setw(6) << tmi.cntCommandWord << "\t";
		file << setw(6) << tmi.cntCallNO << "\t";
		file << setw(6) << tmi.cntNOtoSLEZH << "\t";
		file << setw(6) << tmi.cntCallTO << "\t";
		file << setw(6) << tmi.cntTOtoSLEZH << "\t";
		file << setw(8) << tmi.cntSLEZH << "\t";
		for (int i = 0; i < 12; i++) {
			file << tmi.cntStatOrient[i] << "\t";
		}
		file << (tmi.cntStatOrient[12] & 0x00FF) << "\t";
		file << (tmi.cntStatOrient[12] >> 8) << "\t";
		file << (tmi.cntStatOrient[13] & 0x00FF) << "\t";
		file << (tmi.cntStatOrient[13] >> 8) << "\t";
		file << tmi.cntStatOrient[14] << "\t";
		file << (tmi.cntStatOrient[15] & 0x00FF) << "\t";
		file << (tmi.cntStatOrient[15] >> 8) << "\t";
		file << "\n" << flush;
	}

	void PrintLogMshi_MR(ofstream &file, TMshi_MR tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "Q[0]\t" << "Q[1]\t" << "Q[2]\t" << "Q[3]\t";
			file << "Serial\t" << "KC1\t" << "KC2\t";
			file << "NumFrag\t" << "NumLoc\t";
			file << "NumDet\t" << "LightMv5\t";
            file << "Mean\t" << "Sigma\t";
			file << "ThMax\t" << "Mxy,mkm\t" << "Tcmv\t";
			file << "OZ[0]\t" << "OZ[1]\t" << "OZ[2]\t";
			file << "Wx,''/c\t" << "Wy,''/c\t" << "Wz,''/c\t";
			file << "Al, deg\t" << "Dl, deg\t" << "Az, deg\t";
			file << "\n";
			create = true;
		}
		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec * 0.125 << "\t";
		for (int i = 0; i < 4; i++) {
			file << tmi.Qornt[i] << "\t";
		}

		file << tmi.serial << "\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";

		file << dec << setfill(' ');
		file << (int)tmi.NumFrag << "\t" << (int)tmi.NumLoc << "\t";
		file << (int)tmi.NumDet << "\t" << (int)tmi.LigntMv5 << "\t";
		file << tmi.Mean << "\t" << tmi.Sigma*0.1 << "\t";
		file << tmi.ThMax  << "\t" << tmi.mxy/(1e+4) * 1000. << "\t";
		file << tmi.Tcmv/10. << "\t";

		for (int i = 0; i < 3; i++) {
			file << tmi.OZ[i] << "\t";
		}
		for (int i = 0; i < 3; i++) {
//			file << tmi.W[i] * RTS << "\t";     //only for test
			file << tmi.W[i] << "\t";
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
		file << "\n" << flush;
	}

	void PrintLogTmi_MR(ofstream &file, TTmi_MR tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t";
			file << "Serial\t" << "Texp\t" << "NumLoc\t" ;
			file << " LocXY\n";
			create = true;
		}
		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec * 0.125 << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";

		file << dec << setfill(' ');
		file << setw(6) << tmi.serial << "\t";
		file << setw(6) << (int)tmi.timeExp << "\t";
		file << setw(6) << (int)tmi.nLocalObj << "\t";

		for (int i = 0; i < 18; i++) {
			file << setw(6) << tmi.LocXY[i][0] << "\t";
			file << setw(6) << tmi.LocXY[i][1] << "\t";
		}
		file << "\n" << flush;
	}

	void PrintLogRtmi_MR(ofstream &file, TRtmi_MR tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t";
			file << "Serial\t" << "Texp\t" << "Tcmv\t";
			file << "T1\t" << "T2\t" << "T3\t" << "T4\t" << "Ip\t";
			file << "NumLoc\t" << " LocXY\n";
			create = true;
		}
		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec * 0.125 << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";

		file << dec << setfill(' ');
		file << tmi.serial << "\t";
		file << tmi.timeExp << "\t";
		file << tmi.Tcmv/10. << "\t";
		file << tmi.Tsens[0]/10. << "\t";
		file << tmi.Tsens[1]/10. << "\t";
		file << tmi.Tsens[2]/10. << "\t";
		file << tmi.Tsens[3]/10. << "\t";
        file << tmi.Ip << "\t";
		file << tmi.nLocalObj << "\t";

		for (int i = 0; i < 17; i++) {
			file << tmi.Local[i].x << "\t" << tmi.Local[i].y << "\t";
			file << tmi.Local[i].bright << "\t" << tmi.Local[i].size << "\t";
		}
		for (int i = 0; i < 17; i++) {
			file << tmi.ThFrag[i] << "\t";
        }
		file << "\n" << flush;
	}

	void PrintLogDtmi_MR(ofstream &file, TDtmi_MR tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t" << "POST\t";
			file << "Serial\t" << "Texp\t"<< "Tcmv\t";
			file << "Mean\t" << "Sigma\t";
			file << "nLocalObj[0,1]\t";
			file << "NumLoc[NumPix]\t";
			file << "NumStore\t" << "NumDet\t";
			file << "NumStar\t"  << "NumFrag\t";
			file << "MaxHist\t"  << "MaxDx\t" << "MaxDy\t";
			file << "\n";
			create = true;
		}
		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec * 0.125 << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";

		file << dec << setfill(' ');
		file << setw(6) << tmi.serial << "\t";
		file << setw(6) << tmi.timeExp << "\t";
		file << setw(6) << tmi.Tcmv/10. << "\t";

		file << tmi.Mean << "\t" << tmi.Sigma << "\t";
//		file << (int)tmi.nLocalAll[0] << "\t" << (int)tmi.nLocalAll[1] << "\t";
		file << (int)tmi.nLocalObj[0] << "\t" << (int)tmi.nLocalObj[1] << "\t";
		file << tmi.NumLoc << "\t";
		file << (int)tmi.NumStore << "\t" << (int)tmi.NumDet << "\t";
		file << (int)tmi.NumStar  << "\t" << (int)tmi.NumFrag << "\t";
		file << tmi.MaxH << "\t" << (int)tmi.dxMaxH << "\t" << (int)tmi.dxMaxH;
		file << "\n" << flush;
	}

	void PrintLogMloc_MR(ofstream &file, TMloc_MR tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t";
			file << "Serial\t" << "Texp\t";// << "Sigma\t";
			file << "nLocalObj\t" << "nFixedObj\t";
			file << "nPixFlt\t" << "nPixNoise\t" << "nPixXXX\t";
			file << "mean\t" << "sigma\t" << "Th\t";
			file << "\n";
			create = true;
		}
		file << dec << setfill(' ');
		file << OutputDateTime(curDate).c_str() << "\t";
		file << tmi.time_sec * 0.125 << "\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";

		file << dec << setfill(' ');
		file << setw(6) << tmi.serial << "\t";
		file << setw(6) << tmi.timeExp << "\t";
		file << setw(6) << tmi.nLocalObj << "\t";
		file << setw(6) << tmi.nFixedObj << "\t";
		file << tmi.nPixFlt << "\t";
		file << tmi.nPixNoise << "\t" << tmi.nPixXXX << "\t";
		file << tmi.mean << "\t" << tmi.sigma << "\t" << tmi.Th << "\t";
		file << "\n" << flush;
	}

void PrintDtmi_MR(ofstream &file, TDtmi_MR tmi, TDateTime curDate)
{
		file << dec << setfill(' ');
		file << "Date&Time: \t" << OutputDateTime(curDate).c_str() << "\n";
		file << "Time: \t" << tmi.time_sec * 0.125 << "\n";

		file << uppercase << hex << setfill('0');
		file << "KC1: \t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "KC2: \t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST: \t" << "0x" << setw(4) << tmi.post << "\n";

		file << dec << setfill(' ');
		file << "Serial: \t" << tmi.serial << "\n";
		file << "Tcmv: \t" << tmi.Tcmv/10. << "\n";
		file << "Mean: \t" << tmi.Mean << "\n";
        file << "Sigma: \t" << tmi.Sigma << "\n";
		file << "Texp: \t" << tmi.timeExp << "\n";
//		file << "NumAll: \t" << (int)tmi.nLocalAll[0] << ", " << (int)tmi.nLocalAll[1] << "\n";
		file << "NumObj: \t" << (int)tmi.nLocalObj[0] << ", " << (int)tmi.nLocalObj[1] << "\n";
		file << "NumLoc: \t" << tmi.NumLoc << "\n";
		file << "NumStore: \t" << (int)tmi.NumStore << "\n";
		file << "NumDet: \t" << (int)tmi.NumDet << "\n";
		file << "NumStar: \t" << (int)tmi.NumStar << "\n";
		file << "NumFrag: \t" << (int)tmi.NumFrag << "\n";

		file << "MaxH: \t" << tmi.MaxH << "\n";
		file << "dxdyMaxH: \t" << (int)tmi.dxMaxH << ", " << (int)tmi.dyMaxH << "\n";
		file << "NumSec: \t" << (int)tmi.numSec << "\n";
		file << "Eps: \t" << (int)tmi.eps << "\n";
		file << "deltaT: \t" << tmi.deltaT << "\n";
		file << "Mxy: \t" << tmi.m_cur/(1e+4) << "\n";

		file << "TimeLast: \t" << tmi.timeLast_sec << "\n";
		file << "QuatLast: \t" << "{ " << tmi.Qlast[0] << ", "<< tmi.Qlast[1] << ", ";
		file << tmi.Qlast[2] << ", "<< tmi.Qlast[3] << " }\n";
		file << "QuatApr: \t" << "{ " << tmi.Qapr[0] << ", "<< tmi.Qapr[1] << ", ";
		file << tmi.Qapr[2] << ", "<< tmi.Qapr[3] << " }\n";
		file << "Wop: \t" << "{ " << tmi.Wop[0] << ", "<< tmi.Wop[1] << ", " << tmi.Wop[2] << " }\n";
		file << "Vline: \t" << "{ " << tmi.Vline[0] << ", "<< tmi.Vline[1] << ", " << tmi.Vline[2] << " }\n";
		file << "Era: \t" << tmi.era << "\n";
		file << "HistBright:\n";
		for (int i = 0; i < 16; i++)
			file << (i+1) << "\t" << (int)tmi.brightHist[i] << "\n";

		for (int k = 0; k < 2; k++) {
			file << "Frame π " << (k+1) << ":\n";
			file << "π\t" << "X, pix\t" << "Y, pix\t" << "B, EMP\t" << "S\n";
			for (int i = 0; i < 15; i++) {
				file << (i+1) << "\t" << tmi.RsLoc[k][i][0] << "\t" << tmi.RsLoc[k][i][1] << "\t";
				file << tmi.RsLoc[k][i][2] << "\t" << tmi.RsLocSize[k][i] << "\n";
			}
		}

		file << "π\t" << "X, pix\t" << "Y, pix\n";
		float cft = 1/pow(2., 5);
		for (int i = 0; i < 15; i++) {
			file << (i+1) << "\t" << tmi.XYc[i][0] * cft << "\t" << tmi.XYc[i][1] * cft << "\n";
		}

		file << "π\t" << "Mean, EMP\t" << "Sigma, EMP\t" << "Th, EMP\n";
		cft = 1/pow(2., 4);
		for (int i = 0; i < 15; i++) {
			file << (i+1) << "\t" << tmi.MeanFrag[i][0] * cft << "\t" << tmi.MeanFrag[i][1] * cft << "\t";
			file << tmi.ThFrag[i] << "\n";
		}

//		file << "π\t" << "ObjFrag, EMP\n";
//		for (int i = 0; i < 8; i++) {
//			file << (i*2+1) << "\t" << (int)(tmi.ObjFrag[i] & 0x0F) << "\n";
//			file << (i*2+2) << "\t" << (int)((tmi.ObjFrag[i] >> 4) & 0x0F) << "\n";
//		}

		file << "π\t" << "ObjFrag, EMP\t" << "FragXY\n";
		unsigned short fragX, fragY;
		for (int i = 0; i < 8; i++) {
			file << (i*2+1) << "\t" << (int)(tmi.ObjFrag[i] & 0x0F) << "\t";
			GetSizeFrag_MR( (tmi.MultXY[i] & 0x0F), &fragX, &fragY);
			file << fragX << "\t" << fragY << "\n";
			file << (i*2+2) << "\t" << (int)((tmi.ObjFrag[i] >> 4) & 0x0F) << "\t";
			GetSizeFrag_MR( ((tmi.MultXY[i] >> 4) & 0x0F), &fragX, &fragY);
			file << fragX << "\t" << fragY << "\n";
		}

		file << "π\t" << "ZipXY\n";
		for (int i = 0; i < 14; i++) {
			file << (i+1) << "\t" << (int)(tmi.ZipXY[i] & 0x0F);
			file << "\t" << (int)(tmi.ZipXY[i] >> 4) << "\n" ;
		}
		file << "\n";
		file << flush;
}

void PrintMloc_MR(ofstream &file, TMloc_MR tmi, TDateTime curDate)
{
		file << dec << setfill(' ');
		file << "Date&Time: \t" << OutputDateTime(curDate).c_str() << "\n";
		file << "Time: \t" << tmi.time_sec * 0.125 << "\n";

		file << uppercase << hex << setfill('0');
		file << "KC1: \t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "KC2: \t" << "0x" << setw(4) << tmi.status2 << "\n";

		file << dec << setfill(' ');
		file << "Serial: \t" << tmi.serial << "\n";
		file << "Texp: \t" << tmi.timeExp << "\n";
		file << "NumLoc: \t" << tmi.nLocalObj << "\n";
		file << "NumFix: \t" << tmi.nFixedObj << "\n";
		file << "NumPixFlt: \t" << tmi.nPixFlt << "\n";
		file << "NumPixNoise: \t" << tmi.nPixNoise << "\n";
		file << "NumPixXXX: \t" << tmi.nPixXXX << "\n";
		file << "mean: \t" << tmi.mean << "\n";
		file << "sigma: \t" << tmi.sigma << "\n";
        file << "Th: \t" << tmi.Th << "\n";

		file << "π\t" << "X, pix\t" << "Y, pix\t" << "B, EMP\t" << "S\n";
		for (int i = 0; i < 36; i++) {
				file << (i+1) << "\t" << tmi.RsLoc[i][0] << "\t" << tmi.RsLoc[i][1] << "\t";
				file << tmi.RsLoc[i][2] << "\t" << tmi.RsLocSize[i] << "\n";
		}
		file << "\n";
		file << flush;
}

//-------------ÙÛÌÍˆËË ‰Îˇ ˜ÚÂÌËˇ ÔÓÚÓÍÓÎÓ‚ Ò ¿»—“-2ƒ------------------------//
	void ClearAistQuat(struct TAistQuat &tmi) {
		tmi.timeFirst = 0;
		tmi.timeLast = 0;
		for (int i = 0; i < 4; i++) {
			tmi.Qfirst[i] = 0;
			tmi.Qlast[i] = 0;
			tmi.Qssk[i] = 0;
		}
        tmi.clkPowerOff = 0;
	}

	int TryReadAistQuat(ifstream &finp, struct TAistQuat &tmi) {

		int indexParam;
		string line, word = "";

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if (word == "Õ“") {
				finp >> word;
				if (word == "œ–") {
					finp >> word;
					if (word == "»Õ‘") {
						getline(finp, line, '\n');
						tmi.timeFirst = StringToDayTime(line);
					}
				}
			}
			else if (word == "Õ") {
				finp >> word;
				if (word == " ¬") {
					finp >> word;
					if ((word == "Œ–")){
						finp >> indexParam;
						if ((indexParam >= 0) && (indexParam < 4)) {
							finp >> tmi.Qfirst[indexParam];
						}
					}
				}
			}
			else if (word == " “") {
				finp >> word;
				if (word == "œ–") {
					finp >> word;
					if (word == "»Õ‘") {
						getline(finp, line, '\n');
						tmi.timeLast = StringToDayTime(line);
					}
				}
			}
			else if (word == " ") {
				finp >> word;
				if (word == " ¬") {
					finp >> word;
					if ((word == "Œ–")) {
						finp >> indexParam;
						if ((indexParam >= 0) && (indexParam < 4)) {
							finp >> tmi.Qlast[indexParam];
						}
					}
				}
			}
			else if (word == " ¬A3N-1") {
				finp >> indexParam;
				if ((indexParam >= 0) && (indexParam < 4)) {
					finp >> tmi.Qssk[indexParam];
				}
			}
			else if (word == "—◊") {
				finp >> word;
				if ( (word == "¬€ À1") || (word == "¬€ À2")) {
					finp >> tmi.clkPowerOff;
                    return 1;
				}

            }
		}
		return 0;
	}

	void ClearAistMshi(struct TAistMshi &tmi) {
		tmi.status1 = 0;
		tmi.status2 = 0;
		tmi.time_sec = 0;

		for (int i = 0; i < 3; i++) {
			tmi.OZ[i] = 0;
			tmi.W[i] = 0;
		}
		for (int i = 0; i < 4; i++) {
			tmi.Qornt[i] = 0;
		}
		tmi.serial = 0;
		tmi.NumSec = 0;
		tmi.NumStar = 0;
		tmi.NumFrag = 0;
		tmi.NumLoc = 0;
		tmi.NumDet = 0;
		tmi.ThMax = 0;
        tmi.mxy = 0;
	}

	int TryReadAistMshi(ifstream &finp, struct TAistMshi &tmi) {
		string line, word = "";
		int indexParam;

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if (word == "“Ï¡Œ «") {
					getline(finp, line, '\n');
					tmi.time_sec = StringToDayTime(line);
			}
			else if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if (word == " ¬") {
				finp >> word;
				if (word == "Œ–") {
					finp >> indexParam;
					if ((indexParam >= 0) && (indexParam < 4)) {
						finp >> tmi.Qornt[indexParam];
					}
				}
			}
			else if (word == "”√") {
				finp >> word;
				if (word == "— ") {
					finp >> word;
					if (word == "X") finp >> tmi.W[0];
					else if (word == "Y") finp >> tmi.W[1];
					else if (word == "Z") finp >> tmi.W[2];
				}
			}
			else if (word == "NUMSEC") {
				finp >> tmi.NumSec;
			}
			else if (word == "NPROGFRAG") {
				finp >> tmi.NumStar;
			}
			else if (word == "NUMFRAG") {
				finp >> tmi.NumFrag;
			}
			else if (word == "NUMLOC") {
				finp >> tmi.NumLoc;
			}
			else if (word == "NUMDET") {
				finp >> tmi.NumDet;
			}
			else if (word == "SERNUM") {
				finp >> tmi.serial;
			}
			else if (word == "THMAX1") {
				finp >> tmi.ThMax;
			}
			else if (word == "M_CUR") {
				finp >> tmi.mxy;
				return 1;
			}
		}
        return 0;
	}

	void ClearAistShtmi1(struct TAistShtmi1 &tmi) {
		tmi.time_sec = 0;
		tmi.status1  = 0;
		tmi.status2  = 0;
		tmi.serial   = 0;
		tmi.post     = 0;
		tmi.timeExp  = 0;
		tmi.Foc = 0;
		tmi.Xg  = 0;
		tmi.Mean  = 0;
		tmi.Sigma = 0;
		tmi.ConstCS  = 0;
		tmi.ProgCS  = 0;
		tmi.X_ident = 0;
		tmi.Date = 0;
		tmi.Version = 0;
	}

	int TryReadAistShtmi1(ifstream &finp, struct TAistShtmi1 &tmi) {
		string line, word = "";

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if (word == "“_¡Œ «") {
					getline(finp, line, '\n');
					tmi.time_sec = StringToDayTime(line);
			}
			else if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if (word == "—EP_HOM") {
					finp >> tmi.serial;
			}
			else if ((word == "œŒ—“") || (word == "œOCT") || (word == "œO—T")) {
				finp >> word;
				tmi.post = ReadBinaryString(word);
			}
			else if (word == "“_› —œ") {
					finp >> tmi.timeExp;
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
			else if (word == " ŒÕ—“_ —") {
				finp >> tmi.ConstCS;
			}
			else if (word == "œ–Œ√_ —") {
				finp >> tmi.ProgCS;
			}
			else if (word == "’—‘")
				finp >> tmi.X_ident;
			else if (word == "ƒ¿“¿")
				finp >> tmi.Date;
			else if (word == "¬≈–—»ﬂ") {
				finp >> tmi.Version;
				return 1;
			}
		}
		return 0;
	}

	void ClearAistShtmi2(struct TAistShtmi2 &tmi) {
		tmi.time_sec = 0;
		tmi.status1  = 0;
		tmi.status2  = 0;
		tmi.post     = 0;
		tmi.serial = 0;
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

	int TryReadAistShtmi2(ifstream &finp, struct TAistShtmi2 &tmi) {
		string line, word = "";

		while ( (!finp.eof()) && (!StopReadArray(word)) ) {//(word.find("-----") == string::npos)) {
			finp >> word;
			if (word == "“_¡Œ «") {
					getline(finp, line, '\n');
					tmi.time_sec = StringToDayTime(line);
			}
			if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if (word == "—E–_ÕŒM") {
					finp >> tmi.serial;
			}
			else if ((word == "œŒ—“") || (word == "œOCT") || (word == "œO—T")) {
				finp >> word;
				tmi.post = ReadBinaryString(word);
			}
			else if (word == "“_› —œ") {
					finp >> tmi.timeExp;
			}
			else if (word == "Õ¿Ã_”—ƒ") {
					finp >> tmi.cntCommandWord;
			}
			else if (word == "—◊_ÕŒ") {
					finp >> tmi.cntCallNO;
			}
			else if (word == "◊»—À_ÕŒ") {
					finp >> tmi.cntNOtoSLEZH;
			}
			else if (word == "—◊_“Œ") {
					finp >> tmi.cntCallTO;
			}
			else if (word == "◊»—À_“Œ") {
					finp >> tmi.cntTOtoSLEZH;
			}
			else if (word == "◊»—À_—À≈∆") {
					finp >> tmi.cntSLEZH;
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
		}
		return 0;
	}

	void PrintLogAistQuat(ofstream &file, struct TAistQuat tmi, TDateTime curDate, bool &create) {
		if (!create) {
//			file << "Date&Time\t";
			file << "Day/Time-1\t";
			file << "Day/Time-2\t";
			file << "Qfirst[0]\t" << "Qfirst[1]\t" << "Qfirst[2]\t" << "Qfirst[3]\t";
			file << "Qlast[0]\t" << "Qlast[1]\t" << "Qlast[2]\t" << "Qlast[3]\t";
			file << "cntOff\n";
            create = true;
		}

//		file << OutputDateTime(curDate).c_str() << "\t";
		file << DayTimeToString(tmi.timeFirst).c_str() << "\t";
		file << DayTimeToString(tmi.timeLast).c_str() << "\t";
		file << dec << setfill(' ');
		for (int i = 0; i < 4; i++) {
			file << tmi.Qfirst[i] << "\t";
		}
		for (int i = 0; i < 4; i++) {
			file << tmi.Qlast[i] << "\t";
		}
//		double Qdbl[4], Mornt[3][3], ang[3] = {0, 0, 0};
//		for (int i = 0; i < 4; i++) {
//			 Qdbl[i] = (double)tmi.Qornt[i];
//		}
//		if ( !CheckQuatNorm(Qdbl, 0.001) ) {
//			QuatToMatrix(Qdbl, Mornt);
//			MatrixToEkvAngles(Mornt, ang);
//		}
//		for (int i = 0; i < 3; i++) {
//			file << ang[i] * RTD << "\t";
//		}
		file << "\n";
	}
	void PrintLogAistMshi(ofstream &file, struct TAistMshi tmi, TDateTime curDate, bool &create) {

		if (!create) {
			file << "Date&Time\t";
			file << "Tbshv\t";
			file << "Q[0]\t" << "Q[1]\t" << "Q[2]\t" << "Q[3]\t";
			file << "Serial\t" << "KC1\t" << "KC2\t";
			file << "NumStar\t" << "NumFrag\t";
			file << "NumLoc\t" << "NumDet\t";
			file << "ThMax\t" << "Mxy,mkm\t" << "NumSec\t";
			file << "OZ[0]\t" << "OZ[1]\t" << "OZ[2]\t";
			file << "Wx,''/c\t" << "Wy,''/c\t" << "Wz,''/c\t";
			file << "Al, deg\t" << "Dl, deg\t" << "Az, deg\n";
			create = true;
		}

		file << OutputDateTime(curDate).c_str() << "\t";
		file << DayTimeToString(tmi.time_sec).c_str()<<"\t";

		for (int i = 0; i < 4; i++) {
			file << tmi.Qornt[i] << "\t";
		}
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.serial << "\t";
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << dec << setfill(' ');

		file << (unsigned short)tmi.NumStar << "\t";
		file << (unsigned short)tmi.NumFrag << "\t";
		file << tmi.NumLoc << "\t" << tmi.NumDet << "\t";
		file << tmi.ThMax  << "\t" << tmi.mxy/(1e+4) * 1000. << "\t";
		file << tmi.NumSec << "\t";

		for (int i = 0; i < 3; i++) {
			file << tmi.OZ[i] << "\t";
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

	void PrintAistShtmi1(ofstream &file, struct TAistShtmi1 tmi) {
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ÿ“Ã»1" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.time_sec).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "«‡‚. π\t" << tmi.serial << "\n";
		file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
		file << "Foc, ÏÏ:\t" << tmi.Foc << "\n";
		file << "X0, ÏÏ:\t" << tmi.Xg << "\n";
		file << "Y0, ÏÏ:\t" << tmi.Yg << "\n";
		file << "—Â‰ÌÂÂ, Â.Ï..:\t" << tmi.Mean << "\n";
		file << "— Œ, Â.Ï..:\t" << tmi.Sigma << "\n";
		file << " — ÍÓÌÒÚ‡ÌÚ:\t" << tmi.ConstCS << "\n";
        file << " — ÔÓ„‡ÏÏ˚:\t" << tmi.ProgCS << "\n";
		file << "»‰ÂÌÚËÙËÍ‡ÚÓ:\t" << tmi.X_ident << "\n";
		file << "ƒ‡Ú‡:\t" << tmi.Date << "\n";
		file << "¬ÂÒËˇ:\t" << tmi.Version << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLogAistShtmi1(ofstream &file, struct TAistShtmi1 tmi, bool &create) {
		if (!create) {
			file << "Day/Time\t";
			file << "KC1\t" << "KC2\t" << "POST\t" << "π\t" << "Texp\t";
			file << "Foc\t" << "Xg\t" << "Yg\t";
			file << "Mean\t" << "Sigma\t";
			file << "ConstCS\t" << "ProgCS\t" << "X-ident\t";
			file << "Date\t" << "Version\t";
			file << "\n";
			create = true;
		}
		file << DayTimeToString(tmi.time_sec).c_str() << "\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";
		file << dec << setfill(' ');
		file << tmi.serial << "\t";
		file << tmi.timeExp << "\t";
		file << tmi.Foc << "\t";
		file << tmi.Xg << "\t";
		file << tmi.Yg << "\t";
		file << tmi.Mean << "\t";
		file << tmi.Sigma << "\t";
		file << tmi.ConstCS << "\t";
        file << tmi.ProgCS << "\t";
		file << tmi.X_ident << "\t";
		file << tmi.Date << "\t";
		file << tmi.Version << "\t";
		file << "\n";
		file << flush;
	}

	void PrintAistShtmi2(ofstream &file, struct TAistShtmi2 tmi, TDateTime curDate) {
		file << OutputDateTime(curDate).c_str() << "\n";
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ÿ“Ã»2" << "\n";
		file << "Tpr\t" << DayTimeToString(tmi.time_sec).c_str() << "\n";
		file << uppercase << hex << setfill('0');
		file << " —1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST:\t" << "0x" << setw(4) << tmi.post << "\n";
		file << dec << setfill(' ');
		file << "«‡‚. π\t" << tmi.serial << "\n";
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

	void PrintLogAistShtmi2(ofstream &file, struct TAistShtmi2 tmi, TDateTime curDate, bool &create) {
		if (!create) {
			file << "Date&Time\t";
			file << "Day/Time\t";
			file << "KC1\t" << "KC2\t" << "POST\t" << "π\t" << "Texp\t";
			file << "”—ƒ\t" << "ÕŒ\t" << "ÕŒ—À\t";
			file << "TŒ\t" << "TŒ—À\t" << "—À≈∆\t";
			for (int i = 0; i < MAX_STAT; i++) {
				file << "EC" << (i+1) << "\t";
			}
			file << "\n";
			create = true;
		}
		file << OutputDateTime(curDate).c_str() << "\t";
		file << DayTimeToString(tmi.time_sec).c_str()<<"\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";
		file << dec << setfill(' ');
		file << setw(6) << tmi.serial << "\t";
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

//-------------ÙÛÌÍˆËË ‰Îˇ ˜ÚÂÌËˇ ÔÓÚÓÍÓÎÓ‚ ›Ã »-----------------------------//
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
		file << flush;
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
			file << "Tcmv\t" << "StatKP\t" << "CntKP\t";
			file << "CatCS0\t"   << "CatCS1\t";
			file << "ConstCS0\t" << "ConstCS1\t";
			file << "ProgCS0\t"  << "ProgCS1\t";
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
		file << tmi->CatCS[0]   << "\t" << tmi->CatCS[1]   << "\t";
		file << tmi->ConstCS[0] << "\t" << tmi->ConstCS[1] << "\t";
		file << tmi->ProgCS[0]  << "\t" << tmi->ProgCS[1]  << "\t";
		file << tmi->Version << "\t";
		file << (unsigned short) pack->statTmi << "\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << pack->crcBoard << "\t";
		file << "0x" << setw(4) << pack->crcCalc  << "\n";
		file << flush;
	}

	void PrintShtmi2_2V(ofstream &file,  struct TBoardArray *pack, TDateTime curDate, bool &create)
	{
		struct TShtmi2_2V *tmi;
		if (!create) {
			file << "Date&Time\t";
			file << "Tbshv\t";
			file << "Serial\t" << "KC1\t" << "KC2\t" << "POST\t";
			file << "”—ƒ\t" << "ÕŒ\t" << "ÕŒ—À\t";
			file << "Texp\t" << "TŒ\t" << "TŒ—À\t" << "—À≈∆\t";
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
		file << flush;
	}

//	void PrintDtmi_2V(ofstream &file, struct TDtmi_2V tmi, TDateTime curDate, bool &create)
//	{
//		if (!create) {
//			file << "Date&Time\t" << "Tbshv\t";
//			file << "Serial\t" << "KC1\t" << "KC2\t" << "POST\t";
//			file << "Tcmv\t" << "Texp\t";
//			file << "NumAll[0]\t" << "NumAll[1]\t";
//			file << "NumL[0]\t" << "NumL[1]\t";
//			file << "NumStore\t" << "NumDet\t";
//			file << "NumStar\t" << "NumFrag\t";
//			file << "MaxH" << "dxMaxH\t" << "dyMaxH\t";
//			file << "\n";
//            create = true;
//		}
//
////        file << OutputDateTime(curDate).c_str() << "\t";
//		file << AnsiString(DateTimeToStr(curDate)).c_str() << "\t";
//		file << tmi.time_sec << "." << setfill('0');
//		file << setw(3) << tmi.time_ms << "\t";
//
//		file << uppercase << hex << setfill('0');
//		file << "0x" << setw(4) << tmi.serial << "\t";
//		file << "0x" << setw(4) << tmi.status1 << "\t";
//		file << "0x" << setw(4) << tmi.status2 << "\t";
//		file << "0x" << setw(4) << tmi.post << "\t";
//
//		file << dec << setfill(' ');
//		file << tmi.Tcmv/10. << "\t" << tmi.timeExp << "\t";
//		file << tmi.NumAll[0] << "\t" <<tmi.NumAll[1] << "\t";
//		file << (unsigned short)tmi.NumL[0] << "\t";
//		file << (unsigned short)tmi.NumL[1] << "\t";
//		file << tmi.NumLoc << "\t" ;
//		file << (unsigned short)tmi.NumStore << "\t";
//		file << (unsigned short)tmi.NumDet << "\t";
//		file << (unsigned short)tmi.NumStar << "\t";
//		file << (unsigned short)tmi.NumFrag << "\t";
//		file << tmi.MaxH << "\t" ;
//		file << (short)tmi.dxMaxH << "\t";
//		file << (short)tmi.dyMaxH << "\n";
//	}

	void ClearDtmi2V(struct TDtmi_2V &tmi)
	{
		tmi.time_sec = 0;
		tmi.time_ms  = 0;
		tmi.status1 = 0;
		tmi.status2 = 0;
		tmi.serial  = 0;
		tmi.post  = 0;
		tmi.Tcmv  = 0;
		tmi.cntRegErr = 0;
		tmi.timeExp = 0;
		tmi.NumAll[0] = 0;
		tmi.NumAll[1] = 0;
		tmi.NumL[0] = 0;
		tmi.NumL[1] = 0;
		tmi.NumLoc  = 0;
		tmi.NumStore = 0;
		tmi.NumDet = 0;
		tmi.NumStar = 0;
		tmi.NumFrag = 0;
		tmi.MaxH = 0;
		tmi.dxMaxH = 0;
		tmi.dyMaxH = 0;
		tmi.numSec = 0;
		tmi.eps = 0;
		tmi.deltaT = 0;
		tmi.timeLast_sec = 0;
		tmi.timeLast_ms  = 0;
		for (int i = 0; i < 4; i++) {
			tmi.Qapr[i] = 0;
			tmi.Qlast[i] = 0;
		}
		for (int i = 0; i < 3; i++) {
			tmi.Wop[i] = 0;
			tmi.Vline[i] = 0;
		}

		tmi.m_cur = 0;
		tmi.era   = 0;
		tmi.pixFilterCnt[0]  = 0;  tmi.pixFilterCnt[1]  = 0;
		tmi.pixCurrent = 0;

		for (int i = 0; i < 15; i++) {
			for (int k = 0; k < 2; k++) {
				tmi.RsLocT[k][i].X = 0;
				tmi.RsLocT[k][i].Y = 0;
				tmi.RsLocT[k][i].B = 0;
				tmi.RsLocT[k][i].S = 0;
			}
			tmi.ThFrag[i] = 0;
			tmi.XYc[i][0] = 0;      tmi.XYc[i][1] = 0;
			tmi.MeanFrag[i][0] = 0; tmi.MeanFrag[i][1] = 0;
		}
		for (int i = 0; i < 8; i++) {
			tmi.ObjFrag[i] = 0;
			tmi.MultXY[i]  = 0;
		}
	}

	void PrintDtmi_2V(ofstream &file, struct TDtmi_2V tmi, TDateTime curDate, bool &create)
	{
		file << dec << setfill(' ');
		file << "Date&Time: \t" << AnsiString(DateTimeToStr(curDate)).c_str() << "\n";
		file << "Time: \t" << tmi.time_sec << "." << setfill('0');
		file << setw(3) << tmi.time_ms << "\n";

		file << uppercase << hex << setfill('0');
		file << "Serial: \t" << "0x" << setw(4) << tmi.serial << "\n";
		file << "KC1: \t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "KC2: \t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << "POST: \t" << "0x" << setw(4) << tmi.post << "\n";

		file << dec << setfill(' ');
		file << "Tcmv: \t" << tmi.Tcmv/10. << "\n";
		file << "CntRegErr: \t" << tmi.cntRegErr << "\n";
		file << "Texp: \t" << tmi.timeExp << "\n";
		file << "NumFltPix: \t" << tmi.pixFilterCnt[0] << ", " << tmi.pixFilterCnt[1] << "\n";
		file << "NumCurPix: \t" << tmi.pixCurrent << "\n";
		file << "NumAll: \t" << tmi.NumAll[0] << ", " << tmi.NumAll[1] << "\n";
		file << "NumL: \t" << (int)tmi.NumL[0] << ", " << (int)tmi.NumL[1] << "\n";

		file << "NumLoc: \t" << tmi.NumLoc << "\n";
		file << "NumStore: \t" << (int)tmi.NumStore << "\n";
		file << "NumDet: \t" << (int)tmi.NumDet << "\n";
		file << "NumStar: \t" << (int)tmi.NumStar << "\n";
		file << "NumFrag: \t" << (int)tmi.NumFrag << "\n";

		file << "MaxH: \t" << tmi.MaxH << "\n";
		file << "dxdyMaxH: \t" << (int)tmi.dxMaxH << ", " << (int)tmi.dyMaxH << "\n";
		file << "NumSec: \t" << (int)tmi.numSec << "\n";
		file << "Eps: \t" << (int)tmi.eps << "\n";
		file << "deltaT: \t" << tmi.deltaT << "\n";
		file << "Era: \t" << tmi.era << "\n";
		file << "Mxy: \t" << tmi.m_cur/(1e+4) << "\n";

		file << "TimeLast: \t" << tmi.timeLast_sec << "." << setfill('0');
		file << setw(3) << tmi.timeLast_ms << "\n";
		file << "QuatLast: \t" << "{ " << tmi.Qlast[0] << ", "<< tmi.Qlast[1] << ", ";
		file << tmi.Qlast[2] << ", "<< tmi.Qlast[3] << " }\n";
		file << "QuatApr: \t" << "{ " << tmi.Qapr[0] << ", "<< tmi.Qapr[1] << ", ";
		file << tmi.Qapr[2] << ", "<< tmi.Qapr[3] << " }\n";
		file << "Wop: \t" << "{ " << tmi.Wop[0] << ", "<< tmi.Wop[1] << ", " << tmi.Wop[2] << " }\n";
		file << "Vline: \t" << "{ " << tmi.Vline[0] << ", "<< tmi.Vline[1] << ", " << tmi.Vline[2] << " }\n";

		for (int k = 0; k < 2; k++) {
			file << "Frame π " << (k+1) << ":\n";
			file << "π\t" << "X, pix\t" << "Y, pix\t" << "B, EMP\t" << "S\n";
			for (int i = 0; i < 15; i++) {
				file << (i+1) << "\t" << tmi.RsLocT[k][i].X << "\t" << tmi.RsLocT[k][i].Y << "\t";
				file << tmi.RsLocT[k][i].B << "\t" << tmi.RsLocT[k][i].S << "\n";
			}
		}

		file << "π\t" << "X, pix\t" << "Y, pix\n";
		float cft = 1/pow(2., 5);
		for (int i = 0; i < 15; i++) {
			file << (i+1) << "\t" << tmi.XYc[i][0] * cft << "\t" << tmi.XYc[i][1] * cft << "\n";
		}

		file << "π\t" << "Mean, EMP\t" << "Sigma, EMP\t" << "Th, EMP\n";
		cft = 1/pow(2., 4);
		for (int i = 0; i < 15; i++) {
			file << (i+1) << "\t" << tmi.MeanFrag[i][0] * cft << "\t" << tmi.MeanFrag[i][1] * cft << "\t";
			file << tmi.ThFrag[i] << "\n";
		}

		file << "π\t" << "ObjFrag, EMP\n";
		for (int i = 0; i < 8; i++) {
			file << (i*2+1) << "\t" << (int)(tmi.ObjFrag[i] & 0x0F) << "\n";
			file << (i*2+2) << "\t" << (int)((tmi.ObjFrag[i] >> 4) & 0x0F) << "\n";
		}
		file << "\n";
		file << flush;
	}

	void ClearMloc2V(struct TMloc_2V &tmi)
	{
		tmi.time_sec = 0;
		tmi.time_ms  = 0;
		tmi.status1 = 0;
		tmi.status2 = 0;
		tmi.serial  = 0;
		tmi.timeExp = 0;
		tmi.NumPix  = 0;
		tmi.NumLoc  = 0;
		tmi.NumObj  = 0;
		for (int i = 0; i < 32; i++) {
			for (int j = 0; j < 4; j++) {
				tmi.ArrLoc[i][j] = 0;
			}
		}
	}

	void PrintMloc_2V(ofstream &file, struct TMloc_2V tmi, TDateTime curDate, bool &create)
	{
		file << dec << setfill(' ');
		file << "Date&Time: \t" << AnsiString(DateTimeToStr(curDate)).c_str() << "\n";
		file << "Time: \t" << tmi.time_sec << "." << setfill('0');
		file << setw(3) << tmi.time_ms << "\n";

		file << uppercase << hex << setfill('0');
		file << "Serial: \t" << "0x" << setw(4) << tmi.serial << "\n";
		file << "KC1: \t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << "KC2: \t" << "0x" << setw(4) << tmi.status2 << "\n";

		file << dec << setfill(' ');
		file << "Texp: \t" << tmi.timeExp << "\n";
		file << "NumPix: \t" << tmi.NumPix << "\n";
		file << "NumLoc: \t" << tmi.NumLoc << "\n";
		file << "NumObj: \t" << tmi.NumObj << "\n";
		file << "π\t" << "X, pix\t" << "Y, pix\t" << "B, EMP\t" << "S\n";
		for (int i = 0; i < 32; i++) {
			file << (i+1) << "\t" << tmi.ArrLoc[i][0] << "\t" << tmi.ArrLoc[i][1] << "\t";
			file << tmi.ArrLoc[i][2] << "\t" << tmi.ArrLoc[i][3] << "\n";
		}
		file << "\n";
		file << flush;
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
	void PrintDtmiMeteor(ofstream &file, struct DTMI_METEOR tmi)
	{
		file << "____________________________________" << "\n";
		file << "Ã‡ÒÒË‚ ƒ“Ã»"<<"\n";
		file << "Tpr\t"<<tmi.timeBOKZ<<"\n";
		file << uppercase<<hex<<setfill('0');
		file << " —1\t" << "0x" << setw(4) << tmi.status1 << "\n";
		file << " —2\t" << "0x" << setw(4) << tmi.status2 << "\n";
		file << dec<<setfill(' ');
		file << "«‡‚. π\t" << tmi.serialNumber << "\n";
		file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
		file << "Foc, ÏÏ:\t" << tmi.Foc << "\n";
		file << "X0, ÏÏ:\t" << tmi.Xg << "\n";
		file << "Y0, ÏÏ:\t" << tmi.Yg << "\n";
		file << "NumLoc: \t" << tmi.nLocalObj << "\n";
		file << "NumDet: \t" << tmi.nDeterObj << "\n";
		file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" << " Nel" << "\n";
		for (int i = 0; i < MAX_OBJ_METEOR; i++) {
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

void PrintMSHI_M2(ofstream &file, struct MSHI_M2 tmi, TDateTime curDate) {
	file << OutputDateTime(curDate).c_str() << "\n";
	file << "____________________________________" << "\n";
	file << "Ã‡ÒÒË‚ ƒŒ–" << "\n";
	file << "Tpr\t" << tmi.timeBOKZ << "\n";
	file << uppercase << hex << setfill('0');
	file << " —1:\t" << "0x" << setw(4) << tmi.status1 << "\n";
	file << " —2:\t" << "0x" << setw(4) << tmi.status2 << "\n";
	file << dec << setfill(' ');
	file << "«‡‚. π\t" << tmi.serialNumber << "\n";
	file << "OZ:\t" << tmi.OZ[0] << "; " << tmi.OZ[1] << ";" << tmi.OZ[2] << "\n";
	file << "Qornt:\t" << tmi.Qornt[0] << "; " << tmi.Qornt[1] << "; " << tmi.Qornt[2]<< "; " << tmi.Qornt[3] << "\n";
	file << "W:\t" << tmi.W[0] << "; " << tmi.W[1] << ";" << tmi.W[2] << "\n";
	file << "NumLoc:\t" << tmi.nLocalObj << "\n";
	file << "NumDet:\t" << tmi.nDeterObj << "\n";
	file << "ThMax:\t" << tmi.thMax << "\n";
	file << "mxy, ÏÍÏ:\t" << tmi.m_cur/10. << "\n";
	file << "____________________________________" << "\n";
	file << flush;
}

void PrintTMI1_M2(ofstream &file, struct TMI1_M2 tmi) {
	file << "____________________________________" << "\n";
	file << "Ã‡ÒÒË‚ “Ã»1" << "\n";
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
	file << "◊ËÒÎÓ Á‚ÂÁ‰ ‚ Í‡Ú‡ÎÓ„Â:\t" << tmi.countStar << "\n";
	file << "X-‰ÂÌÚËÙËÍ‡ÚÓ:\t" << tmi.CRC << "\n";
	file << "ƒ‡Ú‡ ÒÓÁ‰‡ÌËˇ œŒ:\t"; PrintProgDate(file, tmi.Date, 2015); file << "\n";
	file << "¬ÂÒËˇ œŒ:\t"; PrintProgVersion(file, tmi.Version); file << "\n";
	file << "—·ÓÍ‡ œŒ:\t" << tmi.Build << "\n";
	file << "____________________________________" << "\n";
	file << flush;
}

void PrintTMI2_M2(ofstream &file, struct TMI2_M2 tmi, TDateTime curDate) {
	file << OutputDateTime(curDate).c_str() << "\n";
	file << "____________________________________" << "\n";
	file << "Ã‡ÒÒË‚ “Ã»2" << "\n";
	file << "Tpr\t" << tmi.timeBOKZ << "\n";
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

	for (int i = 0; i < MAX_STAT_M2; i++) {
			file << "—˜ÂÚ˜ËÍ π " << (i + 1) << ":\t" << tmi.cntStatOrient[i]
				<< "\n";
	}
	file << "____________________________________" << "\n";
	file << flush;
}

void PrintDTMI_M2(ofstream &file, struct DTMI_M2 tmi, TDateTime curDate) {
	file << OutputDateTime(curDate).c_str() << "\n";
	file << "____________________________________" << "\n";
	file << "Ã‡ÒÒË‚ ƒ“Ã»" << "\n";
	file << "Tpr\t" << tmi.timeBOKZ << "\n";
	file << uppercase << hex << setfill('0');
	file << " —1\t" << "0x" << setw(4) << tmi.status1 << "\n";
	file << " —2\t" << "0x" << setw(4) << tmi.status2 << "\n";
	file << "POST\t" << "0x" << setw(4) << tmi.post << "\n";
	file << dec << setfill(' ');
	file << "«‡‚. π\t" << tmi.serialNumber << "\n";
	file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
	file << "Mean: \t" << tmi.meanCadr[0] << "\n";
	file << "Sigma: \t" << tmi.meanCadr[1] << "\n";
	file << "NumLoc1: \t" << tmi.nLocalObj[0] << "\n";
	file << "NumLoc2: \t" << tmi.nLocalObj[1] << "\n";
	file << "NumAll1: \t" << tmi.nLocalAll[0] << "\n";
	file << "NumAll2: \t" << tmi.nLocalAll[1] << "\n";
	file << "NumStore: \t" << tmi.nStoreObj << "\n";
	file << "NumDet: \t" << tmi.nDeterObj << "\n";
	file << "NumFrag:\t" << tmi.nWindows << "\n";
	file << "Sector:\t" << tmi.nSec << "\n";

	for (int nCadr = 0; nCadr < 2; nCadr++) {
		file << setw(6) << "π" << " X, pix" << " Y, pix"
			 << " Bright" << " Nel" << "\n";

		for (int i = 0; i < 15; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.LocalList[nCadr][i].x << "\t" << tmi.LocalList[nCadr][i].y << "\t";
			file << tmi.LocalList[nCadr][i].bright << "\t" << tmi.LocalList[nCadr][i].size << "\n";
		}
	}

	file << setw(6) << "π" << " X, pix" << " Y, pix" << "\n";
	for (int i = 0; i < 16; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << (int)tmi.nObjectWindow[i] << "\n";
	}
	for (int i = 0; i < 3; i++) {
		file << "Wop[" << i << "]:\t" << tmi.omega[i] << "\n";
	}
	for (int i = 0; i < 3; i++) {
		file << "Vline[" << i << "]:\t" << tmi.Vline[i] << "\n";
	}
	file << "Tlst:\t" << tmi.timeQuatLast << "\n";
	for (int i = 0; i < 4; i++) {
		file << "Qlst[" << i << "]:\t" << tmi.quatLast[i] << "\n";
	}
	file << "›ÔÓı‡: \t" << tmi.Epoch << "\n";
	file << "MaxHist: \t" << tmi.maxHist << "\n";
	file << "MaxHistX: \t" << (short)tmi.maxHistX << "\n";
	file << "MaxHistY: \t" << (short)tmi.maxHistY << "\n";
	file << "____________________________________" << "\n";
	file << flush;
}

void PrintMLOC_M2(ofstream &file, struct MLOC_M2 tmi, TDateTime curDate) {
	file << OutputDateTime(curDate).c_str() << "\n";
	file << "____________________________________" << "\n";
	file << "Ã‡ÒÒË‚ ÃÀŒ " << "\n";
	file << "Tpr\t" << tmi.timeBOKZ << "\n";
	file << uppercase << hex << setfill('0');
	file << " —1\t" << "0x" << setw(4) << tmi.status1 << "\n";
	file << " —2\t" << "0x" << setw(4) << tmi.status2 << "\n";
	file << dec << setfill(' ');
	file << "«‡‚. π\t" << tmi.serialNumber << "\n";
	file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
	file << "NumLoc: \t" << tmi.nLocalObj1 << "\n";
	file << "NumAll: \t" << tmi.nLocalObj2 << "\n";
	file << "Mean: \t" << tmi.meanCadr << "\n";

	file << setw(6) << "π" << " X, pix" << " Y, pix" << " Bright" <<
			" Nel" << "\n";
	for (int i = 0; i < 36; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.LocalList[i].x << "\t" << tmi.LocalList[i].y << "\t";
		file << tmi.LocalList[i].bright << "\t" << tmi.LocalList[i].size << "\n";
	}
    file << "____________________________________" << "\n";
	file << flush;
}

void PrintLogMSHI_M2(ofstream &file, MSHI_M2 tmi, TDateTime curDate, bool &create)
{
	if (!create) {
		file << "Day/Time\t" << "Tbshv\t";
		file << "Q[0]\t" << "Q[1]\t" << "Q[2]\t" << "Q[3]\t";
		file << "Serial\t" << "KC1\t" << "KC2\t";
		file << "NumStar\t" << "NumFrag\t";
		file << "NumLoc\t" << "NumDet\t";
		file << "ThMax\t" << "Mxy,mkm\t" << "Tcmv\t";
		file << "OZ[0]\t" << "OZ[1]\t" << "OZ[2]\t";
		file << "Wx,''/c\t" << "Wy,''/c\t" << "Wz,''/c\t";
		file << "Al, deg\t" << "Dl, deg\t" << "Az, deg\t";
		file << "\n";
		create = true;
	}

	file << OutputDateTime(curDate).c_str() << "\t";
	file << tmi.timeBOKZ << "\t";
	for (int i = 0; i < 4; i++) {
		file << tmi.Qornt[i] << "\t";
	}

	file << tmi.serialNumber << "\t";
	file << uppercase << hex << setfill('0');
	file << "0x" << setw(4) << tmi.status1 << "\t";
	file << "0x" << setw(4) << tmi.status2 << "\t";

	file << dec << setfill(' ');
	file << tmi.nLocalObj  << "\t" << tmi.nDeterObj << "\t";
	file << tmi.thMax  << "\t" << tmi.m_cur/(1e+4) * 1000. << "\t";

	for (int i = 0; i < 3; i++) {
		file << tmi.OZ[i] << "\t";
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
	file << "\n" << flush;
}

void PrintLogTMI1_M2(ofstream &file, TMI1_M2 tmi, TDateTime curDate, bool &create)
{
	if (!create) {
		file << "Date&Time\t" << "Tbshv\t";
		file << "KC1\t" << "KC2\t" << "POST\t";
		file << "Serial\t" << "Texp\t";
		file << "Foc\t" << "Xg\t" << "Yg\t";
		file << "Mean\t" << "Sigma\t";
		file << "cntDef\t" << "cntStars\t";
		file << "ProgCS\t" << "Date\t" << "Version\t" << "Build\t";
		file << "\n";
		create = true;
	}

	file << OutputDateTime(curDate).c_str() << "\t";
	file << tmi.timeBOKZ << "\t";
	file << uppercase << hex << setfill('0');
	file << "0x" << setw(4) << tmi.status1 << "\t";
	file << "0x" << setw(4) << tmi.status2 << "\t";
	file << "0x" << setw(4) << tmi.post << "\t";
	file << dec << setfill(' ');
	file << tmi.serialNumber << "\t" << tmi.timeExp << "\t";
	file << tmi.Foc << "\t" << tmi.Xg << "\t" << tmi.Yg << "\t";
	file << tmi.Mean << "\t" << tmi.Sigma << "\t";
	file << tmi.countDefect << "\t" << tmi.countStar << "\t";
	file << uppercase << hex << setfill('0') ;
	file << "0x" << setw(4) << tmi.CRC << "\t";
	file << dec << setfill(' ');
	PrintProgDate(file, tmi.Date, 2015); file << "\t";
	PrintProgVersion(file, tmi.Version); file << "\t";
	file << tmi.Build << "\t";
	file << "\n" << flush;
}

	void PrintLogTMI2_M2(ofstream &file, TMI2_M2 tmi, TDateTime curDate, bool &create)
	{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t" << "POST\t";
			file << "Serial\t" << "Texp\t";
			file << "”—ƒ\t" << "ÕŒ\t" << "ÕŒ—À\t";
			file << "TŒ\t" << "TŒ—À\t" << "—À≈∆\t";
			for (int i = 0; i < MAX_STAT_M2; i++) {
				file << "EC" << (i+1) << "\t";
			}
			file << "\n";
			create = true;
		}

		file << OutputDateTime(curDate) << "\t";
		file << tmi.timeBOKZ << "\t";
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
		for (int i = 0; i < MAX_STAT_M2; i++) {
			file << tmi.cntStatOrient[i] << "\t";
		}
		file << "\n" << flush;
	}

	void PrintLogDTMI_M2(ofstream &file, DTMI_M2 tmi, TDateTime curDate, int cntError, bool &create)
	{
		if (!create) {
			file << "Date&Time\t" << "Tbshv\t";
			file << "KC1\t" << "KC2\t" << "POST\t";
			file << "Serial\t" << "Texp\t";
			file << "NumL[0]\t" << "NumL[1]\t";
			file << "NumAll[0]\t" << "NumAll[1]\t";
			file << "Mean[0]\t" << "Mean[1]\t";
			file << "NumStore\t" << "NumDet\t" << "NumFrag\t";
			file << "Epoch\t" << "ErrorCS\t";
			file << "\n";
			create = true;
		}

		file << OutputDateTime(curDate) << "\t";
		file << tmi.timeBOKZ << "\t";
		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << tmi.status1 << "\t";
		file << "0x" << setw(4) << tmi.status2 << "\t";
		file << "0x" << setw(4) << tmi.post << "\t";

		file << dec << setfill(' ');
		file << tmi.serialNumber << "\t" << tmi.timeExp << "\t";
		file << tmi.nLocalObj[0] << "\t" << tmi.nLocalObj[1] << "\t";
		file << tmi.nLocalAll[0] << "\t" << tmi.nLocalAll[1] << "\t";
		file << tmi.meanCadr[0] << "\t" << tmi.meanCadr[1] << "\t";
		file << tmi.nStoreObj << "\t" << tmi.nDeterObj << "\t";
		file << tmi.nWindows << "\t" << tmi.Epoch << "\t";
		file << cntError << "\t" << flush;
		file << "\n" << flush;
	}

	struct TDateKondor {
	   unsigned short nDay  : 9;   // 1..366   (9 bits)
	   unsigned short nHour : 5;   // 0..23  (5 bits)
	   unsigned short reserve : 2; // 00  (2 bits)
	   unsigned short nMin : 6;    // 0..59 (6 bits)
	   unsigned short nSec : 6;    // 0..59 (6 bits)
	   unsigned short nYear : 4;   // 0..15 (4 bits)
	};

void PrintDtmiM60K(ofstream &file, struct DTMI_M60K tmi)
{
	file << "____________________________________" << "\n";
	file << "Ã‡ÒÒË‚ ƒ“Ã»" << "\n";
	struct TDateKondor curTime;
	memcpy(&curTime, &tmi.timeBOKZ, sizeof(curTime));
	file << "Tpr\t" << tmi.timeBOKZ << " ("<< curTime.nDay << "." << 2017 + curTime.nYear << " ";
	file << curTime.nHour << ":" << curTime.nMin << ":" << curTime.nSec << ")\n";
	file << uppercase << hex << setfill('0');
	file << " —1\t" << "0x" << setw(4) << tmi.status1 << "\n";
	file << " —2\t" << "0x" << setw(4) << tmi.status2 << "\n";
	file << "POST\t" << "0x" << setw(4) << tmi.post << "\n";
	file << dec << setfill(' ');
	file << "«‡‚. π\t" << tmi.serialNumber << "\n";
	file << "Texp, ÏÒ:\t" << tmi.timeExp << "\n";
	file << "MeanC[0]: \t" << tmi.meanCadr[0] << "\n";
	file << "MeanC[1]: \t" << tmi.meanCadr[1] << "\n";
	file << "NumLoc1: \t" << tmi.nLocalObj[0] << "\n";
	file << "NumLoc2: \t" << tmi.nLocalObj[1] << "\n";
	file << "NumAll1: \t" << tmi.nLocalAll[0] << "\n";
	file << "NumAll2: \t" << tmi.nLocalAll[1] << "\n";
	file << "MaxHist: \t" << tmi.maxHist << "\n";
	file << "MaxHistX: \t" << (short)tmi.maxHistX << "\n";
	file << "MaxHistY: \t" << (short)tmi.maxHistY << "\n";
	file << "NumStore: \t" << tmi.nStoreObj << "\n";
	file << "NumDet: \t" << tmi.nDeterObj << "\n";
	file << "NumFrag:\t" << tmi.nWindows << "\n";
	file << "Sector:\t" << tmi.nSec << "\n";
	file << "Tlst:\t" << tmi.timeMatrixLast << "\n";
	file << "Mlst[3][3]:\n";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			file << tmi.matrixLast[i][j] << "\t";
		}
		file << "\n";
	}

	file << "m_cur: \t" << tmi.m_cur << "\n";
	file << "deltaT: \t" << tmi.deltaT << "\n";
	for (int i = 0; i < 3; i++) {
		file << "Wop[" << i << "]:\t" << tmi.omega[i] << "\n";
	}
	for (int i = 0; i < 3; i++) {
		file << "Vline[" << i << "]:\t" << tmi.Vline[i] << "\n";
	}

	file << "Mapr[3][3]:\n";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			file << tmi.matrixBoard[i][j] << "\t";
		}
		file << "\n";
	}
	file << "›ÔÓı‡: \t" << tmi.Epoch << "\n";


	file << setw(6) << "π" << " X, pix" << " Y, pix"
			 << " Bright" << " Nel" << "\n";
	for (int i = 0; i < 15; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.LocalList1[i].x << "\t" << tmi.LocalList1[i].y << "\t";
		file << tmi.LocalList1[i].bright << "\t" << tmi.LocalList1[i].size << "\n";
	}

	file << setw(6) << "π" << " X, pix" << " Y, pix"
			 << " Bright" << " Nel" << "\n";
	for (int i = 0; i < 15; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.LocalList2[i].x << "\t" << tmi.LocalList2[i].y << "\t";
		file << tmi.LocalList2[i].bright << "\t" << tmi.LocalList2[i].size << "\n";
	}

	file << setw(6) << "π" << " X, pix" << " Y, pix"
			 << " Bright" << " Nel" << "\n";
	for (int i = 0; i < 14; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << tmi.ResultList[i].x << "\t" << tmi.ResultList[i].y << "\t";
		file << tmi.ResultList[i].bright << "\t" << tmi.ResultList[i].size << "\n";
	}

	file << setw(6) << "π" << " ObjFrag" << "\n";
	for (int i = 0; i < 14; i++) {
		file << setw(6) << (i + 1) << "\t";
		file << (int)tmi.nObjectWindow[i] << "\n";
	}

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

	if (counter) {
		FileName = FileName + IntToStr((int)counter) + "_" + TDateTime::CurrentDate().DateString() + "_00-00-" + fileNumber + ".iki";
	}
	else {
		TDateTime dateTimeIKI;
		TFormatSettings curFormat;
		dateTimeIKI.Val = cadrInfo.Time;
		curFormat.ShortDateFormat = "yyyy_mm_dd";
		curFormat.LongTimeFormat  = "hh_nn_ss_zzz";
		FileName = FileName + "100ms_" + DateToStr(dateTimeIKI, curFormat)
								 + "_" + TimeToStr(dateTimeIKI, curFormat) + ".iki";
	}

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
//			if (abs(diff) > 5)        // ÔÓÚÓÏ Û·‡Ú¸
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
