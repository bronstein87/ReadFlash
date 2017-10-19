#ifndef IKI_IMG_H
#define IKI_IMG_H
#include <fstream>
#include <stdio.h>
//#include <Classes.hpp>
#include <string.h>
#include <iostream>   // std::cout
#include <sstream>
#include <math.h>

using namespace std;

#pragma region DEFINE

#define StartWord  								0x7678D48B
#define StartId                 				0xABCD

#define ROUND(N) (N<0?ceil((N)-0.5):floor((N)+0.5))
#define VERSION_ID                              "1.22.10.17"

#pragma region whenwhere
#define whenwhere_id                            0xAAAA
#define datetime_id								0x111A
#define	soft_vers_id                            0x112A
#define frame_num_id                            0x113A
#define protons_density_id                      0x114A
#define tracks_brightness_id     	        	0x115A
#define moon_velocity_id                        0x116A
#define moon_brightness_id                      0x117A
#define bright_left_id                          0x118A
#define bright_right_id         	        	0x119A
#define era_id	  								0x11AA
#define orbit_eccentricity_id                   0x11BA
#define length_semiaxis_id       	        	0x11CA
#define longitude_node_id 						0x11DA
#define inclination_orbit_id                    0x11EA
#define argument_perigee_id      	        	0x11FA
#define dev_angular_velocity_id		        	0x12FA
#define dev_angular_acceler_id                  0x13FA
#define type_angle_id							0x14FA
#define type_quat_id            	        	0x15FA
#define type_matr_id							0x16FA
#define proj_line_velo_id						0x17FA
#define year_aber_app_id						0x18FA
#define satel_aber_app_id						0x19FA
#define preces_nutat_app_id	 	 				0x1AFA
#define photon_noise_app_id						0x1BFA
#define distortion_app_id                       0x1CFA

#define length_radius_vector_id                 0x1DFA
#define argument_latitude_id                    0x1EFA

#define datetime_name							" Дата и время приема кадра"
#define	soft_vers_name                          " Версия драйвера структуры"
#define frame_num_name                          " Номер кадра"
#define protons_density_name                    " Плотность потока протонов"
#define tracks_brightness_name     	        	" Яркость треков"
#define moon_velocity_name                      " Угловая скорость образа Луны"
#define moon_brightness_name                    " Яркость образа Луны"
#define bright_left_name                        " Яркость левой границы кадра"
#define bright_right_name         	        	" Яркость правой границы кадра"
#define era_name								" Эпоха"
#define orbit_eccentricity_name                 " Эксцентриситет орбиты [0...1]"
#define length_semiaxis_name       	        	" Длина главной полуоси [км]"
#define longitude_node_name                     " Долгота восходящего узла"
#define inclination_orbit_name                  " Наклонение орбиты к экватору"
#define argument_perigee_name      	        	" Аргумент перигея"
#define dev_angular_velocity_name	        	" Угловая скорость прибора [рад/сек]"
#define dev_angular_acceler_name                " Угловое ускорение прибора [рад/сек^2]"
#define type_angle_name							" Углы ориентации [рад]: al dl az"
#define type_quat_name							" Кватернион ориентации"
#define type_matr_name							" Матрица ориентации"
#define proj_line_velo_name						" Линейная скорость [км/сек]"
#define year_aber_app_name						" Учет годичной аберрации"
#define satel_aber_app_name						" Учет спутниковой аберрации"
#define preces_nutat_app_name		        	" Учет прецессии/нутации"
#define photon_noise_app_name					" Учет фотонного шума"
#define distortion_app_name                     " Учет дисторсии оптической системы"
#define length_radius_vector_name               " Длина радиус вектора"
#define argument_latitude_name                  " Аргумент широты"

#pragma end_region

