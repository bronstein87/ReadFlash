// ---------------------------------------------------------------------------

#ifndef SimplePlotterH
#define SimplePlotterH
#include <Classes.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>

class SimplePlotter {
public:

	_fastcall explicit SimplePlotter();
	_fastcall ~SimplePlotter();

	void AddSeries(TChart* Chart, DWORD SeriesIndex, TColor Color);
	void AddPoint(TChart* Chart, DWORD SeriesIndex, double X, double Y,
		TColor Color = clWhite);
	void AddArray(TChart* Chart, DWORD SeriesIndex, const double* ArrayX,
		const double* ArrayY, DWORD ArraySize);
	void CheckChartSeries(TChart* Chart);

	void SetTitle(const AnsiString& _Title) {
		Title = _Title;
	}

	void SetSeriesColor(const TColor& _SeriesColor) {
		SeriesColor = _SeriesColor;
	}

	void SetPointSize(unsigned short _PointSize) {
		PointSize = _PointSize;
	}

	void SetPointerStyle(TSeriesPointerStyle _PointerStyle) {
		PointerStyle = _PointerStyle;
	}

	void SetShowPoints(bool _ShowPoints) {
		ShowPoints = _ShowPoints;
	}

	void SetShowLines(bool _ShowLines) {
		ShowLines = _ShowLines;
	}

	void SetDateTimeX(bool _DateTimeX) {
		DateTimeX = _DateTimeX;
	}

	void ClearChart(TChart* Chart);

	void CheckGroupSeries(TChart *chartSource, TChart *chartApply);

	void SaveChart(TChart* Chart, AnsiString name, unsigned int Height,
		unsigned int Width);

	void ResetOptions();

private:

	void SetSeriesOptions(TLineSeries* Series);
	_fastcall SimplePlotter(const SimplePlotter&);

	_fastcall SimplePlotter(SimplePlotter &&);

	SimplePlotter& operator = (const SimplePlotter&);

	SimplePlotter& operator = (SimplePlotter &&);

	UnicodeString Title;
	TColor SeriesColor;
	bool ShowPoints;
	bool ShowLines;
	bool DateTimeX;
	unsigned short PointSize;
	TSeriesPointerStyle PointerStyle;

};
// ---------------------------------------------------------------------------
#endif
