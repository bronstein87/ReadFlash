	//---------------------------------------------------------------------------


	#pragma hdrstop

	#include "UnitMain.h"
	//---------------------------------------------------------------------------
	#pragma package(smart_init)
	#pragma resource "*.dfm"


	TMainForm *MainForm;
	//---------------------------------------------------------------------------
	__fastcall TMainForm::TMainForm(TComponent* Owner)
		: TForm(Owner)
	{

	}



	//---------------------------------------------------------------------------

	void __fastcall  TMainForm::MenuLookClick(TObject *Sender)
	{
		TFormGraphOrient *FormGraphOrient= new  TFormGraphOrient(this);
		FormGraphOrient->Show();
	}
	//---------------------------------------------------------------------------



void __fastcall TMainForm::AnalyzeLookClick(TObject *Sender)
{
	TAnalyzeForm * AnalyzeForm = new TAnalyzeForm(this);
	AnalyzeForm->Show();
}
//---------------------------------------------------------------------------

