#ifndef INFO_CADR_H
#define INFO_CADR_H
#include <vector>
#include <string>
#include "MathM.h"

struct StarsInfo // прогнозируемые
{
	__fastcall StarsInfo() :X(0), Y(0), Bright(0), StarID(0),
	Mv(0)
	{
		 Sp[0] = '_';
		 Sp[1] = '_';
	}
	float X, Y, Bright;
	unsigned long StarID;
	float Mv;
	char Sp[2];
};

struct ObjectsInfo  // фактические объекты
{
	__fastcall ObjectsInfo() :X(0), Y(0), Bright(0),Square(0), StarID(0),
	Mv(0), Dx(0), Dy(0)
	{
		 Sp[0] = '_';
		 Sp[1] = '_';
	}
	float X, Y;
	unsigned short Bright;
	short Square;
	unsigned long StarID;
	float Mv;
	char Sp[2];
	float Dx, Dy;
};

struct WindowsInfo
{
	__fastcall WindowsInfo() : Xstart(0), Ystart(0),
	Width(0),Height(0), CountObj(0), ZipX(0), ZipY(0),
	Mean(0), Sigma(0), Level(0), StarID(0), Mv(0)
	{
		 Sp[0] = '_';
		 Sp[1] = '_';
	}
	unsigned short Xstart, Ystart;
	unsigned short Width, Height, CountObj, ZipX, ZipY;
	float Mean, Sigma, Level;
	unsigned long StarID;
	float Mv;  //temporary
	char Sp[2]; //temporary
	float xCenter;  // координаты центра звезды относительно начала фрагмента
	float yCenter;  // координаты центра звезды относительно начала фрагмента
	int Bright;
};

struct LinesInfo
{
	unsigned short Start, Height;
};

struct CadrInfo
{
	_fastcall CadrInfo(): Time(0), FrameNumber(0), CountStars(0), CountWindows(0),
	CountLocalObj(0), CountDeterObj(0), SizeStarsList(0), SizeWindowsList(0),
	 CountLines(0), CountBlock(0), MeanErrorX(0), MeanErrorY(0), MeanErrorXY(0),
	  QuatOrient(), AnglesOrient(), AnglesModel(), //кватернион, матрица и углы ориентации
	 AnglesDiff(),  OmegaOrient(), OmegaModel(), //угловая скорость, прогнозируемая матрица ориентации
	 OmegaDiff(), AxesDiff() { }

	double Time;
	std::string DeviceInfo;
	int FrameNumber;
	bool IsBinary, IsReverse;   //признак бинирования, признак реверса изображения
	int ResolutionACP;
	bool IsOrient; //признак определения ориентации
	unsigned short CountPixFilter;       //число пикселей выше порога
	unsigned short ImageWidth, ImageHeight;  //ширина и высота изображения
	double SizePixel;
	unsigned short StatOrient;  //статус решения задачи (0 - успешно)
	unsigned int DataType; // признак типа 0-byte, 1-ushort, 2-float
	double MeanBright, SigmaBright;
	float Epsilon; // допуск на распознавание

	unsigned short CountStars;  //число спроектированных звезд
	unsigned short CountWindows;  //число прогнозируемых окон
	unsigned short CountLocalObj, CountDeterObj;    //число локализованных и распознанных объектов

	unsigned short SizeStarsList;
	std::vector <StarsInfo>  StarsList;  //список спроектированных звезд

	unsigned short SizeObjectsList;
	std::vector <ObjectsInfo> ObjectsList;    //список локализованных объектов

	unsigned short SizeWindowsList;
	std::vector <WindowsInfo> WindowsList; //список прогнозируемых окон

	unsigned short CountLines, CountBlock;  //число считываемых строк и блоков с матрицы
	std::vector <LinesInfo> LinesList;  //список блоков

