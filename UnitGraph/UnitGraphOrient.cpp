#pragma hdrstop
#include "UnitGraphOrient.h"
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

vector <CadrInfo> mBokz[2];

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
		FontSize(7),
		statusTableInited(false)
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

	InitializeSynchronization();

}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::FormClose(TObject *Sender, TCloseAction &Action)
{
		vCadrInfo.clear();
		mBokz[0].clear();
        mBokz[1].clear();
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
							  || ChartTemp->SeriesCount());

	TabSheetStatFrag->TabVisible = (ChartFragErrX->SeriesCount() || ChartFragErrY->SeriesCount()
								 || ChartFragBright->SeriesCount() || ChartFragSizeEl->SeriesCount());

	TabSheetStatFrag2->TabVisible = (ChartFragMean->SeriesCount() || ChartFragNoise->SeriesCount()
								 || ChartFragLevel->SeriesCount());

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

		for (int i = 0; i < mCadr.SizeWindowsList; i++) {        //!!!
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
		TableObjectsInfo->RowCount = mCadr.SizeObjectsList+TableWindowsInfo->FixedRows;

		for (int i = 0; i<mCadr.SizeObjectsList; i++) {

			int k = 0;
			TableObjectsInfo->Cells[k++][i+1] = String(i+1);
			TableObjectsInfo->Cells[k++][i+1] = FloatToStrF(mCadr.ObjectsList[i].X, ffFixed,10,4);
			TableObjectsInfo->Cells[k++][i+1] = FloatToStrF(mCadr.ObjectsList[i].Y, ffFixed,10,4);
			TableObjectsInfo->Cells[k++][i+1] = FloatToStrF(mCadr.ObjectsList[i].Bright, ffFixed,10,2);
			TableObjectsInfo->Cells[k++][i+1] = String(mCadr.ObjectsList[i].Square);
			TableObjectsInfo->Cells[k++][i+1] = String(mCadr.ObjectsList[i].StarID);
			TableObjectsInfo->Cells[k++][i+1] = FloatToStrF(mCadr.ObjectsList[i].Mv, ffFixed,6,2);
			TableObjectsInfo->Cells[k++][i+1] = String(mCadr.ObjectsList[i].Sp[0])
												+String(mCadr.ObjectsList[i].Sp[1]);
			TableObjectsInfo->Cells[k++][i+1] = FloatToStrF(mCadr.ObjectsList[i].Dx*1000., ffFixed,8,2);
			TableObjectsInfo->Cells[k++][i+1] = FloatToStrF(mCadr.ObjectsList[i].Dy*1000., ffFixed,8,2);
		}
	}
	else
	{
		TableObjectsInfo->RowCount = TableObjectsInfo->FixedRows + 1;
		for (int k = 0; k < TableObjectsInfo->ColCount; k++) {
			TableObjectsInfo->Cells[k][1] = "-";
		}
	}
}

bool TFormGraphOrient::InitStatusInfoTable(const string& deviceName)
{
	StatusInfo.TableRows.clear();
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
		vector <string> splitted = split(line, "/");
		StatusInfo.InitModePos = atoi(splitted[0].c_str());
		StatusInfo.ResultModePos = atoi(splitted[1].c_str());
		getline(in, line);
		splitted = split(line, "\t");
		StatusInfo.ColumnTitles = split(splitted[1], ";");
		TableStatusInfo->ColCount  = StatusInfo.ColumnTitles.size() + 1;
		TableStatusInfo->Cells[0][0] = "Значение статуса";
		for(int i = 0; i < StatusInfo.ColumnTitles.size(); i++)
		{
			TableStatusInfo->Cells[i + 1][0] = toUString(StatusInfo.ColumnTitles[i]);
		}

		int currentRow = 1;
		while (getline(in, line))
		{
			splitted = split(line, "\t");
			unsigned short statusValue = strtol(splitted[1].c_str(), NULL, 16);
			TableStatusInfo->Cells[0][currentRow++] = toUString(splitted[0] + " " + splitted[1]);
			StatusInfo.TableRows.push_back(statusValue);
		}
	}
	else
	{
		  ShowMessage("Не удалось найти файл для построение таблиц по статусам " + toUString(fileName));
		  return false;
    }

	for(int i = 0; i < TableStatusInfo->RowCount; i++)
	{
		for(int j = 0; j < StatusInfo.ColumnTitles.size(); j++)
		{
			TableStatusInfo->Cells[j + 1][i + 1] = "0";
		}
	}
	return true;
}


void TFormGraphOrient::ClearStatusInfoTable()
{
	for(int i = 1; i < TableStatusInfo->RowCount; i++)
	{
		for(int j = 0; j < StatusInfo.ColumnTitles.size(); j++)
		{
			TableStatusInfo->Cells[j + 1][i] = "0";
		}
	}
}


