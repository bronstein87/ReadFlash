//---------------------------------------------------------------------------

#ifndef SimplePlotterH
#define SimplePlotterH
#include <Classes.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>

class SimplePlotter
{
	public:

	_fastcall explicit SimplePlotter ();
	_fastcall ~SimplePlotter();

	void AddPoint (TChart* Chart, DWORD SeriesIndex, double X, double Y);
	void AddArray (TChart* Chart, DWORD SeriesIndex, const double* ArrayX, const double* ArrayY, DWORD ArraySize);

	void SetTitle (const AnsiString& _Title)
	{
		Title = _Title;
	}

	void SetSeriesColor (const TColor& _SeriesColor)
	{
		SeriesColor = _SeriesColor;
	}

	void SetPointSize (unsigned short _PointSize)
	{
		PointSize = _PointSize;
	}

	void SetPointerStyle (TSeriesPointerStyle _PointerStyle)
	{
		PointerStyle = _PointerStyle;
	}

	void SetShowPoints (bool _ShowPoints)
	{
		ShowPoints = _ShowPoints;
	}

	void SetShowLines (bool _ShowLines)
	{
		ShowLines = _ShowLines;
	}

	void ClearChart(TChart* Chart);

	void SaveChart(TChart* Chart, AnsiString name, unsigned int Height, unsigned int Width);

	void ResetOptions();


	private:

	void SetSeriesOptions(TLineSeries* Series);
	_fastcall SimplePlotter(const SimplePlotter&);

	_fastcall SimplePlotter(SimplePlotter&& );

	SimplePlotter& operator=(const SimplePlotter&);

	SimplePlotter& operator=(SimplePlotter&&);

	UnicodeString Title;
	TColor SeriesColor;
	bool ShowPoints;
	bool ShowLines;
	unsigned short PointSize;
	TSeriesPointerStyle  PointerStyle;


};
//---------------------------------------------------------------------------
#endif
