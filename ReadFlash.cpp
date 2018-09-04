// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <SysUtils.hpp>
// ---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("UnitGraph\Analyze.cpp", AnalyzeForm);
USEFORM("UnitGraph\UnitAnimateSetting.cpp", FormAnimateSetting);
USEFORM("UnitGraph\UnitGraphOrient.cpp", FormGraphOrient);
USEFORM("UnitMain.cpp", MainForm);

// ---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {

	try {
		FormatSettings.DecimalSeparator = '.';
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainForm), &MainForm);
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
