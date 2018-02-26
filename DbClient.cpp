// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DbClient.h"

_fastcall DbClient::DbClient(TComponent* owner,
	TFDGUIxLoginDialog *FDGUIxLoginDialog1)
	: TComponent(owner), conn(new TFDConnection(NULL)) {
	conn->DriverName = "PG";
	conn->LoginDialog = FDGUIxLoginDialog1;
}
DbClient& DbClient:: operator = (const DbClient & other) {
	conn->Params = other.conn->Params;
	return *this;
}

_fastcall DbClient::~DbClient() {
	disconnect();
}

void DbClient::connect() {
	if (!conn->Connected) {
		conn->Connected = true;
	}
}

void DbClient::disconnect() {
	conn->Connected = false;
}

vector<AnsiString>DbClient::getKaNames() {
	unique_ptr<TFDQuery>query(new TFDQuery(NULL));
	query->OnExecuteError = onError;
	query->Connection = conn.get();
	query->Open("SELECT ka_name FROM public.ka_work_info;");
	vector<AnsiString>kaNames;
	while (!query->Eof) {
		kaNames.push_back(query->FieldByName("ka_name")->AsAnsiString);
		query->Next();
	}
	query->Close();
	return kaNames;
}

AnsiString DbClient::getSchemaNameByKaName(const AnsiString& kaName) {
	unique_ptr<TFDQuery>query(new TFDQuery(NULL));
	query->OnExecuteError = onError;
	query->Connection = conn.get();
	query->Open
		("SELECT schema_name FROM public.ka_work_info WHERE ka_name = :ka_name;"
		);
	query->ParamByName("ka_name")->AsAnsiString = kaName;

	return query->FieldByName("schema_name")->AsAnsiString;
}

void DbClient::afterConnect(TNotifyEvent event) {
	conn->AfterConnect = event;
}

void DbClient::insertDTMI(const vector<GeneralizedDTMI>& dtmi,
	const AnsiString& kaName) {
	unique_ptr<TFDQuery>query(new TFDQuery(NULL));
	query->OnExecuteError = onError;
	query->ResourceOptions->CmdExecMode = amCancelDialog;
	query->Connection = conn.get();
	query->Params->ArraySize = dtmi.size();
	AnsiString schemaName = getSchemaNameByKaName(kaName);
	query->SQL->Text = "INSERT INTO " + schemaName +
		".dtmi values (:p1, :p2, :p3, :p4, :p5, :p6, :p7, :p8, :p9, :p10, :p11, " ":p12, :p13, :p14, :p15, :p16, :p18, :p19, :p20, :p21, :p22, :p23, :p24)";

	// quat_board последний
	for (int i = 0; i < dtmi.size(); i++) {
		query->Params[i][0]->AsAnsiString = dtmi[i].timeBOKZ;
		query->Params[i][1]->AsFloat = dtmi[i].timePr;
		query->Params[i][2]->AsAnsiString = dtmi[i].status1;
		query->Params[i][3]->AsAnsiString = dtmi[i].status2;
		query->Params[i][4]->AsWord = dtmi[i].serialNumber;
		query->Params[i][5]->AsWord = dtmi[i].timeExp;
		query->Params[i][6]->AsWord = dtmi[i].nLocalObj;
		query->Params[i][7]->AsWord = dtmi[i].nDeterObj;
		query->Params[i][8]->AsWord = dtmi[i].nWindows;
		query->Params[i][9]->AsWord = dtmi[i].epsillon;
		query->Params[i][10]->AsFloat = dtmi[i].dTimeBOKZ;
		query->Params[i][11]->AsAnsiString = dtmi[i].LocalList;
		query->Params[i][12]->AsAnsiString = dtmi[i].omega;
		query->Params[i][13]->AsAnsiString = dtmi[i].centrWindow;
		query->Params[i][14]->AsAnsiString = dtmi[i].levelWindow;
		query->Params[i][15]->AsAnsiString = dtmi[i].nObjectWindow;
		query->Params[i][16]->AsLongword = dtmi[i].timeQuatLast;
		query->Params[i][17]->AsAnsiString = dtmi[i].quatLast;
		query->Params[i][18]->AsFloat = dtmi[i].Epoch;
		query->Params[i][19]->AsAnsiString = dtmi[i].nLocal;
		query->Params[i][20]->AsWord = dtmi[i].maxHist;
		query->Params[i][21]->AsWord = dtmi[i].maxHistX;
		query->Params[i][22]->AsWord = dtmi[i].maxHistY;
		query->Params[i][23]->AsAnsiString = dtmi[i].quatBoard;
	}
	query->Execute(query->Params->ArraySize, 0);
}