	double QuatOrient[4], MatrixOrient[3][3], AnglesOrient[3], AnglesModel[3];   //кватернион, матрица и углы ориентации
	double AnglesDiff[3];
	double OmegaOrient[3], OmegaModel[3], MatrixProg[3][3]; //угловая скорость, прогнозируемая матрица ориентации
	double OmegaDiff[3];
	double AxesDiff[3];
	double MatrixTemp;     //температура КМОП-матрицы
	double MeanErrorX, MeanErrorY, MeanErrorXY;
};


 void GetMeanDeterError(struct CadrInfo &mCadr)
{
int countDxDy = 0;

	mCadr.MeanErrorX = 0.;
	mCadr.MeanErrorY = 0.;
	mCadr.MeanErrorXY = 0.;

	for (int iLocalObj = 0; iLocalObj < mCadr.SizeObjectsList; iLocalObj++) {
		double Dx_2 = mCadr.ObjectsList[iLocalObj].Dx * mCadr.ObjectsList[iLocalObj].Dx;
		double Dy_2 = mCadr.ObjectsList[iLocalObj].Dy * mCadr.ObjectsList[iLocalObj].Dy;
		if ((Dx_2 > 1e-20) && (Dy_2 > 1e-20)) {
			mCadr.MeanErrorX += Dx_2;
			mCadr.MeanErrorY += Dy_2;
			mCadr.MeanErrorXY += Dx_2 + Dy_2;
			countDxDy++;
		}
	}

	if (countDxDy == mCadr.CountDeterObj) {
		mCadr.MeanErrorX = sqrtm(mCadr.MeanErrorX / (double)(mCadr.CountDeterObj - 1));
		mCadr.MeanErrorY = sqrtm(mCadr.MeanErrorY / (double)(mCadr.CountDeterObj - 1));
		mCadr.MeanErrorXY = sqrtm(mCadr.MeanErrorXY / (double)(2 * mCadr.CountDeterObj - 1));
	}
}

void GetImageBright(struct CadrInfo &mCadr)
{
	mCadr.MeanBright = 0.;
	mCadr.SigmaBright = 0.;
	for (int iWindow = 0; iWindow < mCadr.SizeWindowsList; iWindow++) {
		mCadr.MeanBright += mCadr.WindowsList[iWindow].Mean;
		mCadr.SigmaBright += mCadr.WindowsList[iWindow].Sigma;
	}

	if (mCadr.SizeWindowsList) {
		mCadr.MeanBright /= mCadr.SizeWindowsList;
		mCadr.SigmaBright /= mCadr.SizeWindowsList;
	}
}


void calcOmegaDiff(std::vector <CadrInfo>& cadrInfoVec)
{
	double MOrientFirst[3][3];
	double MOrientSecond[3][3];
	double Wop [3];
	double dTime = 1. / 8.;
	for (unsigned int i = 0; i < cadrInfoVec.size() - 1; i++)
	{
		if (cadrInfoVec[i].IsOrient && cadrInfoVec[i + 1].IsOrient)
		{
			calcTransitionMatrix(cadrInfoVec[i].AnglesOrient[0]
			, cadrInfoVec[i].AnglesOrient[1]
			, cadrInfoVec[i].AnglesOrient[2]
			, MOrientFirst);

			calcTransitionMatrix(cadrInfoVec[i + 1].AnglesOrient[0]
			, cadrInfoVec[i + 1].AnglesOrient[1]
			, cadrInfoVec[i + 1].AnglesOrient[2]
			, MOrientSecond);

			double checkangle1[3];
			double checkangle2[3];
			MatrixToEkvAngles(MOrientFirst, checkangle1);
			MatrixToEkvAngles(MOrientSecond, checkangle2);
			getAngularDisplacementFromOrientMatr(MOrientFirst, MOrientSecond, Wop);

			(cadrInfoVec[i + 1].AnglesOrient[0] - cadrInfoVec[i].AnglesOrient[0])/ dTime;
			(cadrInfoVec[i + 1].AnglesOrient[1] - cadrInfoVec[i].AnglesOrient[1])/ dTime;
			(cadrInfoVec[i + 1].AnglesOrient[2] - cadrInfoVec[i].AnglesOrient[2])/ dTime;

			for (int j = 0; j < 3;j ++ )
			{
				cadrInfoVec[i + 1].OmegaDiff[j] = cadrInfoVec[i + 1].OmegaDiff[j] - (Wop[j] / dTime);
			}
		}
	}

}


#endif
