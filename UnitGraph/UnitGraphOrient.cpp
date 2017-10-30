#pragma hdrstop
#include "UnitGraphOrient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VCLTee.TeeTools"
#pragma link "VCLTee.TeeEdit"
#pragma link "VCLTee.ErrorBar"
#pragma link "VCLTee.TeeHighLowLine"

#pragma resource "*.dfm"

extern int NumDoc, NumPar, NumTab, NumImage;

using namespace parse_prot;

void SwapShort(short *word1, short *word2)
{
	short buf;
	buf = *word1;
	*word1 = *word2;
	*word2 = buf;
}

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
		Charts.push_back(ChartFone); Charts.push_back(ChartNoise); Charts.push_back(ChartTemp);
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
		}

	SourceDir = GetCurrentDir();
	FormAnimateSetting->ReadINI(SourceDir+"\\options.ini");
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
	UnicodeString ScreenFolderName = "\\" + FormatDateTime("yyyy-mm-dd hh.mm.ss", Now()) + " " + "Cкриншоты\\";
	TDirectory::CreateDirectoryW(GetCurrentDir() + ScreenFolderName);
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

		Title = GetCurrentDir() + ScreenFolderName + Title;

		if (Charts[i]->SeriesCount()) {
			plotter->SaveChart(Charts[i], Title, 500, 1100);
		}
	}
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

void TFormGraphOrient::InitStatusInfoTable(const string& deviceName)
{
	tableRows.clear();
	string fileName = toStdString(ExtractFileDir(Application->ExeName)) + "\\" + deviceName + ".txt";
	ifstream in (fileName.c_str());
	if (in.is_open())
	{
		TableStatusInfo->ColCount  = 2;
		TableStatusInfo->FixedCols = 1;
		TableStatusInfo->FixedRows = 1;
		TableStatusInfo->ColWidths[0] = 200;
		string line;
		getline(in, line);
		TableStatusInfo->RowCount  = atoi(line.c_str());
		getline(in, line);
		vector <string> splitted = split(line, "\t");
		NOAttribute = splitted.back();

		const short NOTOSLEZHStatus = 2;
		int currentRow = 1;
		while (getline(in, line))
		{
			splitted = split(line, "\t");
			short statusType = atoi(splitted.back().c_str());
			unsigned short statusValue = strtol(splitted[1].c_str(), NULL, 16);
			if (statusType == NOTOSLEZHStatus)
			{
				TableStatusInfo->Cells[0][currentRow++] = toUString("НО " + splitted[0]);
				TableStatusInfo->Cells[0][currentRow++] = toUString("ТО " + splitted[0]);
			}
			else
			{
				TableStatusInfo->Cells[0][currentRow++] = toUString(splitted[0]);
			}
			for(int i = 0; i < statusType; i++)
			{
				tableRows.push_back(std::make_pair(statusValue, statusType));
			}
		}
		TableStatusInfo->Cells[0][TableStatusInfo->RowCount - 1] = "Число запросов НО";
	}
	else throw runtime_error("Не удалось найти файл " + fileName);

	for(int i = 1; i < TableStatusInfo->RowCount; i++)
	{
		TableStatusInfo->Cells[1][i] = "0";
	}
}


void TFormGraphOrient::ClearStatusInfoTable()
{
	for(int i = 1; i < TableStatusInfo->RowCount; i++)
	{
		TableStatusInfo->Cells[1][i] = "0";
	}
}


void TFormGraphOrient::AddRowToStatusTable(const CadrInfo& cadr)
{

	  for (int i = 0; i < tableRows.size(); i++)
	  {
		int k = tableRows[i].first;
			if (tableRows[i].first == cadr.StatOrient)
			{
				// если статус для НО и СЛЕЖ
				if (tableRows[i].second == 2 
				&& cadr.SizeWindowsList == 0)
				{
					if (cadr.SizeWindowsList == 0)
					{
					   TableStatusInfo->Cells[1][i + 1] =  StrToInt(TableStatusInfo->Cells[1][i + 1]) + 1;	
					   break;
					}
					else
					{
					   TableStatusInfo->Cells[1][i + 2] =  StrToInt(TableStatusInfo->Cells[1][i + 2]) + 1;
					   break;	
					}
				}
				else
				{
					TableStatusInfo->Cells[1][i + 1] =  StrToInt(TableStatusInfo->Cells[1][i  + 1]) + 1;
					break;
				}
			}
	  }

	  if (cadr.DeviceInfo.find(NOAttribute) != string::npos)
	  {
		  TableStatusInfo->Cells[1][TableStatusInfo->RowCount] =  StrToInt(TableStatusInfo->Cells[1][TableStatusInfo->RowCount]) + 1;
	  }
}

