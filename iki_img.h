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

#define datetime_name							" ���� � ����� ������ �����"
#define	soft_vers_name                          " ������ �������� ���������"
#define frame_num_name                          " ����� �����"
#define protons_density_name                    " ��������� ������ ��������"
#define tracks_brightness_name     	        	" ������� ������"
#define moon_velocity_name                      " ������� �������� ������ ����"
#define moon_brightness_name                    " ������� ������ ����"
#define bright_left_name                        " ������� ����� ������� �����"
#define bright_right_name         	        	" ������� ������ ������� �����"
#define era_name								" �����"
#define orbit_eccentricity_name                 " �������������� ������ [0...1]"
#define length_semiaxis_name       	        	" ����� ������� ������� [��]"
#define longitude_node_name                     " ������� ����������� ����"
#define inclination_orbit_name                  " ���������� ������ � ��������"
#define argument_perigee_name      	        	" �������� �������"
#define dev_angular_velocity_name	        	" ������� �������� ������� [���/���]"
#define dev_angular_acceler_name                " ������� ��������� ������� [���/���^2]"
#define type_angle_name							" ���� ���������� [���]: al dl az"
#define type_quat_name							" ���������� ����������"
#define type_matr_name							" ������� ����������"
#define proj_line_velo_name						" �������� �������� [��/���]"
#define year_aber_app_name						" ���� �������� ���������"
#define satel_aber_app_name						" ���� ����������� ���������"
#define preces_nutat_app_name		        	" ���� ���������/�������"
#define photon_noise_app_name					" ���� ��������� ����"
#define distortion_app_name                     " ���� ��������� ���������� �������"
#define length_radius_vector_name               " ����� ������ �������"
#define argument_latitude_name                  " �������� ������"

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
#define device_type_name						"��� �������"
#define device_sn_name							"���������� ����� �������"
#define inner_radius_name          	        	"���������� ������ ��������������"
#define outer_radius_name                       "������� ������ ��������������"
#define num_elec_disc_name						"����� ���������� � �������"
#define resolution_acp_name						"����������� ���"
#define ev_acp_name                             "����� ���������� � ������� ���"
#define pixel_size_name							"������ ������� [��]"
#define focal_length_name						"�������� ���������� [��]"
#define diametr_entrace_pupil_name	        	"������� �������� ������ [��]"
#define dark_level_name							"������� ��������� ����"
#define dark_sko_name							"��� ��������� ����"
#define limit_magn_name                         "���������� �������� ��������"
#define stars_dist_name 						"��� ������������� ������� �����"
#define function_distortion_name                "������� ���������"
#define dead_pix_name    						"������� ����� ��������"
#define matrixtemp_name                         "����������� �������"
#define frequency_name                          "������� �������"
#pragma end_region

#pragma region filterframe
#define filterframe_id                          0xCCCC
#define filter_pix_id							0x111C
#define bottomright_id			        		0x112C
#define	filter_pix_name                         "������� ������������� ��������"
#define bottomright_name                        "���� ���� ����, false -  ��, true - ��"
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

#define rezstat_name                            "��������� �������������"
#define epsilon_name							"������ �� �������������"
#define m_X_Y_Cyr_name                          "������ ������� ������� (��)"
#define	local_recog_name                        "���������� ����� �������������� ��������"
#define rec_name								"������ ���������������� �����"
#define rec_orient_angl_name                    "������������ ���� ���������� [���]: al dl az"
#define rec_orient_quat_name                    "������������ ���������� ����������"
#define rec_orient_matr_name                    "������������ ������� ����������"
#define rec_angular_vel_name                    "������������ ������� ��������"
#define	sizelocallist_name                      "���������� ��������������/������������ �������� � ������"
#define sizestarlist_name                       "���������� ����� � ������"
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

#define	videoframe_name                         "������ ����"
#define	videolines_name                         "������ � �����"
#define	videowindow_name						"���� � �����"

#define	exp_time		                        "����� ���������"

#define	stringframe_type                        "��� ������ ������� �����(0-unsigned char, 1-ushort, 2-float)"
#define	stringframe_deg_bin                     "C������ ����������� ������� �����"
#define	stringframe_width                       "������ ������� �����"
#define	stringframe_height                      "������ ������� �����"
#define	stringframe_SizeData                    "��������� ������ ������� ����� � ��������"

#define	stringlines_count                       "���������� �����"
#define	stringlines_type                        "��� ������ ����� � �����(0-unsigned char, 1-ushort, 2-float)"
#define	stringlines_deg_bin                     "C������ ����������� ����� � �����"
#define	stringlines_SizeData                    "��������� ������ ����� � ��������"
#define	stringlines_width                       "������ ����� �����"
#define	stringlines_height                      "������ ����� �����"
#define	stringlines_info                      	"��������� �����"
#define	stringwindow_sizewindowlist             "����������� ������ ������� ����"

