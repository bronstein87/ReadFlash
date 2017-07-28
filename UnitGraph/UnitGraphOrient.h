	//---------------------------------------------------------------------------

	#ifndef UnitGraphOrientH
	#define UnitGraphOrientH


	//---------------------------------------------------------------------------

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
	#include "iki_img_class.cpp"




	#define MaxSeries 6
	#define NumGraph 15
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
		TPageControl *PageControl2;
		TTabSheet *TabSheet5;
		TChart *ChartAl;
		TChart *ChartDl;
		TChart *ChartAz;
		TTabSheet *TabSheet6;
		TChart *ChartWx;
		TChart *ChartWy;
		TChart *ChartWz;
		TTabSheet *TabSheet7;
		TChart *ChartMx;
		TChart *ChartMy;
	TChart *ChartMxy;
		TTabSheet *TabSheet8;
	TChart *ChartNumFrag;
	TChart *ChartNumLoc;
	TChart *ChartNumDet;
		TTabSheet *TabSheet9;
		TChart *ChartFone;
		TChart *ChartNoise;
	TChart *ChartTemp;
		TMainMenu *MainMenu1;
		TMenuItem *MenuSave;
		TMenuItem *MenuClear;
		TTabSheet *TabSheet10;
		TChart *Chart1;
		TLineSeries *Series2;
		TArrowSeries *Series3;
		TLineSeries *Series1;
		TLabel *Label1;
		TLabel *Label2;
		TLabel *Label3;
		TLabel *Label4;
		TLabel *Label5;
		TLabel *Label6;
		TEdit *EditSizeX;
		TEdit *EditSizeY;
		TTabSheet *TabSheet1;
		TChart *Chart2;
		TBarSeries *Series4;
		TLabel *Label10;
		TEdit *EditTimeDev;
		TLabel *Label11;
		TEdit *EditNumCadr;
		TUpDown *UpDown1;
		TChart *Chart3;
		TBarSeries *Series6;
		TChart *Chart4;
		TBarSeries *Series5;
		TStringGrid *TableWindowsInfo;
		TTabSheet *TabSheet2;
		TChart *Chart5;
		TBarSeries *Series7;
		TChart *Chart6;
		TBarSeries *Series8;
		TStringGrid *TableObjectsInfo;
		TMenuItem *MenuOptions;
		TBubbleSeries *Series9;
		TTabSheet *ShowFragmentTab;
		TMenuItem *MenuOpen;
	TOpenDialog *OpenDialog;
		TCheckBox *SortFileCheckBox;
		TScrollBox *FragmentShowScrollBox;
		TMenuItem *MenuOpenFlash;
		TMenuItem *MenuOpenTMI;
		TMenuItem *N1;
		TMenuItem *BOKZ60ParseProtocol;
		TMenuItem *BOKZM2VParseProtocol;
		TMenuItem *N2;
		TCheckBox *ContrastCheckBox;
		TEdit *ScaleEdit;
		TGroupBox *GroupBox1;
		TLabel *Label8;
		TUpDown *UpDown2;
		TLabel *LabelFrameError;
		TCheckBox *PixelBrightCheckBox;
		TEdit *PixelSizeEdit;
		TLabel *Label9;
		TUpDown *UpDown3;
	TLabel *Label7;
	TEdit *FontSizeEdit;
	TUpDown *UpDown4;
	TTabSheet *TabSheet3;
	TChart *Chart7;
	TChart *Chart8;
	TMenuItem *ReadIKIFormat;

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


	private:	// User declarations
		void InitStartColorOptions();
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
		void __fastcall OnScroll(TObject* Sender);
		void SetContrast();

		unique_ptr <TFormAnimateSetting> FormAnimateSetting;
		TLineSeries *LineSeries[MaxSeries*NumGraph];
		TLineSeries *ScrollSeries[NumGraph];
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
		int FragID;


		vector <CadrInfo> vCadrInfo;
		vector <TImage*> ImageVector;
		vector <FragmentData> FragmentVector;
		vector <FragmentScrollBox*> ImageScrollBoxVector;
		vector <TImage*> FragmentsNumbers;

	public:		// User declarations

		__fastcall TFormGraphOrient(TComponent* Owner);
		void CreateLineGraph();
		void DeleteLineGraph();
		void CreateScrollGraph();
		void DeleteScrollGraph();
		void DrawScrollSeries(const struct CadrInfo &mCadr);
        void DrawLineSeries(vector <CadrInfo> _vCadrInfo);
		void SaveGraph(TChart *Chart, AnsiString suff);
		void SetVisible(int CheckLine, bool tf);
		void ApplySeriesSetting(AnsiString Title, TColor color);
		void SetVisibleLabelFrame(bool isVisible);
		int  GetCadrInfo(int NC, struct CadrInfo &mCadr);
		void DrawAnimateHandler(void);
		void PrintTableWindowsHandler(void);
		void PrintTableObjectsHandler(void);
        void DrawBlockHandler(void);
	};
	//---------------------------------------------------------------------------
	extern PACKAGE TFormGraphOrient *FormGraphOrient;
	//---------------------------------------------------------------------------
	#endif