#pragma region devicetype
#define devicetype_id                           0xBBBB
#define device_type_id							0x111B
#define device_sn_id							0x112B
#define inner_radius_id          	        	0x113B
#define outer_radius_id                         0x114B
#define num_elec_disc_id						0x115B
#define resolution_acp_id						0x116B
#define ev_acp_id                               0x117B
#define pixel_size_id							0x118B
#define focal_length_id							0x119B
#define diametr_entrace_pupil_id	        	0x11AB
#define dark_level_id							0x11BB
#define dark_sko_id								0x11CB
#define limit_magn_id                           0x11DB
#define stars_dist_id                           0x11EB
#define function_distortion_id		        	0x11FB
#define dead_pix_id								0x12FB
#define matrixtemp_id                           0x13FB
#define frequency_id                            0x14FB
#define device_type_name						"Тип прибора"
#define device_sn_name							"Сериальный номер прибора"
#define inner_radius_name          	        	"Внутренний радиус виньетирования"
#define outer_radius_name                       "Внешний радиус виньетирования"
#define num_elec_disc_name						"Число электронов в разряде"
#define resolution_acp_name						"Разрядность АЦП"
#define ev_acp_name                             "Число электронов в разряде АЦП"
#define pixel_size_name							"Размер пикселя [мм]"
#define focal_length_name						"Фокусное расстояние [мм]"
#define diametr_entrace_pupil_name	        	"Диаметр входного зрачка [мм]"
#define dark_level_name							"Уровень темнового тока"
#define dark_sko_name							"СКО темнового тока"
#define limit_magn_name                         "Предельная звездная величина"
#define stars_dist_name 						"СКО распределения яркости звезд"
#define function_distortion_name                "Функция дисторсии"
#define dead_pix_name    						"Таблица битых пикселей"
#define matrixtemp_name                         "Температура матрицы"
#define frequency_name                          "Частота прибора"
#pragma end_region

#pragma region filterframe
#define filterframe_id                          0xCCCC
#define filter_pix_id							0x111C
#define bottomright_id			        		0x112C
#define	filter_pix_name                         "Таблица фильтрованных пикселей"
#define bottomright_name                        "Флаг типа угла, false -  лв, true - пн"
#pragma end_region

#pragma region starsinfo
#define starsinfo_id                            0xDDDD
#define rezstat_id                              0x111D
#define epsilon_id                              0x112D
#define m_X_Y_Cyr_id							0x113D
#define local_recog_id							0x11AD
#define rec_id									0x11BD
#define rec_orient_angl_id                      0x116D
#define rec_orient_quat_id						0x117D
#define rec_orient_matr_id						0x118D
#define rec_angular_vel_id                      0x119D
#define	sizelocallist_id                        0x114D
#define sizestarlist_id                         0x115D

#define rezstat_name                            "Результат распознавания"
#define epsilon_name							"Допуск на распознавание"
#define m_X_Y_Cyr_name                          "Ошибка решения системы (мм)"
#define	local_recog_name                        "Справочное число локализованных объектов"
#define rec_name								"Данные смоделированного кадра"
#define rec_orient_angl_name                    "Распознанные углы ориентации [рад]: al dl az"
#define rec_orient_quat_name                    "Распознанный кватернион ориентации"
#define rec_orient_matr_name                    "Распознанная матрица ориентации"
#define rec_angular_vel_name                    "Рассчитанная угловая скорость"
#define	sizelocallist_name                      "Количество локализованных/распознанных объектов в списке"
#define sizestarlist_name                       "Количество звезд в списке"
#pragma end_region

#pragma region videodata
#define	videodata_id               	        	0xEEEE
#define	videoframe_id                           0x111E
#define	videolines_id                           0x112E
#define	videowindow_id                          0x113E

#define	exp_time_id                             0x110E

#define	stringframe_id                          0x114E
#define	stringframe_type_id                     0x124E
#define	stringframe_deg_bin_id                  0x134E
#define	stringframe_width_id                    0x144E
#define	stringframe_height_id                   0x164E
#define	stringframe_SizeData_id                 0x174E

#define	stringlines_id                          0x115E
#define	stringlines_count_id                    0x125E
#define	stringlines_type_id                     0x135E
#define	stringlines_deg_bin_id                  0x145E
#define	stringlines_width_id                    0x155E
#define	stringlines_height_id                   0x165E
#define	stringlines_info_id                     0x175E
#define	stringlines_SizeData_id                 0x185E

