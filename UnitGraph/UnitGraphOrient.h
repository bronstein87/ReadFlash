	//---------------------------------------------------------------------------

	#ifndef UnitGraphOrientH
	#define UnitGraphOrientH


	//---------------------------------------------------------------------------
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
	#include "frxRich.hpp"
	#include "frxClass.hpp"
	#include "frxExportRTF.hpp"
	#include <vector>
	#include <Classes.hpp>
	#include <Controls.hpp>
	#include <StdCtrls.hpp>
	#include <Forms.hpp>
	#include <Menus.hpp>
	#include <Dialogs.hpp>
	#include "MathM.h"
	#include "StatLib.h"
	#include "InfoCadrFormat.h"
	#include "ReadFlashFormat.h"
	#include <limits.h>
	#include <ComCtrls.hpp>
	#include <IniFiles.hpp>
	#include "..\\UnitMain.h"
	#include "UnitAnimateSetting.h"
	#include <vcl.h>
	#include <System.IOUtils.hpp>
	#include <SysUtils.hpp>
	#include <DateUtils.hpp>
	#include <System.StrUtils.hpp>
	#include <System.Uitypes.hpp>
	#include <stdio.h>
	#include <memory>
	#include <fstream>
	#include <iomanip>
	#include "ImageProcessingFcns.h"
	#include <exception>
	#include <unordered_map>
	#include <limits>
	#include <sstream>
	#include <iterator>
	#include <algorithm>
	#include "FragmentScrollBox.h"
	#include "iki_img.cpp"
	#include "SimplePlotter.h"


	#define MaxBlockSeries 8
	#define MaxFrameSeries 30

	//---------------------------------------------------------------------------

	struct RawFileInfo
	{
		long int Pos;
		int Size;
		int SecuenceCounter;
	};


	class TFormAnimateSetting;

	class TFormGraphOrient : public TForm
	{
	__published:	// IDE-managed Components
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
		TMenuItem *MenuOpenTMI;
		TMenuItem *N1;
		TMenuItem *BOKZ60ParseProtocol;
		TMenuItem *BOKZM2VParseProtocol;
		TMenuItem *N2;
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
		TChart *Chart1;
		TBubbleSeries *Series9;
		TLineSeries *Series2;
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

		void __fastcall MenuSaveClick(TObject *Sender);
		void __fastcall MenuClearClick(TObject *Sender);
		void __fastcall FormCreate(TObject *Sender);
		void __fastcall TableObjectsInfoDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
			  TGridDrawState State);
		void __fastcall TableWindowsInfoDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
			  TGridDrawState State);
		void __fastcall MenuOptionsClick(TObject *Sender);
		void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
		void __fastcall FragmentShowScrollBoxResize(TObject *Sender);
		void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift,const TPoint& MousePos,
		  bool &Handled);
		void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift,const TPoint& MousePos,
		  bool &Handled);
		void __fastcall MenuOpenFlashClick(TObject *Sender);
		void __fastcall MenuOpenProgressTMIClick(TObject *Sender);
		void __fastcall EditNumCadrChange(TObject *Sender);
		void __fastcall BOKZ60ParseProtocolClick(TObject *Sender);
		void __fastcall BOKZM2VParseProtocolClick(TObject *Sender);
		void __fastcall MenuOpenEnergyTMIClick(TObject *Sender);
		void __fastcall ScaleEditChange(TObject *Sender);
		void __fastcall PixelBrightCheckBoxClick(TObject *Sender);
		void __fastcall PixelSizeEditChange(TObject *Sender);
		void __fastcall FontSizeEditChange(TObject *Sender);
		void __fastcall ReadIKIFormatClick(TObject *Sender);
		void __fastcall FormResize(TObject *Sender);
		void __fastcall ChartsFragClickLegend(TCustomChart *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
		void __fastcall ChartOrientClickLegend(TCustomChart *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall BOKZM601000ParseProtocolClick(TObject *Sender);


	private:	// User declarations
		void PrepareStartDraw();
        void CheckTabSheet();
		void InitTableObjects(void);
		void PrintTableObjects(const struct CadrInfo &mCadr);
		void InitTableWindows(void);
		void PrintTableWindows(const struct CadrInfo &mCadr);
		void ClearAnimate(void);
		void DrawAnimate(const struct CadrInfo &mCadr);
		void DrawBlock(const struct CadrInfo &mCadr);
		void DrawFragment(const struct CadrInfo &mCadr);
		void PlaceImageFragments (const vector <FragmentScrollBox*>& FragmentImages);
		AnsiString SortRawFlashFile(const AnsiString &RawFileName);
		void __fastcall ImageOnClick(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y);
		void resetFragmentShowScrollBox();
		void readBOKZ60Protocol(ifstream& in,vector <CadrInfo>& cadrInfoVec);
		void readBOKZ60LocProtocol(ifstream& in,vector <CadrInfo>& cadrInfoVec);
		void readmBOKZ2VProtocol(ifstream& in,vector <CadrInfo>& cadrInfoVec);
		void readBOKZ601000Protocol(ifstream& in,vector <CadrInfo>& cadrInfoVec, unsigned int& counter, TDateTime& startDate);
		void readBOKZ601000MKO(ifstream& in, vector <CadrInfo>& cadrInfoVec, unsigned int& counter);
		void __fastcall OnScroll(TObject* Sender);
		void SetContrast();
		void __fastcall ChartMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
			const TPoint &MousePos, bool &Handled);
		void __fastcall ChartMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
        void CalculateSeriesSKO();

		unique_ptr <TFormAnimateSetting> FormAnimateSetting;
		TChartShape *BlockSeries[MaxBlockSeries];
		TChartShape *FrameSeries[MaxFrameSeries];

		AnsiString FileName;
		AnsiString FileTitle;
		AnsiString CurDir;
		AnsiString FragDir;
		AnsiString LocDir;

		float ScaleFactorForScrollBox;
		int ScaleFactorForImage;
		int Contrast;
		unsigned short ResizeCoef;
		unsigned short FontSize;
		bool CompareIKIRes;

		std::unique_ptr <SimplePlotter> plotter;
		vector <TChart*> Charts;
		vector <CadrInfo> vCadrInfo;
		vector <TImage*> ImageVector;
		vector <FragmentData> FragmentVector;
		vector <FragmentScrollBox*> ImageScrollBoxVector;
		vector <TImage*> FragmentsNumbers;

	public:		// User declarations

		__fastcall TFormGraphOrient(TComponent* Owner);
		void DeleteLineGraph();
		void InitializeSynchronization();
		void SynchronizeCharts(double Value);
		void SaveGraph(TChart *Chart, AnsiString suff);
		void SetVisibleLabelFrame(bool isVisible);
		int  GetCadrInfo(int NC, struct CadrInfo &mCadr);
		void DrawAnimateHandler(void);
		void PrintTableWindowsHandler(void);
		void PrintTableObjectsHandler(void);
		void DrawBlockHandler(void);
        void ResizePlot(TChart *chart, double kx, double ky, int indexX, int indexY);
	};
	//---------------------------------------------------------------------------
	extern PACKAGE TFormGraphOrient *FormGraphOrient;
	//---------------------------------------------------------------------------
	#endif
