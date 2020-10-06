// ---------------------------------------------------------------------------

#ifndef AddStringH
#define AddStringH

#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <Classes.hpp>
#include <sysvari.h>
#include <System.RegularExpressions.hpp>
using namespace std;
namespace add_string {

	vector <string> split(const string& str, const string& delim);
	bool contains (const string& str, const string& strToFind);
	bool contains (const string& str, TRegEx& regEx);
	string toStdString(const UnicodeString& str);
	UnicodeString toUString(const string& str);
	AnsiString toString(Variant value);
	//fromTStringDynArray(const TStringDynArray& arrayFrom, TStringList& arrayTo);
	//fromTStringList(const TStringList& arrayFrom, TStringDynArray& arrayTo);

	template<typename Stream>
	size_t findWord(Stream& in, const string& word) {
		string lineToWrite;
		while (in >> lineToWrite) {
			if (lineToWrite.find(word) != string::npos) {
				return in.tellg() - lineToWrite.size();
			}
		}

		if (in.fail() && !in.eof()) {
			throw(string("Ошибка считывания файла"));
		}

		return string::npos;
	}

    	template<typename Stream>
	size_t findWord(Stream& in, const string& word, string& resStr) {
		string lineToWrite;
		while (in >> lineToWrite) {
			if (lineToWrite.find(word) != string::npos) {
				resStr = lineToWrite;
				return in.tellg() - lineToWrite.size();
			}
		}

		if (in.fail() && !in.eof()) {
			throw(string("Ошибка считывания файла"));
		}

		return string::npos;
	}

	template<typename Stream>
	size_t findLine(Stream& in, const string& line)
	{
		string lineToWrite;
		while (getline(in, lineToWrite))
		{
			if (lineToWrite.find(line) != string::npos)
			{
				return in.tellg() - lineToWrite.size();
			}
		}

		if (in.fail() && !in.eof())
		{
			throw(string("Ошибка считывания файла"));
		}

		return string::npos;
	}

	template<typename Stream>
	size_t findLineBack(Stream& in, const string& line, string& findLine) {
		while (getline(in, findLine)) {
			if (findLine.find(line) != string::npos) {
				return in.tellg() - findLine.size();
			}
		}

		if (in.fail() && !in.eof()) {
			throw(string("Ошибка считывания файла"));
		}

		return string::npos;
	}

}
// ---------------------------------------------------------------------------
#endif
