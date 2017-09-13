//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Analyze.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TAnalyzeForm::TAnalyzeForm(TComponent* Owner)
	: TForm(Owner)
{
    Charts.push_back(ChartAnalyzeErrorAl); Charts.push_back(ChartAnalyzeErrorDl); Charts.push_back(ChartAnalyzeErrorAz);
	Charts.push_back(ChartAnalyzeXV); Charts.push_back(ChartAnalyzeYV); Charts.push_back(ChartAnalyzeZV);
}


 // перенести
template <class InputIterator, class Value, class UnaryOperation>
std::pair<Value, Value> calculateMeanStdDv (InputIterator first, InputIterator last, Value init, UnaryOperation extractWtC)
{
	if (first == last) return std::pair <Value, Value> (extractWtC(*first), Value());

	Value dispersio = 0;
	for (InputIterator i = first;i < last; i++)
	{
		init += extractWtC(*i);
		dispersio += pow(extractWtC(*i), 2);
	}
	auto count = std::distance(first,last);
	Value mean = init / count;
	dispersio = (dispersio / count) - pow(mean, 2);

	return std::pair <Value,Value> (mean, sqrt(dispersio));
}

//---------------------------------------------------------------------------
void __fastcall TAnalyzeForm::ChooseDirectoriesClick(TObject *Sender)
{
	FileOpenDialog1->Options << fdoPickFolders << fdoAllowMultiSelect;
	if (FileOpenDialog1->Execute())
	{
        ClearGraphsClick(this);
		THighLowLineSeries* seriesX = new THighLowLineSeries(ChartAnalyzeXV);
		seriesX->Title = "Max/Min";
		seriesX->Pen->Width = 3;
		THighLowLineSeries* seriesY = new THighLowLineSeries(ChartAnalyzeYV);
		seriesY->Title = "Max/Min";
		seriesY->Pen->Width = 3;
		THighLowLineSeries* seriesZ = new THighLowLineSeries(ChartAnalyzeZV);
		seriesZ->Title = "Max/Min";
		seriesZ->Pen->Width = 3;
		THighLowLineSeries* seriesAl = new THighLowLineSeries(ChartAnalyzeErrorAl);
		seriesAl->Title = "Max/Min";
		seriesAl->Pen->Width = 3;
		THighLowLineSeries* seriesDl = new THighLowLineSeries(ChartAnalyzeErrorDl);
		seriesDl->Title = "Max/Min";
		seriesDl->Pen->Width = 3;
		THighLowLineSeries* seriesAz = new THighLowLineSeries(ChartAnalyzeErrorAz);
		seriesAz->Title = "Max/Min";
		seriesAz->Pen->Width = 3;

		THighLowLineSeries* seriesXSKO = new THighLowLineSeries(ChartAnalyzeXV);
		seriesXSKO->Title = "СКО";
		THighLowLineSeries* seriesYSKO = new THighLowLineSeries(ChartAnalyzeYV);
		seriesYSKO->Title = "СКО";
		THighLowLineSeries* seriesZSKO = new THighLowLineSeries(ChartAnalyzeZV);
		seriesZSKO->Title = "СКО";
		THighLowLineSeries* seriesAlSKO = new THighLowLineSeries(ChartAnalyzeErrorAl);
		seriesAlSKO->Title = "СКО";
		THighLowLineSeries* seriesDlSKO = new THighLowLineSeries(ChartAnalyzeErrorDl);
		seriesDlSKO->Title = "СКО";
		THighLowLineSeries* seriesAzSKO = new THighLowLineSeries(ChartAnalyzeErrorAz);
		seriesAzSKO->Title = "СКО";

		TPointSeries* seriesXMean =  new TPointSeries(ChartAnalyzeXV);
		seriesXMean->Title = "Среднее";
		seriesXMean->Pointer->Style = psCircle;
		TPointSeries* seriesYMean =  new TPointSeries(ChartAnalyzeYV);
		seriesYMean->Title = "Среднее";
		seriesYMean->Pointer->Style = psCircle;
		TPointSeries* seriesZMean =  new TPointSeries(ChartAnalyzeZV);
		seriesZMean->Title = "Среднее";
		seriesZMean->Pointer->Style = psCircle;
		TPointSeries* seriesAlMean = new TPointSeries(ChartAnalyzeErrorAl);
		seriesAlMean->Title = "Среднее";
		seriesAlMean->Pointer->Style = psCircle;
		TPointSeries* seriesDlMean = new TPointSeries(ChartAnalyzeErrorDl);
		seriesDlMean->Title = "Среднее";
		seriesDlMean->Pointer->Style = psCircle;
		TPointSeries* seriesAzMean = new TPointSeries(ChartAnalyzeErrorAz);
		seriesAzMean->Title = "Среднее";
		seriesAzMean->Pointer->Style = psCircle;

		ChartAnalyzeXV->AddSeries(seriesX);
		ChartAnalyzeXV->AddSeries(seriesXSKO);
		ChartAnalyzeXV->AddSeries(seriesXMean);
		ChartAnalyzeYV->AddSeries(seriesY);
		ChartAnalyzeYV->AddSeries(seriesYSKO);
		ChartAnalyzeYV->AddSeries(seriesYMean);
		ChartAnalyzeZV->AddSeries(seriesZ);
		ChartAnalyzeZV->AddSeries(seriesZSKO);
		ChartAnalyzeZV->AddSeries(seriesZMean);
		ChartAnalyzeErrorAl->AddSeries(seriesAl);
		ChartAnalyzeErrorAl->AddSeries(seriesAlSKO);
		ChartAnalyzeErrorAl->AddSeries(seriesAlMean);
		ChartAnalyzeErrorDl->AddSeries(seriesDl);
		ChartAnalyzeErrorDl->AddSeries(seriesDlSKO);
		ChartAnalyzeErrorDl->AddSeries(seriesDlMean);
		ChartAnalyzeErrorAz->AddSeries(seriesAz);
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
				if (!AnsiContainsStr(FileList->Strings[j], "Img") && AnsiContainsStr(FileList->Strings[j], ".iki"))
				{
					if	(reader->ReadFormat(FileList->Strings[j], false))
					{
						TStringDynArray SplittedString = SplitString(FileList->Strings[j], "\\");
						UnicodeString ResFileName = FileOpenDialog1->Files->Strings[i] + "\\Img" + SplittedString[SplittedString.Length - 1];
						if (reader->ReadFormat(ResFileName, false))
						{
							AngularSpeedErrorValues.push_back(
							Point::Create(reader->StarsData.RecognizedAngularVelocity[0] - reader->Georeferencing.DeviceAngularVelocity[0],
							reader->StarsData.RecognizedAngularVelocity[1] - reader->Georeferencing.DeviceAngularVelocity[1],
							reader->StarsData.RecognizedAngularVelocity[2] - reader->Georeferencing.DeviceAngularVelocity[2]));

							double diffAz = reader->StarsData.RecognizedOrientationAngles[2] - reader->Georeferencing.OrientationAngles[2];
							if (abs(diffAz) > 5)        // потом убрать
							{
								diffAz = (reader->StarsData.RecognizedOrientationAngles[2] + reader->Georeferencing.OrientationAngles[2])
									-	abs(reader->StarsData.RecognizedOrientationAngles[2] - reader->Georeferencing.OrientationAngles[2]);
							}
							AngleErrorValues.push_back(
							Point::Create(reader->StarsData.RecognizedOrientationAngles[0] - reader->Georeferencing.OrientationAngles[0],
							reader->StarsData.RecognizedOrientationAngles[1] - reader->Georeferencing.OrientationAngles[1], diffAz));
						}
						else throw logic_error(string("Не удалось считать ") + AnsiString(FileList->Strings[j]).c_str());
					}
					else throw logic_error(string("Не удалось считать ") + AnsiString(FileList->Strings[j]).c_str());

				}
			}

				 int gradus, minutes, seconds;
				 double speedModule = sqrt(pow(reader->StarsData.RecognizedAngularVelocity[0] * RTD, 2) +
					  pow(reader->StarsData.RecognizedAngularVelocity[1] * RTD, 2) + pow(reader->StarsData.RecognizedAngularVelocity[2] * RTD, 2));
				 ToGMS (speedModule, gradus, minutes, seconds);
				 UnicodeString Label = FloatToStr(gradus) + "°" + FloatToStr(minutes) + "'" + FloatToStr(seconds) + "''";

				 struct { bool operator()(const Point& a,const Point& b) { return a.X < b.X;} } ComparePointX;
				 struct { float operator() (const Point& a) {return a.X;} } AddX;

				 vector <Point>::iterator Max = std::max_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointX);
				 vector <Point>::iterator Min = std::min_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointX);
				 std::pair <float, float> MeanSkoX = calculateMeanStdDv(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(),0.0, AddX);
				 seriesX->AddHighLow(i, Max->X * RTS, Min->X * RTS, Label);
				 seriesXMean->AddXY(i, MeanSkoX.first * RTS, Label);
				 seriesXSKO->AddHighLow(i, (MeanSkoX.first + MeanSkoX.second) * RTS, (MeanSkoX.first - MeanSkoX.second) * RTS , Label);

				 Max = std::max_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointX);
				 Min = std::min_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointX);
				 MeanSkoX = calculateMeanStdDv(AngleErrorValues.begin(), AngleErrorValues.end(),0.0, AddX);
				 seriesAl->AddHighLow(i, Max->X * RTS, Min->X * RTS, Label);
				 seriesAlMean->AddXY(i, MeanSkoX.first * RTS, Label);
				 seriesAlSKO->AddHighLow(i, (MeanSkoX.first + MeanSkoX.second) * RTS, (MeanSkoX.first - MeanSkoX.second) * RTS , Label);

				 struct { bool operator()(const Point& a,const Point& b) { return a.Y < b.Y; } } ComparePointY;
				 struct { float operator() (const Point& a) { return a.Y;} } AddY;

				 Max = std::max_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointY);
				 Min = std::min_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointY);
				 std::pair <float, float> MeanSkoY = calculateMeanStdDv(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(),0.0, AddY);
				 seriesY->AddHighLow(i, Max->Y * RTS, Min->Y * RTS, Label);
				 seriesYMean->AddXY(i, MeanSkoY.first * RTS, Label);
				 seriesYSKO->AddHighLow(i, (MeanSkoY.first + MeanSkoY.second) * RTS, (MeanSkoY.first - MeanSkoY.second) * RTS , Label);

				 Max = std::max_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointY);
				 Min = std::min_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointY);
				 MeanSkoY = calculateMeanStdDv(AngleErrorValues.begin(), AngleErrorValues.end(),0.0, AddY);
				 seriesDl->AddHighLow(i, Max->Y * RTS, Min->Y * RTS, Label);
				 seriesDlSKO->AddHighLow(i, (MeanSkoY.first + MeanSkoY.second) * RTS, (MeanSkoY.first - MeanSkoY.second) * RTS , Label);
				 seriesDlMean->AddXY(i, MeanSkoY.first * RTS, Label);

				 struct { bool operator() (const Point& a,const Point& b) { return a.Z < b.Z; } } ComparePointZ;
				 struct { float operator() (const Point& a) { return a.Z;} } AddZ;

				 Max = std::max_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointZ);
				 Min = std::min_element(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(), ComparePointZ);
				 std::pair <float, float> MeanSkoZ = calculateMeanStdDv(AngularSpeedErrorValues.begin(), AngularSpeedErrorValues.end(),0.0, AddZ);
				 seriesZ->AddHighLow(i, Max->Z * RTS, Min->Z * RTS, Label);
				 seriesZMean->AddXY(i, MeanSkoZ.first * RTS, Label);
				 seriesZSKO->AddHighLow(i, (MeanSkoZ.first + MeanSkoZ.second) * RTS, (MeanSkoZ.first - MeanSkoZ.second) * RTS , Label);

				 Max = std::max_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointZ);
				 Min = std::min_element(AngleErrorValues.begin(), AngleErrorValues.end(), ComparePointZ);
				 MeanSkoZ = calculateMeanStdDv(AngleErrorValues.begin(), AngleErrorValues.end(),0.0, AddZ);
				 seriesAz->AddHighLow(i, Max->Z * RTS, Min->Z * RTS, Label);
				 seriesAzMean->AddXY(i, MeanSkoZ.first * RTS, Label);
				 seriesAzSKO->AddHighLow(i, (MeanSkoZ.first + MeanSkoZ.second) * RTS, (MeanSkoZ.first - MeanSkoZ.second) * RTS , Label);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeForm::SaveGraphsClick(TObject *Sender)
{
	UnicodeString ScreenFolderName = "\\" + FormatDateTime("yyyy-mm-dd hh.mm.ss", Now()) + " " + "Cкриншоты\\";
	TDirectory::CreateDirectoryW(GetCurrentDir() + ScreenFolderName);
	for (int i = 0; i < Charts.size(); i ++)
	{
		UnicodeString Title = LeftStr(Charts[i]->Title->Text->Text, PosEx(",", Charts[i]->Title->Text->Text, 1) - 1);
		Title = GetCurrentDir() + ScreenFolderName + Title;
		plotter->SaveChart(Charts[i], Title, 500, 900);
	}
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeForm::ClearGraphsClick(TObject *Sender)
{
	for (int i = 0; i < Charts.size(); i ++)
	{
		plotter->ClearChart(Charts[i]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TAnalyzeForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------------
