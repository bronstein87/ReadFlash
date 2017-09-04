#pragma hdrstop
#include "UnitGraphOrient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VCLTee.TeeTools"
#pragma link "VCLTee.TeeEdit"
#pragma link "VCLTee.ErrorBar"
#pragma link "VCLTee.TeeHighLowLine"
#pragma resource "*.dfm"



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
		ScaleFactorForScrollBox(10.2),
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
        Charts.push_back(ChartErrorOX); Charts.push_back(ChartErrorOY); Charts.push_back(ChartErrorOZ);
		Charts.push_back(ChartWxError); Charts.push_back(ChartWyError); Charts.push_back(ChartWzError);
		Charts.push_back(ChartAnalyzeErrorAl); Charts.push_back(ChartAnalyzeErrorDl); Charts.push_back(ChartAnalyzeErrorAz);
		Charts.push_back(ChartAnalyzeXV); Charts.push_back(ChartAnalyzeYV); Charts.push_back(ChartAnalyzeZV);

		for (int i = 0; i < Charts.size(); i++) {
			Charts[i]->OnMouseWheel = &ChartMouseWheel;
			Charts[i]->OnMouseDown = &ChartMouseDown;
		}

}


void TFormGraphOrient::DeleteLineGraph()
{
	for (int i = 0; i < Charts.size(); i ++)
	{
		plotter->ClearChart(Charts[i]);
	}
}

void TFormGraphOrient::InitializeSynchronization()
{
	for (int i = 0; i < Charts.size(); i ++) {
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
	for (int i = 0; i < Charts.size(); i ++)
	{
		UnicodeString Title = LeftStr(Charts[i]->Title->Text->Text, PosEx(",", Charts[i]->Title->Text->Text, 1) - 1);
		plotter->SaveChart(Charts[i], Title, 500, 900);
	}
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
		BlockSeries[i] = new TChartShape(Chart1);
		BlockSeries[i]->Selected->Hover->Hide();
		Chart1->AddSeries(BlockSeries[i]);
		BlockSeries[i]->Style = chasRectangle;
		BlockSeries[i]->X0 = 0;
		BlockSeries[i]->X1= 1024;
		BlockSeries[i]->Y0 = 0;
		BlockSeries[i]->Y1 = 0;
		BlockSeries[i]->Color = clYellow;
		BlockSeries[i]->Transparency = 70;
	}

//InitFrameSeries
	for (int i=0; i < MaxFrameSeries; i++) {
		FrameSeries[i] = new TChartShape(Chart1);
		FrameSeries[i]->Selected->Hover->Hide();
		Chart1->AddSeries(FrameSeries[i]);
		FrameSeries[i]->Style = chasRectangle;
		FrameSeries[i]->X0 = 0;
		FrameSeries[i]->X1 = 0;
		FrameSeries[i]->Y0 = 0;
		FrameSeries[i]->Y1 = 0;
		FrameSeries[i]->Color = clGreen;
		FrameSeries[i]->Transparency = 70;
	}

	InitTableWindows();
	InitTableObjects();

	Series1->Selected->Hover->Hide();
	Series2->Selected->Hover->Hide();
	Series3->Selected->Hover->Hide();
	Series9->Selected->Hover->Hide();

	InitializeSynchronization();
}
//---------------------------------------------------------------------------
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

