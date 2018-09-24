// ---------------------------------------------------------------------------

#ifndef UnitGraphOrientH
#define UnitGraphOrientH

// ---------------------------------------------------------------------------
#include "..\\Word\\WordLibOffice.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <VCLTee.Chart.hpp>
#include <ExtCtrls.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include "VCLTee.ArrowCha.hpp"
#include "VCLTee.BubbleCh.hpp"
#include "VCLTee.GanttCh.hpp"
#include "VCLTee.TeeShape.hpp"
#include <VclTee.TeeGDIPlus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.WinXCtrls.hpp>
#include "VCLTee.TeeTools.hpp"
#include "VCLTee.TeeEdit.hpp"
#include "VCLTee.ErrorBar.hpp"
#include "VCLTee.TeeHighLowLine.hpp"
#include <vector>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "StatLib.h"
#include "ReadFlashFormat.h"
#include <limits.h>
#include <ComCtrls.hpp>
#include <IniFiles.hpp>
#include "..\\UnitMain.h"
#include "UnitAnimateSetting.h"
#include <vcl.h>
#include <SysUtils.hpp>
#include <System.StrUtils.hpp>
#include <System.Uitypes.hpp>
#include <stdio.h>
#include <memory>
#include "ImageProcessingFcns.h"
#include <exception>
#include <unordered_map>
#include <map>
#include <limits>
#include <sstream>
#include <iterator>
#include <algorithm>
#include "FragmentScrollBox.h"
#include "SimplePlotter.h"
#include "ParseProtocol.h"
#include "DbClient.h"


#define MaxBlockSeries 8
#define MaxFrameSeries 30

// ---------------------------------------------------------------------------

struct RawFileInfo {
	long int Pos;
	int Size;
	int SecuenceCounter;
};

struct StatusInfo {
	float NoOneCount;
	float NoFourCount;
};

class TFormAnimateSetting;

