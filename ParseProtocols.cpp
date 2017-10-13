//---------------------------------------------------------------------------

#pragma hdrstop

#include "ParseProtocols.h"
using namespace add_string;
namespace parse_prot {


	unsigned int ReadBinaryString(string binaryString) {
		string test_str;
		double sum = 0;

		for (int k = 0; k < binaryString.length(); k++) {
			test_str = binaryString[binaryString.length() - k - 1];
			sum += pow(2., k) * atoi(test_str.c_str());
		}
		return (unsigned int) sum;
	}

	AnsiString GetTimeString(unsigned long time)
	{
		AnsiString str, sTime = " ";
		int day, hour, min, sec;
		double day_double = time/86400./8.;

		day = (int)day_double;
		hour = (day_double - (int)day) * 24 + 1e-7;
		min  = (day_double - day - hour/24.) * 24 * 60 + 1e-7;
		sec  = (day_double - day - hour/24. - min/24./60.) * 86400. + 1e-7;
		if (day > 0) {
			sTime  = IntToStr(day) + "/";
		}
		str.sprintf("%02d:%02d:%02d", hour, min, sec);
		sTime += str;
		return sTime;
	}

	int TryReadSHTMI1(ifstream &finp, struct SHTMI1 &tmi) {
		string line, word;

		while (!finp.eof()) {
			finp >> word;
			if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "—≈–") || (word == "CEP")) {
				finp >> word;
				if ((word == "ÕŒÃ") || (word == "HOM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "œŒ—“") || (word == "œOCT")) {
				finp >> word;
				tmi.post = ReadBinaryString(word);
			}
			else if ((word == "T") || (word == "“")) {
				finp >> word;
				if (word == "› —œ")
					finp >> tmi.timeExp;
				else
					tmi.timeBOKZ = word; // finp>>tmi.timeBOKZ;
			}
			else if (word == "‘OK")
				finp >> tmi.Foc;
			else if (word == "’0")
				finp >> tmi.Xg;
			else if (word == "”0")
				finp >> tmi.Yg;
			else if ((word == "Ã“") || (word == "MT"))
				finp >> tmi.Mean;
			else if ((word == "—“") || (word == "CT"))
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
			getline(finp, line, '\n');
		}
		return 0;
	}

	int TryReadSHTMI2(ifstream &finp, struct SHTMI2 &tmi) {
		string line, word;

		while (!finp.eof()) {
			finp >> word;
			if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "—≈–") || (word == "CEP")) {
				finp >> word;
				if ((word == "ÕŒÃ") || (word == "HOM")) {
					finp >> tmi.serialNumber;
				}
			}
			else if ((word == "œŒ—“") || (word == "œOCT")) {
				finp >> word;
				tmi.post = ReadBinaryString(word);
			}
			else if ((word == "T") || (word == "“")) {
				finp >> word;
				if (word == "› —œ")
					finp >> tmi.timeExp;
				else {
					finp >> tmi.timeBOKZ; // ReadTimeBOKZ(word)
					tmi.timeBOKZ = word + tmi.timeBOKZ;
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
			else {
				int i = 0, fl_find = 0;
				while ((i < MAX_STAT) && (!fl_find)) {
					if ((word == arrStatErrorRus[i]) ||
						(word == arrStatErrorEng[i])) {
						finp >> tmi.cntStatOrient[i];
						fl_find = 1;
					}
					i++;
				}
			}
			if ((word == arrStatErrorRus[MAX_STAT - 1]) ||
				(word == arrStatErrorEng[MAX_STAT - 1])) {
				return 1;
			}
			getline(finp, line, '\n');
		}
		return 0;
	}

