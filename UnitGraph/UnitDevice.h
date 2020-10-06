//---------------------------------------------------------------------------

#ifndef UnitDeviceH
#define UnitDeviceH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormDevice : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TComboBox *ComboBoxDeviceIndex;
	TPageControl *PageControl1;
	TTabSheet *TabSheetMshi;
	TTabSheet *TabSheetShtmi1;
	TTabSheet *TabSheetShtmi2;
	TTabSheet *TabSheetDtmi;
private:	// User declarations
public:		// User declarations
	__fastcall TFormDevice(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDevice *FormDevice;
//---------------------------------------------------------------------------
#endif
