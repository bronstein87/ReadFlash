//---------------------------------------------------------------------------

#ifndef AnalyzeH
#define AnalyzeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include "VCLTee.TeeHighLowLine.hpp"
#include <Vcl.Dialogs.hpp>
#include <Vcl.ToolWin.hpp>
#include <vector>
#include "SimplePlotter.h"
#include <Vcl.Dialogs.hpp>
#include <memory>
#include <iterator>
#include <algorithm>
#include "iki_img.cpp"
#include <System.IOUtils.hpp>
#include <SysUtils.hpp>
#include <System.StrUtils.hpp>
#include <System.Uitypes.hpp>
#include "MathM.h"
#include <exception>
#include <string>
#include <utility>
#include "AddString.h"
//---------------------------------------------------------------------------
using namespace std;
class TAnalyzeForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *ChooseDirectories;
	TPageControl *PageControl1;
	TTabSheet *TabSheet2;
	TMenuItem *SaveGraphs;
	TMenuItem *ClearGraphs;
	TChart *ChartAnalyzeXV;
	TChart *ChartAnalyzeYV;
	TChart *ChartAnalyzeZV;
	TFileOpenDialog *FileOpenDialog1;
	TTabSheet *TabSheet1;
	TChart *ChartAnalyzeErrorAl;
	TChart *ChartAnalyzeErrorDl;
	TChart *ChartAnalyzeErrorAz;
	TTabSheet *TabSheet3;
	TChart *Chart1;
	TChart *Chart2;
	TChart *Chart3;
	void __fastcall ChooseDirectoriesClick(TObject *Sender);
	void __fastcall SaveGraphsClick(TObject *Sender);
	void __fastcall ClearGraphsClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
	vector <TChart*> Charts;
	std::unique_ptr <SimplePlotter> plotter;
    void plotStatistics (vector <Point>& data, THighLowSeries* minMax, THighLowSeries* sko, TPointSeries* mean, double coef);
public:		// User declarations
	__fastcall TAnalyzeForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAnalyzeForm *AnalyzeForm;
//---------------------------------------------------------------------------
#endif