	int TryReadDTMI(ifstream &finp, struct DTMI &tmi) {
		string line, word, inpstr, test_word, test_str;
		int indexObject = 0, indexParam = 0, intVal, flLow = 1;
		float fl1, fl2, fl3, sum;
		int Stat1, Stat2;
		float mday, mhour, mmin, msec;

		while (!finp.eof()) {
			finp >> word;
			if ((word == " —1") || (word == "KC1")) {
				finp >> word;
				tmi.status1 = ReadBinaryString(word);
			}
			else if ((word == " —2") || (word == "KC2")) {
				finp >> word;
				tmi.status2 = ReadBinaryString(word);
			}
			else if ((word == "—≈–") || (word == "CEP")) {
				finp >> word;
				if ((word == "ÕŒÃ") || (word == "HOM")) {
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

					if (sscanf(line.c_str(), " %f/ %f:%f:%f", &mday, &mhour,
						&mmin, &msec) == 4) {
						tmi.timeQuatLast = mday * 86400 + mhour * 3600 +
							mmin * 60 + msec;
					}
					else if (sscanf(line.c_str(), "%f:%f:%f", &mhour,
						&mmin, &msec) == 3) {
						tmi.timeQuatLast = mhour * 3600 +
							mmin * 60 + msec;
					}
					else  tmi.timeQuatLast = 0;
					tmi.timeQuatLast *= 8;
					// finp>>tmi.timeQuatLast;
				}
				else {
					finp >> tmi.timeBOKZ;
					tmi.timeBOKZ = word + tmi.timeBOKZ;
				}
			}
			else if (word == "ÀŒ ") {
				getline(finp, line, '\n');
				int nread = sscanf(line.c_str(), "%f%f%f", &fl1, &fl2, &fl3);
				if (nread == 3) {
					indexObject = (short)fl1;
					indexParam = (short)fl2;
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
			// else if ((word=="ƒ“Ã»1")||(word=="ÿ“Ã»1")||(word=="ÿ“Ã»2"))
			// return 0;
		}
		return 0;
	}

	void PrintSHTMI1(ofstream &file, struct SHTMI1 tmi) {
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
		file << "»‰ÂÌÚËÙËÍ‡ÚÓ:\t" << tmi.CRC << "\n";
		file << "ƒ‡Ú‡:\t" << tmi.Date << "\n";
		file << "¬ÂÒËˇ:\t" << tmi.Version << "\n";
		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintSHTMI2(ofstream &file, struct SHTMI2 tmi) {
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
		file << "Tpr\t" << tmi.timeBOKZ << "\n";
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
			file << tmi.LocalList[i][2] << "\t" << tmi.LocalList[i][3] << "\n";
		}
		file << setw(6) << "π" << " X, pix" << " Y, pix" << "\n";
		for (int i = 0; i < MAX_WINDOW; i++) {
			file << setw(6) << (i + 1) << "\t";
			file << tmi.centrWindow[i][0] << "\t" << tmi.centrWindow[i][1]
				<< "\t";
			file << tmi.levelWindow[i] << "\t" << tmi.nObjectWindow[i] << "\n";
		}

		for (int i = 0; i < 3; i++) {
			file << "Wop[" << i << "]:\t" << tmi.omega[i] << "\n";
		}


		file << "Tlst:\t" << GetTimeString(tmi.timeQuatLast).c_str() << "\n";
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
		file << "Tpr\t" << tmi.timeBOKZ << "\n";
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
			file << tmi.LocalList[i][2] << "\t" << tmi.LocalList[i][3] << "\n";
		}

		file << "____________________________________" << "\n";
		file << flush;
	}

	void PrintLocalDTMI(struct DTMI tmi) {
		AnsiString fileName;

		fileName.printf("BOKZ_π%d_%s", tmi.serialNumber,
			(tmi.timeBOKZ).c_str());
		fileName += "_DTMI_LOC.txt";

		for (int i = 1; i < fileName.Length() + 1; i++) {
			if ((fileName[i] == ':') || (fileName[i] == ';') ||
				(fileName[i] == '?') || (fileName[i] == '>') ||
				(fileName[i] == '<') || (fileName[i] == '=') ||
				(fileName[i] == '/') || (fileName[i] == '\\')) {
				fileName[i] = '_';
			}
		}

		ofstream file(fileName.c_str());
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
			file << tmi.LocalList[i][2] << "\t" << tmi.LocalList[i][3] << "\n";
		}
		file.close();
	}

