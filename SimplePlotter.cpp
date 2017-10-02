		//---------------------------------------------------------------------------
#include "SimplePlotter.h"
#pragma hdrstop

_fastcall SimplePlotter::SimplePlotter ():
  Title("IKI"),
  SeriesColor(clBlue),
  ShowPoints(true),
  ShowLines(false),
  PointSize(3),
  PointerStyle(psCircle)
{

}

_fastcall SimplePlotter::~SimplePlotter()
 {

 }

void SimplePlotter::ResetOptions()
 {
	Title = "IKI";
	SeriesColor = clBlue;
	ShowPoints = true;
	ShowLines = false;
	PointSize = 3;
  	PointerStyle = psCircle;
 }

 void SimplePlotter::ClearChart(TChart* Chart)
 {
	Chart->RemoveAllSeries();
 }

 void SimplePlotter::CheckGroupSeries(TChart *chartSource, TChart *chartApply)
 {
	for (int iSeries = 0; iSeries < chartSource->SeriesCount(); iSeries++)
	{
		bool visible = chartSource->Series[iSeries]->Visible;
		chartApply->Series[iSeries]->Visible = visible;
	}
 }

void SimplePlotter::SaveChart(TChart* Chart, AnsiString name, unsigned int Height, unsigned int Width)
{
		unsigned int SaveHeigth, SaveWidth;
		Chart->Legend->Alignment = laBottom;
		SaveHeigth = Chart->Height;  // save visible size image
		SaveWidth = Chart->Width;
		Chart->Visible = false;      // paint user's size image
		Chart->Height = StrToInt(Height);
		Chart->Width = StrToInt(Width);
		Chart->SaveToBitmapFile(name + ".bmp");   // save image
		Chart->Visible = true;      // return souse size image
		Chart->Height = SaveHeigth;
		Chart->Width = SaveWidth;
		Chart->Legend->Alignment = laRight;
}


void SimplePlotter::AddPoint (TChart* Chart, DWORD SeriesIndex, double X, double Y, TColor Color)
{
	  if (Chart->SeriesCount()  <= SeriesIndex ) {
		for (int i = Chart->SeriesCount(); i <= SeriesIndex; i++) {
			Chart->AddSeries(new TLineSeries(Chart));
			SetSeriesOptions(dynamic_cast <TLineSeries*> (Chart->Series[i]));

			if (DateTimeX) {
				Chart->BottomAxis->DateTimeFormat   = "dd.mm.yyyy hh:nn:ss";
			}
		}
	  }

	  if (Color == clWhite) {
		  Color = SeriesColor;
	  }
	  Chart->Series[SeriesIndex]->AddXY(X, Y, "", Color);
}

void  SimplePlotter::AddArray (TChart* Chart, DWORD SeriesIndex, const double* ArrayX, const double* ArrayY, DWORD ArraySize)
{
	  if (Chart->SeriesCount()  <= SeriesIndex ) {
		Chart->AddSeries(new TLineSeries(Chart));
		SetSeriesOptions(dynamic_cast <TLineSeries*> (Chart->Series[SeriesIndex]));
	  }

	  Chart->Series[SeriesIndex]->AddArray(ArrayX, ArraySize, ArrayY, ArraySize);
}

void SimplePlotter::SetSeriesOptions(TLineSeries* Series)
{
	Series->Title = Title;
	Series->SeriesColor = SeriesColor;
	Series->Pointer->Visible = ShowPoints;
	if (ShowPoints) Series->Pointer->Size = PointSize;
	Series->Pen->Visible = ShowLines;
	Series->Pointer->Style = PointerStyle;
	Series->XValues->DateTime = DateTimeX;
}

#include "SimplePlotter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
