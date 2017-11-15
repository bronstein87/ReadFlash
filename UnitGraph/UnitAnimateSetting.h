//---------------------------------------------------------------------------

#ifndef UnitAnimateSettingH
#define UnitAnimateSettingH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <memory>
#include <IniFiles.hpp>
#include "UnitGraphOrient.h"
//---------------------------------------------------------------------------


class TFormAnimateSetting : public TForm
{
__published:	// IDE-managed Components
	TColorDialog *ColorDialog1;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TGroupBox *GroupBox2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TShape *ShapeColorLocObjTable;
	TShape *ShapeColorDetObjTable;
	TShape *ShapeColorZeroObjTable;
	TShape *ShapeColorOneObjTable;
	TShape *ShapeColorTwoObjTable;
	TShape *ShapeColorThreeObjTable;
	TCheckBox *CheckBoxFillTableObjects;
	TCheckBox *CheckBoxApplyObjectsSeries;
	TCheckBox *CheckBoxFillTableWindows;
	TCheckBox *CheckBoxApplyWindowsSeries;
	TGroupBox *GroupBox3;
	TCheckBox *CheckBoxLabelFrame;
	TCheckBox *CheckBoxFrameMv;
	TCheckBox *CheckBoxFrameSp;
	TCheckBox *CheckBoxFrameStarID;
	TCheckBox *CheckBoxFrameID;
	TGroupBox *GroupBox4;
	TCheckBox *CheckBoxCurrentTime;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TGroupBox *GroupBox5;
	TLabel *Label7;
	TEdit *EditFilePrefix;
	TLabel *Label8;
	TEdit *BeginFromEdit;
	TCheckBox *CheckBoxDateTime;
	TCheckBox *SkipFrameCheckBox;
	void __fastcall ShapeColorLocObjTableMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ShapeColorZeroObjTableMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ShapeColorOneObjTableMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ShapeColorTwoObjTableMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ShapeColorThreeObjTableMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ShapeColorDetObjTableMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall CheckBoxApplyObjectsSeriesClick(TObject *Sender);
	void __fastcall CheckBoxApplyWindowsSeriesClick(TObject *Sender);
	void __fastcall CheckBoxFillTableObjectsClick(TObject *Sender);
	void __fastcall CheckBoxFillTableWindowsClick(TObject *Sender);
	void __fastcall CheckBoxLabelFrameClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TFormAnimateSetting(TComponent* Owner);
	void ReadINI(const AnsiString& fileName);
	void WriteINI(const AnsiString& fileName);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAnimateSetting *FormAnimateSetting;
//---------------------------------------------------------------------------
#endif