void TFormGraphOrient::InitTableStat()
{
	int k = 0;

	TableStatInfo->RowCount  = 25;
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
//	for (int i = 0; i < MaxBlockSeries; i++)
//		BlockSeries[i]->Visible = false;
//	for (int i = 0; i < MaxFrameSeries; i++)
//		FrameSeries[i]->Visible = false;

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
	double X0, Y0, X1, Y1, Dx, Dy, Ist, Nel;
	double zoomRedArrow  = StrToFloat(EditScale->Text)*1000.;
	double zoomBlueArrow, binCoef;

	if (mCadr.IsBinary) binCoef = 0.5;
	else binCoef = 1.;

	zoomBlueArrow = mCadr.SizePixel * zoomRedArrow / binCoef;
	EditTimeDev->Text = FloatToStrF(mCadr.Time,ffFixed, 10, 3);

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

		if (!mCadr.ObjectsList[iObject].StarID && !Dx && !Dy) {
			Series9->AddBubble(X0, Y0, sizeBubble, "", clBlue);
		}
		else
		{
			//локализованный объект
			Series9->AddBubble(X0, Y0, sizeBubble, "", clGreen);
			meanDx += Dx;
			meanDy += Dy;

			//остаточные рассогласования
			X1 = X0 - mCadr.ObjectsList[iObject].Dx * zoomRedArrow;
			Y1 = Y0 - mCadr.ObjectsList[iObject].Dy * zoomRedArrow;
			Series3->AddArrow(X0, Y0, X1, Y1, "", clRed);

			//ошибка локализации
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
//		int saveWidth = Series3->ArrowWidth = 3;
//		Series3->ArrowWidth = 3;
		Series3->AddArrow(Xc, Yc, Xc - meanDx * zoomRedArrow,
								  Yc - meanDy * zoomRedArrow, "", clRed);
//		Series3->ArrowWidth = saveWidth;

//		saveWidth = Series10->ArrowWidth;
//		Series10->ArrowWidth = 3;
		Series10->AddArrow(Xc, Yc, Xc - meanDistX * zoomBlueArrow,
								   Yc - meanDistY * zoomBlueArrow, "", clBlue);
//		Series10->ArrowWidth = saveWidth;
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

	for (int i = 0; i < mCadr.SizeStarsList; i++){

//	  LabelStar="Mv="+FloatToStrF(mCadr.StarsList[i].Mv,ffFixed,8,2);
		Series1->AddXY(mCadr.StarsList[i].X * binCoef,
					   mCadr.StarsList[i].Y * binCoef);
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

			  NumPixH=mDataPixHdr.NumPix;
			  if (mDataPixHdr.NumPix%2) NumPixH++;

			  for (int i=0; i<NumPixH; i=i+2)
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

			  ConvertDataNO(mDataNO, mCadrInfo,1);

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
						plotter->AddPoint(ChartWx, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[0] * RTM);
						plotter->AddPoint(ChartWy, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[1] * RTM);
						plotter->AddPoint(ChartWz, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[2] * RTM);


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
					plotter->AddPoint(ChartWx, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[0] * RTM);
					plotter->AddPoint(ChartWy, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[1] * RTM);
					plotter->AddPoint(ChartWz, 0, mCadrInfo.Time, mCadrInfo.OmegaOrient[2] * RTM);

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
			  fread(&mDataFragHdr,sizeof(struct DataFragHeader),1,fflesh);
	//          NumPixF*=2; // Игорь исправит!!!
			  ChangeWordFrag(mDataFragHdr);
			  if (mDataFragHdr.NumPix%2) mDataFragHdr.NumPix--;
			  if (mDataFragHdr.NumPix>MaxPix) mDataFragHdr.NumPix=MaxPix;
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




void __fastcall TFormGraphOrient::MenuOpenEnergyTMIClick(TObject *Sender)
{
	struct DTMI_BOKZM tmi;
	const DTMISize = 290;
	unsigned short ArrayDTMI[DTMISize];

    OpenDialog->Options.Clear();
	OpenDialog->Filter = "txt|*.txt";
	if (OpenDialog->Execute()) {

		vCadrInfo.clear();
		DeleteLineGraph();

		AnsiString FileName = OpenDialog->FileName;
		SetCurrentDir(ExtractFileDir(FileName));
		GetFileTitles(FileName,&FileTitle);

		ifstream finp(FileName.c_str());
		if (!finp.is_open()) {
			ShowMessage("Файл не может быть открыт!");
			return;
		}

		ofstream fout((FileTitle + "_decode.txt").c_str());

		string line, word1, word2, word3;
		unsigned short hex_val, dec_val;
		int cntRecDTMI = 0, ind;
		while (!finp.eof())
		{
			getline(finp, line, '\n' );
			if (line.find("ТМОС") != string::npos) {
				struct CadrInfo mCadr;

				for (int i = 0; i < DTMISize; i++) {
					finp >> word1 >> word2 >> dec_val;
					sscanf(word1.c_str(),"[%d]", &ind);
					sscanf(word2.c_str(),"0X%x", &hex_val);
					if (i == ind && hex_val == dec_val) {
						ArrayDTMI[i] = hex_val;
					}
				}
				memcpy(&tmi.timeBOKZ, &ArrayDTMI[2], 30 * sizeof(short));
				memcpy(&tmi.LocalList[2][0], &ArrayDTMI[36], 28 * sizeof(short));
				memcpy(&tmi.LocalList[5][2], &ArrayDTMI[68], 28 * sizeof(short));
				memcpy(&tmi.LocalList[9][0], &ArrayDTMI[100],28 * sizeof(short));
				memcpy(&tmi.LocalList[12][2], &ArrayDTMI[132],28 * sizeof(short));
				memcpy(&tmi.LocalList[16][0], &ArrayDTMI[164],28 * sizeof(short));
				memcpy(&tmi.LocalList[19][2], &ArrayDTMI[196],28 * sizeof(short));
				memcpy(&tmi.LocalList[23][0], &ArrayDTMI[228],28 * sizeof(short));
				memcpy(&tmi.LocalList[26][2], &ArrayDTMI[260],28 * sizeof(short));
				fout << "\n" << line << "\n";

				PrintDTMI_BOKZM(fout, tmi);
				ConvertDataDTMI_BOKZM(tmi, mCadr);
				vCadrInfo.push_back(mCadr);
			}
		}
		finp.close();
		fout.close();

		PrepareStartDraw();
		CheckTabSheet();
	}
}


 //---------------------------------------------------------------------------


void __fastcall TFormGraphOrient::MenuOpenProgressTMIClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Filter = "txt|*.txt";
	if (OpenDialog->Execute()) {

		vCadrInfo.clear();
		DeleteLineGraph();

		AnsiString FileName = OpenDialog->FileName;
		SetCurrentDir(ExtractFileDir(FileName));
		GetFileTitles(FileName, &FileTitle);

		ifstream finp(FileName.c_str());
		if (!finp.is_open()) {
			ShowMessage("Файл не может быть открыт!");
			return;
		}

		ofstream fout((FileTitle + "_decode.txt").c_str());
		ofstream fshtmi1((FileTitle + "_shtmi1.txt").c_str());
		ofstream fshtmi2((FileTitle + "_shtmi2.txt").c_str());
		ofstream fdtmi((FileTitle + "_dtmi.txt").c_str());
		ofstream fmloc((FileTitle + "_mloc.txt").c_str());

		fshtmi1<<"Day/Time\t";
		fshtmi1<<"KC1\t"<<"KC2\t"<<"POST\t"<<"№\t"<<"Texp\t";
		fshtmi1<<"Foc\t"<<"Xg\t"<<"Yg\t";
		fshtmi1<<"Mean\t"<<"Sigma\t";
		fshtmi1<<"Ndef\t"<<"CRC\t";
		fshtmi1<<"Date\t"<<"Version\t";
		fshtmi1<<"\n";

		fshtmi2<<"Day/Time\t";
		fshtmi2<<"KC1\t"<<"KC2\t"<<"POST\t"<<"№\t"<<"Texp\t";
		fshtmi2<<"УСД\t"<<"НО\t"<<"НОСЛ\t";
		fshtmi2<<"TО\t"<<"TОСЛ\t"<<"СЛЕЖ\t";
		for (int i = 0; i < MAX_STAT; i++) {
			fshtmi2<<"EC"<<(i+1)<<"\t";
		}
		fshtmi2<<"\n";

		fdtmi<<"Day/Time\t";
		fdtmi<<"KC1\t"<<"KC2\t"<<"№\t"<<"Texp\t";
		fdtmi<<"NumLoc\t"<<"NumDet\t"<<"NumFrag\t";
		fdtmi<<"Eps\t"<<"DeltaT\t";
		fdtmi<<"TimeLastQ\t";
		fdtmi<<"\n";

		fmloc<<"Day/Time\t";
		fmloc<<"KC1\t"<<"KC2\t"<<"№\t"<<"Texp\t";
		fmloc<<"NumLoc\t"<<"NumFixed\t";
		fmloc<<"Mean\t"<<"Sigma\t";
		fmloc<<"\n";

		string line;
		int cntRecDTMI = 0;

		SHTMI1 mSHTMI1;
		SHTMI2 mSHTMI2;
		DTMI   mDTMI;
		LOC    mLOC;

		while (!finp.eof())
		{
			getline(finp, line, '\n' );
			if (line.find("ШТМИ1")!=string::npos) {
				if(TryReadSHTMI1(finp,mSHTMI1)) {
					PrintSHTMI1(fout,mSHTMI1);
					fshtmi1<<mSHTMI1.timeBOKZ<<"\t";
					fshtmi1<<uppercase<<hex<<setfill('0');
					fshtmi1<<"0x"<<setw(4)<<mSHTMI1.status1<<"\t";
					fshtmi1<<"0x"<<setw(4)<<mSHTMI1.status2<<"\t";
					fshtmi1<<"0x"<<setw(4)<<mSHTMI1.post<<"\t";
					fshtmi1<<dec<<setfill(' ');
					fshtmi1<<mSHTMI1.serialNumber<<"\t";
					fshtmi1<<mSHTMI1.timeExp<<"\t";
					fshtmi1<<mSHTMI1.Foc<<"\t";
					fshtmi1<<mSHTMI1.Xg<<"\t";
					fshtmi1<<mSHTMI1.Yg<<"\t";
					fshtmi1<<mSHTMI1.Mean<<"\t";
					fshtmi1<<mSHTMI1.Sigma<<"\t";
					fshtmi1<<mSHTMI1.countDefect<<"\t";
					fshtmi1<<mSHTMI1.CRC<<"\t";
					fshtmi1<<mSHTMI1.Date<<"\t";
					fshtmi1<<mSHTMI1.Version<<"\t";
					fshtmi1<<"\n";
				}
			}
			else if (line.find("ШТМИ2") != string::npos) {
				if(TryReadSHTMI2(finp,mSHTMI2)) {
					PrintSHTMI2(fout,mSHTMI2);
					fshtmi2<<mSHTMI2.timeBOKZ<<"\t";
					fshtmi2<<uppercase<<hex<<setfill('0');
					fshtmi2<<"0x"<<setw(4)<<mSHTMI2.status1<<"\t";
					fshtmi2<<"0x"<<setw(4)<<mSHTMI2.status2<<"\t";
					fshtmi2<<"0x"<<setw(4)<<mSHTMI2.post<<"\t";
					fshtmi2<<dec<<setfill(' ');
					fshtmi2<<setw(6)<<mSHTMI2.serialNumber<<"\t";
					fshtmi2<<setw(6)<<mSHTMI2.timeExp<<"\t";
					fshtmi2<<setw(6)<<mSHTMI2.cntCommandWord<<"\t";
					fshtmi2<<setw(6)<<mSHTMI2.cntCallNO<<"\t";
					fshtmi2<<setw(6)<<mSHTMI2.cntNOtoSLEZH<<"\t";
					fshtmi2<<setw(6)<<mSHTMI2.cntCallTO<<"\t";
					fshtmi2<<setw(6)<<mSHTMI2.cntTOtoSLEZH<<"\t";
					fshtmi2<<setw(8)<<mSHTMI2.cntSLEZH<<"\t";
					for (int i = 0; i < MAX_STAT; i++) {
						fshtmi2<<mSHTMI2.cntStatOrient[i]<<"\t";
					}
					fshtmi2<<"\n";
				}
			}
			else if (line.find("ДТМИ1")!=string::npos) {

				if (TryReadDTMI(finp, mDTMI)) {
					struct CadrInfo mCadr;

					if (mDTMI.status2 == 0x0005) {
						mDTMI.test_short=0;
						SwapShort((short*)&mDTMI.nWindows, (short*)&mDTMI.epsillon);
						for (int i = 0; i < MAX_WINDOW; i=i+2) {

							SwapShort((short*)&mDTMI.levelWindow[i],
									  (short*)&mDTMI.levelWindow[i+1]);
							SwapShort((short*)&mDTMI.nObjectWindow[i],
									  (short*)&mDTMI.nObjectWindow[i+1]);
						}
						SwapShort((short*)&mDTMI.nLocal[0], (short*)&mDTMI.nLocal[1]);
						SwapShort((short*)&mDTMI.maxHist, (short*)&mDTMI.maxHistX);
						SwapShort((short*)&mDTMI.maxHistY, (short*)&mDTMI.test_short);

						memcpy(&mLOC, &mDTMI, sizeof(mDTMI));
						PrintLOC(fout,mLOC);
						PrintLocalMLOC(mLOC);

						fmloc<<mLOC.timeBOKZ<<"\t";
						fmloc<<uppercase<<hex<<setfill('0');
						fmloc<<"0x"<<setw(4)<<mLOC.status1<<"\t";
						fmloc<<"0x"<<setw(4)<<mLOC.status2<<"\t";
						fmloc<<dec<<setfill(' ');
						fmloc<<setw(6)<<mLOC.serialNumber<<"\t";
						fmloc<<setw(6)<<mLOC.timeExp<<"\t";
						fmloc<<setw(6)<<mLOC.nLocalObj<<"\t";
						fmloc<<setw(6)<<mLOC.nFixedObj<<"\t";
						fmloc<<setw(6)<<mLOC.MeanC<<"\t";
						fmloc<<setw(6)<<mLOC.SigmaC<<"\t";
						fmloc<<"\n";

						ConvertDataLOC(mLOC, mCadr);
						mCadr.Time = cntRecDTMI++;
					}
					else {
						PrintDTMI(fout,mDTMI);
						PrintLocalDTMI(mDTMI);

						fdtmi<<mDTMI.timeBOKZ<<"\t";
						fdtmi<<uppercase<<hex<<setfill('0');
						fdtmi<<"0x"<<setw(4)<<mDTMI.status1<<"\t";
						fdtmi<<"0x"<<setw(4)<<mDTMI.status2<<"\t";
						fdtmi<<dec<<setfill(' ');
						fdtmi<<setw(6)<<mDTMI.serialNumber<<"\t";
						fdtmi<<setw(6)<<mDTMI.timeExp<<"\t";
						fdtmi<<setw(6)<<mDTMI.nLocalObj<<"\t";
						fdtmi<<setw(6)<<mDTMI.nDeterObj<<"\t";
						fdtmi<<setw(6)<<mDTMI.nWindows<<"\t";
						fdtmi<<setw(6)<<mDTMI.epsillon<<"\t";
						fdtmi<<setw(6)<<mDTMI.dTimeBOKZ<<"\t";
						fdtmi<<setw(6)<<GetTimeString(mDTMI.timeQuatLast)<<"\t";
						fdtmi<<"\n";

						ConvertDataDTMI(mDTMI, mCadr);
						mCadr.Time=cntRecDTMI++;
					}
					vCadrInfo.push_back(mCadr);
				}
			}
		}

		finp.close();
		fout.close();
		fshtmi1.close();
		fshtmi2.close();
		fdtmi.close();
		fmloc.close();

		PrepareStartDraw();
		CheckTabSheet();
	}
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
					readBOKZ60LocProtocol (in, vCadrInfo, handler);
				}

				else
				{
					Handle60 handle(this);
					readBOKZ60Protocol(in, vCadrInfo, handle);
				}

				}

			PrepareStartDraw();
			CheckTabSheet();
		}
	}

	catch (exception &e) {
		ShowMessage(e.what());
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
			TDateTime startDate = TDateTime (2017,9,26,0,0,0,0);
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

void TFormGraphOrient::CalculateSeriesSKO()
{
	pair <double, double> meanStd;
	Statistika statParam;
	int numberRow = 1;

	if (ChartFone->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.MeanBright;} } GetMeanBright;
		statParam = calculateStatParam(vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetMeanBright);
		AddRowToStatTable(numberRow++, "Уровень фона, ЕМР", statParam, 8, 2);
		ChartFone->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartNoise->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.SigmaBright;} } GetSigmaBright;
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
		struct { double operator() (const CadrInfo& a, bool& f) {f = false; return a.CountDeterObj;} } GetCountDeterObj;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetCountDeterObj);
		AddRowToStatTable(numberRow++, "Число звезд", statParam, 8, 2);
		ChartNumDet->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	//struct { double operator() (const CadrInfo& a) {return a.MatrixTemp;} } GetMatrixTemp;
	//meanStd = calculateMeanStdDv (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetMatrixTemp);

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
//		ChartAz->Series[0]->Title += " CКО: " + FloatToStrF(meanStd.second, ffFixed, 8, 4);
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
		statParam.mean *= RTM; 		statParam.sigma *= RTM;
		statParam.min *= RTM;		statParam.max *= RTM;
		AddRowToStatTable(numberRow++, "Угловая скорость по оси OX, угл. мин/с (угл. сек/с)", statParam, 8, 4);
		ChartWx->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartWy->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaOrient[1];} } GetOmegaOrientS;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaOrientS);
		statParam.mean *= RTM; 		statParam.sigma *= RTM;
		statParam.min *= RTM;		statParam.max *= RTM;
		AddRowToStatTable(numberRow++, "Угловая скорость по оси OY, угл. мин/с (угл. сек/с)", statParam, 8, 4);
		ChartWy->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
	}

	if (ChartWz->SeriesCount()) {
		struct { double operator() (const CadrInfo& a, bool& f) {f = !a.IsOrient; return a.OmegaOrient[2];} } GetOmegaOrientT;
		statParam = calculateStatParam (vCadrInfo.begin(), vCadrInfo.end(), 0.0, GetOmegaOrientT);
		statParam.mean *= RTM; 		statParam.sigma *= RTM;
		statParam.min *= RTM;		statParam.max *= RTM;
		AddRowToStatTable(numberRow++, "Угловая скорость по оси OZ, угл. мин/с (угл. сек/с)", statParam, 8, 4);
		ChartWz->Series[0]->Title += " CКО: " + FloatToStrF(statParam.sigma, ffFixed, 8, 4);
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
void __fastcall TFormGraphOrient::ReadIKIFormatClick(TObject *Sender)
{
	const int maxDrawFrag = 12;
	TColor colorFrag [maxDrawFrag];

	try
	{
		OpenDialog->Filter = "iki|*.iki";
		FileOpenDialog1->Options << fdoPickFolders;
		OpenDialog->Options << ofAllowMultiSelect;
		if (OpenDialog->Execute())
		{
			unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			SetCurrentDir(ExtractFileDir(FileList->Strings[0]));
			FileOpenDialog1->FileName = GetCurrentDir();
			FileList->Sort();

			if (FileOpenDialog1->Execute())
			{
				plotter->ResetOptions();
				DeleteLineGraph();
				vCadrInfo.clear();
				ClearStatusInfoTable();
				FileTitle = "IKI";

				for (int iFrag  = 0; iFrag < maxDrawFrag; iFrag++) {
					colorFrag[iFrag] = RGB((float)(maxDrawFrag - iFrag) / maxDrawFrag * 255,
												200, (float)iFrag / maxDrawFrag * 255 );
					plotter->SetShowLines(true);
					plotter->SetSeriesColor(colorFrag[iFrag]);
					plotter->SetTitle(IntToStr(iFrag + 1));
					plotter->SetDateTimeX(FormAnimateSetting->CheckBoxDateTime->Checked);

					plotter->AddSeries(ChartFragBright, iFrag, colorFrag[iFrag]);
					plotter->AddSeries(ChartFragSizeEl, iFrag, colorFrag[iFrag]);
					plotter->AddSeries(ChartFragErrX, iFrag, colorFrag[iFrag]);
					plotter->AddSeries(ChartFragErrY, iFrag, colorFrag[iFrag]);
					plotter->AddSeries(ChartFragMean, iFrag, colorFrag[iFrag]);
					plotter->AddSeries(ChartFragNoise, iFrag, colorFrag[iFrag]);
					plotter->AddSeries(ChartFragLevel, iFrag, colorFrag[iFrag]);
				}

				UnicodeString filePrefix = FormAnimateSetting->EditFilePrefix->Text;
				unsigned short startFrom = StrToInt(FormAnimateSetting->BeginFromEdit->Text);
				bool statusTableInited = false;
				for (int i = startFrom; i < FileList->Count; i++)
				{
					unique_ptr <IKI_img> reader(new IKI_img());
					if( !AnsiContainsStr(FileList->Strings[i], filePrefix))
					{
						if (reader->ReadFormat(toStdString(FileList->Strings[i]), false))
						{
							TStringDynArray SplittedString = SplitString(FileList->Strings[i], "\\");
							UnicodeString ResFileName = FileOpenDialog1->FileName + "\\" + filePrefix + SplittedString[SplittedString.Length - 1];
							if (FileExists(ResFileName) && (reader->ReadFormat(toStdString(ResFileName), false)))
							{
								CompareIKIRes = true;
								if (!statusTableInited)
								{
									vector <string> splitted = split(reader->CameraSettings.DataSource, "_");
									InitStatusInfoTable(splitted[0]);
									statusTableInited = true;
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
						vCadrInfo.push_back(move(convertIKIFormatToInfoCadr(reader.get(), CompareIKIRes)));
						double Time =  vCadrInfo.back().Time;
						AddRowToStatusTable(vCadrInfo.back());

						if (vCadrInfo.back().IsOrient)
						{
                            plotter->SetShowLines(false);
							plotter->SetTitle("измерения");
							plotter->SetSeriesColor(clBlue);
							plotter->SetDateTimeX(FormAnimateSetting->CheckBoxDateTime->Checked);

							plotter->AddPoint(ChartNumFrag, 0, Time, vCadrInfo.back().CountWindows);
							plotter->AddPoint(ChartNumLoc, 0, Time, vCadrInfo.back().CountLocalObj);
							plotter->AddPoint(ChartFone, 0, Time, vCadrInfo.back().MeanBright);
							plotter->AddPoint(ChartNoise, 0, Time, vCadrInfo.back().SigmaBright);
							//plotter->AddPoint(ChartTemp, 0, Time, vCadrInfo.back().MatrixTemp);
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
							plotter->AddPoint(ChartAl, 0, Time, vCadrInfo.back().AnglesOrient[0] * RTD);
							plotter->AddPoint(ChartDl, 0, Time, vCadrInfo.back().AnglesOrient[1] * RTD);
							plotter->AddPoint(ChartAz, 0, Time, vCadrInfo.back().AnglesOrient[2] * RTD);
							plotter->AddPoint(ChartWx, 0, Time, vCadrInfo.back().OmegaOrient[0] * RTM);
							plotter->AddPoint(ChartWy, 0, Time, vCadrInfo.back().OmegaOrient[1] * RTM);
							plotter->AddPoint(ChartWz, 0, Time, vCadrInfo.back().OmegaOrient[2] * RTM);

							plotter->SetTitle("модель");
							plotter->SetSeriesColor(clLime);
							plotter->AddPoint(ChartAl, 1, Time, vCadrInfo.back().AnglesModel[0] * RTD);
							plotter->AddPoint(ChartDl, 1, Time, vCadrInfo.back().AnglesModel[1] * RTD);
							plotter->AddPoint(ChartAz, 1, Time, vCadrInfo.back().AnglesModel[2] * RTD);
							plotter->AddPoint(ChartWx, 1, Time, vCadrInfo.back().OmegaModel[0] * RTM);
							plotter->AddPoint(ChartWy, 1, Time, vCadrInfo.back().OmegaModel[1] * RTM);
							plotter->AddPoint(ChartWz, 1, Time, vCadrInfo.back().OmegaModel[2] * RTM);

							//статистика по фрагментам
							int iObject = 0;
							for (int iFrag  = 0; iFrag < vCadrInfo.back().SizeWindowsList; iFrag++) {
								plotter->SetSeriesColor(colorFrag[iFrag]);
								plotter->AddPoint(ChartFragMean, iFrag, Time,
												  				vCadrInfo.back().WindowsList[iFrag].Mean);
								plotter->AddPoint(ChartFragNoise, iFrag, Time,
																vCadrInfo.back().WindowsList[iFrag].Sigma);
								plotter->AddPoint(ChartFragLevel, iFrag, Time,
																vCadrInfo.back().WindowsList[iFrag].Level);
								for (int iObjFrag = 0; iObjFrag < vCadrInfo.back().WindowsList[iFrag].CountObj; iObjFrag++) {
									if (iFrag < maxDrawFrag) {
										plotter->AddPoint(ChartFragBright, iFrag, Time,
																vCadrInfo.back().ObjectsList[iObject].Bright);
										if (vCadrInfo.back().ObjectsList[iObject].Square) {
											plotter->AddPoint(ChartFragSizeEl, iFrag, Time,
																	vCadrInfo.back().ObjectsList[iObject].Square);
										}
										if (vCadrInfo.back().ObjectsList[iObject].StarID) {
											plotter->AddPoint(ChartFragErrX, iFrag, Time,
																	vCadrInfo.back().ObjectsList[iObject].Dx*1000.);
											plotter->AddPoint(ChartFragErrY, iFrag, Time,
																	vCadrInfo.back().ObjectsList[iObject].Dy*1000.);
										}
									}
									iObject++;
								}
							}

							//статистика по звездам
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

									if (vCadrInfo.back().ObjectsList[iObject].Sp!="__") {
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
			PrepareStartDraw();

//печать статистики по серии кадров
//			if (FormAnimateSetting->CheckBoxPrintReport->Checked) {
//				PrintReportRes(vCadrInfo);
//			}
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
	ResizePlot(ChartMx,  1, 3, 0, 0);
	ResizePlot(ChartMy,  1, 3, 0, 1);
	ResizePlot(ChartMxy, 1, 3, 0, 2);

//число объектов
	ResizePlot(ChartNumFrag, 1, 3, 0, 0);
	ResizePlot(ChartNumLoc,  1, 3, 0, 1);
	ResizePlot(ChartNumDet,  1, 3, 0, 2);

//характеристики изображения
	ResizePlot(ChartFone,  1, 3, 0, 0);
	ResizePlot(ChartNoise, 1, 3, 0, 1);
	ResizePlot(ChartTemp,  1, 3, 0, 2);

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
			TDateTime startDate = TDateTime (2017,9,26,0,0,0,0);  //  исправить
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

			PrepareStartDraw();
			CheckTabSheet();
		}
	}
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
		 OpenDialog->Filter = "ip1|*.ip1 | ip2|*.ip2";
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