#define	stringwindow_count                      "���������� ���������� ����"
#define	stringwindow_type                       "��� ������ ����(0-unsigned char, 1-ushort, 2-float)"
#define	stringwindow_deg_bin                    "C������ ����������� ����� � �����"
#define	stringwindow_SizeData                   "��������� ������ ���� � ��������"
#define	stringwindow_flag                       "���� ������� ���������: false-��, true-��"
#define	stringwindow_info                       "��������� ����"

#pragma end_region

#pragma end_region

#pragma region ���������

struct STARREC
{
	unsigned short Ns; // ����� ������
	unsigned long Is; // ������������ �������
	float Xs, Ys; // ���������� ��������������� ������
	float Mv; // �������� ��������
	char Sp[2]; // ������������ �����

	STARREC() {
		Is = 0; Xs = 0; Ys = 0; Mv = 0; Sp[0] = '_'; Sp[1] = '_';
	}
};

struct REC
{
	unsigned short strrec; // ����� ����� �� �����
	unsigned short SizeStarList; // ���������� ����� � ������
	STARREC* StarRec; // ������� �������������� �����
};

struct coefonaxis
{
	double X; // ����������� �� ��� �
	double Y; // ����������� �� ��� �

	coefonaxis() {
		X = 0; Y = 0;
	}
};

struct funcdist
{
	unsigned short PolynomialDegree; // ������� ��������
	unsigned short CoefficientsCount; // ���������� �������������
	coefonaxis* Coefficient;
};

struct infopix // ���������� � ����� �������
{
	float X_coordinate; // ���������� �
	float Y_coordinate; // ���������� �
	float BrightnessObj; // ������� �������
	unsigned short CoefficientObj; // ������ �������

	infopix() {
		X_coordinate = 0; Y_coordinate = 0; BrightnessObj = 0; CoefficientObj = 0;
	}
};

struct deadpixels // ��������� ����� ��������
{
	unsigned short RecordsCount; // ���������� �������
	infopix* PixelData; // ������ ������� ���������� � ��������
};

struct fullframe
{
	unsigned int DataType; // ������� ���� 0-unsigned char, 1-ushort, 2-float
	unsigned int DegreeBinning; // ������� �����������
	unsigned int FrameWidth; // ������ �����
	unsigned int FrameHeight; // ������ �����
	unsigned int SizeData; // ������ ���������� (S+=W[i]*H[i])
	void* Data;
};

struct dataline
{
	int Y_FirstString; // ���������� � ������ ������
	int CountString; // ������ ������

	dataline() {
		Y_FirstString = 0; CountString = 0;
	}
};

struct setlines
{
	unsigned short LinesCount; // ���������� �����
	unsigned int DataType; // ������� ���� 0-unsigned char, 1-ushort, 2-float
	unsigned int DegreeBinning; // ������� �����������
	unsigned int LinesWidth; // ������ ����� �����
	unsigned int LinesHeight; // ������ ����� �����
	unsigned int SizeData; // ������ ���������� (S+=W[i]*H[i])
	dataline* Info; // ���������� � �������
	void* Data; // �������� ������
};

struct datawindow
{
	unsigned int WindowWidth; // ������ ����
	unsigned int WindowHeight; // ������ ����
	unsigned int X; // ���������� ���� ���� �
	unsigned int Y; // ���������� ���� ���� �
	unsigned int Limit; // �����
	unsigned int ObjCount; // ���������� �������� � ����
	float SKO; // ���
	float Average; // �������
	unsigned short ZipX; // ����������� ������ �� �
	unsigned short ZipY; // ����������� ������ �� �

	datawindow() {
		WindowWidth = 0; WindowHeight = 0; X = 0; Y = 0; Limit = 0; ObjCount = 0; SKO = 0; Average = 0; ZipX = 0; ZipY = 0;
	}
};

struct setwindow
{
	unsigned short SizeWindowList; // ���������� ���� �� ��������
	unsigned short WindowCount; // ���������� ����
	unsigned int DataType; // ������� ���� 0-unsigned char, 1-ushort, 2-float
	unsigned int DegreeBinning; // ������� �����������
	unsigned int SizeData; // ������ ���������� (S+=W[i]*H[i])
	bool BottomRight; // ���� ���� ����, false -  ��, true - ��
	datawindow* Info; // ���������� � �����
	void* Data; // �������� ������
};