void DbClient::insertMSHIOR(const vector<GeneralizedMSHIOR>& mshior,
	const AnsiString& kaName) {
	unique_ptr<TFDQuery>query(new TFDQuery(NULL));
	query->OnExecuteError = onError;
	query->ResourceOptions->CmdExecMode = amCancelDialog;
	query->Connection = conn.get();
	query->Params->ArraySize = mshior.size();
	AnsiString schemaName = getSchemaNameByKaName(kaName);
	query->SQL->Text = "INSERT INTO " + schemaName +
		".shtmi1 values (:p1, :p2, :p3, :p4, :p5, :p6, :p7, :p8, :p9, :p10, :p11, " ":p12, :p13, :p14, :p15, :p16)";

	for (int i = 0; i < mshior.size(); i++) {
		query->Params[i][0]->AsWord = mshior[i].deviceNumber;
		query->Params[i][1]->AsAnsiString = mshior[i].datetime;
		query->Params[i][2]->AsFloat = mshior[i].timePr;
		query->Params[i][3]->AsAnsiString = mshior[i].quat;
		query->Params[i][4]->AsAnsiString = mshior[i].angles;
		query->Params[i][5]->AsAnsiString = "{NULL}"; // обработанный кватернион
		query->Params[i][6]->AsAnsiString = mshior[i].status1;
		query->Params[i][7]->AsAnsiString = mshior[i].status2;
		query->Params[i][8]->AsAnsiString = mshior[i].wo;

	}
	query->Execute(query->Params->ArraySize, 0);
}

void DbClient::insertSHTMI1(const vector<GeneralizedSHTMI1>& shtmi1,
	const AnsiString& kaName) {
	unique_ptr<TFDQuery>query(new TFDQuery(NULL));
	query->OnExecuteError = onError;
	query->ResourceOptions->CmdExecMode = amCancelDialog;
	query->Connection = conn.get();
	query->Params->ArraySize = shtmi1.size();
	AnsiString schemaName = getSchemaNameByKaName(kaName);
	query->SQL->Text = "INSERT INTO " + schemaName +
		".shtmi1 values (:p1, :p2, :p3, :p4, :p5, :p6, :p7, :p8, :p9, :p10, :p11, " ":p12, :p13, :p14, :p15, :p16)";

	for (int i = 0; i < shtmi1.size(); i++) {
		query->Params[i][0]->AsAnsiString = shtmi1[i].timeBOKZ;
		query->Params[i][1]->AsFloat = shtmi1[i].timePr;
		query->Params[i][2]->AsAnsiString = shtmi1[i].status1;
		query->Params[i][3]->AsAnsiString = shtmi1[i].status2;
		query->Params[i][4]->AsWord = shtmi1[i].serialNumber;
		query->Params[i][5]->AsWord = shtmi1[i].post;
		query->Params[i][6]->AsFloat = shtmi1[i].Foc;
		query->Params[i][7]->AsFloat = shtmi1[i].Xg;
		query->Params[i][8]->AsFloat = shtmi1[i].Yg;
		query->Params[i][9]->AsWord = shtmi1[i].timeExp;
		query->Params[i][10]->AsWord = shtmi1[i].Mean;
		query->Params[i][11]->AsWord = shtmi1[i].Sigma;
		query->Params[i][12]->AsWord = shtmi1[i].countDefect;
		query->Params[i][13]->AsWord = shtmi1[i].CRC;
		query->Params[i][14]->AsWord = shtmi1[i].Date;
		query->Params[i][15]->AsWord = shtmi1[i].Version;

	}
	query->Execute(query->Params->ArraySize, 0);
}

