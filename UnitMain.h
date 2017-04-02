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

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *MenuLook;
	void __fastcall MenuLookClick(TObject *Sender);
private:	// User declarations


public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
