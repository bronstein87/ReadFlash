// ---------------------------------------------------------------------------

#ifndef UnitMainH
#define UnitMainH
#include <System.Classes.hpp>
#include <Vcl.Menus.hpp>
// ---------------------------------------------------------------------------
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
#include <FireDAC.VCLUI.Async.hpp>
#include <FireDAC.VCLUI.Error.hpp>
#include <FireDAC.DApt.hpp>
#include "DbClient.h"

// ---------------------------------------------------------------------------
class TMainForm : public TForm {
__published: // IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *MenuLook;
	TMenuItem *AnalyzeLook;
	TFDPhysPgDriverLink *FDPhysPgDriverLink1;
	TFDGUIxLoginDialog *FDGUIxLoginDialog1;
	TMenuItem *DbAutorization;
	TFDGUIxAsyncExecuteDialog *FDGUIxAsyncExecuteDialog1;
	TFDGUIxErrorDialog *FDGUIxErrorDialog1;

	void __fastcall MenuLookClick(TObject *Sender);
	void __fastcall AnalyzeLookClick(TObject *Sender);
	void __fastcall DbAutorizationClick(TObject *Sender);
	void __fastcall FDConnection1Error(TObject *ASender, TObject *AInitiator,
		Exception *&AException);


private: // User declarations

public: // User declarations
	__fastcall TMainForm(TComponent* Owner);
	std::unique_ptr <DbClient> client;
	void __fastcall afterDbConnect(TObject* Sender);

};

// ---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
// ---------------------------------------------------------------------------
#endif