#define	stringwindow_id                         0x116E
#define	stringwindow_count_id                   0x186E
#define	stringwindow_type_id                    0x136E
#define	stringwindow_deg_bin_id                 0x146E
#define	stringwindow_SizeData_id                0x156E
#define	stringwindow_flag_id                    0x166E
#define	stringwindow_info_id                    0x176E
#define	stringwindow_sizewindowlist_id          0x126E

#define	videoframe_name                         "Полный кадр"
#define	videolines_name                         "Полосы с кадра"
#define	videowindow_name						"Окна с кадра"

#define	exp_time		                        "Время экспозици"

#define	stringframe_type                        "Тип данных полного кадра(0-unsigned char, 1-ushort, 2-float)"
#define	stringframe_deg_bin                     "Cтепень бинирования полного кадра"
#define	stringframe_width                       "Ширина полного кадра"
#define	stringframe_height                      "Высота полного кадра"
#define	stringframe_SizeData                    "Суммарный размер полного кадра в пикселях"

#define	stringlines_count                       "Количество полос"
#define	stringlines_type                        "Тип данных полос с кадра(0-unsigned char, 1-ushort, 2-float)"
#define	stringlines_deg_bin                     "Cтепень бинирования полос с кадра"
#define	stringlines_SizeData                    "Суммарный размер полос в пикселях"
#define	stringlines_width                       "Ширина блока полос"
#define	stringlines_height                      "Высота блока полос"
#define	stringlines_info                      	"Параметры полос"
#define	stringwindow_sizewindowlist             "Фактический размер массива окон"

#define	stringwindow_count                      "Справочное количество окон"
#define	stringwindow_type                       "Тип данных окон(0-unsigned char, 1-ushort, 2-float)"
#define	stringwindow_deg_bin                    "Cтепень бинирования полос с кадра"
#define	stringwindow_SizeData                   "Суммарный размер окон в пикселях"
#define	stringwindow_flag                       "Угол отсчета координат: false-лв, true-пн"
#define	stringwindow_info                       "Параметры окон"

#pragma end_region

#pragma end_region

#pragma region Структуры

struct STARREC
{
	unsigned short Ns; // номер звезды
	unsigned long Is; // интегральная яркость
	float Xs, Ys; // координаты энергетического центра
	float Mv; // звездная величина
	char Sp[2]; // спектральный класс

	STARREC() {
		Is = 0; Xs = 0; Ys = 0; Mv = 0; Sp[0] = '_'; Sp[1] = '_';
	}
};

struct REC
{
	unsigned short strrec; // число звезд на кадре
	unsigned short SizeStarList; // количество звезд в списке
	STARREC* StarRec; // таблица сформированных звезд
};

struct coefonaxis
{
	double X; // коэффициент по оси Х
	double Y; // коэффициент по оси У

	coefonaxis() {
		X = 0; Y = 0;
	}
};

struct funcdist
{
	unsigned short PolynomialDegree; // степень полинома
	unsigned short CoefficientsCount; // количество коэффициентво
	coefonaxis* Coefficient;
};

struct infopix // информация о битом пикселе
{
	float X_coordinate; // координата Х
	float Y_coordinate; // координата У
	float BrightnessObj; // яркость объекта
	unsigned short CoefficientObj; // размер объекта

	infopix() {
		X_coordinate = 0; Y_coordinate = 0; BrightnessObj = 0; CoefficientObj = 0;
	}
};

struct deadpixels // структура битых пикселей
{
	unsigned short RecordsCount; // количество записей
	infopix* PixelData; // массив записей информации и пикселях
};

struct fullframe
{
	unsigned int DataType; // признак типа 0-unsigned char, 1-ushort, 2-float
	unsigned int DegreeBinning; // степень бинирования
	unsigned int FrameWidth; // ширина кадра
	unsigned int FrameHeight; // высота кадра
	unsigned int SizeData; // размер информации (S+=W[i]*H[i])
	void* Data;
};

struct dataline
{
	int Y_FirstString; // координата У начала полосы
	int CountString; // высота полосы

	dataline() {
		Y_FirstString = 0; CountString = 0;
	}
};