void TFormGraphOrient::AddRowToStatusTable(const CadrInfo& cadr)
{
	  for (int i = 0; i < StatusInfo.TableRows.size(); i++)
	  {
			if (StatusInfo.TableRows[i] == cadr.StatOrient)
			{
					for(int j = 0; j < StatusInfo.ColumnTitles.size(); j++)
					{
						if(cadr.DeviceInfo.find(StatusInfo.ColumnTitles[j]) != string::npos)
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
	try
	{
       	ClearAnimate();

		DrawObjects(mCadr);
		DrawBlock(mCadr);

		DrawFragment(mCadr);

		PrintTableWindows(mCadr);
		PrintTableObjects(mCadr);
	}
	catch(exception &e)
	{
		LabelStatusReport->Caption = toUString(e.what());
	}



//	ApplyScale();
}

void TFormGraphOrient::DrawObjects(const struct CadrInfo &mCadr)
{
	double X0, Y0, X1, Y1, Dx, Dy, Ist, Nel;
	double zoomRedArrow  = StrToFloat(EditScale->Text)*1000.;
	double zoomBlueArrow, binCoef;

	if (mCadr.IsBinary) binCoef = 0.5;
	else binCoef = 1.;

	zoomBlueArrow = mCadr.SizePixel * zoomRedArrow / binCoef;
	UnicodeString uString;
	DateTimeToString(uString, "dd.mm.yyyy hh:mm:ss.zzz",(TDateTime) mCadr.Time);
	EditTimeDev->Text = uString;
	double meanDx = 0., meanDy = 0., meanDistX = 0., meanDistY = 0.;
	int countDetObject = 0;

	for (int iObject = 0; iObject < mCadr.SizeObjectsList; iObject++) {

		X0  = mCadr.ObjectsList[iObject].X;
		Y0  = mCadr.ObjectsList[iObject].Y;
		Nel = mCadr.ObjectsList[iObject].Square;
		Dx  = mCadr.ObjectsList[iObject].Dx;
		Dy  = mCadr.ObjectsList[iObject].Dy;

		float sizeBubble;
		if (Nel) sizeBubble = 3 * sqrtm(fabs(Nel)) + 0.5;
		else sizeBubble = 1.;

		AnsiString LabelObject = FloatToStrF(mCadr.ObjectsList[iObject].Bright,ffFixed,8,0) + " EMP\r\n"
							 + mCadr.ObjectsList[iObject].Square + " PIX";
		if (!mCadr.ObjectsList[iObject].StarID && !Dx && !Dy) {
			Series9->AddBubble(X0, Y0, sizeBubble, LabelObject, clBlue);
		}
		else
		{
			//локализованный объект
			Series9->AddBubble(X0, Y0, sizeBubble, LabelObject, clGreen);
			meanDx += Dx;
			meanDy += Dy;

			//остаточные рассогласования
			X1 = X0 - mCadr.ObjectsList[iObject].Dx * zoomRedArrow;
			Y1 = Y0 - mCadr.ObjectsList[iObject].Dy * zoomRedArrow;
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
			countDetObject++;
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
				Series7->AddXY((k + 1)+0.2 * j, mCadr.ObjectsList[i].Bright, "", CurColor);
				Series8->AddXY((k + 1)+0.2 * j, mCadr.ObjectsList[i].Square, "", CurColor);
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
		for (int i = 0; i < mCadr.SizeObjectsList; i++)     //!!!
		{
			Series7->AddXY(i + 1, mCadr.ObjectsList[i].Bright);
			Series8->AddXY(i + 1, mCadr.ObjectsList[i].Square);
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
			if (FormAnimateSetting->CheckBoxCurrentTime) {
				 SynchronizeCharts(CurCadr.Time);
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
			case PIX_0_MARKER:
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

			case PIX_1_MARKER:
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

			case HO_MARKER:
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

			case SL_MARKER:
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
;

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

			case SINGLE_REG_MARKER:
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

			case ALL_REG_MARKER:
			{
			  unsigned char Reg[COUNT_REG];
			  fread(Reg,sizeof(Reg),1,fflesh);
			  PrintReg(ftxt, Reg);
			  break;
			}

			case FRAG_MARKER:
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

	string cwSinc[4]   = {"3224", "3A24", "6224", "6224"}; //add for №4 M60
	string cwMSHI[4]   = {"3618", "3E18", "6618", "6E18"};
	string cwSHTMI1[4] = {"3440", "3C40", "6440", "6C40"};
	string cwSHTMI2[4] = {"3460", "3C60", "6460", "6C60"};
	string cwDTMI[4]   = {"3480", "3C80", "6480", "6C80"};
	string cwMLOC[4]   = {"34A0", "3CA0", "64A0", "6CA0"};
	string codeOC[4]   = {"3000", "3800", "6000", "6800"};

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
		AnsiString SaveDir  = CurDir + "\\Result\\";
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
			colorStat[iStat] = RGB((float)(MAX_STAT - iStat) / MAX_STAT * 255,
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
		int tpr = 0;
		for (int iFile = 0; iFile < OpenDialog->Files->Count; iFile++) {

			ifstream finp(AnsiString(OpenDialog->Files->Strings[iFile]).c_str());
			if (!finp.is_open()) {
				ShowMessage("Файл не может быть открыт!");
				return;
			}
			finp >> hex;

			while (getline(finp, line, '\n'))
			{
			   //	getline(finp, line, '\n');

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
									mloc.timeBOKZ = tpr++;
									mloc.MeanC = (mloc.LocalList[0][2] > 0 && mloc.LocalList[0][2] < 10000) ? mloc.LocalList[0][2] : 0;//mloc.LocalList[0][2];
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
								   //	OutputDTMI(fout, SaveDir, dtmi, zeroDate, 1);
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

		//PrepareStartDraw();
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

void __fastcall TFormGraphOrient::MenuOpenEnergyClick(TObject *Sender)
{
	const int DTMISize = 290;
	const int MSHISize = 22;
	const int SHTMI1Size = 32;
	const int SHTMI2Size = 32;
	unsigned short ArrayDTMI[DTMISize];
	unsigned short ArrayMSHI[MSHISize];
	unsigned short ArraySHTMI1[SHTMI1Size];
	unsigned short ArraySHTMI2[SHTMI2Size];

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
		AnsiString SaveDir  = CurDir + "\\Result\\";
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

		ofstream fout((SaveDir + FileTitle + "_decode.txt").c_str());
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
		if (isMSHI)   fmshi.close();
		if (isSHTMI1) fshtmi1.close();
		if (isSHTMI2) fshtmi2.close();
		if (isDTMI)   fdtmi.close();
		if (isTemp)   ftemp.close();
		if (isPower)  fpow.close();

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


void __fastcall TFormGraphOrient::MenuOpenSamspaceClick(TObject *Sender)
{
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
		AnsiString SaveDir  = CurDir + "\\Result\\";
		CreateDir(SaveDir);
		GetFileTitles(FileName,&FileTitle);

		ifstream finp(FileName.c_str());
		if (!finp.is_open()) {
			ShowMessage("Файл не может быть открыт!");
			return;
		}

		ofstream fout, fmshi, fshtmi1, fshtmi2;
		fout.open((SaveDir + FileTitle + "_decode.txt").c_str());

		bool isM1000 = false;
		bool isSHTMI1 = 0, isSHTMI2 = 0;
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

		string line;
		while (!finp.eof())
		{
			getline(finp, line, '\n' );
			if ( (line.find("ШТМИ1") != string::npos)
				   || (line.find("У3-05") != std::string::npos)
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

//		int nn = ChartNumLoc->Series[0]->Count();

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

	stTime.sprintf("%x %x %x", timeEmka.hour, timeEmka.min,   timeEmka.sec);
	sscanf(stTime.c_str(), "%ld %ld %ld", &hour, &min,   &sec);

	year += 2000;
	curTime = EncodeDate(year, month, day) + EncodeTime(hour, min, sec, 0);
}

void TFormGraphOrient::DrawMshi_2V(TMshi_2V *mshi, TDateTime curDate, int num)
{
	TColor arColor[4] = {clBlue, clGreen, clRed, clGray};
	if ( (num >= 0) && (num < 2) ) {
		int nSeries = num;
		plotter->SetDateTimeX(true);
		AnsiString title = "мБОКЗ №" + IntToStr(num+1);
		plotter->SetTitle(title);
		plotter->SetSeriesColor(arColor[num]);

		CadrInfo mCadr;
		double Qdbl[4], Mornt[3][3], ang[3] = {0, 0, 0}, angSun = 0;
		//если выполнение команды завершено и ориентация определена успешно
		if ( (mshi->status1 & 0x4000) && !(mshi->status2 & 0xFF00)) {

			for (int i = 0; i < 4; i++) {
				 Qdbl[i] = (double)mshi->Qornt[i];
			}
			if ( !CheckQuatNorm(Qdbl, 0.001) ) {
				QuatToMatrix(Qdbl, Mornt);
				MatrixToEkvAngles(Mornt, ang);
				plotter->AddPoint(ChartAl, nSeries, curDate.Val, ang[0] * RTD);
				plotter->AddPoint(ChartDl, nSeries, curDate.Val, ang[1] * RTD);
				plotter->AddPoint(ChartAz, nSeries, curDate.Val, ang[2] * RTD);

				double DJD, DJD1900, SunI[3], SunD[3];
				_DateTime stDate;
				DecodeDateTime(curDate, stDate.Year, stDate.Month,  stDate.Day,
							stDate.Hour, stDate.Min, stDate.Sec, stDate.mSec);

				DJD = DateTimeToDaysJ2000(&stDate)/36525;
				DJD1900=(DJD + 1.0) * 36525 + 2415020.0;
				SunVector(DJD1900, Mornt, SunI, SunD);
				angSun = acosm(SunD[2]);
				plotter->AddPoint(ChartSunAngle, nSeries, curDate.Val, angSun * RTD);

				mCadr.IsOrient = true;
			}
			else mCadr.IsOrient = false;
			plotter->AddPoint(ChartWx, nSeries, curDate.Val, mshi->W[0] * RTS);
			plotter->AddPoint(ChartWy, nSeries, curDate.Val, mshi->W[1] * RTS);
			plotter->AddPoint(ChartWz, nSeries, curDate.Val, mshi->W[2] * RTS);
			plotter->AddPoint(ChartMxy, nSeries, curDate.Val, (mshi->mxy)/1e+4 * 1000.);
			plotter->AddPoint(ChartNumDet,  nSeries, curDate.Val, mshi->NumDet);
		}
		plotter->AddPoint(ChartNumFrag, nSeries, curDate.Val, mshi->NumFrag);
		plotter->AddPoint(ChartNumLoc,  nSeries, curDate.Val, mshi->NumLoc);
		plotter->AddPoint(ChartLevel,    nSeries, curDate.Val, mshi->ThMax);
		plotter->AddPoint(ChartTemp, nSeries, curDate.Val, (mshi->Tcmv)/10.);

		mCadr.Time = curDate.Val;
		if (mCadr.IsOrient) {
			for (int i = 0; i < 3; i++) {
				mCadr.AnglesOrient[i] = ang[i];
				mCadr.OmegaOrient[i]  = mshi->W[i];
			}
			mCadr.CountDeterObj = mshi->NumDet;
			mCadr.MeanErrorXY = (mshi->mxy)/1e+4; //mm
			mCadr.SunAngle = angSun;
		}
		mCadr.CountStars = mshi->NumStar;
		mCadr.CountWindows  = mshi->NumFrag;
		mCadr.CountLocalObj = mshi->NumLoc;
		mCadr.MatrixTemp = mshi->Tcmv/10;
		mCadr.Level = mshi->ThMax;
		AddCadrInfo(mCadr, mBokz[num]);
	}
}

void __fastcall TFormGraphOrient::MenuOpenEMKAClick(TObject *Sender)
{
	unsigned short cwMSHI[2]   = {0x4E00, 0x5600};
	unsigned short cwSHTMI1[2] = {0x4C40, 0x5440};
	unsigned short cwSHTMI2[2] = {0x4C60, 0x5460};
//	unsigned short cwDTMI[2]   = {0x4C80, 0x5480};
//	unsigned short cwMLOC[2]   = {0x4CA0, 0x54A0};
	unsigned short codeOC[2]   = {0x4800, 0x5000};

	FILE *fbin;
	TTimeEmka timeEmka;
	TDateTime curDate;
	TBoardArray packet;

	ofstream fmshi[2], fshtmi1[2], fshtmi2[2], fdtmi[2];
	bool isMshi[2] = {false, false};
	bool isShtmi1[2] = {false, false};
	bool isShtmi2[2] = {false, false};
//	bool isDtmi[2] = {false, false};
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
		mBokz[0].clear(); mBokz[1].clear();
		fbin = fopen(FileName.c_str(),"rb");
		while (!feof(fbin)) {
			if (fread(&cbuf, sizeof(char), 1, fbin) && (cbuf == 0xE5) ) {
				if (fread(&cbuf, sizeof(char), 1, fbin) && (cbuf == 0xE5) ) {
					fread(&timeEmka, sizeof(TTimeEmka), 1, fbin);
					fread(&packet.cmdWord, sizeof(short), 1, fbin);
//чтение МШИОР
					int devIndex = FindValueInList(packet.cmdWord, cwMSHI, 2);
					if ( (devIndex >= 0) && (devIndex < 2) ) {
						fread(&packet.ansWord, sizeof(short), 1, fbin);
						if (packet.ansWord == codeOC[devIndex]) {
							fread(packet.bufMIL, sizeof(short), 34, fbin);
							packet.crcCalc = GetCRC16((unsigned char*)&packet, 70);
							SwapShort((short*)&packet.bufMIL[2], (short*)&packet.bufMIL[3]);
							for (int i = 5; i <= 23; i = i + 2) {
								SwapShort((short*)&packet.bufMIL[i], (short*)&packet.bufMIL[i + 1]);
							}
							TimeEmkaToDateTime(timeEmka, curDate);
							if (!isMshi[devIndex]) {
								fmshi[devIndex].open((SaveDir + FileTitle +
												"_mshi_"+IntToStr(devIndex)+".txt").c_str());
							}
							PrintMshi_2V(fmshi[devIndex], &packet, curDate, isMshi[devIndex]);
							if ( (packet.statTmi == 2) && (packet.crcCalc == packet.crcBoard) ) {
								TMshi_2V *mshi;
								mshi = (TMshi_2V*)&packet.bufMIL;
								DrawMshi_2V(mshi, curDate, devIndex);
//								DrawMshi_2V(mshi, curDate, 0);
//								DrawMshi_2V(mshi, curDate, 1);
							}
						}
					}
//чтение ШТМИ1
					devIndex = FindValueInList(packet.cmdWord, cwSHTMI1, 2);
					if ( (devIndex >= 0) && (devIndex < 2) ) {
						fread(&packet.ansWord, sizeof(short), 1, fbin);
						if (packet.ansWord == codeOC[devIndex]) {
							fread(packet.bufMIL, sizeof(short), 34, fbin);
							packet.crcCalc = GetCRC16((unsigned char*)&packet, 70);
							SwapShort((short*)&packet.bufMIL[1], (short*)&packet.bufMIL[2]);
							for (int i = 8; i <= 12; i = i + 2) {
								SwapShort((short*)&packet.bufMIL[i], (short*)&packet.bufMIL[i + 1]);
							}
							TimeEmkaToDateTime(timeEmka, curDate);
							if (!isShtmi1[devIndex]) {
								fshtmi1[devIndex].open((SaveDir + FileTitle +
												"_shtmi1_"+IntToStr(devIndex)+".txt").c_str());
							}
							PrintShtmi1_2V(fshtmi1[devIndex], &packet, curDate, isShtmi1[devIndex]);
						}
					}
//чтение ШТМИ2
					devIndex = FindValueInList(packet.cmdWord, cwSHTMI2, 2);
					if ( (devIndex >= 0) && (devIndex < 2) ) {
						fread(&packet.ansWord, sizeof(short), 1, fbin);
						if (packet.ansWord == codeOC[devIndex]) {
							fread(packet.bufMIL, sizeof(short), 34, fbin);
							packet.crcCalc = GetCRC16((unsigned char*)&packet, 70);
							SwapShort((short*)&packet.bufMIL[1], (short*)&packet.bufMIL[2]);
							SwapShort((short*)&packet.bufMIL[14], (short*)&packet.bufMIL[15]);
							TimeEmkaToDateTime(timeEmka, curDate);
							if (!isShtmi2[devIndex]) {
								fshtmi2[devIndex].open((SaveDir + FileTitle +
												"_shtmi2_"+IntToStr(devIndex)+".txt").c_str());
							}
							PrintShtmi2_2V(fshtmi2[devIndex], &packet, curDate, isShtmi2[devIndex]);
							if ( (packet.statTmi == 2) && (packet.crcCalc == packet.crcBoard) ) {
								TShtmi2_2V *pTmi = (TShtmi2_2V*)(packet.bufMIL);
								unsigned short stat = (pTmi->status2 & 0xFF00) >> 8;
								if (!devIndex)
									plotter->AddPoint(ChartStat, 0, curDate.Val, stat, clBlue);
								else plotter->AddPoint(ChartStat, 0, curDate.Val, stat, clGreen);
							}
						}
					}
//чтение ДТМИ
//					devIndex = FindValueInList(cmdWord, cwDTMI, 2);
//					if ( (devIndex >= 0) && (devIndex < 2) ) {
//						if (!isDtmi[devIndex]) {
//									fdtmi[devIndex].open((SaveDir + FileTitle +
//													"_dtmi_"+IntToStr(devIndex)+".txt").c_str());
//						}
//						PrintDtmi_2V(fdtmi[devIndex], &packet, curDate, isDtmi[devIndex]);
//                    }
				}
			}
		}
		fclose(fbin);

		for (int i = 0; i < 2; i++) {
			if (isMshi[i]) fmshi[i].close();
			if (isShtmi1[i]) fshtmi1[i].close();
			if (isShtmi2[i]) fshtmi2[i].close();
//			if (isDtmi[i]) fdtmi[i].close();
		}

		PrepareStartDraw();
		CheckTabSheet();

		int nRow = 1;
		for (int nDevice = 0; nDevice < 2; nDevice++) {
			if (mBokz[nDevice].size()) {
				TableStatInfo->Cells[0][nRow++] = "мБОКЗ №" + IntToStr(nDevice + 1);
				CalculateOrientStat(mBokz[nDevice], nDevice, nRow);
				CalculateParamStat(mBokz[nDevice], nDevice, nRow);
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
			InitStatusInfoTable("BM60");
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
				   //readBOKZ60MSHIOR (in, vCadrInfo, handle, startDate);
				   readBOKZ60Protocol(in, vCadrInfo, handle, startDate);

					ofstream out1("quatinfo.txt");
					for (int i = 0; i < vCadrInfo.size(); i++) {

						out1 << setprecision(10) << vCadrInfo[i].timePr <<  "\t"
						<< vCadrInfo[i].QuatOrient[0] << "\t"
						<< vCadrInfo[i].QuatOrient[1] << "\t"
						<< vCadrInfo[i].QuatOrient[2] << "\t"
						<< vCadrInfo[i].QuatOrient[3] << "\t"
						<< vCadrInfo[i].CountLocalObj << "\t"
						<< vCadrInfo[i].CountDeterObj << "\n";
						//<< vCadrInfo[i].MatrixOrient[2][0] << "\t"
						//<< vCadrInfo[i].MatrixOrient[2][1] << "\t"
						//<< vCadrInfo[i].MatrixOrient[2][2] << "\n";


				}
					 out1.close();
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
		div += StrToInt(TableStatusInfo->Cells[StatusInfo.InitModePos][i]);
	 }

	 if (div == 0)
		 return;
	 else
	 {
		StatusInfo.NoOneCount = div;
		StatusInfo.NoFourCount = StrToFloat(TableStatusInfo->Cells[StatusInfo.ResultModePos][1]);
	 	RatioEdit->Text = FloatToStr((StatusInfo.NoFourCount / StatusInfo.NoOneCount) * 100) ;
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
						ofstream out("quatinfo.txt");
			for (int i = 0; i < vCadrInfo.size(); i++) {
				out << setprecision(10) << vCadrInfo[i].timePr <<  "\t"
				<< vCadrInfo[i].QuatOrient[0] << "\t"
				<< vCadrInfo[i].QuatOrient[1] << "\t"
				<< vCadrInfo[i].QuatOrient[2] << "\t"
				<< vCadrInfo[i].QuatOrient[3] << "\t"
				<< vCadrInfo[i].CountLocalObj << "\t"
				<< vCadrInfo[i].CountDeterObj << "\t"
				<< vCadrInfo[i].AnglesOrient[0] << "\t"
				<< vCadrInfo[i].AnglesOrient[1] << "\t"
				<< vCadrInfo[i].AnglesOrient[2] << "\n";
			}
		}
		catch (exception &e) {
			ShowMessage(e.what());
		}
}

void TFormGraphOrient::StartPrintReport(IKI_img* reader)
{
	OpenWord(true);
	AddDoc();
	SetTextFormat(14,1,0,0,1);
	AddParagraph("ПРОТОКОЛ проверки прибора " + AnsiString(reader->CameraSettings.DataSource.c_str())+"\n");

	SetTextFormat(12,1,0,0,0);
	AddParagraph("Параметры орбиты:");

	int deg, min, sec;
	AnsiString str;

	SetTextFormat(12,0,0,0,0);
	ToGMS(reader->Georeferencing.InclinationOrbit, deg, min, sec);
	str.sprintf("%d\° %02d' %02d""", deg, min, sec);
	AddParagraph("Наклонение орбиты................. " + str);

	ToGMS(reader->Georeferencing.LongitudeAscendingNode, deg, min, sec);
	str.sprintf("%d\° %02d' %02d""", deg, min, sec);
	AddParagraph("Долгота восходящего узла....... " + str);

	ToGMS(reader->Georeferencing.ArgumentPerigee, deg, min, sec);
	str.sprintf("%d\° %02d' %02d""", deg, min, sec);
	AddParagraph("Аргумент перицентра............... " + str);

	AddParagraph("Радиус орбиты " + FloatToStr(reader->Georeferencing.LengthRadiusVector) +"км");
}

void TFormGraphOrient::PrintReportRes(vector <CadrInfo>& cadrInfo)
{
	int count = cadrInfo.size();
	AddParagraph("Количество кадров в обработке: " + IntToStr(count));

	SetTextFormat(11,0,1,0,2);
	AddParagraph("\nТаблица "+String(NumTab+1)+". Статистика параметров ориентации");
	SetTextFormat(12,1,0,0,1);
	AddTable(TableStatInfo->RowCount, TableStatInfo->ColCount);
	for (int iRow = 0; iRow < TableStatInfo->RowCount; iRow++) {
		for (int iCol = 0; iCol < TableStatInfo->ColCount; iCol++) {
				SetCell(iRow+1, iCol+1, TableStatInfo->Cells[iCol][iRow]);
		}
		SetTextBold(0);
	}
}

void TFormGraphOrient::CalculateLocalStat(vector <CadrInfo> &vLocal, int nSeries, int &countRow)
{
	Statistika statParam;
	TRegEx filter = TRegEx(UnicodeString(".*"), TRegExOptions(roNotEmpty));

	if (FormAnimateSetting->CheckBoxFilter->Checked)
	{
		filter = TRegEx(FormAnimateSetting->FilterEdit->Text, TRegExOptions(roNotEmpty));
	}

	if (nSeries < ChartFone->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.MeanBright; f = f || !contains(a.DeviceInfo, filter); return a.MeanBright;} } GetMeanBright;
		statParam = calculateStatParam(vLocal.begin(), vLocal.end(), 0.0, filter, GetMeanBright);
		AddRowToStatTable(countRow++, "Уровень фона (МЛОК), ЕМР", statParam, 8, 2);
		ChartFone->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 1)
									+ " до " + FloatToStrF(statParam.max, ffFixed, 8, 1) + ", МЛОК";
	}

	if (nSeries < ChartNoise->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.SigmaBright; f = f || !contains(a.DeviceInfo, filter); return a.SigmaBright;} } GetSigmaBright;
		statParam = calculateStatParam (vLocal.begin(), vLocal.end(), 0.0, filter, GetSigmaBright);
		AddRowToStatTable(countRow++, "СКО фона (МЛОК), ЕМР", statParam, 8, 2);
		ChartNoise->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 1)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 1) + ", МЛОК";
	}

	if (nSeries < ChartNumLoc->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.CountLocalObj; f = f || !contains(a.DeviceInfo, filter); return a.CountLocalObj;} } GetCountLocalObj;
		statParam = calculateStatParam (vLocal.begin(), vLocal.end(), 0.0, filter, GetCountLocalObj);
		AddRowToStatTable(countRow++, "Число объектов (МЛОК)", statParam, 8, 2);
		ChartNumLoc->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) + ", МЛОК";
	}
}

void TFormGraphOrient::CalculateOrientStat(vector <CadrInfo> &vOrient, int nSeries, int &countRow)
{
	Statistika statParam;
	TRegEx filter = TRegEx(UnicodeString(".*"), TRegExOptions(roNotEmpty));

	if (FormAnimateSetting->CheckBoxFilter->Checked)
	{
		filter = TRegEx(FormAnimateSetting->FilterEdit->Text, TRegExOptions(roNotEmpty));
	}

	if (nSeries < ChartAl->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AnglesOrient[0];} } GetAnglesOrientAl;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetAnglesOrientAl);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++, "Прямое восхождение, град (СКО - угл. сек)", statParam, 8, 4);
		ChartAl->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " '')";
	}

	if (nSeries < ChartDl->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AnglesOrient[1];} } GetAnglesOrientDl;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetAnglesOrientDl);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++,"Склонение, град (СКО - угл. сек)", statParam, 8, 4);
		ChartDl->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " '')";
	}

	if (nSeries < ChartAz->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AnglesOrient[2];} } GetAnglesOrientAz;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetAnglesOrientAz);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(countRow++, "Азимут, град (СКО - угл. сек)", statParam, 8, 4);
		ChartAz->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " '')";
	}

	if (nSeries < ChartWx->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaOrient[0];} } GetOmegaOrientF;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetOmegaOrientF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(countRow++, "Угловая скорость по оси OX, угл. сек/с", statParam, 8, 4);
		ChartWx->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c)";
	}

	if (nSeries < ChartWy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaOrient[1];} } GetOmegaOrientS;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetOmegaOrientS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(countRow++, "Угловая скорость по оси OY, угл. сек/с", statParam, 8, 4);
		ChartWy->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c)";
	}

	if (nSeries < ChartWz->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaOrient[2];} } GetOmegaOrientT;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetOmegaOrientT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(countRow++, "Угловая скорость по оси OZ, угл. сек/с", statParam, 8, 4);
		ChartWz->Series[nSeries]->Title += " (CКО = " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c)";
	}

	if (nSeries < ChartSunAngle->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.SunAngle;} } GetSunAngle;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetSunAngle);
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
	TRegEx filter = TRegEx(UnicodeString(".*"), TRegExOptions(roNotEmpty));

	if (FormAnimateSetting->CheckBoxFilter->Checked)
	{
		filter = TRegEx(FormAnimateSetting->FilterEdit->Text, TRegExOptions(roNotEmpty));
	}

	if (ChartLevel->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = false; f = f || !contains(a.DeviceInfo, filter); return a.Level;} } GetLevel;
		statParam = calculateStatParam(vOrient.begin(), vOrient.end(), 0.0, filter, GetLevel);
		AddRowToStatTable(countRow++, "Порог по фрагментам, ЕМР", statParam, 8, 1);
		ChartLevel->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									+ " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) + " ЕМР)";
	}

	if (nSeries < ChartNumFrag->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.CountWindows; f = f || !contains(a.DeviceInfo, filter); return a.CountWindows;} } GetCountWindows;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetCountWindows);
		AddRowToStatTable(countRow++, "Число фрагментов", statParam, 8, 2);
		ChartNumFrag->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) +")";
	}

	if (nSeries < ChartNumLoc->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = false; f = f || !contains(a.DeviceInfo, filter); return a.CountLocalObj;} } GetCountLocalObj;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetCountLocalObj);
		AddRowToStatTable(countRow++, "Число объектов", statParam, 8, 2);
		ChartNumLoc->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) +")";
	}

	if (nSeries < ChartNumDet->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.CountDeterObj;} } GetCountDeterObj;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetCountDeterObj);
		AddRowToStatTable(countRow++, "Число звезд", statParam, 8, 2);
		ChartNumDet->Series[nSeries]->Title += " (от " + FloatToStrF(statParam.min, ffFixed, 8, 0)
									 + " до " + FloatToStrF(statParam.max, ffFixed, 8, 0) +")";
	}

	if (nSeries < ChartMxy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.MeanErrorXY;} } GetMeanErrXY;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetMeanErrXY);
		statParam.mean *= 1000; 	statParam.sigma *= 1000;
		statParam.min *= 1000;		statParam.max *= 1000;
		AddRowToStatTable(countRow++, "Ост. рассогласования, мкм", statParam, 8, 2);
		ChartMxy->Series[nSeries]->Title += " (среднее = " + FloatToStrF(statParam.mean, ffFixed, 8, 1) +" мкм)";
	}

	if (nSeries < ChartTemp->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = false; f = f || !contains(a.DeviceInfo, filter); return a.MatrixTemp;} } GetMatrixTemp;
		statParam = calculateStatParam (vOrient.begin(), vOrient.end(), 0.0, filter, GetMatrixTemp);
		AddRowToStatTable(countRow++, "Температура, град.", statParam, 8, 2);
		ChartTemp->Series[nSeries]->Title += " (среднее = " + FloatToStrF(statParam.mean, ffFixed, 8, 1) +" град.)";
	}

}

