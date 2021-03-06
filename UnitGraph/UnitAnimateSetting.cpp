//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "UnitAnimateSetting.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------


const int maxDev  = 4;
const int maxZero = 10;
const int maxSpaceCraftInfoCount = 200; // ?? tmp
//struct _DateTime {
//	unsigned short year, month, day;
//	unsigned short hour, min, sec, msec;
//};

struct SateliteInfo {
	string name, typeDev;
	int cntDev, serial[maxDev];
	int cntZeroTime;
	_DateTime zeroTime[maxZero];
};

void LoadSatelliteList(AnsiString _sourceDir, vector <SateliteInfo> &vSat);

void LoadSatelliteList(AnsiString _sourceDir, vector <SateliteInfo> &vSat)
{
	ifstream finp;
	SateliteInfo sat;

//load satellite list
	finp.open(AnsiString(_sourceDir + "\\SatelliteInfo\\Satellite.txt").c_str());
	if (!finp.is_open())
	{
		ShowMessage("�� ������� ������� \\SatelliteInfo\\Satellite.txt");
		return;
	}
	while (getline(finp, sat.name)) {
		vSat.push_back(sat);
	}
	finp.close(); finp.clear();

//load device list
	int iSat = 0;
	finp.open(AnsiString(_sourceDir + "\\SatelliteInfo\\StarTrack.txt").c_str());
	if (!finp.is_open())
	{
		ShowMessage("�� ������� ������� \\SatelliteInfo\\StarTrack.txt");
		return;
	}
	while (!finp.eof()) {
		finp >> vSat[iSat].typeDev >> vSat[iSat].cntDev >> ":";
		if (vSat[iSat].cntDev > maxDev) vSat[iSat].cntDev = maxDev;
		for (int iDev = 0; iDev < vSat[iSat].cntDev; iDev++) {
			finp >> vSat[iSat].serial[iDev] >> ";";
		}
		iSat++;
	}
	finp.close(); finp.clear();

//load 1S-zero list
	string date1S;
	int year, month, day;
	iSat = 0;
	finp.open(AnsiString(_sourceDir + "\\SatelliteInfo\\ZeroTime.txt").c_str());
	if (!finp.is_open())
	{
		ShowMessage("�� ������� ������� \\SatelliteInfo\\ZeroTime.txt");
		return;
	}
	while (!finp.eof()) {
		finp >> vSat[iSat].cntZeroTime >> ":";
		if (vSat[iSat].cntZeroTime > maxZero) vSat[iSat].cntZeroTime = maxZero;
		for (int iZero = 0; iZero < vSat[iSat].cntZeroTime; iZero++) {
			finp >> date1S;
			sscanf(date1S.c_str(), "%ld.%ld.%ld", &day, &month, &year);
			vSat[iSat].zeroTime[iZero].Day = day;
			vSat[iSat].zeroTime[iZero].Month = month;
			vSat[iSat].zeroTime[iZero].Year  = year;
		}
		iSat++;
	}
	finp.close();
}