struct whenwhere
{
	string DateTime; // ���� � ����� ����� � ������� "dd.mm.yyyy hh:nn:ss.zzz"
	string SoftwareVersion; // ������ ���������
	int FrameNumber; // ����� �����
	int ProtonsDensity; // ��������� ������ ��������
	int TracksBrightness; // ������� ������
	int MoonVelocity; // ������� �������� ������ ����
	int MoonBrightness; // ������� ������ ����
	int BrightnessLeft; // ������� ����� ������� �����
	int BrightnessRight; // ������� ������ ������� �����
	float Era; // �����
	double OrbitEccentricity; // �������������� ������ [0,..1)
	double LengthSemiaxis; // ����� ������� ������� [��]
	double LengthRadiusVector; // ����� ������ ������� [��]
	double LongitudeAscendingNode; // ������� ����������� ����
	double InclinationOrbit; // ���������� ������ � ��������
	double ArgumentPerigee; // �������� �������
	double ArgumentLatitude; // �������� ������
	double DeviceAngularVelocity[3]; // ������� �������� �������� �������
	double DeviceAngularAcceleration[3]; // ������� ���������
	double ProjectionLinearVelocity[3]; // �������� �������� ��������
	double OrientationAngles[3]; // ���� ����������
	double OrientationQuaternion[4]; // ���������� ����������
	double OrientationMatrix[3][3]; // ������� ����������
	bool YearAberrationApply; // ���� �������� ���������
	bool SatelliteAberrationApply; // ���� ����������� ���������
	bool PrecessionNutationApply; // ���� ��������� �������
	bool PhotonNoiseApply; // ���� ��������� ����
	bool DistortionApply; // ���� ��������� ���������� �������
};

struct devicetype
{
	string DataSource; // �������� ������
	int SNDevice; // ���������� ����� �������
	int InnerRadius; // ���������� ������ ��������������
	int OuterRadius; // ������� ������ ��������������
	int ResolutionACP; // ����������� ���
	unsigned short Frequency; // ������� �������
	float EV_ACP; // ����� ���������� � ����� ������� ���
	float PixelSize; // ������ �������
	float FocalLength; // �������� ����������
	float DiameterEntrancePupil; // ������� �������� ������
	float DarkLevel; // ������� ��������� ����
	float DarkSKO; // ��� ��������� ����
	float LimitingMagnitude; // ���������� �������� ��������
	float StarsDistribution; // ��� ������������� ������� �����
	double MatrixTemp; // ����������� �������
	funcdist DistortionFunction;
	deadpixels TableDeadObjects;
};

struct filterdata
{
	unsigned short X_coordinate; // ���������� �
	unsigned short Y_coordinate; // ���������� �
	unsigned short BrightnessPixel; // �������

	filterdata() {
		X_coordinate = 0; Y_coordinate = 0; BrightnessPixel = 0;
	}
};

struct filterframe
{
	string TagFilter; // �������� �������
	unsigned short FilteredPixelsCount; // ���������� ������������� ��������
	bool BottomRight; // ���� ���� ����, false -  ��, true - ��
	filterdata* DataPixels; // ���������� � �������
};

struct starinfo
{
	float BrightnessObject; // ������� �������
	float PixelsCount; // ���������� ��������
	float X_coordinate; // ���������� �
	float Y_coordinate; // ���������� �

	unsigned short NumberStar; // ����� ������ � ��������
	float StellarMagnitude; // �������� ��������
	char SpectralClass[2]; // ������������ �����
	float DX; // ������ �� ��� �
	float DY; // ������ �� ��� �

	starinfo() {
		BrightnessObject = 0; PixelsCount = 0; X_coordinate = 0; Y_coordinate = 0; NumberStar = 0; StellarMagnitude = 0; SpectralClass[0] = '_'; SpectralClass[1] = '_'; DX = 0; DY = 0;
	}
};

struct starsinfo
{
	unsigned short RezStat; // ��������� �������������
	float Epsilon; // ������ �� �������������
	float m_X, m_Y, m_Cur; // ������ ������� ������� (��)
	int LocalizedCount; // ���������� �������������� ��������
	int RecognizedCount; // ���������� ������������ ��������
	int SizeLocalList; // ���������� �������� � ������ �����������
	double RecognizedOrientationAngles[3]; // ���� ����������
	double RecognizedOrientationQuaternion[4]; // ���������� ����������
	double RecognizedOrientationMatrix[3][3]; // ������� ����������
	double RecognizedAngularVelocity[3]; // ������� ��������
	starinfo* StarsList; // ���������� � �����/���� �������
	REC SimulatedFrame; // ��������������� ����
};

struct videoframe
{
	float ExposureTime; // ����� ��������������
	fullframe FrameData; // ������ ����
	setlines LinesData; // ������
	setwindow WindowsData; // ����
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

    whenwhere Georeferencing; // �����? ���?
    devicetype CameraSettings; // ��������� ������
    filterframe FilterData; // ������������� �����������
    starsinfo StarsData; // ��������������/������������ �������
    videoframe ImageData; // ��������� ������ �����������

	IKI_img(void);
	~IKI_img();
    void WriteFormat(string filename);
    void Clear_Memory(void);
    bool ReadFormat(string filename, bool Need_Erase);
    unsigned char* ZipStructure(unsigned int &SizeInfo);
    void UnZipStructure(unsigned char* Array, unsigned int SizeInfo);
};
#endif