struct setlines
{
	unsigned short LinesCount; // количество полос
	unsigned int DataType; // признак типа 0-unsigned char, 1-ushort, 2-float
	unsigned int DegreeBinning; // степень бинирования
	unsigned int LinesWidth; // ширина блока полос
	unsigned int LinesHeight; // высота блока полос
	unsigned int SizeData; // размер информации (S+=W[i]*H[i])
	dataline* Info; // информация о полосах
	void* Data; // бинарные данные
};

struct datawindow
{
	unsigned int WindowWidth; // ширина окна
	unsigned int WindowHeight; // высота окна
	unsigned int X; // координата угла окна Х
	unsigned int Y; // координата угла окна У
	unsigned int Limit; // порог
	unsigned int ObjCount; // количество объектов в окне
	float SKO; // СКО
	float Average; // среднее
	unsigned short ZipX; // коэффициент сжатия по Х
	unsigned short ZipY; // коэффициент сжатия по У

	datawindow() {
		WindowWidth = 0; WindowHeight = 0; X = 0; Y = 0; Limit = 0; ObjCount = 0; SKO = 0; Average = 0; ZipX = 0; ZipY = 0;
	}
};

struct setwindow
{
	unsigned short SizeWindowList; // Количество окон со звездами
	unsigned short WindowCount; // количество окон
	unsigned int DataType; // признак типа 0-unsigned char, 1-ushort, 2-float
	unsigned int DegreeBinning; // степень бинирования
	unsigned int SizeData; // размер информации (S+=W[i]*H[i])
	bool BottomRight; // флаг типа угла, false -  лв, true - пн
	datawindow* Info; // информация о окнах
	void* Data; // бинарные данные
};

struct whenwhere
{
	string DateTime; // дата и время кадра в формате "dd.mm.yyyy hh:nn:ss.zzz"
	string SoftwareVersion; // версия программы
	int FrameNumber; // номер кадра
	int ProtonsDensity; // плотность потока протонов
	int TracksBrightness; // яркость треков
	int MoonVelocity; // угловая скорость образа Луны
	int MoonBrightness; // яркость образа Луны
	int BrightnessLeft; // яркость левой границы кадра
	int BrightnessRight; // яркость правой границы кадра
	float Era; // эпоха
	double OrbitEccentricity; // эксцентриситет орбиты [0,..1)
	double LengthSemiaxis; // длина главной полуоси [км]
	double LengthRadiusVector; // длина радиус вектора [км]
	double LongitudeAscendingNode; // долгота восходящего угла
	double InclinationOrbit; // наклонение орбиты к экватору
	double ArgumentPerigee; // аргумент перигея
	double ArgumentLatitude; // аргумент широты
	double DeviceAngularVelocity[3]; // угловая скорость вращения прибора
	double DeviceAngularAcceleration[3]; // угловое ускорение
	double ProjectionLinearVelocity[3]; // проекция линейной скорости
	double OrientationAngles[3]; // углы ориентации
	double OrientationQuaternion[4]; // кватернион ориентации
	double OrientationMatrix[3][3]; // матрица ориентации
	bool YearAberrationApply; // учет годичной аберрации
	bool SatelliteAberrationApply; // учет спутниковой аберрации
	bool PrecessionNutationApply; // учет прецессии нутации
	bool PhotonNoiseApply; // учет фотонного шума
	bool DistortionApply; // учет дисторсии оптической системы
};

struct devicetype
{
	string DataSource; // источник данных
	int SNDevice; // сериальный номер прибора
	int InnerRadius; // внутренний радиус виньетирования
	int OuterRadius; // внешний радиус виньетирования
	int ResolutionACP; // разрядность АЦП
	unsigned short Frequency; // частота прибора
	float EV_ACP; // число электронов в одном разряде АЦП
	float PixelSize; // размер пикселя
	float FocalLength; // фокусное расстояние
	float DiameterEntrancePupil; // диаметр входного зрачка
	float DarkLevel; // уровень темнового тока
	float DarkSKO; // СКО темнового тока
	float LimitingMagnitude; // предельная звездная величина
	float StarsDistribution; // СКО распределения яркости звезд
	double MatrixTemp; // Температура матрицы
	funcdist DistortionFunction;
	deadpixels TableDeadObjects;
};