void DbClient::insertSHTMI2(const vector<GeneralizedSHTMI2>& shtmi2,
	const AnsiString& kaName) {
	unique_ptr<TFDQuery>query(new TFDQuery(NULL));
	query->OnExecuteError = onError;
	query->ResourceOptions->CmdExecMode = amCancelDialog;
	query->Connection = conn.get();
	query->Params->ArraySize = shtmi2.size();
	AnsiString schemaName = getSchemaNameByKaName(kaName);
	query->SQL->Text = "INSERT INTO " + schemaName +
		".shtmi2 values (:p1, :p2, :p3, :p4, :p5, :p6, :p7, :p8, :p9, :p10, :p11, " ":p12, :p13, :p14)";

	for (int i = 0; i < shtmi2.size(); i++) {
		query->Params[i][0]->AsAnsiString = shtmi2[i].timeBOKZ;
		query->Params[i][1]->AsFloat = shtmi2[i].timePr;
		query->Params[i][2]->AsAnsiString = shtmi2[i].status1;
		query->Params[i][3]->AsAnsiString = shtmi2[i].status2;
		query->Params[i][4]->AsWord = shtmi2[i].serialNumber;
		query->Params[i][5]->AsWord = shtmi2[i].post;
		query->Params[i][6]->AsWord = shtmi2[i].timeExp;
		query->Params[i][7]->AsWord = shtmi2[i].cntCommandWord;
		query->Params[i][8]->AsWord = shtmi2[i].cntCallNO;
		query->Params[i][9]->AsWord = shtmi2[i].cntNOtoSLEZH;
		query->Params[i][10]->AsWord = shtmi2[i].cntCallTO;
		query->Params[i][11]->AsWord = shtmi2[i].cntTOtoSLEZH;
		query->Params[i][12]->AsLongword = shtmi2[i].cntSLEZH;
		query->Params[i][13]->AsAnsiString = shtmi2[i].cntStatOrient;
	}
	query->Execute(query->Params->ArraySize, 0);
}

void DbClient::setConnParams(TFDConnectionDefParams *params) {
	conn->Params->ConnectionDef = params->ConnectionDef;
	conn->Params->DriverID = params->DriverID;
	conn->Params->Database = params->Database;
	conn->Params->UserName = params->UserName;
	conn->Params->UserName = params->Password;
}

GeneralizedSHTMI1 DbClient::convertSHTMI1(const SHTMI1& shtmi1,
	const TDateTime& date) {
	GeneralizedSHTMI1 gShtmi1;
	gShtmi1.timeBOKZ = date.DateTimeString();
	gShtmi1.timePr = shtmi1.timeBOKZ;
	gShtmi1.status1 = IntToHex(shtmi1.status1, 4);
	gShtmi1.status2 = IntToHex(shtmi1.status2, 4);
	gShtmi1.serialNumber = shtmi1.serialNumber;
	gShtmi1.post = shtmi1.post;
	gShtmi1.Foc = shtmi1.Foc;
	gShtmi1.Xg = shtmi1.Xg;
	gShtmi1.Yg = shtmi1.Yg;
	gShtmi1.timeExp = shtmi1.timeExp;
	gShtmi1.Mean = shtmi1.Mean;
	gShtmi1.Sigma = shtmi1.Sigma;
	gShtmi1.countDefect = shtmi1.countDefect;
	gShtmi1.CRC = shtmi1.CRC;
	gShtmi1.Date = shtmi1.Date;
	gShtmi1.Version = shtmi1.Version;
	return gShtmi1;
}

GeneralizedSHTMI2 DbClient::convertSHTMI2(const SHTMI2& shtmi2,
	const TDateTime& date) {
	GeneralizedSHTMI2 gShtmi2;
	gShtmi2.timeBOKZ = date.DateTimeString();
	gShtmi2.timePr = shtmi2.timeBOKZ;
	gShtmi2.status1 = IntToHex(shtmi2.status1, 4);
	gShtmi2.status2 = IntToHex(shtmi2.status2, 4);
	gShtmi2.serialNumber = shtmi2.serialNumber;
	gShtmi2.post = shtmi2.post;
	gShtmi2.timeExp = shtmi2.timeExp;
	gShtmi2.cntCommandWord = shtmi2.cntCommandWord;
	gShtmi2.cntCallNO = shtmi2.cntCallNO;
	gShtmi2.cntNOtoSLEZH = shtmi2.cntNOtoSLEZH;
	gShtmi2.cntCallTO = shtmi2.cntCallTO;
	gShtmi2.cntTOtoSLEZH = shtmi2.cntTOtoSLEZH;
	gShtmi2.cntSLEZH = shtmi2.cntSLEZH;
	gShtmi2.cntStatOrient = convertArrayToString(shtmi2.cntStatOrient,
		MAX_STAT);
}

