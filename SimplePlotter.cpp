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

 }


void SimplePlotter::AddPoint (TChart* Chart, DWORD SeriesIndex, double X, double Y)
{
	  if (Chart->SeriesCount()  <= SeriesIndex ) {
		Chart->AddSeries(new TLineSeries(Chart));
		SetSeriesOptions(dynamic_cast <TLineSeries*> (Chart->Series[SeriesIndex]));
	  }
	  Chart->Series[SeriesIndex]->AddXY(X, Y);
}

void SimplePlotter::SetSeriesOptions(TLineSeries* Series)
{

	Series->Title = Title;
		eries->SeriesColor = SeriesColor;
		Series->Pointer->Visible = ShowPoints;
		if (ShowPoints) Series->Pointer->Size = PointSize;
		Series->Pen->Visible = ShowLines;
		Series->Pointer->Style = PointerStyle;

}

#include "SimplePlotter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
