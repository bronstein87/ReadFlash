// ---------------------------------------------------------------------------

#include <Classes.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.UI.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.PG.hpp>
#include <FireDAC.Phys.PGDef.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Login.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include <memory>
#include <string>
#include <vector>
#ifndef DbClientH
#define DbClientH
#include "ParseProtocol.h"
#include "AddString.h"
#include "MathM.h"
using namespace add_string;
using namespace parse_prot;
using namespace std;
// ---------------------------------------------------------------------------

	struct GeneralizedDTMI {
		AnsiString timeBOKZ;
		double timePr;
		AnsiString status1, status2;
		unsigned short serialNumber, timeExp;
		unsigned short nLocalObj, nDeterObj, nWindows, epsillon;
		float dTimeBOKZ;
		AnsiString LocalList;
		AnsiString quatBoard, omega, centrWindow;
		AnsiString levelWindow, nObjectWindow;
		unsigned int timeQuatLast;
		AnsiString quatLast;
		float Epoch;
		AnsiString nLocal;
		unsigned short maxHist;
		unsigned short maxHistX, maxHistY;
	};

	struct GeneralizedMSHIOR {
		short deviceNumber;
		AnsiString datetime;
		double timePr;
		AnsiString quat;
		AnsiString angles;
		AnsiString status1;
		AnsiString status2;
        AnsiString wo;
	};

	struct GeneralizedSHTMI1 {
		AnsiString timeBOKZ;
		double timePr;
		AnsiString status1, status2;
		unsigned short serialNumber, post;
		float Foc, Xg, Yg;
		unsigned short timeExp, Mean, Sigma, countDefect;
		unsigned short CRC, Date, Version;
	};

	struct GeneralizedSHTMI2 {
		AnsiString timeBOKZ;
		double timePr;
		AnsiString status1, status2;
		unsigned short serialNumber, post, timeExp;
		unsigned short cntCommandWord, cntCallNO, cntNOtoSLEZH;
		unsigned short cntCallTO, cntTOtoSLEZH;
		unsigned int cntSLEZH;
		AnsiString cntStatOrient;
	};

class DbClient : public TComponent {

public:

	_fastcall explicit DbClient(TComponent* owner, TFDGUIxLoginDialog *FDGUIxLoginDialog1);

	void connect();
	void disconnect();
	bool isConnected(){return conn->Connected;}
	void insertDTMI(const vector<GeneralizedDTMI>& dtmi, const AnsiString& kaName);
	void insertMSHIOR(const vector<GeneralizedMSHIOR>& mshior, const AnsiString& kaName);
	void insertSHTMI1(const vector<GeneralizedSHTMI1>& shtmi1, const AnsiString& kaName);
	void insertSHTMI2(const vector<GeneralizedSHTMI2>& shtmi2, const AnsiString& kaName);
	void setConnParams(TFDConnectionDefParams *params);
	void afterConnect(TNotifyEvent event);
	vector <AnsiString> getKaNames();
	AnsiString getSchemaNameByKaName(const AnsiString& kaName);


	static GeneralizedSHTMI1 convertSHTMI1(const SHTMI1& shtmi1, const TDateTime& date);
	static GeneralizedSHTMI2 convertSHTMI2(const SHTMI2& shtmi2, const TDateTime& date);
	static GeneralizedDTMI convertDTMI(const DTMI& dtmi, const TDateTime& date);
	static GeneralizedMSHIOR convertMSHI_BOKZM(const MSHI_BOKZM& mshior_BOKZM, const TDateTime& date, unsigned int deviceNumber);
	static GeneralizedSHTMI1 convertSHTMI1_BOKZM(const SHTMI1_BOKZM& shtmi1_BOKZM, const TDateTime& date);
	static GeneralizedSHTMI2 convertSHTMI2_BOKZM(const SHTMI2_BOKZM& shtmi2_BOKZM, const TDateTime& date);
	static GeneralizedDTMI convertDTMI_BOKZM(const DTMI_BOKZM& dtmi_BOKZM, const TDateTime& date);
   	template <typename T>
	static AnsiString convertArrayToString(T array, unsigned int size);
	template <typename T>
	static AnsiString convertMatrixToString(T matrix, unsigned int rows, unsigned int columns);
	_fastcall ~DbClient();

private:
	DbClient& operator = (const DbClient&);
	_fastcall explicit DbClient(const DbClient& other);
	void __fastcall onError(System::TObject* ASender, int ATimes, int AOffset,
	Firedac::Stan::Error::EFDDBEngineException* AError, Firedac::Stan::Intf::TFDErrorAction &AAction);
	void setQuerySettings(TFDQuery* query);


	std::unique_ptr<TFDConnection>conn;
	unsigned int queryCount;
	vector<TFDQuery> queries;
	void makeLog();


};


 	template <typename T>
	AnsiString DbClient::convertArrayToString(T array, unsigned int size)
	{
		AnsiString arrayStr;// = "{}";
		AnsiString toInsert;
		for (int i = 0; i < size; i++)
		{
			toInsert = add_string::toString(array[i]) + ",";
			arrayStr.Insert(toInsert,  arrayStr.Length() + 1);
		}
		arrayStr.Insert("{", 1);
		arrayStr = arrayStr.Delete(arrayStr.Length(), 1);
		arrayStr.Insert("}", arrayStr.Length() + 1);
		return arrayStr;
	}

	template <typename T>
	AnsiString DbClient::convertMatrixToString(T matrix, unsigned int rows, unsigned int columns)
	{
		AnsiString arrayStr;
		for	(int i = 0; i < rows; i++)
		{
			arrayStr += "{";
			for(int j = 0; j < columns; j++)
			{
				arrayStr += add_string::toString(matrix[i][j]) + ",";
			}

			arrayStr.Insert("}", arrayStr.Length());
		}
		arrayStr.Delete(arrayStr.Length(), 1);
        return arrayStr;
	}
// ---------------------------------------------------------------------------
#endif