GeneralizedDTMI DbClient::convertDTMI(const DTMI& dtmi, const TDateTime& date) {
	GeneralizedDTMI gDtmi;
	gDtmi.timeBOKZ = date.DateTimeString();
	gDtmi.timePr = dtmi.timeBOKZ;
	gDtmi.status1 = IntToHex(dtmi.status1, 4);
	gDtmi.status2 = IntToHex(dtmi.status2, 4);
	gDtmi.serialNumber = dtmi.serialNumber;
	gDtmi.timeExp = dtmi.timeExp;
	gDtmi.nLocalObj = dtmi.nLocalObj;
	gDtmi.nDeterObj = dtmi.nDeterObj;
	gDtmi.nWindows = dtmi.nWindows;
	gDtmi.epsillon = dtmi.epsillon;
	gDtmi.dTimeBOKZ = dtmi.dTimeBOKZ;
	gDtmi.LocalList = convertMatrixToString(dtmi.LocalList, MAX_OBJ_DTMI, 4);
	gDtmi.quatBoard = convertArrayToString(dtmi.quatBoard, 4);
	gDtmi.omega = convertArrayToString(dtmi.omega, 3);
	gDtmi.centrWindow = convertMatrixToString(dtmi.centrWindow, MAX_WINDOW, 2);
	gDtmi.levelWindow = convertArrayToString(dtmi.levelWindow, MAX_WINDOW);
	gDtmi.nObjectWindow = convertArrayToString(dtmi.nObjectWindow, MAX_WINDOW);
	gDtmi.timeQuatLast = dtmi.timeQuatLast;
	gDtmi.quatLast = convertArrayToString(dtmi.quatLast, 4);
	gDtmi.Epoch = dtmi.Epoch;
	gDtmi.nLocal = convertArrayToString(dtmi.nLocal, 2);
	gDtmi.maxHist = dtmi.maxHist;
	gDtmi.maxHistX = dtmi.maxHistX;
	gDtmi.maxHistY = dtmi.maxHistY;
	return gDtmi;
}
	GeneralizedMSHIOR DbClient::convertMSHI_BOKZM(const MSHI_BOKZM& mshior_BOKZM,
		const TDateTime& date, unsigned int deviceNumber) {
		GeneralizedMSHIOR gMshior;
		gMshior.datetime = date.DateTimeString();
		gMshior.deviceNumber = deviceNumber;
		gMshior.timePr = mshior_BOKZM.timeBOKZ;
		gMshior.status1 = IntToHex(mshior_BOKZM.status1, 4);
		gMshior.status2 = IntToHex(mshior_BOKZM.status2, 4);

		double angles[3];
		double matrix[3][3];
		for (int i = 0; i < 3; i++)
		{
			for	(int j = 0; j < 3; j++)
			{
                matrix[i][j] = mshior_BOKZM.Mornt[i][j];
			}
		}
		MatrixToEkvAngles(matrix, angles);
		for (int i = 0; i < 3; i++) {
			angles[i] = angles[i] * RTD;
		}
		gMshior.angles = convertArrayToString(angles, 3);
		gMshior.wo = "{0,0,0}";
		return gMshior;
	}


	GeneralizedSHTMI1 DbClient::convertSHTMI1_BOKZM(const SHTMI1_BOKZM& shtmi1_BOKZM, const TDateTime& date)
	{
		GeneralizedSHTMI1 gShtmi1;
		gShtmi1.timeBOKZ = date.DateTimeString();
		gShtmi1.timePr = shtmi1_BOKZM.timeBOKZ;
		gShtmi1.status1 = IntToHex(shtmi1_BOKZM.status1, 4);
		gShtmi1.status2 = IntToHex(shtmi1_BOKZM.status2, 4);
		gShtmi1.serialNumber = shtmi1_BOKZM.serialNumber;
		gShtmi1.post = shtmi1_BOKZM.post;
		gShtmi1.Foc = shtmi1_BOKZM.Foc;
		gShtmi1.Xg = shtmi1_BOKZM.Xg;
		gShtmi1.Yg = shtmi1_BOKZM.Yg;
		gShtmi1.timeExp = shtmi1_BOKZM.timeExp;
		gShtmi1.Mean = shtmi1_BOKZM.Mean;
		gShtmi1.Sigma = shtmi1_BOKZM.Sigma;
		gShtmi1.countDefect = shtmi1_BOKZM.countDefect;
		gShtmi1.CRC = shtmi1_BOKZM.verXCF;
		gShtmi1.Date = shtmi1_BOKZM.Date;
		gShtmi1.Version = shtmi1_BOKZM.verProg;
		return gShtmi1;
	}

	 GeneralizedSHTMI2 DbClient::convertSHTMI2_BOKZM(const SHTMI2_BOKZM& shtmi2_BOKZM, const TDateTime& date)
	 {
		GeneralizedSHTMI2 gShtmi2;
		gShtmi2.timeBOKZ = date.DateTimeString();
		gShtmi2.timePr = shtmi2_BOKZM.timeBOKZ;
		gShtmi2.status1 = IntToHex(shtmi2_BOKZM.status1, 4);
		gShtmi2.status2 = IntToHex(shtmi2_BOKZM.status2, 4);
		gShtmi2.serialNumber = shtmi2_BOKZM.serialNumber;
		gShtmi2.post = shtmi2_BOKZM.post;
		gShtmi2.timeExp = shtmi2_BOKZM.timeExp;
		gShtmi2.cntCommandWord = 0;
		gShtmi2.cntCallNO = shtmi2_BOKZM.cntCallNO;
		gShtmi2.cntNOtoSLEZH = 0;
		gShtmi2.cntCallTO = shtmi2_BOKZM.cntCallTO;
		gShtmi2.cntTOtoSLEZH = 0;
		gShtmi2.cntSLEZH = 0;
		gShtmi2.cntStatOrient = convertArrayToString(shtmi2_BOKZM.cntStatOrient,
			12);
	 }
