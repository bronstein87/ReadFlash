//---------------------------------------------------------------------------

#pragma hdrstop

#include "AddString.h"


namespace add_string {
	vector<string> split(const string& str, const string& delim)
	{
		vector<string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == string::npos) pos = str.length();
			string token = str.substr(prev, pos-prev);
			int i = token.find_first_not_of(" ");
			if (!token.empty() && token.find_last_not_of(" ") != string::npos)
				tokens.push_back(token);
			prev = pos + delim.length();
		}
		while (pos < str.length() && prev < str.length());

		return tokens;
	}

	bool contains (const string& str, const string& strToFind)
	{
		if (str.find(strToFind) != string::npos)
			return true;
		return false;
	}

	string toStdString(UnicodeString str)
	{
		return string(AnsiString(str).c_str());
	}
	UnicodeString toUString(const string& str)
	{
		return UnicodeString(str.c_str());
	}
	AnsiString toString(Variant value)
	{
		AnsiString string;
		switch(TVarData(value).VType)
		{
			case varSmallint:
			case varInteger:
			{
				string = IntToStr((int)value);
				break;
			}

			case varDouble:
			case varSingle:
			{
				string = Sysutils::FloatToStrF((double)value, ffFixed, 10, 4);
				break;
			}

			case varBoolean:
			{
				if (value)
					string = "true";
				else
					string = "false";
			}

		}
		return string;
	}

//	fromTStringDynArray(const TStringDynArray& arrayFrom, TStringList& arrayTo)
//	{
//
//    }
//	fromTStringList(const TStringList& arrayFrom, TStringDynArray& arrayTo)
//	{
//
//	}

}



//---------------------------------------------------------------------------
#pragma package(smart_init)