TFormAnimateSetting* FormAnimateSetting;
vector <SateliteInfo> vSatList(maxSpaceCraftInfoCount);
__fastcall TFormAnimateSetting::TFormAnimateSetting(TComponent* Owner)
	: TForm(Owner)
{
	AnsiString CurDir = GetCurrentDir();

	if (MainForm->client->isConnected())
	{
		updateKaNameList();
	}
	else
	{
		KAComboBox->Clear();
		LoadSatelliteList(CurDir, vSatList);
		for (int i = 0; i < vSatList.size(); i++) {
			KAComboBox->Items->Add(vSatList[i].name.c_str());
		}
		KAComboBox->ItemIndex = 0;
		KAComboBoxChange(Owner);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::KAComboBoxChange(TObject *Sender)
{
	AnsiString date1S;
	int iSat = KAComboBox->ItemIndex;
	if (iSat >= 0) {
		ZeroDate1S->Clear();
		for (int i = 0; i < vSatList[iSat].cntZeroTime; i++) {
			date1S.sprintf("%02d.%02d.%04d", vSatList[iSat].zeroTime[i].Day,
			vSatList[iSat].zeroTime[i].Month, vSatList[iSat].zeroTime[i].Year);
			ZeroDate1S->Items->Add(UnicodeString(date1S));
		}
		ZeroDate1S->ItemIndex = vSatList[iSat].cntZeroTime - 1;
        ZeroTime1S->Time = 0;

		EditListBokz->Clear();
		AnsiString list = (vSatList[iSat].typeDev + " ���. � ").c_str();
		for (int i = 0; i < vSatList[iSat].cntDev; i++) {
			list += IntToStr(vSatList[iSat].serial[i]);
			if (i <  vSatList[iSat].cntDev - 1) list += ",";
        }
        EditListBokz->Text = list;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::updateKaNameList()
{
	 vector<AnsiString> kaNames = MainForm->client->getKaNames();
	 KAComboBox->Clear();
	 for (int i = 0; i < kaNames.size(); i++)
	 {
		   KAComboBox->AddItem(UnicodeString(kaNames[i]), NULL);
	 }
	 KAComboBox->Text = KAComboBox->Items[0][0];
}
//---------------------------------------------------------------------------

void TFormAnimateSetting::LoadDefaultSatelliteInfo(AnsiString _curDir)
{
	KAComboBox->Clear();
	KAComboBox->Items->LoadFromFile(UnicodeString(_curDir + "\\SatelliteInfo\\Satellite.txt"));
	KAComboBox->ItemIndex = 0;
}

void TFormAnimateSetting::ReadINI(const AnsiString& fileName)
{
	std::unique_ptr<TIniFile> Ini ( new TIniFile(fileName));

//������� "��������"
	//������ "������� ��������"
	CheckBoxFillTableObjects->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckFillTableObjects", "0")));
	CheckBoxApplyObjectsSeries->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckApplyObjectsSeries", "0")));
	ShapeColorLocObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "LocObjTable", clWhite)));
	ShapeColorDetObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "DetObjTable", clWhite)));

	//������ "������� ����������"
	CheckBoxFillTableWindows->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckFillTableWindows", "0")));
	CheckBoxApplyObjectsSeries->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckApplyObjectsSeries", "0")));
	ShapeColorZeroObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "ZeroObjTable", clWhite)));
	ShapeColorOneObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "OneObjTable", clWhite)));
	ShapeColorTwoObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "TwoObjTable", clWhite)));
	ShapeColorThreeObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "ThreeObjTable", clWhite)));

	//������ "�������� �����"
	CheckBoxLabelStar->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckLabelStar", "0")));
//	CheckBoxLabelFrame->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckLabelFrame", "0")));

	//������ "������� �������"
	CheckBoxDateTime->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckDateTime", "1")));
	CheckBoxCurrentTime->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckCurrentTime", "1")));
	CheckBoxResultOnly->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "ResultOnly", "1")));
	CheckBoxOnlySummary->Checked =  (int)StrToInt(Ini->ReadString("CheckBox", "PlotStarGraphs", "0"));

//������� "�����"
	EditFilePrefix->Text = Ini->ReadString("File", "FilePrefix", "Img");
	BeginFromEdit->Text = Ini->ReadString("File", "BeginFrom", "0");
	SkipFrameCheckBox->Checked = StrToInt(Ini->ReadString("File", "SkipFrame", "1"));
	CheckBoxFilter->Checked = StrToInt(Ini->ReadString("File", "CheckFilter", "1"));
	CheckBoxFolderFilter->Checked = StrToInt(Ini->ReadString("File", "CheckFolderFilter", "1"));
	FilterEdit->Text  =  Ini->ReadString("File", "FilterEdit", "SLEZH");
    FilterFolderEdit->Text  =  Ini->ReadString("File", "FilterFolderEdit", "*.");

//������� "��������� ��"
	CheckBoxLoadToDb->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckLoadToDb", "0")));
	int iSat = StrToInt(Ini->ReadString("DataBase", "Satellite",  "-1"));
	if ( (iSat >= 0) && (iSat < KAComboBox->Items->Count) ) {
		KAComboBox->ItemIndex = iSat;
		KAComboBoxChange(FormAnimateSetting);
	}
	ZeroDate1S->Text = Ini->ReadString("ComboBox", "ZeroDate1S", "01.01.2018");
	ZeroTime1S->Time = StrToTime(Ini->ReadString("Edits", "ZeroTime1S", "23:00:00"));
	CheckBoxSinc->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckSinc", "0")));