class TFormGraphOrient : public TForm {
__published: // IDE-managed Components
	TButton *ButtonAdd;
	TButton *ButtonClear;
	TMainMenu *MainMenu1;
	TMenuItem *MenuSave;
	TMenuItem *MenuClear;
	TEdit *EditSizeX;
	TEdit *EditSizeY;
	TLabel *Label10;
	TEdit *EditTimeDev;
	TLabel *Label11;
	TEdit *EditNumCadr;
	TUpDown *UpDown1;
	TMenuItem *MenuOptions;
	TMenuItem *MenuOpen;
	TOpenDialog *OpenDialog;
	TCheckBox *SortFileCheckBox;
	TMenuItem *MenuOpenFlash;
	TMenuItem *MenuOpenSamspace;
	TMenuItem *N1;
	TMenuItem *BOKZ60ParseProtocol;
	TMenuItem *BOKZM2VParseProtocol;
	TMenuItem *MenuOpenEnregy;
	TLabel *LabelFrameReport;
	TMenuItem *ReadIKIFormat;
	TChartEditor *ChartEditor1;
	TFileOpenDialog *FileOpenDialog1;
	TPageControl *Fyf;
	TTabSheet *TabSheetAngles;
	TChart *ChartAl;
	TChart *ChartDl;
	TChart *ChartAz;
	TTabSheet *TabSheetAnglesError;
	TChart *ChartAzError;
	TChart *ChartDlError;
	TChart *ChartAlError;
	TTabSheet *TabSheetOmega;
	TChart *ChartWx;
	TChart *ChartWy;
	TChart *ChartWz;
	TTabSheet *TabSheetOmegaError;
	TChart *ChartWxError;
	TChart *ChartWyError;
	TChart *ChartWzError;
	TTabSheet *TabSheetMxy;
	TChart *ChartMx;
	TChart *ChartMy;
	TChart *ChartMxy;
	TTabSheet *TabSheetCountObjects;
	TChart *ChartNumFrag;
	TChart *ChartNumLoc;
	TChart *ChartNumDet;
	TTabSheet *TabSheetImage;
	TChart *ChartFone;
	TChart *ChartNoise;
	TChart *ChartTemp;
	TTabSheet *TabSheetStarsMotion;
	TLabel *Label4;
	TLabel *Label3;
	TLabel *Label2;
	TLabel *Label1;
	TLabel *Label5;
	TLabel *Label6;
	TChart *ChartMatrix;
	TBubbleSeries *Series9;
	TArrowSeries *Series3;
	TLineSeries *Series1;
	TTabSheet *TabSheetTableFrag;
	TChart *Chart2;
	TBarSeries *Series4;
	TChart *Chart3;
	TBarSeries *Series6;
	TChart *Chart4;
	TBarSeries *Series5;
	TStringGrid *TableWindowsInfo;
	TTabSheet *TabSheetTableObject;
	TChart *Chart5;
	TBarSeries *Series7;
	TChart *Chart6;
	TBarSeries *Series8;
	TStringGrid *TableObjectsInfo;
	TTabSheet *ShowFragmentTab;
	TScrollBox *FragmentShowScrollBox;
	TGroupBox *GroupBox1;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label7;
	TCheckBox *ContrastCheckBox;
	TEdit *ScaleEdit;
	TUpDown *UpDown2;
	TCheckBox *PixelBrightCheckBox;
	TEdit *PixelSizeEdit;
	TUpDown *UpDown3;
	TEdit *FontSizeEdit;
	TUpDown *UpDown4;
	TTabSheet *TabSheetStatFrag;
	TChart *ChartFragErrX;
	TChart *ChartFragErrY;
	TTabSheet *TabSheetAxesError;
	TChart *ChartErrorOX;
	TChart *ChartErrorOY;
	TChart *ChartErrorOZ;
	TChart *ChartFragBright;
	TChart *ChartFragSizeEl;
	TArrowSeries *Series10;
	TMenuItem *BOKZM601000ParseProtocol;
	TTabSheet *TabSheetStatStars;
	TChart *ChartBrightMv;
	TChart *ChartBrightSize;
	TChart *ChartBrightSp;
	TChart *ChartSizeMv;
	TTabSheet *TabSheetStatFrag2;
	TChart *ChartFragMean;
	TChart *ChartFragNoise;
	TChart *ChartFragLevel;
	TTabSheet *TabSheetTableStat;
	TEdit *EditCountHistory;
	TCheckBox *CheckBoxHistory;
	TEdit *EditScale;
	TLabel *Label12;
	TStringGrid *TableStatInfo;
	TCheckBox *CheckBoxSaveScale;
	TCheckBox *CheckBoxLimit;
	TMenuItem *BOKZMFParseProtocol;
	TStringGrid *TableStatusInfo;
	TMenuItem *BOKZM2ParseProtocol;
	TMenuItem *N21;
	TEdit *RatioEdit;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TMenuItem *MenuOpenArsenal;
	TMenuItem *BOKZMParse;
	TPopupMenu *PopupMenu1;
	TMenuItem *SaveSeriesData;