void TFormGraphOrient::CalculateSeriesSKO()
{
	Statistika statParam;
	int numberRow = 1;
	TRegEx filter = TRegEx(UnicodeString(".*"), TRegExOptions(roNotEmpty));


	if (FormAnimateSetting->CheckBoxFilter->Checked)
	{
		filter = TRegEx(FormAnimateSetting->FilterEdit->Text, TRegExOptions(roNotEmpty));
	}

	if (ChartFone->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = false; f = f || !contains(a.DeviceInfo, filter); return a.MeanBright;} } GetMeanBright;
		statParam = calculateStatParam(vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetMeanBright);
		AddRowToStatTable(numberRow++, "Уровень фона, ЕМР", statParam, 8, 2);
		ChartFone->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartNoise->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = false; f = f || !contains(a.DeviceInfo, filter); return a.SigmaBright;} } GetSigmaBright;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetSigmaBright);
		AddRowToStatTable(numberRow++, "СКО фона, ЕМР", statParam, 8, 2);
		ChartNoise->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartNumFrag->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = false; f = f || !contains(a.DeviceInfo, filter); return a.CountWindows;} } GetCountWindows;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetCountWindows);
		AddRowToStatTable(numberRow++, "Число фрагментов", statParam, 8, 2);
		ChartNumFrag->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartNumLoc->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = false; f = f || !contains(a.DeviceInfo, filter); return a.CountLocalObj;} } GetCountLocalObj;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetCountLocalObj);
		AddRowToStatTable(numberRow++, "Число объектов", statParam, 8, 2);
		ChartNumLoc->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartNumDet->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = (a.CountDeterObj < 4); f = f || !contains(a.DeviceInfo, filter); return a.CountDeterObj;} } GetCountDeterObj;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetCountDeterObj);
		AddRowToStatTable(numberRow++, "Число звезд", statParam, 8, 2);
		ChartNumDet->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartAl->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AnglesOrient[0];} } GetAnglesOrientAl;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetAnglesOrientAl);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(numberRow++, "Прямое восхождение, град/угл. сек", statParam, 8, 4);