void TFormGraphOrient::PrepareStartDraw()
{
	EditNumCadr->Text = 0;
	UpDown1->Max = vCadrInfo.size() - 1;
	DrawAnimateHandler();
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

void TFormGraphOrient::ClearAnimate(void)
{
//AnimateShapes
	for (int i = 0; i < MaxBlockSeries; i++)
		BlockSeries[i]->Visible = false;
	for (int i = 0; i < MaxFrameSeries; i++)
		FrameSeries[i]->Visible = false;

//AnimatePoints
	Series9->Clear();
	Series1->Clear();
	Series2->Clear();
	Series3->Clear();
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
	for (int i = 0; i < Charts.size(); i++) {
	   TColorLineTool* LineTool = dynamic_cast <TColorLineTool*> (Charts[i]->Tools->First());
	   LineTool->Value = Value;
	}
}

void TFormGraphOrient::DrawBlock(const struct CadrInfo &mCadr)
{
	unsigned short CountLines, CountBlock, TabTakeAway[MaxBlockSeries][2];
	unsigned short Border = 10;

	Chart1->LeftAxis->Automatic = false;
	Chart1->LeftAxis->Minimum = 0;
	Chart1->LeftAxis->Maximum = mCadr.ImageHeight;
	Chart1->BottomAxis->Automatic = false;
	Chart1->BottomAxis->Minimum = 0;
	Chart1->BottomAxis->Maximum = mCadr.ImageWidth;

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
		FrameSeries[i]->Visible = true;
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
	for (int i = 0; i < mCadr.SizeStarsList; i++){

//	  LabelStar="Mv="+FloatToStrF(mCadr.StarsList[i].Mv,ffFixed,8,2);
		Series1->AddXY(mCadr.StarsList[i].X,mCadr.StarsList[i].Y);//,LabelStar);
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

void TFormGraphOrient::DrawAnimate(const struct CadrInfo &mCadr)
{
	double X0, Y0, X1, Y1, Dx, Dy, Ist, Nel;

	EditTimeDev->Text = FloatToStrF(mCadr.Time,ffFixed, 10, 3);
	ClearAnimate();

	for (int i = 0; i < mCadr.SizeObjectsList; i++) {       //!!!
		X0 = mCadr.ObjectsList[i].X;
		Y0 = mCadr.ObjectsList[i].Y;
		Nel = mCadr.ObjectsList[i].Square;
		Dx = mCadr.ObjectsList[i].Dx;
		Dy = mCadr.ObjectsList[i].Dy;

		if ((!mCadr.ObjectsList[i].StarID) && (!Dx)&&(!Dy)) {
//			Series2->AddXY(X0, Y0, "", clBlue);
			Series9->AddBubble(X0, Y0,(int)(3*sqrtm(fabs(Nel))+0.5),"",clBlue);
		}
		else
		{
			X1=X0 - mCadr.ObjectsList[i].Dx * 10000.;
			Y1=Y0 - mCadr.ObjectsList[i].Dy * 10000.;
//			Series2->AddXY(X0, Y0, "", clGreen);
			Series9->AddBubble(X0, Y0,(int)(3 * sqrtm(Nel) + 0.5),"",clGreen);
			Series3->AddArrow(X0, Y0, X1, Y1, "", clRed);
		}
//		Series9->AddBubble(mCadr.StarsList[i].X,mCadr.StarsList[i].Y,
//						(int)(sqrtm(mCadr.StarsList[i].Square)+1));

	}
	DrawFragment(mCadr);
	DrawBlock(mCadr);

	PrintTableWindows(mCadr);
	PrintTableObjects(mCadr);

}


void TFormGraphOrient::DrawFragment(const struct CadrInfo &mCadr)
{
	resetFragmentShowScrollBox();

	for(unsigned int i = 0;i < ImageScrollBoxVector.size();i ++)
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
		LabelFrameReport->Visible = true;
		LabelFrameReport->Caption = "Указан неверный путь к директории фрагментов";
		return;
	}

   TStringDynArray FileNameList;
   FileNameList = TDirectory::GetFiles(NeededDirectory);
   AnsiString TimePrStr = FloatToStr(mCadr.Time);


   AnsiString FragmentFileStr;
   for(int CurrentFileName = 0;CurrentFileName < FileNameList.Length;CurrentFileName ++)
   {
		if(AnsiContainsStr(FileNameList[CurrentFileName], TimePrStr))
		  {
				 FragmentFileStr = FileNameList[CurrentFileName];
				 break;
		  }
   }

   if(!FragmentFileStr.IsEmpty())
   {

   std::ifstream fragmentFile(FragmentFileStr.c_str(), std::ios::binary);;
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
		std::unique_ptr<TBitmap> Fragment(createFragmentBitmap(FragmentVector.back()));


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

		resizeBitmap(FragmentVector.back().SizeX * ResizeCoef, FragmentVector.back().SizeY * ResizeCoef, ImageVector.back()->Picture->Bitmap);

		TImage* FragmentNumber = new TImage(ImageScrollBoxVector.back());
		FragmentNumber->Height = 15;
		FragmentNumber->Width = 15;
		FragmentNumber->Canvas->Brush->Color = clWhite;
		TRect TheRect = Rect(0, 0, 15, 15);
		FragmentNumber->Canvas->TextRect(TheRect, 0, 0, IntToStr(CurrentFragment + 1));
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
		for(int i = 0; i < ImageVector.size(); i ++)
		{
			writePixelValue(FragmentVector[i], ImageVector[i]->Picture->Bitmap, ResizeCoef, 5, 7);
		}
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
	for(int currentFragment = 0;currentFragment < ImageVector.size();currentFragment ++)
	{
	   std::unique_ptr<TBitmap> Fragment(changeContrast(Contrast, FragmentVector[currentFragment]));
	   ImageVector[currentFragment]->Picture->Bitmap->FreeImage();
	   ImageVector[currentFragment]->Picture->Bitmap = NULL;
	   ImageVector[currentFragment]->Canvas->
	   StretchDraw(Rect(0, 0, ImageVector[currentFragment]->Width, ImageVector[currentFragment]->Height),Fragment.get());
	   resizeBitmap(FragmentVector[currentFragment].SizeX * ResizeCoef, FragmentVector[currentFragment].SizeY * ResizeCoef, ImageVector[currentFragment]->Picture->Bitmap);
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
		ScaleFactorForScrollBox = ScaleFactor + 0.2;
	}
}


void __fastcall TFormGraphOrient::PixelBrightCheckBoxClick(TObject *Sender)
{
	if(PixelBrightCheckBox->Checked)
	{
		for(int i = 0; i < ImageVector.size(); i ++)
		{
			writePixelValue(FragmentVector[i], ImageVector[i]->Picture->Bitmap, ResizeCoef, 2 , FontSize);
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
		int cnt = StrToInt(EditNumCadr->Text);
		if (!GetCadrInfo(cnt, CurCadr))
		{
			if (FormAnimateSetting->CheckBoxCurrentTime) {
				 SynchronizeCharts(CurCadr.Time);
			}

			if (CurCadr.FrameNumber != 0) {
				LabelFrameReport->Caption = "Номер кадра из файла: " + IntToStr(CurCadr.FrameNumber);
			}
			DrawAnimate(CurCadr);
		}
	}
	catch(std::exception &e)
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
	catch(std::exception &e)
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
	catch(std::exception &e)
	{
		ShowMessage(e.what());
	}
}

void  TFormGraphOrient::DrawBlockHandler(void)
{
	try
	{
		CadrInfo CurCadr;
		int cnt = StrToInt(EditNumCadr->Text);

		if (!GetCadrInfo(cnt, CurCadr))
		{
			 DrawBlock(CurCadr);
		}
	}
	catch(std::exception &e)
	{
		ShowMessage(e.what());
	}
}


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
					return "";
			   }

			  std::vector<RawFileInfo>  RawFileInfoVector;
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


			   std::sort(RawFileInfoVector.begin(), RawFileInfoVector.end(), FlashCompare);
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

void __fastcall TFormGraphOrient::FormClose(TObject *Sender, TCloseAction &Action)

{
		vCadrInfo.clear();
		Action = caFree;
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
		FileName = OpenDialog->FileName;
		SetCurrentDir(ExtractFileDir(FileName));
		GetFileTitles(FileName,&FileTitle);

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
		FragDir = CurDir + "\\Frag_" + FileTitle;
		LocDir = CurDir + "\\Loc_" + FileTitle;
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
	 }
  }
	catch(std::string &s)
	{
		ShowMessage(s.c_str());
	}

	catch(std::exception &e)
	{
		ShowMessage(e.what());
	}

}






 //---------------------------------------------------------------------------
#define MAX_STAT 	   16
#define MAX_OBJ_DTMI   15
#define MAX_OBJ_MLOC   32
#define MAX_OBJ_BOKZM  30
#define MAX_WINDOW 	   16

struct SHTMI1
{
	std::string timeBOKZ;
	unsigned short status1, status2, post;
	int serialNumber;
	float Foc, Xg, Yg;
	unsigned short timeExp, Mean, Sigma, countDefect;
    unsigned short CRC, Date, Version;
} mSHTMI1;

struct SHTMI2
{
	std::string timeBOKZ;
	unsigned short status1, status2, post;
	int serialNumber, timeExp;
	int cntCommandWord, cntCallNO, cntNOtoSLEZH;
	int cntCallTO, cntTOtoSLEZH, cntSLEZH;
	int cntStatOrient[MAX_STAT];
} mSHTMI2;

struct DTMI
{
	std::string timeBOKZ;
	unsigned short status1, status2;
	unsigned short serialNumber, timeExp;
	unsigned short nLocalObj, nDeterObj, nWindows, epsillon;
	float dTimeBOKZ, LocalList[MAX_OBJ_DTMI][4];
	float quatBoard[4], omega[3], centrWindow[MAX_WINDOW][2];
	unsigned short levelWindow[MAX_WINDOW], nObjectWindow[MAX_WINDOW];
	unsigned long timeQuatLast;
	float quatLast[4], Epoch;
	unsigned short nLocal[2], maxHist;
	unsigned short maxHistX, maxHistY;
	unsigned short test_short, Reserved[10];
} mDTMI;

struct LOC
{
	std::string timeBOKZ;
	unsigned short status1, status2;
	unsigned short serialNumber, timeExp;
	unsigned short nLocalObj, nFixedObj;
	float MeanC, SigmaC;
	float LocalList[32][4];
	unsigned short Reserved[2];
} mLOC;

std::string arrStatErrorEng[MAX_STAT]={{"EC1"},{"EC2"},{"EC3"},{"EC4"},
							{"EC5"},{"EC6"},{"EC7"},{"EC8"},
							{"EC9"},{"EC10"},{"EC11"},{"EC12"},
							{"EC13"},{"EC14"},{"EC15"},{"EC16"}};
std::string arrStatErrorRus[MAX_STAT]={{"ЕС1"},{"ЕС2"},{"ЕС3"},{"ЕС4"},
							{"ЕС5"},{"ЕС6"},{"ЕС7"},{"ЕС8"},
							{"ЕС9"},{"ЕС10"},{"ЕС11"},{"ЕС12"},
							{"ЕС13"},{"ЕС14"},{"ЕС15"},{"ЕС16"}};

unsigned int ReadBinaryString(std::string binaryString)
{
std::string test_str;
double sum=0;

	for (int k=0; k<binaryString.length(); k++)
	{
		test_str=binaryString[binaryString.length()-k-1];
		sum+=pow(2.,k)*atoi(test_str.c_str());
	}
	return (unsigned int)sum;
}

int TryReadSHTMI1(ifstream &finp, struct SHTMI1 &tmi)
{
std::string line, word;

		while (!finp.eof()){
			finp>>word;
			if ((word=="КС1")||(word=="KC1")) {
				finp>>word;
				tmi.status1=ReadBinaryString(word);
			}
			else if ((word=="КС2")||(word=="KC2")) {
				finp>>word;
				tmi.status2=ReadBinaryString(word);
			}
			else if ((word=="СЕР")||(word=="CEP")) {
				finp>>word;
				if ((word=="НОМ")||(word=="HOM")) {
					finp>>tmi.serialNumber;
				}
			}
			else if ((word=="ПОСТ")||(word=="ПOCT")) {
				finp>>word;
				tmi.post=ReadBinaryString(word);
			}
			else if ((word=="T")||(word=="Т")) {
				finp>>word;
				if (word=="ЭКСП") finp>>tmi.timeExp;
				else tmi.timeBOKZ=word;//finp>>tmi.timeBOKZ;
			}
			else if (word=="ФOK") finp>>tmi.Foc;
			else if (word=="Х0") finp>>tmi.Xg;
			else if (word=="У0") finp>>tmi.Yg;
			else if ((word=="МТ")||(word=="MT")) finp>>tmi.Mean;
			else if ((word=="СТ")||(word=="CT")) finp>>tmi.Sigma;
			else if ((word=="НАМ")||(word=="HAM")) {
				finp>>word;
				if ((word=="ДЕФ")||(word=="ДEФ")) finp>>tmi.countDefect;
			}
			else if (word=="ХСФ") finp>>tmi.CRC;
			else if (word=="ДАТА") finp>>tmi.Date;
			else if (word=="ВЕРСИЯ") {
				finp>>tmi.Version;
				return 1;
			}
			getline(finp, line, '\n' );
		}
		return 0;
}

int TryReadSHTMI2(ifstream &finp, struct SHTMI2 &tmi)
{
std::string line, word;

		while (!finp.eof()){
			finp>>word;
			if ((word=="КС1")||(word=="KC1")) {
				finp>>word;
				tmi.status1=ReadBinaryString(word);
			}
			else if ((word=="КС2")||(word=="KC2")) {
				finp>>word;
				tmi.status2=ReadBinaryString(word);
			}
			else if ((word=="СЕР")||(word=="CEP")) {
				finp>>word;
				if ((word=="НОМ")||(word=="HOM")) {
					finp>>tmi.serialNumber;
				}
			}
			else if ((word=="ПОСТ")||(word=="ПOCT")) {
				finp>>word;
				tmi.post=ReadBinaryString(word);
			}
			else if ((word=="T")||(word=="Т")) {
				finp>>word;
				if (word=="ЭКСП") finp>>tmi.timeExp;
				else finp>>tmi.timeBOKZ;    //ReadTimeBOKZ(word)
			}
			else if (word=="НАМ") {
            	finp>>word;
				if (word=="УСД") finp>>tmi.cntCommandWord;
            }
			else if ((word=="ЧИСЛ")||(word=="ЧИСЛО")) {
				finp>>word;
				if ((word=="НО")||(word=="HO")) {
					finp>>tmi.cntCallNO;
				}
				else if (word=="НОСЛ") {
					finp>>tmi.cntNOtoSLEZH;
				}
				if ((word=="TO")||(word=="ТО")) {
					finp>>tmi.cntCallTO;
				}
				else if (word=="ТОСЛ") {
					finp>>tmi.cntTOtoSLEZH;
				}
				else if (word=="СЛЕЖ") {
					finp>>tmi.cntSLEZH;
                    tmi.cntSLEZH*=2;
				}
			}
			else
			{
				int i=0, fl_find=0;
				while ((i < MAX_STAT)&&(!fl_find))
				{
					if ((word==arrStatErrorRus[i])||(word==arrStatErrorEng[i])) {
						finp>>tmi.cntStatOrient[i];
						fl_find=1;
					}
					i++;
				}
			}
			if ((word == arrStatErrorRus[MAX_STAT-1]) || (word == arrStatErrorEng[MAX_STAT-1])) {
				return 1;
			}
			getline(finp, line, '\n' );
		}
		return 0;
}

int TryReadDTMI(ifstream &finp, struct DTMI &tmi)
{
		std::string line, word, inpstr, test_word, test_str;
		int indexObject=0,indexParam=0, intVal, flLow=1;
		float fl1, fl2, fl3, sum;
		int Stat1, Stat2;
		float mday, mhour, mmin, msec;

		while (!finp.eof()){
			finp>>word;
			if ((word=="КС1")||(word=="KC1")) {
				finp>>word;
				tmi.status1=ReadBinaryString(word);
			}
			else if ((word=="КС2")||(word=="KC2")) {
				finp>>word;
				tmi.status2=ReadBinaryString(word);
			}
			else if ((word=="СЕР")||(word=="CEP")) {
				finp>>word;
				if ((word=="НОМ")||(word=="HOM")) {
					finp>>tmi.serialNumber;
				}
			}
			else if ((word=="HAM")||(word=="НАМ")) {
				finp>>word;
				if (word=="ЛОК") {
					finp>>tmi.nLocalObj;
				}
				else if (word=="ОБЖ") {
					finp>>tmi.nDeterObj;
				}
				else if (word=="ФРАГ") {
					finp>>tmi.nWindows;
				}
			}
			else if (word=="ЭПСИЛОН") finp>>tmi.epsillon;
			else if (word=="ДЕЛЬТА") {
				finp>>word;
				if ((word=="T")||(word=="Т")) finp>>tmi.dTimeBOKZ;
			}
			else if ((word=="T")||(word=="Т")) {
				finp>>word;
				if (word=="ЭКСП") finp>>tmi.timeExp;
				else if (word=="ПХОК") {
					getline(finp, line, '\n' );

					if (sscanf(line.c_str()," %f/ %f:%f:%f", &mday, &mhour, &mmin, &msec)==4)
						tmi.timeQuatLast=mday*86400+mhour*3600+mmin*60+msec;
						tmi.timeQuatLast*=8;
//					finp>>tmi.timeQuatLast;
				}
				else finp>>tmi.timeBOKZ;
			}
			else if (word=="ЛОК") {
				getline(finp, line, '\n' );
				int nread=sscanf(line.c_str(),"%f%f%f", &fl1, &fl2, &fl3);
				if (nread==3) {
					indexObject=(short)fl1;
					indexParam =(short)fl2;
					tmi.LocalList[indexObject][indexParam]=fl3;
				}
				else if (nread==2) {
					indexObject=(short)(fl1/10.);
					indexParam =(short)fl1%10;
					tmi.LocalList[indexObject][indexParam]=fl2;
				}
			}
			else  if (word=="ОМЕГАО") {
				finp>>indexParam;
				if ((indexParam>=0)&&(indexParam<3)) {
					finp>>tmi.omega[indexParam];
				}
			}
			else  if ((word=="XYC")||(word=="ХУС")) {
				getline(finp, line, '\n' );
				int nread=sscanf(line.c_str(),"%f%f%f", &fl1, &fl2, &fl3);
				if (nread==3) {
					indexObject=(short)fl1;
					indexParam =(short)fl2;
					tmi.centrWindow[indexObject][indexParam]=fl3;
				}
				else if (nread==2) {
					indexObject=(short)(fl1/10.);
					indexParam =(short)fl1%10;
					tmi.centrWindow[indexObject][indexParam]=fl2;
				}
			}
			else  if (word=="ТНФРАГ") {
				finp>>indexObject;
				if ((indexObject>=0)&&(indexObject<MAX_WINDOW)) {
					finp>>tmi.levelWindow[indexObject];
				}
			}
			else  if (word=="ОБЖФРАГ") {
				finp>>indexObject;
				if ((indexObject>=0)&&(indexObject<MAX_WINDOW)) {
					finp>>tmi.nObjectWindow[indexObject];
				}
			}
			else  if (word=="ПХОК") {
				finp>>indexParam;
				if ((indexParam>=0)&&(indexParam<4)) {
					finp>>tmi.quatLast[indexParam];
				}
			}
			else if (word=="ЭПОХА") {
			int test_int;
			float test_float;
				finp>>test_int;
				test_float=*(float*)&test_int;
				tmi.Epoch=test_float;
			}
			else  if (word=="НУМЛ") {
				finp>>indexParam;
				if ((indexParam>=0)&&(indexParam<2)) {
					finp>>tmi.nLocal[indexParam];
				}
			}
			else if ((word=="MAXH")||(word=="МАХН")) {
				finp>>tmi.maxHist;
			}
			else if (word=="ДХМАХН")  {
				finp>>tmi.maxHistX;
			}
			else if (word=="ДУМАХН") {
				finp>>tmi.maxHistY;
				return 1;
			}
//			else if ((word=="ДТМИ1")||(word=="ШТМИ1")||(word=="ШТМИ2"))
//				return 0;
		}
		return 0;
}

void PrintSHTMI1(ofstream &file, struct SHTMI1 tmi)
{
	file<<"____________________________________"<<"\n";
	file<<"Массив ШТМИ1"<<"\n";
	file<<"Tpr\t"<<tmi.timeBOKZ<<"\n";
	file<<uppercase<<hex<<setfill('0');
	file<<"КС1:\t"<<"0x"<<setw(4)<<tmi.status1<<"\n";
	file<<"КС2:\t"<<"0x"<<setw(4)<<tmi.status2<<"\n";
	file<<"POST:\t"<<"0x"<<setw(4)<<tmi.post<<"\n";
	file<<dec<<setfill(' ');
	file<<"Зав. №\t"<<tmi.serialNumber<<"\n";
	file<<"Texp, мс:\t"<<tmi.timeExp<<"\n";
	file<<"Foc, мм:\t"<<tmi.Foc<<"\n";
	file<<"X0, мм:\t"<<tmi.Xg<<"\n";
	file<<"Y0, мм:\t"<<tmi.Yg<<"\n";
	file<<"Среднее, е.м.р.:\t"<<tmi.Mean<<"\n";
	file<<"СКО, е.м.р.:\t"<<tmi.Sigma<<"\n";
	file<<"Число дефектов:\t"<<tmi.countDefect<<"\n";
	file<<"Идентификатор:\t"<<tmi.CRC<<"\n";
	file<<"Дата:\t"<<tmi.Date<<"\n";
	file<<"Версия:\t"<<tmi.Version<<"\n";
	file<<"____________________________________"<<"\n";
	file<<flush;
}

void PrintSHTMI2(ofstream &file, struct SHTMI2 tmi)
{
	file<<"____________________________________"<<"\n";
	file<<"Массив ШТМИ2"<<"\n";
	file<<"Tpr\t"<<tmi.timeBOKZ<<"\n";
	file<<uppercase<<hex<<setfill('0');
	file<<"КС1:\t"<<"0x"<<setw(4)<<tmi.status1<<"\n";
	file<<"КС2:\t"<<"0x"<<setw(4)<<tmi.status2<<"\n";
	file<<"POST:\t"<<"0x"<<setw(4)<<tmi.post<<"\n";
	file<<dec<<setfill(' ');
	file<<"Зав. №\t"<<tmi.serialNumber<<"\n";
	file<<"Texp, мс:\t"<<tmi.timeExp<<"\n";
	file<<"Число УСД:\t"<<tmi.cntCommandWord<<"\n";
	file<<"Число НО:\t"<<tmi.cntCallNO<<"\n";
	file<<"НО->СЛЕЖ:\t"<<tmi.cntNOtoSLEZH<<"\n";
	file<<"Число TО:\t"<<tmi.cntCallTO<<"\n";
	file<<"TО->СЛЕЖ:\t"<<tmi.cntTOtoSLEZH<<"\n";
	file<<"Число СЛЕЖ:\t"<<tmi.cntSLEZH<<"\n";

	for (int i = 0; i < MAX_STAT; i++) {
		file<<"Счетчик № "<<(i+1)<<":\t"<<tmi.cntStatOrient[i]<<"\n";
	}
	file<<"____________________________________"<<"\n";
	file<<flush;
}

void PrintDTMI(ofstream &file, struct DTMI tmi)
{
	file<<"____________________________________"<<"\n";
	file<<"Массив ДТМИ"<<"\n";
	file<<"Tpr\t"<<tmi.timeBOKZ<<"\n";
	file<<uppercase<<hex<<setfill('0');
	file<<"КС1\t"<<"0x"<<setw(4)<<tmi.status1<<"\n";
	file<<"КС2\t"<<"0x"<<setw(4)<<tmi.status2<<"\n";
	file<<dec<<setfill(' ');
	file<<"Зав. №\t"<<tmi.serialNumber<<"\n";
	file<<"Texp, мс:\t"<<tmi.timeExp<<"\n";
	file<<"NumLoc: \t"<<tmi.nLocalObj<<"\n";
	file<<"NumDet: \t"<<tmi.nDeterObj<<"\n";
	file<<"NumFrag:\t"<<tmi.nWindows<<"\n";
	file<<"EPSILON:\t"<<tmi.epsillon<<"\n";
	file<<"DeltaT:\t"<<tmi.dTimeBOKZ<<"\n";
	file<<std::setw(6)<<"№"<<" X, pix"<<" Y, pix"<<" Bright"<<" Nel"<<"\n";
	for (int i = 0; i < MAX_OBJ_DTMI; i++) {
		file<<std::setw(6)<<(i+1)<<"\t";
		file<<tmi.LocalList[i][0]<<"\t"<<tmi.LocalList[i][1]<<"\t";
		file<<tmi.LocalList[i][2]<<"\t"<<tmi.LocalList[i][3]<<"\n";
	}
	file<<std::setw(6)<<"№"<<" X, pix"<<" Y, pix"<<"\n";
	for (int i = 0; i < MAX_WINDOW; i++) {
		file<<std::setw(6)<<(i+1)<<"\t";
		file<<tmi.centrWindow[i][0]<<"\t"<<tmi.centrWindow[i][1]<<"\t";
		file<<tmi.levelWindow[i]<<"\t"<<tmi.nObjectWindow[i]<<"\n";
	}

	for (int i = 0; i < 3; i++) {
		file<<"Wop["<<i<<"]:\t"<<tmi.omega[i]<<"\n";
	}

	file<<"Tlst:\t"<<tmi.timeQuatLast<<"\n";
	for (int i = 0; i < 4; i++) {
		file<<"Qlst["<<i<<"]:\t"<<tmi.quatLast[i]<<"\n";
	}
	file<<"NumLoc[0]: \t"<<tmi.nLocal[0]<<"\n";
	file<<"NumLoc[1]: \t"<<tmi.nLocal[1]<<"\n";
	file<<"Эпоха: \t"<<tmi.Epoch<<"\n";
	file<<"MaxHist: \t"<<tmi.maxHist<<"\n";
	file<<"MaxHistX: \t"<<tmi.maxHistX<<"\n";
	file<<"MaxHistY: \t"<<tmi.maxHistY<<"\n";
	file<<"____________________________________"<<"\n";
	file<<flush;
}

void PrintLOC(ofstream &file, struct LOC tmi)
{
	file<<"____________________________________"<<"\n";
	file<<"Массив МЛОК"<<"\n";
	file<<"Tpr\t"<<tmi.timeBOKZ<<"\n";
	file<<uppercase<<hex<<setfill('0');
	file<<"КС1\t"<<"0x"<<setw(4)<<tmi.status1<<"\n";
	file<<"КС2\t"<<"0x"<<setw(4)<<tmi.status2<<"\n";
	file<<dec<<setfill(' ');
	file<<"Зав. №\t"<<tmi.serialNumber<<"\n";
	file<<"Texp, мс:\t"<<tmi.timeExp<<"\n";
	file<<"NumLoc: \t"<<tmi.nLocalObj<<"\n";
	file<<"NumFix: \t"<<tmi.nFixedObj<<"\n";
	file<<"MeanC:\t"<<tmi.MeanC<<"\n";
	file<<"SigmaC:\t"<<tmi.SigmaC<<"\n";
	file<<std::setw(6)<<"№"<<" X, pix"<<" Y, pix"<<" Bright"<<" Nel"<<"\n";
	for (int i = 0; i < 32; i++) {
		file<<std::setw(6)<<(i+1)<<"\t";
		file<<tmi.LocalList[i][0]<<"\t"<<tmi.LocalList[i][1]<<"\t";
		file<<tmi.LocalList[i][2]<<"\t"<<tmi.LocalList[i][3]<<"\n";
	}

	file<<"____________________________________"<<"\n";
	file<<flush;
}

void PrintLocalDTMI(struct DTMI tmi)
{
std::string fileName;

	fileName="BOKZ_"+tmi.timeBOKZ;
	for (int i = 0; i < fileName.length(); i++) {
		if ((fileName[i]==':')||(fileName[i]==';')||(fileName[i]=='?')
			||(fileName[i]=='>')||(fileName[i]=='<')||(fileName[i]=='=')
			||(fileName[i]=='/')||(fileName[i]=='\\')) {
			fileName[i]='_';
		}
	}
	fileName+="_DTMI_LOC.txt";
	ofstream file(fileName.c_str());

	file<<std::setw(6)<<"№"<<" X, pix"<<" Y, pix"<<" Bright"<<" Nel"<<"\n";

	int cntLocal;
	if (tmi.nLocalObj<MAX_OBJ_DTMI) cntLocal=tmi.nLocalObj;
	else cntLocal=MAX_OBJ_DTMI;
	for (int i = 0; i < cntLocal; i++) {
		file<<std::setw(6)<<(i+1)<<"\t";
		file<<tmi.LocalList[i][0]<<"\t"<<tmi.LocalList[i][1]<<"\t";
		file<<tmi.LocalList[i][2]<<"\t"<<tmi.LocalList[i][3]<<"\n";
	}
	file.close();
}

void PrintLocalMLOC(struct LOC tmi)
{
	std::string fileName="BOKZ_"+tmi.timeBOKZ;
	for (int i = 0; i < fileName.length(); i++) {
		if ((fileName[i]==':')||(fileName[i]==';')||(fileName[i]=='?')
			||(fileName[i]=='>')||(fileName[i]=='<')||(fileName[i]=='=')
			||(fileName[i]=='/')||(fileName[i]=='\\')) {
			fileName[i]='_';
		}
	}
	fileName+="_MLOC_LOC.txt";
	ofstream file(fileName.c_str());

	file<<std::setw(6)<<"№"<<" X, pix"<<" Y, pix"<<" Bright"<<" Nel"<<"\n";
	int cntLocal;
	if (tmi.nFixedObj<MAX_OBJ_MLOC) cntLocal=tmi.nFixedObj;
	else cntLocal=MAX_OBJ_MLOC;
	for (int i = 0; i < cntLocal; i++) {
		file<<std::setw(6)<<(i+1)<<"\t";
		file<<tmi.LocalList[i][0]<<"\t"<<tmi.LocalList[i][1]<<"\t";
		file<<tmi.LocalList[i][2]<<"\t"<<tmi.LocalList[i][3]<<"\n";
	}
	file.close();
}

void ConvertDataDTMI(struct DTMI tmi, struct CadrInfo &mCadr)
{
	mCadr.IsBinary=true;
//	mCadr.IsReverse=true;
	mCadr.ImageHeight=256;
	mCadr.ImageWidth=256;
//	mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;

	mCadr.CountDeterObj = tmi.nDeterObj;
	mCadr.CountWindows  = tmi.nWindows;
	mCadr.CountLocalObj = tmi.nLocalObj;

	if (tmi.nLocalObj<MAX_OBJ_DTMI) mCadr.SizeObjectsList=tmi.nLocalObj;
	else mCadr.SizeObjectsList=MAX_OBJ_DTMI;

	ObjectsInfo objInfo;
	for (int i = 0; i < mCadr.SizeObjectsList; i++) {
		objInfo.X = tmi.LocalList[i][0];
		objInfo.Y = tmi.LocalList[i][1];
		objInfo.Bright = tmi.LocalList[i][2];
		objInfo.Square = abs(tmi.LocalList[i][3]);
		objInfo.Dx = 0;
		objInfo.Dy = 0;
		objInfo.StarID = 0;
		objInfo.Mv = 0;
		objInfo.Sp[0] = '_';
		objInfo.Sp[1] = '_';
		mCadr.ObjectsList.push_back(objInfo);
	}

	if (tmi.nWindows < MAX_WINDOW) {
		mCadr.SizeWindowsList = tmi.nWindows;
		mCadr.SizeStarsList = tmi.nWindows;
	}
	else {
		mCadr.SizeWindowsList = MAX_WINDOW;
		mCadr.SizeStarsList =  MAX_WINDOW;
	}

	WindowsInfo winInfo;
	StarsInfo starList;
	for (int i = 0; i < mCadr.SizeWindowsList; i++) {
		winInfo.Level = tmi.levelWindow[i];
		winInfo.CountObj = tmi.nObjectWindow[i];
		winInfo.Width = 17;
		winInfo.Height = 17;
		winInfo.Xstart = tmi.centrWindow[i][0]-(winInfo.Width>>1);
		winInfo.Ystart = tmi.centrWindow[i][1]-(winInfo.Height>>1);
		mCadr.WindowsList.push_back(winInfo);

		starList.X = tmi.centrWindow[i][0];
		starList.Y = tmi.centrWindow[i][1];
	}

	mCadr.CountBlock = 0;
	mCadr.CountStars = 0;
}

void ConvertDataLOC(struct LOC tmi, struct CadrInfo &mCadr)
{
	mCadr.IsBinary = true;
//	mCadr.IsReverse=true;
	mCadr.ImageHeight = 256;
	mCadr.ImageWidth = 256;
//	mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;
	mCadr.CountLocalObj=tmi.nLocalObj;

	if (tmi.nLocalObj<MAX_OBJ_MLOC) mCadr.SizeObjectsList=tmi.nLocalObj;
	else mCadr.SizeObjectsList=MAX_OBJ_MLOC;

	ObjectsInfo objInfo;
	for (int i=0; i < mCadr.SizeObjectsList; i++) {
		objInfo.X = tmi.LocalList[i][0];
		objInfo.Y = tmi.LocalList[i][1];
		objInfo.Bright = tmi.LocalList[i][2];
		objInfo.Square = abs(tmi.LocalList[i][3]);
		objInfo.Dx = 0;
		objInfo.Dy = 0;
		objInfo.StarID = 0;
		objInfo.Mv = 0;
		objInfo.Sp[0] = '_';
		objInfo.Sp[1] = '_';
		mCadr.ObjectsList.push_back(objInfo);
	}
	mCadr.CountBlock = 0;
	mCadr.CountWindows = 0;
	mCadr.SizeWindowsList = 0;
	mCadr.CountDeterObj = 0;
	mCadr.CountStars = 0;
	mCadr.SizeStarsList = 0;
}

void __fastcall TFormGraphOrient::MenuOpenProgressTMIClick(TObject *Sender)
{
    OpenDialog->Options.Clear();
	OpenDialog->Filter = "txt|*.txt";
	if (OpenDialog->Execute()) {

		vCadrInfo.clear();
		DeleteLineGraph();

		FileName = OpenDialog->FileName;
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

		fshtmi1<<"KC1\t"<<"KC2\t"<<"POST\t"<<"№\t"<<"Texp\t";
		fshtmi1<<"Foc\t"<<"Xg\t"<<"Yg\t";
		fshtmi1<<"Mean\t"<<"Sigma\t";
		fshtmi1<<"Ndef\t"<<"CRC\t";
		fshtmi1<<"Date\t"<<"Version\t";
		fshtmi1<<"\n";

		fshtmi2<<"KC1\t"<<"KC2\t"<<"POST\t"<<"№\t"<<"Texp\t";
		fshtmi2<<"УСД\t"<<"НО\t"<<"НОСЛ\t";
		fshtmi2<<"TО\t"<<"TОСЛ\t"<<"СЛЕЖ\t";
		for (int i = 0; i < MAX_STAT; i++) {
			fshtmi2<<"EC"<<(i+1)<<"\t";
		}
		fshtmi2<<"\n";

		std::string line;
		int cntRecDTMI = 0;
		while (!finp.eof())
		{
			getline(finp, line, '\n' );
			if (line.find("ШТМИ1")!=std::string::npos) {
				if(TryReadSHTMI1(finp,mSHTMI1)) {
					PrintSHTMI1(fout,mSHTMI1);

					fshtmi1<<uppercase<<hex<<setfill('0');
					fshtmi1<<"0x"<<std::setw(4)<<mSHTMI1.status1<<"\t";
					fshtmi1<<"0x"<<std::setw(4)<<mSHTMI1.status2<<"\t";
					fshtmi1<<"0x"<<std::setw(4)<<mSHTMI1.post<<"\t";
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
			else if (line.find("ШТМИ2")!=std::string::npos) {
				if(TryReadSHTMI2(finp,mSHTMI2)) {
					PrintSHTMI2(fout,mSHTMI2);
					fshtmi2<<uppercase<<hex<<setfill('0');
					fshtmi2<<"0x"<<std::setw(4)<<mSHTMI2.status1<<"\t";
					fshtmi2<<"0x"<<std::setw(4)<<mSHTMI2.status2<<"\t";
					fshtmi2<<"0x"<<std::setw(4)<<mSHTMI2.post<<"\t";
					fshtmi2<<dec<<setfill(' ');
					fshtmi2<<std::setw(6)<<mSHTMI2.serialNumber;
					fshtmi2<<std::setw(6)<<mSHTMI2.timeExp;
					fshtmi2<<std::setw(8)<<mSHTMI2.cntCommandWord;
					fshtmi2<<std::setw(8)<<mSHTMI2.cntCallNO;
					fshtmi2<<std::setw(8)<<mSHTMI2.cntNOtoSLEZH;
					fshtmi2<<std::setw(8)<<mSHTMI2.cntCallTO;
					fshtmi2<<std::setw(8)<<mSHTMI2.cntTOtoSLEZH;
					fshtmi2<<std::setw(12)<<mSHTMI2.cntSLEZH;
					for (int i = 0; i < MAX_STAT; i++) {
						fshtmi2<<std::setw(8)<<mSHTMI2.cntStatOrient[i];
					}
					fshtmi2<<"\n";
				}
			}
			else if (line.find("ДТМИ1")!=std::string::npos) {

				if (TryReadDTMI(finp,mDTMI)) {
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
						ConvertDataLOC(mLOC, mCadr);
						mCadr.Time = cntRecDTMI++;
					}
					else {
						PrintDTMI(fout,mDTMI);
						PrintLocalDTMI(mDTMI);
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
		PrepareStartDraw();
	}
}

struct MSHI_BOKZM {
	unsigned short status1, status2;
	float timeBOKZ;
	float Mornt[3][3];
};

struct DTMI_BOKZM {
	float timeBOKZ;
	unsigned short status1, status2;
	unsigned short serialNumber;
	float foc, X0, Y0;
	unsigned short timeExp;
	unsigned short nLocalObj, nDeterObj;
	float LocalList[MAX_OBJ_BOKZM][4];
};

void ConvertDataDTMI_BOKZM(struct DTMI_BOKZM tmi, struct CadrInfo &mCadr)
{
	mCadr.IsBinary = true;
//	mCadr.IsReverse=true;
	mCadr.ImageHeight = 512;
	mCadr.ImageWidth = 512;
//	mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;
	mCadr.Time = tmi.timeBOKZ;
	mCadr.CountLocalObj = tmi.nLocalObj;

	if (tmi.nLocalObj < MAX_OBJ_BOKZM) mCadr.SizeObjectsList = tmi.nLocalObj;
	else mCadr.SizeObjectsList = MAX_OBJ_BOKZM;

	ObjectsInfo objInfo;
	for (int i = 0; i < mCadr.SizeObjectsList; i++) {
		objInfo.X = tmi.LocalList[i][0];
		objInfo.Y = tmi.LocalList[i][1];
		objInfo.Bright = tmi.LocalList[i][2];
		objInfo.Square = abs(tmi.LocalList[i][3]);
		objInfo.Dx = 0;
		objInfo.Dy = 0;
		objInfo.StarID = 0;
		objInfo.Mv = 0;
		objInfo.Sp[0] = '_';
		objInfo.Sp[1] = '_';
		mCadr.ObjectsList.push_back(objInfo);
	}

	mCadr.CountBlock = 0;
	mCadr.CountWindows = 0;
	mCadr.SizeWindowsList = 0;
	mCadr.CountDeterObj = 0;
	mCadr.CountStars = 0;
	mCadr.SizeStarsList = 0;
}

void PrintMSHI_BOKZM(ofstream &file, struct MSHI_BOKZM tmi)
{
	file<<"____________________________________"<<"\n";
	file<<"Массив МШИОР"<<"\n";
	file<<"Tpr\t"<<tmi.timeBOKZ<<"\n";
	file<<uppercase<<hex<<setfill('0');
	file<<"КС1\t"<<"0x"<<setw(4)<<tmi.status1<<"\n";
	file<<"КС2\t"<<"0x"<<setw(4)<<tmi.status2<<"\n";
	file<<dec<<setfill(' ');
	file<<"Матрица ориентации:\n";
	file<<std::setprecision(8);
	for (int i = 0; i < 3; i++) {
		file<<std::setw(12)<<tmi.Mornt[i][0];
		file<<std::setw(12)<<tmi.Mornt[i][1];
		file<<std::setw(12)<<tmi.Mornt[i][2]<<"\n";
	}

//	double ang[3], MorntT[3][3];
//
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//            MorntT[i][j] =  tmi.Mornt[j][i];
//		}
//	}
//	MatrixToEkvAngles(MorntT, ang);
//	file<<"Углы ориентации:\n";
//	file<<"Al = "<<ang[0]*RTD<<"\t";
//	file<<"Dl = "<<ang[1]*RTD<<"\t";
//	file<<"Az = "<<ang[2]*RTD<<"\n";
	file<<"____________________________________"<<"\n";
	file<<flush;
}

void PrintDTMI_BOKZM(ofstream &file, struct DTMI_BOKZM tmi)
{
	file<<"____________________________________"<<"\n";
	file<<"Массив ДТМИ"<<"\n";
	file<<"Tpr\t"<<tmi.timeBOKZ<<"\n";
	file<<uppercase<<hex<<setfill('0');
	file<<"КС1\t"<<"0x"<<setw(4)<<tmi.status1<<"\n";
	file<<"КС2\t"<<"0x"<<setw(4)<<tmi.status2<<"\n";
	file<<dec<<setfill(' ');
	file<<"Зав. №\t"<<tmi.serialNumber<<"\n";
	file<<"Foc, мм:\t"<<tmi.foc<<"\n";
	file<<"X0, мм:\t"<<tmi.X0<<"\n";
	file<<"Y0, мм:\t"<<tmi.Y0<<"\n";
	file<<"Texp, мс:\t"<<tmi.timeExp<<"\n";
	file<<"NumLoc: \t"<<tmi.nLocalObj<<"\n";
	file<<"NumFix: \t"<<tmi.nDeterObj<<"\n";
	file<<std::setw(6)<<"№"<<" X, pix"<<" Y, pix"<<" Bright"<<" Nel"<<"\n";
	for (int i = 0; i < MAX_OBJ_BOKZM; i++) {
		file<<std::setw(6)<<(i+1)<<"\t";
		file<<tmi.LocalList[i][0]<<"\t"<<tmi.LocalList[i][1]<<"\t";
		file<<tmi.LocalList[i][2]<<"\t"<<tmi.LocalList[i][3]<<"\n";
	}
	file<<"____________________________________"<<"\n";
	file<<flush;
}

unsigned short ArrayDTMI[290];
unsigned short ArrayMSHI[22];

void __fastcall TFormGraphOrient::MenuOpenEnergyTMIClick(TObject *Sender)
{
//	short mDay, mMonth, mYear, mHour, mMin, mSec;

    OpenDialog->Options.Clear();
	OpenDialog->Filter = "txt|*.txt";
	if (OpenDialog->Execute()) {

		vCadrInfo.clear();
		DeleteLineGraph();

		FileName = OpenDialog->FileName;
		SetCurrentDir(ExtractFileDir(FileName));
		GetFileTitles(FileName,&FileTitle);

		ifstream finp(FileName.c_str());
		if (!finp.is_open()) {
			ShowMessage("Файл не может быть открыт!");
			return;
		}

		ofstream fout((FileTitle+"_decode.txt").c_str());
		ofstream fmshi((FileTitle + "_mshi.txt").c_str());

		std::string line, word1, word2, word3;
		unsigned short hex_val, dec_val;
		int cntRecDTMI = 0, ind;
		while (!finp.eof())
		{
			getline(finp, line, '\n' );

//чтение массива МШИОР
			if (line.find("МШИОР") != std::string::npos) {
				getline(finp, line, '\n' );
				if (line.find("ТМОС") != std::string::npos) {
					fout<<"\n"<<line<<"\n";
					getline(finp, line, '\n' );

                    unsigned short nWord = 0;
					do
					{
						getline(finp, line, '\n' );
						if (line.find("CISO_MSHIOR") != std::string::npos) {
							int nBOKZ = -1;
							unsigned short word, buf;
							if (sscanf(line.c_str(), "CISO_MSHIOR[%ld].W[%d] %d", &nBOKZ, &buf, &word) == 3) {
								if ((nWord >= 0) && (nWord < 22)) {
									ArrayMSHI[nWord] =  word;
                                    nWord++;
								}
							}
						}
					} while (line.find("CIS1_DSP.TMOS") == std::string::npos);

					struct MSHI_BOKZM mshi;
					memcpy(&mshi, ArrayMSHI, 22*sizeof(short));
					PrintMSHI_BOKZM(fout, mshi);

					double ang[3], MorntT[3][3];

					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							MorntT[i][j] =  mshi.Mornt[j][i];
						}
					}

					MatrixToEkvAngles(MorntT, ang);

					if (mshi.status1 == 0xE004) {
						plotter->AddPoint(ChartAl, 0, mshi.timeBOKZ, ang[0]*RTD);
						plotter->AddPoint(ChartDl, 0, mshi.timeBOKZ, ang[1]*RTD);
						plotter->AddPoint(ChartAz, 0, mshi.timeBOKZ, ang[2]*RTD);
					}

					fmshi<<std::setprecision(8);
					fmshi<<mshi.timeBOKZ<<"\t";
					fmshi<<uppercase<<hex<<setfill('0');
					fmshi<<"0x"<<setw(4)<<mshi.status1<<"\t";
					fmshi<<"0x"<<setw(4)<<mshi.status2<<"\t";
					fmshi<<dec<<setfill(' ');

					fmshi<<ang[0]*RTD<<"\t";
					fmshi<<ang[1]*RTD<<"\t";
					fmshi<<ang[2]*RTD<<"\n";
                }
			}

//чтение массива ДТМИ
			if (line.find("ДТМИ") != std::string::npos) {
				getline(finp, line, '\n' );
				if (line.find("ТМОС") != std::string::npos) {
					fout<<"\n"<<line<<"\n";

					for (int i = 0; i < 290; i++) {
						finp>>word1>>word2>>dec_val;
						int n1 = sscanf(word1.c_str(),"[%d]", &ind);
						int n2 = sscanf(word2.c_str(),"0X%x", &hex_val);
						if ((i == ind)&&(hex_val == dec_val)) {
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

					PrintDTMI_BOKZM(fout, dtmi);

					struct CadrInfo mCadr;
					ConvertDataDTMI_BOKZM(dtmi, mCadr);
					vCadrInfo.push_back(mCadr);
					plotter->AddPoint(ChartNumLoc, 0, mCadr.Time, mCadr.CountLocalObj);
				}
			}
		}
		finp.close();
		fout.close();
		fmshi.close();
		PrepareStartDraw();
	}
}




//---------------------------------------------------------------------------
// проверяем содержит ли протокол режим локализации
bool checkLocFile(ifstream& in)
{
	std::string line;
	for(int i = 0; i < 3 ; i++)
	{
		std::getline(in,line);
	}
	// проверив, возвращаем указатель на начало файла
	in.seekg(0);

	if(line.find("Локализация") == std::string::npos)
	{
		return false;
	}
	return true;
}

template <typename Stream>
size_t findWord(Stream& in,const std::string& word)
{
  std::string lineToWrite;
  while(in >> lineToWrite)
  {
	if (lineToWrite.find(word) != std::string::npos )
	{
		return  in.tellg() - lineToWrite.size();
	}
  }


  if(in.fail() && !in.eof())
  {
	throw(string("Ошибка считывания файла"));
  }

  return std::string::npos;
}

template <typename Stream>
size_t findLine(Stream& in,const std::string& line)
{
  std::string lineToWrite;
  while(std::getline(in,lineToWrite))
  {
	if (lineToWrite.find(line) != std::string::npos )
	{
		return  in.tellg() - lineToWrite.size();
	}
  }

  if(in.fail() && !in.eof())
  {
	throw(string("Ошибка считывания файла"));
  }

  return std::string::npos;
}



vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());

	return tokens;
}





void TFormGraphOrient::readBOKZ60LocProtocol(ifstream& in,vector <CadrInfo>& cadrInfoVec)
{
	std::string line;
	string errorMessage = string("Cчитывание протокола завершено необычным образом. "
				"Возможно работа прибора была остановлена.");
	while(std::getline(in,line))
	{
		   if(line.find("Состав ДТМИ ЛОК:") != std::string::npos)
		   {
				CadrInfo cadrInfo;
				cadrInfo.CountBlock = 0;
				cadrInfo.CountStars = 0;
				// считываем время привязки
				if(findWord(in,"информации") != std::string::npos)
				{
					in >> cadrInfo.Time;
				}
				else throw std::logic_error(errorMessage);

				// ищем число локализованных объектов
				if(findWord(in,"объектов") != std::string::npos)
				{
					in >> cadrInfo.CountLocalObj;
					plotter->AddPoint(ChartNumLoc, 0,cadrInfo.Time,cadrInfo.CountLocalObj);
				}

				else throw std::logic_error(errorMessage);

				  //ищем число распознанных объектов
				if(findWord(in,"объектов") != std::string::npos)
				{
					in >> cadrInfo.CountDeterObj;
					plotter->AddPoint(ChartNumDet, 0, cadrInfo.Time,cadrInfo.CountDeterObj);
				}
				else throw std::logic_error(errorMessage);

				// ищем начало массива лок
				if(findLine(in,"	Х			Y			I			N") != std::string::npos)
				{
					vector<string> splittedLocData;
					const int сountLocObj = cadrInfo.CountLocalObj;
					ObjectsInfo objInfo;
					for(int i = 0 ; i < сountLocObj; i ++)
					{
						std::getline(in,line);
						// см. эту строку в протоколе, чтобы понять почему так
						splittedLocData = split(line, ")\t");
						splittedLocData = split(splittedLocData[1], "\t");

						objInfo.X = std::atof (splittedLocData[0].c_str());
						objInfo.Y = std::atof (splittedLocData[1].c_str());
						objInfo.Bright = std::atof (splittedLocData[2].c_str());
						objInfo.Square = abs(std::atoi (splittedLocData[3].c_str()));
						objInfo.Square = 0;
						objInfo.StarID = 0;
						objInfo.Mv = 0;
						objInfo.Sp[0]='_';
						objInfo.Sp[1]='_';
						objInfo.Dx = 0;
						objInfo.Dy = 0;

						cadrInfo.ObjectsList.push_back(objInfo);
					}


				}
				else throw std::logic_error(errorMessage);

				cadrInfo.CountBlock = 0;
				cadrInfo.CountWindows = 0;
				cadrInfo.CountStars = 0;
				cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
                cadrInfo.SizeStarsList = 0;

				cadrInfoVec.push_back(cadrInfo);
		   }

	}
}





void TFormGraphOrient::readBOKZ60Protocol(ifstream& in,vector <CadrInfo>& cadrInfoVec)
{
	std::string line;
	string errorMessage = string("Cчитывание протокола завершено необычным образом. "
				"Возможно работа прибора была остановлена.");
	while(std::getline(in,line))
	{

		if(line.find("и) Ориентация				определена") != std::string::npos)
		{
		   CadrInfo cadrInfo;
		   // ищем время привязки
			if(findWord(in,"информации") != std::string::npos)
			{
				in >> cadrInfo.Time;
			}
		   else throw std::logic_error(errorMessage);


		   if(findLine(in,"4) Кватернион ориентации, Qо") != std::string::npos)
		   {

				for(int i = 0; i < 4; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t\t\t\t\t");
					cadrInfo.QuatOrient[i] = std::atof(splittedStr[1].c_str());
				}

				double matrixOfOrientation [3][3];
				quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
				double al = 0;
				double dl = 0;
				double Az = 0;

				dl = asinm(matrixOfOrientation[2][2]) * RTD;
				al = atan2m(matrixOfOrientation[2][1], matrixOfOrientation[2][0])*RTD;   if (al < 0)  al += 360.;
				Az = atan2m(matrixOfOrientation[0][2], matrixOfOrientation[1][2])*RTD;   if (Az < 0)  Az += 360.;

				plotter->AddPoint(ChartAl, 0, cadrInfo.Time, al);
				plotter->AddPoint(ChartDl, 0, cadrInfo.Time, al);
				plotter->AddPoint(ChartAz, 0, cadrInfo.Time, al);

		   }
		   else throw std::logic_error(errorMessage);


		   if(findWord(in,"объектов") != std::string::npos)
		   {
				in >> cadrInfo.CountLocalObj;
				plotter->AddPoint(ChartNumLoc, 0, cadrInfo.Time, cadrInfo.CountLocalObj);

		   }
		   else throw std::logic_error(errorMessage);


		   //ищем число распознанных объектов
		   if(findWord(in,"объектов") != std::string::npos)
		   {
				in >> cadrInfo.CountDeterObj;
				plotter->AddPoint(ChartNumDet, 0, cadrInfo.Time, cadrInfo.CountDeterObj);
		   }
		   else throw std::logic_error(errorMessage);

		   // ищем начало массива лок
		   if(findLine(in,"	Х			Y			I			N") != std::string::npos)
		   {
				vector<string> splittedLocData;
				const int сountLocObj = cadrInfo.CountLocalObj;
				ObjectsInfo objInfo;

				for(int i = 0 ; i < сountLocObj; i ++)
				{
						std::getline(in,line);
						splittedLocData = split(line,")\t");
						splittedLocData = split(splittedLocData[1],"\t\t");

						objInfo.X = std::atof (splittedLocData[0].c_str());
						objInfo.Y = std::atof (splittedLocData[1].c_str());
						objInfo.Bright = std::atof(splittedLocData[2].c_str());
						objInfo.Square = 0;
						objInfo.Square = abs(std::atoi (splittedLocData[3].c_str()));
						objInfo.StarID = 0;
						objInfo.Mv = 0;
						objInfo.Sp[0]='_';
						objInfo.Sp[1]='_';
						objInfo.Dx = 0;
						objInfo.Dy = 0;
						cadrInfo.ObjectsList.push_back(objInfo);
				}
		   }
			else throw std::logic_error(errorMessage);

		   if(findLine(in,"14) Проекции угловой скорости на оси ПСК") != std::string::npos)
		   {
				for(int i = 0; i < 3; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.OmegaOrient[i] = std::atof(splittedStr[1].c_str());
				}
				plotter->AddPoint(ChartWx, 0, cadrInfo.Time , cadrInfo.OmegaOrient[0] * RTS);
				plotter->AddPoint(ChartWy, 0, cadrInfo.Time , cadrInfo.OmegaOrient[1] * RTS);
				plotter->AddPoint(ChartWz, 0, cadrInfo.Time , cadrInfo.OmegaOrient[2] * RTS);

		   }
		   else throw std::logic_error(errorMessage);


		   if(findLine(in,"15) Координаты центров фрагментов") != std::string::npos)
		   {
				WindowsInfo winInfo;
				const int maxCountOfObjects = 16;
				cadrInfo.CountWindows = maxCountOfObjects;
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t");

					if(std::atoi(splittedStr[1].c_str()) == 0)
					{
						cadrInfo.CountWindows = i;
						break;
					}

					winInfo.Xstart = (std::atof(splittedStr[1].c_str()));
					winInfo.Ystart = (std::atof(splittedStr[2].c_str()));
					winInfo.Mean = 0;
					winInfo.Sigma = 0;
					winInfo.Mv = 0;
					cadrInfo.WindowsList.push_back(winInfo);
				}
				plotter->AddPoint(ChartNumFrag, 0, cadrInfo.Time,cadrInfo.CountWindows);




		   }
		  else throw std::logic_error(errorMessage);


		   if(findLine(in,"16) Значение порогов во фрагментах") != std::string::npos)
		   {
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.WindowsList[i].Level = std::atoi(splittedStr[1].c_str());
				}

		   }
		  else throw std::logic_error(errorMessage);

		   
		   if(findLine(in,"17) Количество объектов во фрагментах") != std::string::npos)
		   {
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.WindowsList[i].CountObj = std::atoi(splittedStr[1].c_str());
				}

		   }
		   else throw std::logic_error(errorMessage);

		   cadrInfo.CountBlock = 0;
		   cadrInfo.CountStars = 0;
		   cadrInfo.SizeStarsList = 0;
		   cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
		   cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();

		   cadrInfoVec.push_back(cadrInfo);
		}

	}

}





void __fastcall TFormGraphOrient::BOKZ60ParseProtocolClick(TObject *Sender)
{

	try
	{
        OpenDialog->Options.Clear();
		OpenDialog->Filter = "txt|*.txt";
		if (OpenDialog->Execute()) {
			vCadrInfo.clear();
			FileName = OpenDialog->FileName;
			SetCurrentDir(ExtractFileDir(FileName));

			ifstream in(FileName.c_str());
			if (!in.is_open()) {
				ShowMessage("Не удалось открыть файл");
				return;
			}
			DeleteLineGraph();

			if(checkLocFile(in))
			{
				readBOKZ60LocProtocol(in,vCadrInfo);
			}

			else
			{
				readBOKZ60Protocol(in,vCadrInfo);
			}

			PrepareStartDraw();

		}
	}

	catch(std::exception &e)
	{
		ShowMessage(e.what());
	}
}




void TFormGraphOrient::readmBOKZ2VProtocol(ifstream& in,vector <CadrInfo>& cadrInfoVec)
{
    std::string line;
	string errorMessage = string("Cчитывание протокола завершено необычным образом. "
				"Возможно работа прибора была остановлена.");


	while(std::getline(in,line))
	{

		if(line.find("2 Число принятых пикселей на первом кадре") != std::string::npos)
		{
			// если нашли строку с числом принятых пикселей, проверяем, что их число не ноль
			// если ноль, пропускаем такт
			if(split(line, "\t").back() == "0")
			{
				continue;
			}
		   CadrInfo cadrInfo;
		   cadrInfo.ImageHeight = 1024;
		   cadrInfo.ImageWidth = 1024;

			// ищем начало массива лок и фрагментов
		   if(findLine(in,"18, 19	Массив локализованных объектов на 1-ом кадре") != std::string::npos)
		   {
				vector <string> splittedStr;
				const int maxCountLocObj = 15;
				ObjectsInfo objInfo;
				WindowsInfo winInfo;
				StarsInfo starsInfo;

				for(int i = 0 ; i < maxCountLocObj; i ++)
				{
						std::getline(in,line);
						splittedStr = split(line,"\t");

						// если всё-таки объектов меньше
						if(std::atof (splittedStr[0].c_str()) == 0)
						{
							  break;
						}
						// заполняем все о лок
						objInfo.X = std::atof (splittedStr[0].c_str());
						objInfo.Y = std::atof (splittedStr[1].c_str());
						objInfo.Bright = std::atof(splittedStr[2].c_str());
						objInfo.Square = std::atoi (splittedStr[3].c_str());
						objInfo.StarID = 0;
						objInfo.Mv = 0;
						objInfo.Sp[0]='_';
						objInfo.Sp[1]='_';
						objInfo.Dx = 0;
						objInfo.Dy = 0;
						cadrInfo.ObjectsList.push_back(objInfo);

						// заполняем всё о фрагментах
						winInfo.Mv = 0;
						winInfo.Mean = (std::atof(splittedStr[6].c_str()));
						winInfo.Sigma = (std::atof(splittedStr[7].c_str()));
						winInfo.Level = (std::atof(splittedStr[8].c_str()));
						winInfo.CountObj = std::atoi(splittedStr[9].c_str());
						unsigned short windowSize = std::atoi(splittedStr[10].c_str());

						switch(windowSize)
						{
						case 15:
							winInfo.Width = 24;
							winInfo.Height = 24;
							break;
						case 14:
							winInfo.Width = 24;
							winInfo.Height = 48;
							break;
						case 11:
							winInfo.Width = 48;
							winInfo.Height = 24;
							break;
						case 10:
							winInfo.Width = 48;
							winInfo.Height = 48;
							break;
						}
						winInfo.Xstart = (std::atof(splittedStr[4].c_str())) - winInfo.Width/2;
						winInfo.Ystart = (std::atof(splittedStr[5].c_str())) - winInfo.Height/2;
						cadrInfo.WindowsList.push_back(winInfo);

						starsInfo.X = (std::atof(splittedStr[4].c_str()));
						starsInfo.Y = (std::atof(splittedStr[5].c_str()));
						cadrInfo.StarsList.push_back(starsInfo);

				}

			if(findLine(in,"18, 19	Массив локализованных объектов на 2-ом кадре") != std::string::npos)
		   {
				for(int i = 0 ; i < maxCountLocObj; i ++)
				{
					std::getline(in,line);
					splittedStr = split(line,"\t");

					// если всё-таки объектов меньше
					if(std::atof (splittedStr[0].c_str()) == 0)
					{
						break;
					}
						// заполняем все о лок
					objInfo.X = std::atof (splittedStr[0].c_str());
					objInfo.Y = std::atof (splittedStr[1].c_str());
					objInfo.Bright = std::atof(splittedStr[2].c_str());
					objInfo.Square = std::atoi (splittedStr[3].c_str());
					objInfo.StarID = 0;
					objInfo.Mv = 0;
					objInfo.Sp[0] = '_';
					objInfo.Sp[1] = '_';
					objInfo.Dx = 0;
					objInfo.Dy = 0;
					cadrInfo.ObjectsList.push_back(objInfo);
				}
		   }


		   }
			else throw std::logic_error(errorMessage);

			cadrInfo.SizeWindowsList = cadrInfo.WindowsList.size();
			cadrInfo.SizeObjectsList = cadrInfo.ObjectsList.size();
			cadrInfo.SizeStarsList = cadrInfo.StarsList.size();
			GetImageBright(cadrInfo);

		   // ищем время привязки в секундах
			if(findWord(in, "информации") != std::string::npos)
			{
				int secs = 0;
				int msecs = 0;
				int prevPos = in.tellg();

				in>>line;
				if(line == "с")
				{
					in >> secs;

					std::getline(in, line);
					std::getline(in, line);

					vector <string> splittedStr = split(line, "\t");
					msecs =  std::atoi (splittedStr[1].c_str());
				}

				else
				{
					// если там не "c", возвращаемся назад
					in.seekg(prevPos);
					in >> secs;
					// ищем миллисекунды
					findWord(in, "информации");
					in >> msecs;

				}
				cadrInfo.Time =  static_cast <double> (secs) + static_cast <double> (msecs) / 1000;
			}
		   else throw std::logic_error(errorMessage);

		   if(findLine(in,"6) Кватернион ориентации, Qо") != std::string::npos)
		   {

				for(int i = 0; i < 4; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t\t\t\t");
					cadrInfo.QuatOrient[i] = std::atof(splittedStr[1].c_str());
				}

				double matrixOfOrientation [3][3];
				quatToMatr(cadrInfo.QuatOrient, matrixOfOrientation);
				double al = 0;
				double dl = 0;
				double Az = 0;

				dl = asinm(matrixOfOrientation[2][2]) * RTD;
				al = atan2m(matrixOfOrientation[2][1], matrixOfOrientation[2][0]) * RTD;   if (al<0)  al += 360.;
				Az = atan2m(matrixOfOrientation[0][2], matrixOfOrientation[1][2]) * RTD;   if (Az<0)  Az += 360.;

				plotter->AddPoint(ChartAl, 0, cadrInfo.Time, al);
				plotter->AddPoint(ChartDl, 0, cadrInfo.Time, dl);
				plotter->AddPoint(ChartAz, 0, cadrInfo.Time, Az);

		   }
		   else throw std::logic_error(errorMessage);



		   if(findLine(in,"Угловая скорость по оптическим измерениям в проекциях на оси ПСК") != std::string::npos)
		   {
				for(int i = 0; i < 3; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t\t\t\t");
					cadrInfo.OmegaOrient[i] = std::atof(splittedStr[1].c_str());
				}
				plotter->AddPoint(ChartWx, 0, cadrInfo.Time, cadrInfo.OmegaOrient[0] * RTS);
				plotter->AddPoint(ChartWy, 0, cadrInfo.Time, cadrInfo.OmegaOrient[1] * RTS);
				plotter->AddPoint(ChartWz, 0, cadrInfo.Time, cadrInfo.OmegaOrient[2] * RTS);

		   }
		   else throw std::logic_error(errorMessage);


		   if(findWord(in,"Tcmv") != std::string::npos)
		   {
				double matrixTemperature = 0;
				in >> matrixTemperature;
				plotter->AddPoint(ChartTemp, 0, cadrInfo.Time, matrixTemperature);
		   }
		   else throw std::logic_error(errorMessage);

		   // ищем число спроектированных звезд
		   if(findWord(in,"NumProgFrag") != std::string::npos)
		   {
				in >> cadrInfo.CountStars;
				if(cadrInfo.CountStars <= 0) continue;
		   }
		   else throw std::logic_error(errorMessage);

		   // ищем число фрагментов
		   if(findWord(in,"NumFrag") != std::string::npos)
		   {
				// общее число фрагментов
				in >> cadrInfo.CountWindows;
				if(cadrInfo.CountWindows <= 0) continue;
				plotter->AddPoint(ChartNumFrag, 0, cadrInfo.Time, cadrInfo.CountWindows);
		   }
		   else throw std::logic_error(errorMessage);


		   // число локализ. объектов
		   if(findWord(in,"NumLoc[0]") != std::string::npos)
		   {
				in >> cadrInfo.CountLocalObj;
				if(cadrInfo.CountLocalObj <= 0) continue;
				plotter->AddPoint(ChartNumLoc, 0, cadrInfo.Time, cadrInfo.CountLocalObj);
		   }
		   else throw std::logic_error(errorMessage);


		   //ищем число распознанных объектов
		   if(findWord(in,"NumDet") != std::string::npos)
		   {
				in >> cadrInfo.CountDeterObj;
				if(cadrInfo.CountDeterObj <= 0) continue;
				plotter->AddPoint(ChartNumDet, 0, cadrInfo.Time, cadrInfo.CountDeterObj);
		   }
		   else throw std::logic_error(errorMessage);



		   if(findWord(in,"m_cur") != std::string::npos)
		   {
				double m_cur = 0;
				in >> m_cur;
				plotter->AddPoint(ChartMxy, 0, cadrInfo.Time, m_cur);
				plotter->AddPoint(ChartFone, 0, cadrInfo.Time, cadrInfo.MeanBright);
				plotter->AddPoint(ChartNoise, 0, cadrInfo.Time, cadrInfo.SigmaBright);
		   }
		   else throw std::logic_error(errorMessage);

		   cadrInfo.CountBlock = 0;
		   cadrInfo.CountLines = 0;
		   cadrInfoVec.push_back(cadrInfo);
		}

	}

}



//---------------------------------------------------------------------------


void __fastcall TFormGraphOrient::BOKZM2VParseProtocolClick(TObject *Sender)
{
	try
	{
         OpenDialog->Options.Clear();
		 OpenDialog->Filter = "txt|*.txt";
		if (OpenDialog->Execute()) {
			vCadrInfo.clear();
			FileName = OpenDialog->FileName;
			SetCurrentDir(ExtractFileDir(FileName));

			ifstream in(FileName.c_str());
			if (!in.is_open()) {
				ShowMessage("Не удалось открыть файл");
				return;
			}

			DeleteLineGraph();
			readmBOKZ2VProtocol(in, vCadrInfo);
			PrepareStartDraw();

		}
	}

	catch (std::exception &e)
	{
		ShowMessage(e.what());
	}
}


void convertIKIFormatToInfoCadr (IKI_img* reader, vector <CadrInfo>& cadrInfoVec, bool CompareIKIRes = false)
{
	CadrInfo cadrInfo;
	cadrInfo.Time = reader->Georeferencing.DateTime.Val;
	cadrInfo.FrameNumber = reader->Georeferencing.FrameNumber;
	cadrInfo.IsBinary = reader->ImageData.FrameData.DegreeBinning;
	cadrInfo.DataType = reader->ImageData.FrameData.DataType;
	cadrInfo.IsReverse = false;
	cadrInfo.IsOrient = !reader->StarsData.RezStat;
	cadrInfo.CountPixFilter = reader->FilterData.FilteredPixelsCount;
	cadrInfo.ImageHeight = reader->ImageData.FrameData.FrameHeight;
	cadrInfo.ImageWidth = reader->ImageData.FrameData.FrameWidth;
	cadrInfo.StatOrient = reader->StarsData.RezStat;
	cadrInfo.CountStars = reader->StarsData.SimulatedFrame.strrec;;
	cadrInfo.CountWindows = reader->ImageData.WindowsData.WindowCount;
	cadrInfo.CountLocalObj = reader->StarsData.LocalizedCount;
	cadrInfo.CountDeterObj = reader->StarsData.RecognizedCount;
	cadrInfo.SizeStarsList = reader->StarsData.SimulatedFrame.strrec;
	cadrInfo.SizeObjectsList =  cadrInfo.CountLocalObj;
//	cadrInfo.SizeObjectsList =  cadrInfo.CountDeterObj;
	cadrInfo.SizeWindowsList = cadrInfo.CountWindows;


	for (int i = 0; i < cadrInfo.SizeStarsList; i++)
	{
		if (reader->StarsData.SimulatedFrame.StarRec[i].Xs == 0
		&&  reader->StarsData.SimulatedFrame.StarRec[i].Ys == 0)
		{
		   cadrInfo.SizeStarsList = i + 1;
		   break;
		}
		StarsInfo starInfo;
		starInfo.X = reader->StarsData.SimulatedFrame.StarRec[i].Xs;
		starInfo.Y = reader->StarsData.SimulatedFrame.StarRec[i].Ys;
		starInfo.Bright = reader->StarsData.SimulatedFrame.StarRec[i].Is;
		starInfo.StarID = reader->StarsData.SimulatedFrame.StarRec[i].Ns;
		starInfo.Mv = reader->StarsData.SimulatedFrame.StarRec[i].Mv;
		starInfo.Sp[0] = reader->StarsData.SimulatedFrame.StarRec[i].Sp[0];
		starInfo.Sp[1] = reader->StarsData.SimulatedFrame.StarRec[i].Sp[1];
		cadrInfo.StarsList.push_back(starInfo);
	}

	for (int i = 0; i < cadrInfo.SizeObjectsList; i ++)
	{
		if(reader->StarsData.StarsList[i].X_coordinate == 0
		&& reader->StarsData.StarsList[i].Y_coordinate == 0)
		{
		   cadrInfo.SizeObjectsList = i + 1;
		   break;
		}

		ObjectsInfo objInfo;
		objInfo.X = reader->StarsData.StarsList[i].X_coordinate;
		objInfo.Y = reader->StarsData.StarsList[i].Y_coordinate;
		objInfo.Square = reader->StarsData.StarsList[i].PixelsCount;
		objInfo.Bright = reader->StarsData.StarsList[i].BrightnessObject;
		objInfo.StarID = reader->StarsData.StarsList[i].NumberStar;
		objInfo.Mv = reader->StarsData.StarsList[i].StellarMagnitude;
		objInfo.Sp[0] = reader->StarsData.StarsList[i].SpectralClass[0];
		objInfo.Sp[1] = reader->StarsData.StarsList[i].SpectralClass[1];
		objInfo.Dx = reader->StarsData.StarsList[i].DX;
		objInfo.Dy = reader->StarsData.StarsList[i].DY;
		cadrInfo.ObjectsList.push_back(objInfo);
	}

	for (int i = 0; i < cadrInfo.SizeWindowsList; i ++)
	{

		if(reader->ImageData.WindowsData.Info[i].X == 0
		&& reader->ImageData.WindowsData.Info[i].Y == 0)
		{
		   cadrInfo.SizeWindowsList = i + 1;
		   break;
		}
		 WindowsInfo winInfo;
		 winInfo.Xstart = reader->ImageData.WindowsData.Info[i].X;
		 winInfo.Ystart = reader->ImageData.WindowsData.Info[i].Y;
		 winInfo.Width = reader->ImageData.WindowsData.Info[i].WindowWidth;
		 winInfo.Height = reader->ImageData.WindowsData.Info[i].WindowHeight;
		 winInfo.CountObj = reader->ImageData.WindowsData.Info[i].ObjCount;
		 winInfo.Mean =  reader->ImageData.WindowsData.Info[i].Average;
		 winInfo.Sigma =  reader->ImageData.WindowsData.Info[i].SKO;
		 winInfo.Level =  reader->ImageData.WindowsData.Info[i].Limit;
		 winInfo.Mv = 0;
		 winInfo.ZipX = reader->ImageData.WindowsData.Info[i].ZipX;
		 winInfo.ZipY = reader->ImageData.WindowsData.Info[i].ZipY;
		 winInfo.StarID = 0;
		 winInfo.Sp[0] = 0;
		 winInfo.Sp[1] = 0;
		 cadrInfo.WindowsList.push_back(winInfo);
	}

	if (reader->ImageData.WindowsData.SizeData != 0) {
		
		std::string dirName = AnsiString(GetCurrentDir()).c_str() + std::string("/") + "Frag" + "_" + "IKI";
		TDirectory::CreateDirectory(dirName.c_str());
		std::string time = AnsiString(FloatToStr(cadrInfo.Time)).c_str();
		std::string fileName = dirName + "/" + "Frag" + "_" + time + ".bin";
		std::ofstream fragmentFile(fileName.c_str(), std::ios::binary | std::ios::trunc);

		if(fragmentFile.is_open())
		{
			char bytesInFormat = 1;
			switch (cadrInfo.DataType)
			{
				case 0: break;
				case 1: bytesInFormat = 2; break;
				case 2: bytesInFormat = 4; break;
			}

		  fragmentFile.write(
		  (char*)(reader->ImageData.WindowsData.Data),
		   reader->ImageData.WindowsData.SizeData * bytesInFormat);
		  fragmentFile.close();
		}
	}

	cadrInfo.CountLines = reader->ImageData.LinesData.LinesHeight;
	cadrInfo.CountBlock = reader->ImageData.LinesData.LinesCount;

	for (int i = 0; i < cadrInfo.CountBlock; i ++)
	{
		LinesInfo lineInfo;
		lineInfo.Start = reader->ImageData.LinesData.Info[i].Y_FirstString;
		lineInfo.Height = reader->ImageData.LinesData.Info[i].CountString;
		cadrInfo.LinesList.push_back(lineInfo);
	}

	cadrInfo.MeanErrorX = reader->StarsData.m_X;
	cadrInfo.MeanErrorY = reader->StarsData.m_Y;
	cadrInfo.MeanErrorXY = reader->StarsData.m_Cur;

	for (int i = 0; i < 4; i ++)
	{
		cadrInfo.QuatOrient[i] = reader->StarsData.RecognizedOrientationQuaternion[i];
	}

	for (int i = 0; i < 3; i ++)
	{
		cadrInfo.AnglesOrient[i] = reader->StarsData.RecognizedOrientationAngles[i];
		cadrInfo.AnglesModel[i] = reader->Georeferencing.OrientationAngles[i];
		cadrInfo.OmegaOrient[i] = reader->StarsData.RecognizedAngularVelocity[i];
		cadrInfo.OmegaModel[i] = reader->Georeferencing.DeviceAngularVelocity[i];

		for (int j = 0; j < 3; j ++)
		{
			cadrInfo.MatrixOrient[i][j] = reader->StarsData.RecognizedOrientationMatrix[i][j];
		}
	}

	if (CompareIKIRes)
	{
		for (int i = 0; i < 3; i++)
		{
			cadrInfo.OmegaDiff[i] = reader->StarsData.RecognizedAngularVelocity[i]
			- reader->Georeferencing.DeviceAngularVelocity[i];
		}

		for (int i = 0;  i < 3; i++)
		{
			double diff = cadrInfo.AnglesOrient[i] - reader->Georeferencing.OrientationAngles[i];
			if (abs(diff) > 5)        // потом убрать
			{
				diff = (cadrInfo.AnglesOrient[i] + reader->Georeferencing.OrientationAngles[i])
				-	abs(cadrInfo.AnglesOrient[i] - reader->Georeferencing.OrientationAngles[i]);
			}
			cadrInfo.AnglesDiff[i] = diff;
		}

		for (int i = 0; i < 3; i++)
		{
			cadrInfo.AxesDiff[i] = GetAxisAngle(&reader->Georeferencing.OrientationMatrix[i][0],
                                                &cadrInfo.MatrixOrient[i][0]);
		}
	}

	GetImageBright(cadrInfo);
	cadrInfoVec.push_back(cadrInfo);
}

void __fastcall TFormGraphOrient::ReadIKIFormatClick(TObject *Sender)
{
	try
	{
		LabelFrameReport->Caption = "HELLO";
		OpenDialog->Filter = "iki|*.iki";
		OpenDialog->Options << ofAllowMultiSelect;
		if (OpenDialog->Execute())
		{
			std::unique_ptr <TStringList> FileList (new TStringList());
			FileList->Assign(OpenDialog->Files);
			OpenDialog->Filter = "res|*.res";
			if (OpenDialog->Execute())
			{
				plotter->ResetOptions();
				DeleteLineGraph();
				vCadrInfo.clear();
				FileTitle = "IKI";
				FileList->AddStrings(OpenDialog->Files);
				FileList->Sort();
				SetCurrentDir(ExtractFileDir(FileList->Strings[0]));

				for (int i = 0; i < FileList->Count; i ++)
				{
					std::unique_ptr <IKI_img> reader(new IKI_img());
					if	(reader->ReadFormat(FileList->Strings[i], false))
					{
						if	(i != FileList->Count - 1
						&& AnsiContainsStr(FileList->Strings[i + 1], ".res"))
						{
							if (reader->ReadFormat(FileList->Strings[i + 1], false))
							{
								CompareIKIRes = true;
								i = i + 1;
							}
							else throw logic_error(string("Не удалось считать ") + AnsiString(FileList->Strings[i + 1]).c_str());
						}

						convertIKIFormatToInfoCadr(reader.get(), vCadrInfo, CompareIKIRes);
						double Time =  vCadrInfo.back().Time;

						if (vCadrInfo.back().IsOrient)
						{
							plotter->SetTitle("измерения");
							plotter->AddPoint(ChartMx, 0, Time, vCadrInfo.back().MeanErrorX * 1000.);
							plotter->AddPoint(ChartMy, 0, Time, vCadrInfo.back().MeanErrorY * 1000.);
							plotter->AddPoint(ChartMxy, 0, Time, vCadrInfo.back().MeanErrorXY * 1000.);
							plotter->AddPoint(ChartNumFrag, 0, Time, vCadrInfo.back().CountWindows);
							plotter->AddPoint(ChartNumLoc, 0, Time, vCadrInfo.back().CountLocalObj);
							plotter->AddPoint(ChartNumDet, 0, Time, vCadrInfo.back().CountDeterObj);
							plotter->AddPoint(ChartFone, 0, Time, vCadrInfo.back().MeanBright);
							plotter->AddPoint(ChartNoise, 0, Time, vCadrInfo.back().SigmaBright);
							//plotter->AddPoint(ChartTemp, 0, Time, vCadrInfo.back().MatrixTemp);
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
						}
						if (i % 100 == 0)
						{
							Application->ProcessMessages();
							LabelFrameReport->Caption = "Cчитано " + IntToStr(i / 2) + " файлов из " + IntToStr(FileList->Count / 2);
						}
					}
					else throw logic_error(string("Не удалось считать ") + AnsiString(FileList->Strings[i]).c_str());


				}
				struct {

					bool operator()(const CadrInfo& a,const CadrInfo& b)
					{
						return a.Time < b.Time;
					}

				} CadrCompare ;

				std::sort(vCadrInfo.begin(), vCadrInfo.end(), CadrCompare);
				PrepareStartDraw();
			}

		}
	}
	catch (std::exception &e)
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

 // перенести
template <class InputIterator, class Value, class UnaryOperation>
std::pair<Value, Value> calculateMeanStdDv (InputIterator first, InputIterator last, Value init, UnaryOperation extractWtC)
{
	if (first == last) return std::pair<Value, Value>(extractWtC(*first), Value());

	Value dispersio = 0;
	for (InputIterator i = first;i < last;i ++)
	{
		init += extractWtC(*i);
		dispersio += pow(extractWtC(*i), 2);
	}
	auto count = std::distance(first,last);
	Value mean = init / count;
	dispersio = (dispersio / count) - pow(mean, 2);

	return std::pair <Value,Value> (mean, sqrt(dispersio));
}


void __fastcall TFormGraphOrient::ErrorAnalyzeClick(TObject *Sender)
{
	FileOpenDialog1->Options << fdoPickFolders << fdoAllowMultiSelect;
	if (FileOpenDialog1->Execute())
	{
		TPointSeries* seriesXMax = new TPointSeries(ChartAnalyzeXV);
		seriesXMax->Title = "Максимум";
		TPointSeries* seriesYMax = new TPointSeries(ChartAnalyzeYV);
		seriesYMax->Title = "Максимум";
		TPointSeries* seriesZMax = new TPointSeries(ChartAnalyzeZV);
		seriesZMax->Title = "Максимум";
		TPointSeries* seriesAlMax = new TPointSeries(ChartAnalyzeErrorAl);
		seriesAlMax->Title = "Максимум";
		TPointSeries* seriesDlMax = new TPointSeries(ChartAnalyzeErrorDl);
		seriesDlMax->Title = "Максимум";
		TPointSeries* seriesAzMax = new TPointSeries(ChartAnalyzeErrorAz);
		seriesAzMax->Title = "Максимум";

		TPointSeries* seriesXMin = new TPointSeries(ChartAnalyzeXV);
		seriesXMin->Title = "Минимум";
		TPointSeries* seriesYMin = new TPointSeries(ChartAnalyzeYV);
		seriesYMin->Title = "Минимум";
		TPointSeries* seriesZMin = new TPointSeries(ChartAnalyzeZV);
		seriesZMin->Title = "Минимум";
		TPointSeries* seriesAlMin = new TPointSeries(ChartAnalyzeErrorAl);
		seriesAlMin->Title = "Минимум";
		TPointSeries* seriesDlMin = new TPointSeries(ChartAnalyzeErrorDl);
		seriesDlMin->Title = "Минимум";
		TPointSeries* seriesAzMin = new TPointSeries(ChartAnalyzeErrorAz);
		seriesAzMin->Title = "Минимум";

		TPointSeries* seriesXSKO = new TPointSeries(ChartAnalyzeXV);
		seriesXSKO->Title = "СКО";
		TPointSeries* seriesYSKO = new TPointSeries(ChartAnalyzeYV);
		seriesYSKO->Title = "СКО";
		TPointSeries* seriesZSKO = new TPointSeries(ChartAnalyzeZV);
		seriesZSKO->Title = "СКО";
		TPointSeries* seriesAlSKO = new TPointSeries(ChartAnalyzeErrorAl);
		seriesAlSKO->Title = "СКО";
		TPointSeries* seriesDlSKO = new TPointSeries(ChartAnalyzeErrorDl);
		seriesDlSKO->Title = "СКО";
		TPointSeries* seriesAzSKO = new TPointSeries(ChartAnalyzeErrorAz);
		seriesAzSKO->Title = "СКО";

		TPointSeries* seriesXMean =  new TPointSeries(ChartAnalyzeXV);
		seriesXMean->Title = "Среднее";
		TPointSeries* seriesYMean =  new TPointSeries(ChartAnalyzeYV);
		seriesYMean->Title = "Среднее";
		TPointSeries* seriesZMean =  new TPointSeries(ChartAnalyzeZV);
		seriesZMean->Title = "Среднее";
		TPointSeries* seriesAlMean = new TPointSeries(ChartAnalyzeErrorAl);
		seriesAlMean->Title = "Среднее";
		TPointSeries* seriesDlMean = new TPointSeries(ChartAnalyzeErrorDl);
		seriesDlMean->Title = "Среднее";
		TPointSeries* seriesAzMean = new TPointSeries(ChartAnalyzeErrorAz);
		seriesAzMean->Title = "Среднее";

		ChartAnalyzeXV->AddSeries(seriesXMax);
		ChartAnalyzeXV->AddSeries(seriesXMin);
		ChartAnalyzeXV->AddSeries(seriesXSKO);
		ChartAnalyzeXV->AddSeries(seriesXMean);
		ChartAnalyzeYV->AddSeries(seriesYMax);
		ChartAnalyzeYV->AddSeries(seriesYMin);
		ChartAnalyzeYV->AddSeries(seriesYSKO);
		ChartAnalyzeYV->AddSeries(seriesYMean);
		ChartAnalyzeZV->AddSeries(seriesZMax);
		ChartAnalyzeZV->AddSeries(seriesZMin);
		ChartAnalyzeZV->AddSeries(seriesZSKO);
		ChartAnalyzeZV->AddSeries(seriesZMean);
		ChartAnalyzeErrorAl->AddSeries(seriesAlMax);
		ChartAnalyzeErrorAl->AddSeries(seriesAlMin);
		ChartAnalyzeErrorAl->AddSeries(seriesAlSKO);
		ChartAnalyzeErrorAl->AddSeries(seriesAlMean);
		ChartAnalyzeErrorDl->AddSeries(seriesDlMax);
		ChartAnalyzeErrorDl->AddSeries(seriesDlMin);
		ChartAnalyzeErrorDl->AddSeries(seriesDlSKO);
		ChartAnalyzeErrorDl->AddSeries(seriesDlMean);
		ChartAnalyzeErrorAz->AddSeries(seriesAzMax);
		ChartAnalyzeErrorAz->AddSeries(seriesAzMin);
		ChartAnalyzeErrorAz->AddSeries(seriesAzSKO);
		ChartAnalyzeErrorAz->AddSeries(seriesAzMean);

		std::unique_ptr <IKI_img> reader(new IKI_img());
		for (int i = 0; i < FileOpenDialog1->Files->Count; i ++)
		{
			
			TStringDynArray FileNameList = TDirectory::GetFiles(FileOpenDialog1->Files->Strings[i]);
			std::unique_ptr <TStringList> FileList (new TStringList());
			for (int fileIndex = FileNameList.Low; fileIndex < FileNameList.High; fileIndex ++)
			{
				FileList->Add(FileNameList[fileIndex]);
			} 

			typedef System::Math::Vectors::TPoint3D Point;
			vector <Point> AngularSpeedErrorValues;
			vector <Point> AngleErrorValues;
			
			for (int j = 0; j < FileList->Count; j ++)
			{
			   if (reader->ReadFormat(FileList->Strings[j], false))
			   {

					if	(j != FileList->Count - 1
					&& AnsiContainsStr(FileList->Strings[j + 1], ".res"))
					{
						if (reader->ReadFormat(FileList->Strings[j + 1], false))
						{
							j = j + 1;
						}
						else throw logic_error(string("Не удалось считать ") + AnsiString(FileList->Strings[i + 1]).c_str());

						AngularSpeedErrorValues.push_back(
						Point::Create(reader->StarsData.RecognizedAngularVelocity[0] - reader->Georeferencing.DeviceAngularVelocity[0],
						reader->StarsData.RecognizedAngularVelocity[1] - reader->Georeferencing.DeviceAngularVelocity[1],
						reader->StarsData.RecognizedAngularVelocity[2] - reader->Georeferencing.DeviceAngularVelocity[2]));

						AngleErrorValues.push_back(
						Point::Create(reader->StarsData.RecognizedOrientationAngles[0] - reader->Georeferencing.OrientationAngles[0],
						reader->StarsData.RecognizedOrientationAngles[1] - reader->Georeferencing.OrientationAngles[1],
						reader->StarsData.RecognizedOrientationAngles[2] - reader->Georeferencing.OrientationAngles[2]));	
					}
					
			   }
			   else throw logic_error(string("Не удалось считать ") + AnsiString(FileList->Strings[i + 1]).c_str());
			}


				 int gradus, minutes, seconds;
				 ToGMS (reader->StarsData.RecognizedAngularVelocity[0] * RTD, gradus, minutes, seconds);
				 UnicodeString Label = FloatToStr(gradus) + "г." + FloatToStr(minutes) + "м." + FloatToStr(seconds) + "c.";

				 struct { bool operator()(const Point& a,const Point& b) { return a.X < b.X;} } ComparePointX;
				 struct { float operator() (const Point& a) {return a.X;} } AddX;

				 vector <Point>::iterator Max = std::max_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointX);
				 vector <Point>::iterator Min = std::min_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointX);
				 std::pair <float, float> MeanSkoX = calculateMeanStdDv(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(),0.0, AddX);
				 seriesXMax->AddXY(i, Max->X * RTS, Label);
				 seriesXMin->AddXY(i, Min->X * RTS, Label);
				 seriesXSKO->AddXY(i, MeanSkoX.second * RTS, Label);
				 seriesXMean->AddXY(i, MeanSkoX.first * RTS, Label);

				 Max = std::max_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointX);
				 Min = std::min_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointX);
				 MeanSkoX = calculateMeanStdDv(AngleErrorValues.begin(), AngleErrorValues.end(),0.0, AddX);
				 seriesAlMax->AddXY(i, Max->X * RTS, Label);
				 seriesAlMin->AddXY(i, Min->X * RTS, Label);
				 seriesAlSKO->AddXY(i, MeanSkoX.second * RTS, Label);
				 seriesAlMean->AddXY(i, MeanSkoX.first * RTS, Label);

				 struct { bool operator()(const Point& a,const Point& b) { return a.Y < b.Y; } } ComparePointY;
				 struct { float operator() (const Point& a) { return a.Y;} } AddY;

				 Max = std::max_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointY);
				 Min = std::min_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointY);
				 std::pair <float, float> MeanSkoY = calculateMeanStdDv(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(),0.0, AddY);
				 seriesYMax->AddXY(i, Max->X * RTS, Label);
				 seriesYMin->AddXY(i, Min->X * RTS, Label);
				 seriesYSKO->AddXY(i, MeanSkoY.second * RTS, Label);
				 seriesYMean->AddXY(i, MeanSkoY.first * RTS, Label);

				 Max = std::max_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointY);
				 Min = std::min_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointY);
				 MeanSkoY = calculateMeanStdDv(AngleErrorValues.begin(), AngleErrorValues.end(),0.0, AddY);
				 seriesDlMax->AddXY(i, Max->X * RTS, Label);
				 seriesDlMin->AddXY(i, Min->X * RTS, Label);
				 seriesDlSKO->AddXY(i, MeanSkoY.second * RTS, Label);
				 seriesDlMean->AddXY(i, MeanSkoY.first * RTS, Label);

				 struct { bool operator() (const Point& a,const Point& b) { return a.Z < b.Z; } } ComparePointZ;
				 struct { float operator() (const Point& a) { return a.Z;} } AddZ;
				
				 Max = std::max_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointZ);
				 Min = std::min_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointZ);
				 std::pair <float, float> MeanSkoZ = calculateMeanStdDv(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(),0.0, AddZ);
				 seriesZMax->AddXY(i, Max->X * RTS, Label);
				 seriesZMin->AddXY(i, Min->X * RTS, Label);
				 seriesZSKO->AddXY(i, MeanSkoZ.second * RTS, Label);
				 seriesZMean->AddXY(i, MeanSkoZ.first * RTS, Label);

				 Max = std::max_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointZ);
				 Min = std::min_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointZ);
				 MeanSkoZ = calculateMeanStdDv(AngleErrorValues.begin(), AngleErrorValues.end(),0.0, AddZ);
				 seriesAzMax->AddXY(i, Max->X * RTS, Label);
				 seriesAzMin->AddXY(i, Min->X * RTS, Label);
				 seriesAzSKO->AddXY(i, MeanSkoZ.second * RTS, Label);
				 seriesAzMean->AddXY(i, MeanSkoZ.first * RTS, Label);
		}
	}


}
//---------------------------------------------------------------------------