struct filterdata
{
	unsigned short X_coordinate; // координата Х
	unsigned short Y_coordinate; // координата У
	unsigned short BrightnessPixel; // яркость

	filterdata() {
		X_coordinate = 0; Y_coordinate = 0; BrightnessPixel = 0;
	}
};

struct filterframe
{
	string TagFilter; // название фильтра
	unsigned short FilteredPixelsCount; // количество фильтрованных пикселей
	bool BottomRight; // флаг типа угла, false -  лв, true - пн
	filterdata* DataPixels; // информация о пиксели
};

struct starinfo
{
	float BrightnessObject; // яркость объекта
	float PixelsCount; // количество пикселей
	float X_coordinate; // координата Х
	float Y_coordinate; // координата У

	unsigned short NumberStar; // номер звезды в каталоге
	float StellarMagnitude; // звездная величина
	char SpectralClass[2]; // спектральный класс
	float DX; // ошибка по оси Х
	float DY; // ошибка по оси У

	starinfo() {
		BrightnessObject = 0; PixelsCount = 0; X_coordinate = 0; Y_coordinate = 0; NumberStar = 0; StellarMagnitude = 0; SpectralClass[0] = '_'; SpectralClass[1] = '_'; DX = 0; DY = 0;
	}
};

struct starsinfo
{
	unsigned short RezStat; // результат распознавания
	float Epsilon; // допуск на распознавание
	float m_X, m_Y, m_Cur; // ошибка решения системы (мм)
	int LocalizedCount; // количество локализованных объектов
	int RecognizedCount; // количество распознанных объектов
	int SizeLocalList; // количество объектов в списке локализации
	double RecognizedOrientationAngles[3]; // углы ориентации
	double RecognizedOrientationQuaternion[4]; // кватернион ориентации
	double RecognizedOrientationMatrix[3][3]; // матрица ориентации
	double RecognizedAngularVelocity[3]; // угловая скорость
	starinfo* StarsList; // информация о локал/расп объекте
	REC SimulatedFrame; // смоделированный кадр
};

struct videoframe
{
	float ExposureTime; // время экспонирования
	fullframe FrameData; // полный кадр
	setlines LinesData; // полосы
	setwindow WindowsData; // окна
};







#pragma end_region

const int MassType[3] = {1, 2, 4};

class IKI_img
{
private:
    template <typename T>
    static void _FreeMemory(T*&point)
    {
        delete point;
        point = NULL;
    }
    // ---------------------------------------------------------------------------

    template <typename T>
    static void _FreeMemoryArray (T*&array)
    {
        delete[]array;
        array = NULL;
    }
    // ---------------------------------------------------------------------------

    template <typename T>
    string toString(T val)
    {
        ostringstream oss;
        oss << val;
        return oss.str();
    }
    // ---------------------------------------------------------------------------

    template <typename T>
    string FormatFloatTo_string(string Format, T val)
    {
        string bufer_str;
        char * st_buf = new char[100];
        sprintf(st_buf, Format.c_str(), val);
        bufer_str = st_buf;
        delete[]st_buf;
        return bufer_str;
    }
	// ---------------------------------------------------------------------------
	void print_string(FILE* FStream, unsigned int ID, string name, string value);
	void print_data(FILE* FStream, string value);

    const string BoolToString(bool b);

    const bool StringToBool(string b);

    string ArcsecToStr(double arcsec);

    double StrToArcsec(string _str);

    void StringToFloatMass(double(*mass), unsigned int N, string _str);

    void StringToFloatMass3D(double(*mas3d)[3], unsigned int N, unsigned int M, string _str);

public:

    whenwhere Georeferencing; // когда? где?
    devicetype CameraSettings; // параметры камеры
    filterframe FilterData; // фильтрованное изображение
    starsinfo StarsData; // локализованные/распознанные объекты
    videoframe ImageData; // структура данных изображения

	IKI_img(void);
	~IKI_img();
    void WriteFormat(string filename);
    void Clear_Memory(void);
    bool ReadFormat(string filename, bool Need_Erase);
    unsigned char* ZipStructure(unsigned int &SizeInfo);
    void UnZipStructure(unsigned char* Array, unsigned int SizeInfo);
};
#endif