//	struct DTMI_BOKZM {
//		float timeBOKZ;
//		unsigned short status1, status2;
//		unsigned short serialNumber;
//		float Foc, Xg, Yg;
//		unsigned short timeExp;
//		unsigned short nLocalObj, nDeterObj;
//		float LocalList[MAX_OBJ_BOKZM][4];
//	};
	GeneralizedDTMI DbClient::convertDTMI_BOKZM(const DTMI_BOKZM& dtmi_BOKZM, const TDateTime& date)
	{
		GeneralizedDTMI gDtmi;
		gDtmi.timeBOKZ = date.DateTimeString();
		gDtmi.timePr = dtmi_BOKZM.timeBOKZ;
		gDtmi.status1 = IntToHex(dtmi_BOKZM.status1, 4);
		gDtmi.status2 = IntToHex(dtmi_BOKZM.status2, 4);
		gDtmi.serialNumber = dtmi_BOKZM.serialNumber;
		gDtmi.timeExp = dtmi_BOKZM.timeExp;
		gDtmi.nLocalObj = dtmi_BOKZM.nLocalObj;
		gDtmi.nDeterObj = dtmi_BOKZM.nDeterObj;
		gDtmi.nWindows = 0;
		gDtmi.epsillon = 0;
		gDtmi.dTimeBOKZ = 0;
		gDtmi.LocalList = convertMatrixToString(dtmi_BOKZM.LocalList, MAX_OBJ_BOKZM, 4);
		gDtmi.quatBoard = "{}";
		gDtmi.omega = "{}";
		gDtmi.centrWindow = "{}";
		gDtmi.levelWindow = "{}";
		gDtmi.nObjectWindow = "{}";
		gDtmi.timeQuatLast = 0;
		gDtmi.quatLast =  0;
		gDtmi.Epoch = 0;
		gDtmi.nLocal = "{}";
		gDtmi.maxHist = 0;
		gDtmi.maxHistX = 0;
		gDtmi.maxHistY = 0;
		return gDtmi;

	}

	 void DbClient::setQuerySettings(TFDQuery* query)
	 {

	 }

		void __fastcall DbClient::onError(System::TObject* ASender, int ATimes, int AOffset,
	Firedac::Stan::Error::EFDDBEngineException* AError, Firedac::Stan::Intf::TFDErrorAction &AAction)
	{
		ShowMessage(" Ошибка : " + AError->ToString() + " в строке № " + IntToStr(AOffset));
	}
//---------------------------------------------------------------------------



#pragma package(smart_init)
