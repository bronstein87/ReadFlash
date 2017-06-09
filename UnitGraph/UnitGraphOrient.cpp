

#pragma hdrstop


#include "UnitGraphOrient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

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
		: TForm(Owner),FragID(0),FormAnimateSetting(new TFormAnimateSetting(this))
{
		ScaleFactorForScrollBox = 6.5;
		ScaleFactorForImage = 6;

}
//рисование сводных графиков по серии
//---------------------------------------------------------------------------
void TFormGraphOrient::CreateGraph()
{

	int numberOfSeries = 0;
	LineSeries[numberOfSeries]= new TLineSeries(ChartAl);
	ChartAl->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartDl);
	ChartDl->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartAz);
	ChartAz->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartWx);
	ChartWx->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartWy);
	ChartWy->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartWz);
	ChartWz->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartMx);
	ChartMx->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartMy);
	ChartMy->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartNst);
	ChartNst->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartFoc);
	ChartFoc->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartX0);
	ChartX0->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartY0);
	ChartY0->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartFone);
	ChartFone->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartNoise);
	ChartNoise->AddSeries(LineSeries[numberOfSeries]);
	numberOfSeries++;
	LineSeries[numberOfSeries]= new TLineSeries(ChartNloc);
	ChartNloc->AddSeries(LineSeries[numberOfSeries]);


}

void TFormGraphOrient::DeleteGraph()
{
	if(LineSeries[0] != NULL)
	{
		for (int numberOfSeries = 0; numberOfSeries < NumGraph ;numberOfSeries ++)
		{
			LineSeries[numberOfSeries]->Clear();
			delete LineSeries[numberOfSeries];
			LineSeries[numberOfSeries] = NULL;
		}
	}

}

//---------------------------------------------------------------------------
void __fastcall TFormGraphOrient::MenuClearClick(TObject *Sender)
{
	DeleteGraph();
//  for (int i=0; i<MaxSeries; i++) {
//    ColorArr[i]=DefColors[i%NumColor];
//	FreeIndex[i]=0;
//  }


//  FormSetting->ListBoxSeries->Clear();
//  FormSetting->EditFileName->Text="";
//  FormSetting->EditSeriesName->Text="";
}
//---------------------------------------------------------------------------

void TFormGraphOrient::SaveGraph(TChart *Chart, AnsiString suff)
{
unsigned int SaveHeigth, SaveWidth;

//        if (FormSetting->CheckBoxLabelVisible->Checked)
//        {
//            Chart->Title->Visible=false;
//        }
		Chart->Legend->Alignment = laBottom;
		SaveHeigth = Chart->Height;  // save visible size image
		SaveWidth = Chart->Width;
		Chart->Visible = false;      // paint user's size image
		Chart->Height = StrToInt(this->EditSizeY->Text);
		Chart->Width = StrToInt(EditSizeX->Text);
		Chart->SaveToBitmapFile(suff+".bmp");   // save image
		Chart->Visible = true;      // return souse size image
		Chart->Height = SaveHeigth;
		Chart->Width = SaveWidth;
		Chart->Legend->Alignment = laRight;

//        if (FormSetting->CheckBoxLabelVisible->Checked)
//        {
//            Chart->Title->Visible=true;
//        }
}

void __fastcall TFormGraphOrient::MenuSaveClick(TObject *Sender)
{
   SaveGraph(ChartAl, "Alpha");
   SaveGraph(ChartDl, "Delta");
   SaveGraph(ChartAz, "Azimut");
   SaveGraph(ChartMx, "Mx");
   SaveGraph(ChartMy, "My");
   SaveGraph(ChartNst, "Nst");
   SaveGraph(ChartFoc, "Foc");
   SaveGraph(ChartX0, "Xg");
   SaveGraph(ChartY0, "Yg");
}
//---------------------------------------------------------------------------
void TFormGraphOrient::ApplySeriesSetting(AnsiString Title, TColor color)
{
//  int CheckLine=MainForm->SeriesID[ListBoxSeries->ItemIndex];
//  MainForm->FilesName[CheckLine]=EditFileName->Text;
//  MainForm->SeriesTitle[CheckLine]=EditSeriesName->Text;
//  ListBoxSeries->Items->Strings[ListBoxSeries->ItemIndex]=EditSeriesName->Text;   //???

//  MainForm->SeriesColor[CheckLine]=ColorBoxSeries->Selected;
//  MainForm->SeriesLineWidth[CheckLine]=StrToInt(EditLineWidth->Text);

  for (int numberOfSeries = 0; numberOfSeries < NumGraph; numberOfSeries ++)
  {
	if (LineSeries[numberOfSeries] != NULL)
	{
	  LineSeries[numberOfSeries]->Title = Title;
	  LineSeries[numberOfSeries]->SeriesColor = color;
	  LineSeries[numberOfSeries]->Pointer->Visible = true;
	  LineSeries[numberOfSeries]->Pointer->Style = psCircle;
	  LineSeries[numberOfSeries]->Pointer->Size = 3;
	  LineSeries[numberOfSeries]->Pointer->Pen->Visible = false;
	  LineSeries[numberOfSeries]->Pen->Visible = false;
//	  LineSeries[num]->LinePen->Width=StrToInt(EditLineWidth->Text);
	}
  }
}
//---------------------------------------------------------------------------
void TFormGraphOrient::SetVisible(int CheckLine, bool tf)
{
  for (int num = NumGraph*CheckLine; num < NumGraph * (CheckLine + 1); num ++)
  {
	if (LineSeries[num] != NULL)
	{
	  LineSeries[num]->Visible = tf;
	}
  }
}

