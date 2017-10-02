//---------------------------------------------------------------------------

#ifndef UnitMainH
#define UnitMainH
#include <System.Classes.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
	#include <Classes.hpp>
	#include <Controls.hpp>
	#include <StdCtrls.hpp>
	#include <Forms.hpp>
	#include <Menus.hpp>
	#include <Dialogs.hpp>
	#include <vcl.h>
	#include "MathM.h"
	#include "StatLib.h"
	#include "InfoCadrFormat.h"
	#include "ReadFlashFormat.h"
	#include <limits.h>
	#include <ComCtrls.hpp>
	#include "UnitGraphOrient.h"
	#include "UnitAnimateSetting.h"
	#include <Vcl.WinXCtrls.hpp>
	#include "Analyze.h"
	#include <Data.DB.hpp>
	#include <Data.Win.ADODB.hpp>

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *MenuLook;
	TMenuItem *AnalyzeLook;
	void __fastcall MenuLookClick(TObject *Sender);
	void __fastcall AnalyzeLookClick(TObject *Sender);
private:	// User declarations


public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
