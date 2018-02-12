//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "UnitAnimateSetting.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------

	TFormAnimateSetting* FormAnimateSetting;
__fastcall TFormAnimateSetting::TFormAnimateSetting(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TFormAnimateSetting::ReadINI(const AnsiString& fileName)
{
	std::unique_ptr<TIniFile> Ini ( new TIniFile(fileName));

	CheckBoxFillTableObjects->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckFillTableObjects", "0")));
	CheckBoxApplyObjectsSeries->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckApplyObjectsSeries", "0")));
	ShapeColorLocObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "LocObjTable", clWhite)));
	ShapeColorDetObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "DetObjTable", clWhite)));

	CheckBoxFillTableWindows->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckFillTableWindows", "0")));
	CheckBoxApplyObjectsSeries->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckApplyObjectsSeries", "0")));
	ShapeColorZeroObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "ZeroObjTable", clWhite)));
	ShapeColorOneObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "OneObjTable", clWhite)));
	ShapeColorTwoObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "TwoObjTable", clWhite)));
	ShapeColorThreeObjTable->Brush->Color = TColor(StrToInt(Ini->ReadString("Colors", "ThreeObjTable", clWhite)));

	EditFilePrefix->Text = Ini->ReadString("File", "FilePrefix", "Img");
	BeginFromEdit->Text = Ini->ReadString("File", "BeginFrom", "0");
	SkipFrameCheckBox->Checked = StrToInt(Ini->ReadString("File", "SkipFrame", "1"));

	CheckBoxDateTime->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckDateTime", "1")));
	CheckBoxCurrentTime->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "CheckCurrentTime", "1")));
	CheckBoxResultOnly->Checked = (int)(StrToInt(Ini->ReadString("CheckBox", "ResultOnly", "1")));
	CheckBoxOnlySummary->Checked =  (int)StrToInt(Ini->ReadString("CheckBox", "PlotStarGraphs", "0"));

//настройки FormGraphOrient
	TFormGraphOrient* FormGraphOrient =  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->ContrastCheckBox->Checked    = (int)(StrToInt(Ini->ReadString("DrawFrag", "CheckContrast", "0")));
	FormGraphOrient->PixelBrightCheckBox->Checked = (int)(StrToInt(Ini->ReadString("DrawFrag", "CheckPixelBright", "0")));
	FormGraphOrient->ScaleEdit->Text     = Ini->ReadString("DrawFrag", "FragScale", "25");
	FormGraphOrient->PixelSizeEdit->Text = Ini->ReadString("DrawFrag", "PixelSize", "25");
	FormGraphOrient->FontSizeEdit->Text  = Ini->ReadString("DrawFrag", "FontSize",  "10");
}
//---------------------------------------------------------------------------
void TFormAnimateSetting::WriteINI(const AnsiString& fileName)
{
	std::unique_ptr<TIniFile> Ini ( new TIniFile(fileName));

	Ini->WriteString("CheckBox", "CheckFillTableObjects",   IntToStr((int)CheckBoxFillTableObjects->Checked));
	Ini->WriteString("CheckBox", "CheckApplyObjectsSeries", IntToStr((int)CheckBoxApplyObjectsSeries->Checked));
	Ini->WriteString("Colors", "LocObjTable", IntToStr(ShapeColorLocObjTable->Brush->Color));
	Ini->WriteString("Colors", "DetObjTable", IntToStr(ShapeColorDetObjTable->Brush->Color));

	Ini->WriteString("CheckBox", "CheckFillTableWindows",   IntToStr((int)CheckBoxFillTableWindows->Checked));
	Ini->WriteString("CheckBox", "CheckApplyWindowsSeries", IntToStr((int)CheckBoxApplyWindowsSeries->Checked));
	Ini->WriteString("Colors", "ZeroObjTable", IntToStr(ShapeColorZeroObjTable->Brush->Color));
	Ini->WriteString("Colors", "OneObjTable", IntToStr(ShapeColorOneObjTable->Brush->Color));
	Ini->WriteString("Colors", "TwoObjTable", IntToStr(ShapeColorTwoObjTable->Brush->Color));
	Ini->WriteString("Colors", "ThreeObjTable", IntToStr(ShapeColorThreeObjTable->Brush->Color));

	Ini->WriteString("File", "FilePrefix", EditFilePrefix->Text);
	Ini->WriteString("File", "BeginFrom",  BeginFromEdit->Text);
	Ini->WriteString("File", "SkipFrame",  IntToStr((int)CheckBoxCurrentTime->Checked));

	Ini->WriteString("CheckBox", "DateTime", IntToStr((int)CheckBoxDateTime->Checked));
	Ini->WriteString("CheckBox", "CheckCurrentTime", IntToStr((int)CheckBoxCurrentTime->Checked));
	Ini->WriteString("CheckBox", "ResultOnly", IntToStr((int)CheckBoxResultOnly->Checked));
	Ini->WriteString("CheckBox", "PlotStarGraphs", IntToStr((int)CheckBoxOnlySummary->Checked));

//настройки FormGraphOrient
	TFormGraphOrient* FormGraphOrient = dynamic_cast<TFormGraphOrient*>(this->Owner);
	Ini->WriteString("DrawFrag", "CheckContrast", IntToStr((int)FormGraphOrient->ContrastCheckBox->Checked));
	Ini->WriteString("DrawFrag", "CheckPixelBright", IntToStr((int)FormGraphOrient->PixelBrightCheckBox->Checked));
	Ini->WriteString("DrawFrag", "FragScale", FormGraphOrient->ScaleEdit->Text);
	Ini->WriteString("DrawFrag", "PixelSize", FormGraphOrient->PixelSizeEdit->Text);
	Ini->WriteString("DrawFrag", "FontSize",  FormGraphOrient->FontSizeEdit->Text);


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
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->SetVisibleLabelFrame(CheckBoxLabelFrame->Checked);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxApplyWindowsSeriesClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
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




