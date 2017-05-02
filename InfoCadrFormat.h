#ifndef InfoCadr
#define InfoCadr

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
	unsigned short Width, Height, CountObj;
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
	CadrInfo();
	double Time;
	bool IsBinary, IsReverse;   //признак бинирования
	unsigned short CountPixFilter;       //число пикселей выше порога
	unsigned short ImageWidth, ImageHeight;  //ширина и высота изображения
	unsigned short StatOrient;  //статус решения задачи (0 - успешно)
	unsigned short CountStars;  //число спроектированных звезд
	struct StarsInfo   *StarsList;  //список спроектированных звезд
	unsigned short CountLocalObj, CountDeterObj;    //число локализованных и распознанных объектов
	struct ObjectsInfo *ObjectsList;    //список локализованных объектов
	unsigned short CountWindows;  //число прогнозируемых окон
	struct WindowsInfo *WindowsList; //список прогнозируемых окон
	unsigned short CountLines, CountBlock;  //число считываемых строк и блоков с матрицы
	struct LinesInfo   *LinesList;  //список блоков
	double QuatOrient[4], MatrixOrient[3][3], AnglesOrient[3];   //кватернион, матрица и углы ориентации
	double OmegaOrient[3], MatrixProg[3][3]; //угловая скорость, прогнозируемая матрица ориентации
	double MatrixTemp;     //температура КМОП-матрицы

	~CadrInfo();
};


	CadrInfo::CadrInfo()
	{
		StarsList = NULL;
		ObjectsList = NULL;
		LinesList = NULL;
		WindowsList = NULL

	}
	 CadrInfo::~CadrInfo()
	 {

		delete [] StarsList;
		delete [] ObjectsList;
		delete [] LinesList;
		delete [] WindowsList;
	}


#endif