	void PrintLocalMLOC(struct LOC tmi) {
		AnsiString fileName;

		fileName.printf("BOKZ_π%d_%s", tmi.serialNumber,
			(tmi.timeBOKZ).c_str());
		fileName += "_MLOC_LOC.txt";

		for (int i = 1; i < fileName.Length() + 1; i++) {
			if ((fileName[i] == ':') || (fileName[i] == ';') ||
				(fileName[i] == '?') || (fileName[i] == '>') ||
				(fileName[i] == '<') || (fileName[i] == '=') ||
				(fileName[i] == '/') || (fileName[i] == '\\')) {
				fileName[i] = '_';
			}
		}

		ofstream file(fileName.c_str());

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
			file << tmi.LocalList[i][2] << "\t" << tmi.LocalList[i][3] << "\n";
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



    void ConvertDataDTMI_BOKZM(struct DTMI_BOKZM tmi, struct CadrInfo &mCadr)
{
	mCadr.IsBinary = true;
//	mCadr.IsReverse=true;
	mCadr.ImageHeight = 512;
	mCadr.ImageWidth = 512;
//	mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;
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

void PrintDTMI_BOKZM(ofstream &file, struct DTMI_BOKZM tmi)
{
	file<<"____________________________________"<<"\n";
	file<<"Ã‡ÒÒË‚ ƒ“Ã»"<<"\n";
	file<<"Tpr\t"<<tmi.timeBOKZ<<"\n";
	file<<uppercase<<hex<<setfill('0');
	file<<" —1\t"<<"0x"<<setw(4)<<tmi.status1<<"\n";
	file<<" —2\t"<<"0x"<<setw(4)<<tmi.status2<<"\n";
	file<<dec<<setfill(' ');
	file<<"«‡‚. π\t"<<tmi.serialNumber<<"\n";
	file<<"Foc, ÏÏ:\t"<<tmi.foc<<"\n";
	file<<"X0, ÏÏ:\t"<<tmi.X0<<"\n";
	file<<"Y0, ÏÏ:\t"<<tmi.Y0<<"\n";
	file<<"Texp, ÏÒ:\t"<<tmi.timeExp<<"\n";
	file<<"NumLoc: \t"<<tmi.nLocalObj<<"\n";
	file<<"NumFix: \t"<<tmi.nDeterObj<<"\n";
	file<<setw(6)<<"π"<<" X, pix"<<" Y, pix"<<" Bright"<<" Nel"<<"\n";
	for (int i = 0; i < MAX_OBJ_BOKZM; i++) {
		file<<setw(6)<<(i+1)<<"\t";
		file<<tmi.LocalList[i][0]<<"\t"<<tmi.LocalList[i][1]<<"\t";
		file<<tmi.LocalList[i][2]<<"\t"<<tmi.LocalList[i][3]<<"\n";
	}
	file<<"____________________________________"<<"\n";
	file<<flush;
}



// ÔÓ‚ÂˇÂÏ ÒÓ‰ÂÊËÚ ÎË ÔÓÚÓÍÓÎ ÂÊËÏ ÎÓÍ‡ÎËÁ‡ˆËË
bool checkLocFile(ifstream& in)
{
	string line;
	for(int i = 0; i < 3 ; i++)
	{
		getline(in,line);
	}
	// ÔÓ‚ÂË‚, ‚ÓÁ‚‡˘‡ÂÏ ÛÍ‡Á‡ÚÂÎ¸ Ì‡ Ì‡˜‡ÎÓ Ù‡ÈÎ‡
	in.seekg(0);

	if(line.find("ÀÓÍ‡ÎËÁ‡ˆËˇ") == string::npos)
	{
		return false;
	}
	return true;
}



void writeBOKZ1000ProtocolToIKI (CadrInfo& cadrInfo, bool InfoVecEmpty, TDateTime& startDate, double& timeStep, unsigned int& counter)
{
	unique_ptr <IKI_img> writer (new IKI_img());
	if (InfoVecEmpty)
	{
		writer->Georeferencing.DateTime = startDate;
	}
	else
	{
		startDate = IncMilliSecond(startDate, timeStep * 1000);
		writer->Georeferencing.DateTime = startDate;
		timeStep = 0.25;
	}

	writer->Georeferencing.FrameNumber = ++counter;
	writer->StarsData.RezStat = 0;
	writer->ImageData.FrameData.FrameHeight = 1024;
	writer->ImageData.FrameData.FrameWidth = 1024;
	writer->StarsData.SimulatedFrame.strrec = cadrInfo.SizeObjectsList;
	writer->StarsData.LocalizedCount = cadrInfo.CountLocalObj;
	//writer->StarsData.RecognizedCount = cadrInfo.CountDeterObj;
	writer->StarsData.Epsilon = cadrInfo.Epsilon;
	//writer->ImageData.WindowsData.WindowCount = cadrInfo.CountWindows;

	writer->StarsData.SimulatedFrame.StarRec = new STARREC [cadrInfo.SizeObjectsList];
	for (int i = 0; i < cadrInfo.SizeObjectsList; i++)
	{
		writer->StarsData.SimulatedFrame.StarRec[i].Xs = cadrInfo.ObjectsList[i].X;
		writer->StarsData.SimulatedFrame.StarRec[i].Ys = cadrInfo.ObjectsList[i].Y;
		writer->StarsData.SimulatedFrame.StarRec[i].Is = cadrInfo.ObjectsList[i].Bright;
		writer->StarsData.SimulatedFrame.StarRec[i].Ns = cadrInfo.ObjectsList[i].StarID;
		writer->StarsData.SimulatedFrame.StarRec[i].Mv = cadrInfo.ObjectsList[i].Mv;
		writer->StarsData.SimulatedFrame.StarRec[i].Sp[0] = cadrInfo.ObjectsList[i].Sp[0];
		writer->StarsData.SimulatedFrame.StarRec[i].Sp[1] = cadrInfo.ObjectsList[i].Sp[1];
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
	writer->WriteFormat(FileName);

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
	cadrInfo.Time = reader->Georeferencing.DateTime.Val;
	cadrInfo.FrameNumber = reader->Georeferencing.FrameNumber;
	cadrInfo.IsBinary = reader->ImageData.FrameData.DegreeBinning;
	cadrInfo.DataType = reader->ImageData.FrameData.DataType;
	cadrInfo.IsReverse = false;
	cadrInfo.IsOrient = !reader->StarsData.RezStat;
	cadrInfo.CountPixFilter = reader->FilterData.FilteredPixelsCount;
	cadrInfo.ImageHeight = reader->ImageData.FrameData.FrameHeight;
	cadrInfo.ImageWidth = reader->ImageData.FrameData.FrameWidth;
	cadrInfo.SizePixel = reader->CameraSettings.PixelSize;
	cadrInfo.StatOrient = reader->StarsData.RezStat;
	cadrInfo.CountStars = reader->StarsData.SimulatedFrame.strrec;
	cadrInfo.CountWindows = reader->ImageData.WindowsData.WindowCount;
	cadrInfo.CountLocalObj = reader->StarsData.LocalizedCount;
	cadrInfo.CountDeterObj = reader->StarsData.RecognizedCount;
	cadrInfo.SizeStarsList = reader->StarsData.SimulatedFrame.strrec;
	cadrInfo.SizeObjectsList = cadrInfo.CountLocalObj;
	cadrInfo.ResolutionACP = pow(2., reader->CameraSettings.ResolutionACP) - 1;

	// cadrInfo.SizeObjectsList =  cadrInfo.CountDeterObj;
	cadrInfo.SizeWindowsList = cadrInfo.CountWindows;

	for (int i = 0; i < cadrInfo.SizeStarsList; i++) {
		if (reader->StarsData.SimulatedFrame.StarRec[i].Xs == 0 &&  reader->StarsData.SimulatedFrame.StarRec[i].Ys == 0)
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

		if(reader->ImageData.WindowsData.Info[i].X == 0
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
		 winInfo.Mv = 0;
		 winInfo.ZipX = reader->ImageData.WindowsData.Info[i].ZipX;
		 winInfo.ZipY = reader->ImageData.WindowsData.Info[i].ZipY;
		 winInfo.StarID = 0;
		 winInfo.Sp[0] = 0;
		 winInfo.Sp[1] = 0;
		 cadrInfo.WindowsList.push_back(winInfo);
	}

	for (int i = 0; i < cadrInfo.SizeWindowsList; i++)
	{
		if (cadrInfo.ObjectsList[i].StarID == 0 && cadrInfo.WindowsList[i].CountObj > 1)
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

	if (CompareIKIRes)
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
