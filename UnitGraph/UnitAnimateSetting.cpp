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
void __fastcall TFormAnimateSetting::FormCreate(TObject *Sender)
{
	AnsiString SourceDir;
	SourceDir=GetCurrentDir();
	ReadINI(SourceDir+"\\options.ini");
}
//---------------------------------------------------------------------------
void __fastcall TFormAnimateSetting::FormClose(TObject *Sender, TCloseAction &Action)
{
	AnsiString SourceDir;
	SourceDir=GetCurrentDir();
	WriteINI(SourceDir+"\\options.ini");
}
//---------------------------------------------------------------------------
void TFormAnimateSetting::ReadINI(const AnsiString& fileName)
{
	std::unique_ptr<TIniFile> Ini ( new TIniFile(fileName));

	CheckBoxFillTableObjects->Checked=TColor(StrToInt(Ini->ReadString("CheckBox","CheckFillTableObjects","0")));
	CheckBoxApplyObjectsSeries->Checked=TColor(StrToInt(Ini->ReadString("CheckBox","CheckApplyObjectsSeries","0")));
	ShapeColorLocObjTable->Brush->Color=TColor(StrToInt(Ini->ReadString("Colors", "LocObjTable", clWhite)));
	ShapeColorDetObjTable->Brush->Color=TColor(StrToInt(Ini->ReadString("Colors", "DetObjTable", clWhite)));

	CheckBoxFillTableWindows->Checked=TColor(StrToInt(Ini->ReadString("CheckBox","CheckFillTableWindows","0")));
	CheckBoxApplyObjectsSeries->Checked=TColor(StrToInt(Ini->ReadString("CheckBox","CheckApplyObjectsSeries","0")));
	ShapeColorZeroObjTable->Brush->Color=TColor(StrToInt(Ini->ReadString("Colors", "ZeroObjTable", clWhite)));
	ShapeColorOneObjTable->Brush->Color=TColor(StrToInt(Ini->ReadString("Colors", "OneObjTable", clWhite)));
	ShapeColorTwoObjTable->Brush->Color=TColor(StrToInt(Ini->ReadString("Colors", "TwoObjTable", clWhite)));
	ShapeColorThreeObjTable->Brush->Color=TColor(StrToInt(Ini->ReadString("Colors", "ThreeObjTable", clWhite)));

}
//---------------------------------------------------------------------------
void TFormAnimateSetting::WriteINI(const AnsiString& fileName)
{
	std::unique_ptr<TIniFile> Ini ( new TIniFile(fileName));

	Ini->WriteString("CheckBox","CheckFillTableObjects",   IntToStr((int)CheckBoxFillTableObjects->Checked));
	Ini->WriteString("CheckBox","CheckApplyObjectsSeries", IntToStr((int)CheckBoxApplyObjectsSeries->Checked));
	Ini->WriteString("Colors", "LocObjTable", IntToStr(ShapeColorLocObjTable->Brush->Color));
	Ini->WriteString("Colors", "DetObjTable", IntToStr(ShapeColorDetObjTable->Brush->Color));

	Ini->WriteString("CheckBox", "CheckFillTableWindows",   IntToStr((int)CheckBoxFillTableWindows->Checked));
	Ini->WriteString("CheckBox", "CheckApplyWindowsSeries", IntToStr((int)CheckBoxApplyWindowsSeries->Checked));
	Ini->WriteString("Colors", "ZeroObjTable", IntToStr(ShapeColorZeroObjTable->Brush->Color));
	Ini->WriteString("Colors", "OneObjTable", IntToStr(ShapeColorOneObjTable->Brush->Color));
	Ini->WriteString("Colors", "TwoObjTable", IntToStr(ShapeColorTwoObjTable->Brush->Color));
	Ini->WriteString("Colors", "ThreeObjTable", IntToStr(ShapeColorThreeObjTable->Brush->Color));

}
//---------------------------------------------------------------------------
void __fastcall TFormAnimateSetting::ShapeColorLocObjTableMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorLocObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->DrawAnimateHandler();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormAnimateSetting::ShapeColorDetObjTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorDetObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->DrawAnimateHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::ShapeColorZeroObjTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorZeroObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->DrawAnimateHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::ShapeColorOneObjTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorOneObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->DrawAnimateHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::ShapeColorTwoObjTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorTwoObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->DrawAnimateHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::ShapeColorThreeObjTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (ColorDialog1->Execute()) {
		ShapeColorThreeObjTable->Brush->Color=ColorDialog1->Color;
		TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
		FormGraphOrient->DrawAnimateHandler();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxApplyObjectsSeriesClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxApplyWindowsSeriesClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxFillTableObjectsClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxFillTableWindowsClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->DrawAnimateHandler();
}
//---------------------------------------------------------------------------

void __fastcall TFormAnimateSetting::CheckBoxLabelFrameClick(TObject *Sender)
{
	TFormGraphOrient* FormGraphOrient=  dynamic_cast<TFormGraphOrient*>(this->Owner);
	FormGraphOrient->SetVisibleLabelFrame(CheckBoxLabelFrame->Checked);
}
//---------------------------------------------------------------------------


