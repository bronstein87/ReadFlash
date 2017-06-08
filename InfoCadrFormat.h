#ifndef InfoCadr
#define InfoCadr
#include <vector>

struct StarsInfo
{
	float X, Y, Bright;
	unsigned long StarID;
	float Mv;
	char Sp[2];
};

struct ObjectsInfo
{
	float X, Y, Bright;
	unsigned short Square;
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
	bool IsBinary, IsReverse;   //признак бинирования
	unsigned short CountPixFilter;       //число пикселей выше порога
	unsigned short ImageWidth, ImageHeight;  //ширина и высота изображения
	unsigned short StatOrient;  //статус решения задачи (0 - успешно)
	unsigned short CountStars;  //число спроектированных звезд
	std::vector <StarsInfo>  StarsList;  //список спроектированных звезд
	unsigned short CountLocalObj, CountDeterObj;    //число локализованных и распознанных объектов
	std::vector <ObjectsInfo> ObjectsList;    //список локализованных объектов
	unsigned short CountWindows;  //число прогнозируемых окон
    unsigned short InfoCountWindows;  //общее число окон, часть из которых не попала в ДТМИ
	std::vector <WindowsInfo> WindowsList; //список прогнозируемых окон
	unsigned short CountLines, CountBlock;  //число считываемых строк и блоков с матрицы
	std::vector <LinesInfo> LinesList;  //список блоков
	double QuatOrient[4], MatrixOrient[3][3], AnglesOrient[3];   //кватернион, матрица и углы ориентации
	double OmegaOrient[3], MatrixProg[3][3]; //угловая скорость, прогнозируемая матрица ориентации
	double MatrixTemp;     //температура КМОП-матрицы

};





#endif