//��������� FormGraphOrient
	TFormGraphOrient* FormGraphOrient =  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->ContrastCheckBox->Checked    = (int)(StrToInt(Ini->ReadString("DrawFrag", "CheckContrast", "0")));
	FormGraphOrient->PixelBrightCheckBox->Checked = (int)(StrToInt(Ini->ReadString("DrawFrag", "CheckPixelBright", "0")));
	FormGraphOrient->ScaleEdit->Text     = Ini->ReadString("DrawFrag", "FragScale", "25");
	FormGraphOrient->PixelSizeEdit->Text = Ini->ReadString("DrawFrag", "PixelSize", "25");
	FormGraphOrient->FontSizeEdit->Text  = Ini->ReadString("DrawFrag", "FontSize",  "10");

// �������������
	FillStatusTableCheckBox->Checked = StrToInt(Ini->ReadString("Additional", "FillStatusTable", "0"));
	AccuracyReportCheckBox->Checked = StrToInt(Ini->ReadString("Additional", "AccuracyReport", "0"));
	HandleAllFoldersLikeOneSampleCheckBox->Checked =  StrToInt(Ini->ReadString("Additional", "HandleAll", "0"));
}
//---------------------------------------------------------------------------

void TFormAnimateSetting::WriteINI(const AnsiString& fileName)
{
	std::unique_ptr<TIniFile> Ini ( new TIniFile(fileName));

//������� "��������"
	//������ "������� ��������"
	Ini->WriteString("CheckBox", "CheckFillTableObjects",   IntToStr((int)CheckBoxFillTableObjects->Checked));
	Ini->WriteString("CheckBox", "CheckApplyObjectsSeries", IntToStr((int)CheckBoxApplyObjectsSeries->Checked));
	Ini->WriteString("Colors", "LocObjTable", IntToStr(ShapeColorLocObjTable->Brush->Color));
	Ini->WriteString("Colors", "DetObjTable", IntToStr(ShapeColorDetObjTable->Brush->Color));

	//������ "������� ����������"
	Ini->WriteString("CheckBox", "CheckFillTableWindows",   IntToStr((int)CheckBoxFillTableWindows->Checked));
	Ini->WriteString("CheckBox", "CheckApplyWindowsSeries", IntToStr((int)CheckBoxApplyWindowsSeries->Checked));
	Ini->WriteString("Colors", "ZeroObjTable", IntToStr(ShapeColorZeroObjTable->Brush->Color));
	Ini->WriteString("Colors", "OneObjTable", IntToStr(ShapeColorOneObjTable->Brush->Color));
	Ini->WriteString("Colors", "TwoObjTable", IntToStr(ShapeColorTwoObjTable->Brush->Color));
	Ini->WriteString("Colors", "ThreeObjTable", IntToStr(ShapeColorThreeObjTable->Brush->Color));

	//������ "�������� �����"
	Ini->WriteString("CheckBox", "CheckLabelStar",   IntToStr((int)CheckBoxLabelStar->Checked));
	Ini->WriteString("CheckBox", "CheckLabelFrame",  IntToStr((int)CheckBoxLabelFrame->Checked));

	//������ "������� �������"
	Ini->WriteString("CheckBox", "DateTime", IntToStr((int)CheckBoxDateTime->Checked));
	Ini->WriteString("CheckBox", "CheckCurrentTime", IntToStr((int)CheckBoxCurrentTime->Checked));
	Ini->WriteString("CheckBox", "ResultOnly", IntToStr((int)CheckBoxResultOnly->Checked));
	Ini->WriteString("CheckBox", "PlotStarGraphs", IntToStr((int)CheckBoxOnlySummary->Checked));

//������� "�����"
	Ini->WriteString("File", "FilePrefix", EditFilePrefix->Text);
	Ini->WriteString("File", "BeginFrom",  BeginFromEdit->Text);
	Ini->WriteString("File", "SkipFrame",  IntToStr((int)SkipFrameCheckBox->Checked));
	Ini->WriteString("File", "CheckFilter",  IntToStr((int)CheckBoxFilter->Checked));
	Ini->WriteString("File", "CheckFolderFilter",  IntToStr((int)CheckBoxFolderFilter->Checked));
	Ini->WriteString("File", "FilterEdit",  FilterEdit->Text);
	Ini->WriteString("File", "FilterFolderEdit",  FilterFolderEdit->Text);
//������� "��������� ��"
	Ini->WriteString("DataBase", "Satellite",  IntToStr((int)KAComboBox->ItemIndex));
	Ini->WriteString("CheckBox", "CheckLoadToDb",   IntToStr((int)CheckBoxLoadToDb->Checked));
	Ini->WriteString("ComboBox","ZeroDate1S", ZeroDate1S->Text);
	Ini->WriteString("Edits","ZeroTime1S", TimeToStr(ZeroTime1S->Time));
	Ini->WriteString("CheckBox", "CheckSinc",   IntToStr((int)CheckBoxSinc->Checked));

//��������� FormGraphOrient
	TFormGraphOrient* FormGraphOrient = dynamic_cast<TFormGraphOrient*>(this->Owner);
	Ini->WriteString("DrawFrag", "CheckContrast", IntToStr((int)FormGraphOrient->ContrastCheckBox->Checked));
	Ini->WriteString("DrawFrag", "CheckPixelBright", IntToStr((int)FormGraphOrient->PixelBrightCheckBox->Checked));
	Ini->WriteString("DrawFrag", "FragScale", FormGraphOrient->ScaleEdit->Text);
	Ini->WriteString("DrawFrag", "PixelSize", FormGraphOrient->PixelSizeEdit->Text);
	Ini->WriteString("DrawFrag", "FontSize",  FormGraphOrient->FontSizeEdit->Text);

	// �������������
	Ini->WriteString("Additional", "FillStatusTable", IntToStr((int)FillStatusTableCheckBox->Checked));
	Ini->WriteString("Additional", "AccuracyReport",  IntToStr((int)AccuracyReportCheckBox->Checked));
	Ini->WriteString("Additional", "HandleAll", IntToStr((int)HandleAllFoldersLikeOneSampleCheckBox->Checked));
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::ShapeColorLocObjTableMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorLocObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->PrintTableObjectsHandler();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormAnimateSetting::ShapeColorDetObjTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorDetObjTable->Brush->Color = ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->PrintTableObjectsHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::ShapeColorZeroObjTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorZeroObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->PrintTableWindowsHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::ShapeColorOneObjTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorOneObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->PrintTableWindowsHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::ShapeColorTwoObjTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorTwoObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->PrintTableWindowsHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::ShapeColorThreeObjTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorThreeObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->PrintTableWindowsHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxFillTableObjectsClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->PrintTableObjectsHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxFillTableWindowsClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->PrintTableWindowsHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxLabelFrameClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient = dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->SetVisibleLabelFrame(CheckBoxLabelFrame->Checked);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxLabelStarClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient = dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->SetVisibleLabelStar(CheckBoxLabelStar->Checked);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxLabelObjectClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient = dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->SetVisibleLabelObject(CheckBoxLabelObject->Checked);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxApplyWindowsSeriesClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient = dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxApplyObjectsSeriesClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient =  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------


void __fastcall TFormAnimateSetting::FormClose(TObject *Sender, TCloseAction &Action)

{
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
	WriteINI(FormGraphOrient->SourceDir+"\\options.ini");
}
//---------------------------------------------------------------------------






//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::StatusReportCheckBoxClick(TObject *Sender)
{
	if (StatusReportCheckBox->Checked) {
		FillStatusTableCheckBox->Checked = true;
	}

}
//---------------------------------------------------------------------------


void __fastcall TFormAnimateSetting::HandleAllFoldersLikeOneSampleCheckBoxClick(TObject *Sender)

{
	if (!HandleAllFoldersLikeOneSampleCheckBox->Checked)
	{
		ThroughDateTimeCheckBox->Checked = false;
    }
}
//---------------------------------------------------------------------------


