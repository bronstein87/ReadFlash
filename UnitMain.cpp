// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UnitMain.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TMainForm *MainForm;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : client(new DbClient(NULL, FDGUIxLoginDialog1)), TForm(Owner)
{
	client->afterConnect(&afterDbConnect);
	AnsiString string = "";
	string.Insert("1", 2);
	int i = 1;
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::MenuLookClick(TObject *Sender) {
	TFormGraphOrient *FormGraphOrient = new TFormGraphOrient(this);
	FormGraphOrient->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::AnalyzeLookClick(TObject *Sender) {
	TAnalyzeForm * AnalyzeForm = new TAnalyzeForm(this);
	AnalyzeForm->Show();
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TMainForm::DbAutorizationClick(TObject *Sender) {
	client->connect();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FDConnection1Error(TObject *ASender,
	TObject *AInitiator, Exception *&AException) {
	ShowMessage(AException->ToString());
}
// ---------------------------------------------------------------------------

void _fastcall TMainForm::afterDbConnect(System::TObject* Sender)
{
	if (FormAnimateSetting != NULL)
	{
		FormAnimateSetting->updateKaNameList();
	}
}
