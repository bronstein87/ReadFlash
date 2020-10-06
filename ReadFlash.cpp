// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <SysUtils.hpp>
// ---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("UnitMain.cpp", MainForm);
USEFORM("UnitGraph\Analyze.cpp", AnalyzeForm);
USEFORM("UnitGraph\UnitGraphOrient.cpp", FormGraphOrient);
USEFORM("UnitGraph\UnitAnimateSetting.cpp", FormAnimateSetting);
USEFORM("UnitGraph\UnitDevice.cpp", FormDevice);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {

	try {
		FormatSettings.DecimalSeparator = '.';
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TFormDevice), &FormDevice);
		Application->Run();
	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (...) {
		try {
			throw Exception("");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}
	return 0;
}
// ---------------------------------------------------------------------------