//рисование анимации по одному кадру
//---------------------------------------------------------------------------
void __fastcall TFormGraphOrient::FormCreate(TObject *Sender)
{
  Label1->Caption="Звезд в поле зрения: 0";
  Label2->Caption="Фрагментов в обработке: 0";
  Label3->Caption="Локализовано объектов: 0";
  Label4->Caption="Распознано объектов: 0";
  Label5->Caption="Блоков для чтения: 0";
  Label6->Caption="Общее число строк: 0";

//InitFrameSeries
	for (int i=0; i < MaxBlockSeries; i++) {
		BlockSeries[i]= new TChartShape(Chart1);
		BlockSeries[i]->Selected->Hover->Hide();
		Chart1->AddSeries(BlockSeries[i]);
		BlockSeries[i]->Style=chasRectangle;
		BlockSeries[i]->X0=0;
		BlockSeries[i]->X1=1024;
		BlockSeries[i]->Y0=0;
		BlockSeries[i]->Y1=0;
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
}
//---------------------------------------------------------------------------
void TFormGraphOrient::InitTableWindows(void)
{
	int k=0;

	TableWindowsInfo->RowCount=2;
	TableWindowsInfo->ColCount=10;
	TableWindowsInfo->FixedCols=0;
	TableWindowsInfo->FixedRows=1;

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

	if (mCadr.CountWindows) {
		TableWindowsInfo->RowCount=mCadr.CountWindows+TableWindowsInfo->FixedRows;

		for (int i=0; i < mCadr.CountWindows; i++) {
			int k=0;
			TableWindowsInfo->Cells[k++][i+1]=String(i+1);
			TableWindowsInfo->Cells[k++][i+1]=FloatToStrF(mCadr.WindowsList[i].Mv, ffFixed,6,2);
			TableWindowsInfo->Cells[k++][i+1]=String(mCadr.WindowsList[i].Xstart);
			TableWindowsInfo->Cells[k++][i+1]=String(mCadr.WindowsList[i].Ystart);
			TableWindowsInfo->Cells[k++][i+1]=FloatToStrF(mCadr.WindowsList[i].Mean, ffFixed,6,2);
			TableWindowsInfo->Cells[k++][i+1]=FloatToStrF(mCadr.WindowsList[i].Sigma, ffFixed,6,2);
			TableWindowsInfo->Cells[k++][i+1]=String(mCadr.WindowsList[i].Level);
			TableWindowsInfo->Cells[k++][i+1]=String(mCadr.WindowsList[i].CountObj);
			TableWindowsInfo->Cells[k++][i+1]=String(mCadr.WindowsList[i].Width)+"x"
											 +String(mCadr.WindowsList[i].Height);
			TableWindowsInfo->Cells[k++][i+1]=String(mCadr.WindowsList[i].ZipX)+"x"
											 +String(mCadr.WindowsList[i].ZipY);
	//		TableWindowsInfo->Cells[4][i]=String(mCadr.WindowsList[i].StarID);
		}
	}
	else
	{
		TableWindowsInfo->RowCount=TableWindowsInfo->FixedRows+1;
		for (int k=0; k<TableWindowsInfo->ColCount; k++) {
			TableWindowsInfo->Cells[k][1]="";
        }
    }
}

void TFormGraphOrient::InitTableObjects(void)
{
	int k=0;

	TableObjectsInfo->RowCount=2;
	TableObjectsInfo->ColCount=10;
	TableObjectsInfo->FixedCols=0;
	TableObjectsInfo->FixedRows=1;

	TableObjectsInfo->Cells[k++][0]="№";
	TableObjectsInfo->Cells[k++][0]="Xloc";
	TableObjectsInfo->Cells[k++][0]="Yloc";
	TableObjectsInfo->Cells[k++][0]="Bright";
	TableObjectsInfo->Cells[k++][0]="CountPix";
	TableObjectsInfo->Cells[k++][0]="Star №";
	TableObjectsInfo->Cells[k++][0]="Mv";
	TableObjectsInfo->Cells[k++][0]="Sp";
	TableObjectsInfo->Cells[k++][0]="Dx, mkm";
	TableObjectsInfo->Cells[k++][0]="Dy, mkm";
}

void TFormGraphOrient::PrintTableObjects(const struct CadrInfo &mCadr)
{

	if (mCadr.CountLocalObj) {
		TableObjectsInfo->RowCount=mCadr.CountLocalObj+TableWindowsInfo->FixedRows;

		for (int i=0; i<mCadr.CountLocalObj; i++) {
			if (mCadr.ObjectsList[i].StarID) {
//				for (int j=0; j<TableObjectsInfo->ColCount; j++)
//					TableObjectsInfo->Cells[j][i+1]->Color=clRed;
			}
			int k=0;
			TableObjectsInfo->Cells[k++][i+1]=String(i+1);
			TableObjectsInfo->Cells[k++][i+1]=FloatToStrF(mCadr.ObjectsList[i].X, ffFixed,10,4);
			TableObjectsInfo->Cells[k++][i+1]=FloatToStrF(mCadr.ObjectsList[i].Y, ffFixed,10,4);
			TableObjectsInfo->Cells[k++][i+1]=FloatToStrF(mCadr.ObjectsList[i].Bright, ffFixed,10,2);
			TableObjectsInfo->Cells[k++][i+1]=String(mCadr.ObjectsList[i].Square);
			TableObjectsInfo->Cells[k++][i+1]=String(mCadr.ObjectsList[i].StarID);
			TableObjectsInfo->Cells[k++][i+1]=FloatToStrF(mCadr.ObjectsList[i].Mv, ffFixed,6,2);
			TableObjectsInfo->Cells[k++][i+1]=String(mCadr.ObjectsList[i].Sp);
			TableObjectsInfo->Cells[k++][i+1]=FloatToStrF(mCadr.ObjectsList[i].Dx*1000., ffFixed,8,2);
			TableObjectsInfo->Cells[k++][i+1]=FloatToStrF(mCadr.ObjectsList[i].Dy*1000., ffFixed,8,2);
		}
	}
	else
	{
		TableObjectsInfo->RowCount=TableObjectsInfo->FixedRows+1;
		for (int k=0; k<TableObjectsInfo->ColCount; k++) {
			TableObjectsInfo->Cells[k][1]="-";
        }
	}
}

void TFormGraphOrient::ClearAnimate(void)
{
//AnimateShapes
	for (int i = 0; i < MaxBlockSeries; i++)
		BlockSeries[i]->Visible=false;
	for (int i = 0; i < MaxFrameSeries; i++)
		FrameSeries[i]->Visible=false;

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
		FrameSeries[i]->Marks->Visible=isVisible;
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

	for (int i=0; i<CountBlock; i++)
	{
		if (mCadr.IsBinary)
		{
			TabTakeAway[i][0]=mCadr.LinesList[i].Start>>1;
			TabTakeAway[i][1]=mCadr.LinesList[i].Height>>1;
		}
		BlockSeries[i]->Visible=true;
		BlockSeries[i]->X0=0;
		BlockSeries[i]->X1=mCadr.ImageWidth;
		BlockSeries[i]->Y0=TabTakeAway[i][0];
		BlockSeries[i]->Y1=TabTakeAway[i][0]+TabTakeAway[i][1];
		CountLines+=mCadr.LinesList[i].Height;
	}

//DrawWindows()
	for (int i=0; i < mCadr.CountWindows; i++)
	{
		FrameSeries[i]->X0=mCadr.WindowsList[i].Xstart;
		FrameSeries[i]->X1=mCadr.WindowsList[i].Xstart
							  +mCadr.WindowsList[i].Width;
		FrameSeries[i]->Y0=mCadr.WindowsList[i].Ystart;
		FrameSeries[i]->Y1=mCadr.WindowsList[i].Ystart
							  +mCadr.WindowsList[i].Height;
	}

//ShowLabelWindows()
	AnsiString LabelWindow;
	for (int i = 0; i < mCadr.CountWindows; i++)
	{
		FrameSeries[i]->Marks->Item[0]->Visible = false;
		FrameSeries[i]->Marks->Item[1]->Visible = true;
		FrameSeries[i]->Marks->Item[1]->Text->Clear();

		LabelWindow.sprintf("%d) Mv = %.2f,\r\n № CAT = %ld", i+1,
					mCadr.WindowsList[i].Mv, mCadr.WindowsList[i].StarID);
		FrameSeries[i]->Marks->Item[1]->Text->Add(LabelWindow);
		FrameSeries[i]->Visible=true;
	}

//DrawBarWindows()
	for (int i = 0; i < mCadr.CountWindows; i ++)
	{
		Series4->AddXY(i+1,mCadr.WindowsList[i].Mean);
		Series5->AddXY(i+1,mCadr.WindowsList[i].Sigma);
		Series6->AddXY(i+1,mCadr.WindowsList[i].CountObj);
	}

//DrawBarObjects()
	if (mCadr.CountWindows)
	{
		unsigned short *ObjShiftWnd, CountLocalObj;
		TColor CurColor, ColorBrightDef=Series7->Color;
		TColor ColorBright[]={ColorBrightDef, clYellow, clRed};
		ObjShiftWnd = new unsigned short [mCadr.CountWindows];
		CountLocalObj = 0;
		for (int k = 0;k < mCadr.CountWindows; k++)
		{
			if (!k) ObjShiftWnd[k]=0;
			else ObjShiftWnd[k] = ObjShiftWnd[k-1] + mCadr.WindowsList[k-1].CountObj;

			int j = 0;
			for (int i = ObjShiftWnd[k]; i<ObjShiftWnd[k]+mCadr.WindowsList[k].CountObj; i++)
			{
				if (FormAnimateSetting->CheckBoxApplyWindowsSeries->Checked) {
					switch (j+1)
					{
						case 1: {
							CurColor=FormAnimateSetting->ShapeColorOneObjTable->Brush->Color;
							break;
						}
						case 2: {
							CurColor=FormAnimateSetting->ShapeColorTwoObjTable->Brush->Color;
							break;
						}
						default:
						{
							CurColor=FormAnimateSetting->ShapeColorThreeObjTable->Brush->Color;
							break;
						}
					}
				}
				else CurColor=ColorBright[j];
				Series7->AddXY((k+1)+0.2*j,mCadr.ObjectsList[i].Bright, "", CurColor);
				Series8->AddXY((k+1)+0.2*j,mCadr.ObjectsList[i].Square, "", CurColor);
				CountLocalObj++;
				j++;
	//			WndObj[i]=k;
			}
		}

		if (CountLocalObj != mCadr.CountLocalObj) {

			LabelFrameError->Caption = "Несоответствие числа фрагментов и массива ObjFrag[]!";
		}
		delete [] ObjShiftWnd;

	}
	else
	{
		for (int i = 0; i < mCadr.CountLocalObj; i++)
		{
			Series7->AddXY(i+1,mCadr.ObjectsList[i].Bright);
			Series8->AddXY(i+1,mCadr.ObjectsList[i].Square);
		}
	}

  AnsiString LabelStar;
//  Series1->Marks->Visible=true;
  for (int i=0; i<mCadr.CountStars; i++){

//	  LabelStar="Mv="+FloatToStrF(mCadr.StarsList[i].Mv,ffFixed,8,2);
	  Series1->AddXY(mCadr.StarsList[i].X,mCadr.StarsList[i].Y);//,LabelStar);
  }

  Label1->Caption="Звезд в поле зрения: "+IntToStr(mCadr.CountStars);
  Label2->Caption="Фрагментов в обработке: "+IntToStr(mCadr.CountWindows);
  Label3->Caption="Локализовано объектов: "+IntToStr(mCadr.CountLocalObj);
  Label4->Caption="Распознано объектов: "+IntToStr(mCadr.CountDeterObj);
  Label5->Caption="Блоков для чтения: "+IntToStr(mCadr.CountBlock);

  if (CountLines == mCadr.CountLines)
	Label6->Caption = "Общее число строк: "+IntToStr(mCadr.CountLines);
  else
	Label6->Caption = "ОШИБКА! Число строк: "+IntToStr(CountLines)
	+" вместо "+IntToStr(mCadr.CountLines);
}

void TFormGraphOrient::DrawAnimate(const struct CadrInfo &mCadr)
{
double X0, Y0, X1, Y1, Dx, Dy, Ist, Nel;

  EditTimeDev->Text = FloatToStrF(mCadr.Time,ffFixed, 10,3);
  ClearAnimate();

  for (int i = 0; i < mCadr.CountLocalObj; i++) {
	   X0=mCadr.ObjectsList[i].X;
	   Y0=mCadr.ObjectsList[i].Y;
	   Nel=mCadr.ObjectsList[i].Square;
	   Dx=mCadr.ObjectsList[i].Dx;
	   Dy=mCadr.ObjectsList[i].Dy;

	   if ((!mCadr.ObjectsList[i].StarID) && (!Dx)&&(!Dy)) {
//		   Series2->AddXY(X0, Y0, "", clBlue);
		   Series9->AddBubble(X0, Y0,(int)(3*sqrtm(Nel)+0.5),"",clBlue);
	   }
	   else
	   {
		   X1=X0 - mCadr.ObjectsList[i].Dx*10000.;
		   Y1=Y0 - mCadr.ObjectsList[i].Dy*10000.;
//		   Series2->AddXY(X0, Y0, "", clGreen);
		   Series9->AddBubble(X0, Y0,(int)(3*sqrtm(Nel)+0.5),"",clGreen);
		   Series3->AddArrow(X0, Y0, X1, Y1, "", clRed);
	   }
//	   Series9->AddBubble(mCadr.StarsList[i].X,mCadr.StarsList[i].Y,
//						(int)(sqrtm(mCadr.StarsList[i].Square)+1));

  }


  DrawFragment(mCadr);
  DrawBlock(mCadr);
  PrintTableWindows(mCadr);
  PrintTableObjects(mCadr);

}


void TFormGraphOrient::DrawFragment(const struct CadrInfo &mCadr)
{

	for(unsigned int i = 0;i < ImageScrollBoxVector.size();i ++)
	{
		 ImageScrollBoxVector[i]->Free();
	}

	ImageVector.clear();
	ImageScrollBoxVector.clear();
	AnsiString NeededDirectory=GetCurrentDir()+"\\Frag_"+FileTitle;

	if (!TDirectory::Exists(NeededDirectory))
	{
		LabelFrameError->Visible=true;
		LabelFrameError->Caption="Указан неверный путь к директории фрагментов";
		return;
	}
    else 		LabelFrameError->Visible=false;
   TStringDynArray FileNameList;
   FileNameList=TDirectory::GetFiles(NeededDirectory);
   AnsiString TimePrStr=FloatToStrF(mCadr.Time,ffFixed,10,10);
   TStringDynArray TimePrTwoParts=System::Strutils::SplitString(TimePrStr,".");

   // отсекаем лишние нули с конца
   int IndexOfZeroStart = 3;
	UnicodeString TimePrWithOutNulls = TimePrTwoParts[1].SubString(0, IndexOfZeroStart);
   // и добавляем их в конце, чтобы было как в названии файла фрагментов
//   TimePrTwoParts[1]= "00000" + TimePrWithOutNulls;
   TimePrTwoParts[1]=TimePrWithOutNulls; //убраны лишние нули в дробной части времени

   AnsiString FragmentFileStr;
   for(int CurrentFileName = 0;CurrentFileName < FileNameList.Length;CurrentFileName ++)
   {

		if(System::Strutils::AnsiContainsStr(FileNameList[CurrentFileName],TimePrTwoParts[0]) &&
		  System::Strutils::AnsiContainsStr(FileNameList[CurrentFileName],TimePrTwoParts[1]))
		  {
				 FragmentFileStr=FileNameList[CurrentFileName];
				 break;
		  }
   }

   if(FragmentFileStr!="")
   {

   FILE *FragmentFile;
   FragmentFile=fopen(FragmentFileStr.c_str(),"rb");
   if(!FragmentFile)
   {
		ShowMessage(AnsiString("Не удалось открыть файл ")+FragmentFileStr.c_str());
		return;
   }

	for(int CurrentFragment = 0;CurrentFragment < mCadr.CountWindows; CurrentFragment++)
	{               //mCadr.CountWindows
		int FragmentWidth = mCadr.WindowsList [CurrentFragment].Width;
		int FragmentHeight = mCadr.WindowsList [CurrentFragment].Height;
		int FragmentSize = FragmentWidth * FragmentHeight;
		unique_ptr<unsigned short[]> RawFragment(new unsigned short [FragmentSize]);
		fread(RawFragment.get(), sizeof(unsigned short), FragmentSize, FragmentFile);
		std::unique_ptr<TBitmap> Fragment (new TBitmap());
		Fragment->PixelFormat = pf24bit;
		Fragment->Width = FragmentWidth;
		Fragment->Height = FragmentHeight;


		TRGBTriple *BitmapLine; // структура, хранящая RBG
	for (int currentColumn = 0; currentColumn < FragmentHeight; currentColumn++)
	{
		// указатель на currentColumn строку Bitmap
		BitmapLine = (TRGBTriple*) Fragment->ScanLine[currentColumn];
		for (unsigned int currentRow = 0, adress = 0; currentRow < FragmentWidth; currentRow++, adress = currentColumn * FragmentWidth + currentRow)
		{           // снижаем градацию 255=4095/16
					BitmapLine[currentRow].rgbtBlue = RawFragment[adress]/8;
					BitmapLine[currentRow].rgbtGreen = RawFragment[adress]/8;
					BitmapLine[currentRow].rgbtRed = RawFragment[adress]/8;
		}
	}



	ImageScrollBoxVector.push_back(new TScrollBox(FragmentShowScrollBox));
	FragmentShowScrollBox->Color = clBlack;
	ImageScrollBoxVector.back()->Color = clBlack;
	ImageScrollBoxVector.back()->Width=Fragment->Width*ScaleFactorForScrollBox;
	ImageScrollBoxVector.back()->Height=Fragment->Height*ScaleFactorForScrollBox;
	ImageScrollBoxVector.back()->OnMouseWheelUp= &FormMouseWheelUp;
	ImageScrollBoxVector.back()->OnMouseWheelDown= &FormMouseWheelDown;
	ImageScrollBoxVector.back()->SetParentComponent(FragmentShowScrollBox);




	ImageVector.push_back(new TImage(ImageScrollBoxVector.back()));
	ImageVector.back()->Width=Fragment->Width*ScaleFactorForImage;
	ImageVector.back()->Height=Fragment->Height*ScaleFactorForImage;
	ImageVector.back()->Left=0;
	ImageVector.back()->Top=0;
	ImageVector.back()->OnMouseDown=&ImageOnClick;
	ImageVector.back()->Stretch=true;
	ImageVector.back()->Canvas->
	StretchDraw(Rect(0, 0, ImageVector.back()->Width, ImageVector.back()->Height),Fragment.get());
	ImageVector.back()->SetParentComponent(ImageScrollBoxVector.back());

	}

	if(ContrastCheckBox->Checked) ApplyContrastStory();
	PlaceImageFragments(ImageScrollBoxVector);
	fclose(FragmentFile);

   }

}


void TFormGraphOrient::ApplyContrastStory()
{
	if (!ContrastStory.empty())
	{
		for(int currentFragment = 0;currentFragment < ImageVector.size();currentFragment ++)
		{
			for(int i = 0; i < ContrastStory.size(); i ++)
			{
				changeContrast(ContrastStory[i], ImageVector[currentFragment]);
			}
		}
	}
}

void TFormGraphOrient::PlaceImageFragments (const vector<TScrollBox*>& FragmentImages)
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

		FragmentImages[CurrentImage]->Left = OffsetStep*WidthOffset;
		FragmentImages[CurrentImage]->Top = HeightOffset;
		++WidthOffset;
	}


}

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
		int cnt=StrToInt(EditNumCadr->Text);

		if (!GetCadrInfo(cnt, CurCadr))
		{
			  DrawAnimate(CurCadr);
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

	TStringGrid *thisGrid=(TStringGrid*)Sender;

	if (FormAnimateSetting->CheckBoxFillTableWindows->Checked) {
		if (ARow)
		{
			unsigned long StarID;
			float Dx, Dy;
			StarID=StrToInt(thisGrid->Cells[5][ARow]);
			Dx=StrToFloat(thisGrid->Cells[8][ARow]);
			Dy=StrToFloat(thisGrid->Cells[9][ARow]);
			if ((!StarID)&&(!Dx)&&(!Dy))
				thisGrid->Canvas->Brush->Color=FormAnimateSetting->ShapeColorLocObjTable->Brush->Color;
			else thisGrid->Canvas->Brush->Color=FormAnimateSetting->ShapeColorDetObjTable->Brush->Color;

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
		if ((ARow)&&(thisGrid->Cells[7][ARow]!=""))
		{

			if (thisGrid->Cells[7][ARow]=="0")
				thisGrid->Canvas->Brush->Color=FormAnimateSetting->ShapeColorZeroObjTable->Brush->Color;
			else if ((thisGrid->Cells[7][ARow]=="1"))
				thisGrid->Canvas->Brush->Color=FormAnimateSetting->ShapeColorOneObjTable->Brush->Color;
			else if ((thisGrid->Cells[7][ARow]=="2"))
				thisGrid->Canvas->Brush->Color=FormAnimateSetting->ShapeColorTwoObjTable->Brush->Color;
			else
				thisGrid->Canvas->Brush->Color=FormAnimateSetting->ShapeColorThreeObjTable->Brush->Color;

			thisGrid->Canvas->FillRect(Rect);
			thisGrid->Canvas->TextOutW(Rect.Left, Rect.Top, thisGrid->Cells[ACol][ARow]);
		}
	}
}
//---------------------------------------------------------------------------


void GetFileTitles(AnsiString file_name, AnsiString *file_title)
{
	  AnsiString file_ext;

	  file_ext =ExtractFileName(file_name);
	  if (file_ext.LastDelimiter("."))
		*file_title=file_ext.SubString(0,file_ext.LastDelimiter(".")-1);
	  else *file_title=file_ext.SubString(0,file_ext.Length());
}


	// сортируем и перезаписываем исходный файл в новый файл с постфиксом _sorted
			AnsiString TFormGraphOrient::SortRawFlashFile(const AnsiString &RawFileName)
			{
			   FILE* RawFlashFile;
			   RawFlashFile=fopen(RawFileName.c_str(),"rb");
			   if(!RawFlashFile)
			   {
					return "";
			   }

			  std::vector<RawFileInfo>  RawFileInfoVector;
			  int Marker;
			   while(fread(&Marker,sizeof(int),1,RawFlashFile) == 1)
			   {
					if (GetInt(Marker)==0x55550000)
					{
							RawFileInfo CurrentInfo;

							// текущая позиция на начале заголовочной структуры
							CurrentInfo.Pos =  ftell(RawFlashFile)-4 ;

							if(RawFileInfoVector.size()==0)
							{
								CurrentInfo.Size = 0;
							}
							else
							{
								CurrentInfo.Size=CurrentInfo.Pos - RawFileInfoVector.back().Pos;
							}

							unsigned short SequenceCounterAndAdress [2];
							fread(&SequenceCounterAndAdress,sizeof(short),2,RawFlashFile);
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


			   std::sort(RawFileInfoVector.begin(),RawFileInfoVector.end(),FlashCompare);

			   FILE * SortedFile;
			   AnsiString SortedFlashFileName= GetCurrentDir()+"\\flash_sorted.dat";
			   SortedFile = fopen(SortedFlashFileName.c_str(),"wb");

			   if(!SortedFile)
			   {
					return "";
			   }

			   // -1 т.к размер блока хранится  в следующем элементе
			   for(unsigned int CurrentFlashStruct = 0; CurrentFlashStruct < RawFileInfoVector.size()-1 ;CurrentFlashStruct++)
			   {
					fseek(RawFlashFile,RawFileInfoVector[CurrentFlashStruct].Pos,SEEK_SET);
					char* CurrentStructBuffer= new char [RawFileInfoVector[CurrentFlashStruct+1].Size];
					fread(CurrentStructBuffer,sizeof(char),RawFileInfoVector[CurrentFlashStruct+1].Size,RawFlashFile);
					fwrite(CurrentStructBuffer,sizeof(char),RawFileInfoVector[CurrentFlashStruct+1].Size,SortedFile);
					delete [] CurrentStructBuffer;
			   }

			   fclose(RawFlashFile);
			   fclose(SortedFile);
			   return SortedFlashFileName;
			}




int TFormGraphOrient::GetCadrInfo(int NC, struct CadrInfo &mCadr)
{
	if ((NC >= 0)&&(NC < vCadrInfo.size()))
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
		Action=caFree;
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphOrient::MenuOpenFlashClick(TObject *Sender)
{
  try
  {
  OpenDialog1->Filter="dat|*.dat";
  if (OpenDialog1->Execute()) {
		vCadrInfo.clear();
		FileName=OpenDialog1->FileName;
		SetCurrentDir(ExtractFileDir(FileName));
		GetFileTitles(FileName,&FileTitle);

		if(SortFileCheckBox->Checked)
		{
			FileName=SortRawFlashFile(FileName);
			if(FileName=="")
			{
				ShowMessage("Не удалось отсортировать файл. Возможно, исходный файл не удалось открыть");
				return;
			}
		}

		FILE *ffrag;
		FILE *fflesh;
		FILE *ftxt;
		FILE *floc;

		fflesh=fopen(FileName.c_str(),"rb");
		FileName=FileTitle+"_sec.dat";
		ffrag =fopen(FileName.c_str(),"wb");
		ftxt= fopen((FileTitle+"_LogSec.txt").c_str(),"wt");

		unsigned short buf[2];
		unsigned int CntWord=0;

		int Marker;
		while(!feof(fflesh))
		{
			fread(&Marker,sizeof(int),1,fflesh);
			if (GetInt(Marker)==0x55550000)
			{
				fread(&buf,sizeof(short),2,fflesh);
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
		ftxt=fopen((FileTitle+".txt").c_str(),"wt");



		CurDir=GetCurrentDir();
		FragDir=CurDir+"\\Frag_"+FileTitle;
	    LocDir=CurDir+"\\Loc_"+FileTitle;
		CreateDir(FragDir);
		CreateDir(LocDir);


		FILE *flog_pix;
		flog_pix=fopen((FileTitle+"_LogPix.txt").c_str(),"wt");
		fprintf(flog_pix,"%6s  NC  %6s %10s %10s %10s %8s\n",
						"SerNum","CntRec","T","Tbshv","Tpr","NumPix");


		FILE *flog_reg;
		flog_reg=fopen((FileTitle+"_LogReg.txt").c_str(),"wt");
		fprintf(flog_reg,"%6s %6s %10s %10s %10s %8s %4s %4s\n",
						"SerNum","CntRec","T","Tbshv","Tpr","CntErr", "Reg", "Val");

		FILE *flog_orient;
		flog_orient=fopen((FileTitle+"_LogOrient.txt").c_str(),"wt");
		fprintf(flog_orient,"%6s RG %6s %10s %10s %10s %6s %8s %8s %8s %8s %10s %10s %10s ",
						"SerNum","CntRec","T","Tbshv","Tpr","Stat","NumProg",
						"NumFrag","NumLoc","NumDet","al,deg","dl,deg","Az,deg");


		fprintf(flog_orient,"%10s %8s %8s %8s %12s %12s %12s %8s %8s %8s %8s\n",
						"m_cur,mm","Temp","Texp,ms","dT,c","Wx","Wy","Wz",
						"mean_bf","sigma_bf","mean_fr","sigma_fr");


        DeleteGraph();
		CreateGraph();
		ApplySeriesSetting("мБОКЗ-2В",
											clBlue);//ColorRes[FormGraphOrient->NumLine]);
//		DataPixHeader mDataPixHdr;
		while (!feof(fflesh))
		{
		  fread(&Marker,sizeof(int),1,fflesh);
		  int NumPixH, PixMas[3];

		// хэш мэп для с соответствиями перечислений
//		std::unordered_map<unsigned int ,unsigned int> MarkerMap
//		({
//			{SECTOR_MARKER,SECTOR_MARKER_ACCORDANCE},
//			{HO_MARKER,HO_MARKER_ACCORDANCE},
//			{SL_MARKER,SL_MARKER_ACCORDANCE},
//			{PIX_0_MARKER,PIX_0_MARKER_ACCORDANCE},
//			{PIX_1_MARKER,PIX_1_MARKER_ACCORDANCE},
//			{ALL_REG_MARKER,ALL_REG_MARKER_ACCORDANCE},
//			{SINGLE_REG_MARKER,SINGLE_REG_MARKER_ACCORDANCE},
//			{FRAG_MARKER,FRAG_MARKER_ACCORDANCE}
//		});

//		  MARKER_ACCORDANCE MarkerAccordance=
//		  static_cast<MARKER_ACCORDANCE>(MarkerMap.at(GetInt(Marker)));

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

			  for (int i=0; i<NumPixH; i=i+2)
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
			  vCadrInfo.push_back(mCadrInfo);

			  double al=0;
			  double dl=0;
			  double Az=0;

			  if (!mDataNO.res_stat)
			  {
				dl=asinm(mDataNO.M_ornt[2][2])*RTD;
				al=atan2m(mDataNO.M_ornt[2][1],mDataNO.M_ornt[2][0])*RTD;   if (al<0)  al+=360.;
				Az=atan2m(mDataNO.M_ornt[0][2],mDataNO.M_ornt[1][2])*RTD;   if (Az<0)  Az+=360.;
			  }


			  fprintf(flog_orient,"    %02d NO %6d %10ld %10ld %6ld.%03ld 0x%04x %8d ",
					  mDataNO.SerNum, mDataNO.CntRecord, mDataNO.T, mDataNO.Tbshv,
					  mDataNO.Tpr_sec, mDataNO.Tpr_msec, mDataNO.res_stat, 0);
			  fprintf(flog_orient,"%8d %8d %8d %10.6f %10.6f %10.6f %10.6f %8.2f %8d %8.2f ",
					  mDataNO.NumL[0], mDataNO.NumL[1], mDataNO.NumDet, al, dl, Az,
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
			  vCadrInfo.push_back(mCadrInfo);


				double al=0;
				double dl=0;
				double Az=0;

			  if (!mDataSLEZH.res_stat)
			  {
				dl=asinm(mDataSLEZH.M_ornt[2][2])*RTD;
				al=atan2m(mDataSLEZH.M_ornt[2][1],mDataSLEZH.M_ornt[2][0])*RTD;   if (al<0)  al+=360.;
				Az=atan2m(mDataSLEZH.M_ornt[0][2],mDataSLEZH.M_ornt[1][2])*RTD;   if (Az<0)  Az+=360.;
			  }


			  double mean_frag=0;
			  double sigma_frag=0;
			  for (int i=0; i<mDataSLEZH.NumFrag; i++)
	          {
				mean_frag+=mDataSLEZH.MeanFrag[i][0];
				sigma_frag+=mDataSLEZH.MeanFrag[i][1];
	          }
			  if (mDataSLEZH.NumFrag>0)
			  {
	            mean_frag/=mDataSLEZH.NumFrag;
				sigma_frag/=mDataSLEZH.NumFrag;
	          }

			  fprintf(flog_orient,"    %02d SL %6d %10ld %10ld %6ld.%03ld 0x%04x %8d ",
					  mDataSLEZH.SerNum, mDataSLEZH.CntRecord, mDataSLEZH.T, mDataSLEZH.Tbshv,
					  mDataSLEZH.Tpr_sec, mDataSLEZH.Tpr_msec, mDataSLEZH.res_stat, mDataSLEZH.NumProgFrag);
			  fprintf(flog_orient,"%8d %8d %8d %10.6f %10.6f %10.6f %10.6f %8.2f %8d %8.2f ",
					  mDataSLEZH.NumFrag,mDataSLEZH.NumLoc, mDataSLEZH.NumDet, al, dl, Az,
					  mDataSLEZH.m_cur, mDataSLEZH.Temp, mDataSLEZH.StarExp, mDataSLEZH.TimeExp);
			  fprintf(flog_orient,"%12.4f %12.4f %12.4f %8.2f %8.2f %8.2f %8.2f\n",
					  mDataSLEZH.Wop[0]*RTS,mDataSLEZH.Wop[1]*RTS,mDataSLEZH.Wop[2]*RTS,
					  mDataSLEZH.mean, mDataSLEZH.sigma,mean_frag, sigma_frag);

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
			  double Tpr = mDataSLEZH.Tpr_sec+mDataSLEZH.Tpr_msec/1000.;
			  if (mDataSLEZH.NumDet>=3) {

				LineSeries[0]->AddXY(Tpr,al);
				LineSeries[1]->AddXY(Tpr,dl);
				LineSeries[2]->AddXY(Tpr,Az);
				LineSeries[3]->AddXY(Tpr,mDataSLEZH.Wop[0]*RTS);
				LineSeries[4]->AddXY(Tpr,mDataSLEZH.Wop[1]*RTS);
				LineSeries[5]->AddXY(Tpr,mDataSLEZH.Wop[2]*RTS);
	//			FormGraphOrient->LineSeries[FormGraphOrient->NumLine*NumGraph+6]->AddXY(Tpr,mx); //рассчитать
	//			FormGraphOrient->LineSeries[FormGraphOrient->NumLine*NumGraph+7]->AddXY(Tpr,my); //рассчитать
				LineSeries[8]->AddXY(Tpr,mDataSLEZH.m_cur);
				LineSeries[9]->AddXY(Tpr,mDataSLEZH.NumFrag);
				LineSeries[11]->AddXY(Tpr,mDataSLEZH.NumDet);
			  }
			  LineSeries[10]->AddXY(Tpr,mDataSLEZH.NumLoc);
			  LineSeries[12]->AddXY(Tpr,mean_frag);
			  LineSeries[13]->AddXY(Tpr,sigma_frag);
			  LineSeries[14]->AddXY(Tpr,mDataSLEZH.Temp);
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
					 mDataFragHdr.Tpr_sec,mDataFragHdr.Tpr_msec,FragID);
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

		EditNumCadr->Text=0;
		UpDown1->Max=vCadrInfo.size();

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




void __fastcall TFormGraphOrient::FragmentShowScrollBoxResize(TObject *Sender)
{
	PlaceImageFragments(ImageScrollBoxVector);
}
//---------------------------------------------------------------------------




void __fastcall TFormGraphOrient::FormMouseWheelUp(TObject *Sender, TShiftState Shift,
		 const TPoint& MousePos, bool &Handled)
{

	if(!(FindVCLWindow(MousePos)->Name=="FragmentShowScrollBox") &&
	!(FindVCLWindow(MousePos)->ClassName()=="TScrollBox"))
   {
		return;
   }

	if(ContrastCheckBox->Checked)  ContrastStory.push_back(20);

	for(int currentFragment = 0;currentFragment< ImageVector.size();currentFragment ++)
	{
	   changeContrast(20, ImageVector[currentFragment]);

	}

}

void __fastcall TFormGraphOrient::FormMouseWheelDown(TObject *Sender, TShiftState Shift, const TPoint& MousePos,
bool &Handled)
{
	if(!(FindVCLWindow(MousePos)->Name=="FragmentShowScrollBox") &&
	!(FindVCLWindow(MousePos)->ClassName()=="TScrollBox"))
   {
		return;
   }

	if(ContrastCheckBox->Checked)  ContrastStory.push_back(-20);
	for(int currentFragment = 0;currentFragment < ImageVector.size();currentFragment++)
	{
		changeContrast(-20, ImageVector[currentFragment]);
	}

}






 void __fastcall TFormGraphOrient::ImageOnClick(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TImage* Image = dynamic_cast<TImage*> (Sender);
	TScrollBox* ScrollBox= dynamic_cast<TScrollBox*>(Image->Parent);

	if(Button==mbLeft)
	{
		Image->Width= Image->Width * 1.2;
		Image->Height= Image->Height * 1.2;
		// определяем диапазоны ползунков ( ThumpSize всегда почему-то возвращает ноль, так что только таким способом )
		ScrollBox->VertScrollBar->Position= INT_MAX;
		ScrollBox->HorzScrollBar->Position= INT_MAX;
		ScrollBox->VertScrollBar->Position=(((double)(ScrollBox->VertScrollBar->Position)/Image->Height) * Y) * 1.2;
		ScrollBox->HorzScrollBar->Position=(((double)(ScrollBox->HorzScrollBar->Position)/Image->Width)* X) * 1.2;
	}
	else if(Button==mbRight)
	{
		Image->Width= Image->Width / 1.2;
		Image->Height= Image->Height / 1.2;
		ScrollBox->VertScrollBar->Position= INT_MAX;
		ScrollBox->HorzScrollBar->Position= INT_MAX;
		ScrollBox->VertScrollBar->Position=(((double)(ScrollBox->VertScrollBar->Position)/Image->Height) * Y) / 1.2;
		ScrollBox->HorzScrollBar->Position=(((double)(ScrollBox->HorzScrollBar->Position)/Image->Width)* X) / 1.2;
	}


}

//-------------------------------------------------------------------------

void __fastcall TFormGraphOrient::ContrastCheckBoxClick(TObject *Sender)
{
	if (!ContrastCheckBox->Checked)
	{
		ContrastStory.clear();
	}
}
//---------------------------------------------------------------------------


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
                else finp>>tmi.timeBOKZ;
			}
			else if (word=="ФOK") finp>>tmi.Foc;
			else if (word=="Х0") finp>>tmi.Xg;
			else if (word=="У0") finp>>tmi.Yg;
			else if ((word=="МТ")||(word=="MT")) finp>>tmi.Mean;
			else if ((word=="СТ")||(word=="CT")) finp>>tmi.Sigma;
			else if (word=="НАМ") {
				finp>>word;
				if (word=="ДЕФ") finp>>tmi.countDefect;
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
				else finp>>tmi.timeBOKZ;
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
			if ((word==arrStatErrorRus[MAX_STAT-1])||(word==arrStatErrorEng[MAX_STAT-1])) {
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
	mCadr.CountWindows = tmi.nWindows;

	int cntLocal;
	if (tmi.nLocalObj<MAX_OBJ_DTMI) cntLocal=tmi.nLocalObj;
	else cntLocal=MAX_OBJ_DTMI;

	mCadr.CountLocalObj = cntLocal;

	ObjectsInfo objInfo;
	for (int i = 0; i < cntLocal; i++) {
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

	int cntWindows;
	if (tmi.nWindows < MAX_WINDOW) cntWindows = tmi.nWindows;
	else cntWindows = MAX_WINDOW;



	WindowsInfo winInfo;
	for (int i = 0; i < cntWindows; i++) {
		winInfo.Level = tmi.levelWindow[i];
		winInfo.CountObj = tmi.nObjectWindow[i];
		winInfo.Width = 17;
		winInfo.Height = 17;
		winInfo.Xstart = tmi.centrWindow[i][0]-(mCadr.WindowsList[i].Width>>1);
		winInfo.Ystart = tmi.centrWindow[i][1]-(mCadr.WindowsList[i].Height>>1);
		mCadr.WindowsList.push_back(winInfo);
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
	mCadr.CountLocalObj=tmi.nFixedObj; //nLocalObj

	int cntLocal;
	if (tmi.nFixedObj<MAX_OBJ_MLOC) cntLocal = tmi.nFixedObj;
	else cntLocal = MAX_OBJ_MLOC;


	ObjectsInfo objInfo;

	for (int i=0; i < cntLocal; i++) {
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
	mCadr.CountDeterObj = 0;
	mCadr.CountStars = 0;
}

void __fastcall TFormGraphOrient::MenuOpenProgressTMIClick(TObject *Sender)
{
	OpenDialog1->Filter="txt|*.txt";
	if (OpenDialog1->Execute()) {

		vCadrInfo.clear();
		DeleteGraph();
		CreateGraph();
		ApplySeriesSetting("мБОКЗ-2В",	clBlue);

		FileName = OpenDialog1->FileName;
		SetCurrentDir(ExtractFileDir(FileName));
		GetFileTitles(FileName,&FileTitle);

		ifstream finp(FileName.c_str());
		if (!finp.is_open()) {
			ShowMessage("Файл не может быть открыт!");
			return;
		}

		ofstream fout((FileTitle+"_decode.txt").c_str());
		ofstream fshtmi1((FileTitle+"_shtmi1.txt").c_str());
		ofstream fshtmi2((FileTitle+"_shtmi2.txt").c_str());

		fshtmi2<<std::setw(16)<<"KC1"<<std::setw(18)<<"KC2"<<std::setw(18)<<"POST";
		fshtmi2<<std::setw(6)<<"№"<<std::setw(6)<<"Texp";
		fshtmi2<<std::setw(8)<<"УСД"<<std::setw(8)<<"НО"<<std::setw(8)<<"НОСЛ";
		fshtmi2<<std::setw(8)<<"TО"<<std::setw(8)<<"TОСЛ"<<std::setw(12)<<"СЛЕЖ";
		for (int i = 0; i < MAX_STAT; i++) {
			if (i<10) fshtmi2<<std::setw(7)<<"EC"<<(i+1);
			else fshtmi2<<std::setw(6)<<"EC"<<(i+1);
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
					fshtmi1<<std::setw(16)<<mSHTMI1.status1<<std::setw(18)<<mSHTMI1.status2;
					fshtmi1<<std::setw(18)<<mSHTMI1.post;
					fshtmi1<<std::setw(6)<<mSHTMI1.serialNumber;
					fshtmi1<<std::setw(6)<<mSHTMI1.timeExp;
					fshtmi1<<std::setw(8)<<mSHTMI1.Foc;
					fshtmi1<<std::setw(8)<<mSHTMI1.Xg;
					fshtmi1<<std::setw(8)<<mSHTMI1.Yg;
					fshtmi1<<std::setw(8)<<mSHTMI1.Mean;
					fshtmi1<<std::setw(8)<<mSHTMI1.Sigma;
					fshtmi1<<std::setw(8)<<mSHTMI1.countDefect;
					fshtmi1<<std::setw(8)<<mSHTMI1.CRC;
					fshtmi1<<std::setw(8)<<mSHTMI1.Date;
					fshtmi1<<std::setw(8)<<mSHTMI1.Version;
					fshtmi1<<"\n";
				}
			}
			else if (line.find("ШТМИ2")!=std::string::npos) {
				if(TryReadSHTMI2(finp,mSHTMI2)) {
					PrintSHTMI2(fout,mSHTMI2);
					fshtmi2<<std::setw(16)<<mSHTMI2.status1<<std::setw(18)<<mSHTMI2.status2;
					fshtmi2<<std::setw(18)<<mSHTMI2.post;
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
		EditNumCadr->Text = 0;
		UpDown1->Max = vCadrInfo.size();
	}
}

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


	int cntLocal;
	if (tmi.nLocalObj < 30) cntLocal = tmi.nLocalObj;
	else cntLocal = 30;
	mCadr.CountLocalObj = cntLocal; //nLocalObj

	ObjectsInfo objInfo;
	for (int i = 0; i < cntLocal; i++) {
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
	mCadr.CountDeterObj = 0;
	mCadr.CountStars = 0;
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
void __fastcall TFormGraphOrient::MenuOpenEnergyTMIClick(TObject *Sender)
{
	struct DTMI_BOKZM tmi;
//	short mDay, mMonth, mYear, mHour, mMin, mSec;

	OpenDialog1->Filter="txt|*.txt";
	if (OpenDialog1->Execute()) {

		vCadrInfo.clear();
		DeleteGraph();
		CreateGraph();
		ApplySeriesSetting("мБОКЗ-2В",	clBlue);

		FileName = OpenDialog1->FileName;
		SetCurrentDir(ExtractFileDir(FileName));
		GetFileTitles(FileName,&FileTitle);

		ifstream finp(FileName.c_str());
		if (!finp.is_open()) {
			ShowMessage("Файл не может быть открыт!");
			return;
		}

		ofstream fout((FileTitle+"_decode.txt").c_str());

		std::string line, word1, word2, word3;
		unsigned short hex_val, dec_val;
		int cntRecDTMI = 0, ind;
		while (!finp.eof())
		{
			getline(finp, line, '\n' );
			if (line.find("ТМОС") != std::string::npos) {
				struct CadrInfo mCadr;

				//sscanf(line, "ТМОС %d.%d.%d %d:%d:%d ", );
				for (int i = 0; i < 290; i++) {
					finp>>word1>>word2>>dec_val;
					sscanf(word1.c_str(),"[%d]", &ind);
					sscanf(word2.c_str(),"0X%x", &hex_val);
					if ((i == ind)&&(hex_val == dec_val)) {
						ArrayDTMI[i] = hex_val;
					}
				}
				memcpy(&tmi.timeBOKZ, &ArrayDTMI[2], 30*sizeof(short));
				memcpy(&tmi.LocalList[2][0], &ArrayDTMI[36], 28*sizeof(short));
				memcpy(&tmi.LocalList[5][2], &ArrayDTMI[68], 28*sizeof(short));
				memcpy(&tmi.LocalList[9][0], &ArrayDTMI[100],28*sizeof(short));
				memcpy(&tmi.LocalList[12][2], &ArrayDTMI[132],28*sizeof(short));
				memcpy(&tmi.LocalList[16][0], &ArrayDTMI[164],28*sizeof(short));
				memcpy(&tmi.LocalList[19][2], &ArrayDTMI[196],28*sizeof(short));
				memcpy(&tmi.LocalList[23][0], &ArrayDTMI[228],28*sizeof(short));
				memcpy(&tmi.LocalList[26][2], &ArrayDTMI[260],28*sizeof(short));
				fout<<"\n"<<line<<"\n";

				PrintDTMI_BOKZM(fout, tmi);
				ConvertDataDTMI_BOKZM(tmi, mCadr);
				vCadrInfo.push_back(mCadr);
			}
		}
		finp.close();
		fout.close();
		EditNumCadr->Text = 0;
		UpDown1->Max = vCadrInfo.size();
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
				else throw (errorMessage);

				// ищем число локализованных объектов
				if(findWord(in,"объектов") != std::string::npos)
				{
					in >> cadrInfo.CountLocalObj;
					LineSeries[10]->AddXY(cadrInfo.Time,cadrInfo.CountLocalObj);
				}

				else throw (errorMessage);


				  //ищем число распознанных объектов
				if(findWord(in,"объектов") != std::string::npos)
				{
					in >> cadrInfo.CountDeterObj;
					LineSeries[11]->AddXY(cadrInfo.Time,cadrInfo.CountDeterObj);
				}
				else throw (errorMessage);

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
						splittedLocData = split(line,")\t");
						splittedLocData = split(splittedLocData[1],"\t");

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
				else throw (errorMessage);

				cadrInfo.CountBlock = 0;
				cadrInfo.CountWindows = 0;
				cadrInfo.CountStars = 0;
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
		   else throw (errorMessage);


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
				double al=0;
				double dl=0;
				double Az=0;

				dl=asinm(matrixOfOrientation[2][2])*RTD;
				al=atan2m(matrixOfOrientation[2][1],matrixOfOrientation[2][0])*RTD;   if (al<0)  al+=360.;
				Az=atan2m(matrixOfOrientation[0][2],matrixOfOrientation[1][2])*RTD;   if (Az<0)  Az+=360.;

				LineSeries[0]->AddXY(cadrInfo.Time,al);
				LineSeries[1]->AddXY(cadrInfo.Time,dl);
				LineSeries[2]->AddXY(cadrInfo.Time,Az);

		   }
		   else throw (errorMessage);


		   if(findWord(in,"объектов") != std::string::npos)
		   {
				in >> cadrInfo.CountLocalObj;
				LineSeries[10]->AddXY(cadrInfo.Time,cadrInfo.CountLocalObj);

		   }
		   else throw (errorMessage);


		   //ищем число распознанных объектов
		   if(findWord(in,"объектов") != std::string::npos)
		   {
				in >> cadrInfo.CountDeterObj;
				LineSeries[11]->AddXY(cadrInfo.Time,cadrInfo.CountDeterObj);
		   }
		   else throw (errorMessage);

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
			else throw (errorMessage);

		   if(findLine(in,"14) Проекции угловой скорости на оси ПСК") != std::string::npos)
		   {
				for(int i = 0; i < 3; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.OmegaOrient[i] = std::atof(splittedStr[1].c_str());
				}
				LineSeries[3]->AddXY(cadrInfo.Time,cadrInfo.OmegaOrient[0]*RTS);
				LineSeries[4]->AddXY(cadrInfo.Time,cadrInfo.OmegaOrient[1]*RTS);
				LineSeries[5]->AddXY(cadrInfo.Time,cadrInfo.OmegaOrient[2]*RTS);

		   }
		   else throw (errorMessage);


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
						LineSeries[9]->AddXY(cadrInfo.Time,i);
						break;
					}

					winInfo.Xstart = (std::atof(splittedStr[1].c_str()));
					winInfo.Ystart = (std::atof(splittedStr[2].c_str()));
					winInfo.Mean = 0;
					winInfo.Sigma = 0;
					winInfo.Mv = 0;
					cadrInfo.WindowsList.push_back(winInfo);
				}
				LineSeries[9]->AddXY(cadrInfo.Time,maxCountOfObjects);



		   }
		  else throw (errorMessage);

		   if(findLine(in,"16) Значение порогов во фрагментах") != std::string::npos)
		   {
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.WindowsList[i].Level = std::atoi(splittedStr[1].c_str());
				}

		   }
		   else throw (errorMessage);

		   
		   if(findLine(in,"17) Количество объектов во фрагментах") != std::string::npos)
		   {
				for(int i = 0; i < cadrInfo.CountWindows; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t");
					cadrInfo.WindowsList[i].CountObj = std::atoi(splittedStr[1].c_str());
				}

		   }
		   else throw (errorMessage);

		   cadrInfo.CountBlock = 0;
		   cadrInfo.CountStars = 0;
		   cadrInfoVec.push_back(cadrInfo);
		}

	}

}





void __fastcall TFormGraphOrient::BOKZ60ParseProtocolClick(TObject *Sender)
{

	try
	{
		OpenDialog1->Filter="txt|*.txt";
		if (OpenDialog1->Execute()) {
			vCadrInfo.clear();
			FileName = OpenDialog1->FileName;
			SetCurrentDir(ExtractFileDir(FileName));
			GetFileTitles(FileName,&FileTitle);

			ifstream in(FileName.c_str());
			if (!in.is_open()) {
				ShowMessage("Не удалось открыть файл");
				return;
			}
			DeleteGraph();
			CreateGraph();
			ApplySeriesSetting("мБОКЗ-2В",	clBlue);

			if(checkLocFile(in))
			{
			readBOKZ60LocProtocol(in,vCadrInfo);
			}

			else
			{
				readBOKZ60Protocol(in,vCadrInfo);
			}

			EditNumCadr->Text = 0;
			UpDown1->Max = vCadrInfo.size();

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
		   cadrInfo.ImageHeight = 1000;
		   cadrInfo.ImageWidth = 1000;

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
							  cadrInfo.CountWindows = i+1;
							  cadrInfo.CountLocalObj = i+1;
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
						case 14:
							winInfo.Width = 24;
							winInfo.Height = 48;
						case 11:
							winInfo.Width = 48;
							winInfo.Height = 24;
						case 10:
							winInfo.Width = 48;
							winInfo.Height = 48;
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
						cadrInfo.CountLocalObj  += i+1;
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
			else throw (errorMessage);

		   }
			else throw (errorMessage);



		   // ищем время привязки в секундах
			if(findWord(in, "информации") != std::string::npos)
			{
				int secs = 0;
				int msecs = 0;
				int prevPos = in.tellg();

				in>>line;
				if(line== "с")
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
		   else throw (errorMessage);



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

				LineSeries[0]->AddXY(cadrInfo.Time, al);
				LineSeries[1]->AddXY(cadrInfo.Time, dl);
				LineSeries[2]->AddXY(cadrInfo.Time, Az);


		   }
		   else throw (errorMessage);



		   if(findLine(in,"Угловая скорость по оптическим измерениям в проекциях на оси ПСК") != std::string::npos)
		   {
				for(int i = 0; i < 3; i++)
				{
					std::getline(in,line);
					vector<string> splittedStr = split(line,"\t\t\t\t");
					cadrInfo.OmegaOrient[i] = std::atof(splittedStr[1].c_str());
				}
				LineSeries[3]->AddXY(cadrInfo.Time,cadrInfo.OmegaOrient[0] * RTS);
				LineSeries[4]->AddXY(cadrInfo.Time,cadrInfo.OmegaOrient[1] * RTS);
				LineSeries[5]->AddXY(cadrInfo.Time,cadrInfo.OmegaOrient[2] * RTS);

		   }
		   else throw (errorMessage);


		   if(findWord(in,"Tcmv") != std::string::npos)
		   {
				double matrixTemperature = 0;
				in >> matrixTemperature;
				LineSeries[14]->AddXY(cadrInfo.Time, matrixTemperature);
		   }
		   else throw (errorMessage);


		   // ищем число фрагментов
		   if(findWord(in,"NumFrag") != std::string::npos)
		   {
				// выяснить
				in >> cadrInfo.InfoCountWindows;
				// если numFrag <= 0, значит так полубитый, пропускаем
				if(cadrInfo.InfoCountWindows <= 0) continue;
				cadrInfo.CountWindows =  cadrInfo.WindowsList.size();
				LineSeries[9]->AddXY(cadrInfo.Time, cadrInfo.CountWindows);
		   }
		   else throw (errorMessage);


		   // число локализ. объектов
		   if(findWord(in,"NumLoc[0]") != std::string::npos)
		   {
				in >> cadrInfo.CountLocalObj;
				// если NumLoc[0] <= 0, значит так полубитый, пропускаем
				if(cadrInfo.CountLocalObj <= 0) continue;
				
				LineSeries[10]->AddXY(cadrInfo.Time,cadrInfo.CountLocalObj);
		   }
		   else throw (errorMessage);


		   //ищем число распознанных объектов
		   if(findWord(in,"NumDet") != std::string::npos)
		   {
				in >> cadrInfo.CountDeterObj;
				// если NumDet <= 0, значит так полубитый, пропускаем
				if(cadrInfo.CountDeterObj <= 0) continue;
				LineSeries[11]->AddXY(cadrInfo.Time,cadrInfo.CountDeterObj);
		   }
		   else throw (errorMessage);



			if(findWord(in,"m_cur") != std::string::npos)
		   {
				double m_cur = 0;
				in >> m_cur;
				LineSeries[8]->AddXY(cadrInfo.Time, m_cur);
		   }
		   else throw (errorMessage);


		   cadrInfo.CountStars = cadrInfo.CountWindows;
		   cadrInfo.CountBlock = 0;
		   cadrInfoVec.push_back(cadrInfo);
		}

	}

}



//---------------------------------------------------------------------------


void __fastcall TFormGraphOrient::BOKZM2VParseProtocolClick(TObject *Sender)
{
	try
	{
		 OpenDialog1->Filter="txt|*.txt";
		if (OpenDialog1->Execute()) {
			vCadrInfo.clear();
			FileName = OpenDialog1->FileName;
			SetCurrentDir(ExtractFileDir(FileName));
			GetFileTitles(FileName,&FileTitle);

			ifstream in(FileName.c_str());

			if (!in.is_open()) {
				ShowMessage("Не удалось открыть файл");
				return;
			}

			DeleteGraph();
			CreateGraph();

			ApplySeriesSetting("мБОКЗ-2В", clBlue);

			readmBOKZ2VProtocol(in, vCadrInfo);

			EditNumCadr->Text = 0;
			UpDown1->Max = vCadrInfo.size();

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