	void __fastcall MenuSaveClick(TObject *Sender);
	void __fastcall MenuClearClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TableObjectsInfoDrawCell(TObject *Sender, int ACol,
		int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall TableWindowsInfoDrawCell(TObject *Sender, int ACol,
		int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall MenuOptionsClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FragmentShowScrollBoxResize(TObject *Sender);
	void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift,
		const TPoint& MousePos, bool &Handled);
	void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift,
		const TPoint& MousePos, bool &Handled);
	void __fastcall MenuOpenFlashClick(TObject *Sender);
	void __fastcall MenuOpenSamspaceClick(TObject *Sender);
	void __fastcall EditNumCadrChange(TObject *Sender);
	void __fastcall BOKZ60ParseProtocolClick(TObject *Sender);
	void __fastcall BOKZM2VParseProtocolClick(TObject *Sender);
	void __fastcall MenuOpenEnergyClick(TObject *Sender);
	void __fastcall ScaleEditChange(TObject *Sender);
	void __fastcall PixelBrightCheckBoxClick(TObject *Sender);
	void __fastcall PixelSizeEditChange(TObject *Sender);
	void __fastcall FontSizeEditChange(TObject *Sender);
	void __fastcall ReadIKIFormatClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ChartsFragClickLegend(TCustomChart *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ChartOrientClickLegend(TCustomChart *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall BOKZM601000ParseProtocolClick(TObject *Sender);
	void __fastcall ChartFrag2ClickLegend(TCustomChart *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ChartMatrixClickLegend(TCustomChart *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall CheckBoxLimitClick(TObject *Sender);
	void __fastcall BOKZMFParseProtocolClick(TObject *Sender);
	void __fastcall N21Click(TObject *Sender);
	void __fastcall BOKZM2ParseProtocolClick(TObject *Sender);
	void __fastcall MenuOpenArsenalClick(TObject *Sender);
	void __fastcall BOKZMParseClick(TObject *Sender);
	void __fastcall SaveSeriesDataClick(TObject *Sender);


private: // User declarations

	class Handle {
	public:
		_fastcall Handle(TFormGraphOrient* _Form) : Form(_Form) {
		}
		virtual void operator()(CadrInfo& cadrInfo, TColor pointColor =
			clBlue) = 0;

	protected:
		TFormGraphOrient* Form;
	};

	class HandleLoc60 : public Handle {
	public:
		_fastcall HandleLoc60(TFormGraphOrient* _Form) : Handle(_Form) {
		}

		void operator()(CadrInfo& cadrInfo, TColor pointColor = clBlue) {
			Form->plotter->AddPoint(Form->ChartNumLoc, 0, cadrInfo.Time,
				cadrInfo.CountLocalObj);
			Form->plotter->AddPoint(Form->ChartNumDet, 0, cadrInfo.Time,
				cadrInfo.CountDeterObj);
		}

	};

	class Handle60 : public Handle {
	public:
		_fastcall Handle60(TFormGraphOrient* _Form) : Handle(_Form) {
		}

		void operator()(CadrInfo& cadrInfo, TColor pointColor = clBlue) {

			Form->plotter->AddPoint(Form->ChartAl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[0] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartDl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[1] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartAz, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[2] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartNumLoc, 0, cadrInfo.Time,
				cadrInfo.CountLocalObj, pointColor);
			Form->plotter->AddPoint(Form->ChartNumDet, 0, cadrInfo.Time,
				cadrInfo.CountDeterObj, pointColor);
			Form->plotter->AddPoint(Form->ChartWx, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[0] * RTM, pointColor);
			Form->plotter->AddPoint(Form->ChartWy, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[1] * RTM, pointColor);
			Form->plotter->AddPoint(Form->ChartWz, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[2] * RTM, pointColor);
			Form->plotter->AddPoint(Form->ChartNumFrag, 0, cadrInfo.Time,
				cadrInfo.CountWindows);
		}

	};

		class HandleM : public Handle {
	public:
		_fastcall HandleM(TFormGraphOrient* _Form) : Handle(_Form) {
		}

		void operator()(CadrInfo& cadrInfo, TColor pointColor = clBlue) {

			Form->plotter->AddPoint(Form->ChartAl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[0] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartDl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[1] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartAz, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[2] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartNumLoc, 0, cadrInfo.Time,
				cadrInfo.CountLocalObj, pointColor);
			Form->plotter->AddPoint(Form->ChartNumDet, 0, cadrInfo.Time,
				cadrInfo.CountDeterObj, pointColor);
			Form->plotter->AddPoint(Form->ChartNumFrag, 0, cadrInfo.Time,
				cadrInfo.CountWindows);
		}

	};

	class HandleM2V : public Handle {

	public:
		_fastcall HandleM2V(TFormGraphOrient* _Form) : Handle(_Form) {
		}

		void operator()(CadrInfo& cadrInfo, TColor pointColor = clBlue) {

			Form->plotter->AddPoint(Form->ChartAl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[0] * RTD);
			Form->plotter->AddPoint(Form->ChartDl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[1] * RTD);
			Form->plotter->AddPoint(Form->ChartAz, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[2] * RTD);
			Form->plotter->AddPoint(Form->ChartWx, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[0] * RTM);
			Form->plotter->AddPoint(Form->ChartWy, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[1] * RTM);
			Form->plotter->AddPoint(Form->ChartWz, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[2] * RTM);
			Form->plotter->AddPoint(Form->ChartNumFrag, 0, cadrInfo.Time,
				cadrInfo.CountWindows);
			Form->plotter->AddPoint(Form->ChartNumLoc, 0, cadrInfo.Time,
				cadrInfo.CountLocalObj);
			Form->plotter->AddPoint(Form->ChartNumDet, 0, cadrInfo.Time,
				cadrInfo.CountDeterObj);
			Form->plotter->AddPoint(Form->ChartMxy, 0, cadrInfo.Time,
				cadrInfo.MeanErrorXY);
			Form->plotter->AddPoint(Form->ChartTemp, 0, cadrInfo.Time,
				cadrInfo.MatrixTemp);
			Form->plotter->AddPoint(Form->ChartFone, 0, cadrInfo.Time,
				cadrInfo.MeanBright);
			Form->plotter->AddPoint(Form->ChartNoise, 0, cadrInfo.Time,
				cadrInfo.SigmaBright);
		}

	};

	class HandleM2 : public Handle {

	public:
		_fastcall HandleM2(TFormGraphOrient* _Form) : Handle(_Form) {
		}

		void operator()(CadrInfo& cadrInfo, TColor pointColor = clBlue) {

			Form->plotter->AddPoint(Form->ChartAl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[0] * RTD);
			Form->plotter->AddPoint(Form->ChartDl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[1] * RTD);
			Form->plotter->AddPoint(Form->ChartAz, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[2] * RTD);
			Form->plotter->AddPoint(Form->ChartWx, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[0] / 60);
			Form->plotter->AddPoint(Form->ChartWy, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[1] / 60);
			Form->plotter->AddPoint(Form->ChartWz, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[2] / 60);
			Form->plotter->AddPoint(Form->ChartNumFrag, 0, cadrInfo.Time,
				cadrInfo.CountWindows);
			Form->plotter->AddPoint(Form->ChartNumLoc, 0, cadrInfo.Time,
				cadrInfo.CountLocalObj);
			Form->plotter->AddPoint(Form->ChartNumDet, 0, cadrInfo.Time,
				cadrInfo.CountDeterObj);
			Form->plotter->AddPoint(Form->ChartMxy, 0, cadrInfo.Time,
				cadrInfo.MeanErrorXY);
		}

	};

	class Handle1000 {

	public:
		_fastcall Handle1000(TFormGraphOrient* _Form) : Form(_Form) {
		}

		void operator()(CadrInfo& cadrInfo, TColor pointColor) {

			Form->plotter->AddPoint(Form->ChartAl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[0] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartDl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[1] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartAz, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[2] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartNumFrag, 0, cadrInfo.Time,
				cadrInfo.CountWindows, pointColor);
			Form->plotter->AddPoint(Form->ChartNumDet, 0, cadrInfo.Time,
				cadrInfo.CountDeterObj, pointColor);
			Form->plotter->AddPoint(Form->ChartNumLoc, 0, cadrInfo.Time,
				cadrInfo.CountLocalObj, pointColor);
			Form->plotter->AddPoint(Form->ChartWx, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[0] * RTM /** BOKZ1000ConvCoef*/, pointColor);
			Form->plotter->AddPoint(Form->ChartWy, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[1] * RTM /** BOKZ1000ConvCoef*/, pointColor);
			Form->plotter->AddPoint(Form->ChartWz, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[2] * RTM /** BOKZ1000ConvCoef*/, pointColor);
//			for (int i = 0; i < cadrInfo.ObjectsList.size(); i++) {
//
//			}
		}

	protected:
		TFormGraphOrient* Form;

	};

	class HandleMF : public Handle1000 {
	public:
		_fastcall HandleMF(TFormGraphOrient* _Form) : Handle1000(_Form) {
		}

		void operator()(CadrInfo& cadrInfo, TColor pointColor) {

			Form->plotter->AddPoint(Form->ChartAl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[0] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartDl, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[1] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartAz, 0, cadrInfo.Time,
				cadrInfo.AnglesOrient[2] * RTD, pointColor);
			Form->plotter->AddPoint(Form->ChartNumFrag, 0, cadrInfo.Time,
				cadrInfo.CountWindows, pointColor);
			Form->plotter->AddPoint(Form->ChartNumDet, 0, cadrInfo.Time,
				cadrInfo.CountDeterObj, pointColor);
			Form->plotter->AddPoint(Form->ChartNumLoc, 0, cadrInfo.Time,
				cadrInfo.CountLocalObj, pointColor);
			Form->plotter->AddPoint(Form->ChartWx, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[0] / 60, pointColor);
			Form->plotter->AddPoint(Form->ChartWy, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[1] / 60, pointColor);
			Form->plotter->AddPoint(Form->ChartWz, 0, cadrInfo.Time,
				cadrInfo.OmegaOrient[2] / 60, pointColor);
		}
	};

	void PrepareStartDraw();
	void CheckTabSheet();
	void InitTableObjects(void);
	void PrintTableObjects(const struct CadrInfo &mCadr);
	void InitTableWindows(void);
	void PrintTableWindows(const struct CadrInfo &mCadr);
	void InitTableStat();
	void InitStatusInfoTable(const string& deviceName);
	void ClearStatusInfoTable();
	void AddRowToStatTable(int nRow, AnsiString stringName, Statistika _stat,
		int p1, int p2);
	void AddRowToStatusTable(const CadrInfo& cadr);
	void ClearAnimate(void);
	void SaveScale();
	void ApplyScale();
	void SetDefaultScale(const struct CadrInfo &mCadr);
	void DrawAnimate(const struct CadrInfo &mCadr);
	void DrawObjects(const struct CadrInfo &mCadr);
	void DrawBlock(const struct CadrInfo &mCadr);
	void DrawFragment(const struct CadrInfo &mCadr);
	void PlaceImageFragments(const vector<FragmentScrollBox*>& FragmentImages);
	AnsiString SortRawFlashFile(const AnsiString &RawFileName);
	void __fastcall ImageOnClick(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void resetFragmentShowScrollBox();
	void __fastcall OnScroll(TObject* Sender);
	void SetContrast();
	void __fastcall ChartMouseWheel(TObject *Sender, TShiftState Shift,
		int WheelDelta, const TPoint &MousePos, bool &Handled);
	void __fastcall ChartMouseDown(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y);


	void OutputDTMI(ofstream &_fout, AnsiString &_SaveDir, DTMI &_tmi, TDateTime zeroDate);
	void OutputLOC(ofstream &_fout, AnsiString &_SaveDir, LOC &_tmi, TDateTime zeroDate);
	void StartPrintReport(IKI_img* reader);
	void PrintReportRes(vector<CadrInfo>& cadrInfo);
	void CalculateSeriesSKO();
	void SaveTableToFile(TStringGrid* table, short rowCount, short columnCount,
		string filename);
	void FillStatusTable();
	UnicodeString SaveScreenShots(UnicodeString folder = "");

	void DeleteLineGraph();
	void InitializeSynchronization();
	void SynchronizeCharts(double Value);
	void SaveGraph(TChart *Chart, AnsiString suff);
	int GetCadrInfo(int NC, struct CadrInfo &mCadr);

	void ResizePlot(TChart *chart, double kx, double ky, int indexX,
		int indexY);

	unique_ptr<TFormAnimateSetting>FormAnimateSetting;
	TChartShape *BlockSeries[MaxBlockSeries];
	TChartShape *FrameSeries[MaxFrameSeries];

	AnsiString FileTitle;
	AnsiString CurDir;

	std::unique_ptr<SimplePlotter>plotter;
	std::unique_ptr<FragmentPainter>fPainter;
	std::unique_ptr<DbClient> clientDb;

	float ScaleFactorForScrollBox;
	int ScaleFactorForImage;
	int Contrast;
	unsigned short ResizeCoef;
	unsigned short FontSize;
	StatusInfo statusInfo;

	vector <unsigned short> tableRows;
	vector <string> columnTitles;

	vector <TChart*> Charts;
	vector <CadrInfo> vCadrInfo;
	vector <TImage*> ImageVector;
	vector <FragmentData> FragmentVector;
	vector <FragmentScrollBox*> ImageScrollBoxVector;
	vector <TImage*> FragmentsNumbers;

public: // User declarations

	__fastcall TFormGraphOrient(TComponent* Owner);
	void PrintTableWindowsHandler(void);
	void PrintTableObjectsHandler(void);
	void DrawAnimateHandler(void);
	void SetVisibleLabelFrame(bool isVisible);
	void SetVisibleLabelStar(bool isVisible);
    void SetVisibleLabelObject(bool isVisible);
	void PrepareChartFrag(TColor *_colorFrag, const int _maxDrawFrag);
	void DrawChartFrag(const TColor *_colorFrag, const int _maxDrawFrag, CadrInfo& _cadrInfo);

	AnsiString SourceDir;

};

// ---------------------------------------------------------------------------
extern PACKAGE TFormGraphOrient *FormGraphOrient;
// ---------------------------------------------------------------------------
#endif