//		ChartAl->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartDl->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AnglesOrient[1];} } GetAnglesOrientDl;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetAnglesOrientDl);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(numberRow++,"Склонение, град/угл. сек", statParam, 8, 4);
//		ChartDl->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartAz->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AnglesOrient[2];} } GetAnglesOrientAz;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetAnglesOrientAz);
		statParam.mean *= RTD; 		statParam.sigma *= RTS;
		statParam.min *= RTD;		statParam.max *= RTD;
		AddRowToStatTable(numberRow++, "Азимут, град/угл. сек", statParam, 8, 4);
//		ChartAz->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartAlError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AnglesDiff[0];} } GetAnglesDiffF;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetAnglesDiffF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка угла Al, угл. сек", statParam, 8, 4);
		ChartAlError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartDlError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AnglesDiff[1];} } GetAnglesDiffS;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetAnglesDiffS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++,"Ошибка угла Dl, угл. сек", statParam, 8, 4);
		ChartDlError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartAzError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AnglesDiff[2];} } GetAnglesDiffT;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetAnglesDiffT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка угла Az, угл. сек", statParam, 8, 4);
		ChartAzError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartErrorOX->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AxesDiff[0];} } GetAxesDiffF;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetAxesDiffF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка по оси OX, угл. сек", statParam, 8, 4);
		ChartErrorOX->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartErrorOY->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AxesDiff[1];} } GetAxesDiffS;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetAxesDiffS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка по оси OY, угл. сек", statParam, 8, 4);
		ChartErrorOY->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartErrorOZ->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.AxesDiff[2];} } GetAxesDiffT;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetAxesDiffT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка по оси OZ, угл. сек", statParam, 8, 4);
		ChartErrorOZ->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartWx->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaOrient[0];} } GetOmegaOrientF;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetOmegaOrientF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Угловая скорость по оси OX, угл. сек/с", statParam, 8, 4);
		ChartWx->Series[1]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c";
	}

	if (ChartWy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaOrient[1];} } GetOmegaOrientS;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetOmegaOrientS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Угловая скорость по оси OY, угл. сек/с", statParam, 8, 4);
		ChartWy->Series[1]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c";
	}

	if (ChartWz->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaOrient[2];} } GetOmegaOrientT;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetOmegaOrientT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Угловая скорость по оси OZ, угл. сек/с", statParam, 8, 4);
		ChartWz->Series[1]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 2) + " ''/c";
	}

	if (ChartWxError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaDiff[0];} } GetOmegaDiffF;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetOmegaDiffF);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка Wx, угл. сек/с", statParam, 8, 4);
		ChartWxError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartWyError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaDiff[1];} } GetOmegaDiffS;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetOmegaDiffS);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка Wy, угл. сек/с", statParam, 8, 4);
		ChartWyError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartWzError->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaDiff[2];} } GetOmegaDiffT;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetOmegaDiffT);
		statParam.mean *= RTS; 		statParam.sigma *= RTS;
		statParam.min *= RTS;		statParam.max *= RTS;
		AddRowToStatTable(numberRow++, "Ошибка Wz, угл. сек/с", statParam, 8, 4);
		ChartWzError->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartMx->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.MeanErrorX;} } GetMeanErrX;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetMeanErrX);
		statParam.mean *= 1000; 	statParam.sigma *= 1000;
		statParam.min *= 1000;		statParam.max *= 1000;
		AddRowToStatTable(numberRow++, "mx, мкм", statParam, 8, 4);
		ChartMx->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartMy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.MeanErrorY;} } GetMeanErrY;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetMeanErrY);
		statParam.mean *= 1000; 	statParam.sigma *= 1000;
		statParam.min *= 1000;		statParam.max *= 1000;
		AddRowToStatTable(numberRow++, "my, мкм", statParam, 8, 4);
		ChartMy->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartMxy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.MeanErrorXY;} } GetMeanErrXY;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetMeanErrXY);
		statParam.mean *= 1000; 	statParam.sigma *= 1000;
		statParam.min *= 1000;		statParam.max *= 1000;
		AddRowToStatTable(numberRow++, "mxy, мкм", statParam, 8, 4);
		ChartMxy->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}
	TableStatInfo->RowCount = numberRow;

