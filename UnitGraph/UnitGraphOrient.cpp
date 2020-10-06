#pragma hdrstop
#include "UnitGraphOrient.h"
#include "UnitDevice.h"
#include <Windef.h>
#include <codecvt>
//#include <FileStream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VCLTee.TeeTools"
#pragma link "VCLTee.TeeEdit"
#pragma link "VCLTee.ErrorBar"
#pragma link "VCLTee.TeeHighLowLine"

#pragma resource "*.dfm"
using namespace parse_prot;
using namespace std;

extern int NumDoc, NumPar, NumTab, NumImage;

bool isDTMI = 0, isMLOC = 0, isOpenDtmi = 0;
bool isLoadDb = false;

ofstream fdtmi, fmloc;
vector <GeneralizedMSHIOR> vGeneralMSHI;
vector <GeneralizedDTMI>   vGeneralDTMI;
vector <GeneralizedSHTMI1> vGeneralSHTMI1;
vector <GeneralizedSHTMI2> vGeneralSHTMI2;

#define MaxStarTrack 4
#define MaxAngVz 6
vector <CadrInfo> StarTrack[MaxStarTrack];
vector <TAngVz> StarAngVz[MaxAngVz];
AnsiString nameStarTrack[MaxStarTrack] = {"БОКЗ-1", "БОКЗ-2", "БОКЗ-3", "БОКЗ-4"};
TColor arColorOk[MaxStarTrack]  = {clBlue,   clGreen, clAqua, clLime};
TColor arColorErr[MaxStarTrack] = {clYellow, clRed, clFuchsia, clBlack};

void SwapArrByte(char *byte, int n);
int FindWordInList(string _line, string *list, int size);
int FindValueInList(unsigned short _val, unsigned short *list, int size);
int GetOrientAng(double Quat[4], TDateTime curDate, double angOr[3], double &angSun);
//---------------------------------------------------------------------------
__fastcall TFormGraphOrient::TFormGraphOrient(TComponent* Owner)
		: TForm(Owner),
		FormAnimateSetting(new TFormAnimateSetting(this)),
		plotter(new SimplePlotter()),
		fPainter(new FragmentPainter()),
		ScaleFactorForScrollBox(16),
		ScaleFactorForImage(10),
		Contrast(1),
	 	ResizeCoef(16),
		FontSize(7)
{
		Charts.push_back(ChartAl); Charts.push_back(ChartDl); Charts.push_back(ChartAz);
		Charts.push_back(ChartWx); Charts.push_back(ChartWy); Charts.push_back(ChartWz);
		Charts.push_back(ChartMx); Charts.push_back(ChartMy); Charts.push_back(ChartMxy);
		Charts.push_back(ChartNumFrag); Charts.push_back(ChartNumLoc); Charts.push_back(ChartNumDet);
		Charts.push_back(ChartLevel); Charts.push_back(ChartFone); Charts.push_back(ChartNoise);
		Charts.push_back(ChartSunAngle); Charts.push_back(ChartTemp);
		Charts.push_back(ChartAlError); Charts.push_back(ChartDlError); Charts.push_back(ChartAzError);
		Charts.push_back(ChartWxError); Charts.push_back(ChartWyError); Charts.push_back(ChartWzError);
		Charts.push_back(ChartErrorOX); Charts.push_back(ChartErrorOY); Charts.push_back(ChartErrorOZ);
		Charts.push_back(ChartFragErrX); Charts.push_back(ChartFragErrY);
		Charts.push_back(ChartFragBright); Charts.push_back(ChartFragSizeEl);
		Charts.push_back(ChartFragMean); Charts.push_back(ChartFragBright); Charts.push_back(ChartFragLevel);
		Charts.push_back(ChartAngXX); Charts.push_back(ChartAngYY); Charts.push_back(ChartAngZZ);
		Charts.push_back(ChartStat); //Charts.push_back(ChartCounter);

//может быть здесь вызывать функцию синхронизации по времени - ?
		Charts.push_back(ChartBrightMv);  Charts.push_back(ChartSizeMv);
		Charts.push_back(ChartBrightSize); Charts.push_back(ChartBrightSp);

		for (unsigned int i = 0; i < Charts.size(); i++) {
			Charts[i]->OnMouseWheel = &ChartMouseWheel;
			Charts[i]->OnMouseDown = &ChartMouseDown;
			Charts[i]->PopupMenu = PopupMenu1;
		}

		SourceDir = GetCurrentDir();
		FormAnimateSetting->ReadINI(SourceDir + "\\options.ini");
}

void __fastcall TFormGraphOrient::FormCreate(TObject *Sender)
{
	InitializeSynchronization();

	Label1->Caption = "Звезд в поле зрения: 0";
	Label2->Caption = "Фрагментов в обработке: 0";
	Label3->Caption = "Локализовано объектов: 0";
	Label4->Caption = "Распознано объектов: 0";
	Label5->Caption = "Блоков для чтения: 0";
	Label6->Caption = "Общее число строк: 0";

//InitFrameSeries
	for (int i = 0; i < MaxBlockSeries; i++) {
		BlockSeries[i] = new TChartShape(ChartMatrix);
		BlockSeries[i]->Selected->Hover->Hide();
		ChartMatrix->AddSeries(BlockSeries[i]);
		BlockSeries[i]->Style = chasRectangle;
		BlockSeries[i]->X0 = 0;
		BlockSeries[i]->X1= 1024;
		BlockSeries[i]->Y0 = 0;
		BlockSeries[i]->Y1 = 0;
		BlockSeries[i]->Color = clYellow;
		BlockSeries[i]->Transparency = 70;
		BlockSeries[i]->Legend->Visible = false;
	}
	BlockSeries[0]->Legend->Visible = true;
	BlockSeries[0]->Title = "строки";

//InitFrameSeries
	for (int i=0; i < MaxFrameSeries; i++) {
		FrameSeries[i] = new TChartShape(ChartMatrix);
		FrameSeries[i]->Selected->Hover->Hide();
		ChartMatrix->AddSeries(FrameSeries[i]);
		FrameSeries[i]->Style = chasRectangle;
		FrameSeries[i]->X0 = 0;
		FrameSeries[i]->X1 = 0;
		FrameSeries[i]->Y0 = 0;
		FrameSeries[i]->Y1 = 0;
		FrameSeries[i]->Color = clGreen;
		FrameSeries[i]->Transparency = 70;
		FrameSeries[i]->Legend->Visible = false;
	}
	FrameSeries[0]->Legend->Visible = true;
	FrameSeries[0]->Title = "фрагменты";

	InitTableWindows();
	InitTableObjects();
	InitTableStat();

	Series1->Selected->Hover->Hide();
	Series3->Selected->Hover->Hide();
	Series9->Selected->Hover->Hide();
	Series10->Selected->Hover->Hide();

	Series1->Title  = "звёзды";
	Series3->Title  = "ошибка уравнивания";
	Series10->Title = "ошибка локализации";
	Series9->Title  = "объекты";
}
//---------------------------------------------------------------------------
void ClearStarTrack(void)
{
	for (int i = 0; i < MaxStarTrack; i++) {
		StarTrack[i].clear();
	}

	for (int i = 0; i < MaxAngVz; i++) {
		StarAngVz[i].clear();
	}
}

void __fastcall TFormGraphOrient::FormClose(TObject *Sender, TCloseAction &Action)
{
	vCadrInfo.clear();
	ClearStarTrack();
	Action = caFree;
	FormAnimateSetting->WriteINI(SourceDir+"\\options.ini");
}
//---------------------------------------------------------------------------

void TFormGraphOrient::CheckTabSheet()
{
	TabSheetAngles->TabVisible = (ChartAl->SeriesCount() || ChartDl->SeriesCount()
							   || ChartAz->SeriesCount());

	TabSheetAnglesError->TabVisible = (ChartAlError->SeriesCount() || ChartDlError->SeriesCount()
									|| ChartAlError->SeriesCount());

	TabSheetAxesError->TabVisible =   (ChartErrorOX->SeriesCount() || ChartErrorOY->SeriesCount()
									|| ChartErrorOZ->SeriesCount());

	TabSheetOmega->TabVisible = (ChartWx->SeriesCount() || ChartWy->SeriesCount()
							  || ChartWz->SeriesCount());

	TabSheetOmegaError->TabVisible = (ChartWxError->SeriesCount() || ChartWyError->SeriesCount()
								   || ChartWzError->SeriesCount());

	TabSheetMxy->TabVisible = (ChartMx->SeriesCount() || ChartMy->SeriesCount()
							|| ChartMxy->SeriesCount());

	TabSheetCountObjects->TabVisible = (ChartNumFrag->SeriesCount() || ChartNumLoc->SeriesCount()
									 || ChartNumDet->SeriesCount());

	TabSheetImage->TabVisible = (ChartFone->SeriesCount() || ChartNoise->SeriesCount()
							  || ChartLevel->SeriesCount());

	TabSheetAdditional->TabVisible = (ChartSunAngle->SeriesCount() || ChartTemp->SeriesCount());

	TabSheetStatFrag->TabVisible = (ChartFragErrX->SeriesCount() || ChartFragErrY->SeriesCount()
								 || ChartFragBright->SeriesCount() || ChartFragSizeEl->SeriesCount());

	TabSheetStatFrag2->TabVisible = (ChartFragMean->SeriesCount() || ChartFragNoise->SeriesCount()
								 || ChartFragLevel->SeriesCount());

	TabSheetAngVz->TabVisible = (ChartAngXX->SeriesCount() || ChartAngYY->SeriesCount()
								 || ChartAngZZ->SeriesCount());								 

	TabSheetStatStars->TabVisible = (ChartBrightMv->SeriesCount() || ChartSizeMv->SeriesCount()
								  || ChartBrightSize->SeriesCount() || ChartBrightSp->SeriesCount());
}

void TFormGraphOrient::DeleteLineGraph()
{
	for (unsigned int i = 0; i < Charts.size(); i ++)
	{
		plotter->ClearChart(Charts[i]);
	}
}

void TFormGraphOrient::InitializeSynchronization()
{
	for (unsigned int i = 0; i < Charts.size(); i ++) {
		TColorLineTool* LineTool = new TColorLineTool(Charts[i]);
		LineTool->Axis = Charts[i]->Axes->Bottom;
		LineTool->AllowDrag = false;
		Charts[i]->Tools->Add(LineTool);
	}
}

void __fastcall TFormGraphOrient::MenuClearClick(TObject *Sender)
{
	if (Application->
	MessageBoxW(L"Точно очистить, Маша? Ты не промахнулась?", L"Вы уверены, Мария?", MB_OKCANCEL) == 1)
	{
		DeleteLineGraph();
	}
}


void __fastcall TFormGraphOrient::MenuSaveClick(TObject *Sender)
{
	SaveScreenShots();
}

UnicodeString TFormGraphOrient::SaveScreenShots(UnicodeString folder)
{
	UnicodeString ScreenFolderName;
	if (folder.Length() == 0)
	{
		ScreenFolderName = GetCurrentDir() + "\\" + FormatDateTime("yyyy-mm-dd hh.mm.ss", Now()) + " " + "Cкриншоты\\";
		TDirectory::CreateDirectoryW(ScreenFolderName);
	}
	else
	{
	   ScreenFolderName = folder + "\\" + FormatDateTime("yyyy-mm-dd hh.mm.ss", Now()) + " " + "Cкриншоты\\";
	   TDirectory::CreateDirectoryW(ScreenFolderName);
	}
	for (unsigned int i = 0; i < Charts.size(); i ++)
	{
		UnicodeString Title = IntToStr(int(i + 1)) + " - " + Charts[i]->Title->Text->Text;
		if (AnsiContainsStr(Title, ","))
		{
			Title = LeftStr(Title, PosEx(",", Title, 1) - 1);
		}
		else
		{
			Title = LeftStr(Title, PosEx("\r", Title, 1) - 1);
		}

		Title = ScreenFolderName + Title;

		if (Charts[i]->SeriesCount()) {
			plotter->SaveChart(Charts[i], Title, 500, 1100);
		}
	}
	return ScreenFolderName;
}

void TFormGraphOrient::InitTableWindows(void)
{
	int k = 0;

	TableWindowsInfo->RowCount = 2;
	TableWindowsInfo->ColCount = 10;
	TableWindowsInfo->FixedCols = 0;
	TableWindowsInfo->FixedRows = 1;

	TableWindowsInfo->Cells[k++][0]="№";
	TableWindowsInfo->Cells[k++][0]="Mv";
	TableWindowsInfo->Cells[k++][0]="Xstart";
	TableWindowsInfo->Cells[k++][0]="Ystart";
//	TableWindowsInfo->Cells[k++][0]="№ CAT";
	TableWindowsInfo->Cells[k++][0]="Mean";
	TableWindowsInfo->Cells[k++][0]="Sigma";
	TableWindowsInfo->Cells[k++][0]="Level";
	TableWindowsInfo->Cells[k++][0]="CountObj";
	TableWindowsInfo->Cells[k++][0]="Size";
	TableWindowsInfo->Cells[k++][0]="Zip";
}
void TFormGraphOrient::PrintTableWindows(const struct CadrInfo &mCadr)
{

	if (mCadr.SizeWindowsList) {
		TableWindowsInfo->RowCount = mCadr.SizeWindowsList + TableWindowsInfo->FixedRows;

		for (int i = 0; i < mCadr.SizeWindowsList; i++) {
			int k = 0;
			TableWindowsInfo->Cells[k++][i+1] = String(i+1);
			TableWindowsInfo->Cells[k++][i+1] = FloatToStrF(mCadr.WindowsList[i].Mv, ffFixed, 6, 2);
			TableWindowsInfo->Cells[k++][i+1] = String(mCadr.WindowsList[i].Xstart);
			TableWindowsInfo->Cells[k++][i+1] = String(mCadr.WindowsList[i].Ystart);
			TableWindowsInfo->Cells[k++][i+1] = FloatToStrF(mCadr.WindowsList[i].Mean, ffFixed, 6, 2);
			TableWindowsInfo->Cells[k++][i+1] = FloatToStrF(mCadr.WindowsList[i].Sigma, ffFixed, 6, 2);
			TableWindowsInfo->Cells[k++][i+1] = String(mCadr.WindowsList[i].Level);
			TableWindowsInfo->Cells[k++][i+1] = String(mCadr.WindowsList[i].CountObj);
			TableWindowsInfo->Cells[k++][i+1] = String(mCadr.WindowsList[i].Width) + "x"
											  + String(mCadr.WindowsList[i].Height);
			TableWindowsInfo->Cells[k++][i+1] = String(mCadr.WindowsList[i].ZipX) + "x"
											  + String(mCadr.WindowsList[i].ZipY);
		}
	}
	else
	{
		TableWindowsInfo->RowCount = TableWindowsInfo->FixedRows+1;
		for (int k = 0; k < TableWindowsInfo->ColCount; k ++) {
			TableWindowsInfo->Cells[k][1] = "";
		}
	}
}

bool CheckSelectObject(TObjectParam &select, double x, double y,
					   unsigned int bright, unsigned int size)
{
	if ((!select.isBright) || ((bright >= select.BrightMin) && (bright <= select.BrightMax))) {
		if ((!select.isSquare) || ((size >= select.SquareMin) && (size <= select.SquareMax))) {
			if ((!select.isCoordX) || ((x >= select.Xmin) && (x <= select.Xmax))) {
				if ((!select.isCoordY) || ((y >= select.Ymin) && (y <= select.Ymax))) {
					return true;
				}
			}
		}
	}
	return false;
}

void TFormGraphOrient::InitTableObjects(void)
{
	int k = 0;

	TableObjectsInfo->RowCount = 2;
	TableObjectsInfo->ColCount = 10;
	TableObjectsInfo->FixedCols = 0;
	TableObjectsInfo->FixedRows = 1;
	TableObjectsInfo->Cells[k++][0] = "№";
	TableObjectsInfo->Cells[k++][0] = "Xloc";
	TableObjectsInfo->Cells[k++][0] = "Yloc";
	TableObjectsInfo->Cells[k++][0] = "Bright";
	TableObjectsInfo->Cells[k++][0] = "CountPix";
	TableObjectsInfo->Cells[k++][0] = "Star №";
	TableObjectsInfo->Cells[k++][0] = "Mv";
	TableObjectsInfo->Cells[k++][0] = "Sp";
	TableObjectsInfo->Cells[k++][0] = "Dx, mkm";
	TableObjectsInfo->Cells[k++][0] = "Dy, mkm";
}

void TFormGraphOrient::PrintTableObjects(const struct CadrInfo &mCadr)
{

	if (mCadr.SizeObjectsList) {
        int iSel = 0;
		TableObjectsInfo->RowCount = mCadr.SizeObjectsList+TableWindowsInfo->FixedRows;
		for (int i = 0; i<mCadr.SizeObjectsList; i++) {

			if (CheckSelectObject(objSelect, mCadr.ObjectsList[i].X, mCadr.ObjectsList[i].Y,
								mCadr.ObjectsList[i].Bright, mCadr.ObjectsList[i].Square))
			{
				int k = 0;
				TableObjectsInfo->Cells[k++][iSel+1] = String(iSel+1);
				TableObjectsInfo->Cells[k++][iSel+1] = FloatToStrF(mCadr.ObjectsList[i].X, ffFixed,10,4);
				TableObjectsInfo->Cells[k++][iSel+1] = FloatToStrF(mCadr.ObjectsList[i].Y, ffFixed,10,4);
				TableObjectsInfo->Cells[k++][iSel+1] = FloatToStrF(mCadr.ObjectsList[i].Bright, ffFixed,10,2);
				TableObjectsInfo->Cells[k++][iSel+1] = String(mCadr.ObjectsList[i].Square);
				TableObjectsInfo->Cells[k++][iSel+1] = String(mCadr.ObjectsList[i].StarID);
				TableObjectsInfo->Cells[k++][iSel+1] = FloatToStrF(mCadr.ObjectsList[i].Mv, ffFixed,6,2);
				TableObjectsInfo->Cells[k++][iSel+1] = String(mCadr.ObjectsList[i].Sp[0])
													+String(mCadr.ObjectsList[i].Sp[1]);
				TableObjectsInfo->Cells[k++][iSel+1] = FloatToStrF(mCadr.ObjectsList[i].Dx*1000., ffFixed,8,2);
				TableObjectsInfo->Cells[k++][iSel+1] = FloatToStrF(mCadr.ObjectsList[i].Dy*1000., ffFixed,8,2);
				iSel++;
			}
		}
		TableObjectsInfo->RowCount = iSel+TableWindowsInfo->FixedRows;
	}
	else
	{
		TableObjectsInfo->RowCount = TableObjectsInfo->FixedRows + 1;
//		for (int k = 0; k < TableObjectsInfo->ColCount; k++) {
//			TableObjectsInfo->Cells[k][1] = " ";
//		}
	}
}

void TFormGraphOrient::InitStatusInfoTable(const string& deviceName)
{
	tableRows.clear();
	string fileName = toStdString(ExtractFileDir(Application->ExeName)) + "\\" + deviceName + ".txt";
	ifstream in (fileName.c_str());
	if (in.is_open())
	{
		TableStatusInfo->FixedCols = 1;
		TableStatusInfo->FixedRows = 1;
		TableStatusInfo->ColWidths[0] = 200;
		string line;
		getline(in, line);
		TableStatusInfo->RowCount  = atoi(line.c_str());
		getline(in, line);

		vector <string> splitted = split(line, "\t");
		columnTitles = split(splitted[1], ";");
		TableStatusInfo->ColCount  = columnTitles.size() + 1;
		TableStatusInfo->Cells[0][0] = "Значение статуса";
		for(int i = 0; i < columnTitles.size(); i++)
		{
			TableStatusInfo->Cells[i + 1][0] = toUString(columnTitles[i]);
		}

		int currentRow = 1;
		while (getline(in, line))
		{
			splitted = split(line, "\t");
			unsigned short statusValue = strtol(splitted[1].c_str(), NULL, 16);
			TableStatusInfo->Cells[0][currentRow++] = toUString(splitted[0] + " " + splitted[1]);
			tableRows.push_back(statusValue);
		}
	}
//	else
//	 throw runtime_error("Не удалось найти файл для построение таблиц по статусам " + fileName);

	for(int i = 0; i < TableStatusInfo->RowCount; i++)
	{
		for(int j = 0; j < columnTitles.size(); j++)
		{
			TableStatusInfo->Cells[j + 1][i + 1] = "0";
		}
	}
}


void TFormGraphOrient::ClearStatusInfoTable()
{
	for(int i = 1; i < TableStatusInfo->RowCount; i++)
	{
		for(int j = 0; j < columnTitles.size(); j++)
		{
			TableStatusInfo->Cells[j + 1][i] = "0";
		}
	}
}


void TFormGraphOrient::AddRowToStatusTable(const CadrInfo& cadr)
{
	  for (int i = 0; i < tableRows.size(); i++)
	  {
			if (tableRows[i] == cadr.StatOrient)
			{
					for(int j = 0; j < columnTitles.size(); j++)
					{
						if(cadr.DeviceInfo.find(columnTitles[j]) != string::npos)
						{
							int count = StrToInt(TableStatusInfo->Cells[j + 1][i + 1]) + 1;
							TableStatusInfo->Cells[j + 1][i + 1] =  IntToStr(count);
							break;
						}
					}

			}
	  }

}

void TFormGraphOrient::InitTableStat()
{
	int k = 0;

	TableStatInfo->RowCount  = 5;
	TableStatInfo->ColCount  = 6;
	TableStatInfo->FixedCols = 0;
	TableStatInfo->FixedRows = 1;
	TableStatInfo->Cells[k++][0] = "Параметр";
	TableStatInfo->Cells[k++][0] = "Среднее";
	TableStatInfo->Cells[k++][0] = "1 СКО";
	TableStatInfo->Cells[k++][0] = "Минимум";
	TableStatInfo->Cells[k++][0] = "Максимум";
	TableStatInfo->Cells[k++][0] = "Разброс";
}

void TFormGraphOrient::AddRowToStatTable(int nRow, AnsiString stringName, Statistika _stat, int p1,int p2)
{
	if (TableStatInfo->RowCount <= nRow) TableStatInfo->RowCount = nRow + 1;
	TableStatInfo->Cells[0][nRow] = stringName;
	TableStatInfo->Cells[1][nRow] = FloatToStrF(_stat.mean, ffFixed, p1, p2);
	TableStatInfo->Cells[2][nRow] = FloatToStrF(_stat.sigma, ffFixed, p1, p2);
	TableStatInfo->Cells[3][nRow] = FloatToStrF(_stat.min,  ffFixed, p1, p2);
	TableStatInfo->Cells[4][nRow] = FloatToStrF(_stat.max,  ffFixed, p1, p2);
	TableStatInfo->Cells[5][nRow] = FloatToStrF(_stat.max - _stat.min,  ffFixed, p1, p2);
}

void TFormGraphOrient::PrepareStartDraw()
{
	EditNumCadr->Text = 0;
	UpDown1->Max = vCadrInfo.size() - 1;
	DrawAnimateHandler();
}

void TFormGraphOrient::ClearAnimate(void)
{
//AnimateShapes
	for (int i = 0; i < MaxBlockSeries; i++)
		BlockSeries[i]->Visible = false;	//Clear();
	for (int i = 0; i < MaxFrameSeries; i++)
		FrameSeries[i]->Visible = false;    //Clear();

//AnimatePoints
	Series9->Clear();
	Series1->Clear();
	Series3->Clear();
	Series10->Clear();

//WindowsBar
	Series4->Clear();
	Series5->Clear();
	Series6->Clear();

//ObjectsBar
	Series7->Clear();
	Series8->Clear();
}

void TFormGraphOrient::SetVisibleLabelFrame(bool isVisible)
{
	for (int i=0; i<MaxFrameSeries; i++)
	{
		FrameSeries[i]->Marks->Visible = isVisible;
	}
}

void TFormGraphOrient::SetVisibleLabelStar(bool isVisible)
{
	Series1->Marks->Visible = isVisible;
}

void TFormGraphOrient::SetVisibleLabelObject(bool isVisible)
{
	Series9->Marks->Visible = isVisible;
}

void TFormGraphOrient::SetBrightSelect(bool isSelect, unsigned int min, unsigned int max)
{
	objSelect.isBright = isSelect;
	objSelect.BrightMin = min;
	objSelect.BrightMax = max;
}

void TFormGraphOrient::SetSquareSelect(bool isSelect, unsigned int min, unsigned int max)
{
	objSelect.isSquare = isSelect;
	objSelect.SquareMin = min;
	objSelect.SquareMax = max;
}

void TFormGraphOrient::SetCoordXSelect(bool isSelect, float min, float max)
{
	objSelect.isCoordX = isSelect;
	objSelect.Xmin = min;
	objSelect.Xmax = max;
}

void TFormGraphOrient::SetCoordYSelect(bool isSelect, float min, float max)
{
	objSelect.isCoordY = isSelect;
	objSelect.Ymin = min;
	objSelect.Ymax = max;
}

void DrawOneScroll(double _Time, TChart *_Chart, TLineSeries *_Series)
{
	double minLeftAxis, maxLeftAxis;
	_Chart->LeftAxis->CalcMinMax(minLeftAxis, maxLeftAxis);
	_Series->Clear();
	_Series->AddXY(_Time, minLeftAxis, "", clRed);
	_Series->AddXY(_Time, maxLeftAxis, "", clRed);
}

void TFormGraphOrient::SynchronizeCharts(double Value)
{
	for (unsigned int i = 0; i < Charts.size(); i++) {
	   TColorLineTool* LineTool = dynamic_cast <TColorLineTool*> (Charts[i]->Tools->First());
	   LineTool->Value = Value;
	}
}

double  saveMinX, saveMaxX, saveMinY, saveMaxY;
void TFormGraphOrient::SaveScale()
{
	saveMinX = ChartMatrix->LeftAxis->Minimum;
	saveMaxX = ChartMatrix->LeftAxis->Maximum;
	saveMinY = ChartMatrix->BottomAxis->Minimum;
	saveMaxY = ChartMatrix->BottomAxis->Maximum;
}

void TFormGraphOrient::ApplyScale()
{
	ChartMatrix->LeftAxis->Minimum = saveMinX;
	ChartMatrix->LeftAxis->Maximum = saveMaxX;
	ChartMatrix->BottomAxis->Minimum = saveMinY;
	ChartMatrix->BottomAxis->Maximum = saveMaxY;
}

void TFormGraphOrient::SetDefaultScale(const struct CadrInfo &mCadr)
{
//		ChartMatrix->LeftAxis->Automatic = true;
//		ChartMatrix->BottomAxis->Automatic = true;

		ChartMatrix->LeftAxis->Automatic = false;
		ChartMatrix->LeftAxis->Minimum = 0;
		ChartMatrix->LeftAxis->Maximum = mCadr.ImageHeight;
		ChartMatrix->BottomAxis->Automatic = false;
		ChartMatrix->BottomAxis->Minimum = 0;
		ChartMatrix->BottomAxis->Maximum = mCadr.ImageWidth;
}

void TFormGraphOrient::DrawAnimate(const struct CadrInfo &mCadr)
{
//	SaveScale();
	ClearAnimate();

	DrawObjects(mCadr);
	DrawBlock(mCadr);

	DrawFragment(mCadr);

	PrintTableWindows(mCadr);
	PrintTableObjects(mCadr);

//	ApplyScale();
}

void TFormGraphOrient::DrawObjects(const struct CadrInfo &mCadr)
{
	double X0, Y0, Nel;
	unsigned int BR;
	double zoomRedArrow  = StrToFloat(EditScale->Text)*1000.;
	double zoomBlueArrow, binCoef;
	TColor colorObj;

	if (mCadr.IsBinary) binCoef = 0.5;
	else binCoef = 1.;

	if ( (mCadr.SizePixel > 0 ) && (mCadr.SizePixel < 0.1) )  {
		zoomBlueArrow = mCadr.SizePixel * zoomRedArrow / binCoef;    	
	}
	else zoomBlueArrow = zoomRedArrow;

	UnicodeString uString;
	TDateTime curDate;
	curDate.Val = mCadr.Time;
	DateTimeToString(uString, UnicodeString("dd.mm.yyyy hh:mm:ss.zzz"), curDate);
	EditDateTime->Text = uString;
	EditTimeBoard->Text = FloatToStr(mCadr.TimeBoard);
	double meanDx = 0., meanDy = 0., meanDistX = 0., meanDistY = 0.;
	int countDetObject = 0;

	for (int iObject = 0; iObject < mCadr.SizeObjectsList; iObject++) {

		X0  = mCadr.ObjectsList[iObject].X;
		Y0  = mCadr.ObjectsList[iObject].Y;
		BR  = mCadr.ObjectsList[iObject].Bright;
		Nel = fabs((float)mCadr.ObjectsList[iObject].Square);

		if (CheckSelectObject(objSelect, X0, Y0, BR, Nel))
		{
			float sizeBubble;
			if (Nel) sizeBubble = 3 * sqrtm(Nel) + 0.5;
			else sizeBubble = 1.;

			AnsiString LabelObject = FloatToStrF(mCadr.ObjectsList[iObject].Bright,ffFixed,8,0) + " EMP\r\n"
								 + mCadr.ObjectsList[iObject].Square + " PIX";
			double Dx = mCadr.ObjectsList[iObject].Dx;
			double Dy = mCadr.ObjectsList[iObject].Dy;
			if (!mCadr.ObjectsList[iObject].StarID && !Dx && !Dy) {

				colorObj = clBlue;
				Series9->AddBubble(X0, Y0, sizeBubble, LabelObject, clBlue);
			}
			else
			{
				//локализованный объект
				colorObj = clGreen;
				Series9->AddBubble(X0, Y0, sizeBubble, LabelObject, clGreen);

				//остаточные рассогласования
				double X1 = X0 - Dx * zoomRedArrow;
				double Y1 = Y0 - Dy * zoomRedArrow;
				Series3->AddArrow(X0, Y0, X1, Y1, "", clRed);

				//ошибка локализации
				if (mCadr.SizeStarsList) {
					double minDist = 1000, minDistX, minDistY;

					for (int iStar = 0; iStar < mCadr.SizeStarsList; iStar++) {
						double distX = mCadr.ObjectsList[iObject].X
										  - mCadr.StarsList[iStar].X * binCoef;
						double distY = mCadr.ObjectsList[iObject].Y
										  - mCadr.StarsList[iStar].Y * binCoef;
						double curDist = sqrtm(distX * distX + distY * distY);

						if (curDist < minDist) {
							minDist  = curDist;
							minDistX = distX;
							minDistY = distY;
						}
					}
					meanDistX += minDistX;
					meanDistY += minDistY;
					X1 = X0 - minDistX * zoomBlueArrow;
					Y1 = Y0 - minDistY * zoomBlueArrow;
					Series10->AddArrow(X0, Y0, X1, Y1, "", clBlue);
				}
				meanDx += Dx;
				meanDy += Dy;
				countDetObject++;
			}
//			if (false) {
//				Series9->AddBubble(X0, Y0, sizeBubble, LabelObject, colorObj);
//			}
//			else Series2->AddXY(X0, Y0, LabelObject, colorObj);
		}
	}

	//вывод средних остаточных рассогласований
	if (countDetObject) {
		meanDx /= countDetObject;
		meanDy /= countDetObject;
		meanDistX /= countDetObject;
		meanDistY /= countDetObject;
		float Xc = mCadr.ImageWidth  / 2.;
		float Yc = mCadr.ImageHeight / 2.;

		Series3->AddArrow(Xc, Yc, Xc - meanDx * zoomRedArrow,
								  Yc - meanDy * zoomRedArrow, "", clRed);
		Series10->AddArrow(Xc, Yc, Xc - meanDistX * zoomBlueArrow,
								   Yc - meanDistY * zoomBlueArrow, "", clBlue);
	}
}

void TFormGraphOrient::DrawBlock(const struct CadrInfo &mCadr)
{
	unsigned short CountLines, CountBlock, TabTakeAway[MaxBlockSeries][2];

//DrawBlocks()
	CountLines = 0;
	CountBlock = mCadr.CountBlock;
	if (CountBlock > MaxBlockSeries) {
		ShowMessage("Превышено число допустимых блоков!");
		CountBlock = MaxBlockSeries;
	}

	for (int i = 0; i < CountBlock; i ++)
	{
		TabTakeAway[i][0] = mCadr.LinesList[i].Start;
		TabTakeAway[i][1] = mCadr.LinesList[i].Height;

		BlockSeries[i]->Visible = true;
		BlockSeries[i]->X0 = 0;
		BlockSeries[i]->X1 = mCadr.ImageWidth;
		BlockSeries[i]->Y0 = TabTakeAway[i][0];
		BlockSeries[i]->Y1 = TabTakeAway[i][0] + TabTakeAway[i][1];
		CountLines += mCadr.LinesList[i].Height;
	}

//DrawWindows()
	for (int i = 0; i < mCadr.SizeWindowsList; i++)    //!!!
	{
		FrameSeries[i]->Visible = true;
		FrameSeries[i]->X0 = mCadr.WindowsList[i].Xstart;
		FrameSeries[i]->X1 = mCadr.WindowsList[i].Xstart
							  + mCadr.WindowsList[i].Width;
		FrameSeries[i]->Y0 = mCadr.WindowsList[i].Ystart;
		FrameSeries[i]->Y1 = mCadr.WindowsList[i].Ystart
							  + mCadr.WindowsList[i].Height;
	}

//ShowLabelWindows()
	AnsiString LabelWindow;
	for (int i = 0; i < mCadr.SizeWindowsList; i++)    //!!!
	{
		FrameSeries[i]->Marks->Item[0]->Visible = false;
		FrameSeries[i]->Marks->Item[1]->Visible = true;
		FrameSeries[i]->Marks->Item[1]->Text->Clear();

		LabelWindow.sprintf("%d) Mv = %.2f,\r\n № CAT = %ld", i+1,
					mCadr.WindowsList[i].Mv, mCadr.WindowsList[i].StarID);
		FrameSeries[i]->Marks->Item[1]->Text->Add(LabelWindow);
//		FrameSeries[i]->Visible = true;
	}

//DrawBarWindows()
	for (int i = 0; i < mCadr.SizeWindowsList; i ++)    //!!!
	{
		Series4->AddXY(i + 1, mCadr.WindowsList[i].Mean);
		Series5->AddXY(i + 1, mCadr.WindowsList[i].Sigma);
		Series6->AddXY(i + 1, mCadr.WindowsList[i].CountObj);
	}

//DrawBarObjects()
	if (mCadr.SizeWindowsList)     //!!!
	{
		unsigned short *ObjShiftWnd, CountLocalObj;
		TColor CurColor, ColorBrightDef = Series7->Color;
		TColor ColorBright[] = {ColorBrightDef, clYellow, clRed};
		ObjShiftWnd = new unsigned short [mCadr.CountWindows];
		CountLocalObj = 0;
		for (int k = 0;k < mCadr.SizeWindowsList; k++)
		{
			if (!k) ObjShiftWnd[k] = 0;
			else ObjShiftWnd[k] = ObjShiftWnd[k - 1] + mCadr.WindowsList[k - 1].CountObj;

			int j = 0;
			for (int i = ObjShiftWnd[k]; i < ObjShiftWnd[k]+mCadr.WindowsList[k].CountObj; i++)
			{
				if (FormAnimateSetting->CheckBoxApplyWindowsSeries->Checked) {
					switch (j + 1)
					{
						case 1: {
							CurColor = FormAnimateSetting->ShapeColorOneObjTable->Brush->Color;
							break;
						}
						case 2: {
							CurColor = FormAnimateSetting->ShapeColorTwoObjTable->Brush->Color;
							break;
						}
						default:
						{
							CurColor = FormAnimateSetting->ShapeColorThreeObjTable->Brush->Color;
							break;
						}
					}
				}
				else CurColor = ColorBright[j];
				if (CheckSelectObject(objSelect, mCadr.ObjectsList[i].X, mCadr.ObjectsList[i].Y,
									mCadr.ObjectsList[i].Bright, mCadr.ObjectsList[i].Square)) {
					Series7->AddXY((k + 1)+0.2 * j, mCadr.ObjectsList[i].Bright, "", CurColor);
					Series8->AddXY((k + 1)+0.2 * j, mCadr.ObjectsList[i].Square, "", CurColor);
				}
				CountLocalObj++;
				j++;
	//			WndObj[i]=k;
			}
		}

		if (CountLocalObj != mCadr.CountLocalObj) {
			LabelFrameReport->Caption = "Несоответствие числа фрагментов и массива ObjFrag[]!";
		}
		delete [] ObjShiftWnd;

	}
	else
	{
		int CountSelectObj = 0;
		for (int i = 0; i < mCadr.SizeObjectsList; i++)
		{
			if (CheckSelectObject(objSelect, mCadr.ObjectsList[i].X, mCadr.ObjectsList[i].Y,
									mCadr.ObjectsList[i].Bright, mCadr.ObjectsList[i].Square)) {
				Series7->AddXY(CountSelectObj + 1, mCadr.ObjectsList[i].Bright);
				Series8->AddXY(CountSelectObj + 1, mCadr.ObjectsList[i].Square);
                CountSelectObj++;
			}
		}
	}

	AnsiString LabelStar;
//  Series1->Marks->Visible=true;
	float binCoef = 1.;
	if (mCadr.IsBinary) binCoef = 0.5;

	for (int i = 0; i < mCadr.SizeStarsList; i++) {

		AnsiString LabelStar = FloatToStrF(mCadr.StarsList[i].Mv,ffFixed,8,2) + " "
							 + mCadr.StarsList[i].Sp[0] + mCadr.StarsList[i].Sp[1];
		Series1->AddXY(mCadr.StarsList[i].X * binCoef,
					   mCadr.StarsList[i].Y * binCoef, LabelStar);
	}

	Label1->Caption = "Звезд в поле зрения: " + IntToStr(mCadr.CountStars);
	Label2->Caption = "Фрагментов в обработке: " + IntToStr(mCadr.CountWindows);
	Label3->Caption = "Локализовано объектов: " + IntToStr(mCadr.CountLocalObj);
	Label4->Caption = "Распознано объектов: " + IntToStr(mCadr.CountDeterObj);
	Label5->Caption = "Блоков для чтения: " + IntToStr(mCadr.CountBlock);

	if (CountLines == mCadr.CountLines)
		Label6->Caption = "Общее число строк: "+IntToStr(mCadr.CountLines);
	else
		Label6->Caption = "ОШИБКА! Число строк: "+IntToStr(CountLines)
						+ " вместо " + IntToStr(mCadr.CountLines);

	if (!mCadr.StatOrient) {
		Label15->Caption = "Al = " + FloatToStrF(mCadr.AnglesOrient[0] * RTD, ffFixed, 8, 4)
						 + ", Dl = " + FloatToStrF(mCadr.AnglesOrient[1] * RTD, ffFixed, 8, 4)
						 + ", Az = " + FloatToStrF(mCadr.AnglesOrient[2] * RTD, ffFixed, 8, 4);
	}
	else {
		Label15->Caption = "Ориентация не определена";
	}

}

void TFormGraphOrient::DrawFragment(const struct CadrInfo &mCadr)
{
	resetFragmentShowScrollBox();

	for(unsigned int i = 0;i < ImageScrollBoxVector.size(); i++)
	{
		 ImageScrollBoxVector[i]->Free();
	}

	ImageVector.clear();
	FragmentVector.clear();
	FragmentsNumbers.clear();
	ImageScrollBoxVector.clear();

	AnsiString NeededDirectory = GetCurrentDir() + "\\Frag_" + FileTitle;
	if (!TDirectory::Exists(NeededDirectory))
	{
		return;
	}

   TStringDynArray FileNameList;
   FileNameList = TDirectory::GetFiles(NeededDirectory);
   AnsiString TimePrStr = FloatToStr(mCadr.Time);

   AnsiString FragmentFileStr;
   for(int CurrentFileName = 0;CurrentFileName < FileNameList.Length;CurrentFileName++)
   {
		if(Strutils::AnsiContainsStr(FileNameList[CurrentFileName], TimePrStr))
		{
			FragmentFileStr = FileNameList[CurrentFileName];
			break;
		}
   }

   if(!FragmentFileStr.IsEmpty())
   {

	ifstream fragmentFile(FragmentFileStr.c_str(), ios::binary);
	if(!fragmentFile.is_open())
	{
		ShowMessage(AnsiString("Не удалось открыть файл ") + FragmentFileStr.c_str());
		return;
	}

	for(int CurrentFragment = 0;CurrentFragment < mCadr.CountWindows; CurrentFragment++)
	{
		FragmentVector.push_back(FragmentData());
		FragmentVector.back().SizeX = mCadr.WindowsList [CurrentFragment].Width;
		FragmentVector.back().SizeY = mCadr.WindowsList [CurrentFragment].Height;
		int FragmentSize =  FragmentVector.back().SizeX * FragmentVector.back().SizeY;
		FragmentVector.back().RawFragment = new unsigned short [FragmentSize];
		fragmentFile.read((char*)FragmentVector.back().RawFragment, sizeof(unsigned short) * FragmentSize);
		int Limit = -1;
		if (CheckBoxLimit->Checked)
		{
			Limit = mCadr.WindowsList[CurrentFragment].Level;
		}
		fPainter->setLimit(Limit);
		unique_ptr<TBitmap> Fragment(fPainter->createFragmentBitmap(FragmentVector.back()));


		ImageScrollBoxVector.push_back(new FragmentScrollBox(FragmentShowScrollBox));
		FragmentShowScrollBox->Color = clBlack;
		ImageScrollBoxVector.back()->Color = clBlack;
		ImageScrollBoxVector.back()->Width = Fragment->Width * ScaleFactorForScrollBox;
		ImageScrollBoxVector.back()->OnVerticalScroll = &OnScroll;
		ImageScrollBoxVector.back()->OnHorizontalScroll = &OnScroll;
		ImageScrollBoxVector.back()->Height = Fragment->Height * ScaleFactorForScrollBox;
		ImageScrollBoxVector.back()->OnMouseWheelUp= &FormMouseWheelUp;
		ImageScrollBoxVector.back()->OnMouseWheelDown= &FormMouseWheelDown;
		ImageScrollBoxVector.back()->SetParentComponent(FragmentShowScrollBox);


		ImageVector.push_back(new TImage(ImageScrollBoxVector.back()));
		ImageVector.back()->Width = Fragment->Width* ScaleFactorForImage;
		ImageVector.back()->Height = Fragment->Height* ScaleFactorForImage;
		ImageVector.back()->Left = 0;
		ImageVector.back()->Top = 0;
		ImageVector.back()->OnMouseDown = &ImageOnClick;
		ImageVector.back()->Stretch = true;
		ImageVector.back()->Canvas->
		StretchDraw(Rect(0, 0, ImageVector.back()->Width, ImageVector.back()->Height),Fragment.get());
		ImageVector.back()->SetParentComponent(ImageScrollBoxVector.back());

		fPainter->resizeBitmap(FragmentVector.back().SizeX * ResizeCoef, 
		FragmentVector.back().SizeY * ResizeCoef, 
		ImageVector.back()->Picture->Bitmap);
		
		fPainter->drawFragmentCenter(ImageVector.back()->Picture->Bitmap,
		mCadr.WindowsList [CurrentFragment].xCenter, 
		mCadr.WindowsList [CurrentFragment].yCenter, 
		ResizeCoef);

		TImage* FragmentNumber = new TImage(ImageScrollBoxVector.back());
		FragmentNumber->Height = 15;
		FragmentNumber->Width = 150;
		FragmentNumber->Canvas->Brush->Color = clWhite;
		TRect TheRect = Rect(0, 0, 150, 15);
		UnicodeString fragInfo =
		" L: " + IntToStr((int)mCadr.WindowsList [CurrentFragment].Level) +
		" C: " + IntToStr(mCadr.WindowsList [CurrentFragment].CountObj) +
		" S: " + IntToStr((int)mCadr.WindowsList [CurrentFragment].StarID) +
		" B: " + IntToStr((int)mCadr.WindowsList [CurrentFragment].Bright) + " ";
		FragmentNumber->Canvas-> TextRect(TheRect, 0, 0, IntToStr(CurrentFragment + 1) + fragInfo);
		FragmentNumber->SetParentComponent(ImageScrollBoxVector.back());
		FragmentsNumbers.push_back(FragmentNumber);

	}

	PlaceImageFragments(ImageScrollBoxVector);
	fragmentFile.close();

   }

	if(!ContrastCheckBox->Checked) Contrast = 1;
	else SetContrast();
	if(PixelBrightCheckBox->Checked)
	{
		for(unsigned int i = 0; i < ImageVector.size(); i++)
		{
			fPainter->writePixelValue(FragmentVector[i], ImageVector[i]->Picture->Bitmap, ResizeCoef, 5, 7);
		}
	}

	for(unsigned int i = 0; i < ImageVector.size(); i++)
	{
		fPainter->drawFragmentCenter(ImageVector[i]->Picture->Bitmap,
		mCadr.WindowsList[i].xCenter, mCadr.WindowsList [i].yCenter, ResizeCoef);
	}
}

void __fastcall TFormGraphOrient::OnScroll(TObject* Sender)
{
	FragmentScrollBox* ScrollBox = dynamic_cast <FragmentScrollBox*> (Sender);
	TImage* FragmentNumber = dynamic_cast  <TImage*> ( ScrollBox->Components[1]);
	FragmentNumber->Top = 0;
	FragmentNumber->Left = 0;
}

void TFormGraphOrient::resetFragmentShowScrollBox()
{
   FragmentShowScrollBox->VertScrollBar->Position = 0;
   FragmentShowScrollBox->HorzScrollBar->Position = 0;
}

void TFormGraphOrient::PlaceImageFragments (const vector<FragmentScrollBox*>& FragmentImages)
{
	if(FragmentImages.empty())
	{
		return;
	}

	int HeightOffset = 0;
	int WidthOffset = 0;

	vector <int> heights;
	for(unsigned int CurrentImage = 0;CurrentImage < FragmentImages.size();CurrentImage ++)
	{
		heights.push_back(FragmentImages[CurrentImage]->Height);
		int OffsetStep = FragmentImages[CurrentImage]->Width;

	// если координата правого края текущего фрагмента превосходит
	// координату правого угла окна просмотра, сбрасываем смещение по ширине,
	// увеличиваем смещение по высоте
		if(OffsetStep * WidthOffset + FragmentImages[CurrentImage]->Width > FragmentShowScrollBox->Width)
		{
			int maxHeight = *max_element( heights.begin(), heights.end());
			heights.clear();
			WidthOffset = 0;
			HeightOffset += maxHeight;
		}

		FragmentImages[CurrentImage]->Left = OffsetStep * WidthOffset;
		FragmentImages[CurrentImage]->Top = HeightOffset;
		++WidthOffset;
	}


}


void __fastcall TFormGraphOrient::FragmentShowScrollBoxResize(TObject *Sender)
{
	PlaceImageFragments(ImageScrollBoxVector);
}


void TFormGraphOrient::SetContrast()
{
	for(unsigned int currentFragment = 0; currentFragment < ImageVector.size(); currentFragment++)
	{

	   CadrInfo& curCadr = vCadrInfo[StrToInt(EditNumCadr->Text)];
	   if (CheckBoxLimit->Checked)
	   {
			fPainter->setLimit(curCadr.WindowsList[currentFragment].Level);
	   }
	   unique_ptr <TBitmap> Fragment(fPainter->changeContrast(Contrast, FragmentVector[currentFragment]));
	   ImageVector[currentFragment]->Picture->Bitmap->FreeImage();
	   ImageVector[currentFragment]->Picture->Bitmap = NULL;
	   ImageVector[currentFragment]->Canvas->
	   StretchDraw(Rect(0, 0, ImageVector[currentFragment]->Width, ImageVector[currentFragment]->Height),Fragment.get());

	   fPainter->resizeBitmap(FragmentVector[currentFragment].SizeX * ResizeCoef, 
	   FragmentVector[currentFragment].SizeY * ResizeCoef,
	   ImageVector[currentFragment]->Picture->Bitmap);

	   fPainter->drawFragmentCenter(ImageVector[currentFragment]->Picture->Bitmap,
	   curCadr.WindowsList[currentFragment].xCenter, 
	   curCadr.WindowsList[currentFragment].yCenter, 
	   ResizeCoef);
	}
}

void __fastcall TFormGraphOrient::FormMouseWheelUp(TObject *Sender, TShiftState Shift,
		 const TPoint& MousePos, bool &Handled)
{

	if(!(FindVCLWindow(MousePos)->Name == "FragmentShowScrollBox")
	&& !(FindVCLWindow(MousePos)->ClassName() == "TScrollBox"))
   {
		return;
   }

	if(Shift.Contains(ssCtrl))
	{
		if( Contrast == -1) Contrast = 1;
		else ++Contrast;
		SetContrast();
	}

}

void __fastcall TFormGraphOrient::FormMouseWheelDown(TObject *Sender, TShiftState Shift, const TPoint& MousePos,
bool &Handled)
{
	if (!(FindVCLWindow(MousePos)->Name == "FragmentShowScrollBox")
	&& !(FindVCLWindow(MousePos)->ClassName() == "TScrollBox"))
   {
		return;
   }

	if(Shift.Contains(ssCtrl))
	{
		if( Contrast == 1) Contrast = -1;
		else --Contrast;
		SetContrast();
	}

}


 void __fastcall TFormGraphOrient::ImageOnClick(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TImage* Image = dynamic_cast<TImage*> (Sender);
	FragmentScrollBox* ScrollBox = dynamic_cast <FragmentScrollBox*> (Image->Parent);

	if(Button == mbLeft)
	{
		Image->Width = Image->Width * 1.2;
		Image->Height = Image->Height * 1.2;
		// определяем диапазоны ползунков ( ThumpSize всегда почему-то возвращает ноль, так что только таким способом )
		ScrollBox->VertScrollBar->Position = INT_MAX;
		ScrollBox->HorzScrollBar->Position = INT_MAX;
		ScrollBox->VertScrollBar->Position = (((double)(ScrollBox->VertScrollBar->Position)/Image->Height) * Y) * 1.2;
		ScrollBox->HorzScrollBar->Position = (((double)(ScrollBox->HorzScrollBar->Position)/Image->Width)* X) * 1.2;
	}
	else if(Button == mbRight)
	{
		Image->Width = Image->Width / 1.2;
		Image->Height = Image->Height / 1.2;
		ScrollBox->VertScrollBar->Position = INT_MAX;
		ScrollBox->HorzScrollBar->Position = INT_MAX;
		ScrollBox->VertScrollBar->Position =(((double)(ScrollBox->VertScrollBar->Position)/Image->Height) * Y) / 1.2;
		ScrollBox->HorzScrollBar->Position =(((double)(ScrollBox->HorzScrollBar->Position)/Image->Width)* X) / 1.2;
	}

	TImage* FragmentNumber = dynamic_cast<TImage*>(ScrollBox->Components[1]);
	FragmentNumber->Top = 0;
	FragmentNumber->Left = 0;
}


void __fastcall TFormGraphOrient::ScaleEditChange(TObject *Sender)
{
	int ScaleFactor = StrToInt(ScaleEdit->Text);
	if(ScaleFactor <= 0)
	{
		 ShowMessage("Недопустимое значение.");
		 ScaleEdit->Text = IntToStr(ScaleFactorForImage);
	}
	else
	{
		ScaleFactorForImage = ScaleFactor;
		ScaleFactorForScrollBox = ScaleFactor + 1;
	}
}


void __fastcall TFormGraphOrient::PixelBrightCheckBoxClick(TObject *Sender)
{
	if(PixelBrightCheckBox->Checked)
	{
	
		for(unsigned int i = 0; i < ImageVector.size(); i++)
		{
			CadrInfo& curCadr = vCadrInfo[StrToInt(EditNumCadr->Text)];
			if (CheckBoxLimit->Checked)
			{
				fPainter->setLimit(curCadr.WindowsList[i].Level);
			}
			fPainter->writePixelValue(FragmentVector[i], ImageVector[i]->Picture->Bitmap, ResizeCoef, 2 , FontSize);
		}
	}

}
//---------------------------------------------------------------------------


void __fastcall TFormGraphOrient::PixelSizeEditChange(TObject *Sender)
{
	int PixelSize= StrToInt(PixelSizeEdit->Text);
	if(PixelSize < 16)
	{
		 ShowMessage("Недопустимое значение.");
		 PixelSizeEdit->Text = IntToStr(ResizeCoef);
	}
	else
	{
		ResizeCoef = PixelSize;
	}
}

//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::FontSizeEditChange(TObject *Sender)
{
	int FSize = StrToInt(FontSizeEdit->Text);
	if(FSize < 5)
	{
		 ShowMessage("Недопустимое значение.");
		 FontSizeEdit->Text = IntToStr(FontSize);
	}
	else
	{
		FontSize = FSize;
	}
}
//---------------------------------------------------------------------------



void __fastcall TFormGraphOrient::EditNumCadrChange(TObject *Sender)
{
	DrawAnimateHandler();
}
//---------------------------------------------------------------------------
void  TFormGraphOrient::DrawAnimateHandler(void)
{
	try
	{
		CadrInfo CurCadr;
		int iCurrent = StrToInt(EditNumCadr->Text);

		if (!GetCadrInfo(iCurrent, CurCadr))
		{
			if (FormAnimateSetting->CheckBoxCurrentTime->Checked) {
				if (FormAnimateSetting->CheckBoxDateTime->Checked) {
					SynchronizeCharts(CurCadr.Time);
				}
				else SynchronizeCharts(CurCadr.TimeBoard);
			}

			if (CurCadr.FrameNumber != 0) {
				LabelFrameReport->Caption = "Номер кадра из файла: " + IntToStr(CurCadr.FrameNumber);
			}


			if (CheckBoxSaveScale->Checked) {
				SaveScale();
			}

			DrawAnimate(CurCadr);
			if (CheckBoxHistory->Checked) {
				int cntHistory = StrToInt(EditCountHistory->Text);
				for (int iHistory = 1; iHistory < cntHistory; iHistory++) {
					if (!GetCadrInfo(iCurrent + iHistory, CurCadr)) {
						DrawObjects(CurCadr);
					}
				}
			}

			if (CheckBoxSaveScale->Checked) {
				ApplyScale();
			}
			else {
				SetDefaultScale(CurCadr);
			}

		}
	}
	catch(exception &e)
	{
		ShowMessage(e.what());
	}
}

void  TFormGraphOrient::PrintTableWindowsHandler(void)
{
	try
	{
		CadrInfo CurCadr;
		int cnt = StrToInt(EditNumCadr->Text);

		if (!GetCadrInfo(cnt, CurCadr))
		{
			 PrintTableWindows(CurCadr);
		}
	}
	catch(exception &e)
	{
		ShowMessage(e.what());
	}
}

void  TFormGraphOrient::PrintTableObjectsHandler(void)
{
	try
	{
		CadrInfo CurCadr;
		int cnt = StrToInt(EditNumCadr->Text);

		if (!GetCadrInfo(cnt, CurCadr))
		{
			 PrintTableObjects(CurCadr);
		}
	}
	catch(exception &e)
	{
		ShowMessage(e.what());
	}
}

//void  TFormGraphOrient::DrawBlockHandler(void)
//{
//	try
//	{
//		CadrInfo CurCadr;
//		int cnt = StrToInt(EditNumCadr->Text);
//
//		if (!GetCadrInfo(cnt, CurCadr))
//		{
//			 DrawBlock(CurCadr);
//		}
//	}
//	catch(exception &e)
//	{
//		ShowMessage(e.what());
//	}
//}


void __fastcall TFormGraphOrient::TableObjectsInfoDrawCell(TObject *Sender, int ACol,
		  int ARow, TRect &Rect, TGridDrawState State)
{
		TStringGrid *thisGrid = (TStringGrid*)Sender;

		if (FormAnimateSetting->CheckBoxFillTableObjects->Checked) {
			if ( ARow && !thisGrid->Cells[5][ARow].IsEmpty())
			{
				unsigned long StarID;
				float Dx, Dy;
				StarID = StrToInt(thisGrid->Cells[5][ARow]);
				Dx = StrToFloat(thisGrid->Cells[8][ARow]);
				Dy = StrToFloat(thisGrid->Cells[9][ARow]);
				if ((!StarID) && (!Dx) && (!Dy))
					thisGrid->Canvas->Brush->Color = FormAnimateSetting->ShapeColorLocObjTable->Brush->Color;
				else thisGrid->Canvas->Brush->Color = FormAnimateSetting->ShapeColorDetObjTable->Brush->Color;

				thisGrid->Canvas->FillRect(Rect);
				thisGrid->Canvas->TextOutW(Rect.Left, Rect.Top, thisGrid->Cells[ACol][ARow]);
			}
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphOrient::TableWindowsInfoDrawCell(TObject *Sender, int ACol,
		  int ARow, TRect &Rect, TGridDrawState State)
{

	TStringGrid *thisGrid=(TStringGrid*)Sender;

	if (FormAnimateSetting->CheckBoxFillTableWindows->Checked) {
		if (ARow && !thisGrid->Cells[7][ARow].IsEmpty())
		{
			if (thisGrid->Cells[7][ARow] == "0")
				thisGrid->Canvas->Brush->Color = FormAnimateSetting->ShapeColorZeroObjTable->Brush->Color;
			else if ((thisGrid->Cells[7][ARow] == "1"))
				thisGrid->Canvas->Brush->Color = FormAnimateSetting->ShapeColorOneObjTable->Brush->Color;
			else if ((thisGrid->Cells[7][ARow] == "2"))
				thisGrid->Canvas->Brush->Color = FormAnimateSetting->ShapeColorTwoObjTable->Brush->Color;
			else
				thisGrid->Canvas->Brush->Color = FormAnimateSetting->ShapeColorThreeObjTable->Brush->Color;

			thisGrid->Canvas->FillRect(Rect);
			thisGrid->Canvas->TextOutW(Rect.Left, Rect.Top, thisGrid->Cells[ACol][ARow]);
		}
	}
}
//---------------------------------------------------------------------------


void GetFileTitles(AnsiString file_name, AnsiString *file_title)
{
	  AnsiString file_ext;

	  file_ext = ExtractFileName(file_name);
	  if (file_ext.LastDelimiter("."))
		*file_title = file_ext.SubString(0,file_ext.LastDelimiter(".")-1);
	  else *file_title = file_ext.SubString(0,file_ext.Length());
}


	// сортируем и перезаписываем исходный файл в новый файл с постфиксом _sorted
			AnsiString TFormGraphOrient::SortRawFlashFile(const AnsiString &RawFileName)
			{
			   FILE* RawFlashFile;
			   RawFlashFile = fopen(RawFileName.c_str(),"rb");
			   if(!RawFlashFile)
			   {
					return AnsiString();
			   }

			  vector<RawFileInfo>  RawFileInfoVector;
			  int Marker;
			   while(fread(&Marker, sizeof(int), 1, RawFlashFile) == 1)
			   {
					if (GetInt(Marker) == 0x55550000)
					{
							RawFileInfo CurrentInfo;

							// текущая позиция на начале заголовочной структуры
							CurrentInfo.Pos =  ftell(RawFlashFile) - sizeof(int);

							if(RawFileInfoVector.size() == 0)
							{
								CurrentInfo.Size = 0;
							}
							else
							{
								CurrentInfo.Size = CurrentInfo.Pos - RawFileInfoVector.back().Pos;
							}

							unsigned short SequenceCounterAndAdress [2];
							fread(&SequenceCounterAndAdress,sizeof(short), 2, RawFlashFile);
							CurrentInfo.SecuenceCounter = SequenceCounterAndAdress[0];
							RawFileInfoVector.push_back(CurrentInfo);

					}
			   }

			   if(ferror(RawFlashFile))
			   {
                    throw("Ошибка чтения файла");
			   }

			   // забиваем вручную для последней структуры
				fseek (RawFlashFile, 0, SEEK_END);
				RawFileInfo LastInfo;
				LastInfo.Pos= ftell(RawFlashFile);
				LastInfo.Size=LastInfo.Pos - RawFileInfoVector.back().Pos;
				// для того чтобы после сортировки всегда был в конце
				LastInfo.SecuenceCounter= LONG_MAX;
				RawFileInfoVector.push_back(LastInfo);


			   // функтор для сортировки
			struct {

				bool operator()(const RawFileInfo& a,const RawFileInfo& b)
				{
					return a.SecuenceCounter < b.SecuenceCounter;
				}

			} FlashCompare ;


			   sort(RawFileInfoVector.begin(), RawFileInfoVector.end(), FlashCompare);
			   FILE* SortedFile;
			   AnsiString SortedFlashFileName = GetCurrentDir() + "\\flash_sorted.dat";
			   SortedFile = fopen(SortedFlashFileName.c_str(), "wb");

			   if(!SortedFile)
			   {
					return "";
			   }

			   // -1 т.к размер блока хранится  в следующем элементе
			   for(unsigned int CurrentFlashStruct = 0; CurrentFlashStruct < RawFileInfoVector.size() - 1 ;CurrentFlashStruct++)
			   {
					fseek(RawFlashFile, RawFileInfoVector[CurrentFlashStruct].Pos,SEEK_SET);
					char* CurrentStructBuffer = new char [RawFileInfoVector[CurrentFlashStruct + 1].Size];
					fread(CurrentStructBuffer,sizeof(char), RawFileInfoVector[CurrentFlashStruct + 1].Size,RawFlashFile);
					fwrite(CurrentStructBuffer,sizeof(char), RawFileInfoVector[CurrentFlashStruct + 1].Size,SortedFile);
					delete [] CurrentStructBuffer;
			   }

			   fclose(RawFlashFile);
			   fclose(SortedFile);
			   return SortedFlashFileName;
			}




int TFormGraphOrient::GetCadrInfo(int NC, struct CadrInfo &mCadr)
{
	if ( NC >= 0 && NC < vCadrInfo.size())
		mCadr = vCadrInfo[NC];
	else
		return -1;

	return 0;
}


void __fastcall TFormGraphOrient::MenuOptionsClick(TObject *Sender)
{
   FormAnimateSetting->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::MenuOpenFlashClick(TObject *Sender)
{
  try
  {
  OpenDialog->Options.Clear();
  OpenDialog->Filter = "dat|*.dat";
  if (OpenDialog->Execute()) {
		vCadrInfo.clear();
		AnsiString FileName = OpenDialog->FileName;
		SetCurrentDir(ExtractFileDir(FileName));
		GetFileTitles(FileName, &FileTitle);

		if(SortFileCheckBox->Checked)
		{
			FileName = SortRawFlashFile(FileName);
			if(FileName.IsEmpty())
			{
				ShowMessage("Не удалось отсортировать файл. Возможно, исходный файл не удалось открыть");
				return;
			}
		}

		bool isBlock1984 = true;
		char buf1984[1984];
		if (isBlock1984) {
			FILE *finp, *fout;
			finp = fopen(FileName.c_str(), "rb");
			fout = fopen((FileTitle + "_block.dat").c_str(), "wb");
			while (!feof(finp)) {
				fread(buf1984, sizeof(char), 1984, finp);
				fwrite(buf1984, sizeof(char), 1984, fout);
				fread(buf1984, sizeof(char), 96, finp);
			}
			fclose(finp);
			fclose(fout);
			FileName = FileTitle + "_block.dat";
		}


		FILE *ffrag;
		FILE *fflesh;
		FILE *ftxt;
		FILE *floc;

		fflesh = fopen(FileName.c_str(), "rb");
		FileName = FileTitle + "_sec.dat";
		ffrag = fopen(FileName.c_str(),"wb");
		ftxt = fopen((FileTitle + "_LogSec.txt").c_str(), "wt");

		unsigned short buf[2];
		unsigned int CntWord = 0;
		int FragID = 0;
		int Marker;
		while(!feof(fflesh))
		{
			fread(&Marker,sizeof(int),1,fflesh);
			if (GetInt(Marker) == 0x55550000)
			{
				fread(&buf,sizeof(short), 2, fflesh);
				fprintf(ftxt,"%6d 0x%04x %10d\n",buf[0],buf[1], CntWord*sizeof(CntWord));
				CntWord=2;
			}
			else    {
				fwrite(&Marker,sizeof(int),1,ffrag);
				CntWord++;
			}
		}
		fclose(fflesh);
		fclose(ffrag);
		fclose(ftxt);

		fflesh=fopen(FileName.c_str(),"rb");
		ftxt=fopen((FileTitle + ".txt").c_str(), "wt");


		CurDir = GetCurrentDir();
		AnsiString FragDir = CurDir + "\\Frag_" + FileTitle;
		AnsiString LocDir = CurDir + "\\Loc_" + FileTitle;
		CreateDir(FragDir);
		CreateDir(LocDir);


		FILE *flog_pix;
		flog_pix = fopen((FileTitle + "_LogPix.txt").c_str(), "wt");
		fprintf(flog_pix,"%6s  NC  %6s %10s %10s %10s %8s\n",
						"SerNum","CntRec","T","Tbshv","Tpr","NumPix");


		FILE *flog_reg;
		flog_reg=fopen((FileTitle+"_LogReg.txt").c_str(),"wt");
		fprintf(flog_reg,"%6s %6s %10s %10s %10s %8s %4s %4s\n",
						"SerNum","CntRec","T","Tbshv","Tpr","CntErr", "Reg", "Val");

		FILE *flog_orient;
		flog_orient = fopen((FileTitle+"_LogOrient.txt").c_str(),"wt");
		fprintf(flog_orient,"%6s RG %6s %10s %10s %10s %6s %8s %8s %8s %8s %10s %10s %10s ",
						"SerNum","CntRec","T","Tbshv","Tpr","Stat","NumProg",
						"NumFrag","NumLoc","NumDet","al,deg","dl,deg","Az,deg");


		fprintf(flog_orient,"%10s %8s %8s %8s %12s %12s %12s %8s %8s %8s %8s\n",
						"m_cur,mm","Temp","Texp,ms","dT,c","Wx","Wy","Wz",
						"mean_bf","sigma_bf","mean_fr","sigma_fr");


		DeleteLineGraph();

		while (!feof(fflesh))
		{
		  fread(&Marker,sizeof(int),1,fflesh);
		  int NumPixH, PixMas[3];

		  switch (GetInt(Marker))
		  {
			case (int)PIX_0_MARKER:
			{
			  DataPixHeader mDataPixHdr;
			  fread(&mDataPixHdr,sizeof(struct DataPixHeader),1,fflesh);
			  ChangeWordPix(mDataPixHdr);
			  PrintDataPix(ftxt, mDataPixHdr, 1);

			  NumPixH=mDataPixHdr.NumPix;
			  if (mDataPixHdr.NumPix%2) NumPixH++;

			  for (int i = 0; i < NumPixH; i = i+2)
			  {
				fread(PixMas, sizeof (int),3, fflesh);
				fprintf(ftxt,"%6d %6d %6d %6d\n",i+1, PixMas[0]>>16,PixMas[0]&0x0000FFFF, PixMas[1]>>16);
				fprintf(ftxt,"%6d %6d %6d %6d\n",i+2, PixMas[1]&0x0000FFFF,PixMas[2]>>16, PixMas[2]&0x0000FFFF);
			  }
			  fprintf(flog_pix,"    %02d PIX1 %6d %10ld %10ld %6ld.%03ld %8d\n",
					  mDataPixHdr.SerNum, mDataPixHdr.CntRecord, mDataPixHdr.T, mDataPixHdr.Tbshv,
					  mDataPixHdr.Tpr_sec, mDataPixHdr.Tpr_msec, mDataPixHdr.NumPix);
			  break;
			}

			case (int)PIX_1_MARKER:
			{
			  DataPixHeader mDataPixHdr;
			  fread(&mDataPixHdr,sizeof(struct DataPixHeader),1,fflesh);
//			  fread(&mDataPixHdr.T,sizeof(struct DataPixHeader)-4,1,fflesh);
			  ChangeWordPix(mDataPixHdr);
			  PrintDataPix(ftxt, mDataPixHdr,2);

			  NumPixH = mDataPixHdr.NumPix;
			  if (mDataPixHdr.NumPix % 2) NumPixH++;

			  for (int i=0; i < NumPixH; i = i+2)
			  {
				fread(PixMas, sizeof (int),3, fflesh);
				fprintf(ftxt,"%6d %6d %6d %6d\n",i+1, PixMas[0]>>16,PixMas[0]&0x0000FFFF, PixMas[1]>>16);
				fprintf(ftxt,"%6d %6d %6d %6d\n",i+2, PixMas[1]&0x0000FFFF,PixMas[2]>>16, PixMas[2]&0x0000FFFF);
			  }
			  fprintf(flog_pix,"    %02d PIX2 %6d %10ld %10ld %6ld.%03ld %8d\n",
					  mDataPixHdr.SerNum, mDataPixHdr.CntRecord, mDataPixHdr.T, mDataPixHdr.Tbshv,
					  mDataPixHdr.Tpr_sec, mDataPixHdr.Tpr_msec, mDataPixHdr.NumPix);
			  break;
			}

			case (int)HO_MARKER:
			{
              DataNO mDataNO;
			  fread(&mDataNO,sizeof(struct DataNO_st),1,fflesh);
			  if ((mDataNO.NumL[0]>MaxObj)||(mDataNO.NumL[1]>MaxObj))
			  {
				fprintf(ftxt,"\nОшибка чтения данных!\n");
				break;
	          }

			  fread(&mDataNO.RsLocT[0][0][0],sizeof(float)*4,mDataNO.NumL[0],fflesh);
			  fread(&mDataNO.RsLocT[1][0][0],sizeof(float)*4,mDataNO.NumL[1],fflesh);
			  fread(&mDataNO.NumLoc[0],sizeof(struct DataNO_fin),1,fflesh);

			  ChangeWordNO(mDataNO);
			  PrintDataNO(ftxt, mDataNO);

			  CadrInfo mCadrInfo;//1, mCadrInfo2;
			  ConvertDataNO(mDataNO, mCadrInfo,0);
			  vCadrInfo.push_back(mCadrInfo);

			  ConvertDataNO(mDataNO, mCadrInfo, 1);

			  if (mCadrInfo.IsOrient) {
				  MatrixToEkvAngles(mCadrInfo.MatrixOrient, mCadrInfo.AnglesOrient);
			  }
			  vCadrInfo.push_back(mCadrInfo);

			  if (mCadrInfo.IsOrient) {
						plotter->AddPoint(ChartAl, 0, mCadrInfo.Time, mCadrInfo.AnglesOrient[0] * RTD);
						plotter->AddPoint(ChartDl, 0, mCadrInfo.Time, mCadrInfo.AnglesOrient[1] * RTD);
						plotter->AddPoint(ChartAz, 0, mCadrInfo.Time, mCadrInfo.AnglesOrient[2] * RTD);
						plotter->AddPoint(ChartMx, 0, mCadrInfo.Time, mCadrInfo.MeanErrorX * 1000.);
						plotter->AddPoint(ChartMy, 0, mCadrInfo.Time, mCadrInfo.MeanErrorY * 1000.);
						plotter->AddPoint(ChartMxy, 0, mCadrInfo.Time, mCadrInfo.MeanErrorXY * 1000.);
						plotter->AddPoint(ChartNumDet, 0, mCadrInfo.Time, mCadrInfo.CountDeterObj);
						plotter->AddPoint(ChartWx, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[0] * RTS);
						plotter->AddPoint(ChartWy, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[1] * RTS);
						plotter->AddPoint(ChartWz, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[2] * RTS);

			  }
			  plotter->AddPoint(ChartTemp, 0, mCadrInfo.Time, mCadrInfo.MatrixTemp);
			  plotter->AddPoint(ChartNumLoc, 0, mCadrInfo.Time, mCadrInfo.CountLocalObj);


			  double al = 0;
			  double dl = 0;
			  double az = 0;

			  if (!mDataNO.res_stat) {
				al = mCadrInfo.AnglesOrient[0] * RTD;
				dl = mCadrInfo.AnglesOrient[1] * RTD;
				az = mCadrInfo.AnglesOrient[2] * RTD;
			  }

			  fprintf(flog_orient,"    %02d NO %6d %10ld %10ld %6ld.%03ld 0x%04x %8d ",
					  mDataNO.SerNum, mDataNO.CntRecord, mDataNO.T, mDataNO.Tbshv,
					  mDataNO.Tpr_sec, mDataNO.Tpr_msec, mDataNO.res_stat, 0);
			  fprintf(flog_orient,"%8d %8d %8d %10.6f %10.6f %10.6f %10.6f %8.2f %8d %8.2f ",
					  mDataNO.NumL[0], mDataNO.NumL[1], mDataNO.NumDet, al, dl, az,
					  mDataNO.m_cur, mDataNO.Temp, mDataNO.StarExp, mDataNO.TimeExp);
			  fprintf(flog_orient,"%12.4f %12.4f %12.4f %8.2f %8.2f %8.2f %8.2f\n",
			  mDataNO.Wop[0]*RTS,mDataNO.Wop[1]*RTS,mDataNO.Wop[2]*RTS, 0., 0., 0., 0.);
/**
			  for (int j=0; j<2; j++)
	          {
				sprintf(loc_name,"%s\\Loc_%06d_%08d_%02d.txt",LocDir.c_str(),mDataNO.T,mDataNO.Tbshv,j+1);
				floc=fopen(loc_name,"wt");
				for (int i=0; i<mDataNO.NumL[j]; i++)
	            {
				  fprintf(floc,"%4d %12.4f %12.4f %6.0f %6.0f\n", i+1, mDataNO.RsLocT[j][i][0],mDataNO.RsLocT[j][i][1],
				  mDataNO.RsLocT[j][i][2],mDataNO.RsLocT[j][i][3]);
				}
	            fclose(floc);
	          }
/**/
	          break;
			}

			case (int)SL_MARKER:
			{
              DataSLEZH mDataSLEZH;
			  fread(&mDataSLEZH,sizeof(struct DataSLEZH_st),1,fflesh);
	          if (mDataSLEZH.NumLoc>MaxObj)
			  {
	            fprintf(ftxt,"\nОшибка чтения данных!\n");
				break;
			  }

			  fread(&mDataSLEZH.RsLocT[0][0],sizeof(float)*4,mDataSLEZH.NumLoc,fflesh);
			  fread(&mDataSLEZH.RsLoc[0][0],sizeof(struct DataSLEZH_fin),1,fflesh);

			  ChangeWordSLEZH(mDataSLEZH);
			  PrintDataSLEZH(ftxt, mDataSLEZH);

			  CadrInfo mCadrInfo;
			  ConvertDataSLEZH(mDataSLEZH, mCadrInfo);

			  GetImageBright(mCadrInfo);

			  if (mCadrInfo.IsOrient) {
				  MatrixToEkvAngles(mCadrInfo.MatrixOrient, mCadrInfo.AnglesOrient);
			  }

			  vCadrInfo.push_back(mCadrInfo);

			  if (mCadrInfo.IsOrient) {

					plotter->AddPoint(ChartAl, 0, mCadrInfo.Time, mCadrInfo.AnglesOrient[0] * RTD);
					plotter->AddPoint(ChartDl, 0, mCadrInfo.Time, mCadrInfo.AnglesOrient[1] * RTD);
					plotter->AddPoint(ChartAz, 0, mCadrInfo.Time, mCadrInfo.AnglesOrient[2] * RTD);
					plotter->AddPoint(ChartMx, 0, mCadrInfo.Time, mCadrInfo.MeanErrorX * 1000.);
					plotter->AddPoint(ChartMy, 0, mCadrInfo.Time, mCadrInfo.MeanErrorY * 1000.);
					plotter->AddPoint(ChartMxy, 0, mCadrInfo.Time, mCadrInfo.MeanErrorXY * 1000.);
					plotter->AddPoint(ChartNumDet, 0, mCadrInfo.Time, mCadrInfo.CountDeterObj);
					plotter->AddPoint(ChartWx, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[0] * RTS);
					plotter->AddPoint(ChartWy, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[1] * RTS);
					plotter->AddPoint(ChartWz, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[2] * RTS);

			  }
			  plotter->AddPoint(ChartTemp, 0, mCadrInfo.Time, mCadrInfo.MatrixTemp);
			  plotter->AddPoint(ChartNumLoc, 0, mCadrInfo.Time, mCadrInfo.CountLocalObj);
			  plotter->AddPoint(ChartNumFrag, 0, mCadrInfo.Time, mCadrInfo.CountWindows);
			  plotter->AddPoint(ChartFone, 0, mCadrInfo.Time, mCadrInfo.MeanBright);
			  plotter->AddPoint(ChartNoise, 0, mCadrInfo.Time, mCadrInfo.SigmaBright);

			  double al=0;
			  double dl=0;
			  double Az=0;



			  if (!mDataSLEZH.res_stat) {
				al=mCadrInfo.AnglesOrient[0]*RTD;
				dl=mCadrInfo.AnglesOrient[1]*RTD;
				Az=mCadrInfo.AnglesOrient[2]*RTD;
			  }

			  double mean_frag=mCadrInfo.MeanBright;
			  double sigma_frag=mCadrInfo.SigmaBright;
			  fprintf(flog_orient,"    %02d SL %6d %10ld %10ld %6ld.%03ld 0x%04x %8d ",
					  mDataSLEZH.SerNum, mDataSLEZH.CntRecord, mDataSLEZH.T, mDataSLEZH.Tbshv,
					  mDataSLEZH.Tpr_sec, mDataSLEZH.Tpr_msec, mDataSLEZH.res_stat, mDataSLEZH.NumProgFrag);
			  fprintf(flog_orient,"%8d %8d %8d %10.6f %10.6f %10.6f %10.6f %8.2f %8d %8.2f ",
					  mDataSLEZH.NumFrag,mDataSLEZH.NumLoc, mDataSLEZH.NumDet, al, dl, Az,
					  mDataSLEZH.m_cur, mDataSLEZH.Temp, mDataSLEZH.StarExp, mDataSLEZH.TimeExp);
			  fprintf(flog_orient,"%12.4f %12.4f %12.4f %8.2f %8.2f %8.2f %8.2f\n",
					  mDataSLEZH.Wop[0]*RTS,mDataSLEZH.Wop[1]*RTS,mDataSLEZH.Wop[2]*RTS,
					  mDataSLEZH.mean, mDataSLEZH.sigma, mean_frag, sigma_frag);

	//		  delete [] mCadrInfo.pLocObj;

	//SaveSLEZH_IKI
/**
			  for (int i=0; i<mDataSLEZH.NumLoc; i++) {
				StructIKI->StarsData.StarsList
				[i].X_coordinate=mDataSLEZH.RsLocT[i][0];
				StructIKI->StarsData.StarsList[i].Y_coordinate=mDataSLEZH.RsLocT[i][1];
				StructIKI->StarsData.StarsList[i].BrightnessObject=mDataSLEZH.RsLocT[i][2];
				StructIKI->StarsData.StarsList[i].PixelsCount=mDataSLEZH.RsLocT[i][3];
				StructIKI->StarsData.StarsList[i].NumberStar=0;
				StructIKI->StarsData.StarsList[i].StellarMagnitude=0;
	//            StructIKI->StarsData.StarsList[i].SpectralClass='';
				StructIKI->StarsData.StarsList[i].SpectralClass[0]='_';
				StructIKI->StarsData.StarsList[i].SpectralClass[1]='_';
				StructIKI->StarsData.StarsList[i].DX=0;
				StructIKI->StarsData.StarsList[i].DY=0;
			  }
			  if (!mDataSLEZH.res_stat)
			  {
				for (int i=0; i<mDataSLEZH.NumDet; i++) {
				  int iloc=mDataSLEZH.MasRes[i][1]; //меняем местами  0-1
				  int ifrag=mDataSLEZH.MasRes[i][0]; //меняем местами  0-1
				  StructIKI->StarsData.StarsList[iloc].NumberStar=
									(unsigned int)mDataSLEZH.XYc[ifrag][2];
				  StructIKI->StarsData.StarsList[iloc].StellarMagnitude=
												  mDataSLEZH.XYc[ifrag][3];
				  StructIKI->StarsData.StarsList[iloc].DX=mDataSLEZH.Dx[i];
				  StructIKI->StarsData.StarsList[iloc].DY=mDataSLEZH.Dy[i];
				}
				}
/**/

/**
			  sprintf(loc_name,"%s\\Loc_%06d_%08d.txt",LocDir.c_str(),mDataSLEZH.T,mDataSLEZH.Tbshv);
			  floc=fopen(loc_name,"wt");
			  for (int i=0; i<mDataSLEZH.NumLoc; i++)
			  {
				fprintf(floc,"%4d %12.4f %12.4f %6.0f %6.0f\n", i+1, mDataSLEZH.RsLocT[i][0],mDataSLEZH.RsLocT[i][1],
				mDataSLEZH.RsLocT[i][2],mDataSLEZH.RsLocT[i][3]);
			  }
			  fclose(floc);
//**/
	//SavedataIKI()
/**
	          StructIKI = new IKI_img();

			  StructIKI->ImageData.ExposureTime=(float)mDataSLEZH.StarExp;
	          StructIKI->ImageData.WindowsData.WindowCount=mDataSLEZH.NumFrag;
	          StructIKI->ImageData.WindowsData.BottomRight=true;
			  StructIKI->ImageData.WindowsData.Info= new datawindow[mDataSLEZH.NumFrag];
	          for (int i=0; i<mDataSLEZH.NumFrag; i++) {
				StructIKI->ImageData.WindowsData.Info[i].WindowWidth =mDataSLEZH.PfragXY0[i][1]; //меняем местами 0-1
				StructIKI->ImageData.WindowsData.Info[i].WindowHeight=mDataSLEZH.PfragXY0[i][0]; //меняем местами 0-1
				StructIKI->ImageData.WindowsData.Info[i].Average=mDataSLEZH.MeanFrag[i][1];  //меняем местами 0-1
				StructIKI->ImageData.WindowsData.Info[i].SKO=mDataSLEZH.MeanFrag[i][0];      //меняем местами 0-1
				StructIKI->ImageData.WindowsData.Info[i].Limit=mDataSLEZH.ThFrag[i];
				StructIKI->ImageData.WindowsData.Info[i].ObjCount=mDataSLEZH.ObjFrag[i];
				StructIKI->ImageData.WindowsData.Info[i].X=mDataSLEZH.FragYX[i][0];          //меняем местами 0-1
				StructIKI->ImageData.WindowsData.Info[i].Y=mDataSLEZH.YendLoc[i];
			  }

			  StructIKI->StarsData.RezStat=mDataSLEZH.res_stat;
			  StructIKI->StarsData.LocalizedCount=mDataSLEZH.NumLoc;
			  StructIKI->StarsData.StarsList = new starinfo[mDataSLEZH.NumLoc];
			  for (int i=0; i<mDataSLEZH.NumLoc; i++) {
				StructIKI->StarsData.StarsList[i].X_coordinate=mDataSLEZH.RsLocT[i][0];
				StructIKI->StarsData.StarsList[i].Y_coordinate=mDataSLEZH.RsLocT[i][1];
				StructIKI->StarsData.StarsList[i].BrightnessObject=mDataSLEZH.RsLocT[i][2];
				StructIKI->StarsData.StarsList[i].PixelsCount=mDataSLEZH.RsLocT[i][3];
				StructIKI->StarsData.StarsList[i].NumberStar=0;
				StructIKI->StarsData.StarsList[i].StellarMagnitude=0;
	//            StructIKI->StarsData.StarsList[i].SpectralClass='';
				StructIKI->StarsData.StarsList[i].SpectralClass[0]='_';
				StructIKI->StarsData.StarsList[i].SpectralClass[1]='_';
				StructIKI->StarsData.StarsList[i].DX=0;
				StructIKI->StarsData.StarsList[i].DY=0;
			  }
			  if (!mDataSLEZH.res_stat)
			  {
				for (int i=0; i<mDataSLEZH.NumDet; i++) {
				  int iloc=mDataSLEZH.MasRes[i][1]; //меняем местами  0-1
				  int ifrag=mDataSLEZH.MasRes[i][0]; //меняем местами  0-1
				  StructIKI->StarsData.StarsList[iloc].NumberStar=
									(unsigned int)mDataSLEZH.XYc[ifrag][2];
				  StructIKI->StarsData.StarsList[iloc].StellarMagnitude=
												  mDataSLEZH.XYc[ifrag][3];
				  StructIKI->StarsData.StarsList[iloc].DX=mDataSLEZH.Dx[i];
				  StructIKI->StarsData.StarsList[iloc].DY=mDataSLEZH.Dy[i];
				}
	            StructIKI->StarsData.RecognizedCount=mDataSLEZH.NumDet;
				for (int i=0; i<4; i++) {
	              StructIKI->StarsData.RecognizedOrientationQuaternion[i]=mDataSLEZH.Quat[i];
	            }
				for (int i=0; i<3; i++)  {
	              for (int j=0; j<3; j++) {
					StructIKI->StarsData.RecognizedOrientationMatrix[i][j]=mDataSLEZH.M_ornt[i][j];
				  }
				}
			  }
	//          StructIKI->WriteFormat("C:\\TestStructIKI_data.txt");
	//          delete [] StructIKI->StarsData.StarsList;
	//          delete [] StructIKI->ImageData.WindowsData.Info;
	//          delete StructIKI;
/**/
	          break;
			}

			case (int)SINGLE_REG_MARKER:
			{
              DataSingleReg mDataSingleReg;
			  fread(&mDataSingleReg,sizeof(struct DataSingleReg),1,fflesh);
			  ChangeWordSingleReg(mDataSingleReg);
			  PrintSingleReg(ftxt, mDataSingleReg);
			  fprintf(flog_reg,"    %02d %6d %10ld %10ld %6ld.%03ld %8d %4d %4d\n",
				  mDataSingleReg.SerNum, mDataSingleReg.CntRecord, mDataSingleReg.T,
				  mDataSingleReg.Tbshv, mDataSingleReg.Tpr_sec, mDataSingleReg.Tpr_msec,
				  mDataSingleReg.CntErr, mDataSingleReg.Nreg, mDataSingleReg.RegVal);
			  break;
			}

			case (int)ALL_REG_MARKER:
			{
			  unsigned char Reg[COUNT_REG];
			  fread(Reg,sizeof(Reg),1,fflesh);
			  PrintReg(ftxt, Reg);
			  break;
			}

			case (int)FRAG_MARKER:
			{
			  DataFragHeader mDataFragHdr;
			  fread(&mDataFragHdr,sizeof(struct DataFragHeader), 1, fflesh);
	//          NumPixF*=2; // Игорь исправит!!!
			  ChangeWordFrag(mDataFragHdr);
			  if (mDataFragHdr.NumPix % 2) mDataFragHdr.NumPix--;
			  if (mDataFragHdr.NumPix > MaxPix) mDataFragHdr.NumPix = MaxPix;
			  PrintDataFrag(ftxt, mDataFragHdr);

			  FragID++;
			  int NumPixF = 0;
			  NumPixF=mDataFragHdr.NumPix;
			  unsigned short buf_frag[MaxPix];
			  fread(buf_frag, sizeof(short),NumPixF, fflesh);

			  for (int nPix = 0; nPix < NumPixF; nPix+=2) {
				SwapShort((short*)&buf_frag[nPix],(short*)&buf_frag[nPix+1]);
			  }

			  char frag_name[300];
			  sprintf(frag_name,"%s\\Frag_%06d.%03d_%03d.bin",FragDir.c_str(),
					 mDataFragHdr.Tpr_sec,mDataFragHdr.Tpr_msec, FragID);
			  ffrag=fopen(frag_name,"wb");
			  fwrite(buf_frag,sizeof(short),NumPixF,ffrag);
			  fclose(ffrag);
	//SaveFragIKI
/**
	//          StructIKI = new IKI_img();
			  StructIKI->ImageData.WindowsData.DataType=1; //unsigned short
			  StructIKI->ImageData.WindowsData.DegreeBinning=1;
			  StructIKI->ImageData.WindowsData.SizeData=NumPixF;
			  StructIKI->ImageData.WindowsData.Data = (void*)new unsigned short[NumPixF];
			  memcpy((unsigned short*)StructIKI->ImageData.WindowsData.Data,buf_frag,NumPixF*sizeof(unsigned short));
			  StructIKI->WriteFormat("C:\\TestStructIKI_data_frag"+IntToStr(FragID)+".iki");
			  delete [] StructIKI->ImageData.WindowsData.Info;
			  delete [] StructIKI->ImageData.WindowsData.Data;
			  delete [] StructIKI->StarsData.StarsList;
			  delete StructIKI;
	/**/

			  break;
			}
			case (int)PIX_LOC_MARKER:
			{

				DataPixLocHeader_Aist mDataPixHdrAist;
				fread(&mDataPixHdrAist,sizeof(struct DataPixLocHeader_Aist),1,fflesh);
				SwapArrByte((char*)&mDataPixHdrAist, sizeof(mDataPixHdrAist));

				  NumPixH=mDataPixHdrAist.NumPix;
				  if (mDataPixHdrAist.NumPix%2) NumPixH++;

				  fprintf(ftxt,"Texp = %6d, NumPix = %6d\n", mDataPixHdrAist.StarExp, mDataPixHdrAist.NumPix);
				  for (int i = 0; i < NumPixH; i = i+2)
				  {
					fread(PixMas, sizeof (int),3, fflesh);
					SwapArrByte((char*)&PixMas, sizeof(PixMas));
					fprintf(ftxt,"%6d %6d %6d %6d\n",i+1, PixMas[0]>>16,PixMas[0]&0x0000FFFF, PixMas[1]>>16);
					fprintf(ftxt,"%6d %6d %6d %6d\n",i+2, PixMas[1]&0x0000FFFF,PixMas[2]>>16, PixMas[2]&0x0000FFFF);
				  }
				  if (mDataPixHdrAist.NumPix%2 == 0) {
                    fprintf(ftxt,"%6d %6d %6d %6d\n", mDataPixHdrAist.NumPix + 1, 0, 0, 0);
				  }
//              мБОКЗ-2В
//				DataPixLocHeader mDataPixHdr;
//				fread(&mDataPixHdr,sizeof(struct DataPixLocHeader),1,fflesh);
//				ChangeWordPixLoc(mDataPixHdr);
//				PrintDataPixLoc(ftxt, mDataPixHdr);

//				DataPixFlt PixLoc;
//				for (int i = 0; i < mDataPixHdr.NumPix[1]/3; i++)
//				{
//					fread(&PixLoc, sizeof(struct DataPixFlt),1, fflesh);
//					fprintf(ftxt,"%6d %6d %6d %6d\n",i+1, PixLoc.Y,PixLoc.X, PixLoc.BR);
//
//				}
//				fprintf(flog_pix,"    %02d PIXL %6d %6ld.%03ld %8d %8d\n",
//						  mDataPixHdr.SerNum, mDataPixHdr.CntRecord,
//						  mDataPixHdr.Tpr_sec, mDataPixHdr.Tpr_msec, mDataPixHdr.NumPix[0], mDataPixHdr.NumPix[1]);

				break;
			}
			case (int)LOC_MARKER:
			{
				DataLocHeader mDataLocHdr;
				fread(&mDataLocHdr,sizeof(struct DataLocHeader),1,fflesh);
				ChangeWordLocal(mDataLocHdr);
				PrintDataLocal(ftxt, mDataLocHdr);

				DataLocObj LocObj;
				for (int i = 0; i < mDataLocHdr.NumL; i++)
				{
					fread(&LocObj, sizeof(struct DataLocObj),1, fflesh);
					LocObj.X = GetFloat(LocObj.X);
					LocObj.Y = GetFloat(LocObj.Y);
					LocObj.B = GetFloat(LocObj.B);
					fprintf(ftxt,"%6d %8.3f %8.3f %8.0f %8d\n",i+1, LocObj.X,
								LocObj.Y, LocObj.B, LocObj.S);
				}
				fprintf(flog_reg,"    %02d PIXL %6d %6ld.%03ld %8d %8d %8d %8d\n",
						  mDataLocHdr.SerNum, mDataLocHdr.CntRecord,
						  mDataLocHdr.Tpr_sec, mDataLocHdr.Tpr_msec,
						  mDataLocHdr.NumPix[0], mDataLocHdr.NumPix[1],
						  mDataLocHdr.NumL, mDataLocHdr.NumLoc);
				break;
			}
            default: break;
		  }
		}
		fclose(fflesh);
		fclose(ftxt);
		fclose(flog_reg);
		fclose(flog_pix);
		fclose(flog_orient);

		PrepareStartDraw();
		CheckTabSheet();
	 }
  }
	catch(string &s)
	{
		ShowMessage(s.c_str());
	}

	catch(exception &e)
	{
		ShowMessage(e.what());
	}

}

int FindWordInList(string _line, string *list, int size)
{
	for (int i = 0; i < size; i++) {
		if (_line.find(list[i]) != std::string::npos) return i;
	}
	return -1;
}

TDateTime EpochToDateTime (double era)
{
	double daysJ2000 = era * 36525;
	TDateTime dateTimeJ2000 = EncodeDateTime(2000, 1, 1, 12, 0, 0, 0);
	TDateTime curDateTime;
	curDateTime.Val =  dateTimeJ2000.Val + daysJ2000 + 0.125;// +3 hours for Msk
	return curDateTime;
}

void AddCadrInfo(CadrInfo _curCadr, vector <CadrInfo> &vectCadr)
{
	int n = vectCadr.size();
	if (n) {
		if (fabs(_curCadr.Time - vectCadr[n-1].Time) * 86400 > 0.001)
			vectCadr.push_back(_curCadr);
		}
	else vectCadr.push_back(_curCadr);
}

vector <CadrInfo> vMshiInfo, vLocalInfo, vDtmiInfo;
void __fastcall TFormGraphOrient::MenuOpenMILClick(TObject *Sender)
{
	const int sizeSinc   =  4;
	const int sizeMSHI   = 24;
	const int sizeSHTMI1 = 32;
	const int sizeSHTMI2 = 32;
	const int sizeDTMI   = 32;
	const int sizeMLOC   = 32;
	const int sizeMIL    = 32;

	string cwSinc[4]   = {"3224", "3A24", "6224", "6A24"};
	string cwMSHI[4]   = {"3618", "3E18", "4E18", "5618"};//"6618", "6E18"};
	string cwSHTMI1[4] = {"3440", "3C40", "4C40", "5440"};//"6440", "6C40"};
	string cwSHTMI2[4] = {"3460", "3C60", "4C60", "5460"};//"6460", "6C60"};
	string cwDTMI[4]   = {"3480", "3C80", "4C80", "5480"};//"6480", "6C80"};
	string cwMLOC[4]   = {"34A0", "3CA0", "64A0", "6CA0"};
	string codeOC[4]   = {"3000", "3800", "4800", "6000"};//"6800"};

	unsigned short ArraySinc[sizeSinc];
	unsigned short ArrayMSHI[sizeMSHI];
	unsigned short ArraySHTMI1[sizeSHTMI1];
	unsigned short ArraySHTMI2[sizeSHTMI2];
	unsigned short ArrayDTMI[30*9];
	unsigned short ArrayMLOC[30*9];
	unsigned short ArrayMIL32[sizeMIL];

	int isM1000 = false;
	bool isMSHI = 0, isSHTMI1 = 0, isSHTMI2 = 0;
	isDTMI = 0, isMLOC = 0, isOpenDtmi = 0;

	OpenDialog->Options.Clear();
	OpenDialog->Filter = "Текстовые файлы|*.dat; *.txt|Все файлы|*.*";
	OpenDialog->Options << ofAllowMultiSelect;

	if (OpenDialog->Execute()) {

		vMshiInfo.clear();
		vLocalInfo.clear();
		vDtmiInfo.clear();

		DTMI dtmi;
		TFormatSettings curFormat;
		curFormat.ShortDateFormat = "dd.mm.yyyy";
		TDateTime zeroDate  = StrToDate(FormAnimateSetting->ZeroDate1S->Text)
				  			+ StrToTime(TimeToStr(FormAnimateSetting->ZeroTime1S->Time));
		TDateTime curDate  = zeroDate;

		string str = AnsiString(FormAnimateSetting->EditListBokz->Text).c_str();
		if (str.find("БОКЗ-М60/1000") != string::npos) {
			isM1000 = true;
		}

		double dtBokz;
		if (isM1000) dtBokz = 0.25;
		else  dtBokz = 1.0;
		SetClkBokz(dtBokz);

		bool isLoadDb = FormAnimateSetting->CheckBoxLoadToDb->Checked;

		vMshiInfo.clear();
		DeleteLineGraph();

		AnsiString FileName = OpenDialog->FileName;
		AnsiString CurDir   = ExtractFileDir(FileName);
		AnsiString SaveDir  = CurDir + "/Result/";
		CreateDir(SaveDir);
		GetFileTitles(FileName,&FileTitle);

		if (OpenDialog->Files->Count > 1) {
			FileTitle += "("+IntToStr(OpenDialog->Files->Count) + ")";
		}
		ofstream fout, fmshi, fshtmi1, fshtmi2, fquat;
		fout.open((SaveDir + FileTitle + "_decode.txt").c_str());

		string line, word;
		int devIndex;
		TColor colorStat[MAX_STAT];
		for (int iStat = 0; iStat < MAX_STAT; iStat++) {
			colorStat[iStat] = (TColor)RGB((float)(MAX_STAT - iStat) / MAX_STAT * 255,
									200, (float)iStat / MAX_STAT * 255 );
			plotter->SetShowLines(true);
			plotter->SetSeriesColor(colorStat[iStat]);
			plotter->SetTitle("EC" + IntToStr(iStat + 1));
			plotter->AddSeries(ChartCounter, iStat, colorStat[iStat]);
		}
		plotter->SetShowLines(false);
		plotter->SetSeriesColor(clBlue);
		plotter->SetTitle("БОКЗ");

		vCadrInfo.clear();
		for (int iFile = 0; iFile < OpenDialog->Files->Count; iFile++) {

			ifstream finp(AnsiString(OpenDialog->Files->Strings[iFile]).c_str());
			if (!finp.is_open()) {
				ShowMessage("Файл не может быть открыт!");
				return;
			}
			finp >> hex;

			while (!finp.eof())
			{
				getline(finp, line, '\n' );

	//чтение КС "Синхро"
//				devIndex = FindWordInList(line, cwSinc, 4);
//				if ( (devIndex >= 0) && (devIndex < 4) ) {
//					finp >> word;
//					for (int i = 0; i < sizeSinc; i++) {
//						finp >> ArraySinc[i];
//					}
//					getline(finp, line, '\n' );
//					getline(finp, line, '\n' );
//					if (line.find(codeOC[devIndex]) != std::string::npos) {
//						SwapShort((short*)&ArraySinc[0], (short*)&ArraySinc[1]);
//						SwapShort((short*)&ArraySinc[2], (short*)&ArraySinc[3]);
//						float val = *(int*)&ArraySinc[2]/(double)0x80000000;
//						memcpy(&ArraySinc[2], &val, sizeof(float));
//
//						SINCHRO sinc;
//						memcpy(&sinc, ArraySinc, sizeof(sinc));
//						if (FormAnimateSetting->CheckBoxSinc->Checked) {
//							zeroDate.Val = EpochToDateTime(sinc.curEpoch).Val
//										 - sinc.countClk/86400.;
//						}
//					}
//				}
	//чтение массива МШИОР
				devIndex = FindWordInList(line, cwMSHI, 4);
				if ( (devIndex >= 0) && (devIndex < 4) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
						finp >> word;
						for (int i = 0; i < sizeMSHI; i++) {
							finp >> ArrayMSHI[i];
						}

						int *pInt;
						for (int i = 2; i <= 22; i = i + 2) {
							SwapShort((short*)&ArrayMSHI[i], (short*)&ArrayMSHI[i + 1]);
							if (i > 2) {
								pInt = (int*)&ArrayMSHI[i];
								float val = (double)*pInt/(double)0x80000000;
								if (i >= 18) val *= (PI/32);
								memcpy(&ArrayMSHI[i], &val, sizeof(float));
							}
						}

						MSHI mshi;
						memcpy(&mshi, ArrayMSHI, sizeof(mshi));
						curDate = GetDateTime(zeroDate, mshi.timeBOKZ);
						PrintMSHI(fout, mshi, curDate);
						if (!isMSHI) {
							fmshi.open((SaveDir + FileTitle + "_mshi.txt").c_str());
							fquat.open((SaveDir + FileTitle + "_quat.txt").c_str());
						}
						PrintLogMSHI(fmshi, mshi, curDate, isMSHI);

						UnicodeString uString;
						double ang[3], Mornt[3][3], Qornt[4];
						double Tpr = mshi.timeBOKZ * dtBokz;

						DateTimeToString(uString, UnicodeString("yyyy-mm-dd hh:nn:ss.zzz"), curDate);
						fquat << AnsiString(uString).c_str() << "\t";
						fquat << Tpr << "\t" << std::setprecision(8);
						for (int i = 0; i < 4; i++) {
							fquat << mshi.Qornt[i] << "\t";
						}
						fquat << "\n";

						for (int i = 0; i < 4; i++) {
							Qornt[i] =  mshi.Qornt[i];
						}

						if (!CheckQuatNorm(Qornt, 0.001)) {
							QuatToMatrix(Qornt, Mornt);
							MatrixToEkvAngles(Mornt, ang);
							plotter->AddPoint(ChartAl, 0, Tpr, ang[0] * RTD);
							plotter->AddPoint(ChartDl, 0, Tpr, ang[1] * RTD);
							plotter->AddPoint(ChartAz, 0, Tpr, ang[2] * RTD);
							plotter->AddPoint(ChartWx, 0, Tpr, mshi.W[0] * RTS);
							plotter->AddPoint(ChartWy, 0, Tpr, mshi.W[1] * RTS);
							plotter->AddPoint(ChartWz, 0, Tpr, mshi.W[2] * RTS);

							CadrInfo mCadr;
							mCadr.Time = curDate.Val;
							mCadr.IsOrient = true;
							for (int i = 0; i < 3; i++) {
								mCadr.AnglesOrient[i] = ang[i];
								mCadr.OmegaOrient[i]  = mshi.W[i];
							}
							AddCadrInfo(mCadr, vMshiInfo);
						}
					}
				}
	//чтение массива ШТМИ1
				devIndex = FindWordInList(line, cwSHTMI1, 4);
				if ( (devIndex >= 0) && (devIndex < 4) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
						finp >> word;
						for (int i = 0; i < sizeSHTMI1; i++) {
							finp >> ArraySHTMI1[i];
						}

						SwapShort((short*)&ArraySHTMI1[2], (short*)&ArraySHTMI1[3]);
						SwapShort((short*)&ArraySHTMI1[8], (short*)&ArraySHTMI1[9]);
						SwapShort((short*)&ArraySHTMI1[10], (short*)&ArraySHTMI1[11]);
						SwapShort((short*)&ArraySHTMI1[12], (short*)&ArraySHTMI1[13]);

						SHTMI1 shtmi1;
						memcpy(&shtmi1, &ArraySHTMI1[2], sizeof(shtmi1));
						PrintSHTMI1(fout, shtmi1);
						if (!isSHTMI1) {
							fshtmi1.open((SaveDir + FileTitle + "_shtmi1.txt").c_str());
						}
						PrintLogSHTMI1(fshtmi1, shtmi1, isSHTMI1);
					}
				}
	//чтение массива ШТМИ2
				devIndex = FindWordInList(line, cwSHTMI2, 4);
				if ( (devIndex >= 0) && (devIndex < 4) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
						finp >> word;
						for (int i = 0; i < sizeSHTMI2; i++) {
							finp >> ArraySHTMI2[i];
						}
						SwapShort((short*)&ArraySHTMI2[2], (short*)&ArraySHTMI2[3]);
						SwapShort((short*)&ArraySHTMI2[14], (short*)&ArraySHTMI2[15]);

						SHTMI2 shtmi2;
						memcpy(&shtmi2, &ArraySHTMI2[2], sizeof(shtmi2));

						curDate = GetDateTime(zeroDate, shtmi2.timeBOKZ);
						PrintSHTMI2(fout, shtmi2, curDate);
						if (!isSHTMI2) {
							fshtmi2.open((SaveDir + FileTitle + "_shtmi2.txt").c_str());
						}
						PrintLogSHTMI2(fshtmi2, shtmi2, curDate, isSHTMI2);

						plotter->AddPoint(ChartStat, 0, shtmi2.timeBOKZ,
										  (shtmi2.status2 & 0xFF00) >> 8);

						for (int iStat = 0; iStat < MAX_STAT; iStat++) {
							plotter->AddPoint(ChartCounter, iStat, shtmi2.timeBOKZ,
										  shtmi2.cntStatOrient[iStat], colorStat[iStat]);
						}
					}
				}

				short nAr;
				unsigned short *pArray, CC1, CC2;
	//чтение массива ДТМИ
				devIndex = FindWordInList(line, cwDTMI, 4);
				if ( (devIndex >= 0) && (devIndex < 4) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
						finp >> word;
						for (int i = 0; i < sizeDTMI; i++) {
							finp >> ArrayMIL32[i];
						}
						CC2 = ArrayMIL32[1];
						nAr = (CC2 - 0x7aaa) >> 12;
						if ( (nAr >=0) && (nAr < 9))  {
							memcpy(&ArrayDTMI[30*nAr], &ArrayMIL32[2], sizeof(short)*30);
							if (CC2 == 0xfaaa) {
								SwapShort((short*)&ArrayDTMI[0], (short*)&ArrayDTMI[1]);
								for (int i = 10; i < 30*7; i = i + 2) {
									SwapShort((short*)&ArrayDTMI[i], (short*)&ArrayDTMI[i+1]);
								}
								for (int i = 30*8 + 2; i <= 30*8 + 12; i = i + 2) {
									SwapShort((short*)&ArrayDTMI[i], (short*)&ArrayDTMI[i+1]);
								}
								DTMI dtmi;
								memcpy(&dtmi, ArrayDTMI, sizeof(ArrayDTMI));
								OutputDTMI(fout, SaveDir, dtmi, zeroDate, 1);
							}
						}
					}
				}

	//чтение массива МЛОК / ДТМИ по подадресу МЛОК
				devIndex = FindWordInList(line, cwMLOC, 4);
				if ( (devIndex >= 0) && (devIndex < 4) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
						finp >> word;
						for (int i = 0; i < sizeMIL; i++) {
							finp >> ArrayMIL32[i];
						}

						CC1 = ArrayMIL32[0];
						CC2 = ArrayMIL32[1];
						if (CC1 == 0x42FB) pArray = ArrayMLOC;
						else pArray = ArrayDTMI;

						nAr = (CC2 - 0x7aaa) >> 12;
						if ( (nAr >=0) && (nAr < 9) )  {
							memcpy(&pArray[30*nAr], &ArrayMIL32[2], sizeof(short)*30);
							if (CC2 == 0xfaaa) {
								if (CC1 == 0x42FB) {
									SwapShort((short*)&ArrayMLOC[0], (short*)&ArrayMLOC[1]);
									for (int i = 8; i < 30*9 - 2; i = i + 2) {
										SwapShort((short*)&ArrayMLOC[i], (short*)&ArrayMLOC[i+1]);
									}
									LOC mloc;
									memcpy(&mloc, ArrayMLOC, sizeof(ArrayMLOC));
									OutputLOC(fout, SaveDir, mloc, zeroDate, 1);
								}
								else {
									SwapShort((short*)&ArrayDTMI[0], (short*)&ArrayDTMI[1]);
									for (int i = 10; i < 30*7; i = i + 2) {
										SwapShort((short*)&ArrayDTMI[i], (short*)&ArrayDTMI[i+1]);
									}
									for (int i = 30*8 + 2; i <= 30*8 + 12; i = i + 2) {
										SwapShort((short*)&ArrayDTMI[i], (short*)&ArrayDTMI[i+1]);
									}
									DTMI dtmi;
									memcpy(&dtmi, ArrayDTMI, sizeof(ArrayDTMI));
									OutputDTMI(fout, SaveDir, dtmi, zeroDate, 1);
								}
							}
						}
					}
				}
			}
			finp.close();
		}
		fout.close();
		if (isMSHI)   { fmshi.close(); fquat.close(); }
		if (isSHTMI1) fshtmi1.close();
		if (isSHTMI2) fshtmi2.close();
		if (isDTMI)   fdtmi.close();
		if (isMLOC)   fmloc.close();

		PrepareStartDraw();
		CheckTabSheet();
		int nRow = 1;
		CalculateOrientStat(vMshiInfo, 0, nRow);
		CalculateParamStat(vDtmiInfo, 0, nRow);
		CalculateLocalStat(vLocalInfo, 0, nRow);
		SaveScreenShots(SaveDir.c_str());
		SaveTableToFile(TableStatInfo, TableStatInfo->RowCount, TableStatInfo->ColCount,
									AnsiString(SaveDir + FileTitle + "_stat.txt").c_str());
		vMshiInfo.clear();
		vLocalInfo.clear();
		vDtmiInfo.clear();
	}
}
//---------------------------------------------------------------------------
void GetAttrCW(unsigned short word, short &addr, short &subaddr, short &count)
{
	addr = (word & 0xF800) >> 11;
	subaddr = (word & 0x03E0) >> 5;
	count = (word & 0x001F);
	if (!count) count = 0x0020;
}
void OutputUSD(ofstream &file, unsigned short data)
{
	switch (data) {
	case (3): { file << "Подана команда НО\n"; break; }
	case (4): { file << "Подана команда ТО\n"; break; }
	case (5): { file << "Подана команда ЛОК\n"; break; }
	case (15): { file << "Подана команда ОТКЛР\n"; break; }
	default: break;
	}
}

int GetOrientAng(double Quat[4], TDateTime curDate, double angOr[3], double &angSun)
{
_DateTime stDate;
double Mornt[3][3];
double era1900, SunI[3], SunD[3];

	if ( !CheckQuatNorm(Quat, 0.001) ) {
		QuatToMatrix(Quat, Mornt);
		MatrixToEkvAngles(Mornt, angOr);
		DecodeDateTime(curDate, stDate.Year, stDate.Month,  stDate.Day,
							stDate.Hour, stDate.Min, stDate.Sec, stDate.mSec);

		era1900 = DateTimeToDaysJ2000(&stDate)/36525. + 1;
		SunVector(era1900, Mornt, SunI, SunD);
		angSun = acosm(SunD[2]);
		return 0;
	}
	return 1;
}

bool CheckControlSumMR(unsigned short *buf, unsigned short &lastSum)
{
	int sumInt = lastSum + buf[0];
	for (int i = 2; i < 32; i++) {
		sumInt += buf[i];
	}
	sumInt = ((sumInt & 0xFFFF0000) >> 16) + (sumInt & 0xFFFF);
	unsigned short CS = ((sumInt & 0xFFFF0000) >> 16) + (sumInt & 0xFFFF);
	lastSum = CS;

	if (CS == buf[1]) return true;
	else return false;
}

void ConvertMshiMR(TMshi_MR *mshi, TDateTime curDate, CadrInfo &mCadr)
{
	mCadr.Time = curDate.Val;
	mCadr.TimeBoard = mshi->time_sec * 0.125;
	mCadr.IsOrient = false;
//	(mshi->status1 & 0x4000) - ????!!!!
	if ((mshi->status1 & 0x8000) && !(mshi->status2 & 0xFF00)) {
		double Qdbl[4], ang[3], angSun;
		for (int i = 0; i < 4; i++) {
			 Qdbl[i] = (double)mshi->Qornt[i];
		}
		if ( !GetOrientAng(Qdbl, curDate, ang, angSun)) {
			for (int i = 0; i < 3; i++) {
				mCadr.AnglesOrient[i] = ang[i];
				mCadr.OmegaOrient[i]  = mshi->W[i];
			}
			mCadr.CountDeterObj = mshi->NumDet;
			mCadr.MeanErrorXY = mshi->mxy/10000.; //mm
			mCadr.SunAngle = angSun;
			mCadr.IsOrient = true;
		}
	}
	mCadr.StatOrient = (mshi->status2 & 0xFF00) >> 8;
//	mCadr.CountStars = mshi->NumStar;
	mCadr.CountWindows  = mshi->NumFrag;
	mCadr.CountLocalObj = mshi->NumLoc;
	mCadr.MatrixTemp = mshi->Tcmv/10.;
	mCadr.Level = mshi->ThMax;
    mCadr.MeanBright = mshi->Mean;
}

void ConvertTmiMR(TTmi_MR *tmi, TDateTime curDate, CadrInfo &mCadr)
{
	ObjectsInfo objInfo;
	objInfo.Dx = 0;
	objInfo.Dy = 0;
	objInfo.StarID = 0;
	objInfo.Mv = 0;
	objInfo.Sp[0] = '_';
	objInfo.Sp[1] = '_';
	objInfo.Bright = 100;
	objInfo.Square = 10;

	mCadr.Time = curDate.Val;
	mCadr.TimeBoard = tmi->time_sec * 0.125;
	mCadr.IsOrient = false;
	mCadr.ImageHeight = 2048;
	mCadr.ImageWidth  = 2048;

	mCadr.CountLocalObj = tmi->nLocalObj;
	if (tmi->nLocalObj <= 18) mCadr.SizeObjectsList = tmi->nLocalObj;
	else mCadr.SizeObjectsList = 18;

	mCadr.ObjectsList.clear();
	for (int i = 0; i < mCadr.SizeObjectsList; i++) {
		objInfo.X = tmi->LocXY[i][0];
		objInfo.Y = tmi->LocXY[i][1];
		mCadr.ObjectsList.push_back(objInfo);
	}
}

void ConvertRtmiMR(TRtmi_MR *rtmi, TDateTime curDate, CadrInfo &mCadr)
{
	ObjectsInfo objInfo;
	objInfo.Dx = 0;
	objInfo.Dy = 0;
	objInfo.StarID = 0;
	objInfo.Mv = 0;
	objInfo.Sp[0] = '_';
	objInfo.Sp[1] = '_';
	objInfo.Bright = 100;
	objInfo.Square = 10;

	mCadr.Time = curDate.Val;
	mCadr.TimeBoard = rtmi->time_sec * 0.125;
	mCadr.IsOrient = false;
	mCadr.ImageHeight = 2048;
	mCadr.ImageWidth  = 2048;

	mCadr.CountLocalObj = rtmi->nLocalObj;
	if (rtmi->nLocalObj <= 17) mCadr.SizeObjectsList = rtmi->nLocalObj;
	else mCadr.SizeObjectsList = 17;

//	mCadr.ObjectsList.clear();
//	for (int i = 0; i < mCadr.SizeObjectsList; i++) {
//		if (!_isnan(rtmi->Local[i].x))
//			objInfo.X = rtmi->Local[i].x;
//		if (!_isnan(rtmi->Local[i].y))
//			objInfo.Y = rtmi->Local[i].y;
//		if (!_isnan(rtmi->Local[i].bright))
//		objInfo.Bright = rtmi->Local[i].bright;
//		objInfo.Square = rtmi->Local[i].size;
//		mCadr.ObjectsList.push_back(objInfo);
//	}

//	WindowsInfo winInfo;
//	winInfo.xCenter = 0;
//	winInfo.yCenter = 0;
//	winInfo.Width  = 1;
//	winInfo.Height = 1;
//	winInfo.Xstart = 0;
//	winInfo.Ystart = 0;
//	winInfo.CountObj = 0;
//	winInfo.Mean  = 0;
//	winInfo.Sigma = 0;
//	mCadr.SizeWindowsList = 17;
//	mCadr.WindowsList.clear();
//	for (int i = 0; i < mCadr.SizeWindowsList; i++) {
//		winInfo.Level = rtmi->ThFrag[i];
//		mCadr.WindowsList.push_back(winInfo);
//	}
}

void ConvertDtmiMR(TDtmi_MR *dtmi, TDateTime curDate, CadrInfo arCadr[2], int &cntCadr)
{
//	float Tcmv, mxy;
	CadrInfo mCadr;
	ObjectsInfo objInfo;
	objInfo.Dx = 0;
	objInfo.Dy = 0;
	objInfo.StarID = 0;
	objInfo.Mv = 0;
	objInfo.Sp[0] = '_';
	objInfo.Sp[1] = '_';

	mCadr.IsOrient = false;
	mCadr.ImageHeight = 2048;
	mCadr.ImageWidth  = 2048;
//	mCadr.CountLocalObj = dtmi->NumLoc;
	mCadr.CountDeterObj = dtmi->NumDet;

	int nAr = 0, nLoc = 0;
	if (dtmi->NumFrag) {  //Слежение
		mCadr.Time = curDate.Val;
		mCadr.TimeBoard = dtmi->time_sec * 0.125;
		mCadr.CountLocalObj = dtmi->NumLoc;
		if (dtmi->NumLoc <= 30) mCadr.SizeObjectsList = dtmi->NumLoc;
		else mCadr.SizeObjectsList = 30;
		for (int i = 0; i < mCadr.SizeObjectsList; i++) {
			if (i < 15) {
				nAr = 0;  nLoc = i;
			}
			else {
				nAr = 1; nLoc = 15 - i;
			}
			if (!_isnan(dtmi->RsLoc[nAr][nLoc][0]) )
				objInfo.X = dtmi->RsLoc[nAr][nLoc][0];
			if (!_isnan(dtmi->RsLoc[nAr][nLoc][1]) )
				objInfo.Y = dtmi->RsLoc[nAr][nLoc][1];
			if (!_isnan(dtmi->RsLoc[nAr][nLoc][2]) )
				objInfo.Bright = dtmi->RsLoc[nAr][nLoc][2];
			objInfo.Square = dtmi->RsLocSize[nAr][nLoc];
			mCadr.ObjectsList.push_back(objInfo);
		}
		mCadr.CountStars = dtmi->NumStar;
		mCadr.CountWindows = dtmi->NumFrag;
		if (mCadr.CountWindows <= 15) mCadr.SizeWindowsList = dtmi->NumFrag;
		else mCadr.SizeWindowsList = 15;

		WindowsInfo winInfo;
		float cftXY = 1/32., cftMean = 1/16.;
		for (int i = 0; i < mCadr.SizeWindowsList; i++) {
			int shift = (i%2) * 4;
			winInfo.xCenter = dtmi->XYc[i][0] * cftXY;
			winInfo.yCenter = dtmi->XYc[i][1] * cftXY;
//			winInfo.Width  = 24;  //tmp
//			winInfo.Height = 24;  //tmp
			GetSizeFrag_MR( ((dtmi->MultXY[i>>1] >> shift) & 0x0F),
							&winInfo.Width, &winInfo.Height);
			winInfo.Xstart = winInfo.xCenter - (winInfo.Width >> 1);
			winInfo.Ystart = winInfo.yCenter - (winInfo.Height >> 1);
			winInfo.CountObj = (int)((dtmi->ObjFrag[i>>1] >> shift) & 0x0F);
			winInfo.Mean  = dtmi->MeanFrag[i][0] * cftMean;
			winInfo.Sigma = dtmi->MeanFrag[i][1] * cftMean;
			winInfo.Level = dtmi->ThFrag[i];
			winInfo.ZipX = dtmi->ZipXY[i] & 0x0F;
			winInfo.ZipY = dtmi->ZipXY[i] >> 4;

			mCadr.WindowsList.push_back(winInfo);
		}
		arCadr[0] = mCadr;
		cntCadr = 1;
	}
	else {  // НО/ТО
		double dt[2] = {dtmi->deltaT/1000., 0};
		for (int nCadr = 0; nCadr < 2; nCadr++) {
			mCadr.Time = curDate.Val - dt[nCadr]/86400.;
			mCadr.TimeBoard = dtmi->time_sec * 0.125 - dt[nCadr];
			mCadr.CountLocalObj = dtmi->nLocalObj[nCadr];
			if (dtmi->nLocalObj[nCadr] <= 15) mCadr.SizeObjectsList = dtmi->nLocalObj[nCadr];
			else mCadr.SizeObjectsList = 15;

			mCadr.ObjectsList.clear();
			for (int i = 0; i < mCadr.SizeObjectsList; i++) {
				if (!_isnan(dtmi->RsLoc[nCadr][i][0]))
					objInfo.X = dtmi->RsLoc[nCadr][i][0];
				if (!_isnan(dtmi->RsLoc[nCadr][i][1]))
					objInfo.Y = dtmi->RsLoc[nCadr][i][1];
				if (!_isnan(dtmi->RsLoc[nCadr][i][2]))
					objInfo.Bright = dtmi->RsLoc[nCadr][i][2];
				objInfo.Square = dtmi->RsLocSize[nCadr][i];
				mCadr.ObjectsList.push_back(objInfo);
			}
			arCadr[nCadr] = mCadr;
		}
		cntCadr = 2;
	}
}

void ConvertMlocMR(TMloc_MR *mloc, TDateTime curDate, CadrInfo &mCadr)
{
	ObjectsInfo objInfo;
	objInfo.Dx = 0;
	objInfo.Dy = 0;
	objInfo.StarID = 0;
	objInfo.Mv = 0;
	objInfo.Sp[0] = '_';
	objInfo.Sp[1] = '_';

	mCadr.Time = curDate.Val;
	mCadr.TimeBoard = mloc->time_sec * 0.125;
	mCadr.IsOrient = false;
	mCadr.ImageHeight = 2048;
	mCadr.ImageWidth  = 2048;
	mCadr.CountLocalObj = mloc->nLocalObj;
	if (mloc->nFixedObj <= 36) mCadr.SizeObjectsList = mloc->nFixedObj;
	else mCadr.SizeObjectsList = 36;
	mCadr.MeanBright = mloc->mean;
	mCadr.SigmaBright = mloc->sigma;
    mCadr.Level = mloc->Th;
	mCadr.CountDeterObj = 0;

	mCadr.ObjectsList.clear();
	for (int i = 0; i < mCadr.SizeObjectsList; i++) {
		objInfo.X = mloc->RsLoc[i][0];
		objInfo.Y = mloc->RsLoc[i][1];
		objInfo.Bright = mloc->RsLoc[i][2];
		objInfo.Square = mloc->RsLocSize[i];
		mCadr.ObjectsList.push_back(objInfo);
	}
}

void TFormGraphOrient::DrawOrientInfo(CadrInfo mCadr, TDateTime curDate, AnsiString title, int num)
{
	TColor curColor;
    double timeX;

	if ( (num >= 0) && (num < MaxStarTrack) ) {

		int nSeries = num;
		plotter->SetSeriesColor(arColorOk[num]);
		plotter->SetTitle(title);
		if (FormAnimateSetting->CheckBoxDateTime->Checked) {
			plotter->SetDateTimeX(true);
			timeX = curDate.Val; //mCadr.Time;
		}
		else {
			plotter->SetDateTimeX(false);
			timeX = mCadr.TimeBoard;
		}

		if (mCadr.IsOrient) {
			curColor = arColorOk[num];
			plotter->AddPoint(ChartAl, nSeries, timeX, mCadr.AnglesOrient[0] * RTD, curColor);
			plotter->AddPoint(ChartDl, nSeries, timeX, mCadr.AnglesOrient[1] * RTD, curColor);
			plotter->AddPoint(ChartAz, nSeries, timeX, mCadr.AnglesOrient[2] * RTD, curColor);
			plotter->AddPoint(ChartSunAngle, nSeries, timeX, mCadr.SunAngle * RTD, curColor);
			plotter->AddPoint(ChartWx, nSeries, timeX, mCadr.OmegaOrient[0]  * RTS, curColor);
			plotter->AddPoint(ChartWy, nSeries, timeX, mCadr.OmegaOrient[1] * RTS, curColor);
			plotter->AddPoint(ChartWz, nSeries, timeX, mCadr.OmegaOrient[2] * RTS, curColor);
			plotter->AddPoint(ChartMxy, nSeries, timeX, mCadr.MeanErrorXY * 1000., curColor);
			plotter->AddPoint(ChartNumDet,  nSeries, timeX, mCadr.CountDeterObj, curColor);
		}
		else curColor = arColorErr[num];

		plotter->AddPoint(ChartNumFrag, nSeries, timeX, mCadr.CountWindows, curColor);
		plotter->AddPoint(ChartNumLoc,  nSeries, timeX, mCadr.CountLocalObj, curColor);
		plotter->AddPoint(ChartLevel,   nSeries, timeX, mCadr.Level, curColor);
		plotter->AddPoint(ChartFone,   nSeries, timeX, mCadr.MeanBright, curColor);

		if (mCadr.StatOrient) {
			plotter->AddPoint(ChartStat, nSeries, timeX, mCadr.StatOrient, curColor);
		}
		if (mCadr.MatrixTemp < 6000) {
			plotter->AddPoint(ChartTemp, nSeries, timeX, mCadr.MatrixTemp, curColor);
		}
		AddCadrInfo(mCadr, StarTrack[num]);
	}
}

void TFormGraphOrient::PrintLogSKF(ofstream &file, CadrInfo mCadr, bool &create)
{
		if (!create) {
			file << "Day/Time\t" << "Tbshv\t";
			file << "nLocalObj\t"; //Tcmv - ?
			file << "\n";
			create = true;
		}
		file << dec << setfill(' ');
		TDateTime curDate;
		curDate.Val = mCadr.Time;
		file << OutputDateTime(curDate).c_str() << "\t";
		file << mCadr.TimeBoard << "\t";
		for (int i = 0; i < mCadr.SizeObjectsList; i++) {
			if (CheckSelectObject(objSelect, mCadr.ObjectsList[i].X, mCadr.ObjectsList[i].Y,
											mCadr.ObjectsList[i].Bright, mCadr.ObjectsList[i].Square)) {
				file << mCadr.ObjectsList[i].X << "\t";
				file << mCadr.ObjectsList[i].Y << "\t";
				file << mCadr.ObjectsList[i].Bright << "\t";
				file << mCadr.ObjectsList[i].Square << "\t";
			}
		}
		file << "\n";
}

void arifmetica( int a, int b, int sum = 10, int mult = 20)
{
	sum = a + b;
    mult = a * b;
}

void __fastcall TFormGraphOrient::MenuOpenMR_MILClick(TObject *Sender)
{
	TDateTime dtTest;
	const int sizeMIL = 32;
	wstring line,word;
	unsigned short bufMIL[sizeMIL], lastSum = 0;
	unsigned short lastSumTmi = 0, lastSumRtmi = 0;
	unsigned short lastSumDtmi = 0, lastSumMloc = 0;
	unsigned short bufRtmi[sizeMIL * 8];
	unsigned short bufDtmi[sizeMIL * 12];
	unsigned short bufMloc[sizeMIL * 9];
	unsigned short cmdWord, ansWord;
	short addr, subaddr, cntWord;
	bool isSHTMI1 = false, isSHTMI2 = false, isMSHI = false, isFileSKF = false,
		 isTMI = false, isRTMI = false, isDTMI = false, isMLOC = false, isFragRtmi = false;
	ofstream fshtmi1, fshtmi2, fmshi, ftmi, frtmi, fdtmi, fmloc, ffrag, fskf;

	OpenDialog->Options.Clear();
	OpenDialog->Filter = "Текстовые файлы|*.dat; *.txt|Все файлы|*.*";
	OpenDialog->Options << ofAllowMultiSelect;

	if (OpenDialog->Execute()) {

		AnsiString SaveDir  = ExtractFileDir(OpenDialog->FileName) + "/Result/";
		CreateDir(SaveDir);
//		GetFileTitles(OpenDialog->FileName,&FileTitle);

		TFormatSettings curFormat;
		curFormat.ShortDateFormat = "dd.mm.yyyy";
		TDateTime zeroDate = StrToDate(FormAnimateSetting->ZeroDate1S->Text)
						   + StrToTime(TimeToStr(FormAnimateSetting->ZeroTime1S->Time));
		TDateTime curDate = zeroDate;

		GetFileTitles(OpenDialog->Files->Strings[0],&FileTitle);
		FileTitle += "-"+IntToStr(OpenDialog->Files->Count);
		ofstream finfo((SaveDir + FileTitle + "_info.txt").c_str());

		StarTrack[0].clear();
		for (int iFile = 0; iFile < OpenDialog->Files->Count; iFile++) {

			wifstream finp(AnsiString(OpenDialog->Files->Strings[iFile]).c_str());
			finp.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			if (!finp.is_open()) {
				ShowMessage("Файл не может быть открыт!");
				return;
			}
			SetClkBokz(0.125);
			finp >> hex;
			int Tbshv = 0;
			while (!finp.eof())
			{
				finp >> word;
				if (word == L"Такт") {
					getline(finp, line, L'\n' );
					finp >> word >> Tbshv;
				}
				if ((word == L"CW:") || (word == L"КС:") ) {      //if (FindFirstHex(finp, "CW:", cmdWord))
					finp >> cmdWord;
					GetAttrCW(cmdWord, addr, subaddr, cntWord);
					getline(finp, line, L'\n' );
					finp >> word;
					if ( (word == L"DW:") || (word == L"ИС:") ) {
						for (int i = 0; i < cntWord; i++) {
							finp >> bufMIL[i];
						}
						if (subaddr == 16) OutputUSD(finfo, bufMIL[0]);
						else if (subaddr == 17) {
							struct SINCHRO *sinc = (struct SINCHRO *)bufMIL;
//							if (FormAnimateSetting->CheckBoxSinc->Checked) {
//								zeroDate.Val = EpochToDateTime(sinc->curEpoch).Val
//											 - sinc->countClk/86400.;
//							}
						}
					}
					else if ( (word == L"AW:") || (word == L"ОС:") ) {  //if (FindFirstHex(finp, "AW:", ansWord))
						finp >> ansWord;  //
						getline(finp, line, L'\n' );
						if (!(ansWord & 0x07FF) ) {
							finp >> word;
							for (int i = 0; i < cntWord; i++) {
								finp >> bufMIL[i];
							}
							switch (subaddr) {
								case (2): {
									TShtmi1_MR *shtmi1 = (TShtmi1_MR *)bufMIL;
									if (!isSHTMI1) {
										fshtmi1.open((SaveDir + FileTitle + "_shtmi1.txt").c_str());
									}
									PrintLogShtmi1_MR(fshtmi1, *shtmi1, curDate, isSHTMI1);
									break;
								}
								case (3): {
									if (!isSHTMI2) {
										fshtmi2.open((SaveDir + FileTitle + "_shtmi2.txt").c_str());
									}
                                    TShtmi2_MR *shtmi2 = (TShtmi2_MR *)bufMIL;
									curDate =  GetDateTime(zeroDate, shtmi2->time_sec);
									PrintLogShtmi2_MR(fshtmi2, *shtmi2, curDate, isSHTMI2);
									break;
								}

								case (4): {
									int nAr = bufMIL[0] - 0x1001;
//									if (!nAr)
									lastSumDtmi = 0;
									if (!CheckControlSumMR(bufMIL, lastSumDtmi)) {
											finfo << Tbshv << ": error CS (ДТМИ, ";
											finfo << hex << "0x" << bufMIL[0] << ")\n" << dec;
										}
									if ( (nAr >=0) && (nAr < 12) ) {
										memcpy(&bufDtmi[nAr*30], &bufMIL[2], sizeof(short)*30);
										if (nAr == 11) {
											if (!isDTMI) {
												fdtmi.open((SaveDir + FileTitle + "_dtmi.txt").c_str());
											}
											TDtmi_MR *dtmi = (TDtmi_MR *)bufDtmi;
											curDate =  GetDateTime(zeroDate, dtmi->time_sec);
											finfo << uppercase << hex << setfill('0');
											finfo << hex << "Получение ДТМИ: ";
											finfo << "Stat1 = " << "0x" << setw(4) << dtmi->status1 << ", ";
											finfo << "Stat2 = " << "0x" << setw(4) << dtmi->status2 << "\n";
											PrintDtmi_MR(finfo, *dtmi, curDate);
											PrintLogDtmi_MR(fdtmi, *dtmi, curDate, isDTMI);
											CadrInfo mCadr[2];
											int n;
											ConvertDtmiMR(dtmi, curDate, mCadr, n);
											for (int i = 0; i < n; i++) {
												AddCadrInfo(mCadr[i], vCadrInfo);
											}
										}
									}
									break;
								}
								case (5): {
									int nAr = bufMIL[0] - 0x2001;
//									if (!nAr)
									lastSumMloc = 0;
									if (!CheckControlSumMR(bufMIL, lastSumMloc)) {
											finfo << Tbshv << ": error CS (МЛОК, ";
											finfo << hex << "0x" << bufMIL[0] << ")\n" << dec;
									}
									if ( (nAr >=0) && (nAr < 9) ) {
										memcpy(&bufMloc[nAr*30], &bufMIL[2], sizeof(short)*30);
										if (nAr == 8) {
											if (!isMLOC) {
												fmloc.open((SaveDir + FileTitle + "_mloc.txt").c_str());
											}
											TMloc_MR *mloc = (TMloc_MR *)bufMloc;
											curDate =  GetDateTime(zeroDate, mloc->time_sec);
											finfo << uppercase << hex << setfill('0');
											finfo << hex << "Получение МЛОК: ";
											finfo << "Stat1 = " << "0x" << setw(4) << mloc->status1 << ", ";
											finfo << "Stat2 = " << "0x" << setw(4) << mloc->status2 << "\n";
											PrintMloc_MR(finfo, *mloc, curDate);
											PrintLogMloc_MR(fmloc, *mloc, curDate, isMLOC);
											CadrInfo mCadr;
											ConvertMlocMR(mloc, curDate, mCadr);
//											AddCadrInfo(mCadr, vCadrInfo);
											if (!isFileSKF){
												fskf.open(AnsiString(SaveDir + FileTitle + "_skf.txt").c_str());
											}
											PrintLogSKF(fskf, mCadr, isFileSKF);
										}
									}
									break;
								}
								case (6): {
									lastSumTmi = 0;
									if (!CheckControlSumMR(bufMIL, lastSumTmi)) {
										finfo << Tbshv << ": error CS (ТМИ, ";
										finfo << hex << "0x" << bufMIL[0] << ")\n" << dec;
									}

									TTmi_MR tmi;
									unsigned short bufTmi[60];
									//before september 2019
									if (bufMIL[0] == 0x3001) {
										memcpy(&bufTmi[0], &bufMIL[2], sizeof(short)*30);
									}
									else if (bufMIL[0] == 0x3002) {
										memcpy(&bufTmi[30], &bufMIL[2], sizeof(short)*30);
										if (!isTMI) {
											ftmi.open((SaveDir + FileTitle + "_tmi.txt").c_str());
										}
										memcpy(&tmi, &bufTmi, sizeof(short) * 12);

										for (int i = 0; i < 18; i++) {
											int X = 0, Y = 0;
											int j = 6 + i * 3;
											X = *( (int*)(&bufTmi[j]) );
											X = (X & 0xFFFFFF);
											Y = *( (int*)(&bufTmi[j+1]) );
											Y = Y >> 8;
											tmi.LocXY[i][0] = (float)X / pow(2.,12);
											tmi.LocXY[i][1] = (float)Y / pow(2.,12);
										}
										curDate = GetDateTime(zeroDate, tmi.time_sec);
										PrintLogTmi_MR(ftmi, tmi, curDate, isTMI);
//										CadrInfo mCadr;
//										ConvertTmiMR(&tmi, curDate, mCadr);
////										if (vCadrInfo.size()) {
////											int nn = vCadrInfo.size() - 1;
////											for (int i = 0; i < mCadr.SizeObjectsList; i++) {
////												if (i < vCadrInfo[nn].SizeObjectsList) {
////													mCadr.ObjectsList[i].Dx = mCadr.ObjectsList[i].X
////																			- vCadrInfo[nn].ObjectsList[i].X;
////													mCadr.ObjectsList[i].Dy = mCadr.ObjectsList[i].Y
////																			- vCadrInfo[nn].ObjectsList[i].Y;
////												}
////											}
////										}
//										AddCadrInfo(mCadr, vCadrInfo);
									}
									break;
								}
								case (7): {
									int nAr = (bufMIL[0] - 0x4001) & 0xF00F;
//									if (!nAr)   //before december 2019
									lastSumRtmi = 0;
									if (!CheckControlSumMR(bufMIL, lastSumRtmi)) {
										finfo << Tbshv << ": error CS (РТМИ, ";
										finfo << hex << "0x" << bufMIL[0] << ")\n" << dec;
									}

									switch (nAr) {
										case (0):
										case (1):
										case (2): { memcpy(&bufRtmi[nAr*30], &bufMIL[2], sizeof(short)*30); break; }
										case (3): { memcpy(&bufRtmi[nAr*30], &bufMIL[2], sizeof(short)*29); break; }
										case (4): { memcpy(&bufRtmi[nAr*30-1], &bufMIL[2], sizeof(short)*30);
											if (!isRTMI) {
												frtmi.open((SaveDir + FileTitle + "_rtmi.txt").c_str());
											}
											TRtmi_MR *rtmi = (TRtmi_MR *)bufRtmi;
											curDate = GetDateTime(zeroDate, rtmi->time_sec);
											PrintLogRtmi_MR(frtmi, *rtmi, curDate, isRTMI);
//											CadrInfo mCadr;
//											ConvertRtmiMR(rtmi, curDate, mCadr);
//											AddCadrInfo(mCadr, vCadrInfo);
											break;
										}
										case (5): {
											if (!isFragRtmi) {
												TRtmiFrag_MR fragInfo;
												memcpy(&fragInfo, &bufMIL[2], sizeof(fragInfo));
												//сделать лог-файл: время привязки, время экспозиции, число окон

												AnsiString FragTitle, FragDir  = SaveDir + "/Frag/";
												TFormatSettings fragFormat;
												curFormat.ShortDateFormat = "yyyy_mm_dd";
												curFormat.LongTimeFormat  = "hh_nn_ss_zzz";
												TDateTime curDateFrag = GetDateTime(zeroDate, fragInfo.time_sec);
												FragTitle.sprintf("%dms_%s_%s_(%.3f).txt", fragInfo.timeExp,
														  AnsiString(DateToStr(curDateFrag, curFormat)).c_str(),
														  AnsiString(TimeToStr(curDateFrag, curFormat)).c_str(),
														  fragInfo.time_sec * 0.125);
												CreateDir(FragDir);
												if (fragInfo.NumFrag) {
													ffrag.open((FragDir + FragTitle + ".bin").c_str(), ios::binary);
													isFragRtmi = true;
												}
											}
										}
										case (6):
										case (7): {
											if (isFragRtmi) {
//												unsigned short clkRtmi = bufMIL[0] & 0xFFF;
//												ffrag.write((char*)&clkRtmi, sizeof(short));
												ffrag.write((char*)&bufMIL[2], sizeof(short)*30);
											}
											break;
										}
										default: break;
									}
									break;
								}
								case (16): {
									TMshi_MR *mshi = (TMshi_MR *)bufMIL;
									if (!isMSHI) {
										fmshi.open((SaveDir + FileTitle + "_mshi.txt").c_str());
									}
									curDate =  GetDateTime(zeroDate, mshi->time_sec);
									PrintLogMshi_MR(fmshi, *mshi, curDate, isMSHI);
									CadrInfo mCadr;
									ConvertMshiMR(mshi, curDate, mCadr);
									AnsiString title = "БОКЗ-МР № " + IntToStr(mshi->serial);
									DrawOrientInfo(mCadr, curDate, title, 0);
									break;
								}
								default: break;
							}
						}
					}
				}
			}
			finp.close();
//			fshtmi1.close();
//			fshtmi2.close();
//			fmshi.close();
//			ftmi.close();
//			frtmi.close();
//			ffrag.close();
		}
        finfo.close();
		fshtmi1.close();
		fshtmi2.close();
		fmshi.close();
		ftmi.close();
		frtmi.close();
		ffrag.close();
        fskf.close();

//      Check it !!!!
//		if (objSelect.isCoordX && objSelect.isCoordY) {
//			bool isFileSKF;
//			ofstream fskf(AnsiString(SaveDir + FileTitle + "_skf.txt").c_str());
//			for (int i = 0; i < vCadrInfo.size(); i++) {
//				PrintLogSKF(fskf, vCadrInfo[i], isFileSKF);
//			}
//			fskf.close();
//        }

		if (FormAnimateSetting->CheckBoxSaveIki->Checked) {
			SetCurrentDir(SaveDir);
			for (int i = 0; i < vCadrInfo.size(); i++) {
				writeProtocolToIKI(vCadrInfo[i], -1);
			}
		}

		int nRow = 1;
		if (StarTrack[0].size()) {
				TableStatInfo->Cells[0][nRow++] = "БОКЗ-МР";
				CalculateOrientStat(StarTrack[0], 0, nRow);
				CalculateParamStat(StarTrack[0], 0, nRow);
		}

		CheckTabSheet();
        PrepareStartDraw();
		SaveScreenShots(SaveDir.c_str());
		SaveTableToFile(TableStatInfo, TableStatInfo->RowCount, TableStatInfo->ColCount,
									AnsiString(SaveDir + FileTitle + "_stat.txt").c_str());
	}
}
//---------------------------------------------------------------------------
const int CountTg = 6;
struct TTmiBI_MR
{
	float PowerIP[2], PowerVIP[2];
	float AmpFid1[2], AmpFid2[2];
	float TempTg1[CountTg][2], TempTg2[CountTg][2];
};

	size_t findLineW(wifstream& in, const wstring& words, wstring &resStr) {
		wstring curLine;
		while (getline(in, curLine)) {
			if (curLine.find(words) != string::npos) {
			size_t nn = words.size();
				resStr = curLine.substr(curLine.find(words) + words.size());
				return (unsigned int)in.tellg();
			}
		}
		return string::npos;
	}

int TryReadBI_MR(wifstream &finp, struct TTmiBI_MR &tmi)
{
	wstring line;
	if (findLineW(finp, L"Напряжение источника питания", line) != string::npos) {
		swscanf(line.c_str(),L"%f %f", &tmi.PowerIP[0], &tmi.PowerIP[1]);
	}
	else return 0;
	if (findLineW(finp, L"Напряжение ВИП", line) != string::npos) {
		swscanf(line.c_str(),L"%f %f", &tmi.PowerVIP[0], &tmi.PowerVIP[1]);
	}
	else return 0;
	if (findLineW(finp, L"Ток на фидере 1 прибора", line) != string::npos) {
		swscanf(line.c_str(),L"%f %f", &tmi.AmpFid1[0], &tmi.AmpFid1[1]);
	}
	if (findLineW(finp, L"Ток на фидере 2 прибора", line) != string::npos) {
		swscanf(line.c_str(),L"%f %f", &tmi.AmpFid2[0], &tmi.AmpFid2[1]);
	}
	else return 0;

	wstring findWordTg[2]= {L"Температура 1 термогруппы", L"Температура 2 термогруппы"};
	for (int i = 0; i < 2; i++) {
		for (int iTg = 0; iTg < CountTg; iTg++) {
			if (findLineW(finp, findWordTg[i], line) != string::npos) {
				int nTg;
				float T1,T2;
				swscanf(line.c_str(),L"%d %f %f", &nTg, &T1, &T2);
				nTg--;
				if ((nTg >= 0) && (nTg < CountTg)) {
					if (!i) {
						tmi.TempTg1[nTg][0] = T1;
						tmi.TempTg1[nTg][1] = T2;
					}
					else {
						tmi.TempTg2[nTg][0] = T1;
						tmi.TempTg2[nTg][1] = T2;
					}

				}
			}
            else return 0;
		}
	}

	return 1;
}

void PrintBI_MR(ofstream &file, const TTmiBI_MR &tmi, TDateTime curDate, bool &create)
{
	if (!create) {
		file << "Date&Time\t";
		file << "PowerIP[0]\t" << "PowerIP[1]\t";
		file << "PowerVIP[0]\t" << "PowerVIP[1]\t";
		file << "AmpFid1[0]\t" << "AmpFid1[1]\t";
		file << "AmpFid2[0]\t" << "AmpFid2[1]\t";
		for (int iTg = 0; iTg < CountTg; iTg++) {
			file << "T1-" << (iTg+1) << "[0]" << "\t";
			file << "T1-" << (iTg+1) << "[1]" << "\t";
		}
		for (int iTg = 0; iTg < CountTg; iTg++) {
			file << "T2-" << (iTg+1) << "[0]" << "\t";
			file << "T2-" << (iTg+1) << "[1]" << "\t";
		}
		file << "\n";
		create = true;
	 }

	 file << OutputDateTime(curDate).c_str() << "\t";
	 file << tmi.PowerIP[0] << "\t" << tmi.PowerIP[1]<< "\t";
	 file << tmi.PowerVIP[0] << "\t" << tmi.PowerVIP[1]<< "\t";
	 file << tmi.AmpFid1[0] << "\t" << tmi.AmpFid1[1] << "\t";
	 file << tmi.AmpFid2[0] << "\t" << tmi.AmpFid2[1] << "\t";

	 for (int iTg = 0; iTg < CountTg; iTg++) {
		file << tmi.TempTg1[iTg][0] << "\t" << tmi.TempTg1[iTg][1] << "\t";
	 }

	 for (int iTg = 0; iTg < CountTg; iTg++) {
		file << tmi.TempTg2[iTg][0] << "\t" << tmi.TempTg2[iTg][1] << "\t";
	 }

	 file << "\n";
}

void __fastcall TFormGraphOrient::MenuOpenMR_BIClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Filter = "Текстовые файлы|*.dat; *.txt|Все файлы|*.*";
	OpenDialog->Options << ofAllowMultiSelect;
	if (OpenDialog->Execute()) {

		AnsiString SaveDir  = ExtractFileDir(OpenDialog->FileName) + "/Result/";
		CreateDir(SaveDir);

		GetFileTitles(OpenDialog->Files->Strings[0],&FileTitle);
		FileTitle += "-"+IntToStr(OpenDialog->Files->Count);
		ofstream finfo((SaveDir + FileTitle + "_info.txt").c_str());

		for (int iFile = 0; iFile < OpenDialog->Files->Count; iFile++) {
			wifstream finp(AnsiString(OpenDialog->Files->Strings[iFile]).c_str());
			finp.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			if (!finp.is_open()) {
				ShowMessage("Файл не может быть открыт!");
				return;
			}
//			GetFileTitles(OpenDialog->Files->Strings[iFile],&FileTitle);
			bool isBI_MR = false;

			finp >> dec;
			while (!finp.eof())
			{
				wstring line;
				getline(finp, line, L'\n' );
				if (line.find(L"BI_GetSecMarkTelemetry") != string::npos) {
					unsigned int year, month, day, hour, min, sec, msec;
					swscanf(line.c_str(), L"%d.%d.%d %d:%d:%d.%d", &year, &month, &day, &hour, &min, &sec, &msec);
					TDateTime curDate = EncodeDateTime(year, month, day, hour, min, sec, msec);
					TTmiBI_MR tmiBI;
					if (TryReadBI_MR(finp, tmiBI)) {
						PrintBI_MR(finfo, tmiBI, curDate, isBI_MR);
					}
				}
			}
            finp.close();
		}
		finfo.close();
	}
}
//---------------------------------------------------------------------------

//struct TestBOKZM
//{
//	unsigned long timeBOKZ;
//	unsigned short cwWatch;
//	unsigned short cwExchange;
//	unsigned short codeOC;
//};

TDateTime timeTestToDateTime(unsigned long timeMshi)
{
	TDateTime curDate;
	curDate = EncodeDate(2008, 1, 1) + EncodeTime(0,0,0,0);
	curDate += (double)timeMshi * 0.2/86400;

	return curDate;
}

void __fastcall TFormGraphOrient::MenuOpenEnergyClick(TObject *Sender)
{
	unsigned short cwWatch[3] = {0x45ec, 0x65ec, 0x6dec};
	unsigned short cwCoord[3] = {0x4640, 0x6640, 0x6e40};
	unsigned short cwMshiA[3] = {0x4600, 0x6600, 0x6e00};
	unsigned short cwDtmiA[3] = {0x44a0, 0x64a0, 0x6ca0};
	unsigned short codeOC[3]  = {0x4000, 0x6000, 0x6800};

	const int MSHISize = 22;
	const int DTMISize = 290;
	const int TMISize  = 722;
	const int SHTMI1Size = 32;
	const int SHTMI2Size = 32;

	const int sizeWatch  = 12;
	const int cntArCoord = 6;
	const int sizeCoord  = 32;	
	const int sizeTest   = 259;		

	unsigned short ArrayMSHI[MSHISize];
	unsigned short ArrayDTMI[DTMISize];
	unsigned short ArraySHTMI1[SHTMI1Size];
	unsigned short ArraySHTMI2[SHTMI2Size];
	unsigned short ArrayTMI[TMISize];
	unsigned short ArrayTest[sizeTest];
	unsigned short ArrayWatch[sizeWatch];
	unsigned short ArrayCoord[sizeCoord * cntArCoord];
	
	ofstream fmshi, fshtmi1, fshtmi2, fdtmi, ftemp, fpow;
	bool isMSHI = 0, isSHTMI1 = 0, isSHTMI2 = 0, isDTMI = 0, isTemp = 0, isPower = 0;

	bool isLoadDb = FormAnimateSetting->CheckBoxLoadToDb->Checked;

	vector <GeneralizedMSHIOR> vGeneralMSHI;
	vector <GeneralizedDTMI>   vGeneralDTMI;
	vector <GeneralizedSHTMI1> vGeneralSHTMI1;
	vector <GeneralizedSHTMI2> vGeneralSHTMI2;

	OpenDialog->Options.Clear();
	OpenDialog->Filter = "txt|*.txt";
	if (OpenDialog->Execute()) {

		vCadrInfo.clear();
		DeleteLineGraph();

		AnsiString FileName = OpenDialog->FileName;
		AnsiString CurDir   = ExtractFileDir(FileName);
		AnsiString SaveDir  = CurDir + "/Result/";
		CreateDir(SaveDir);
//		SetCurrentDir(CurDir);
		GetFileTitles(FileName,&FileTitle);

		ifstream finp(FileName.c_str());
		if (!finp.is_open()) {
			ShowMessage("Файл не может быть открыт!");
			return;
		}

		isSHTMI1 = false; isSHTMI2 = false;
		isDTMI = false, isMLOC = false, isOpenDtmi = false;

		int devIndex;
		ofstream fwatch[3], fcoord[3];
		AnsiString fname;
		for (devIndex = 0; devIndex < 3; devIndex++) {
			fname = SaveDir + FileTitle + "_watch_" + IntToStr(devIndex+1) + ".txt";
			fwatch[devIndex].open(fname.c_str());
			fwatch[devIndex] << "Date&Time\t" << "timeBOKZ\t";
			fwatch[devIndex] << "Mean\t" << "Sigma\t" << "Th\t";
			fwatch[devIndex] << "NumLoc\t" << "NumDet\t";
			fwatch[devIndex] << "NumFrag\t" << "Omega, ''/s\t";
			fwatch[devIndex] << "deltaT, s\t" << "NumSec\n";

			fname = SaveDir + FileTitle + "_loc_" + IntToStr(devIndex+1) + ".txt";
			fcoord[devIndex].open(fname.c_str());
		}

		ofstream fout((SaveDir + FileTitle + "_decode.txt").c_str());
		ofstream fnew((SaveDir + FileTitle + "_test.txt").c_str());
		string line;

		while (!finp.eof())
		{
			getline(finp, line, '\n' );

//чтение массива МШИОР
			if (line.find("МШИОР") != std::string::npos) {
				getline(finp, line, '\n' );
				if (line.find("ТМОС") != std::string::npos) {
					char cdate[20], ctime[20];
					sscanf(line.c_str(), "ТМОС %s %s", &cdate, &ctime);
					string sdate = cdate, stime = ctime;

					int errorMSHI = ReadTMIArray(finp, "CISO_MSHIOR", ArrayMSHI, MSHISize);

					if (!errorMSHI) {
						struct MSHI_BOKZM mshi;
						memcpy(&mshi, ArrayMSHI, sizeof(mshi));
						fout<<"\n"<<line<<"\n";
						PrintMSHI_BOKZM(fout, mshi);

						double ang[3], MorntT[3][3];

						for (int i = 0; i < 3; i++) {
							for (int j = 0; j < 3; j++) {
								MorntT[i][j] =  mshi.Mornt[j][i];
							}
						}

						MatrixToEkvAngles(MorntT, ang);

						if (mshi.status1 == 0xE004) {
							plotter->AddPoint(ChartAl, 0, mshi.timeBOKZ, ang[0] * RTD);
							plotter->AddPoint(ChartDl, 0, mshi.timeBOKZ, ang[1] * RTD);
							plotter->AddPoint(ChartAz, 0, mshi.timeBOKZ, ang[2] * RTD);
						}

						if (!isMSHI) {
							isMSHI = true;
							fmshi.open((SaveDir + FileTitle + "_mshi.txt").c_str());
							fmshi << "Date&Time\t" << "Tbokz\t";
							fmshi << "KC1\t" << "KC2\t";
							fmshi << "\n";
						}

						fmshi << sdate << " " << stime << "\t";
						fmshi << std::setprecision(8);
						fmshi << mshi.timeBOKZ << "\t";
						fmshi << uppercase<<hex<<setfill('0');
						fmshi << "0x" << setw(4) << mshi.status1 << "\t";
						fmshi << "0x" << setw(4) << mshi.status2 << "\t";
						fmshi << dec<<setfill(' ');

						fmshi << ang[0] * RTD << "\t";
						fmshi << ang[1] * RTD << "\t";
						fmshi << ang[2] * RTD << "\n";

//						if (isLoadDb) {
//							TDateTime saveTime = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
//							GeneralizedMSHIOR gMSHI = clientDb->convertMSHI_BOKZM(mshi, saveTime, 0);
//							vGeneralMSHI.push_back(gMSHI);
//						}
					}
				}
			}

//чтение массива ШТМИ1
			if (line.find("ШТМИ1") != std::string::npos) {
				getline(finp, line, '\n' );
				if (line.find("ТМОС") != std::string::npos) {
					char cdate[20], ctime[20];
					sscanf(line.c_str(), "ТМОС %s %s", &cdate, &ctime);
					string sdate = cdate, stime = ctime;

					int errorSHTMI1 = ReadTMIArray(finp, "CISO_SHTMI1", ArraySHTMI1, SHTMI1Size);
					if (!errorSHTMI1) {
						struct SHTMI1_BOKZM shtmi1;
						memcpy(&shtmi1, &ArraySHTMI1[2], sizeof(shtmi1));
						fout << "\n" << line << "\n";
						PrintSHTMI1_BOKZM(fout, shtmi1);

						if (!isSHTMI1) {
							isSHTMI1 = true;
							fshtmi1.open((SaveDir + FileTitle + "_shtmi1.txt").c_str());
							fshtmi1 << "Date&Time\t" << "Tbokz\t";
							fshtmi1 << "KC1\t" << "KC2\t" << "POST\t" << "№\t" << "Texp\t";
							fshtmi1 << "Foc\t" << "Xg\t" << "Yg\t";
							fshtmi1 << "Mean\t" << "Sigma\t";
							fshtmi1 << "Ndef\t" << "Date\t";
							fshtmi1 << "verXCF\t" << "verProg\t";
							fshtmi1 << "\n";
						}

						fshtmi1 << sdate << " " << stime << "\t";
						fshtmi1 << shtmi1.timeBOKZ << "\t";
						fshtmi1 << uppercase << hex << setfill('0');
						fshtmi1 << "0x" << setw(4) << shtmi1.status1 << "\t";
						fshtmi1 << "0x" << setw(4) << shtmi1.status2 << "\t";
						fshtmi1 << "0x" << setw(4) << shtmi1.post << "\t";
						fshtmi1 << dec << setfill(' ');
						fshtmi1 << shtmi1.serialNumber << "\t";
						fshtmi1 << shtmi1.timeExp << "\t" << shtmi1.Foc << "\t";
						fshtmi1 << shtmi1.Xg << "\t" << shtmi1.Yg << "\t";
						fshtmi1 << shtmi1.Mean << "\t" << shtmi1.Sigma << "\t";
						fshtmi1 << shtmi1.countDefect << "\t" << shtmi1.Date << "\t";
						fshtmi1 << shtmi1.verXCF << "\t" << shtmi1.verProg << "\n";

						if (isLoadDb) {
							TDateTime saveTime = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
							GeneralizedSHTMI1 gSHTMI1 = clientDb->convertSHTMI1_BOKZM(shtmi1, saveTime);
							vGeneralSHTMI1.push_back(gSHTMI1);
						}
					}
				}
			}

//чтение массива ШТМИ2
			if (line.find("ШТМИ2") != std::string::npos) {
				getline(finp, line, '\n' );
				if (line.find("ТМОС") != std::string::npos) {
					char cdate[20], ctime[20];
					sscanf(line.c_str(), "ТМОС %s %s", &cdate, &ctime);
					string sdate = cdate, stime = ctime;

					int errorSHTMI2 = ReadTMIArray(finp, "CISO_SHTMI2", ArraySHTMI2, SHTMI1Size);
					if (!errorSHTMI2) {
//						char _date[10], _time[10];
//						sscanf(line.c_str(), "ТМОС %s %s", &_date, &_time);
						fout << "\n" << line << "\n";

						struct SHTMI2_BOKZM shtmi2;
						memcpy(&shtmi2, &ArraySHTMI2[2], sizeof(shtmi2));
						PrintSHTMI2_BOKZM(fout, shtmi2);

						if (!isSHTMI2) {
							isSHTMI2 = true;
							fshtmi2.open((SaveDir + FileTitle + "_shtmi2.txt").c_str());
							fshtmi2 << "Date&Time\t" << "Tbokz\t";
							fshtmi2 << "KC1\t" << "KC2\t" << "POST\t" << "№\t" << "Texp\t";
							fshtmi2 << "Foc\t" << "Xg\t" << "Yg\t";
							fshtmi2 << "Mean\t" << "Sigma\t";
							fshtmi2 << "Ndef\t" << "НО\t" << "TО\t";
							for (int i = 0; i < 12; i++) {
								fshtmi2 << "EC" << (i+1) << "\t";
							}
							fshtmi2 << "\n";
						}

						fshtmi2 << sdate << " " << stime << "\t";
						fshtmi2 << shtmi2.timeBOKZ << "\t";
						fshtmi2 << uppercase << hex << setfill('0');
						fshtmi2 << "0x" << setw(4) << shtmi2.status1 << "\t";
						fshtmi2 << "0x" << setw(4) << shtmi2.status2 << "\t";
						fshtmi2 << "0x" << setw(4) << shtmi2.post << "\t";
						fshtmi2 << dec << setfill(' ');
						fshtmi2 << shtmi2.serialNumber << "\t";
						fshtmi2 << shtmi2.timeExp << "\t" << shtmi2.Foc << "\t";
						fshtmi2 << shtmi2.Xg << "\t" << shtmi2.Yg << "\t";
						fshtmi2 << shtmi2.Mean << "\t" << shtmi2.Sigma << "\t";
						fshtmi2 << shtmi2.countDefect << "\t";
						fshtmi2 << shtmi2.cntCallNO << "\t";
						fshtmi2 << shtmi2.cntCallTO << "\t";
						for (int i = 0; i < 12; i++) {
							fshtmi2 << shtmi2.cntStatOrient[i] << "\t";
						}
						fshtmi2 <<"\n";

						if (isLoadDb) {
							TDateTime saveTime = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
							GeneralizedSHTMI2 gSHTMI2 = clientDb->convertSHTMI2_BOKZM(shtmi2, saveTime);
							vGeneralSHTMI2.push_back(gSHTMI2);
						}
					}
				}
			}

//чтение служебных массивов
			if (line.find("CIUV_VBC") != std::string::npos) {
				int cntRecWatch = 0;
				getline(finp, line, '\n' );
				if (line.find("ТМОС") != std::string::npos) {
					char cdate[20], ctime[20];
					sscanf(line.c_str(), "ТМОС %s %s", &cdate, &ctime);
					string sdate = cdate, stime = ctime;
					getline(finp, line, '\n' );
					
					int nWord = 0, buf; 
					unsigned short word;
					do {
						getline(finp, line, '\n');
						if (line.find("CIUV_VBC") != std::string::npos) {
							if ( (sscanf(line.c_str(), "CIUV_VBC[%ld] %d", &buf, &word) == 2) 
								&& (nWord >= 0) && (nWord < sizeTest) ) {
								ArrayTest[nWord] = word;
								nWord++;
								if (nWord == 127) {
									string lineBuf;
									getline(finp, lineBuf, '\n');getline(finp, lineBuf, '\n');
									getline(finp, lineBuf, '\n');getline(finp, lineBuf, '\n');
									getline(finp, lineBuf, '\n');
								}
							}
						}
					} while (line.find("CIUV_VBC") != std::string::npos);

					//расшифровка
					if (nWord > 127) {

						MSI_BOKZM msi[3];
						TDateTime curDate = timeTestToDateTime(*(unsigned long *)ArrayTest);
						fnew << OutputDateTime(curDate).c_str() << "\t";

						unsigned int lastTime = 0;
						short sumWord = 2;
						short nAr = 0;
						while (sumWord < nWord)
						{
							unsigned short curCodeKC = ArrayTest[sumWord];
							unsigned short statExchange = ArrayTest[sumWord+1];
							unsigned short curCodeOC = ArrayTest[sumWord+2];
							sumWord += 3;
							int devIndex = FindValueInList(curCodeKC, cwWatch, 3);
							if ( (statExchange == 0x8010) || (statExchange == 0xA010) ) {
								fnew <<  "OK\t";
							} else fnew <<  "ERROR\t";
							if ((devIndex >= 0) && (devIndex < 3) && (curCodeOC == codeOC[devIndex]) ) {
								if ( (statExchange == 0x8010) || (statExchange == 0xA010) ) {

									memcpy(&msi[devIndex], &ArrayTest[sumWord], sizeof(msi));
									lastTime = msi[devIndex].timeBOKZ;
									fnew << devIndex << "\t";
									fnew << msi[devIndex].timeBOKZ << "\t";
									fnew << msi[devIndex].nLocalObj << "\t" << (short)msi[devIndex].nDeterObj << "\t";
									//печать массива
									fwatch[devIndex] << OutputDateTime(curDate).c_str() << "\t";
									fwatch[devIndex] << msi[devIndex].timeBOKZ << "\t" << msi[devIndex].timeExp << "\t";
									fwatch[devIndex] << msi[devIndex].SKOCadrAfterFilter << "\t" << msi[devIndex].thresholdLoc << "\t";
									fwatch[devIndex] << msi[devIndex].nLocalObj << "\t" << (short)msi[devIndex].nDeterObj << "\t";
									fwatch[devIndex] << (short)msi[devIndex].nFrag << "\t" << msi[devIndex].omega * RTS << "\t";
									fwatch[devIndex] << msi[devIndex].timeBetwCadrs << "\t" << msi[devIndex].numSec << "\n";
									sumWord += sizeWatch;
								}
							}
							else {
								int devIndex = FindValueInList(curCodeKC, cwCoord, 3);
								if ( (devIndex >= 0) && (devIndex < 3) && (curCodeOC == codeOC[devIndex]) ) {
									if ( (statExchange == 0x8010) || (statExchange == 0xA010) ) {
										fnew << devIndex << "\t";
										memcpy(&ArrayCoord[nAr*sizeCoord], &ArrayTest[sumWord], sizeCoord * sizeof(short));
										sumWord += sizeCoord;
										nAr++;
										if (nAr == 6) {
											TLOCBOKZM coord;
											memcpy(&coord, ArrayCoord, sizeof(coord));
//											fcoord[devIndex] << OutputDateTime(curDate).c_str() << "\n";
//											fcoord[devIndex] << lastTime << "\n";
//											fcoord[devIndex] << "X\tY\t" << "Bright\t" << "Size\n";
//											for (int nObj = 0; nObj < 24; nObj++) {//MAX_OBJ_MLOC; nObj++)
//												fcoord[devIndex] << (nObj+1) << "\t";
//												for (int i = 0; i < 4; i++) {
//													fcoord[devIndex] << coord.LocalList[nObj][i] << "\t";
//												}
//												fcoord[devIndex] << "\n";
//											}
//											fcoord[devIndex] << "\n";
											_DateTime stDate;
											DecodeDateTime(curDate, stDate.Year, stDate.Month,  stDate.Day,
																stDate.Hour, stDate.Min, stDate.Sec, stDate.mSec);
											double era = DateTimeToDaysJ2000(&stDate)/36525.;

											int nLocOut = msi[devIndex].nLocalObj;
											if (nLocOut > 24 ) nLocOut = 24;

//											fcoord[devIndex] << OutputDateTime(curDate).c_str() << "\n";
											fcoord[devIndex] << "Tpr = " << msi[devIndex].timeBOKZ;
											fcoord[devIndex] << ", EPOCH = " << era;
											fcoord[devIndex] << ", NumLoc = " << nLocOut;
											fcoord[devIndex] << ", Vx = 0, Vy = 0, Vz = 0";
											UnicodeString strFormat = "yyyy-MM-dd hh:mm:ss.zzz";
											fcoord[devIndex] << ", Dt = " << AnsiString(curDate.FormatString(strFormat)).c_str();
											fcoord[devIndex] << " \n";

											for (int nObj = 0; nObj < nLocOut; nObj++) {//MAX_OBJ_MLOC; nObj++)
												fcoord[devIndex] << (nObj+1) << "\t";
												for (int i = 0; i < 4; i++) {
													fcoord[devIndex] << coord.LocalList[nObj][i] << "\t";
												}
												fcoord[devIndex] << "\n";
											}
//											fcoord[devIndex] << "\n";
										}
									}
								}
							}
						}
						fnew << "\n";
					}

//					getline(finp, line, '\n' );
//					int n1, n2, n3, flRead = 1;
//					for (int i = 0; i < 259; i++) {
////						finp>>word1>>word2>>dec_val;
//						finp >> word1 >> word2 >> word3;
//						n1 = sscanf(word1.c_str(),"[%d]", &ind);
//						n2 = sscanf(word2.c_str(),"0X%lx", &hex_val);
//						n3 = sscanf(word3.c_str(),"%ld", &dec_val);
//						if (n1 && n2 && n3 && (i == ind)&&(hex_val == dec_val)) {
//							ArrayTest[i] = hex_val;			
//						}
//						else {
//							flRead = 0;
//							break;
//						}
//						if (i == 126) {
//							for (int j = 0; i < 5; i++) {
//								getline(finp, line, '\n' );    
//							}							
//						}
//					}
//
//					if (flRead) {
//						float timeBOKZ;
//						memcpy(&timeBOKZ,ArrayWatch, sizeof(timeBOKZ));
//					}
				}
			}

////чтение служебных массивов (начиная с 4-го изделия)
//			if (line.find("Дополнительная ТМИ БОКЗ") != std::string::npos) {
//
//				getline(finp, line, '\n' );
//				if (line.find("ТМОС") != std::string::npos) {
//					char cdate[20], ctime[20];
//					sscanf(line.c_str(), "ТМОС %s %s", &cdate, &ctime);
//					string sdate = cdate, stime = ctime;
//					getline(finp, line, '\n' );
//
//					int n1, n2, n3;
//					for (int i = 0; i < 722; i++) {
//						finp >> word1 >> word2 >> word3;
//						n1 = sscanf(word1.c_str(),"[%d]", &ind);
//						n2 = sscanf(word2.c_str(),"0X%lx", &hex_val);
//						n3 = sscanf(word3.c_str(),"%ld", &dec_val);
//						if (n1 && n2 && n3 && (i == ind)&&(hex_val == dec_val)) {
//							ArrayTMI[i] = hex_val;
//						}
//						else break;
//					}
//
//					short sumWord = 2; //чтение времени
//					short nAr = 0;
//					unsigned short curCodeKC, statExcahge, curCodeOC;
//					MSHIA_BOKZM mshiA[3];
//					DTMIA_BOKZM dtmiA[3];
//
//					while (sumWord < nWord)
//					{
//						curCodeKC = ArrayTMI[sumWord];
//						sumWord++;
//						int devIndex = FindValueInList(curCodeKC, cwMshiA, 3);
//						if ( (devIndex >= 0) && (devIndex < 3) && (curCodeOC == codeOC[devIndex]) ) {
//							fnew << devIndex << "\t";
//							memcpy(&mshiA[devIndex], &ArrayTMI[sumWord], sizeof(MSHIA_BOKZM));
//							//print mshiA
//						}
//						int devIndex = FindValueInList(curCodeKC, cwDtmiA, 3);
//						if ( (devIndex >= 0) && (devIndex < 3) {
//							statExcahge = ArrayTMI[sumWord];
//							curCodeOC = ArrayTMI[sumWord+1];
//							sumWord += 2;
//							if ( (curCodeOC == codeOC[devIndex]) &&
//								 ((statExcahge == 0x8010) || (statExcahge == 0xA010)) ) {
//								fnew << devIndex << "\t";
//
//								memcpy(&dtmiA[devIndex], &ArrayTMI[sumWord], sizeof());
//								sumWord += sizeDtmiA;
//								nAr++;
//								if (nAr == 9) {
//									TDTMIA_BOKZM dtmiA;
//									memcpy(&coord, ArrayCoord, sizeof(coord));
//								}
//							}
//						}
//					}
//				}
//			}

//чтение массива ДТМИ
			if (line.find("ДТМИ") != std::string::npos){
				string word1, word2, word3;
				unsigned int hex_val, dec_val;
				int cntRecDTMI = 0, ind;
				getline(finp, line, '\n' );
				if (line.find("ТМОС") != std::string::npos) {
					char cdate[20], ctime[20];
					sscanf(line.c_str(), "ТМОС %s %s", &cdate, &ctime);
					string sdate = cdate, stime = ctime;

					int n1, n2, n3 ;
					for (int i = 0; i < 290; i++) {
//						finp>>word1>>word2>>dec_val;
						finp>>word1>>word2>>word3;
						n1 = sscanf(word1.c_str(),"[%d]", &ind);
						n2 = sscanf(word2.c_str(),"0X%lx", &hex_val);
						n3 = sscanf(word3.c_str(),"%ld", &dec_val);
						if (n1 && n2 && n3 && (i == ind)&&(hex_val == dec_val)) {
							ArrayDTMI[i] = hex_val;
						}
						else break;
					}

					struct DTMI_BOKZM dtmi;
					memcpy(&dtmi.timeBOKZ, &ArrayDTMI[2], 30*sizeof(short));
					memcpy(&dtmi.LocalList[2][0], &ArrayDTMI[36], 28*sizeof(short));
					memcpy(&dtmi.LocalList[5][2], &ArrayDTMI[68], 28*sizeof(short));
					memcpy(&dtmi.LocalList[9][0], &ArrayDTMI[100],28*sizeof(short));
					memcpy(&dtmi.LocalList[12][2], &ArrayDTMI[132],28*sizeof(short));
					memcpy(&dtmi.LocalList[16][0], &ArrayDTMI[164],28*sizeof(short));
					memcpy(&dtmi.LocalList[19][2], &ArrayDTMI[196],28*sizeof(short));
					memcpy(&dtmi.LocalList[23][0], &ArrayDTMI[228],28*sizeof(short));
					memcpy(&dtmi.LocalList[26][2], &ArrayDTMI[260],28*sizeof(short));

					fout << "\n" << line << "\n";
					PrintDTMI_BOKZM(fout, dtmi);

					AnsiString stringSerial;
					stringSerial.sprintf("Зав. № %02d", dtmi.serialNumber);
					AnsiString localDir = SaveDir + "\\" + stringSerial;
					CreateDir(localDir);

					TFormatSettings curFormat;
					curFormat.ShortDateFormat = "dd.mm.yyyy";
					TDateTime newDateTime = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
					localDir = localDir + "\\" + AnsiString(DateToStr(newDateTime)).c_str();
					CreateDir(localDir);

					curFormat.ShortDateFormat = "yyyy_mm_dd";
					curFormat.LongTimeFormat  = "hh_nn_ss_zzz";
					AnsiString localName, localDate, localTime;
					localName.sprintf("%s_%s(%.2f)_LOC.txt",
								AnsiString(DateToStr(newDateTime, curFormat)).c_str(),
								AnsiString(TimeToStr(newDateTime, curFormat)).c_str(), dtmi.timeBOKZ);

					CheckFileName(localName);
					localName = localDir + "\\" + localName;

					PrintLocalDTMI_BOKZM(localName, dtmi);

					if (!isDTMI) {
						isDTMI = true;
						fdtmi.open((SaveDir + FileTitle + "_dtmi.txt").c_str());
						fdtmi << "Date&Time\t" << "Tbokz\t";
						fdtmi << "KC1\t" << "KC2\t" <<  "№\t" << "Texp\t";
						fdtmi << "Foc\t" << "Xg\t"  << "Yg\t";
						fdtmi << "NumLoc\t" << "NumFix\t";
						fdtmi << "\n";
					}

					fdtmi << sdate << " " << stime << "\t";
					fdtmi << dtmi.timeBOKZ << "\t";
					fdtmi << uppercase << hex << setfill('0');
					fdtmi << "0x" << setw(4) << dtmi.status1 << "\t";
					fdtmi << "0x" << setw(4) << dtmi.status2 << "\t";
					fdtmi << dec << setfill(' ');
					fdtmi << dtmi.serialNumber << "\t";
					fdtmi << dtmi.timeExp << "\t" << dtmi.Foc << "\t";
					fdtmi << dtmi.Xg << "\t" << dtmi.Yg << "\t";
					fdtmi << dtmi.nLocalObj << "\t" << dtmi.nDeterObj << "\n";

					if ( (dtmi.serialNumber > 0) && (dtmi.serialNumber < 200) ) {
						if (isLoadDb) {
							TDateTime saveTime = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
							GeneralizedDTMI gDTMI = clientDb->convertDTMI_BOKZM(dtmi, saveTime);
							vGeneralDTMI.push_back(gDTMI);
						}

						struct CadrInfo mCadr;
						mCadr.Time = newDateTime;
						ConvertDataDTMI_BOKZM(dtmi, mCadr);
						vCadrInfo.push_back(mCadr);
						if (mCadr.Time > 0) {
							plotter->AddPoint(ChartNumLoc, 0, mCadr.Time, mCadr.CountLocalObj);
						}
					}
				}
			}

//чтение температуры
			if (line.find("температур") != std::string::npos) {
				getline(finp, line, '\n' );
				if (line.find("ТМОС") != std::string::npos) {
					char cdate[20], ctime[20];
					sscanf(line.c_str(), "ТМОС %s %s", &cdate, &ctime);
					string sdate = cdate, stime = ctime;

					float temp, arrayTemp[3] = {0, 0, 0};
					bool readTemp[3] = {0, 0, 0};
					int ind, countTemp = 0;
					for (int i = 0; i < 3; i++) {
						getline(finp, line, '\n' );
						int n = sscanf(line.c_str(), "CSUH_VHT[%d] %f", &ind, &temp);
						if ((ind >= 6) && (ind <= 8) && (n == 2)) {
							arrayTemp[ind - 6] = temp;
							readTemp [ind - 6] = true;
							countTemp++;
						}
						else break;
					}
					if (countTemp) {
						if (!isTemp) {
							isTemp = true;
							ftemp.open((SaveDir + FileTitle + "_temp.txt").c_str());
							ftemp << "Date&Time\t" <<"ТЕМП-1\t" << "ТЕМП-2\t" << "ТЕМП-3\n";
						}

						ftemp << sdate << " " << stime << "\t";
						for (int i = 0; i < 3; i++) {
							if (readTemp[i]) ftemp << arrayTemp[i] << "\t";
							else ftemp << "-\t";
						}
						ftemp << "\n";
					}
				}
			}

//чтение признака наличия питания
			if (line.find("дискрет") != std::string::npos) {
				getline(finp, line, '\n' );
				if (line.find("ТМОС") != std::string::npos) {
					char cdate[20], ctime[20];
					sscanf(line.c_str(), "ТМОС %s %s", &cdate, &ctime);
					string sdate = cdate, stime = ctime;

					int power, ArrayPower[3] = {0, 0, 0};
					int ind, countPower = 0;
					int keyPower[3] = {98, 106, 114};
					for (int i = 0; i < 3; i++) {
						getline(finp, line, '\n' );
						int n = sscanf(line.c_str(),"CSAH.D.VHD%d %d", &ind, &power);
						if (( ind == keyPower[i]) && (n == 2)) {
							ArrayPower[i] = abs(1 - power);
							countPower++;
						}
						else break;
					}
					if (countPower) {
						if (!isPower) {
							isPower = true;
							fpow.open((SaveDir + FileTitle  + "_power.txt").c_str());
							fpow << "Date&Time\t" <<"КП-1\t" << "КП-2\t" << "КП-3\n";
						}

						fpow << sdate << " " << stime << "\t";
						for (int i = 0; i < 3; i++) {
							fpow << ArrayPower[i] << "\t";
						}
						fpow << "\n";
					}
				}
			}
		}
		finp.close();
		fout.close();
		fnew.close();
		if (isMSHI)   fmshi.close();
		if (isSHTMI1) fshtmi1.close();
		if (isSHTMI2) fshtmi2.close();
		if (isDTMI)   fdtmi.close();
		if (isTemp)   ftemp.close();
		if (isPower)  fpow.close();

		for (int i = 0; i<3; i++) {
			fwatch[i].close();
            fcoord[i].close();
		}

		PrepareStartDraw();
		CheckTabSheet();

		if (vGeneralSHTMI1.size()) {
			clientDb->insertSHTMI1(vGeneralSHTMI1, FormAnimateSetting->KAComboBox->Text);
		}
		if (vGeneralSHTMI2.size()) {
			clientDb->insertSHTMI2(vGeneralSHTMI2, FormAnimateSetting->KAComboBox->Text);
		}
		if (vGeneralDTMI.size()) {
			clientDb->insertDTMI(vGeneralDTMI, FormAnimateSetting->KAComboBox->Text);
		}
		if (vGeneralMSHI.size()) {
			clientDb->insertMSHIOR(vGeneralMSHI, FormAnimateSetting->KAComboBox->Text);
		}
	}
}
 //---------------------------------------------------------------------------
void __fastcall TFormGraphOrient::MenuOpenEnergyTestClick(TObject *Sender)
{

	const int sizeOrient  = 22;
	const int sizeOrientA  = 32;
	const int sizeWatch  = 12;
	const int sizeMIL  = 32;
	const int cntArCoord = 8;
	const int cntArDtmiA = 9;
	unsigned short ArrayOrient[sizeOrient];
	unsigned short ArrayOrientA[sizeOrientA];
	unsigned short ArrayWatch[sizeWatch];
	unsigned short ArrayMIL[sizeMIL];
	unsigned short ArrayCoord[sizeMIL * cntArCoord];
	unsigned short ArrayDtmiA[sizeMIL * cntArDtmiA];

	string cwOrient[3]  = {"4616", "6616", "6e16"};
	string cwOrientA[3] = {"4600", "6600", "6e00"};
	string cwDtmiA[3]  = {"44a0", "64a0", "6ca0"};
	string cwWatch[3]  = {"45ec", "65ec", "6dec"};
	string cwCoord[3]  = {"4640", "6640", "6e40"};
	string codeOC[3]   = {"4000", "6000", "6800"};

	int isCoord = 0, isWatch = 0;
	OpenDialog->Options.Clear();
	OpenDialog->Filter = "Текстовые файлы|*.mtt; *.mtt|Все файлы|*.*";
	OpenDialog->Options << ofAllowMultiSelect;

	if (OpenDialog->Execute()) {
		AnsiString FileName = OpenDialog->FileName;
		AnsiString CurDir   = ExtractFileDir(FileName);
		AnsiString SaveDir  = CurDir + "\\Расшифровка\\";
		CreateDir(SaveDir);
		GetFileTitles(FileName,&FileTitle);

		if (OpenDialog->Files->Count > 1) {
			FileTitle += "("+IntToStr(OpenDialog->Files->Count) + ")";
		}

		int devIndex, nAr[3];
		ofstream forient[3], fwatch[3], fcoord[3];
		AnsiString fname;
		for (devIndex = 0; devIndex < 3; devIndex++) {
			fname = SaveDir + FileTitle + "_watch_" + IntToStr(devIndex+1) + ".txt";
			fwatch[devIndex].open(fname.c_str());
			fwatch[devIndex] << "Date&Time\t" << "timeBOKZ\t";
			fwatch[devIndex] << "Mean\t" << "Sigma\t" << "Th\t";
			fwatch[devIndex] << "NumLoc\t" << "NumDet\t";
			fwatch[devIndex] << "NumFrag\t" << "Omega, ''/s\t";
			fwatch[devIndex] << "deltaT, s\t" << "NumSec\n";

			fname = SaveDir + FileTitle + "_orient_" + IntToStr(devIndex+1) + ".txt";
			forient[devIndex].open(fname.c_str());
			forient[devIndex] << "Date&Time\t" << "timeBOKZ\t" ;
			forient[devIndex] << "Status1\t" << "Status2\t" << "Mornt\n";

			fname = SaveDir+FileTitle+"_loc_"+IntToStr(devIndex+1)+".txt";
			fcoord[devIndex].open(fname.c_str());
			nAr[devIndex] = 0;
		}

		string line, word;
		TDateTime curDate;
		for (int iFile = 0; iFile < OpenDialog->Files->Count; iFile++) {
			ifstream finp(AnsiString(OpenDialog->Files->Strings[iFile]).c_str());
			if (!finp.is_open()) {
				ShowMessage("Файл не может быть открыт!");
				return;
			}

			finp >> hex;
			while (!finp.eof())
			{
				getline(finp, line);
				if (line.find("T=") != std::string::npos) {
					int year, month, day, hour, min, sec, msec;
					sscanf(line.c_str(), "T=%ld-%ld-%ld %ld:%ld:%ld.%ld", &day, &month, &year,
														 &hour, &min, &sec, &msec);
					curDate = EncodeDateTime(year, month, day, hour, min, sec, msec);
				}
	//чтение массива МШИОР
				devIndex = FindWordInList(line, cwOrient, 3);
				if ( (devIndex >= 0) && (devIndex < 3) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
						for (int i = 0; i < sizeOrient; i++) {
							finp >> ArrayOrient[i];
						}
						MSHI_BOKZM orient;
						memcpy(&orient, ArrayOrient, sizeof(orient));
						//расшифровка массива
						forient[devIndex] << OutputDateTime(curDate).c_str() << "\t";
						forient[devIndex] << orient.timeBOKZ << "\t";
						forient[devIndex] << uppercase << hex << setfill('0');
						forient[devIndex] << "0x" << setw(4) << orient.status1 << "\t";
						forient[devIndex] << "0x" << setw(4) << orient.status2 << "\t";
						forient[devIndex] << dec <<setfill(' ');

						for (int i = 0; i < 3; i++) {
							for (int j = 0; j < 3; j++) {
								forient[devIndex] << orient.Mornt[i][j] << "\t";
							}
						}
						forient[devIndex] << "\n";
					}
				}
//чтение массива МШИОР-А
				devIndex = FindWordInList(line, cwOrientA, 3);
				if ( (devIndex >= 0) && (devIndex < 3) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
						for (int i = 0; i < sizeOrientA; i++) {
							finp >> ArrayOrientA[i];
						}
						MSHIA_BOKZM orient;
						memcpy(&orient, ArrayOrientA, sizeof(orient));
						//расшифровка массива
						forient[devIndex] << OutputDateTime(curDate).c_str() << "\t";
						forient[devIndex] << orient.timeBOKZ << "\t";
						forient[devIndex] << uppercase << hex << setfill('0');
						forient[devIndex] << "0x" << setw(4) << orient.status1 << "\t";
						forient[devIndex] << "0x" << setw(4) << orient.status2 << "\t";
						forient[devIndex] << dec <<setfill(' ');

						for (int i = 0; i < 3; i++) {
							for (int j = 0; j < 3; j++) {
								forient[devIndex] << orient.Mornt[i][j] << "\t";
							}
						}

						forient[devIndex] << orient.serialNumber << "\t";
						forient[devIndex] << orient.numSec << "\t" << orient.nFrag << "\t";
						forient[devIndex] << orient.nLocalObj << "\t" << orient.nDeterObj << "\t";
						forient[devIndex] << orient.thMax << "\t" << orient.mxy/10000. << "\t";
						forient[devIndex] << orient.timeExp << "\t" << orient.deltaT << "\n";
					}
				}
	//чтение массива служебной информации
				devIndex = FindWordInList(line, cwWatch, 3);
				if ( (devIndex >= 0) && (devIndex < 3) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
						for (int i = 0; i < sizeWatch; i++) {
							finp >> ArrayWatch[i];
						}
						MSI_BOKZM msi;
						memcpy(&msi, ArrayWatch, sizeof(msi));
						//расшифровка массива
						fwatch[devIndex] << OutputDateTime(curDate).c_str() << "\t";
						fwatch[devIndex] << msi.timeBOKZ << "\t" << msi.timeExp << "\t";
						fwatch[devIndex] << msi.SKOCadrAfterFilter << "\t" << msi.thresholdLoc << "\t";
						fwatch[devIndex] << msi.nLocalObj << "\t" << (short)msi.nDeterObj << "\t";
						fwatch[devIndex] << (short)msi.nFrag << "\t" << msi.omega * RTS << "\t";
						fwatch[devIndex] << msi.timeBetwCadrs << "\t" << msi.numSec << "\n";
					}
				}
	//чтение массива локализованных объектов
				devIndex = FindWordInList(line, cwCoord, 3);
				if ( (devIndex >= 0) && (devIndex < 3) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
						for (int i = 0; i < sizeMIL; i++) {
							finp >> ArrayCoord[i + nAr[devIndex] * sizeMIL];
						}
						nAr[devIndex]++;
						if (nAr[devIndex] == cntArCoord) {
							TLOCBOKZM coord;
							memcpy(&coord, ArrayCoord, sizeof(coord));
							nAr[devIndex] = 0;
							//расшифровка массива
							fcoord[devIndex] << OutputDateTime(curDate).c_str() << "\n";
							fcoord[devIndex] << "X\tY\t" << "Bright\t" << "Size\n";
							for (int nObj = 0; nObj < 24; nObj++) {//MAX_OBJ_MLOC; nObj++) {
								fcoord[devIndex] << (nObj+1) << "\t";
								for (int i = 0; i < 4; i++) {
									fcoord[devIndex] << coord.LocalList[nObj][i] << "\t";
								}
								fcoord[devIndex] << "\n";
							}
							fcoord[devIndex] << "\n";
						}
					}
				}
	//чтение массива ДТМИ-А
				devIndex = FindWordInList(line, cwDtmiA, 3);
				if ( (devIndex >= 0) && (devIndex < 3) ) {
					getline(finp, line, '\n' );
					if (line.find(codeOC[devIndex]) != std::string::npos) {
                        unsigned short CC1, CC2;
						finp >> CC1 >> CC2;
						if (CC2 == 0x42C3) {
							unsigned short nArDtmi = (CC1 - 0x3AAA) >> 12;
							if ( (nArDtmi >= 0) && (nArDtmi < cntArDtmiA) ){
								for (int i = 0; i < 30; i++) {
									finp >> ArrayMIL[i];
								}
								if ( !nArDtmi && (ArrayMIL[2]== 0xE004) ) {
									int aaa = 5;
								}
								memcpy(&ArrayDtmiA[nArDtmi * 30], ArrayMIL, sizeof(short) * 30);
								if (nArDtmi == cntArDtmiA - 1) {
									DTMIA_BOKZM dtmi;
									memcpy(&dtmi, ArrayDtmiA, sizeof(ArrayDtmiA));
									//расшифровка массива
									fcoord[devIndex] << OutputDateTime(curDate).c_str() << "\n";
									fcoord[devIndex] << "X\tY\t" << "Bright\t" << "Size\n";
									for (int nObj = 0; nObj < 15; nObj++) {//MAX_OBJ_MLOC; nObj++) {
										fcoord[devIndex] << (nObj+1) << "\t";
										fcoord[devIndex] << dtmi.LocalList[nObj].x << "\t";
										fcoord[devIndex] << dtmi.LocalList[nObj].y << "\t";
										fcoord[devIndex] << dtmi.LocalList[nObj].bright << "\t";
										fcoord[devIndex] << dtmi.LocalList[nObj].size << "\n";
									}
									fcoord[devIndex] << "\n";
								}
							}
						}
					}
				}
			}
		}

		for (devIndex = 0; devIndex < 3; devIndex++) {
			fwatch[devIndex].close();
			fcoord[devIndex].close();
			forient[devIndex].close();
		}
    }
}
//---------------------------------------------------------------------------

 void TFormGraphOrient::OutputDTMI(ofstream &_fout, AnsiString &_SaveDir, DTMI &_tmi, TDateTime zeroDate, int isM1000)
 {
	LOC    mLOC;
	TDateTime curDate;
	struct CadrInfo mCadr;
	double dtBokz, Tpr;
	if (isM1000) dtBokz = 0.25;
	else dtBokz = 1.0;
					if (_tmi.status2 == 0x0005) {
						SwapShort((short*)&_tmi.nWindows, (short*)&_tmi.epsillon);
						for (int i = 0; i < MAX_WINDOW; i = i + 2) {
							SwapShort((short*)&_tmi.levelWindow[i],
									  (short*)&_tmi.levelWindow[i+1]);
							SwapShort((short*)&_tmi.nObjectWindow[i],
									  (short*)&_tmi.nObjectWindow[i+1]);
						}
						SwapShort((short*)&_tmi.nLocal[0], (short*)&_tmi.nLocal[1]);
						SwapShort((short*)&_tmi.maxHist,   (short*)&_tmi.maxHistX);
						SwapShort((short*)&_tmi.maxHistY,  (short*)&_tmi.test_short);
						for (int i = 0; i < 10; i = i + 2) {
							SwapShort((short*)&_tmi.Reserved[i], (short*)&_tmi.Reserved[i+1]);
						}
						memcpy(&mLOC, &_tmi, sizeof(_tmi));

						curDate = GetDateTime(zeroDate, mLOC.timeBOKZ);
						PrintLOC(_fout, curDate, mLOC, isM1000);
						PrintLocalMLOC(_SaveDir, curDate, mLOC, isM1000);

						if (!isMLOC) {
							isMLOC = true;
							fmloc.open((_SaveDir + FileTitle + "_mloc.txt").c_str());
							fmloc << "Date&Time\t";
							fmloc << "Day/Time\t";
							fmloc << "KC1\t" << "KC2\t" << "№\t" << "Texp\t";
							fmloc << "NumLoc\t" << "NumFixed\t";
							fmloc << "Mean\t" << "Sigma\t";
							fmloc << "\n";
						}
						fmloc << OutputDateTime(curDate).c_str() << "\t";
						fmloc << DayTimeToString(mLOC.timeBOKZ).c_str() << "\t";
						fmloc << uppercase << hex << setfill('0');
						fmloc << "0x" << setw(4) << mLOC.status1 << "\t";
						fmloc << "0x" << setw(4) << mLOC.status2 << "\t";
						fmloc << dec << setfill(' ');
						fmloc << setw(6) << mLOC.serialNumber << "\t";
						fmloc << setw(6) << mLOC.timeExp << "\t";
						fmloc << setw(6) << mLOC.nLocalObj << "\t";
						fmloc << setw(6) << mLOC.nFixedObj << "\t";
						fmloc << setw(6) << mLOC.MeanC << "\t";
						fmloc << setw(6) << mLOC.SigmaC << "\t";
						fmloc << "\n";
						fmloc << flush;

//						if (isLoadDb) {
//							TDateTime saveTime;// = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
//							GeneralizedDTMI gDTMI = clientDb->convertDTMI(mDTMI, saveTime);
//							vGeneralDTMI.push_back(gDTMI);
//						}

						ConvertDataLOC(mLOC, mCadr, isM1000);
						plotter->AddPoint(ChartNumLoc, 0, mLOC.timeBOKZ * dtBokz, mLOC.nLocalObj);
						plotter->AddPoint(ChartFone,   0, mLOC.timeBOKZ * dtBokz, mLOC.MeanC);
						plotter->AddPoint(ChartNoise,  0, mLOC.timeBOKZ * dtBokz, mLOC.SigmaC);
					}
					else {
						curDate = GetDateTime(zeroDate, _tmi.timeBOKZ);
						PrintDTMI(_fout, curDate, _tmi, isM1000);
						PrintLocalDTMI(_SaveDir, curDate, _tmi, isM1000);

						if (!isDTMI) {
							isDTMI = true;
							fdtmi.open((_SaveDir + FileTitle + "_dtmi.txt").c_str());
							fdtmi << "Date&Time\t";
							fdtmi << "Day/Time\t";
							fdtmi << "TimeLastQ\t";
							fdtmi << "KC1\t" << "KC2\t" << "№\t" << "Texp\t";
							fdtmi << "NumLoc\t"  << "NumDet\t" << "NumFrag\t";
							fdtmi << "NumL[0]\t" << "NumL[1]\t" << "MaxHist\t";
							fdtmi << "Eps\t" << "DeltaT\t";
							fdtmi << "\n";
						}
						fdtmi << OutputDateTime(curDate).c_str() << "\t";
						fdtmi << DayTimeToString(_tmi.timeBOKZ).c_str() << "\t";
						fdtmi << setw(6) << DayTimeToString(_tmi.timeQuatLast) << "\t";
						fdtmi << uppercase << hex << setfill('0');
						fdtmi << "0x" << setw(4) << _tmi.status1 << "\t";
						fdtmi << "0x" << setw(4) << _tmi.status2 << "\t";
						fdtmi << dec << setfill(' ');
						fdtmi << setw(6) << _tmi.serialNumber << "\t";
						fdtmi << setw(6) << _tmi.timeExp << "\t";
						fdtmi << setw(6) << _tmi.nLocalObj << "\t";
						fdtmi << setw(6) << _tmi.nDeterObj << "\t";
						fdtmi << setw(6) << _tmi.nWindows << "\t";
						fdtmi << setw(6) << _tmi.nLocal[0] << "\t";
						fdtmi << setw(6) << _tmi.nLocal[1] << "\t";
						fdtmi << setw(6) << _tmi.maxHist << "\t";
						fdtmi << setw(6) << _tmi.epsillon << "\t";
						fdtmi << setw(6) << _tmi.dTimeBOKZ << "\t";
						fdtmi << "\n";
						fdtmi << flush;

//						if (isLoadDb) {
//							TDateTime saveTime;// = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
//							GeneralizedDTMI gDTMI = clientDb->convertDTMI(_tmi, saveTime);
//							vGeneralDTMI.push_back(gDTMI);
//						}
						ConvertDataDTMI(_tmi, mCadr, isM1000);
						plotter->AddPoint(ChartNumLoc, 0, _tmi.timeBOKZ * dtBokz, _tmi.nLocalObj);
						plotter->AddPoint(ChartNumDet, 0, _tmi.timeBOKZ * dtBokz, _tmi.nDeterObj);
						plotter->AddPoint(ChartNumFrag, 0, _tmi.timeBOKZ* dtBokz, _tmi.nWindows);

						if (mCadr.SizeWindowsList) {
							plotter->AddPoint(ChartLevel, 0, _tmi.timeBOKZ * dtBokz, mCadr.Level);
						}

					}
					mCadr.Time = curDate.Val;
					AddCadrInfo(mCadr, vCadrInfo);
					AddCadrInfo(mCadr, vDtmiInfo);
					isOpenDtmi = false;
 }

 void TFormGraphOrient::OutputLOC(ofstream &_fout, AnsiString &_SaveDir, LOC &_tmi, TDateTime zeroDate, int isM1000)
 {
	double dtBokz, Tpr;
	if (isM1000) dtBokz = 0.25;
	else dtBokz = 1.0;
						TDateTime curDate;
						curDate = GetDateTime(zeroDate, _tmi.timeBOKZ);
						PrintLOC(_fout, curDate, _tmi, isM1000);
						PrintLocalMLOC(_SaveDir, curDate, _tmi, isM1000);

						if (!isMLOC) {
							isMLOC = true;
							fmloc.open((_SaveDir + FileTitle + "_mloc.txt").c_str());
							fmloc << "Date&Time\t";
							fmloc << "Day/Time\t";
							fmloc << "KC1\t" << "KC2\t" << "№\t" << "Texp\t";
							fmloc << "NumLoc\t" << "NumFixed\t";
							fmloc << "Mean\t" << "Sigma\t";
							fmloc << "\n";
						}
						fmloc << OutputDateTime(curDate).c_str() << "\t";
						fmloc << DayTimeToString(_tmi.timeBOKZ).c_str() << "\t";
						fmloc << uppercase << hex << setfill('0');
						fmloc << "0x" << setw(4) << _tmi.status1 << "\t";
						fmloc << "0x" << setw(4) << _tmi.status2 << "\t";
						fmloc << dec << setfill(' ');
						fmloc << setw(6) << _tmi.serialNumber << "\t";
						fmloc << setw(6) << _tmi.timeExp << "\t";
						fmloc << setw(6) << _tmi.nLocalObj << "\t";
						fmloc << setw(6) << _tmi.nFixedObj << "\t";
						fmloc << setw(6) << _tmi.MeanC << "\t";
						fmloc << setw(6) << _tmi.SigmaC << "\t";
						fmloc << "\n";

//						if (isLoadDb) {
//							TDateTime saveTime;// = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
//							GeneralizedDTMI gDTMI = clientDb->convertDTMI(mDTMI, saveTime);
//							vGeneralDTMI.push_back(gDTMI);
//						}

						struct CadrInfo mCadr;
						ConvertDataLOC(_tmi, mCadr, isM1000);
						plotter->AddPoint(ChartNumLoc, 0, _tmi.timeBOKZ * dtBokz, _tmi.nLocalObj);
						plotter->AddPoint(ChartFone,   0, _tmi.timeBOKZ * dtBokz, _tmi.MeanC);
						plotter->AddPoint(ChartNoise,  0, _tmi.timeBOKZ * dtBokz, _tmi.SigmaC);
						mCadr.Time = curDate.Val;//cntRecDTMI++;
						AddCadrInfo(mCadr, vCadrInfo);
						AddCadrInfo(mCadr, vLocalInfo);
 }

void TFormGraphOrient::DrawQuatStat(TDateTime timeFirst, TDateTime timeLast, int num, TColor arColor[2])
{
	if ( (num >= 0) && (num < 2) && (timeFirst != timeLast) ) {
		int nSeries = num;
		plotter->SetShowPoints(false);
		plotter->SetShowLines(true);
		plotter->SetLineWidth(4);
		plotter->SetDateTimeX(true);
		AnsiString title = "мБОКЗ-2 №" + IntToStr(num+1);
		plotter->SetTitle(title);
		plotter->SetSeriesColor(arColor[num]);

		int yes;
		if (!num) yes = 1;
		else yes= -1;

		plotter->AddPoint(ChartStat, nSeries, timeFirst.Val, 0, arColor[num]);
		plotter->AddPoint(ChartStat, nSeries, timeFirst.Val, yes, arColor[num]);
		plotter->AddPoint(ChartStat, nSeries, timeLast.Val,  yes, arColor[num]);
		plotter->AddPoint(ChartStat, nSeries, timeLast.Val,  0, arColor[num]);
	}
}

void __fastcall TFormGraphOrient::MenuOpenSamspaceClick(TObject *Sender)
{
	float testVar = 56;
	testVar = 6373;
	OpenDialog->Options.Clear();
	OpenDialog->Filter = "txt|*.txt";
	if (OpenDialog->Execute()) {

		TFormatSettings curFormat;
		curFormat.ShortDateFormat = "dd.mm.yyyy";
		TDateTime zeroDate = StrToDate(FormAnimateSetting->ZeroDate1S->Text);
		TDateTime curDate  = zeroDate;

		bool isLoadDb = FormAnimateSetting->CheckBoxLoadToDb->Checked;

		vCadrInfo.clear();
		DeleteLineGraph();

		AnsiString FileName = OpenDialog->FileName;
		AnsiString CurDir   = ExtractFileDir(FileName);
		AnsiString SaveDir  = CurDir + "/Result/";
		CreateDir(SaveDir);
		GetFileTitles(FileName,&FileTitle);

		ifstream finp(FileName.c_str());
		if (!finp.is_open()) {
			ShowMessage("Файл не может быть открыт!");
			return;
		}

		ofstream fout, fmshi, fshtmi1, fshtmi2, fgyro, fquat, ferr[2];
		fout.open((SaveDir + FileTitle + "_decode.txt").c_str());

		bool isM1000 = false;
		bool isSHTMI1 = 0, isSHTMI2 = 0, isGyro = 0, isQuat = 0, isErrorLog[2] = {0, 0};
		isDTMI = 0, isMLOC = 0, isOpenDtmi = 0;

		string str = AnsiString(FormAnimateSetting->EditListBokz->Text).c_str();
		if (str.find("БОКЗ-М60/1000") != string::npos) {
			isM1000 = true;
		}

		double dtBokz;
		if (isM1000) dtBokz = 0.25;
		else  dtBokz = 1.0;
		SetClkBokz(dtBokz);

		SHTMI1 mSHTMI1;
		SHTMI2 mSHTMI2;
		DTMI   mDTMI;
		LOC    mLOC;
		GYRO mGyro;
		QBOKZ mBokzQuat;
		ERR_BOKZ mError;

		TColor arColorFirst[2] = {clBlue, clGreen};
		TColor arColorLast[2]  = {clRed, clFuchsia};

		string line;
		while (!finp.eof())
		{
			getline(finp, line, '\n' );
			if ( (line.find("А6-3") != std::string::npos)
				   || (line.find("A6-3") != std::string::npos)) {
				if (isOpenDtmi) OutputDTMI(fout, SaveDir, mDTMI, zeroDate, isM1000);
				ClearGyro(mGyro);

				if (TryReadGyro(finp, mGyro)) {
					if (!isGyro) {
						fgyro.open((SaveDir + FileTitle + "_gyro.txt").c_str());
					}
					curDate = GetDateTime(zeroDate, mGyro.timeGyro);
					PrintLogGyro(fgyro, mGyro, curDate, isGyro);
				}
			}
			else if (line.find("У3-2 ") != std::string::npos) {
				if (isOpenDtmi) OutputDTMI(fout, SaveDir, mDTMI, zeroDate, isM1000);
				if (TryReadErrorBokz(finp, mError)) {
					int iBokz = 0;
					if (!isErrorLog[iBokz]) {
						ferr[iBokz].open((SaveDir + FileTitle + "_error-2.txt").c_str());
					}
					curDate = GetDateTime(zeroDate, mError.timeBoard);
					PrintLogError(ferr[iBokz], mError, curDate, isErrorLog[iBokz]);

					TDateTime curDateFirst = GetDateTime(zeroDate, mError.timeFirst);
					TDateTime curDateLast = GetDateTime(zeroDate, mError.timeLast);
					DrawQuatInfo(curDateFirst, mError.Qfirst, iBokz, arColorFirst);
					DrawQuatInfo(curDateLast, mError.Qlast, iBokz, arColorLast);
					DrawQuatStat(curDateFirst, curDateLast, iBokz, arColorFirst);

					double dT = (curDateLast.Val - curDateFirst.Val)*86400;
					if (dT > 0) {
						double MorntD_pr[3][3], MorntD[3][3], Wop[3];
						double Qfirst[4], Qlast[4];
						for (int i = 0; i < 4; i++) {
							Qfirst[i] =  (double)mError.Qfirst[i];
							Qlast[i] =  (double)mError.Qlast[i];
						}
						QuatToMatrix(Qfirst, MorntD_pr);
						QuatToMatrix(Qlast, MorntD);
						GetRotation(MorntD_pr,MorntD, Wop);
//						getAngularDisplacementFromOrientMatr(MorntD_pr, MorntD, Wop);
						plotter->AddPoint(ChartWx, 0, curDateLast.Val, Wop[0]/dT * RTD*60, clBlue);
						plotter->AddPoint(ChartWy, 0, curDateLast.Val, Wop[1]/dT * RTD*60, clBlue);
						plotter->AddPoint(ChartWz, 0, curDateLast.Val, Wop[2]/dT * RTD*60, clBlue);
					}
				}
			}
			else if (line.find("У3-4 ") != std::string::npos) {
				if (isOpenDtmi) OutputDTMI(fout, SaveDir, mDTMI, zeroDate, isM1000);
				if (TryReadErrorBokz(finp, mError)) {
					int iBokz = 1;
					if (!isErrorLog[iBokz]) {
						ferr[iBokz].open((SaveDir + FileTitle + "_error-4.txt").c_str());
					}
					curDate = GetDateTime(zeroDate, mError.timeBoard);
					PrintLogError(ferr[iBokz], mError, curDate, isErrorLog[iBokz]);

					TDateTime curDateFirst = GetDateTime(zeroDate, mError.timeFirst);
					TDateTime curDateLast = GetDateTime(zeroDate, mError.timeLast);
					DrawQuatInfo(curDateFirst, mError.Qfirst, iBokz, arColorFirst);
					DrawQuatInfo(curDateLast, mError.Qlast, iBokz, arColorLast);
					DrawQuatStat(curDateFirst, curDateLast, iBokz, arColorFirst);
					double dT = (curDateLast.Val - curDateFirst.Val)*86400;
					if (dT > 0) {
						double MorntD_pr[3][3], MorntD[3][3], Wop[3];
						double Qfirst[4], Qlast[4];
						for (int i = 0; i < 4; i++) {
							Qfirst[i] =  (double)mError.Qfirst[i];
							Qlast[i] =  (double)mError.Qlast[i];
						}
						QuatToMatrix(Qfirst, MorntD_pr);
						QuatToMatrix(Qlast, MorntD);
						GetRotation(MorntD_pr,MorntD, Wop);
//						getAngularDisplacementFromOrientMatr(MorntD_pr, MorntD, Wop);
						plotter->AddPoint(ChartWx, 1, curDateLast.Val, Wop[0]/dT * RTD*60, clGreen);
						plotter->AddPoint(ChartWy, 1, curDateLast.Val, Wop[1]/dT * RTD*60, clGreen);
						plotter->AddPoint(ChartWz, 1, curDateLast.Val, Wop[2]/dT * RTD*60, clGreen);
					}
				}
			}
			else if ( (line.find("В62-2") != std::string::npos)
				   || (line.find("В62-2") != std::string::npos)) {
				if (isOpenDtmi) OutputDTMI(fout, SaveDir, mDTMI, zeroDate, isM1000);
				ClearQuat(mBokzQuat);

				if (TryReadQuat(finp, mBokzQuat)) {
					if (!isQuat) {
						fquat.open((SaveDir + FileTitle + "_quat.txt").c_str());
					}
					curDate = GetDateTime(zeroDate, mBokzQuat.timeBokz1);
					PrintLogQuat(fquat, mBokzQuat, curDate, isQuat);
				}
			}
			else if ( (line.find("ШТМИ1") != string::npos)
				   || (line.find("У3-05") != std::string::npos)
				   || (line.find("У3-5") != std::string::npos)
				   || (line.find("U3-5") != std::string::npos)) {

				if (isOpenDtmi) OutputDTMI(fout, SaveDir, mDTMI, zeroDate, isM1000);
				ClearSHTMI1(mSHTMI1);

				if (TryReadSHTMI1(finp, mSHTMI1)) {

					PrintSHTMI1(fout, mSHTMI1);
					if (!isSHTMI1) {
						fshtmi1.open((SaveDir + FileTitle + "_shtmi1.txt").c_str());
					}
					PrintLogSHTMI1(fshtmi1, mSHTMI1, isSHTMI1);

					if (isLoadDb) {
						TDateTime saveTime;// = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
						GeneralizedSHTMI1 gSHTMI1 = clientDb->convertSHTMI1(mSHTMI1, saveTime);
						vGeneralSHTMI1.push_back(gSHTMI1);
					}
				}
			}
			else if ( (line.find("ШТМИ2") != string::npos)
				   || (line.find("У3-06") != std::string::npos)
				   || (line.find("У3-6") != std::string::npos)
				   || (line.find("U3-6") != std::string::npos) ) {

				if (isOpenDtmi) OutputDTMI(fout, SaveDir, mDTMI, zeroDate, isM1000);
				ClearSHTMI2(mSHTMI2);

				if (TryReadSHTMI2(finp, mSHTMI2)) {
					curDate = GetDateTime(zeroDate, mSHTMI2.timeBOKZ);
					PrintSHTMI2(fout, mSHTMI2, curDate);
					if (!isSHTMI2) {
						fshtmi2.open((SaveDir + FileTitle + "_shtmi2.txt").c_str());
					}
					PrintLogSHTMI2(fshtmi2, mSHTMI2, curDate, isSHTMI2);

					if (isLoadDb) {
						TDateTime saveTime;// = StrToDate(toUString(sdate)) +  StrToTime(toUString(stime));
						GeneralizedSHTMI2 gSHTMI2 = clientDb->convertSHTMI2(mSHTMI2, saveTime);
						vGeneralSHTMI2.push_back(gSHTMI2);
					}
				}
			}
			else if ( (line.find("# ДТМИ1") != string::npos)
				   || (line.find("У3-07") != std::string::npos)
				   || (line.find("U3-7") != std::string::npos) ) {

				if (isOpenDtmi) OutputDTMI(fout, SaveDir, mDTMI, zeroDate, isM1000);
				isOpenDtmi = true;
				ClearDTMI(mDTMI);   // or after OutputDTMI ?????
				TryReadDTMI(finp, mDTMI);
			}
			else if ( (line.find("# ДТМИ2") != string::npos)
				   || (line.find("У3-08") != std::string::npos)
				   || (line.find("U3-8") != std::string::npos) ) {
//				if (!isOpenDtmi)  {
//					isOpenDtmi = true;
//					ClearDTMI(mDTMI);
//				}
				TryReadDTMI(finp, mDTMI);
			}
			else if ( (line.find("# ДТМИ3") != string::npos)
				   || (line.find("У3-09") != std::string::npos)
				   || (line.find("U3-9") != std::string::npos) ) {
//				if (!isOpenDtmi)  {
//					isOpenDtmi = true;
//					ClearDTMI(mDTMI);
//				}
				TryReadDTMI(finp, mDTMI);
			}
			else if ( (line.find("# ДТМИ4") != string::npos)
				   || (line.find("У3-10") != std::string::npos)
				   || (line.find("U3-10") != std::string::npos) ) {
//				if (!isOpenDtmi)  {
//					isOpenDtmi = true;
//					ClearDTMI(mDTMI);
//				}
				TryReadDTMI(finp, mDTMI);
			}
			else if ( (line.find("# ДТМИ5") != string::npos)
				   || (line.find("У3-11") != std::string::npos)
				   || (line.find("U3-11") != std::string::npos) ) {
//				if (!isOpenDtmi)  {
//					isOpenDtmi = true;
//					ClearDTMI(mDTMI);
//				}
				TryReadDTMI(finp, mDTMI);
			}
			else if ( (line.find("# ДТМИ6") != string::npos)
				   || (line.find("У3-12") != std::string::npos)
				   || (line.find("U3-12") != std::string::npos) ) {
//				if (!isOpenDtmi)  {
//					isOpenDtmi = true;
//					ClearDTMI(mDTMI);
//				}
				TryReadDTMI(finp, mDTMI);
			}
			else if ( (line.find("# ДТМИ7") != string::npos)
				   || (line.find("У3-13") != std::string::npos)
				   || (line.find("U3-13") != std::string::npos) ) {
//				if (!isOpenDtmi)  {
//					isOpenDtmi = true;
//					ClearDTMI(mDTMI);
//				}
				TryReadDTMI(finp, mDTMI);
			}
			else if ( (line.find("# ДТМИ8") != string::npos)
				   || (line.find("У3-14") != std::string::npos)
				   || (line.find("U3-14") != std::string::npos) ) {
//				if (!isOpenDtmi)  {
//					isOpenDtmi = true;
//					ClearDTMI(mDTMI);
//				}
				TryReadDTMI(finp, mDTMI);
			}
			else if ( (line.find("# ДТМИ9") != string::npos)
				   || (line.find("У3-15") != std::string::npos)
				   || (line.find("U3-15") != std::string::npos) ) {
//				if (!isOpenDtmi)  {
//					isOpenDtmi = true;
//					ClearDTMI(mDTMI);
//				}
				TryReadDTMI(finp, mDTMI);
				if (isOpenDtmi) OutputDTMI(fout, SaveDir, mDTMI, zeroDate, isM1000);
			}
			else if ( (line.find("U3-16") != std::string::npos)
				   || (line.find("U3-17") != std::string::npos)
				   || (line.find("U3-18") != std::string::npos)
				   || (line.find("U3-19") != std::string::npos)
				   || (line.find("U3-20") != std::string::npos)
				   || (line.find("U3-21") != std::string::npos)
				   || (line.find("U3-22") != std::string::npos)
				   || (line.find("U3-23") != std::string::npos)	) {
				   TryReadLOC(finp, mLOC);
			}
			else if (line.find("U3-24") != std::string::npos) {
				TryReadLOC(finp, mLOC);
				OutputLOC(fout, SaveDir, mLOC, zeroDate, isM1000);
			}
		}

		if (isOpenDtmi) OutputDTMI(fout, SaveDir, mDTMI, zeroDate, isM1000);
		finp.close();
		fout.close();
		fshtmi1.close();
		fshtmi2.close();
		fdtmi.close();
		fmloc.close();
		ferr[0].close();
		ferr[1].close();

		PrepareStartDraw();
		CheckTabSheet();
	}
}
void TFormGraphOrient::DrawQuatInfo(TDateTime curDate, float Q[4], int num, TColor arColor[2])
{
	if ( (num >= 0) && (num < 2) ) {
		int nSeries = num;
		plotter->SetDateTimeX(true);
		AnsiString title = "мБОКЗ-2 №" + IntToStr(num+1);
		plotter->SetTitle(title);
		plotter->SetSeriesColor(arColor[num]);

		double Mornt[3][3], ang[3], Qdbl[4];
		for (int i = 0; i < 4; i++) {
			Qdbl[i] = (double) Q[i];
		}
		if ( !CheckQuatNorm(Qdbl, 0.001) ) {
			QuatToMatrix(Qdbl, Mornt);
			MatrixToEkvAngles(Mornt, ang);
			//------------------------------------------------------------//
			_DateTime stDate;
			double era1900, SunI[3], SunD[3];
			DecodeDateTime(curDate, stDate.Year, stDate.Month,  stDate.Day,
									stDate.Hour, stDate.Min, stDate.Sec, stDate.mSec);

			era1900 = DateTimeToDaysJ2000(&stDate)/36525. + 1;
			SunVector(era1900, Mornt, SunI, SunD);
			double angSun = acosm(SunD[2]);
			//------------------------------------------------------------//
			plotter->AddPoint(ChartAl, nSeries, curDate.Val, ang[0] * RTD, arColor[num]);
			plotter->AddPoint(ChartDl, nSeries, curDate.Val, ang[1] * RTD, arColor[num]);
			plotter->AddPoint(ChartAz, nSeries, curDate.Val, ang[2] * RTD, arColor[num]);
			plotter->AddPoint(ChartSunAngle, nSeries, curDate.Val, angSun * RTD, arColor[num]);
		}
    }
}

void __fastcall TFormGraphOrient::MenuOpenAistClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Filter = "txt|*.txt";
	if (OpenDialog->Execute()) {

		TFormatSettings curFormat;
		curFormat.ShortDateFormat = "dd.mm.yyyy";
		TDateTime zeroDate = StrToDate("01.01.1970");//FormAnimateSetting->ZeroDate1S->Text);
		TDateTime curDate  = zeroDate;

		bool isLoadDb = FormAnimateSetting->CheckBoxLoadToDb->Checked;

		vCadrInfo.clear();
		DeleteLineGraph();

		AnsiString FileName = OpenDialog->FileName;
		AnsiString CurDir   = ExtractFileDir(FileName);
		AnsiString SaveDir  = CurDir + "/Result/";
		CreateDir(SaveDir);
		GetFileTitles(FileName,&FileTitle);

		ifstream finp(FileName.c_str());
		if (!finp.is_open()) {
			ShowMessage("Файл не может быть открыт!");
			return;
		}

		ofstream fout, fquat[2], fmshi[2], fshtmi1, fshtmi2;
		fout.open((SaveDir + FileTitle + "_decode.txt").c_str());

		bool isSHTMI1 = 0, isSHTMI2 = 0, isMshi[2] = {0, 0}, isQuat[2] = {0, 0};
//		SetClkBokz(dtBokz);

		TAistShtmi1 mShtmi1;
		TAistShtmi2 mShtmi2;
		TAistMshi   mMshi;

		string line;
		string keyQuat[2] = {"U3-1 ", "U3-2 "};
		string keyMshi[2] = {"U3-27", "U3-28"};

		TColor arColorFirst[2] = {clBlue, clGreen};
		TColor arColorLast[2]  = {clRed, clFuchsia};

		while (!finp.eof())
		{
			getline(finp, line, '\n' );
			for (int iBokz = 0; iBokz < 2; iBokz++) {
				if (line.find(keyQuat[iBokz]) != std::string::npos) {
					TAistQuat   mBokzQuat;
					ClearAistQuat(mBokzQuat);
					if (TryReadAistQuat(finp, mBokzQuat)) {
						if (!isQuat[iBokz]) {
							fquat[iBokz].open((SaveDir + FileTitle + "_quat"+IntToStr(iBokz+1)+".txt").c_str());
						}
						curDate = GetDateTime(zeroDate, mBokzQuat.timeFirst);
						PrintLogAistQuat(fquat[iBokz], mBokzQuat, curDate, isQuat[iBokz]);

						curDate = GetDateTime(zeroDate, mBokzQuat.timeFirst);
						DrawQuatInfo(curDate, mBokzQuat.Qfirst, iBokz, arColorFirst);
						curDate = GetDateTime(zeroDate, mBokzQuat.timeLast);
						DrawQuatInfo(curDate, mBokzQuat.Qlast, iBokz, arColorLast);

//						CadrInfo mCadr;
//						mCadr.Time = curDate.Val;
//						mCadr.IsOrient = true;
//						for (int i = 0; i < 3; i++) {
//							mCadr.AnglesOrient[i] = ang[i];
//							mCadr.OmegaOrient[i]  = mshi.W[i];
//						}
//						AddCadrInfo(mCadr, vMshiInfo);
					}
				}
				else if (line.find(keyMshi[iBokz]) != std::string::npos) {
					ClearAistMshi(mMshi);
					if (TryReadAistMshi(finp, mMshi)) {
						if (!isMshi[iBokz]) {
							fmshi[iBokz].open((SaveDir + FileTitle + "_mshi"+IntToStr(iBokz+1)+".txt").c_str());
						}
						curDate = GetDateTime(zeroDate, mMshi.time_sec);
						PrintLogAistMshi(fmshi[iBokz], mMshi, curDate, isMshi[iBokz]);
					}
				}
			}

			if ( (line.find("ШТМИ1") != string::npos)
				   || (line.find("У3-05") != std::string::npos)
				   || (line.find("U3-5") != std::string::npos)) {

				ClearAistShtmi1(mShtmi1);
				if (TryReadAistShtmi1(finp, mShtmi1)) {

					PrintAistShtmi1(fout, mShtmi1);
					if (!isSHTMI1) {
						fshtmi1.open((SaveDir + FileTitle + "_shtmi1.txt").c_str());
					}
					PrintLogAistShtmi1(fshtmi1, mShtmi1, isSHTMI1);
				}
			}
			else if ( (line.find("ШТМИ2") != string::npos)
				   || (line.find("У3-06") != std::string::npos)
				   || (line.find("U3-6") != std::string::npos) ) {

				ClearAistShtmi2(mShtmi2);
				if (TryReadAistShtmi2(finp, mShtmi2)) {
					curDate = GetDateTime(zeroDate, mShtmi2.time_sec);
					PrintAistShtmi2(fout, mShtmi2, curDate);
					if (!isSHTMI2) {
						fshtmi2.open((SaveDir + FileTitle + "_shtmi2.txt").c_str());
					}
					PrintLogAistShtmi2(fshtmi2, mShtmi2, curDate, isSHTMI2);
				}
			}
		}

		finp.close();
		fout.close();
		fquat[0].close(); fquat[1].close();
		fmshi[0].close(); fmshi[1].close();
		fshtmi1.close();
		fshtmi2.close();

		PrepareStartDraw();
		CheckTabSheet();
	}
}

struct TTimeEmka {
	unsigned char year, month, day;
	unsigned char hour, min, sec;
};

int FindValueInList(unsigned short _val, unsigned short *list, int size)
{
	for (int i = 0; i < size; i++) {
		if (_val == list[i]) return i;
	}
	return -1;
}

int TimeEmkaToDateTime(TTimeEmka timeEmka, TDateTime &curTime)
{
	AnsiString stDate, stTime;
	int year, day, month, hour, min, sec;

	stDate.sprintf("%x %x %x", timeEmka.day,  timeEmka.month, timeEmka.year);
	sscanf(stDate.c_str(), "%ld %ld %ld", &day,  &month, &year);
	if ( (day >0) && (day <=31) && (month >0) && (month <=12) && (year >=0) && (year <100) ) {
		stTime.sprintf("%x %x %x", timeEmka.hour, timeEmka.min,   timeEmka.sec);
		sscanf(stTime.c_str(), "%ld %ld %ld", &hour, &min,   &sec);
		if ( (hour >= 0) && (hour <24) && (min >=0) && (min <60) && (sec >=0) && (sec <60) ){
			year += 2000;
			curTime = EncodeDate(year, month, day) + EncodeTime(hour, min, sec, 0);
			return 0;
		}
	}

    return -1;
}

int LoadOrient(TDateTime _curDate, double _Q[], CadrInfo &_mCadr)
{
	_DateTime stDate;
	double DJD, DJD1900, SunI[3], SunD[3];
	double Mornt[3][3], ang[3] = {0, 0, 0}, angSun = 0;

	if ( !CheckQuatNorm(_Q, 0.001) ) {
		QuatToMatrix(_Q, Mornt);
		MatrixToEkvAngles(Mornt, ang);
		DecodeDateTime(_curDate, stDate.Year, stDate.Month,  stDate.Day,
						stDate.Hour, stDate.Min, stDate.Sec, stDate.mSec);
		DJD = DateTimeToDaysJ2000(&stDate)/36525;
		DJD1900=(DJD + 1.0) * 36525 + 2415020.0;
		SunVector(DJD1900, Mornt, SunI, SunD);
		angSun = acosm(SunD[2]);
		_mCadr.IsOrient = true;
		for (int i = 0; i < 3; i++) {
			_mCadr.AnglesOrient[i] = ang[i];
		}
		_mCadr.SunAngle = angSun;

	}
	_mCadr.IsOrient = false;
}

void TFormGraphOrient::DrawMshi_2V(TMshi_2V *mshi, TDateTime curDate, int num)
{
	CadrInfo mCadr;
	TColor curColor;
	TColor arColorOk[2]  = {clBlue,   clGreen};
	TColor arColorErr[2] = {clYellow, clRed};

	float Tcmv, mxy;
	double Qdbl[4], Mornt[3][3];
	double ang[3] = {0, 0, 0}, angSun = 0;

	if ( (num >= 0) && (num < 2) ) {

		int nSeries = num;
		plotter->SetDateTimeX(true);
		plotter->SetSeriesColor(arColorOk[num]);
		AnsiString title = "мБОКЗ №" + IntToStr(num+1);
		plotter->SetTitle(title);

		mCadr.IsOrient = false;
		Tcmv = (mshi->Tcmv)/10.;
		mxy  = (mshi->mxy)/1e+4;
		//если выполнение команды завершено и ориентация определена успешно
		if ( (mshi->status1 & 0x4000) && !(mshi->status2 & 0xFF00)) {
			curColor = arColorOk[num];
			for (int i = 0; i < 4; i++) {
				 Qdbl[i] = (double)mshi->Qornt[i];
			}
			if ( !CheckQuatNorm(Qdbl, 0.001) ) {
				QuatToMatrix(Qdbl, Mornt);
				MatrixToEkvAngles(Mornt, ang);
				//------------------------------------------------------------//
				_DateTime stDate;
				double era1900, SunI[3], SunD[3];
				DecodeDateTime(curDate, stDate.Year, stDate.Month,  stDate.Day,
							stDate.Hour, stDate.Min, stDate.Sec, stDate.mSec);

				era1900 = DateTimeToDaysJ2000(&stDate)/36525. + 1;
				SunVector(era1900, Mornt, SunI, SunD);
				angSun = acosm(SunD[2]);
				//------------------------------------------------------------//
				plotter->AddPoint(ChartAl, nSeries, curDate.Val, ang[0] * RTD, curColor);
				plotter->AddPoint(ChartDl, nSeries, curDate.Val, ang[1] * RTD, curColor);
				plotter->AddPoint(ChartAz, nSeries, curDate.Val, ang[2] * RTD, curColor);
				plotter->AddPoint(ChartSunAngle, nSeries, curDate.Val, angSun * RTD, curColor);

				mCadr.IsOrient = true;
			}
			plotter->AddPoint(ChartWx, nSeries, curDate.Val, mshi->W[0] * RTS, curColor);
			plotter->AddPoint(ChartWy, nSeries, curDate.Val, mshi->W[1] * RTS, curColor);
			plotter->AddPoint(ChartWz, nSeries, curDate.Val, mshi->W[2] * RTS, curColor);
			plotter->AddPoint(ChartMxy, nSeries, curDate.Val, mxy * 1000., curColor);
			plotter->AddPoint(ChartNumDet,  nSeries, curDate.Val, mshi->NumDet, curColor);

//          FOR DEBUG ONLY
//			unsigned short stat = (mshi->status2 & 0xFF00) >> 8;
//			plotter->AddPoint(ChartStat, nSeries, curDate.Val, stat, curColor);
//          plotter->AddPoint(ChartCounter, nSeries, curDate.Val, stat, curColor);
		}
		else {
			unsigned short stat = (mshi->status2 & 0xFF00) >> 8;
			plotter->AddPoint(ChartStat, nSeries, curDate.Val, stat, arColorOk[num]);
			curColor = arColorErr[num];
		}
		plotter->AddPoint(ChartNumFrag, nSeries, curDate.Val, mshi->NumFrag, curColor);
		plotter->AddPoint(ChartNumLoc,  nSeries, curDate.Val, mshi->NumLoc, curColor);
		plotter->AddPoint(ChartLevel,   nSeries, curDate.Val, mshi->ThMax, curColor);

		if ( (Tcmv < 6253) || (Tcmv > 6254) ) {
			plotter->AddPoint(ChartTemp, nSeries, curDate.Val, Tcmv, curColor);
		}

		mCadr.Time = curDate.Val;
		if (mCadr.IsOrient) {
			for (int i = 0; i < 3; i++) {
				mCadr.AnglesOrient[i] = ang[i];
				mCadr.OmegaOrient[i]  = mshi->W[i];
			}
			mCadr.CountDeterObj = mshi->NumDet;
			mCadr.MeanErrorXY = mxy; //mm
			mCadr.SunAngle = angSun;
		}
		mCadr.CountStars = mshi->NumStar;
		mCadr.CountWindows  = mshi->NumFrag;
		mCadr.CountLocalObj = mshi->NumLoc;
		mCadr.MatrixTemp = Tcmv;
		mCadr.Level = mshi->ThMax;
		AddCadrInfo(mCadr, StarTrack[num]);
//		AddCadrInfo(mCadr, vCadrInfo);
	}
}

void TFormGraphOrient::DrawDtmi_2V(TDtmi_2V *dtmi, TDateTime curDate, int num)
{
	CadrInfo mCadr;
	TColor curColor;
	TColor arColorOk[2]  = {clBlue,   clGreen};
	TColor arColorErr[2] = {clYellow, clRed};

	float Tcmv, mxy;
	double Qdbl[4], Mornt[3][3];
	double ang[3] = {0, 0, 0}, angSun = 0;

	if ( (num >= 0) && (num < 2) ) {

		ObjectsInfo objInfo;
		objInfo.Dx = 0;
		objInfo.Dy = 0;
		objInfo.StarID = 0;
		objInfo.Mv = 0;
		objInfo.Sp[0] = '_';
		objInfo.Sp[1] = '_';

		mCadr.IsOrient = false;
		mCadr.ImageHeight = 1024;
		mCadr.ImageWidth  = 1024;
		mCadr.CountLocalObj = dtmi->NumLoc;
		mCadr.CountDeterObj = dtmi->NumDet;

		if (dtmi->NumFrag) {  //Слежение
			mCadr.Time = curDate.Val;
			if (dtmi->NumLoc <= 30) mCadr.SizeObjectsList = dtmi->NumLoc;
			else mCadr.SizeObjectsList = 30;
			DataLocObj* RsLocSl = (DataLocObj*)&dtmi->RsLocT[0];
			for (int i = 0; i < mCadr.SizeObjectsList; i++) {
				objInfo.X = RsLocSl[i].X;
				objInfo.Y = RsLocSl[i].Y;
				objInfo.Bright = RsLocSl[i].B;
				objInfo.Square = RsLocSl[i].S;
				mCadr.ObjectsList.push_back(objInfo);
			}
			mCadr.CountStars = dtmi->NumStar;
			mCadr.CountWindows = dtmi->NumFrag;
			if (mCadr.CountWindows <= 15) mCadr.SizeWindowsList = dtmi->NumFrag;
			else mCadr.SizeWindowsList = 15;

			WindowsInfo winInfo;
			float cftXY = 1/32., cftMean = 1/16.;
			for (int i = 0; i < mCadr.SizeWindowsList; i++) {
				int shift = (i%2) * 4;
				winInfo.xCenter = dtmi->XYc[i][0] * cftXY;
				winInfo.yCenter = dtmi->XYc[i][1] * cftXY;
				winInfo.Width  = 24;  //tmp
				winInfo.Height = 24;  //tmp
				winInfo.Xstart = winInfo.xCenter - (winInfo.Width >> 1);
				winInfo.Ystart = winInfo.yCenter - (winInfo.Height >> 1);
				winInfo.CountObj = (int)((dtmi->ObjFrag[i>>1] >> shift) & 0x0F);
				winInfo.Mean  = dtmi->MeanFrag[i][0] * cftMean;
				winInfo.Sigma = dtmi->MeanFrag[i][1] * cftMean;
				winInfo.Level = dtmi->ThFrag[i];
				mCadr.WindowsList.push_back(winInfo);
			}
			AddCadrInfo(mCadr, vCadrInfo);
		}
		else {  // НО/ТО
			double dt[2] = {dtmi->deltaT/1000./86400., 0};
			for (int nCadr = 0; nCadr < 2; nCadr++) {
				mCadr.Time = curDate.Val - dt[nCadr];
				if (dtmi->NumL[nCadr] <= 15) mCadr.SizeObjectsList = dtmi->NumL[nCadr];
				else mCadr.SizeObjectsList = 15;

				mCadr.ObjectsList.clear();
				for (int i = 0; i < mCadr.SizeObjectsList; i++) {
					objInfo.X = dtmi->RsLocT[nCadr][i].X;
					objInfo.Y = dtmi->RsLocT[nCadr][i].Y;
					objInfo.Bright = dtmi->RsLocT[nCadr][i].B;
					objInfo.Square = dtmi->RsLocT[nCadr][i].S;
					mCadr.ObjectsList.push_back(objInfo);
				}
				AddCadrInfo(mCadr, vCadrInfo);
			}
		}
	}
}

void TFormGraphOrient::DrawMloc_2V(TMloc_2V *mloc, TDateTime curDate, int num)
{
	CadrInfo mCadr;
	ObjectsInfo objInfo;
	objInfo.Dx = 0;
	objInfo.Dy = 0;
	objInfo.StarID = 0;
	objInfo.Mv = 0;
	objInfo.Sp[0] = '_';
	objInfo.Sp[1] = '_';

	if ( (num >= 0) && (num < 2) ) {
		mCadr.CountLocalObj = mloc->NumLoc;
		mCadr.ImageHeight = 1024;
		mCadr.ImageWidth  = 1024;
		mCadr.Time = curDate.Val;
		if (mloc->NumLoc <= 32) mCadr.SizeObjectsList = mloc->NumLoc;
		else mCadr.SizeObjectsList = 32;

		for (int i = 0; i < mCadr.SizeObjectsList; i++) {
			objInfo.X = mloc->ArrLoc[i][0];
			objInfo.Y = mloc->ArrLoc[i][1];
			objInfo.Bright = mloc->ArrLoc[i][2];
			objInfo.Square = abs(mloc->ArrLoc[i][3]);
			mCadr.ObjectsList.push_back(objInfo);
		}
		AddCadrInfo(mCadr, vCadrInfo);
		mCadr.IsOrient = false;
	}
}

void SwapArrByte(char *byte, int n)
{
	char buf;
	for (int i = 0; i < n; i=i+2) {
		buf = byte[i];
		byte[i] = byte[i+1];
		byte[i+1] = buf;
	}
}

void SwapMshi_2V(TMshi_2V *tmi)
{
	Swap2Word((short*)&tmi->time_sec);
	for (int i = 0; i < 3; i++) {
		Swap2Word((short*)&tmi->OZ[i]);
		Swap2Word((short*)&tmi->W[i]);
	}
	for (int i = 0; i < 4; i++) {
		Swap2Word((short*)&tmi->Qornt[i]);
	}
}

void SwapShtmi1_2V(TShtmi1_2V *tmi)
{
	Swap2Word((short*)&tmi->time_sec);
	Swap2Word((short*)&tmi->Foc);
	Swap2Word((short*)&tmi->Xg);
	Swap2Word((short*)&tmi->Yg);
}

void SwapDtmi_2V(TDtmi_2V *tmi)
{
	Swap2Word((short*)&tmi->time_sec);
	Swap2Word((short*)&tmi->timeLast_sec);
	Swap2Word((short*)&tmi->era);

	for (int i = 0; i < 3; i++) {
		Swap2Word((short*)&tmi->Wop[i]);
		Swap2Word((short*)&tmi->Vline[i]);
	}

	for (int i = 0; i < 4; i++) {
		Swap2Word((short*)&tmi->Qapr[i]);
		Swap2Word((short*)&tmi->Qlast[i]);
	}

	for (int i = 0; i < 15; i++) {
		for (int k = 0; k < 2; k++) {
			Swap2Word((short*)&tmi->RsLocT[k][i].X);
			Swap2Word((short*)&tmi->RsLocT[k][i].Y);
			Swap2Word((short*)&tmi->RsLocT[k][i].B);
		}
	}
}

void SwapMloc_2V(TMloc_2V *tmi)
{
	Swap2Word((short*)&tmi->time_sec);

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 4; j++) {
			Swap2Word((short*)&tmi->ArrLoc[i][j]);
		}
	}
}

void __fastcall TFormGraphOrient::MenuOpenEMKAClick(TObject *Sender)
{
	unsigned short cwMSHI[2]   = {0x4E00, 0x5600};
	unsigned short cwSHTMI1[2] = {0x4C40, 0x5440};
	unsigned short cwSHTMI2[2] = {0x4C60, 0x5460};
	unsigned short cwDTMI[2]   = {0x4C80, 0x5480};
	unsigned short cwMLOC[2]   = {0x4CA0, 0x54A0};
	unsigned short codeOC[2]   = {0x4800, 0x5000};

	FILE *fbin;
	TTimeEmka timeEmka;
	TDateTime curDate;//, curDateMloc, curDateDtmi;

	TBoardArray packet;
	TDtmi_2V arDtmi[2];
	TMloc_2V arMloc[2];

	ofstream fmshi[2], fshtmi1[2], fshtmi2[2], fdtmi[2], fmloc[2];
	bool isMshi[2] = {false, false};
	bool isShtmi1[2] = {false, false};
	bool isShtmi2[2] = {false, false};
	bool isDtmi[2] = {false, false};
	bool isMloc[2] = {false, false};
	bool isOpenDtmi[2] = {false, false}, isOpenMloc[2] = {false, false};
	unsigned char cbuf, indexBokz;

	OpenDialog->Options.Clear();
	OpenDialog->Filter = "bin|*.bin";
	if (OpenDialog->Execute()) {

		AnsiString FileName = OpenDialog->FileName;
		AnsiString CurDir   = ExtractFileDir(FileName);
		AnsiString SaveDir  = CurDir + "\\Расшифровка\\";
		CreateDir(SaveDir);
		GetFileTitles(FileName,&FileTitle);

		MakeCRC16Table();
		StarTrack[0].clear(); StarTrack[1].clear();
		vCadrInfo.clear();
		fbin = fopen(FileName.c_str(),"rb");
		while (!feof(fbin)) {
			if (fread(&cbuf, sizeof(char), 1, fbin) && (cbuf == 0xE5) ) {
				if (fread(&cbuf, sizeof(char), 1, fbin) && (cbuf == 0xE5) ) {
					fread(&timeEmka, sizeof(TTimeEmka), 1, fbin);

					if (!TimeEmkaToDateTime(timeEmka, curDate)) {
						fread(&packet, 1, 72, fbin);
						packet.crcCalc = GetCRC16((unsigned char*)&packet, 70);

						if (packet.crcCalc != packet.crcBoard) {

							unsigned short qPack[36], curPack[35];
							unsigned short newCrcBoard, newCrcCalc;
							memcpy(qPack, &packet, sizeof(qPack));
							fread(&newCrcBoard, sizeof(short), 1, fbin); //crcBoard

//-------------------------------------------------------------------------
//							TryFindPack(qPack, newCrcBoard, curPack);
							int cnt = 0;
							do  {
								for (int j = 0; j < cnt; j++) {
									curPack[j]=qPack[j];
								}
								for (int j = cnt+1; j < 36; j++) {
									curPack[j-1]=qPack[j];
								}
								newCrcCalc = GetCRC16((unsigned char*)curPack, 70);
								cnt++;
							} while ( (newCrcCalc != newCrcBoard) && (cnt < 36) );

							if (newCrcCalc == newCrcBoard) {
								memcpy(&packet, curPack, sizeof(curPack));
								packet.crcBoard = newCrcBoard;
								packet.crcCalc  = newCrcCalc;
							}
//-------------------------------------------------------------------------
						}
					}

//					fread(&packet.cmdWord, sizeof(short), 1, fbin);
//					int devIndex = FindValueInList(packet.cmdWord, cwMSHI, 2);
//					if (devIndex != -1) codeArr = MSHI_ARR;
//					else {
//						devIndex = FindValueInList(packet.cmdWord, cwSHTMI1, 2);
//						if (devIndex != -1) codeArr = SHTMI1_ARR;
//						else {
//							devIndex = FindValueInList(packet.cmdWord, cwSHTMI2, 2);
//							if (devIndex != -1) codeArr = SHTMI2_ARR;
//							else {
//								devIndex = FindValueInList(packet.cmdWord, cwDTMI, 2);
//								if (devIndex != -1) codeArr = DTMI_ARR;
//								else {
//									devIndex = FindValueInList(packet.cmdWord, cwMLOC, 2);
//									if (devIndex != -1) codeArr = MLOC_ARR;
//								}
//							}
//						}
//					}


//чтение МШИОР
					int devIndex = FindValueInList(packet.cmdWord, cwMSHI, 2);
					if ( (devIndex >= 0) && (devIndex < 2) ) {
						if (packet.ansWord == codeOC[devIndex]) {
							if (!isMshi[devIndex]) {
								fmshi[devIndex].open((SaveDir + FileTitle +
												"_mshi_"+IntToStr(devIndex)+".txt").c_str());
							}
							TMshi_2V *mshi = (TMshi_2V*)&packet.bufMIL;
							SwapMshi_2V(mshi);
							PrintMshi_2V(fmshi[devIndex], &packet, curDate, isMshi[devIndex]);
							if ( (packet.statTmi == 2) && (packet.crcCalc == packet.crcBoard) ) {
								DrawMshi_2V(mshi, curDate, devIndex);
							}
						}
					}
//чтение ШТМИ1
					devIndex = FindValueInList(packet.cmdWord, cwSHTMI1, 2);
					if ( (devIndex >= 0) && (devIndex < 2) ) {
						if (packet.ansWord == codeOC[devIndex]) {
							if (!isShtmi1[devIndex]) {
								fshtmi1[devIndex].open((SaveDir + FileTitle +
												"_shtmi1_"+IntToStr(devIndex)+".txt").c_str());
							}
							TShtmi1_2V *pTmi1 = (TShtmi1_2V*)(packet.bufMIL);
							SwapShtmi1_2V(pTmi1);
							PrintShtmi1_2V(fshtmi1[devIndex], &packet, curDate, isShtmi1[devIndex]);
						}
					}
//чтение ШТМИ2
					devIndex = FindValueInList(packet.cmdWord, cwSHTMI2, 2);
					if ( (devIndex >= 0) && (devIndex < 2) ) {
						if (packet.ansWord == codeOC[devIndex]) {
							if (!isShtmi2[devIndex]) {
								fshtmi2[devIndex].open((SaveDir + FileTitle +
												"_shtmi2_"+IntToStr(devIndex)+".txt").c_str());
							}
							TShtmi2_2V *pTmi2 = (TShtmi2_2V*)(packet.bufMIL);
							Swap2Word((short*)&pTmi2->time_sec);
							Swap2Word((short*)&pTmi2->cntSLEZH);
							PrintShtmi2_2V(fshtmi2[devIndex], &packet, curDate, isShtmi2[devIndex]);
							if ( (packet.statTmi == 2) && (packet.crcCalc == packet.crcBoard) ) {
								unsigned short stat = (pTmi2->status2 & 0xFF00) >> 8;
								if (!devIndex)
									plotter->AddPoint(ChartCounter, 0, curDate.Val, stat, clBlue);
								else plotter->AddPoint(ChartCounter, 0, curDate.Val, stat, clGreen);
							}
						}
					}
//чтение ДТМИ
					devIndex = FindValueInList(packet.cmdWord, cwDTMI, 2);
					if (devIndex == -1) devIndex = FindValueInList(packet.cmdWord, cwMLOC, 2);
					if ( (devIndex >= 0) && (devIndex < 2) ) {
						if (packet.ansWord == codeOC[devIndex]) {
							if (!isDtmi[devIndex]) {
								fdtmi[devIndex].open((SaveDir + FileTitle +
													"_dtmi_"+IntToStr(devIndex)+".txt").c_str());
								isDtmi[devIndex] = true;
							}

							fdtmi[devIndex] << dec << setfill(' ');
							fdtmi[devIndex] << AnsiString(DateTimeToStr(curDate)).c_str() << "\t";
							fdtmi[devIndex] << packet.bufMIL[0] << "\t" << packet.statTmi << "\t";
							fdtmi[devIndex] << uppercase << hex << setfill('0');
							fdtmi[devIndex] << packet.crcBoard << "\t" << packet.crcCalc << "\t";
							fdtmi[devIndex] << packet.cmdWord << "\n";

							switch (packet.bufMIL[0]) {
								case 3: {
									if (isOpenDtmi[devIndex]) {
										SwapDtmi_2V(&arDtmi[devIndex]);
										PrintDtmi_2V(fdtmi[devIndex], arDtmi[devIndex], curDate, isDtmi[devIndex]);
										isOpenDtmi[devIndex] = false;
									}
									ClearDtmi2V(arDtmi[devIndex]);
									memcpy(&arDtmi[devIndex].time_sec, &packet.bufMIL[1], sizeof(short) * 31);
									isOpenDtmi[devIndex] = true;
									break;
								}
								case 4: {
									memcpy(&arDtmi[devIndex].m_cur, &packet.bufMIL[1], sizeof(short) * 23);
									break;
								}
								case 5: {
									memcpy(&arDtmi[devIndex].pixFilterCnt[0], &packet.bufMIL[1], sizeof(short));
									memcpy(&arDtmi[devIndex].RsLocT[0][0].X, &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 6: {
									memcpy(&arDtmi[devIndex].pixFilterCnt[1], &packet.bufMIL[1], sizeof(short));
									memcpy(&arDtmi[devIndex].RsLocT[0][4].Y, &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 7: {
									memcpy(&arDtmi[devIndex].pixCurrent, &packet.bufMIL[1], sizeof(short));
									memcpy(&arDtmi[devIndex].RsLocT[0][8].B, &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 8: {
									memcpy(&arDtmi[devIndex].RsLocT[0][12].S, &packet.bufMIL[1], sizeof(short) * 31);
									break;
								}
								case 9: {
									memcpy(&arDtmi[devIndex].RsLocT[1][2].Y, &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 10: {
									memcpy(&arDtmi[devIndex].RsLocT[1][6].B, &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 11: {
									memcpy(&arDtmi[devIndex].RsLocT[1][10].S, &packet.bufMIL[2], sizeof(short) * 29);
									break;
								}
								case 12: {
									memcpy(&arDtmi[devIndex].XYc[0][0], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 13: {
									memcpy(&arDtmi[devIndex].MeanFrag[0][0], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 14: {
									memcpy(&arDtmi[devIndex].ThFrag[0], &packet.bufMIL[2], sizeof(short) * 23);
									SwapDtmi_2V(&arDtmi[devIndex]);
									PrintDtmi_2V(fdtmi[devIndex], arDtmi[devIndex], curDate, isDtmi[devIndex]);
									DrawDtmi_2V(&arDtmi[devIndex], curDate, devIndex);
									isOpenDtmi[devIndex] = false;
									break;
								}
								case 19: {
									if (isOpenMloc[devIndex]) {
										SwapMloc_2V(&arMloc[devIndex]);
										PrintMloc_2V(fmloc[devIndex], arMloc[devIndex], curDate, isMloc[devIndex]);
										isOpenMloc[devIndex] = false;
									}
									ClearMloc2V(arMloc[devIndex]);
									memcpy(&arMloc[devIndex].time_sec, &packet.bufMIL[1], sizeof(short) * 31);
									isOpenMloc[devIndex] = true;
									break;
								}
								case 20: {
									memcpy(&arMloc[devIndex].ArrLoc[2][1], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 21: {
									memcpy(&arMloc[devIndex].ArrLoc[6][0], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 22: {
									memcpy(&arMloc[devIndex].ArrLoc[9][3], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 23: {
									memcpy(&arMloc[devIndex].ArrLoc[13][2], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 24: {
									memcpy(&arMloc[devIndex].ArrLoc[17][1], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 25: {
									memcpy(&arMloc[devIndex].ArrLoc[21][0], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 26: {
									memcpy(&arMloc[devIndex].ArrLoc[24][3], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 27: {							
									memcpy(&arMloc[devIndex].ArrLoc[28][2], &packet.bufMIL[2], sizeof(short) * 29);
									SwapMloc_2V(&arMloc[devIndex]);
									PrintMloc_2V(fmloc[devIndex], arMloc[devIndex], curDate, isMloc[devIndex]);
									DrawMloc_2V(&arMloc[devIndex], curDate, devIndex);
									isOpenMloc[devIndex] = false;
									break;
								}
								default: break;
							}
						}
					}
//чтение МЛОК
/**
					devIndex = FindValueInList(packet.cmdWord, cwMLOC, 2);
					if ( (devIndex >= 0) && (devIndex < 2) ) {
						if (packet.ansWord == codeOC[devIndex]) {
							if (!isMloc[devIndex]) {
								fmloc[devIndex].open((SaveDir + FileTitle +
													"_mloc_"+IntToStr(devIndex)+".txt").c_str());
								isMloc[devIndex] = true;
							}

							fmloc[devIndex] << dec << setfill(' ');
							fmloc[devIndex] << AnsiString(DateTimeToStr(curDate)).c_str() << "\t";
							fmloc[devIndex] << "\t" << packet.bufMIL[0] << "\t" << packet.statTmi << "\t";
							fmloc[devIndex] << uppercase << hex << setfill('0');
							fmloc[devIndex] << "\t" << packet.crcBoard << "\t" << packet.crcCalc << "\n";

							switch (packet.bufMIL[0]) {
								case 19: {
									ClearMloc2V(arMloc);
									memcpy(&arMloc[devIndex].time_sec, &packet.bufMIL[1], sizeof(short) * 31);
									break;
								}
								case 20: {
									memcpy(&arMloc[devIndex].ArrLoc[2][1], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 21: {
									memcpy(&arMloc[devIndex].ArrLoc[6][0], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 22: {
									memcpy(&arMloc[devIndex].ArrLoc[9][3], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 23: {
									memcpy(&arMloc[devIndex].ArrLoc[13][2], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 24: {
									memcpy(&arMloc[devIndex].ArrLoc[17][1], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 25: {
									memcpy(&arMloc[devIndex].ArrLoc[21][0], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 26: {
									memcpy(&arMloc[devIndex].ArrLoc[24][3], &packet.bufMIL[2], sizeof(short) * 30);
									break;
								}
								case 27: {
									memcpy(&arMloc[devIndex].ArrLoc[28][2], &packet.bufMIL[2], sizeof(short) * 29);
									SwapMloc_2V(&arMloc[devIndex]);
									PrintMloc_2V(fmloc[devIndex], arMloc[devIndex], curDate, isMloc[devIndex]);
									break;
								}
								default: break;
							}
						}
					}
/**/
				}
			}
		}
		fclose(fbin);

		for (int i = 0; i < 2; i++) {
			if (isOpenDtmi[i]) {
				SwapDtmi_2V(&arDtmi[i]);
				PrintDtmi_2V(fdtmi[i], arDtmi[i], curDate, isDtmi[i]);
			}
			if (isOpenMloc[i]) {
				SwapMloc_2V(&arMloc[i]);
				PrintMloc_2V(fmloc[i], arMloc[i], curDate, isMloc[i]);
			}

			if (isMshi[i]) fmshi[i].close();
			if (isShtmi1[i]) fshtmi1[i].close();
			if (isShtmi2[i]) fshtmi2[i].close();
			if (isDtmi[i]) fdtmi[i].close();
			if (isMloc[i]) fmloc[i].close();
		}

		PrepareStartDraw();
		CheckTabSheet();

		int nRow = 1;
		for (int nDevice = 0; nDevice < 2; nDevice++) {
			if (StarTrack[nDevice].size()) {
				TableStatInfo->Cells[0][nRow++] = "мБОКЗ №" + IntToStr(nDevice + 1);
				CalculateOrientStat(StarTrack[nDevice], nDevice, nRow);
				CalculateParamStat(StarTrack[nDevice], nDevice, nRow);
			}
		}

		for (int i = 0; i < Charts.size(); i++) {
			plotter->CheckChartSeries(Charts[i]);
		}

		SaveScreenShots(SaveDir.c_str());
		SaveTableToFile(TableStatInfo, TableStatInfo->RowCount, TableStatInfo->ColCount,
									AnsiString(SaveDir + FileTitle + "_stat.txt").c_str());
	}
}
//---------------------------------------------------------------------------

	void PrintMshiMF(ofstream &file, struct TMshi_MF mshi, bool &create)
	{
		if (!create) {
			file << "Tbshv\t";
			file << "KC1\t" << "KC2\t";
			file << "Tpr\t";
			file << "Mornt\n";
			create = true;
		}

		file << dec << setfill(' ');
		file << mshi.time_sec << "\t(" << (mshi.dT*64*1E-6)+0.125 << ")\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << mshi.status1 << "\t";
		file << "0x" << setw(4) << mshi.status2 << "\t";

		file << dec << setfill(' ');
		file << mshi.Tpr << "\t";
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				file << mshi.Mornt[i][j] << "\t";
			}
		}
		file << "\n" << flush;
	}

	void SwapMshiSED(struct TMshi_SED &mshi)
	{
		Swap2Word((short*)&mshi.time_sec);
		short bufS4[4], bufE4[4];
		for (int i = 0; i < 4; i++) {
			memcpy(bufS4, &mshi.Qornt[i], sizeof(bufS4));
			bufE4[0] = bufS4[3]; bufE4[1] = bufS4[2];
			bufE4[2] = bufS4[1]; bufE4[3] = bufS4[0];
			memcpy(&mshi.Qornt[i], bufE4, sizeof(mshi.Qornt[i]));
		}
		for (int i = 0; i < 3; i++) {
			Swap2Word((short*)&mshi.W[i]);
		}
	}

	void PrintMshiSED(ofstream &file, struct TMshi_SED mshi, bool &create)
	{
		if (!create) {
			file << "Tbshv\t" << "dT1\t" << "dT2\t";
			file << "STAT\t" << "Mean\t" << "NumLoc\t" << "NumDet\t";
			file << "Qornt[0]\t" << "Qornt[1]\t" << "Qornt[2]\t" << "Qornt[3]\t";
			file << "WX, ''/sec\t" << "WY, ''/sec\t" << "WZ, ''/sec\n";
			create = true;
		}

		file << dec << setfill(' ');
		file << mshi.time_sec << "\t(" << (mshi.dT1*32*1E-6)-0.1 << ",\t" << mshi.dT2*20*1e-6 <<")\t";

		file << uppercase << hex << setfill('0');
		file << "0x" << setw(4) << mshi.status << "\t";

		file << dec << setfill(' ');
		file << mshi.mean << "\t" << (int)mshi.NumLoc << "\t" << (int)mshi.NumDet << "\t";
		for (int i = 0; i < 4; i++) {
				file << mshi.Qornt[i] << "\t";
		}
		for (int i = 0; i < 3; i++) {
				file << mshi.W[i] * RTS << "\t";
		}
		file << "\n" << flush;
	}

void TranspMatrix(double matrix[3][3], double matrixT[3][3])
{
	int i,j;
	double matrixC[3][3];

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			matrixC[i][j] = matrix[j][i];
		}
	}

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			matrixT[i][j] = matrixC[i][j];
		}
	}
}

//double GetTimeSED(unsigned int T, unsigned short dT)
//{
////	TDateTime startDate = EncodeDateTime(2000, 1, 1, 0, 0, 0, 0), curDate;
//	double timeSec = mshi.time_sec + (mshi.dT1*32*1E-6)-0.1 + mshi.dT2*20*1e-6;
//	curDate.Val = startDate.Val + timeSec/86400.;
//	return curDate;
//}

#define SizeBufMF 20
#define SizeBufSED 34

void __fastcall TFormGraphOrient::SpectrClick(TObject *Sender)
{
//	double M1[3][3] =  {{0.55, 0.54, -0.628},
//						{0.51, 0.37, 0.77},
//						{0.65, -0.75,-0.06}};
//
//	double M2[3][3] =  {{0.44, -0.85, -0.26},
//						{0.61, 0.074, 0.785},
//						{-0.65, -0.51,0.55}};
//
//	double M2T[3][3], M12[3][3];
//	TranspMatrix(M2, M2T);
//	multMatrix(M1, M2T, M12);

	plotter->SetPointSize(2);
	plotter->SetDateTimeX(true);
	TDateTime startDate = EncodeDateTime(2000, 1, 1, 0, 0, 0, 0);

	StarTrack[0].clear(); nameStarTrack[0] = "БОКЗ-МФ";
	StarTrack[1].clear(); nameStarTrack[1] = "SED-1";
	StarTrack[2].clear(); nameStarTrack[2] = "SED-2";
	vCadrInfo.clear();

	OpenDialog->Options.Clear();
	OpenDialog->Options << ofAllowMultiSelect;
	OpenDialog->Filter = "BOKZ-MF|*.e1.data";

	if (OpenDialog->Execute()) {

		AnsiString FileName = OpenDialog->FileName;
		AnsiString CurDir   = ExtractFileDir(FileName);
		AnsiString SaveDir  = CurDir + "\\Расшифровка\\";
		CreateDir(SaveDir);
		GetFileTitles(FileName,&FileTitle);
		ofstream fbokz(AnsiString(SaveDir + FileTitle +"(mf).txt").c_str());
		ofstream fquat(AnsiString(SaveDir + FileTitle +"(quat).txt").c_str());
		TMshi_MF mshi;
		bool isBokzMF = false;
		char bufh[SizeBufMF], nArrMshi = 20;
		double MorntD[3][3],  ang[3], QorntD[4], timeBokz_pr = -1;
		double MorntD_pr[3][3], Wop[3];

		for (int iFiles = 0; iFiles < OpenDialog->Files->Count; iFiles++) {

			FILE *fbin = fopen(AnsiString(OpenDialog->Files->Strings[iFiles]).c_str(),"rb");
			while (!feof(fbin)) {
				for (int i = 0; i < nArrMshi; i++) {
					if (!feof(fbin)) {
						fread(&mshi, sizeof(mshi), 1, fbin);
						SwapArrByte((char*)&mshi, sizeof(mshi));
						Swap2Word((short*)&mshi.time_sec);
						PrintMshiMF(fbokz, mshi, isBokzMF);
						double timeBokz = mshi.time_sec + (mshi.dT*64*1E-6 + 0.125) - 0.8;
						TDateTime curDate = startDate.Val + timeBokz/86400.;

						CadrInfo mCadr;
						mCadr.IsOrient = false;
						if (mshi.status1 == 0xE000) {

							for (int i1 = 0; i1 < 3; i1++) {
								for (int j1 = 0; j1 < 3; j1++) {
									MorntD[i1][j1] = (double)mshi.Mornt[i1][j1];
								}
							}

							MatrixToQuat(MorntD, QorntD);
							MatrixToEkvAngles(MorntD, ang);
							fquat << OutputDateTime2(curDate).c_str() << "\t";
							fquat << std::setprecision(12) << timeBokz << "\t";
							fquat << std::setprecision(8);
//							for (int i = 0; i < 3; i++)
//								for (int j = 0; j < 3; j++)
//									fquat << mshi.Mornt[i][j] << "\t";
							fquat << QorntD[0] << "\t" << QorntD[1] << "\t";
							fquat << QorntD[2] << "\t" << QorntD[3] << "\t";
							fquat << std::setprecision(7) << ang[0] * RTD << "\t";
							fquat << ang[1] * RTD << "\t" << ang[2] * RTD << "\n";

							plotter->SetTitle(nameStarTrack[0]);
							plotter->AddPoint(ChartAl, 0, curDate.Val, ang[0] * RTD, clBlue);
							plotter->AddPoint(ChartDl, 0, curDate.Val, ang[1] * RTD, clBlue);
							plotter->AddPoint(ChartAz, 0, curDate.Val, ang[2] * RTD, clBlue);
							if (timeBokz_pr > 0) {
								double dT = timeBokz - timeBokz_pr;
								if (dT > 0) {
									GetRotation(MorntD_pr,MorntD, Wop);
//									getAngularDisplacementFromOrientMatr(MorntD_pr, MorntD, Wop);
									plotter->AddPoint(ChartWx, 0, curDate.Val, Wop[0]/dT * RTS, clBlue);
									plotter->AddPoint(ChartWy, 0, curDate.Val, Wop[1]/dT * RTS, clBlue);
									plotter->AddPoint(ChartWz, 0, curDate.Val, Wop[2]/dT * RTS, clBlue);
								}
							}

							mCadr.Time = curDate.Val;
							mCadr.IsOrient = true;
							for (int i = 0; i < 3; i++) {
								mCadr.AnglesOrient[i] = ang[i];
							}
							AddCadrInfo(mCadr, StarTrack[0]);
							timeBokz_pr = timeBokz;
//							memcpy(Mornt_pr, mshi.Mornt, sizeof(Mornt_pr));
							memcpy(MorntD_pr, MorntD, sizeof(MorntD));
							//расчет угла оптической оси к Солнцу
						}
					}
				}
				if (!feof(fbin))
				{
					fread(bufh, sizeof(bufh), 1, fbin);
				}
			}
			fclose(fbin);
		}
		fbokz.close();
		fquat.close();
	}


	OpenDialog->Filter = "SED|*.e2.data";
	if (OpenDialog->Execute()) {

		AnsiString FileName = OpenDialog->FileName;
		AnsiString CurDir   = ExtractFileDir(FileName);
		AnsiString SaveDir  = CurDir + "\\Расшифровка\\";
		CreateDir(SaveDir);
		GetFileTitles(FileName,&FileTitle);
		ofstream fbokz(AnsiString(SaveDir + FileTitle +"(sed).txt").c_str());
		ofstream fquat[2];
		fquat[0].open(AnsiString(SaveDir + FileTitle +"(quat1).txt").c_str());
		fquat[1].open(AnsiString(SaveDir + FileTitle +"(quat2).txt").c_str());

		TMshi_SED mshi;
		bool isSED = false;
		char bufh[SizeBufSED], nArrMshi = 17;
		for (int iFiles = 0; iFiles < OpenDialog->Files->Count; iFiles++) {

			FILE *fbin = fopen(AnsiString(OpenDialog->Files->Strings[iFiles]).c_str(),"rb");
			while (!feof(fbin)) {
				for (int i = 0; i < nArrMshi; i++) {
					if (!feof(fbin)) {
						fread(&mshi, sizeof(mshi), 1, fbin);
						SwapArrByte((char*)&mshi, sizeof(mshi));
						SwapMshiSED(mshi);
						PrintMshiSED(fbokz, mshi, isSED);

						double timeSed = mshi.time_sec + (mshi.dT1*32*1E-6)-0.1 + mshi.dT2*20*1e-6;
						TDateTime curDate = startDate.Val + timeSed/86400.;

						CadrInfo mCadr;
						mCadr.IsOrient = false;
						if (mshi.status >= 0x4000) {
							double MorntD[3][3], Mres[3][3], ang[3];
							double Mrt[3][3] = {{0,0,1},
												{0,1,0},
												{-1,0,0}};

							QuatToMatrix(mshi.Qornt, MorntD);
							MultMatrix(Mrt, MorntD, Mres);
							MatrixToEkvAngles(Mres, ang);

							int nSeries =  mshi.status >> 14;
							unsigned short nSed = nSeries - 1;
							if (nSed < 2) {
								fquat[nSed] << OutputDateTime2(curDate).c_str() << "\t";
								fquat[nSed] << std::setprecision(12) << timeSed << "\t";
								fquat[nSed] << std::setprecision(8);
								fquat[nSed] << mshi.Qornt[0] << "\t" << mshi.Qornt[1] << "\t";
								fquat[nSed] << mshi.Qornt[2] << "\t" << mshi.Qornt[3] << "\t";
								fquat[nSed] << std::setprecision(7) << ang[0] * RTD << "\t";
								fquat[nSed] << ang[1] * RTD << "\t" << ang[2] * RTD << "\n";
							}

	//						nSeries = 1;
							plotter->SetTitle(nameStarTrack[nSeries]);
							plotter->AddPoint(ChartAl, nSeries, curDate.Val, ang[0] * RTD, clGreen);
							plotter->AddPoint(ChartDl, nSeries, curDate.Val, ang[1] * RTD, clGreen);
							plotter->AddPoint(ChartAz, nSeries, curDate.Val, ang[2] * RTD, clGreen);
							plotter->AddPoint(ChartWx, nSeries, curDate.Val, mshi.W[0] * RTS, clGreen);
							plotter->AddPoint(ChartWy, nSeries, curDate.Val, mshi.W[1] * RTS, clGreen);
							plotter->AddPoint(ChartWz, nSeries, curDate.Val, mshi.W[2] * RTS, clGreen);

							plotter->AddPoint(ChartNumLoc, nSeries, curDate.Val, mshi.NumLoc, clGreen);
							plotter->AddPoint(ChartNumDet, nSeries, curDate.Val, mshi.NumDet, clGreen);
							mCadr.Time = curDate.Val;
							mCadr.IsOrient = true;
							for (int i = 0; i < 3; i++) {
								mCadr.AnglesOrient[i] = ang[i];
								mCadr.OmegaOrient[i] = mshi.W[i];
							}
							mCadr.CountLocalObj = mshi.NumLoc;
							mCadr.CountDeterObj = mshi.NumDet;
							AddCadrInfo(mCadr, StarTrack[nSeries]);
	//                        расчет угла оптической оси к Солнцу
						}
					}
				}
				if (!feof(fbin))
				{
					fread(bufh, sizeof(bufh), 1, fbin);
				}
			}
			fclose(fbin);
		}
		fbokz.close();
		fquat[0].close();
		fquat[1].close();
	}

	OpenDialog->Filter = "TEMP|*.txt";
	if (OpenDialog->Execute()) {
		ifstream ftemp(AnsiString(OpenDialog->FileName).c_str());
		while (!ftemp.eof()) {
			char cc;
            float Tsed1, Tsed2, Tmf1, Tmf2;
			short day, month, year, hour, min, sec;
			ftemp >> day >> cc >> month >> cc >> year;
			ftemp >> hour >> cc >> min >> cc >> sec;
			ftemp >> Tsed1 >> Tsed2 >> Tmf1 >> Tmf2;
			TDateTime curDate = EncodeDateTime(year, month, day, hour, min, sec, 0);

				plotter->SetTitle("SED-1");
				plotter->AddPoint(ChartTemp, 0, curDate.Val, Tsed1, clGreen);

				plotter->SetTitle("SED-2");
				plotter->AddPoint(ChartTemp, 1, curDate.Val, Tsed2, clLime);

				plotter->SetTitle("MF-1");
				plotter->AddPoint(ChartTemp, 2, curDate.Val, Tmf1, clBlue);

				plotter->SetTitle("MF-2");
				plotter->AddPoint(ChartTemp, 3, curDate.Val, Tmf2, clAqua);
		}
	}

	PrepareStartDraw();
	CheckTabSheet();

	int nRow = 1;
	for (int nDevice = 0; nDevice < 3; nDevice++) {
		if (StarTrack[nDevice].size()) {
			TableStatInfo->Cells[0][nRow++] = nameStarTrack[nDevice];
			CalculateOrientStat(StarTrack[nDevice], nDevice, nRow);
			CalculateParamStat(StarTrack[nDevice], nDevice, nRow);
		}
	}

	for (int i = 0; i < Charts.size(); i++) {
		plotter->CheckChartSeries(Charts[i]);
	}

//	SaveScreenShots(SaveDir.c_str());
//	SaveTableToFile(TableStatInfo, TableStatInfo->RowCount, TableStatInfo->ColCount,
//									AnsiString(SaveDir + FileTitle + "_stat.txt").c_str());
}

void __fastcall TFormGraphOrient::BOKZ60ParseProtocolClick(TObject *Sender)
{
	try
	{
		OpenDialog->Options.Clear();
		OpenDialog->Filter = "txt|*.txt";
		OpenDialog->Options << ofAllowMultiSelect;
		if (OpenDialog->Execute()) {
			vCadrInfo.clear();
			DeleteLineGraph();
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			TDateTime startDate;
			FileAge(FileList->Strings[0], startDate);
			InitStatusInfoTable("M2");
			for (int i = 0; i < FileList->Count; i++)
			{
				AnsiString FileName = FileList->Strings[i];
				ifstream in(FileName.c_str());
				if (!in.is_open())
				{
					ShowMessage("Не удалось открыть файл");
					return;
				}
				if(checkLocFile(in))
				{
					HandleLoc60 handler (this);
					readBOKZ60LocProtocol (in, vCadrInfo, handler, startDate);
				}

				else
				{
					Handle60 handle(this);
				   //	readBOKZ60MSHIOR (in, vCadrInfo, handle, startDate);
					readBOKZ60Protocol(in, vCadrInfo, handle, startDate);
				}

			}

			FillStatusTable();
			PrepareStartDraw();
			CheckTabSheet();
			CalculateSeriesSKO();
		}
	}

	catch (exception &e)
	{
		ShowMessage(e.what());
	}
}


void TFormGraphOrient::FillStatusTable()
{
	for (int i = 0; i < vCadrInfo.size(); i++)
	{
		AddRowToStatusTable(vCadrInfo[i]);
	}

	int div = 0;
	for (int i = 1; i < TableStatusInfo->RowCount; i++)
	{
		div += StrToInt(TableStatusInfo->Cells[1][i]);
	}

	if (div == 0)
	   return;
	else
	{ 
		statusInfo.NoOneCount = div;
		statusInfo.NoFourCount = StrToFloat(TableStatusInfo->Cells[4][1]); 
		RatioEdit->Text = FloatToStr((statusInfo.NoFourCount / statusInfo.NoOneCount) * 100) ;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::BOKZM2VParseProtocolClick(TObject *Sender) {
		try {
		OpenDialog->Options.Clear();
		OpenDialog->Filter = "txt|*.txt";
		OpenDialog->Options << ofAllowMultiSelect;
		if (OpenDialog->Execute())
		{
			vCadrInfo.clear();
			DeleteLineGraph();
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			TDateTime startDate;
			FileAge(FileList->Strings[0], startDate);
				for (int i = 0; i < FileList->Count; i++)
				{
					AnsiString FileName = FileList->Strings[i];
					ifstream in(FileName.c_str());
					if (!in.is_open())
					{
						ShowMessage("Не удалось открыть файл");
						return;
					}

					HandleM2V handle(this);
					readmBOKZ2VProtocol(in, vCadrInfo, handle, startDate);
				}
		}
			PrepareStartDraw();
			CheckTabSheet();
		}
		catch (exception &e) {
			ShowMessage(e.what());
		}
}

void TFormGraphOrient::StartPrintReport(IKI_img* reader)
{
//	OpenWord(true);
//	AddDoc();
//	SetTextFormat(14,1,0,0,1);
//	AddParagraph("ПРОТОКОЛ проверки прибора " + AnsiString(reader->CameraSettings.DataSource.c_str())+"\n");
//
//	SetTextFormat(12,1,0,0,0);
//	AddParagraph("Параметры орбиты:");
//
//	int deg, min, sec;
//	AnsiString str;
//
//	SetTextFormat(12,0,0,0,0);
//	ToGMS(reader->Georeferencing.InclinationOrbit, deg, min, sec);
//	str.sprintf("%d\° %02d' %02d""", deg, min, sec);
//	AddParagraph("Наклонение орбиты................. " + str);
//
//	ToGMS(reader->Georeferencing.LongitudeAscendingNode, deg, min, sec);
//	str.sprintf("%d\° %02d' %02d""", deg, min, sec);
//	AddParagraph("Долгота восходящего узла....... " + str);
//
//	ToGMS(reader->Georeferencing.ArgumentPerigee, deg, min, sec);
//	str.sprintf("%d\° %02d' %02d""", deg, min, sec);
//	AddParagraph("Аргумент перицентра............... " + str);
//
//	AddParagraph("Радиус орбиты " + FloatToStr(reader->Georeferencing.LengthRadiusVector) +"км");
}

void TFormGraphOrient::PrintReportRes(vector <CadrInfo>& cadrInfo)
{
//	int count = cadrInfo.size();
//	AddParagraph("Количество кадров в обработке: " + IntToStr(count));
//
//	SetTextFormat(11,0,1,0,2);
//	AddParagraph("\nТаблица "+String(NumTab+1)+". Статистика параметров ориентации");
//	SetTextFormat(12,1,0,0,1);
//	AddTable(TableStatInfo->RowCount, TableStatInfo->ColCount);
//	for (int iRow = 0; iRow < TableStatInfo->RowCount; iRow++) {
//		for (int iCol = 0; iCol < TableStatInfo->ColCount; iCol++) {
//				SetCell(iRow+1, iCol+1, TableStatInfo->Cells[iCol][iRow]);
//		}
//		SetTextBold(0);
//	}
}

void TFormGraphOrient::CalculateLocalStat(vector <CadrInfo> &vLocal, int nSeries, int &countRow)
{
	Statistika statParam;

	if (nSeries < ChartFone->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.MeanBright; return a.MeanBright;} } GetMeanBright;
		statParam = calculateStatParam(vLocal.begin(), vLocal.end(), 0.0, GetMeanBright);
		AddRowToStatTable(countRow++, "Уровень фона (МЛОК), ЕМР", statParam, 8, 2);
		ChartFone->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 1)
									+ " до " + FloatToStrF(statParam.max, ffFixed, 8, 1) + ", МЛОК";
	}

	if (nSeries < ChartNoise->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.SigmaBright; return a.SigmaBright;} } GetSigmaBright;
		statParam = calculateStatParam (vLocal.begin(), vLocal.end(), 0.0, GetSigmaBright);
		AddRowToStatTable(countRow++, "СКО фона (МЛОК), ЕМР", statParam, 8, 2);
		ChartNoise->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 1)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 1) + ", МЛОК";
	}

	if (nSeries < ChartNumLoc->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.CountLocalObj; return a.CountLocalObj;} } GetCountLocalObj;
		statParam = calculateStatParam (vLocal.begin(), vLocal.end(), 0.0, GetCountLocalObj);
		AddRowToStatTable(countRow++, "Число объектов (МЛОК)", statParam, 8, 2);
		ChartNumLoc->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) + ", МЛОК";
	}
}

void TFormGraphOrient::CalculateOrientStat(vector <CadrInfo> &vOrient, int nSeries, int &countRow)
{
	Statistika statParam;

	if (nSeries < ChartAl->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AnglesOrient[0];} } GetAnglesOrientAl;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetAnglesOrientAl);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++, "Прямое восхождение, град (СКО - угл. сек)", statParam, 8, 4);
		ChartAl->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " '')";
	}

	if (nSeries < ChartDl->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AnglesOrient[1];} } GetAnglesOrientDl;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetAnglesOrientDl);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++,"Склонение, град (СКО - угл. сек)", statParam, 8, 4);
		ChartDl->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " '')";
	}

	if (nSeries < ChartAz->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AnglesOrient[2];} } GetAnglesOrientAz;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetAnglesOrientAz);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++, "Азимут, град (СКО - угл. сек)", statParam, 8, 4);
		ChartAz->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " '')";
	}

	if (nSeries < ChartWx->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaOrient[0];} } GetOmegaOrientF;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetOmegaOrientF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(countRow++, "Угловая скорость по оси OX, угл. сек/с", statParam, 8, 4);
		ChartWx->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c)";
	}

	if (nSeries < ChartWy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaOrient[1];} } GetOmegaOrientS;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetOmegaOrientS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(countRow++, "Угловая скорость по оси OY, угл. сек/с", statParam, 8, 4);
		ChartWy->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c)";
	}

	if (nSeries < ChartWz->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaOrient[2];} } GetOmegaOrientT;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetOmegaOrientT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(countRow++, "Угловая скорость по оси OZ, угл. сек/с", statParam, 8, 4);
		ChartWz->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c)";
	}

	if (nSeries < ChartSunAngle->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.SunAngle;} } GetSunAngle;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetSunAngle);
		statParam.mean *= RTD; 		statParam.sigma *= RTD;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++, "Угол опт. оси к Солнцу, град.", statParam, 8, 2);
		ChartSunAngle->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 1)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 1) + ", град.)";
	}
}

void TFormGraphOrient::CalculateParamStat(vector <CadrInfo> &vOrient, int nSeries, int &countRow)
{
	Statistika statParam;

	if (ChartLevel->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = false; return a.Level;} } GetLevel;
		statParam = calculateStatParam(vOrient.begin(), vOrient.end(), 0.0, GetLevel);
		AddRowToStatTable(countRow++, "Порог по фрагментам, ЕМР", statParam, 8, 1);
		ChartLevel->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									+ " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) + " ЕМР)";
	}

	if (nSeries < ChartNumFrag->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.CountWindows; return a.CountWindows;} } GetCountWindows;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetCountWindows);
		AddRowToStatTable(countRow++, "Число фрагментов", statParam, 8, 2);
		ChartNumFrag->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) +")";
	}

	if (nSeries < ChartNumLoc->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = false; return a.CountLocalObj;} } GetCountLocalObj;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetCountLocalObj);
		AddRowToStatTable(countRow++, "Число объектов", statParam, 8, 2);
		ChartNumLoc->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) +")";
	}

	if (nSeries < ChartNumDet->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.CountDeterObj;} } GetCountDeterObj;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetCountDeterObj);
		AddRowToStatTable(countRow++, "Число звезд", statParam, 8, 2);
		ChartNumDet->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) +")";
	}

	if (nSeries < ChartMxy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.MeanErrorXY;} } GetMeanErrXY;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetMeanErrXY);
		statParam.mean *= 1000; 	statParam.sigma *= 1000;
		statParam.min *= 1000;		statParam.max *= 1000;
		AddRowToStatTable(countRow++, "Ост. рассогласования, мкм", statParam, 8, 2);
		ChartMxy->Series[nSeries]->Title += " (среднее = " + FloatToStrF(statParam.mean, ffFixed, 8, 1) +" мкм)";
	}

	if (nSeries < ChartTemp->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = false; return a.MatrixTemp;} } GetMatrixTemp;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, GetMatrixTemp);
		AddRowToStatTable(countRow++, "Температура, град.", statParam, 8, 2);
//		ChartTemp->Series[nSeries]->Title += " (среднее = " + FloatToStrF(statParam.mean, ffFixed, 8, 1) +" град.)";
	}
}

void TFormGraphOrient::CalculateAngVzStat(vector <TAngVz> &vAngVz, int nSeries, int &countRow)
{
	Statistika statParam;

	if (nSeries < ChartAngXX->SeriesCount()) {
		struct { double operator() (const TAngVz& a, bool& f) {f = false; return a.Ang[0];} } GetAngVzXX;
		statParam = calculateStatParam (vAngVz.begin(), vAngVz.end(), 0.0, GetAngVzXX);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++, "Угол между осями OX, град (СКО - угл. сек)", statParam, 8, 4);
		ChartAngXX->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " '')";
	}
	if (nSeries < ChartAngYY->SeriesCount()) {
		struct { double operator() (const TAngVz& a, bool& f) {f = false; return a.Ang[1];} } GetAngVzYY;
		statParam = calculateStatParam (vAngVz.begin(), vAngVz.end(), 0.0, GetAngVzYY);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++, "Угол между осями OY, град (СКО - угл. сек)", statParam, 8, 4);
		ChartAngYY->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " '')";
	}
	if (nSeries < ChartAngZZ->SeriesCount()) {
		struct { double operator() (const TAngVz& a, bool& f) {f = false; return a.Ang[2];} } GetAngVzZZ;
		statParam = calculateStatParam (vAngVz.begin(), vAngVz.end(), 0.0, GetAngVzZZ);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++, "Угол между осями OZ, град (СКО - угл. сек)", statParam, 8, 4);
		ChartAngZZ->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " '')";
	}
}
void TFormGraphOrient::CalculateSeriesSKO()
{
	Statistika statParam;
	int numberRow = 1;

	if (ChartFone->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = false; return a.MeanBright;} } GetMeanBright;
		statParam = calculateStatParam(vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetMeanBright);
		AddRowToStatTable(numberRow++, "Уровень фона, ЕМР", statParam, 8, 2);
		ChartFone->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartNoise->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = false; return a.SigmaBright;} } GetSigmaBright;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetSigmaBright);
		AddRowToStatTable(numberRow++, "СКО фона, ЕМР", statParam, 8, 2);
		ChartNoise->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartNumFrag->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = false; return a.CountWindows;} } GetCountWindows;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetCountWindows);
		AddRowToStatTable(numberRow++, "Число фрагментов", statParam, 8, 2);
		ChartNumFrag->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartNumLoc->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = false; return a.CountLocalObj;} } GetCountLocalObj;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetCountLocalObj);
		AddRowToStatTable(numberRow++, "Число объектов", statParam, 8, 2);
		ChartNumLoc->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartNumDet->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = (a.CountDeterObj < 4); return a.CountDeterObj;} } GetCountDeterObj;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetCountDeterObj);
		AddRowToStatTable(numberRow++, "Число звезд", statParam, 8, 2);
		ChartNumDet->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartAl->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AnglesOrient[0];} } GetAnglesOrientAl;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetAnglesOrientAl);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(numberRow++, "Прямое восхождение, град/угл. сек", statParam, 8, 4);
//		ChartAl->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartDl->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AnglesOrient[1];} } GetAnglesOrientDl;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetAnglesOrientDl);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(numberRow++,"Склонение, град/угл. сек", statParam, 8, 4);
//		ChartDl->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartAz->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AnglesOrient[2];} } GetAnglesOrientAz;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetAnglesOrientAz);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(numberRow++, "Азимут, град/угл. сек", statParam, 8, 4);
//		ChartAz->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartAlError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AnglesDiff[0];} } GetAnglesDiffF;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetAnglesDiffF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка угла Al, угл. сек", statParam, 8, 4);
		ChartAlError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartDlError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AnglesDiff[1];} } GetAnglesDiffS;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetAnglesDiffS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++,"Ошибка угла Dl, угл. сек", statParam, 8, 4);
		ChartDlError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartAzError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AnglesDiff[2];} } GetAnglesDiffT;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetAnglesDiffT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка угла Az, угл. сек", statParam, 8, 4);
		ChartAzError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartErrorOX->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AxesDiff[0];} } GetAxesDiffF;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetAxesDiffF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка по оси OX, угл. сек", statParam, 8, 4);
		ChartErrorOX->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartErrorOY->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AxesDiff[1];} } GetAxesDiffS;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetAxesDiffS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка по оси OY, угл. сек", statParam, 8, 4);
		ChartErrorOY->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartErrorOZ->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.AxesDiff[2];} } GetAxesDiffT;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetAxesDiffT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка по оси OZ, угл. сек", statParam, 8, 4);
		ChartErrorOZ->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartWx->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaOrient[0];} } GetOmegaOrientF;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaOrientF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Угловая скорость по оси OX, угл. сек/с", statParam, 8, 4);
		ChartWx->Series[1]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c";
	}

	if (ChartWy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaOrient[1];} } GetOmegaOrientS;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaOrientS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Угловая скорость по оси OY, угл. сек/с", statParam, 8, 4);
		ChartWy->Series[1]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c";
	}

	if (ChartWz->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaOrient[2];} } GetOmegaOrientT;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaOrientT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Угловая скорость по оси OZ, угл. сек/с", statParam, 8, 4);
		ChartWz->Series[1]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c";
	}

	if (ChartWxError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaDiff[0];} } GetOmegaDiffF;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaDiffF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка Wx, угл. сек/с", statParam, 8, 4);
		ChartWxError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartWyError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaDiff[1];} } GetOmegaDiffS;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaDiffS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка Wy, угл. сек/с", statParam, 8, 4);
		ChartWyError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartWzError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaDiff[2];} } GetOmegaDiffT;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaDiffT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка Wz, угл. сек/с", statParam, 8, 4);
		ChartWzError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartMx->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.MeanErrorX;} } GetMeanErrX;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetMeanErrX);
		statParam.mean *= 1000; 	statParam.sigma *= 1000;
		statParam.min *= 1000;		statParam.max *= 1000;
		AddRowToStatTable(numberRow++, "mx, мкм", statParam, 8, 4);
		ChartMx->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartMy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.MeanErrorY;} } GetMeanErrY;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetMeanErrY);
		statParam.mean *= 1000; 	statParam.sigma *= 1000;
		statParam.min *= 1000;		statParam.max *= 1000;
		AddRowToStatTable(numberRow++, "my, мкм", statParam, 8, 4);
		ChartMy->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartMxy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.MeanErrorXY;} } GetMeanErrXY;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetMeanErrXY);
		statParam.mean *= 1000; 	statParam.sigma *= 1000;
		statParam.min *= 1000;		statParam.max *= 1000;
		AddRowToStatTable(numberRow++, "mxy, мкм", statParam, 8, 4);
		ChartMxy->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}
	TableStatInfo->RowCount = numberRow;

//	if (ChartWx->SeriesCount() > 1) {
//		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaModel[0];} } GetOmegaModelF;
//		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaModelF);
//		AddRowToStatTable(numberRow++, "Wx_model, угл. мин/с", statParam, 8, 4);
//				FloatToStrF(meanStd.first * RTM, ffFixed, 8, 4), FloatToStrF(meanStd.second * RTM, ffFixed, 8, 4));
//	}

//	if (ChartWy->SeriesCount() > 1) {
//		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaModel[1];} } GetOmegaModelS;
//		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaModelS);
//		AddRowToStatTable(numberRow++, "Wy_model, угл. мин/с", statParam, 8, 4);
//	}

//	if (ChartWz->SeriesCount() > 1) {
//		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaModel[2];} } GetOmegaModelT;
//		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaModelT);
//		AddRowToStatTable(numberRow++, "Wz_model, угл. мин/с", statParam, 8, 4);
//	}
}

void TFormGraphOrient::PrepareChartFrag(TColor *_colorFrag, const int _maxDrawFrag)
{
	for (int iFrag  = 0; iFrag < _maxDrawFrag; iFrag++) {
		_colorFrag[iFrag] = (TColor)RGB((float)(_maxDrawFrag - iFrag) / _maxDrawFrag * 255,
							200, (float)iFrag / _maxDrawFrag * 255 );
		plotter->SetShowLines(true);
		plotter->SetSeriesColor(_colorFrag[iFrag]);
		plotter->SetTitle(IntToStr(iFrag + 1));
		plotter->SetDateTimeX(FormAnimateSetting->CheckBoxDateTime->Checked);

		plotter->AddSeries(ChartFragBright, iFrag, _colorFrag[iFrag]);
		plotter->AddSeries(ChartFragSizeEl, iFrag, _colorFrag[iFrag]);
		plotter->AddSeries(ChartFragErrX, iFrag, _colorFrag[iFrag]);
		plotter->AddSeries(ChartFragErrY, iFrag, _colorFrag[iFrag]);

		plotter->AddSeries(ChartFragMean, iFrag, _colorFrag[iFrag]);
		plotter->AddSeries(ChartFragNoise, iFrag, _colorFrag[iFrag]);
		plotter->AddSeries(ChartFragLevel, iFrag, _colorFrag[iFrag]);
	}
}

void TFormGraphOrient::DrawChartFrag(const TColor *_colorFrag, const int _maxDrawFrag, CadrInfo& _cadrInfo)
{
	int iObject = 0;
	double Time =  _cadrInfo.Time;

	plotter->SetShowLines(true);
	for (int iFrag  = 0; iFrag < _cadrInfo.SizeWindowsList; iFrag++) {

		plotter->SetSeriesColor(_colorFrag[iFrag]);
		plotter->SetTitle(IntToStr(iFrag + 1));

		plotter->AddPoint(ChartFragMean, iFrag, Time,
			_cadrInfo.WindowsList[iFrag].Mean);
		plotter->AddPoint(ChartFragNoise, iFrag, Time,
			_cadrInfo.WindowsList[iFrag].Sigma);
		plotter->AddPoint(ChartFragLevel, iFrag, Time,
			_cadrInfo.WindowsList[iFrag].Level);

		for (int iObjFrag = 0; iObjFrag < _cadrInfo.WindowsList[iFrag].CountObj; iObjFrag++) {
			if (iFrag < _maxDrawFrag) {
				plotter->AddPoint(ChartFragBright, iFrag, Time,
					_cadrInfo.ObjectsList[iObject].Bright);
				if (_cadrInfo.ObjectsList[iObject].Square) {
					plotter->AddPoint(ChartFragSizeEl, iFrag, Time,
					_cadrInfo.ObjectsList[iObject].Square);
				}
				if (_cadrInfo.ObjectsList[iObject].StarID) {
					plotter->AddPoint(ChartFragErrX, iFrag, Time,
						_cadrInfo.ObjectsList[iObject].Dx*1000.);
					plotter->AddPoint(ChartFragErrY, iFrag, Time,
						_cadrInfo.ObjectsList[iObject].Dy*1000.);
				}
			}
			iObject++;
		}
	}
}

void __fastcall TFormGraphOrient::ReadIKIFormatClick(TObject *Sender)
{
	const int maxDrawFrag = 12;
	TColor colorFrag [maxDrawFrag];
	bool CompareIKIRes = false;
	int seriesID;
	try
	{
		OpenDialog->Filter = "iki|*.iki";
		FileOpenDialog1->Options << fdoPickFolders << fdoAllowMultiSelect;
		OpenDialog->Options << ofAllowMultiSelect;
		OpenDialog->FileName  = GetCurrentDir();
		if (OpenDialog->Execute())
		{
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			FileList->Sort();
			if (FileOpenDialog1->Execute())
			{
				FileTitle = "IKI";
				PrepareChartFrag(colorFrag, maxDrawFrag);
				if(FormAnimateSetting->CheckBoxResultOnly->Checked) seriesID = 0;
				else seriesID = 1;

				UnicodeString filePrefix = FormAnimateSetting->EditFilePrefix->Text;
				unsigned short startFrom = StrToInt(FormAnimateSetting->BeginFromEdit->Text);
				bool statusTableInited = false;
				bool skipFrame = FormAnimateSetting->SkipFrameCheckBox->Checked;
				unique_ptr <TStringList> FoldersList (new TStringList());
				FoldersList->Assign(FileOpenDialog1->Files);

				for (int curFolder = 0; curFolder < FoldersList->Count; curFolder++)
				{
					vCadrInfo.clear();
					plotter->ResetOptions();
					DeleteLineGraph();
					ClearStatusInfoTable();
					
					for (int i = startFrom; i < FileList->Count; i++)
					{
						unique_ptr <IKI_img> reader(new IKI_img());
						if(!AnsiContainsStr(FileList->Strings[i], filePrefix))
						{
							if (reader->ReadFormat(toStdString(FileList->Strings[i]), false, skipFrame))
							{
								TStringDynArray SplittedString = SplitString(FileList->Strings[i], "\\");
								UnicodeString ResFileName = FoldersList->Strings[curFolder] + "\\"
								+ filePrefix + SplittedString[SplittedString.Length - 1];
								if (FileExists(ResFileName)
								&& (reader->ReadFormat(toStdString(ResFileName), false, skipFrame)))
								{
									CompareIKIRes = true;
									if (!statusTableInited)
									{
										vector <string> splitted = split(reader->CameraSettings.DataSource, "_");
										if (!contains(splitted[0], "FrameMaker")
										&& !contains(splitted[0], "IdentStar"))
										{
											InitStatusInfoTable(splitted[0]);
//											statusTableInited = true;
										}
									}
									if ( (!i) /* && (FormAnimateSetting->CheckBoxPrintReport->Checked)*/ ) {
//									печать параметров модели
									//StartPrintReport(reader.get());
									}
								}
								else
								{
									CompareIKIRes = false;
								}
							}
							else ShowMessage("Не удалось считать " + AnsiString(FileList->Strings[i]));
						}

						if (CompareIKIRes)
						{
							if (FormAnimateSetting->CheckBoxOnlySummary->Checked)
							{
								vCadrInfo.clear();
							}
							vCadrInfo.push_back(move(convertIKIFormatToInfoCadr(reader.get(), CompareIKIRes)));
							double Time =  vCadrInfo.back().Time;
							plotter->SetDateTimeX(FormAnimateSetting->CheckBoxDateTime->Checked);

							plotter->SetShowLines(false);
							plotter->SetTitle("модель");
							plotter->SetSeriesColor(clLime);
							if(!FormAnimateSetting->CheckBoxResultOnly->Checked)
							{
								plotter->AddPoint(ChartAl, 0, Time, vCadrInfo.back().AnglesModel[0] * RTD);
								plotter->AddPoint(ChartDl, 0, Time, vCadrInfo.back().AnglesModel[1] * RTD);
								plotter->AddPoint(ChartAz, 0, Time, vCadrInfo.back().AnglesModel[2] * RTD);
								plotter->AddPoint(ChartWx, 0, Time, vCadrInfo.back().OmegaModel[0] * RTM);
								plotter->AddPoint(ChartWy, 0, Time, vCadrInfo.back().OmegaModel[1] * RTM);
								plotter->AddPoint(ChartWz, 0, Time, vCadrInfo.back().OmegaModel[2] * RTM);
							}

							plotter->SetTitle("измерения");
							plotter->SetSeriesColor(clBlue);
							plotter->AddPoint(ChartNumFrag, 0, Time, vCadrInfo.back().CountWindows);
							plotter->AddPoint(ChartNumLoc, 0, Time, vCadrInfo.back().CountLocalObj);
							plotter->AddPoint(ChartLevel, 0, Time, vCadrInfo.back().Level);
							plotter->AddPoint(ChartFone, 0, Time, vCadrInfo.back().MeanBright);
							plotter->AddPoint(ChartNoise, 0, Time, vCadrInfo.back().SigmaBright);
							//plotter->AddPoint(ChartTemp, 0, Time, vCadrInfo.back().MatrixTemp);

							if (vCadrInfo.back().IsOrient)
							{
								plotter->AddPoint(ChartNumDet, 0, Time, vCadrInfo.back().CountDeterObj);
								plotter->AddPoint(ChartMx, 0, Time, vCadrInfo.back().MeanErrorX * 1000.);
								plotter->AddPoint(ChartMy, 0, Time, vCadrInfo.back().MeanErrorY * 1000.);
								plotter->AddPoint(ChartMxy, 0, Time, vCadrInfo.back().MeanErrorXY * 1000.);
								plotter->AddPoint(ChartErrorOX, 0, Time, vCadrInfo.back().AxesDiff[0] * RTS);
								plotter->AddPoint(ChartErrorOY, 0, Time, vCadrInfo.back().AxesDiff[1] * RTS);
								plotter->AddPoint(ChartErrorOZ, 0, Time, vCadrInfo.back().AxesDiff[2] * RTS);
								plotter->AddPoint(ChartAlError, 0, Time, vCadrInfo.back().AnglesDiff[0] * RTS);
								plotter->AddPoint(ChartDlError, 0, Time, vCadrInfo.back().AnglesDiff[1] * RTS);
								plotter->AddPoint(ChartAzError, 0, Time, vCadrInfo.back().AnglesDiff[2] * RTS);
								plotter->AddPoint(ChartWxError, 0, Time, vCadrInfo.back().OmegaDiff[0] * RTS);
								plotter->AddPoint(ChartWyError, 0, Time, vCadrInfo.back().OmegaDiff[1] * RTS);
								plotter->AddPoint(ChartWzError, 0, Time, vCadrInfo.back().OmegaDiff[2] * RTS);

								plotter->AddPoint(ChartAl, seriesID, Time, vCadrInfo.back().AnglesOrient[0] * RTD);
								plotter->AddPoint(ChartDl, seriesID, Time, vCadrInfo.back().AnglesOrient[1] * RTD);
								plotter->AddPoint(ChartAz, seriesID, Time, vCadrInfo.back().AnglesOrient[2] * RTD);
								plotter->AddPoint(ChartWx, seriesID, Time, vCadrInfo.back().OmegaOrient[0] * RTM);
								plotter->AddPoint(ChartWy, seriesID, Time, vCadrInfo.back().OmegaOrient[1] * RTM);
								plotter->AddPoint(ChartWz, seriesID, Time, vCadrInfo.back().OmegaOrient[2] * RTM);

							//статистика по фрагментам
								DrawChartFrag(colorFrag, maxDrawFrag, vCadrInfo.back());

								//статистика по звездам -> DrawChartStars()
									plotter->SetShowLines(false);
									plotter->SetDateTimeX(false);
									if (vCadrInfo.back().IsBinary) {
										plotter->SetSeriesColor(clLime);
									}
									else plotter->SetSeriesColor(clBlue);

									for (int iObject = 0; iObject < vCadrInfo.back().SizeObjectsList; iObject++) {
										if (vCadrInfo.back().ObjectsList[iObject].StarID /* && Dx && Dy */) {
											plotter->AddPoint(ChartBrightMv,   0, vCadrInfo.back().ObjectsList[iObject].Mv,
																				  vCadrInfo.back().ObjectsList[iObject].Bright);
											if (vCadrInfo.back().ObjectsList[iObject].Square) {
												plotter->AddPoint(ChartSizeMv,     0, vCadrInfo.back().ObjectsList[iObject].Mv,
																				  vCadrInfo.back().ObjectsList[iObject].Square);
												plotter->AddPoint(ChartBrightSize, 0, vCadrInfo.back().ObjectsList[iObject].Square,
																		  vCadrInfo.back().ObjectsList[iObject].Bright);
											}

											if (vCadrInfo.back().ObjectsList[iObject].Sp != "__") {
												float tempSpec = GetTempSpec(vCadrInfo.back().ObjectsList[iObject].Sp);
												float brightMv0 = vCadrInfo.back().ObjectsList[iObject].Bright *
																	pow ((float)2.512, (float)vCadrInfo.back().ObjectsList[iObject].Mv);
												plotter->AddPoint(ChartBrightSp, 0, tempSpec, brightMv0);

											}
										}
									}
								}
						}

						if (i % 100 == 0)
						{
							Application->ProcessMessages();
							LabelFrameReport->Caption = "Cчитано " + IntToStr(i) + " файлов из " + IntToStr(FileList->Count);
						}
					}

					plotter->CheckChartSeries(ChartFragBright);
					plotter->CheckChartSeries(ChartFragSizeEl);
					plotter->CheckChartSeries(ChartFragErrX);
					plotter->CheckChartSeries(ChartFragErrY);
					plotter->CheckChartSeries(ChartFragMean);
					plotter->CheckChartSeries(ChartFragNoise);
					plotter->CheckChartSeries(ChartFragLevel);

					struct {
						bool operator()(const CadrInfo& a,const CadrInfo& b)
						{
							return a.Time < b.Time;
						}
					} CadrCompare ;


					sort(vCadrInfo.begin(), vCadrInfo.end(), CadrCompare);
					CalculateSeriesSKO();
					if (statusTableInited) {
						FillStatusTable();
					}

					if (FoldersList->Count == 1) {
						PrepareStartDraw();
					}

                    				//печать статистики по серии кадров
//			if (FormAnimateSetting->CheckBoxPrintReport->Checked) {
//				PrintReportRes(vCadrInfo);
//			}

					UnicodeString folder = SaveScreenShots(FoldersList->Strings[curFolder]);
					SaveTableToFile(TableStatInfo, TableStatInfo->RowCount, TableStatInfo->ColCount,
									toStdString(folder + "\\table_stat.txt"));
					SaveTableToFile(TableStatusInfo, TableStatusInfo->RowCount, TableStatusInfo->ColCount,
									toStdString(folder + "\\table_status.txt"));
					ofstream out(string(toStdString(folder) + "\\table_status.txt").c_str(), std::ofstream::out | std::ofstream::app);
					out << toStdString(RatioEdit->Text) <<"\t"<< statusInfo.NoOneCount <<"\t"<< statusInfo.NoFourCount;

				}
		}
	}
	CheckTabSheet();

}
   	catch (exception &e)
	{
		ShowMessage(e.what());
	}

}




 void __fastcall TFormGraphOrient::ChartMouseWheel(TObject *Sender, TShiftState Shift,
		  int WheelDelta, const TPoint &MousePos, bool &Handled)
{
	TChart* Chart = dynamic_cast <TChart*> (Sender);
	if (Shift.Contains(ssCtrl))
	{
		Chart->Zoom->Direction = tzdVertical;
	}
	else
	{
	   Chart->Zoom->Direction = tzdBoth;
	}
}


void __fastcall TFormGraphOrient::ChartMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	TChart* Chart = dynamic_cast <TChart*> (Sender);
	if (Shift.Contains(ssCtrl))
	{
		Chart->Zoom->Direction = tzdVertical;
	}
	else
	{
	   Chart->Zoom->Direction = tzdBoth;
	}
}


//---------------------------------------------------------------------------
void TFormGraphOrient::ResizePlot(TChart *chart, double kx, double ky, int indexX, int indexY)
{
	int width  = chart->Parent->Width/kx;
	int height = chart->Parent->Height/ky;

	chart->Width  = width;
	chart->Height = height;
	chart->Top  = indexY * height;
	chart->Left = indexX *width;
}

void __fastcall TFormGraphOrient::FormResize(TObject *Sender)
{
//углы ориентации
	ResizePlot(ChartAl, 1, 3, 0, 0);
	ResizePlot(ChartDl, 1, 3, 0, 1);
	ResizePlot(ChartAz, 1, 3, 0, 2);

//ошибка углов ориентации
	ResizePlot(ChartAlError, 1, 3, 0, 0);
	ResizePlot(ChartDlError, 1, 3, 0, 1);
	ResizePlot(ChartAzError, 1, 3, 0, 2);

//угловая скорость
	ResizePlot(ChartWx, 1, 3, 0, 0);
	ResizePlot(ChartWy, 1, 3, 0, 1);
	ResizePlot(ChartWz, 1, 3, 0, 2);

//ошибка угловой скорости
	ResizePlot(ChartWxError, 1, 3, 0, 0);
	ResizePlot(ChartWyError, 1, 3, 0, 1);
	ResizePlot(ChartWzError, 1, 3, 0, 2);

//ошибка осей ПСК
	ResizePlot(ChartErrorOX, 1, 3, 0, 0);
	ResizePlot(ChartErrorOY, 1, 3, 0, 1);
	ResizePlot(ChartErrorOZ, 1, 3, 0, 2);

//остаточные рассогласования
	ResizePlot(ChartMxy, 1, 3, 0, 0);
	ResizePlot(ChartMx,  1, 3, 0, 1);
	ResizePlot(ChartMy,  1, 3, 0, 2);

//число объектов
	ResizePlot(ChartNumFrag, 1, 3, 0, 0);
	ResizePlot(ChartNumLoc,  1, 3, 0, 1);
	ResizePlot(ChartNumDet,  1, 3, 0, 2);

//характеристики изображения
	ResizePlot(ChartFone,  1, 3, 0, 0);
	ResizePlot(ChartFone,  1, 3, 0, 1);
	ResizePlot(ChartNoise, 1, 3, 0, 2);

//дополнительные параметры
    ResizePlot(ChartSunAngle,  1, 3, 0, 0);
	ResizePlot(ChartTemp,  1, 3, 0, 1);

//статистика по фрагментам
	ResizePlot(ChartFragErrX,   2, 2, 0, 0);
	ResizePlot(ChartFragErrY,   2, 2, 0, 1);
	ResizePlot(ChartFragBright, 2, 2, 1, 0);
	ResizePlot(ChartFragSizeEl, 2, 2, 1, 1);

//статистика по фрагментам-2
	ResizePlot(ChartFragMean,  1, 3, 0, 0);
	ResizePlot(ChartFragNoise, 1, 3, 0, 1);
	ResizePlot(ChartFragLevel, 1, 3, 0, 2);

//статистика по звездам
	ResizePlot(ChartBrightMv, 2, 2, 0, 0);
	ResizePlot(ChartSizeMv,   2, 2, 0, 1);
	ResizePlot(ChartBrightSize, 2, 2, 1, 0);
	ResizePlot(ChartBrightSp,   2, 2, 1, 1);

//статистика по взаимным углам
	ResizePlot(ChartAngXX, 1, 3, 0, 0);
	ResizePlot(ChartAngYY, 1, 3, 0, 1);
	ResizePlot(ChartAngZZ, 1, 3, 0, 2);	

//дополнительные параметры
	ResizePlot(ChartStat,  1, 2, 0, 0);
	ResizePlot(ChartCounter,  1, 2, 0, 1);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::ChartsFragClickLegend(TCustomChart *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	TChart *currentChart = (TChart*)Sender;
	plotter->CheckGroupSeries(currentChart, ChartFragErrX);
	plotter->CheckGroupSeries(currentChart, ChartFragErrY);
	plotter->CheckGroupSeries(currentChart, ChartFragBright);
	plotter->CheckGroupSeries(currentChart, ChartFragSizeEl);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::ChartFrag2ClickLegend(TCustomChart *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	TChart *currentChart = (TChart*)Sender;
	plotter->CheckGroupSeries(currentChart, ChartFragMean);
	plotter->CheckGroupSeries(currentChart, ChartFragNoise);
	plotter->CheckGroupSeries(currentChart, ChartFragLevel);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::ChartOrientClickLegend(TCustomChart *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	TChart *currentChart = (TChart*)Sender;
	plotter->CheckGroupSeries(currentChart, ChartAl);
	plotter->CheckGroupSeries(currentChart, ChartDl);
	plotter->CheckGroupSeries(currentChart, ChartAz);
	plotter->CheckGroupSeries(currentChart, ChartWx);
	plotter->CheckGroupSeries(currentChart, ChartWy);
	plotter->CheckGroupSeries(currentChart, ChartWz);
}
//---------------------------------------------------------------------------


	// ---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::ChartMatrixClickLegend(TCustomChart *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	for (int i = 1; i < MaxBlockSeries; i++) {
		BlockSeries[i]->Visible = BlockSeries[0]->Visible;
	}
	for (int i = 1; i < MaxFrameSeries; i++) {
		FrameSeries[i]->Visible = FrameSeries[0]->Visible;
	}
}
//---------------------------------------------------------------------------



void __fastcall TFormGraphOrient::CheckBoxLimitClick(TObject *Sender)
{
	SetContrast();
	PixelBrightCheckBoxClick(this);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphOrient::BOKZMFParseProtocolClick(TObject *Sender)
{
		 OpenDialog->Options.Clear();
		 OpenDialog->Filter = "ip1|*.ip1";
		 OpenDialog->Options << ofAllowMultiSelect;
		 if (OpenDialog->Execute())
		 {
			plotter->SetDateTimeX(FormAnimateSetting->CheckBoxDateTime->Checked);
			vCadrInfo.clear();
			DeleteLineGraph();
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			TDateTime dt;
			for (int i = 0; i < FileList->Count; i++)
			{
				AnsiString FileName = FileList->Strings[i];
				ifstream in(FileName.c_str());
				if (!in.is_open())
				{
					ShowMessage("Не удалось открыть файл");
					return;
				}

				HandleMF handle(this);
				readBOKZMFProtocol(in, vCadrInfo, handle, dt);
			}

			PrepareStartDraw();
			CheckTabSheet();
		}
}
//---------------------------------------------------------------------------
bool CheckControlSumM2(unsigned short *buf)
{
	int sumInt = 0;
	for (int i = 0; i < 31; i++) {
		sumInt += buf[i];
	}
	unsigned short CS = ((sumInt & 0xFFFF0000) >> 16) + (sumInt & 0xFFFF);
	if (CS == buf[31]) return true;
	else return false;
}

void ConvertMshiM2(MSHI_M2 *mshi, TDateTime curDate, CadrInfo &mCadr)
{
	mCadr.Time = curDate.Val;
	mCadr.IsOrient = false;
	if ((mshi->status1 & 0x4000) && !(mshi->status2 & 0xFF00)) {
		double Qdbl[4], ang[3], angSun;
		for (int i = 0; i < 4; i++) {
			 Qdbl[i] = (double)mshi->Qornt[i];
		}
		if ( !GetOrientAng(Qdbl, curDate, ang, angSun)) {
			for (int i = 0; i < 3; i++) {
				mCadr.AnglesOrient[i] = ang[i];
				mCadr.OmegaOrient[i]  = mshi->W[i];
			}
			mCadr.CountDeterObj = mshi->nDeterObj;
			mCadr.MeanErrorXY = mshi->m_cur/10000.; //mm
			mCadr.SunAngle = angSun;
			mCadr.IsOrient = true;
		}
	}
    else int aaa = 5;

	mCadr.StatOrient = (mshi->status2 & 0xFF00) >> 8;
	mCadr.CountLocalObj = mshi->nLocalObj;
	mCadr.Level = mshi->thMax;
	mCadr.CountStars = 0;
	mCadr.CountWindows  = 0;
	mCadr.MatrixTemp = 0;
}

void ConvertMLOC_M2(MLOC_M2 *mloc, TDateTime curDate, CadrInfo &mCadr)
{
//		unsigned short meanCadr;
	mCadr.IsOrient = false;
	mCadr.ImageHeight = 512;
	mCadr.ImageWidth  = 512;

	mCadr.CountLocalObj = mloc->nLocalObj1;

	ObjectsInfo objInfo;
	objInfo.Dx = 0;
	objInfo.Dy = 0;
	objInfo.StarID = 0;
	objInfo.Mv = 0;
	objInfo.Sp[0] = '_';
	objInfo.Sp[1] = '_';

	mCadr.Time = curDate.Val;
	if (mloc->nLocalObj1 <= 36) mCadr.SizeObjectsList = mloc->nLocalObj1;
	else mCadr.SizeObjectsList = 36;

	mCadr.ObjectsList.clear();
	for (int i = 0; i < mCadr.SizeObjectsList; i++) {
		objInfo.X = mloc->LocalList[i].x;
		objInfo.Y = mloc->LocalList[i].y;
		objInfo.Bright = mloc->LocalList[i].bright;
		objInfo.Square = mloc->LocalList[i].size;
		mCadr.ObjectsList.push_back(objInfo);
	}
}

void ConvertDTMI_M2(DTMI_M2 *dtmi, TDateTime curDate, CadrInfo mCadr[2], int &cntCadr)
{
	ObjectsInfo objInfo;
	objInfo.Dx = 0;
	objInfo.Dy = 0;
	objInfo.StarID = 0;
	objInfo.Mv = 0;
	objInfo.Sp[0] = '_';
	objInfo.Sp[1] = '_';

	for (int nCadr = 0; nCadr < 2; nCadr++) {
		mCadr[nCadr].Time = curDate.Val;
		mCadr[nCadr].IsOrient = false;
		mCadr[nCadr].ImageHeight = 512;
		mCadr[nCadr].ImageWidth  = 512;
		mCadr[nCadr].ObjectsList.clear();
		if (dtmi->nLocalObj[nCadr] == -1)
			mCadr[nCadr].SizeObjectsList = 0;
		else if (dtmi->nLocalObj[nCadr] <= 15)
			mCadr[nCadr].SizeObjectsList = dtmi->nLocalObj[nCadr];
		else mCadr[nCadr].SizeObjectsList = 15;
		mCadr[nCadr].CountLocalObj = dtmi->nLocalObj[0];

		for (int i = 0; i < mCadr[nCadr].SizeObjectsList; i++) {
			objInfo.X = dtmi->LocalList[nCadr][i].x;
			objInfo.Y = dtmi->LocalList[nCadr][i].y;
			objInfo.Bright = dtmi->LocalList[nCadr][i].bright;
			objInfo.Square = dtmi->LocalList[nCadr][i].size;
			mCadr[nCadr].ObjectsList.push_back(objInfo);
		}
	}
    cntCadr = 2;
}

const int sizeCodeBokzM2 = 17;
unsigned short listCodeBokzM2[sizeCodeBokzM2] = {0x1AAA, 0x5AAA, 0x6AAA,
						0x7AAA, 0x8AAA, 0x9AAA, 0xAAAA, 0xBAAA, 0xCAAA, 0xDAAA,
						0xEAAA, 0xFAAA, 0xFAAB, 0xFAAC, 0xFAAD, 0xFAAE, 0xFAAF};
string nameCodeBokzM2[sizeCodeBokzM2] = {"MSHIOR", "SHTMI1", "SHTMI2",
						"DTMI1", "DTMI2", "DTMI3", "DTMI4", "DTMI5", "DTMI6", "DTMI7",
						"DTMI8", "DTMI9", "DTMI10", "DTMI11", "DTMI12", "DTMI13", "DTMI14"};
void __fastcall TFormGraphOrient::N21Click(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Filter = "Бинарные файлы|*.rsm|Текстовые файлы|*.txt|Все файлы|*.*";
//	OpenDialog->Options << ofAllowMultiSelect;

	if (OpenDialog->Execute()) {
		AnsiString SaveDir  = ExtractFileDir(OpenDialog->FileName) + "/Result/";
		CreateDir(SaveDir);
		GetFileTitles(OpenDialog->FileName,&FileTitle);

		bool isTMI1 = false, isTMI2 = false, isMSHI = false, isDTMI = false, isMLOC = false;
		ofstream fmshi, ftmi1, ftmi2, fdtmi, fmloc;

		DeleteLineGraph();
		StarTrack[0].clear(); StarTrack[1].clear(); StarTrack[2].clear();

		FILE *fbin;
//		unsigned short bufShort1, bufShort2;
//		unsigned char  bufC1, bufC2;
//		unsigned short bufDec;
//		fbin = fopen("Telem_M2.bin", "wb");
//		for (int i = 0; i < OpenDialog->Files->Count; i++) {
//			std::ifstream finp (AnsiString(OpenDialog->Files->Strings[i]).c_str());
//			if (finp.is_open()) {
//				while (!finp.eof()) {
//					finp >> dec;
//					finp >> bufDec >> " : ";
//					finp >> uppercase >> hex;
//					for (int i = 0; i < 8; i++) {
//						finp >> bufShort2 >> bufShort1;
//						bufC1 = bufShort1;
//						bufC2 = bufShort2;
//						fwrite(&bufC1, 1, 1, fbin);
//						fwrite(&bufC2, 1, 1, fbin);
//					}
//				}
//			}
//			finp.close();
//		}
//		fclose(fbin);
//
//		fbin = fopen("Telem_M2.bin", "rb");

		fbin = fopen(AnsiString(OpenDialog->FileName).c_str(), "rb");

		TFormatSettings curFormat;
		curFormat.ShortDateFormat = "dd.mm.yyyy";
		TDateTime zeroDate  = StrToDate(FormAnimateSetting->ZeroDate1S->Text);
		TDateTime curDate;

		std::ofstream fout (AnsiString(SaveDir + FileTitle + "_info.txt").c_str());
		unsigned short buf, bufMIL[32], bufDtmi[29*14];
		int isStartArray = 0;
		int cntErrorCS;
		while(!feof(fbin)) {
			fread(&bufMIL[0], 2, 1, fbin);
			if (bufMIL[0] == 0x42C3) {
				fread(&bufMIL[1], 2, 1, fbin);
				int nAr = FindValueInList(bufMIL[1], listCodeBokzM2, sizeCodeBokzM2);
				if (nAr != -1) {
					fread(&bufMIL[2], sizeof(short), 30, fbin);

					bool CS_OK = CheckControlSumM2(bufMIL);
					if (!CS_OK) {
						fout << "Error CS (" << nameCodeBokzM2[nAr] << ")\n";
						fout << "____________________________________" << "\n";
					}
					if (!nAr) {
						MSHI_M2 *mshi = (MSHI_M2 *)&bufMIL[2];
						int nBokzM2 = 0;
						switch (mshi->serialNumber) {
							case 5: nBokzM2 = 0; break;
							case 6: nBokzM2 = 1; break;
							case 7: nBokzM2 = 2; break;
							default: nBokzM2 = - 1; break;
						}

						curDate.Val = zeroDate.Val + mshi->timeBOKZ/86400.;
						if (!CS_OK) {
	//							curColor = clRed;
						}
						else {
							if (nBokzM2 != -1) {
								CadrInfo mCadr;
								ConvertMshiM2(mshi, curDate.Val, mCadr);
								AnsiString title = "БОКЗ-М2 № " + IntToStr(mshi->serialNumber);
//								TDateTime testDate = EncodeDate(2019, 8, 19) + EncodeTime(13, 40, 0, 0);
//								if (curDate > testDate) {
									DrawOrientInfo(mCadr, curDate, title, nBokzM2);
//								}
							}
						}
						PrintMSHI_M2(fout, *mshi, curDate);
						if (!isMSHI) {
							fmshi.open((SaveDir + FileTitle + "_orient.txt").c_str());
						}
						PrintLogMSHI_M2(fmshi, *mshi, curDate, isMSHI);
					}
					else if (nAr == 1) {
						TMI1_M2 *tmi1 = (TMI1_M2 *)&bufMIL[2];
						PrintTMI1_M2(fout, *tmi1);
						if (!isTMI1) {
							ftmi1.open((SaveDir + FileTitle + "_tmi1.txt").c_str());
						}
						PrintLogTMI1_M2(ftmi1, *tmi1, curDate, isTMI1);
					}
					else if (nAr == 2) {
						TMI2_M2 *tmi2 = (TMI2_M2 *)&bufMIL[2];
						PrintTMI2_M2(fout, *tmi2, curDate);
						if (!isTMI2) {
							ftmi2.open((SaveDir + FileTitle + "_tmi2.txt").c_str());
						}
						PrintLogTMI2_M2(ftmi2, *tmi2, curDate, isTMI2);
					}
					else if ( (nAr > 2) && (nAr < sizeCodeBokzM2) ) {

						if (bufMIL[1] == 0x7AAA) {
							isStartArray = 1;
							cntErrorCS = 0;
							if (!CS_OK) cntErrorCS++;
						}

						memcpy(&bufDtmi[(nAr - 3) * 29], &bufMIL[2], sizeof(short) * 29);
						if ( (bufMIL[1] == 0xFAAA) && (isStartArray) ) {
							MLOC_M2 *mloc = (MLOC_M2 *)bufDtmi;
							if (mloc->status2 == 0x0005) {
								curDate.Val = zeroDate.Val + mloc->timeBOKZ/86400.;
								PrintMLOC_M2(fout, *mloc, curDate);
								CadrInfo mCadr;
								ConvertMLOC_M2(mloc, curDate, mCadr);
								AddCadrInfo(mCadr, vCadrInfo);
								isStartArray = 0;
							}
						}
						else if ( (bufMIL[1] == 0xFAAF) && (isStartArray) ) {
							DTMI_M2 *dtmi = (DTMI_M2 *)bufDtmi;
							curDate.Val = zeroDate.Val + dtmi->timeBOKZ/86400.;
							PrintDTMI_M2(fout, *dtmi, curDate);
							if (!isDTMI) {
								fdtmi.open((SaveDir + FileTitle + "_dtmi.txt").c_str());
							}
							PrintLogDTMI_M2(fdtmi, *dtmi, curDate, cntErrorCS, isDTMI);

							int n;
							CadrInfo mCadr[2];
							ConvertDTMI_M2(dtmi, curDate, mCadr, n);
							for (int i = 0; i < n; i++) {
								AddCadrInfo(mCadr[i], vCadrInfo);
							}
							isStartArray = 0;
						}
					}
				}
			}
		}
		fclose(fbin);
		fout.close();
        fmshi.close();
		ftmi1.close();
		ftmi2.close();
		fdtmi.close();
		fmloc.close();

		ChartNumFrag->RemoveAllSeries();
		ChartTemp->RemoveAllSeries();

		int nRow = 1;
		for (int nBokz = 0; nBokz < 3; nBokz++) {
			if (StarTrack[nBokz].size()) {
					TableStatInfo->Cells[0][nRow++] = "БОКЗ-M2 №" + IntToStr(nBokz+5);
					CalculateOrientStat(StarTrack[nBokz], nBokz, nRow);
					CalculateParamStat(StarTrack[nBokz], nBokz, nRow);
			}
		}


		CheckTabSheet();
		PrepareStartDraw();
		for (int i = 0; i < Charts.size(); i++) {
            plotter->CheckChartSeries(Charts[i]);
		}
		plotter->CheckChartSeries(ChartFragBright);
		SaveScreenShots(SaveDir.c_str());
		SaveTableToFile(TableStatInfo, TableStatInfo->RowCount, TableStatInfo->ColCount,
									AnsiString(SaveDir + FileTitle + "_stat.txt").c_str());
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphOrient::BOKZM601000ParseProtocolClick(TObject *Sender)
{
		 OpenDialog->Options.Clear();
		 OpenDialog->Filter = "txt|*.txt";
		 OpenDialog->Options << ofAllowMultiSelect;
		 if (OpenDialog->Execute())
		 {
			vCadrInfo.clear();
			DeleteLineGraph();
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			unsigned int counter = 0;
			TDateTime startDate = TDateTime(2017,02,22,17,27,23,0);
			FileAge(FileList->Strings[0], startDate);
			InitStatusInfoTable("M1000");
			for (int i = 0; i < FileList->Count; i++)
			{
				AnsiString FileName = FileList->Strings[i];
				ifstream in(FileName.c_str());
				if (!in.is_open())
				{
					ShowMessage("Не удалось открыть файл");
					return;
				}

				Handle1000 handle(this);
				readBOKZ601000Protocol(in, vCadrInfo, counter, startDate, handle);
			}

			FillStatusTable();
			CalculateSeriesSKO();
			PrepareStartDraw();
			CheckTabSheet();
		}
	}

void __fastcall TFormGraphOrient::BOKZM2ParseProtocolClick(TObject *Sender)
{
		try {
		OpenDialog->Options.Clear();
		OpenDialog->Filter = "csv|*.csv";
		OpenDialog->Options << ofAllowMultiSelect;
		if (OpenDialog->Execute())
		{
			vCadrInfo.clear();
			DeleteLineGraph();
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			TDateTime startDate;
			FileAge(FileList->Strings[0], startDate);
			InitStatusInfoTable("M2");
				for (int i = 0; i < FileList->Count; i++)
				{
					AnsiString FileName = FileList->Strings[i];
					ifstream in(FileName.c_str());
					if (!in.is_open())
					{
						ShowMessage("Не удалось открыть файл");
						return;
					}

					bool isLoc = checkM2Loc(in);
					if (isLoc)
					{
						HandleLoc60 handle(this);
						readBOKZM2LocProtocol(in, vCadrInfo, handle, startDate);
					}
					else
					{
						HandleM2 handle(this);
						readBOKZM2Protocol(in, vCadrInfo, handle, startDate);
					}

				}

			FillStatusTable();
			CalculateSeriesSKO();
		}
			PrepareStartDraw();
			CheckTabSheet();
		}
		catch (exception &e) {
			ShowMessage(e.what());
		}
}
//---------------------------------------------------------------------------

 void TFormGraphOrient::SaveTableToFile(TStringGrid* table, short rowCount, short columnCount, string filename)
 {
	ofstream out(filename.c_str(), std::ofstream::out);
	if(out.is_open())
	{
		for (int i = 0; i < rowCount; i++)
		{
			string row;
			for (int j = 0; j < columnCount; j++)
			{
				row.append(toStdString(table->Cells[j][i]) + "\t");
			}
			row.append("\n");
			out << row; 
		}
	}
	else throw logic_error("Неверный путь к директории");
 }

void __fastcall TFormGraphOrient::BOKZMParseClick(TObject *Sender)
{
		try
	{
		OpenDialog->Options.Clear();
		OpenDialog->Filter = "mil|*.mil";
		OpenDialog->Options << ofAllowMultiSelect;
		if (OpenDialog->Execute()) {
			vCadrInfo.clear();
			DeleteLineGraph();
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			for (int i = 0; i < FileList->Count; i++)
			{
				AnsiString FileName = FileList->Strings[i];
				ifstream in(FileName.c_str());
				if (!in.is_open())
				{
					ShowMessage("Не удалось открыть файл");
					return;
				}
				HandleM handler (this);
				readBOKZMMil (in, vCadrInfo, handler);

			}

			PrepareStartDraw();
			CheckTabSheet();
			CalculateSeriesSKO();
		}
	}

	catch (exception &e)
	{
		ShowMessage(e.what());
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::SaveSeriesDataClick(TObject *Sender)
{
	 TComponent *pComponent = PopupMenu1->PopupComponent;
	 if (pComponent)
	 {
		TChart* chart = dynamic_cast <TChart*> (pComponent);
		if (chart)
		{
			string dirName = toStdString(GetCurrentDir()) + "\\" + "Графики в txt";
			TDirectory::CreateDirectoryW("Графики в txt");
			TDateTime currentDt = TDateTime::CurrentDateTime();
			UnicodeString dtStr;
			DateTimeToString(dtStr, UnicodeString("yyyy-MM-dd hh-mm-ss"), currentDt);
			string chartName =  toStdString(chart->Name) + "_" + toStdString(dtStr);
			string path = dirName + "\\" + chartName + ".txt";
			ofstream out(path.c_str(), std::ofstream::out);
			if (out.is_open())
			{
				for (int i = 0; i < chart->SeriesCount(); i++)
				{
					out << toStdString(chart->Series[i]->Name) << "\n";
					out << "X\t" << "Y\t" << "\n";
					for (int j = 0; j < chart->Series[i]->Count(); j++)
					{
						out << chart->Series[i]->XValues->Value[j] << "\t" << chart->Series[i]->YValues->Value[j] << "\n";
					}
					out << "===================================\n";
					out.close();
				}

			}

		}
	 }
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------



void __fastcall TFormGraphOrient::MenuOpenMeteorClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Filter = "txt|*.txt";
	if (OpenDialog->Execute()) {
		SetCurrentDir(ExtractFileDir(OpenDialog->FileName));
		FILE *fbin;
		unsigned short bufShort;
		fbin = fopen("Telem_Meteor.bin", "wb");
		std::ifstream finp (AnsiString(OpenDialog->FileName).c_str());
		if (finp.is_open()) {
			while (!finp.eof()) {
				finp >> uppercase >> hex;
				short buf[8];
				for (int i = 0; i < 8; i++) {
					finp >> bufShort;
					buf[i] = bufShort;
				}
				fwrite(&buf, sizeof(buf), 1, fbin);
			}
		}

		finp.close();
		fclose(fbin);
		fbin = fopen("Telem_Meteor.bin", "rb");
		std::ofstream fout ("Telem_Meteor.txt");
		unsigned short buf;
		DTMI_METEOR dtmi;
		unsigned long time2, time3, time4, time5;
		while(!feof(fbin)) {
			fread(&buf, 2, 1, fbin);
			if (buf == 0x42C3) {
				fread(&buf, 2, 1, fbin);
				switch (buf) {
					case 0x5AAA: {
						fread(&dtmi, sizeof(short), 30, fbin);
						break;
					}
					case 0x6AAA: {
						fread(&time2, sizeof(long), 1, fbin);
						fread(&dtmi.LocalList[2][0], sizeof(short), 28, fbin);
						break;
					}
					case 0x7AAA: {
						fread(&time3, sizeof(long), 1, fbin);
						fread(&dtmi.LocalList[5][2], sizeof(short), 28, fbin);
						break;
					}
					case 0x8AAA: {
						fread(&time4, sizeof(long), 1, fbin);
						fread(&dtmi.LocalList[9][0], sizeof(short), 28, fbin);
						break;
					}
					case 0x9AAA: {
						fread(&time5, sizeof(long), 1, fbin);
						fread(&dtmi.LocalList[12][2], sizeof(short), 28, fbin);
						Swap2Word((short *)&dtmi.timeBOKZ);
						Swap2Word((short *)&dtmi.Foc);
						Swap2Word((short *)&dtmi.Xg);
						Swap2Word((short *)&dtmi.Yg);
						for (int i = 0; i < 16; i++) {
							for (int j = 0; j < 4; j++) {
								Swap2Word((short *)&dtmi.LocalList[i][j]);
							}
						}
						PrintDtmiMeteor(fout, dtmi);
						break;
					}
					default: break;
				}
			}
		}
		fclose(fbin);
		fout.close();
	}
}
//---------------------------------------------------------------------------

void TFormGraphOrient::GetZoomChart(double t1, double t2)
{
	vector <CadrInfo> tmpStarTrack[MaxStarTrack];
	for (int nBokz = 0; nBokz < MaxStarTrack; nBokz++) {
		tmpStarTrack[nBokz].clear();
		if (StarTrack[nBokz].size()) {
			for (int i = 0; i < StarTrack[nBokz].size(); i++) {
				if ( (StarTrack[nBokz][i].Time >= t1) && (StarTrack[nBokz][i].Time <= t2) ) {
					tmpStarTrack[nBokz].push_back(StarTrack[nBokz][i]);
				}
			}
			if (tmpStarTrack[nBokz].size()) {
				int nRow = 1;
				CalculateOrientStat(tmpStarTrack[nBokz], nBokz, nRow);
//				CalculateParamStat(tmpStarTrack[nBokz], nBokz, nRow);
			}
		}
		tmpStarTrack[nBokz].clear();
	}
}


void __fastcall TFormGraphOrient::ChartZoom(TObject *Sender)
{
	TChart *pChart;
	pChart = (TChart*)Sender;
	double timeStart = pChart->BottomAxis->Minimum;
	double timeStop  = pChart->BottomAxis->Maximum;
//	GetZoomChart(timeStart, timeStop);
	ChartAl->BottomAxis->Automatic = false;
	ChartAl->BottomAxis->Minimum = timeStart;
	ChartAl->BottomAxis->Maximum = timeStop;
	ChartDl->BottomAxis->Automatic = false;
	ChartDl->BottomAxis->Minimum = timeStart;
	ChartDl->BottomAxis->Maximum = timeStop;
	ChartAz->BottomAxis->Automatic = false;
	ChartAz->BottomAxis->Minimum = timeStart;
	ChartAz->BottomAxis->Maximum = timeStop;
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::ChartMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
//	TChart *pChart;
//	pChart = (TChart*)Sender;
//	double timeStart = pChart->BottomAxis->Minimum;
//	double timeStop  = pChart->BottomAxis->Maximum;
//
//	ChartAl->BottomAxis->Automatic = false;
//	ChartAl->BottomAxis->Minimum = timeStart;
//	ChartAl->BottomAxis->Maximum = timeStop;
//	ChartDl->BottomAxis->Automatic = false;
//	ChartDl->BottomAxis->Minimum = timeStart;
//	ChartDl->BottomAxis->Maximum = timeStop;
//	ChartAz->BottomAxis->Automatic = false;
//	ChartAz->BottomAxis->Minimum = timeStart;
//	ChartAz->BottomAxis->Maximum = timeStop;
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::ChartDblClick(TObject *Sender)
{
	ChartAl->BottomAxis->Automatic = true;
	ChartDl->BottomAxis->Automatic = true;
	ChartAz->BottomAxis->Automatic = true;
	ChartAl->LeftAxis->Automatic = true;
	ChartDl->LeftAxis->Automatic = true;
	ChartAz->LeftAxis->Automatic = true;
}
//---------------------------------------------------------------------------

void ConvertMshi2R_old(const TMshi_2R_old &mshi, TDateTime curDate, CadrInfo &mCadr)
{
	mCadr.Time = curDate.Val;
	mCadr.TimeBoard = mshi.time_sec + mshi.time_ms/1000.;
	mCadr.IsOrient = false;
	if ((mshi.status1 & 0xE000) && !(mshi.status2 & 0xFF00)) {
		double Qdbl[4], ang[3], angSun;
		for (int i = 0; i < 4; i++) {
			 Qdbl[i] = (double)mshi.Qornt[i];
		}
		if ( !GetOrientAng(Qdbl, curDate, ang, angSun)) {
			for (int i = 0; i < 3; i++) {
				mCadr.AnglesOrient[i] = ang[i];
				mCadr.OmegaOrient[i]  = mshi.W[i];
			}
			mCadr.CountDeterObj = mshi.NumDet;
			mCadr.MeanErrorXY = mshi.mxy/(1E+5); //mm   //(1E+4)
			mCadr.SunAngle = angSun;
			mCadr.IsOrient = true;
		}
	}
	mCadr.StatOrient = (mshi.status2 & 0xFF00) >> 8;
	mCadr.CountStars = mshi.NumStar;
	mCadr.CountWindows  = mshi.NumFrag;
	mCadr.CountLocalObj = mshi.NumLoc;
	mCadr.MatrixTemp = mshi.Tcmv/10.;
	mCadr.Level = mshi.ThMax;
}

void ConvertMshi2R(const TMshi_2R &mshi, TDateTime curDate, CadrInfo &mCadr)
{
	mCadr.Time = curDate.Val;
	mCadr.TimeBoard = mshi.time_sec + mshi.time_ms/1000.;
	mCadr.IsOrient = false;
	if ((mshi.status1 & 0xE000) && !(mshi.status2 & 0xFF00)) {
		double Qdbl[4], ang[3], angSun;
		for (int i = 0; i < 4; i++) {
			 Qdbl[i] = (double)mshi.Qornt[i];
		}
		if ( !GetOrientAng(Qdbl, curDate, ang, angSun)) {
			for (int i = 0; i < 3; i++) {
				mCadr.AnglesOrient[i] = ang[i];
				mCadr.OmegaOrient[i]  = mshi.W[i];
			}
			mCadr.CountDeterObj = mshi.NumDet;
			mCadr.MeanErrorXY = mshi.mxy/(1E+5); //mm   //(1E+4)
			mCadr.SunAngle = angSun;
			mCadr.IsOrient = true;
		}
	}
	mCadr.StatOrient = (mshi.status2 & 0xFF00) >> 8;
//	mCadr.CountStars = mshi.NumStar;
	mCadr.CountWindows  = mshi.NumFrag;
	mCadr.CountLocalObj = mshi.NumLoc;
	mCadr.MatrixTemp = mshi.Tcmv/10.;
	mCadr.Level = mshi.ThMax;
    mCadr.MeanBright = mshi.Mean;
}

void ConvertDtmi2R(const TDtmi_2R &dtmi, TDateTime curDate, CadrInfo arCadr[2], int &cntCadr)
{
    CadrInfo mCadr;
	ObjectsInfo objInfo;

	objInfo.Dx = 0;
	objInfo.Dy = 0;
	objInfo.StarID = 0;
	objInfo.Mv = 0;
	objInfo.Sp[0] = '_';
	objInfo.Sp[1] = '_';

	mCadr.IsOrient = false;
	mCadr.ImageHeight = 1024;
	mCadr.ImageWidth  = 1024;
	mCadr.CountLocalObj = dtmi.NumLoc;
	mCadr.CountDeterObj = dtmi.NumDet;
	mCadr.MatrixTemp = dtmi.Tcmv/10.;

	if (dtmi.NumFrag) {  //Слежение
		mCadr.Time = curDate.Val;
		mCadr.TimeBoard = dtmi.time_sec + dtmi.time_ms/1000.;
		if (dtmi.NumLoc <= 30) mCadr.SizeObjectsList = dtmi.NumLoc;
		else mCadr.SizeObjectsList = 30;
		stLocal* RsLocSl = (stLocal*)&dtmi.RsLocT[0];
		for (int i = 0; i < mCadr.SizeObjectsList; i++) {
			objInfo.X = RsLocSl[i].x;
			objInfo.Y = RsLocSl[i].y;
			if (FloatToInteger(RsLocSl[i].bright, objInfo.Bright)) {
				objInfo.Bright = -1;
			}
			objInfo.Square = RsLocSl[i].size;
			mCadr.ObjectsList.push_back(objInfo);
		}
		mCadr.CountStars = dtmi.NumStar;
		mCadr.CountWindows = dtmi.NumFrag;
		if (mCadr.CountWindows <= 15) mCadr.SizeWindowsList = dtmi.NumFrag;
		else mCadr.SizeWindowsList = 15;

		WindowsInfo winInfo;
		float cftXY = 1/32., cftMean = 1/16.;
		for (int i = 0; i < mCadr.SizeWindowsList; i++) {
			int shift = (i%2) * 4;
			winInfo.xCenter = dtmi.XYc[i][0] * cftXY;
			winInfo.yCenter = dtmi.XYc[i][1] * cftXY;
//			winInfo.Width  = 24;  //tmp
//			winInfo.Height = 24;  //tmp
			GetSizeFrag_2R( ((dtmi.MultXY[i>>1] >> shift) & 0x0F),
							&winInfo.Width, &winInfo.Height);
			winInfo.Xstart = winInfo.xCenter - (winInfo.Width >> 1);
			winInfo.Ystart = winInfo.yCenter - (winInfo.Height >> 1);
			winInfo.CountObj = (int)((dtmi.ObjFrag[i>>1] >> shift) & 0x0F);
			winInfo.Mean  = dtmi.MeanFrag[i][0] * cftMean;
			winInfo.Sigma = dtmi.MeanFrag[i][1] * cftMean;
			winInfo.Level = dtmi.ThFrag[i];
			winInfo.ZipX = dtmi.ZipXY[i] & 0x0F;
			winInfo.ZipY = dtmi.ZipXY[i] >> 4;
			mCadr.WindowsList.push_back(winInfo);
		}
		arCadr[0] = mCadr;
		cntCadr = 1;
	}
	else {  // НО/ТО
		double dt[2] = {0.45, 0};//{dtmi.deltaT, 0};
		for (int nCadr = 0; nCadr < 2; nCadr++) {
			mCadr.Time = curDate.Val - dt[nCadr]/86400.;
			mCadr.TimeBoard = dtmi.time_sec + dtmi.time_ms/1000. - dt[nCadr];
			if (dtmi.NumL[nCadr] <= 15) mCadr.SizeObjectsList = dtmi.NumL[nCadr];
			else mCadr.SizeObjectsList = 15;

			mCadr.ObjectsList.clear();
			for (int i = 0; i < mCadr.SizeObjectsList; i++) {
				objInfo.X = dtmi.RsLocT[nCadr][i].x;
				objInfo.Y = dtmi.RsLocT[nCadr][i].y;
				objInfo.Bright = dtmi.RsLocT[nCadr][i].bright;
				objInfo.Square = dtmi.RsLocT[nCadr][i].size;
				mCadr.ObjectsList.push_back(objInfo);
			}
			arCadr[nCadr] = mCadr;
		}
		cntCadr = 2;
	}
}

void ConvertMloc2R(const TMloc_2R &mloc, TDateTime curDate, CadrInfo &mCadr)
{
	ObjectsInfo objInfo;
	objInfo.Dx = 0;
	objInfo.Dy = 0;
	objInfo.StarID = 0;
	objInfo.Mv = 0;
	objInfo.Sp[0] = '_';
	objInfo.Sp[1] = '_';

	mCadr.Time = curDate.Val;
	mCadr.TimeBoard = mloc.time_sec + mloc.time_ms/1000.;
	mCadr.IsOrient = false;
	mCadr.ImageHeight = 1024;
	mCadr.ImageWidth  = 1024;
	mCadr.CountLocalObj = mloc.NumLoc;
	if (mloc.NumLoc <= 36) mCadr.SizeObjectsList = mloc.NumLoc;   //NumFix = 0!!!!
	else mCadr.SizeObjectsList = 36;
//	mCadr.MeanBright = mloc->mean;
	mCadr.SigmaBright = mloc.sigma;
	mCadr.Level = mloc.Th;
	mCadr.CountDeterObj = 0;

	mCadr.ObjectsList.clear();
	for (int i = 0; i < mCadr.SizeObjectsList; i++) {
		objInfo.X = mloc.RsLoc[i].x;
		objInfo.Y = mloc.RsLoc[i].y;
		if (FloatToInteger(mloc.RsLoc[i].bright, objInfo.Bright)) {
			objInfo.Bright = -1;
		}
		objInfo.Bright = mloc.RsLoc[i].bright;
		objInfo.Square = mloc.RsLoc[i].size;
		mCadr.ObjectsList.push_back(objInfo);
	}
}

void __fastcall TFormGraphOrient::MenuOpen2R_MILClick(TObject *Sender)
{
	double zeroEra;
	const int sizeMIL = 32;
	wstring line, word;
	unsigned short bufMIL[sizeMIL], bufMloc[sizeMIL * 9];
	TDtmi_2R bufDtmi;
	unsigned short cmdWord, ansWord;
	short addr, subaddr, cntWord;
	bool isSHTMI1 = false, isSHTMI2 = false, isMSHI = false,
		 isDTMI = false, isMLOC = false;
	ofstream finfo, fshtmi1, fshtmi2, fmshi, fdtmi, fmloc;

	OpenDialog->Options.Clear();
	OpenDialog->Filter = "Текстовые файлы|*.dat; *.txt|Все файлы|*.*";
	OpenDialog->Options << ofAllowMultiSelect;
	if (OpenDialog->Execute()) {

    	TFormatSettings curFormat;
		curFormat.ShortDateFormat = "dd.mm.yyyy";
		TDateTime zeroDate = StrToDate(FormAnimateSetting->ZeroDate1S->Text)
					   	   + StrToTime(TimeToStr(FormAnimateSetting->ZeroTime1S->Time));
		TDateTime curDate = zeroDate;

		AnsiString SaveDir  = ExtractFileDir(OpenDialog->FileName) + "/Result/";
		CreateDir(SaveDir);
		StarTrack[0].clear(); StarTrack[1].clear();

		for (int iFile = 0; iFile < OpenDialog->Files->Count; iFile++) {

			isSHTMI1 = isSHTMI2 = isMSHI = isDTMI = isMLOC = false;
			wifstream finp(AnsiString(OpenDialog->Files->Strings[iFile]).c_str());
			finp.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
//			TFileStream* finps = new TFileStream(OpenDialog->Files->Strings[iFile],fmOpenRead);
//			string strstr;
//			finps->Read(&strstr, sizeof(strstr));

			if (!finp.is_open()) {
				ShowMessage("Файл не может быть открыт!");
				return;
			}

			GetFileTitles(OpenDialog->Files->Strings[iFile],&FileTitle);
			finfo.open((SaveDir + FileTitle + "_info.txt").c_str());
			SetClkBokz(1.0);
			bool oldFormat_2R = false;
			finp >> hex;
			while (!finp.eof())
			{
//				getline(finp, line, L'\n' );    //for test only
				finp >> word;
				if ( (word == L"CW:") || (word == L"КС:") ) {      //if (FindFirstHex(finp, "CW:", cmdWord))
					finp >> cmdWord;
					GetAttrCW(cmdWord, addr, subaddr, cntWord);
					getline(finp, line, L'\n' );
					finp >> word;
					if (word == L"wResult") {
						getline(finp, line, L'\n' );
						finp >> word;
					}
					if ( (word == L"DW:") || (word == L"ИС:") ) {
						for (int i = 0; i < cntWord; i++) {
							finp >> bufMIL[i];
						}
						if (subaddr == 14) OutputUSD(finfo, bufMIL[0]);
						else if (subaddr == 17) {
							struct SINCHRO *sinc = (struct SINCHRO *)bufMIL;
							Swap2Word((short*)&sinc->countClk);
							Swap2Word((short*)&sinc->curEpoch);
//							if (FormAnimateSetting->CheckBoxSinc->Checked) {
//								zeroDate.Val = EpochToDateTime(sinc->curEpoch).Val
//											 - sinc->countClk/86400.;
//							}
						}
					}
					else if ( (word == L"AW:") || (word == L"ОС:") ) {  //if (FindFirstHex(finp, "AW:", ansWord))
						finp >> ansWord;  //
						getline(finp, line, L'\n' );
						if (!(ansWord & 0x07FF) ) {
							finp >> word; //ИС:
							if ( (word == L"DW:") || (word == L"ИС:") ) {
								for (int i = 0; i < cntWord; i++) {
									finp >> bufMIL[i];
								}
								switch (subaddr) {
									case (2): {
										TShtmi1_2R *shtmi1 = (TShtmi1_2R *)bufMIL;
										SwapShtmi1_2R(shtmi1);
										if (!isSHTMI1) {
											fshtmi1.open((SaveDir + FileTitle + "_shtmi1.txt").c_str());
										}
										PrintLogShtmi1_2R(fshtmi1, shtmi1, curDate, isSHTMI1);
										break;
									}
									case (3): {
										TShtmi2_2R *shtmi2 = (TShtmi2_2R *)bufMIL;
										SwapShtmi2_2R(shtmi2);
										if (!isSHTMI2) {
											fshtmi2.open((SaveDir + FileTitle + "_shtmi2.txt").c_str());
										}
										curDate = GetDateTime_2R(zeroDate, shtmi2->time_sec,
																		shtmi2->time_ms);
										PrintLogShtmi2_2R(fshtmi2, shtmi2, curDate, isSHTMI2);
										break;
									}

									case (4):
									case (5): {
										int nAr = bufMIL[0];
										switch (nAr) {
											case 3: { memcpy(&bufDtmi.time_sec, &bufMIL[1], sizeof(short) * 31); break; }
											case 4: { memcpy(&bufDtmi.m_cur, &bufMIL[1], sizeof(short) * 31); break; }
											case 5: {
												memcpy(&bufDtmi.pixFilterCnt[0], &bufMIL[1], sizeof(short));
												memcpy(&bufDtmi.RsLocT[0][0].x, &bufMIL[2], sizeof(short) * 30);
												break;
											}
											case 6: {
												memcpy(&bufDtmi.pixFilterCnt[1], &bufMIL[1], sizeof(short));
												memcpy(&bufDtmi.RsLocT[0][4].y, &bufMIL[2], sizeof(short) * 30);
												break;
											}
											case 7: {
												memcpy(&bufDtmi.pixCurrent, &bufMIL[1], sizeof(short));
												memcpy(&bufDtmi.RsLocT[0][8].bright, &bufMIL[2], sizeof(short) * 30);
												break;
											}
											case 8: { memcpy(&bufDtmi.RsLocT[0][12].size, &bufMIL[1], sizeof(short) * 31); break; }
											case 9: { memcpy(&bufDtmi.RsLocT[1][2].y, &bufMIL[2], sizeof(short) * 30); break; }
											case 10: { memcpy(&bufDtmi.RsLocT[1][6].bright, &bufMIL[2], sizeof(short) * 30); break; }
											case 11: { memcpy(&bufDtmi.RsLocT[1][10].size, &bufMIL[2], sizeof(short) * 29); break; }
											case 12: { memcpy(&bufDtmi.XYc[0][0], &bufMIL[2], sizeof(short) * 30); break; }
											case 13: { memcpy(&bufDtmi.MeanFrag[0][0], &bufMIL[2], sizeof(short) * 30); break; }
											case 14: { memcpy(&bufDtmi.ThFrag[0], &bufMIL[2], sizeof(short) * 30);
												int n;
												CadrInfo mCadr[2];
												SwapDtmi_2R(&bufDtmi);
												if (!isDTMI) {
													fdtmi.open((SaveDir + FileTitle + "_dtmi.txt").c_str());
												}
												curDate = GetDateTime_2R(zeroDate, bufDtmi.time_sec,
																				   bufDtmi.time_ms);
												if (oldFormat_2R) {
													TDtmi_2R_old bufDtmi_old;
													memcpy(&bufDtmi_old, &bufDtmi, sizeof(bufDtmi_old));
													PrintDtmi_2R_old(finfo, bufDtmi_old, curDate, isDTMI);
													PrintLogDtmi_2R_old(fdtmi, bufDtmi_old, curDate, isDTMI);
													ConvertDtmi2R(bufDtmi, curDate, mCadr, n);

												}
												else {
													PrintDtmi_2R(finfo, bufDtmi, curDate, isDTMI);
													PrintLogDtmi_2R(fdtmi, bufDtmi, curDate, isDTMI);
													ConvertDtmi2R(bufDtmi, curDate, mCadr, n);
												}
												for (int i = 0; i < n; i++) {
													AddCadrInfo(mCadr[i], vCadrInfo);
												}
												break;
											}
											case 19: { 	memcpy(&bufMloc[0], &bufMIL[1], sizeof(short)*31); break; }
											case 20: { 	memcpy(&bufMloc[31], &bufMIL[2], sizeof(short)*30); break; }
											case 21: { 	memcpy(&bufMloc[61], &bufMIL[2], sizeof(short)*29); break; }
											case 22:
											case 23:
											case 24: {	memcpy(&bufMloc[(nAr-19)*30], &bufMIL[2], sizeof(short)*30); break; }
											case 25: {	memcpy(&bufMloc[(nAr-19)*30], &bufMIL[2], sizeof(short)*29); break; }
											case 26: { 	memcpy(&bufMloc[(nAr-19)*30-1], &bufMIL[2], sizeof(short)*30); break; }
											case 27: { 	memcpy(&bufMloc[(nAr-19)*30-1], &bufMIL[2], sizeof(short)*28);
												if (!isMLOC) {
													fmloc.open((SaveDir + FileTitle + "_mloc.txt").c_str());
												}
												TMloc_2R *mloc = (TMloc_2R *)bufMloc;
												SwapMloc_2R(mloc);
												curDate = GetDateTime_2R(zeroDate, mloc->time_sec,
																				mloc->time_ms);
												PrintMloc_2R(finfo, *mloc, curDate);
												PrintLogMloc_2R(fmloc, *mloc, curDate, isMLOC);
												CadrInfo mCadr;
												ConvertMloc2R(*mloc, curDate, mCadr);
												AddCadrInfo(mCadr, vCadrInfo);
												break;
											}
											default: break;
										}
										break;
									}

									case (16): {
										if (!isMSHI) {
											fmshi.open((SaveDir + FileTitle + "_mshi.txt").c_str());
										}
										CadrInfo mCadr;
										TMshi_2R *mshi = (TMshi_2R *)bufMIL;
										SwapMshi_2R(mshi);
										curDate = GetDateTime_2R(zeroDate, mshi->time_sec,
																		mshi->time_ms);
										if (oldFormat_2R) {
											TMshi_2R_old *mshi_old = (TMshi_2R_old *)bufMIL;
											PrintLogMshi_2R_old(fmshi, mshi_old, curDate, isMSHI);
											ConvertMshi2R_old(*mshi_old, curDate, mCadr);
										}
										else {
											PrintLogMshi_2R(fmshi, mshi, curDate, isMSHI);
											ConvertMshi2R(*mshi, curDate, mCadr);
										}

										int nBokz = mshi->serial >> 15;
										AnsiString strSerial = IntToStr(mshi->serial & 0x7FFF)
															+ "-" + IntToStr(mshi->serial >> 15);
										AnsiString title = "мБОКЗ-2Р № " + strSerial;
										DrawOrientInfo(mCadr, curDate, title, nBokz);
										break;
									}
									default: break;
								}
							}
						}
					}
				}
			}
			finp.close();
			finfo.close();
			fmshi.close();
			fshtmi1.close();
			fshtmi2.close();
			fdtmi.close();
			fmloc.close();
		}
//        delete []localBuffer;
		if (FormAnimateSetting->CheckBoxSaveIki->Checked) {
			SetCurrentDir(SaveDir);
			for (int i = 0; i < vCadrInfo.size(); i++) {
				writeProtocolToIKI(vCadrInfo[i], -1);
			}
		}

		int nRow = 1;
		for (int nDevice = 0; nDevice < 2; nDevice++) {
			if (StarTrack[nDevice].size()) {
				TableStatInfo->Cells[0][nRow++] = "мБОКЗ-2P №" + IntToStr(nDevice + 1);
				CalculateOrientStat(StarTrack[nDevice], nDevice, nRow);
				CalculateParamStat(StarTrack[nDevice], nDevice, nRow);
			}
		}

		vector<TAngVz> AngVz12;
		if (StarTrack[0].size() && StarTrack[1].size()) {
			CalcAngVz(StarTrack, AngVz12);
			if (FormAnimateSetting->CheckBoxDateTime->Checked)
				plotter->SetDateTimeX(true);
			else
				plotter->SetDateTimeX(false);

			for (int i = 0; i < AngVz12.size(); i++) {
				plotter->AddPoint(ChartAngXX, 0, AngVz12[i].Time, AngVz12[i].Ang[0] * RTD, clBlue);
				plotter->AddPoint(ChartAngYY, 0, AngVz12[i].Time, AngVz12[i].Ang[1] * RTD, clBlue);
				plotter->AddPoint(ChartAngZZ, 0, AngVz12[i].Time, AngVz12[i].Ang[2] * RTD, clBlue);
			}
			TableStatInfo->Cells[0][nRow++] = "мБОКЗ-2P №1 - мБОКЗ-2P №2";
			CalculateAngVzStat(AngVz12, 0, nRow);
		}

//		for (int i = 0; i < Charts.size(); i++) {
//			plotter->CheckChartSeries(Charts[i]);
//		}

		CheckTabSheet();
		PrepareStartDraw();
		SaveScreenShots(SaveDir.c_str());
		SaveTableToFile(TableStatInfo, TableStatInfo->RowCount, TableStatInfo->ColCount,
									AnsiString(SaveDir + FileTitle + "_stat.txt").c_str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::ChartAngGetAxisLabel(TChartAxis *Sender, TChartSeries *Series,
		  int ValueIndex, UnicodeString &LabelText)
{

	if ( (Sender == ChartAl->LeftAxis) || (Sender == ChartDl->LeftAxis)
		 || (Sender == ChartAz->LeftAxis) || (Sender == ChartAngXX->LeftAxis)
		 || (Sender == ChartAngYY->LeftAxis) || (Sender == ChartAngZZ->LeftAxis) )
	{
		int deg, min, sec;
		float flt = StrToFloat((AnsiString)LabelText);
		ToGMS(flt, deg, min, sec);

		if (sec) {
			LabelText = FloatToStr(deg) + "° " + FloatToStr(min) + "' " + FloatToStr(sec) + "''";
		}
		else if (min) LabelText = FloatToStr(deg) + "° " + FloatToStr(min) + "' " ;
		else LabelText = FloatToStr(deg) + "° ";
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::MenuDeviceClick(TObject *Sender)
{
	TFormDevice *FormDeviceNew = new TFormDevice (this);
	FormDeviceNew->Show();

}
//---------------------------------------------------------------------------



void __fastcall TFormGraphOrient::FormShow(TObject *Sender)
{
//	FormAnimateSetting->InitAnimateOptions();
	SetVisibleLabelFrame(FormAnimateSetting->CheckBoxLabelFrame->Checked);
	SetVisibleLabelFrame(FormAnimateSetting->CheckBoxLabelFrame->Checked);
	SetVisibleLabelStar(FormAnimateSetting->CheckBoxLabelStar->Checked);
	SetVisibleLabelObject(FormAnimateSetting->CheckBoxLabelObject->Checked);

	SetBrightSelect(FormAnimateSetting->CheckBoxBright->Checked,
					StrToInt(FormAnimateSetting->EditBrightMin->Text),
					StrToInt(FormAnimateSetting->EditBrightMax->Text));
	SetSquareSelect(FormAnimateSetting->CheckBoxSquare->Checked,
					StrToInt(FormAnimateSetting->EditSquareMin->Text),
					StrToInt(FormAnimateSetting->EditSquareMax->Text));
	SetCoordXSelect(FormAnimateSetting->CheckBoxCoordX->Checked,
					StrToInt(FormAnimateSetting->EditCoordXMin->Text),
					StrToInt(FormAnimateSetting->EditCoordXMax->Text));
	SetCoordYSelect(FormAnimateSetting->CheckBoxCoordY->Checked,
					StrToInt(FormAnimateSetting->EditCoordYMin->Text),
					StrToInt(FormAnimateSetting->EditCoordYMax->Text));
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::MenuOpenKondorClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Filter = "Текстовые файлы|*.dat; *.txt|Все файлы|*.*";
	OpenDialog->Options << ofAllowMultiSelect;
	int nArDtmi;
	if (OpenDialog->Execute()) {

		AnsiString SaveDir  = ExtractFileDir(OpenDialog->FileName) + "/Result/";
		CreateDir(SaveDir);

		AnsiString fileTitle;
		GetFileTitles(OpenDialog->Files->Strings[0],&fileTitle);
		fileTitle += "-"+IntToStr(OpenDialog->Files->Count);

		vector <unsigned short> bufMIL;
		bufMIL.clear();
		ofstream finfo((SaveDir + fileTitle + "_info.txt").c_str());
		for (int iFile = 0; iFile < OpenDialog->Files->Count; iFile++) {
			ifstream finp(AnsiString(OpenDialog->Files->Strings[iFile]).c_str());

			string line;
			char str[20];
			unsigned short buf;
			while (getline(finp, line)) {
				if (sscanf(line.c_str(), "%s 0x%x", str, &buf) == 2) {
					bufMIL.push_back(buf);
				}
			}
			finp.close();
		}


		const int countArDtmi = 14;
		const int sizeArDtmi = 29;
		unsigned short bufDtmi[sizeArDtmi * 14];
		for (int i = 0; i < bufMIL.size() - sizeArDtmi; i++) {
			if (bufMIL[i] == 0x42C3) {
				nArDtmi = (bufMIL[i+1] - 0x7AAA);
				nArDtmi =  (nArDtmi >> 12) + (nArDtmi & 0xF);
				if ( (nArDtmi >= 0) && (nArDtmi <= 13) ) {
					memcpy(&bufDtmi[nArDtmi * sizeArDtmi], &bufMIL[i+2], sizeof(short) * sizeArDtmi);
					i += sizeArDtmi;
					if (nArDtmi == 13) {
						DTMI_M60K dtmi;
						memcpy(&dtmi, bufDtmi, sizeof(bufDtmi));
						PrintDtmiM60K(finfo, dtmi);
						return;
					}
				}
			}
		}
        finfo.close();
		bufMIL.clear();
	}
}
//---------------------------------------------------------------------------

