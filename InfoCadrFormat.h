#ifndef InfoCadr
#define InfoCadr
#include <vector>

struct StarsInfo // прогнозируемые
{
	float X, Y, Bright;
	unsigned long StarID;
	float Mv;
	char Sp[2];
};

struct ObjectsInfo  // фактические объекты
{
	float X, Y, Bright;
	short Square;
	unsigned long StarID;
	float Mv;
	char Sp[2];
	float Dx, Dy;
};

struct WindowsInfo
{
	unsigned short Xstart, Ystart;
	unsigned short Width, Height, CountObj, ZipX, ZipY;
	float Mean, Sigma, Level;
	unsigned long StarID;
	float Mv;  //temporary
	char Sp[2]; //temporary
};

struct LinesInfo
{
	unsigned short Start, Height;
};

struct CadrInfo
{
	double Time;
	bool IsBinary, IsReverse;   //признак бинирования, признак реверса изображения
	bool IsOrient; //признак определения ориентации
	unsigned short CountPixFilter;       //число пикселей выше порога
	unsigned short ImageWidth, ImageHeight;  //ширина и высота изображения
	unsigned short StatOrient;  //статус решения задачи (0 - успешно)
	double MeanBright, SigmaBright;

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

	double QuatOrient[4], MatrixOrient[3][3], AnglesOrient[3];   //кватернион, матрица и углы ориентации
	double OmegaOrient[3], MatrixProg[3][3]; //угловая скорость, прогнозируемая матрица ориентации
	double MatrixTemp;     //температура КМОП-матрицы
	double MeanErrorX, MeanErrorY, MeanErrorXY;
};





#endif