//	if (ChartWx->SeriesCount() > 1) {
//		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaModel[0];} } GetOmegaModelF;
//		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetOmegaModelF);
//		AddRowToStatTable(numberRow++, "Wx_model, угл. мин/с", statParam, 8, 4);
//				FloatToStrF(meanStd.first * RTM, ffFixed, 8, 4), FloatToStrF(meanStd.second * RTM, ffFixed, 8, 4));
//	}

//	if (ChartWy->SeriesCount() > 1) {
//		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaModel[1];} } GetOmegaModelS;
//		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetOmegaModelS);
//		AddRowToStatTable(numberRow++, "Wy_model, угл. мин/с", statParam, 8, 4);
//	}

//	if (ChartWz->SeriesCount() > 1) {
//		struct { double operator() (const CadrInfo& a, TRegEx& filter, bool& f) {f = !a.IsOrient; f = f || !contains(a.DeviceInfo, filter); return a.OmegaModel[2];} } GetOmegaModelT;
//		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, filter, GetOmegaModelT);
//		AddRowToStatTable(numberRow++, "Wz_model, угл. мин/с", statParam, 8, 4);
//	}
}

void TFormGraphOrient::PrepareChartFrag(TColor *_colorFrag, const int _maxDrawFrag)
{
	for (int iFrag  = 0; iFrag < _maxDrawFrag; iFrag++) {
		_colorFrag[iFrag] = RGB((float)(_maxDrawFrag - iFrag) / _maxDrawFrag * 255,
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
void __fastcall TFormGraphOrient::N21Click(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Filter = "txt|*.txt";
	if (OpenDialog->Execute()) {
		SetCurrentDir(ExtractFileDir(OpenDialog->FileName));
		FILE *fbin;
		unsigned short bufShort1, bufShort2;
		unsigned char  bufC1, bufC2;
		unsigned short bufDec;
		fbin = fopen("Telem_M2.bin", "wb");
		std::ifstream finp (AnsiString(OpenDialog->FileName).c_str());
		if (finp.is_open()) {
			for (int j = 0; j < 64; j++) {
				finp >> dec;
				finp>>bufDec>>" - ";

				finp >> uppercase >> hex;
				for (int i = 0; i < 8; i++) {
					finp >> bufShort2 >> bufShort1;
					bufC1 = bufShort1;
					bufC2 = bufShort2;
					fwrite(&bufC1, 1, 1, fbin);
					fwrite(&bufC2, 1, 1, fbin);
				}
			}
		}

		finp.close();
		fclose(fbin);

		fbin = fopen("Telem_M2.bin", "rb");
		unsigned short buf;
		DTMI_M2 dtmi;
		char *ptr;
		int addr;
		while(!feof(fbin)) {
			fread(&buf, 2, 1, fbin);
			if (buf == 0x42C3) {
				fread(&buf, 2, 1, fbin);
				switch (buf) {
					case 0x7AAA: {
						fread(&dtmi, 2, 29, fbin);
						break;
					}
					case 0x8AAA: {
						ptr  = (char*) (&dtmi.quatLast[3]);
						addr = (int)(ptr)+2;
						fread((int *)addr, 2, 29, fbin);
						break;
					}
					case 0x9AAA: {
						fread(&dtmi.LocalList1[0].bright, 2, 29, fbin);
						break;
					}
					case 0xAAAA: {
						ptr  = (char*) (&dtmi.LocalList1[4].x);
						addr = (int)(ptr)+2;
						fread((int *)addr, 2, 29, fbin);
						break;
					}
					case 0xBAAA: {
						fread(&dtmi.LocalList1[7].size, 2, 29, fbin);
						break;
					}
					case 0xCAAA: {
						ptr  = (char*) (&dtmi.LocalList1[11].y);
						addr = (int)(ptr)+2;
						fread((int *)addr, 2, 29, fbin);
						break;
					}
					case 0xDAAA: {
						fread(&dtmi.LocalList2[0].x, 2, 29, fbin);
						break;
					}
					case 0xEAAA: {
						ptr  = (char*) (&dtmi.LocalList2[3].bright);
						addr = (int)(ptr)+2;
						fread((int *)addr, 2, 29, fbin);
						break;
					}
					case 0xFAAA: {
						fread(&dtmi.LocalList2[7].y, 2, 29, fbin);
						break;
					}
					case 0xFAAB: {
						ptr  = (char*) (&dtmi.LocalList2[10].size);
						addr = (int)(ptr)+2;
						fread((int *)addr, 2, 29, fbin);
						break;
					}
					case 0xFAAC: {
						fread(&dtmi.LocalList2[14].bright, 2, 29, fbin);
						break;
					}
					case 0xFAAD: {
						ptr  = (char*) (&dtmi.ResultList[3].x);
						addr = (int)(ptr)+2;
						fread((int *)addr, 2, 29, fbin);
						break;
					}
					case 0xFAAE: {
						fread(&dtmi.ResultList[6].size, 2, 29, fbin);
						break;
					}
					case 0xFAAF: {
						ptr  = (char*) (&dtmi.ResultList[10].y);
						addr = (int)(ptr)+2;
						fread((int *)addr, 2, 29, fbin);
						break;
					}
					default: break;
				}
			}
		}
		fclose(fbin);

		std::ofstream fout ("Telem_M2.txt");
		PrintDTMI_M2(fout, dtmi);
		fout.close();
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

			ofstream out("quatinfo.txt");
			for (int i = 0; i < vCadrInfo.size(); i++) {
				out << setprecision(10) << vCadrInfo[i].timePr <<  "\t"
				<< vCadrInfo[i].QuatOrient[0] << "\t"
				<< vCadrInfo[i].QuatOrient[1] << "\t"
				<< vCadrInfo[i].QuatOrient[2] << "\t"
				<< vCadrInfo[i].QuatOrient[3] << "\t"
				<< vCadrInfo[i].CountLocalObj << "\t"
				<< vCadrInfo[i].CountDeterObj << "\t"
				<< vCadrInfo[i].AnglesOrient[0] << "\t"
				<< vCadrInfo[i].AnglesOrient[1] << "\t"
				<< vCadrInfo[i].AnglesOrient[2] << "\n";
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
						ofstream out("quatinfo.txt");
			for (int i = 0; i < vCadrInfo.size(); i++) {
				out << setprecision(10) << vCadrInfo[i].timePr <<  "\t"
				<< vCadrInfo[i].QuatOrient[0] << "\t"
				<< vCadrInfo[i].QuatOrient[1] << "\t"
				<< vCadrInfo[i].QuatOrient[2] << "\t"
				<< vCadrInfo[i].QuatOrient[3] << "\t"
				<< vCadrInfo[i].CountLocalObj << "\t"
				<< vCadrInfo[i].CountDeterObj << "\t"
				<< vCadrInfo[i].AnglesOrient[0] << "\t"
				<< vCadrInfo[i].AnglesOrient[1] << "\t"
				<< vCadrInfo[i].AnglesOrient[2] << "\n";
			}
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


			ofstream out("quatinfo.txt");
			for (int i = 0; i < vCadrInfo.size(); i++) {
				out << setprecision(10) << vCadrInfo[i].timePr <<  "\t"
				<< vCadrInfo[i].QuatOrient[0] << "\t"
				<< vCadrInfo[i].QuatOrient[1] << "\t"
				<< vCadrInfo[i].QuatOrient[2] << "\t"
				<< vCadrInfo[i].QuatOrient[3] << "\t"
				<< vCadrInfo[i].CountLocalObj << "\t"
				<< vCadrInfo[i].CountDeterObj << "\t"
				<< vCadrInfo[i].AnglesOrient[0] << "\t"
				<< vCadrInfo[i].AnglesOrient[1] << "\t"
				<< vCadrInfo[i].AnglesOrient[2] << "\n";
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


//void __fastcall TFormGraphOrient::BOKZM60MILParseClick(TObject *Sender)
//{
//   	try
//	{
//		OpenDialog->Options.Clear();
//		OpenDialog->Filter = "txt|*.txt";
//		OpenDialog->Options << ofAllowMultiSelect;
//		if (OpenDialog->Execute()) {
//			vCadrInfo.clear();
//			DeleteLineGraph();
//			unique_ptr <TStringList> FileList (new TStringList());
//			FileList->Assign(OpenDialog->Files);
//			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
//			for (int i = 0; i < FileList->Count; i++)
//			{
//				AnsiString FileName = FileList->Strings[i];
//				ifstream in(FileName.c_str());
//				if (!in.is_open())
//				{
//					ShowMessage("Не удалось открыть файл");
//					return;
//				}

//				Handle60 handler (this);
//				readBOKZM60Mil (in, vCadrInfo, handler);

//			}

//			PrepareStartDraw();
//			CheckTabSheet();
//			CalculateSeriesSKO();
//		}
//	}

//	catch (exception &e)
//	{
//		ShowMessage(e.what());
//	}
//}
//---------------------------------------------------------------------------




void TFormGraphOrient::handleIKIMainFolder(TStringList* FileList, TStringList* FoldersList, const TDateTime& dt,  double& deltaTime, bool handleAll, bool last)
{
	const int maxDrawFrag = 32;
	TColor colorFrag [maxDrawFrag];
	bool CompareIKIRes = false;

	unsigned short startFrom = StrToInt(FormAnimateSetting->BeginFromEdit->Text);
	UnicodeString filePrefix = FormAnimateSetting->EditFilePrefix->Text;
	bool skipFrame = FormAnimateSetting->SkipFrameCheckBox->Checked;
	int seriesID;

	PrepareChartFrag(colorFrag, maxDrawFrag);
	if (FormAnimateSetting->CheckBoxResultOnly->Checked)
	{
		seriesID = 0;
	}
	else
	{
		seriesID = 1;
	}

	bool test = false;

   //ofstream outt("C:/Users/Yumatov/Documents/GitHub/ReadFlash/Release/Win32/test_file.txt", std::ofstream::out | std::ofstream::app);

	TRegEx filter = TRegEx(UnicodeString(".*"), TRegExOptions(roNotEmpty));

	if (FormAnimateSetting->CheckBoxFolderFilter->Checked)
	{
		filter = TRegEx(FormAnimateSetting->FilterFolderEdit->Text, TRegExOptions(roNotEmpty));
	}

	for (int curFolder = 0; curFolder < FoldersList->Count; curFolder++)
	{
		if (!contains(toStdString(FoldersList->Strings[curFolder]) ,filter))
		{
			continue;
		}

		if (!handleAll)
		{
			statusTableInited = false;
			vCadrInfo.clear();
			plotter->ResetOptions();
			DeleteLineGraph();
			ClearStatusInfoTable();
		}
		for (int i = startFrom; i < FileList->Count; i++)
		{
			unique_ptr <IKI_img> reader(new IKI_img());
			if (!AnsiContainsStr(FileList->Strings[i], filePrefix))
			{
				if (reader->ReadFormat(toStdString(FileList->Strings[i]), false, skipFrame))
				{
					//outt << toStdString(FileList->Strings[i]) << "\n";
					//outt.flush();
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
								if (FormAnimateSetting->FillStatusTableCheckBox->Checked)
								{
									statusTableInited = InitStatusInfoTable(splitted[0]);
								}

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

				if (FormAnimateSetting->ThroughDateTimeCheckBox->Checked && vCadrInfo.size() > 1)
				{
					if (vCadrInfo.size() == 2)
					{
						deltaTime = vCadrInfo[vCadrInfo.size() - 1].Time - vCadrInfo[vCadrInfo.size() - 2].Time;
					}
					vCadrInfo[vCadrInfo.size() - 1].Time = vCadrInfo[vCadrInfo.size() - 2].Time + deltaTime;
				}

				if (FormAnimateSetting->CheckBoxFilter->Checked)
				{
					TRegEx filter = TRegEx(FormAnimateSetting->FilterEdit->Text, TRegExOptions(roNotEmpty));
					if (!contains(vCadrInfo.back().DeviceInfo, filter))
					{
						continue;
					}
				}
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

		//
		//			ofstream out1("quatinfo.txt");
		//			for (int i = 0; i < vCadrInfo.size(); i++) {
		//			 UnicodeString dtStr;
		//			   DateTimeToString(dtStr, UnicodeString("dd.mm.yyyy hh:nn:ss.zzz"), vCadrInfo[i].Time);
		//				out1 << setprecision(10) << i <<  "\t"
		//				<< vCadrInfo[i].QuatOrient[0] << "\t"
		//				<< vCadrInfo[i].QuatOrient[1] << "\t"
		//				<< vCadrInfo[i].QuatOrient[2] << "\t"
		//				<< vCadrInfo[i].QuatOrient[3] << "\t"
		//				<< vCadrInfo[i].CountLocalObj << "\t"
		//				<< vCadrInfo[i].CountDeterObj << "\t"
		//				<< vCadrInfo[i].MatrixOrient[2][0] << "\t"
		//				<< vCadrInfo[i].MatrixOrient[2][1] << "\t"
		//				<< vCadrInfo[i].MatrixOrient[2][2] << "\t"
		//				<< toStdString(dtStr) << "\n";
		//		}
		//		 out1.close();
		//outt << "finish FOLDER" << "\n";
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

		if ((handleAll && last)  || !handleAll)
		{
			//outt << "handle ALL" << "\n";
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
			out << toStdString(RatioEdit->Text) << "\t" << StatusInfo.NoOneCount << "\t" << StatusInfo.NoFourCount;


			UnicodeString dtStr;
			DateTimeToString(dtStr, UnicodeString("yyyy-MM-dd hh-mm-ss"), dt);
			//outt << "handle ALL2" << "\n";
			if (FormAnimateSetting->AccuracyReportCheckBox->Checked)
			{
				string filepath = toStdString(ExtractFilePath(Application->ExeName)) + "/accuracyReport "
						+ toStdString(dtStr) + ".txt";
				ofstream accuracyReport(filepath.c_str(), ios_base::app);
				accuracyReport << toStdString(FoldersList->Strings[curFolder]) << "\t";
				int errorStatusCount = 0;
				for (int j = 2; j < TableStatusInfo->RowCount; j++)
				{
					errorStatusCount += StrToInt(TableStatusInfo->Cells[StatusInfoHeaders::SLEZH][j]);
				}
				int successStatusCount = StrToInt(TableStatusInfo->Cells[StatusInfoHeaders::SLEZH][1]);
				accuracyReport << successStatusCount << "\t";
				accuracyReport << errorStatusCount << "\t";
				accuracyReport  << toStdString(TableStatInfo->Cells[StatInfoHeader::SKO][StatInfoFields::ErrorRa]) << "\t"
				<< toStdString(TableStatInfo->Cells[StatInfoHeader::Min][StatInfoFields::ErrorRa]) << "\t"
				<< toStdString(TableStatInfo->Cells[StatInfoHeader::Max][StatInfoFields::ErrorRa]) << "\t";
				accuracyReport << toStdString(TableStatInfo->Cells[StatInfoHeader::SKO][StatInfoFields::ErrorDecl]) << "\t"
				<< toStdString(TableStatInfo->Cells[StatInfoHeader::Min][StatInfoFields::ErrorDecl]) << "\t"
				<< toStdString(TableStatInfo->Cells[StatInfoHeader::Max][StatInfoFields::ErrorDecl]) << "\t";
				accuracyReport << toStdString(TableStatInfo->Cells[StatInfoHeader::SKO][StatInfoFields::ErrorAz]) << "\t"
				<< toStdString(TableStatInfo->Cells[StatInfoHeader::Min][StatInfoFields::ErrorAz]) << "\t"
				<< toStdString(TableStatInfo->Cells[StatInfoHeader::Max][StatInfoFields::ErrorAz]) << endl;
			}

			if (FormAnimateSetting->StatusReportCheckBox->Checked)
			{
				toStdString(TDateTime::CurrentDate().DateTimeString());
				string filepath = toStdString(ExtractFilePath(Application->ExeName)) + "/statusReport "
						+ toStdString(dtStr) + ".txt";
				ofstream statusReport(filepath.c_str(), ios_base::app);
				statusReport << toStdString(FoldersList->Strings[curFolder]) << "\t";
				int errorStatusCount = 0;
				for (int i = StatusInfoHeaders::NO1; i < StatusInfoHeaders::Count; i++)
				{
					int successCount = 0;
					for (int j = 1; j < 3; j++)
					{
						successCount += StrToInt(TableStatusInfo->Cells[i][j]);
					}
					statusReport <<  successCount << "\t";
				}

				for (int i = StatusInfoHeaders::NO1; i < StatusInfoHeaders::Count; i++)
				{
					int errorCount = 0;
					for (int j = 3; j < TableStatusInfo->RowCount; j++)
					{
						errorCount += StrToInt(TableStatusInfo->Cells[i][j]);
					}
					statusReport <<  errorCount << "\t";
				}
				statusReport << toStdString(TableStatInfo->Cells[StatInfoHeader::Mean][StatInfoFields::ObjectCount]) << "\t"
				<< toStdString(TableStatInfo->Cells[StatInfoHeader::Mean][StatInfoFields::StarCount]) << endl;
			}
			//outt << "handle ALL3" << "\n";
		}
		if (skipFrame)
		{
			LabelFrameReport->Caption = "Бинарные данные не были считаны";
		}
	}

}







void __fastcall TFormGraphOrient::ChooseIKIFilesThenFoldersClick(TObject *Sender)
{
	try
	{
		OpenDialog->Filter = "iki|*.iki";
		FileOpenDialog1->Options << fdoPickFolders << fdoAllowMultiSelect;
		OpenDialog->Options << ofAllowMultiSelect;
		OpenDialog->FileName  = GetCurrentDir();
		TDateTime currentDt = TDateTime::CurrentDateTime();
		double deltaTime = 0;
		vCadrInfo.clear();
		if (OpenDialog->Execute())
		{
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			FileList->Sort();
			if (FileOpenDialog1->Execute())
			{
				FileTitle = "IKI";
				unique_ptr <TStringList> FoldersList (new TStringList());
				FoldersList->Assign(FileOpenDialog1->Files);
				handleIKIMainFolder(FileList.get(), FoldersList.get(), currentDt, deltaTime);
			}
		}
		CheckTabSheet();
	}
	catch (exception &e)
	{
		ShowMessage(e.what());
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::ChooseIKIFoldersClick(TObject *Sender)
{
	try
	{
		FileOpenDialog1->Options << fdoPickFolders << fdoAllowMultiSelect;
		bool handleAll = false;
		bool last = false;
		if (FormAnimateSetting->HandleAllFoldersLikeOneSampleCheckBox->Checked)
		{
		   handleAll = true;
		}
		vCadrInfo.clear();
		if (FileOpenDialog1->Execute())
		{
			FileTitle = "IKI";
			unique_ptr <TStringList> FoldersList (new TStringList());
			FoldersList->Assign(FileOpenDialog1->Files);
			TDateTime currentDt = TDateTime::CurrentDateTime();
			double deltaTime = 0;
			for (int i = 0; i < FoldersList->Count; i++)
			{
				unique_ptr <TStringList> EntireFoldersList (new TStringList());
				TStringDynArray dirsArray = TDirectory::GetDirectories(FoldersList->Strings[i]);
				for (int j = 0; j <= dirsArray.High; j++)
				{
					EntireFoldersList->Add(dirsArray[j]);
				}
				unique_ptr <TStringList> FileList (new TStringList());
				TStringDynArray filesArray = TDirectory::GetFiles(FoldersList->Strings[i], "*.iki");
				for (int j = 0; j <= filesArray.High; j++)
				{
					FileList->Add(filesArray[j]);
				}
				if (FormAnimateSetting->HandleAllFoldersLikeOneSampleCheckBox->Checked
				&& i == FoldersList->Count - 1)
				{
				   last = true;
				}
				handleIKIMainFolder(FileList.get(), EntireFoldersList.get(),  currentDt, deltaTime, handleAll, last);

			}
			CheckTabSheet();

		}
	}
	catch (exception &e)
	{
		ShowMessage(e.what());
	}
}
//---------------------------------------------------------------------------





void __fastcall TFormGraphOrient::BOKZM60KondorParseClick(TObject *Sender)
{
	try
	{
		OpenDialog->Options.Clear();
		OpenDialog->Filter = "csv|*.csv";
		OpenDialog->Options << ofAllowMultiSelect;
		if (OpenDialog->Execute()) {
			vCadrInfo.clear();
			DeleteLineGraph();
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			InitStatusInfoTable("BM60");
			for (int i = 0; i < FileList->Count; i++)
			{
				AnsiString FileName = FileList->Strings[i];
				ifstream in(FileName.c_str());
				if (!in.is_open())
				{
					ShowMessage("Не удалось открыть файл");
					return;
				}
				Handle60 handle(this);
				readBOKZ60DORKondor(in, vCadrInfo, handle);

				ofstream out1("quatinfo.txt");
				for (int i = 0; i < vCadrInfo.size(); i++) {

					out1 << setprecision(10) << vCadrInfo[i].timePr <<  "\t"
					<< vCadrInfo[i].QuatOrient[0] << "\t"
					<< vCadrInfo[i].QuatOrient[1] << "\t"
					<< vCadrInfo[i].QuatOrient[2] << "\t"
					<< vCadrInfo[i].QuatOrient[3] << "\t"
					<< vCadrInfo[i].CountLocalObj << "\t"
					<< vCadrInfo[i].CountDeterObj << "\t"
					<< vCadrInfo[i].AnglesOrient[0] << "\t"
					<< vCadrInfo[i].AnglesOrient[1] << "\t"
					<< vCadrInfo[i].AnglesOrient[2] << "\n";
				}
				out1.close();
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
//---------------------------------------------------------------------------




void __fastcall TFormGraphOrient::N3Click(TObject *Sender)
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
			InitStatusInfoTable("2R");
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
				   	try
	{
					HandleM2V handle(this);
					readAISProtocol(in, vCadrInfo, handle, startDate);
                 	}

	catch (exception &e)
	{
		ShowMessage(e.what());
	}
				ofstream out1("quatinfo.txt");
				for (int i = 0; i < vCadrInfo.size(); i++) {

					out1 << setprecision(10) << vCadrInfo[i].timePr <<  "\t"
					<< vCadrInfo[i].QuatOrient[0] << "\t"
					<< vCadrInfo[i].QuatOrient[1] << "\t"
					<< vCadrInfo[i].QuatOrient[2] << "\t"
					<< vCadrInfo[i].QuatOrient[3] << "\t"
					<< vCadrInfo[i].CountLocalObj << "\t"
					<< vCadrInfo[i].CountDeterObj << "\t"
					<< vCadrInfo[i].AnglesOrient[0] << "\t"
					<< vCadrInfo[i].AnglesOrient[1] << "\t"
					<< vCadrInfo[i].AnglesOrient[2] << "\n";
				}
				out1.close();
			}

			FillStatusTable();
			PrepareStartDraw();
			CheckTabSheet();
			CalculateSeriesSKO();
		}

}
//---------------------------------------------------------------------------

