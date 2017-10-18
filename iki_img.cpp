#ifndef IKI_IMG_H
#define IKI_IMG_H
#include <fstream>
#include <stdio.h>
#include <Classes.hpp>

using namespace std;

#pragma region DEFINE

#define StartWord  								0x7678D48B
#define StartId                 				0xABCD

#define ROUND(N) (N<0?ceil((N)-0.5):floor((N)+0.5))
#define VERSION_ID                              "1.09.10.17"

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

#define datetime_name							"���� � ����� ������ �����"
#define	soft_vers_name                          "������ �������� ���������"
#define frame_num_name                          "����� �����"
#define protons_density_name                    "��������� ������ ��������"
#define tracks_brightness_name     	        	"������� ������"
#define moon_velocity_name                      "������� �������� ������ ����"
#define moon_brightness_name                    "������� ������ ����"
#define bright_left_name                        "������� ����� ������� �����"
#define bright_right_name         	        	"������� ������ ������� �����"
#define era_name								"�����"
#define orbit_eccentricity_name                 "�������������� ������ [0...1]"
#define length_semiaxis_name       	        	"����� ������� ������� [��]"
#define longitude_node_name                     "������� ����������� ����"
#define inclination_orbit_name                  "���������� ������ � ��������"
#define argument_perigee_name      	        	"�������� �������"
#define dev_angular_velocity_name	        	"������� �������� ������� [���/���]"
#define dev_angular_acceler_name                "������� ��������� ������� [���/���^2]"
#define type_angle_name							"���� ���������� [���]: al dl az"
#define type_quat_name							"���������� ����������"
#define type_matr_name							"������� ����������"
#define proj_line_velo_name						"�������� �������� [��/���]"
#define year_aber_app_name						"���� �������� ���������"
#define satel_aber_app_name						"���� ����������� ���������"
#define preces_nutat_app_name		        	"���� ���������/�������"
#define photon_noise_app_name					"���� ��������� ����"
#define distortion_app_name                     "���� ��������� ���������� �������"
#define length_radius_vector_name               "����� ������ �������"
#define argument_latitude_name                  "�������� ������"

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
#define local_recog_id							0x114D
#define rec_id									0x115D
#define rec_orient_angl_id                      0x116D
#define rec_orient_quat_id						0x117D
#define rec_orient_matr_id						0x118D
#define rec_angular_vel_id                      0x119D
#define	sizelocallist_id                        0x11AD
#define sizestarlist_id                         0x11BD

#define rezstat_name                            "��������� �������������"
#define epsilon_name							"������ �� �������������"
#define m_X_Y_Cyr_name                          "������ ������� ������� (��)"
#define	local_recog_name                        "��������������/������������ �������"
#define rec_name								"������ ���������������� �����"
#define rec_orient_angl_name                    "������������ ���� ���������� [���]: al dl az"
#define rec_orient_quat_name                    "������������ ���������� ����������"
#define rec_orient_matr_name                    "������������ ������� ����������"
#define rec_angular_vel_name                    "������������ ������� ��������"
#define	sizelocallist_name                      "���������� �������������� �������� � ������"
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
#define	stringwindow_count_id                   0x126E
#define	stringwindow_type_id                    0x136E
#define	stringwindow_deg_bin_id                 0x146E
#define	stringwindow_SizeData_id                0x156E
#define	stringwindow_flag_id                    0x166E
#define	stringwindow_info_id                    0x176E
#define	stringwindow_sizewindowlist_id          0x186E

#define	videoframe_name                         "������ ����"
#define	videolines_name                         "������ � �����"
#define	videowindow_name						"���� � �����"

#define	exp_time		                        "����� ���������"

#define	stringframe_type                        "��� ������ ������� �����(0-byte, 1-ushort, 2-float)"
#define	stringframe_deg_bin                     "C������ ����������� ������� �����"
#define	stringframe_width                       "������ ������� �����"
#define	stringframe_height                      "������ ������� �����"
#define	stringframe_SizeData                    "��������� ������ ������� ����� � ��������"

#define	stringlines_count                       "���������� �����"
#define	stringlines_type                        "��� ������ ����� � �����(0-byte, 1-ushort, 2-float)"
#define	stringlines_deg_bin                     "C������ ����������� ����� � �����"
#define	stringlines_SizeData                    "��������� ������ ����� � ��������"
#define	stringlines_width                       "������ ����� �����"
#define	stringlines_height                      "������ ����� �����"
#define	stringlines_info                      	"��������� �����"
#define	stringwindow_sizewindowlist             "���������� ���� � ������"

#define	stringwindow_count                      "���������� ����"
#define	stringwindow_type                       "��� ������ ����(0-byte, 1-ushort, 2-float)"
#define	stringwindow_deg_bin                    "C������ ����������� ����� � �����"
#define	stringwindow_SizeData                   "��������� ������ ���� � ��������"
#define	stringwindow_flag                       "���� ������� ���������: false-��, true-��"
#define	stringwindow_info                       "��������� ����"

#pragma end_region

#pragma end_region

#pragma region ���������

struct STARREC
{
	WORD Ns; // ����� ������
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
	WORD strrec; // ����� ����� �� �����
	WORD SizeStarList; // ���������� ����� � ������
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
	WORD PolynomialDegree; // ������� ��������
	WORD CoefficientsCount; // ���������� �������������
	coefonaxis* Coefficient;
};

struct infopix // ���������� � ����� �������
{
	float X_coordinate; // ���������� �
	float Y_coordinate; // ���������� �
	float BrightnessObj; // ������� �������
	WORD CoefficientObj; // ������ �������

	infopix() {
		X_coordinate = 0; Y_coordinate = 0; BrightnessObj = 0; CoefficientObj = 0;
	}
};

struct deadpixels // ��������� ����� ��������
{
	WORD RecordsCount; // ���������� �������
	infopix* PixelData; // ������ ������� ���������� � ��������
};

struct fullframe
{
	unsigned int DataType; // ������� ���� 0-byte, 1-ushort, 2-float
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
	unsigned int DataType; // ������� ���� 0-byte, 1-ushort, 2-float
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
	unsigned int DataType; // ������� ���� 0-byte, 1-ushort, 2-float
	unsigned int DegreeBinning; // ������� �����������
	unsigned int SizeData; // ������ ���������� (S+=W[i]*H[i])
	bool BottomRight; // ���� ���� ����, false -  ��, true - ��
	datawindow* Info; // ���������� � �����
	void* Data; // �������� ������
};

struct whenwhere
{
	TDateTime DateTime; // ���� � ����� �����
	AnsiString SoftwareVersion; // ������ ���������
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
	AnsiString DataSource; // �������� ������
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
	AnsiString TagFilter; // �������� �������
	WORD FilteredPixelsCount; // ���������� ������������� ��������
	bool BottomRight; // ���� ���� ����, false -  ��, true - ��
	filterdata* DataPixels; // ���������� � �������
};

struct starinfo
{
	float BrightnessObject; // ������� �������
	float PixelsCount; // ���������� ��������
	float X; // ���������� �
	float Y; // ���������� �

	WORD NumberStar; // ����� ������ � ��������
	float Mv; // �������� ��������
	char Sp[2]; // ������������ �����
	float DX; // ������ �� ��� �
	float DY; // ������ �� ��� �

	starinfo() {
		BrightnessObject = 0; PixelsCount = 0; X = 0; Y = 0; NumberStar = 0; Mv = 0; Sp[0] = '_'; Sp[1] = '_'; DX = 0; DY = 0;
	}
};

struct starsinfo
{
	WORD RezStat; // ��������� �������������
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
	template<typename T>
	static void _FreeMemory(T*&point)
	{
		// if (point != NULL)
		// {
			delete point;
			point = NULL;
		// }
	}
	// ---------------------------------------------------------------------------

	template<typename T>
	static void _FreeMemoryArray(T*&array)
	{
		// if (array != NULL)
		// {
			delete[]array;
			array = NULL;
		// }
	}
	// ---------------------------------------------------------------------------

	AnsiString ArcsecToStr(double arcsec)
	{
		int deg, min, sec, msc, rem, arc;
		double arf;
		AnsiString str;
		arf = 1000.0 * arcsec; // in msec
		arc = ROUND(arf);
		arc = abs(arc);
		deg = arc / 3600000; rem = arc % 3600000;
		min = rem / 60000; sec = (rem - min * 60000) / 1000;
		msc = arc - deg * 3600000 - min * 60000 - sec * 1000;
		if (arf >= 0)
			str.sprintf("%d\�%02d'%02d\".%03d", deg, min, sec, msc);
		else
			str.sprintf("-%d\�%02d'%02d\".%03d", deg, min, sec, msc);
		return str;
	}
	// ---------------------------------------------------------------------------

	double StrToArcsec(AnsiString _str)
	{
		AnsiString _strbuf;
		double arcsec;
		bool negative = false;
		int deg, min, sec, msc, rem, arc;
		int pos_index;
		pos_index = _str.AnsiPos("{");
		if (pos_index != 0)
			_str = _str.SubString(pos_index + 1, _str.Length());
		pos_index = _str.AnsiPos("}");
		if (pos_index != 0)
			_str = _str.SubString(0, pos_index - 1);
		// �������� �� ���������������, ���� ���� "-", ��� �������,
		// � ����� ���������� �������� �������� �� -1
		pos_index = _str.AnsiPos("-");
		if (pos_index != 0)
		{
			_str = _str.SubString(pos_index + 1, _str.Length());
			negative = true;
		}
		pos_index = _str.AnsiPos("\�"); _strbuf = _str.SubString(0, pos_index - 1);
		deg = StrToInt(_strbuf); _str = _str.SubString(pos_index + 1, _str.Length() - pos_index);
		pos_index = _str.AnsiPos("'"); _strbuf = _str.SubString(0, pos_index - 1);
		min = StrToInt(_strbuf); _str = _str.SubString(pos_index + 1, _str.Length() - pos_index);
		pos_index = _str.AnsiPos("\""); _strbuf = _str.SubString(0, pos_index - 1);
		sec = StrToInt(_strbuf); _str = _str.SubString(pos_index + 1, _str.Length() - pos_index);
		pos_index = _str.AnsiPos("."); _str = _str.SubString(pos_index + 1, _str.Length());
		msc = StrToInt(_str);
		arcsec = (double)(deg * 3600000 + min * 60000 + sec * 1000 + msc) / (double)1000.0;
		if (negative)
			arcsec *= -1;
		return arcsec;
	}
	// ---------------------------------------------------------------------------

	void print_AnsiString(TFileStream *FStream, AnsiString _string)
	{
		unsigned int sizedata = _string.Length();
		char* bufer;
		AnsiString _endstr = "\r\n";
		bufer = new char[sizedata];
		Move(_string.c_str(), &bufer[0], sizedata);
		FStream->Write(&sizedata, sizeof(unsigned int));
		FStream->Write(&bufer[0], sizedata);
		FStream->Write(_endstr.c_str(), _endstr.Length());
		_FreeMemoryArray(bufer);
	}
	// ---------------------------------------------------------------------------

	void PrintString(TFileStream *FStream, unsigned int start, AnsiString bufer_name, AnsiString DataString)
	{
		AnsiString _endstr = "\r\n";
		FStream->Write(&start, sizeof(unsigned int));
		char* bufer = new char[80];
		memset(&bufer[0], 0, 80);
		Move(bufer_name.c_str(), &bufer[0], bufer_name.Length());
		FStream->Write(&bufer[0], 80);
		_FreeMemoryArray(bufer);
		int sizedata = DataString.Length();
		bufer = new char[sizedata];
		Move(DataString.c_str(), &bufer[0], sizedata);
		FStream->Write(&sizedata, sizeof(sizedata));
		FStream->Write(&bufer[0], sizedata);
		FStream->Write(_endstr.c_str(), _endstr.Length());
		_FreeMemoryArray(bufer);
	}
	// ---------------------------------------------------------------------------

	const AnsiString BoolToString(bool b)
	{
		return b ? "true" : "false";
	}
	// ---------------------------------------------------------------------------

	const bool StringToBool(AnsiString b)
	{
		return (b == "true");
	}
	// ---------------------------------------------------------------------------

	void StrArcsecToMass(double(*mass), unsigned int N, AnsiString _str)
	{
		int pos_index;
		AnsiString _strbuf;
		for (unsigned int i = 0; i < N; i++)
		{
			pos_index = _str.AnsiPos(" ");
			if (pos_index != 0)
			{
				_strbuf = _str.SubString(0, pos_index - 1);
				mass[i] = StrToArcsec(_strbuf);
				_str = _str.SubString(pos_index + 1, _str.Length() - pos_index);
			}
			else
				mass[i] = StrToArcsec(_str);
		}
	}
	// ---------------------------------------------------------------------------

	void StringToFloatMass(double(*mass), unsigned int N, AnsiString _str)
	{
		int pos_index = 0;
		int pos_C = 0;
		AnsiString _strbuf;
		for (unsigned int i = 0; i < N; i++)
		{
			pos_index = _str.AnsiPos(" ");
			if (pos_index != 0)
			{
				_strbuf = _str.SubString(0, pos_index - 1);
				pos_C = _strbuf.AnsiPos("{");
				if (pos_C != 0)
					_strbuf = _strbuf.SubString(pos_C + 1, _strbuf.Length() - pos_C); // - pos_C !!!!!!!!!!!!!!!!!!!!!!
				pos_C = _strbuf.AnsiPos("}");
				if (pos_C != 0)
					_strbuf = _strbuf.SubString(0, pos_C - 1);
				mass[i] = StrToFloat(_strbuf);
				_str = _str.SubString(pos_index + 1, _str.Length() - pos_index);
			}
			else
			{
				pos_C = _str.AnsiPos("}");
				if (pos_C != 0)
					_str = _str.SubString(0, pos_C - 1);
				mass[i] = StrToFloat(_str);
				break;
			}
		}
	}
	// ---------------------------------------------------------------------------

	void StringToFloatMass3D(double(*mas3d)[3], unsigned int N, unsigned int M, AnsiString _str)
	{
		int pos_index = 0;
		int pos_C = 0;
		AnsiString _strbuf;
		for (unsigned int i = 0; i < N; i++)
			for (unsigned int j = 0; j < M; j++)
			{
				pos_index = _str.AnsiPos(" ");
				if (pos_index != 0)
				{
					_strbuf = _str.SubString(0, pos_index - 1);
					pos_C = _strbuf.AnsiPos("{");
					if (pos_C != 0)
						_strbuf = _strbuf.SubString(pos_C + 1, _strbuf.Length());
					pos_C = _strbuf.AnsiPos("}");
					if (pos_C != 0)
						_strbuf = _strbuf.SubString(0, pos_C - 1);
					mas3d[i][j] = StrToFloat(_strbuf);
					_str = _str.SubString(pos_index + 1, _str.Length() - pos_index);
				}
				else
				{
					pos_C = _str.AnsiPos("}");
					if (pos_C != 0)
						_str = _str.SubString(0, pos_C - 1);
					mas3d[i][j] = StrToFloat(_str);
					break;
				}
			}
	}
	// ---------------------------------------------------------------------------

public:

#pragma region ����������

	whenwhere Georeferencing; // �����? ���?
	devicetype CameraSettings; // ��������� ������
	filterframe FilterData; // ������������� �����������
	starsinfo StarsData; // ��������������/������������ �������
	videoframe ImageData; // ��������� ������ �����������
#pragma end_region

#pragma region �������

	__fastcall IKI_img::IKI_img(void) /* : Georeferencing(), CameraSettings(), FilterData(),StarsData(),ImageData() */
	{
		memset(&Georeferencing, 0, sizeof(Georeferencing));
		memset(&CameraSettings, 0, sizeof(CameraSettings));
		memset(&FilterData, 0, sizeof(FilterData));
		memset(&StarsData, 0, sizeof(StarsData));
		memset(&ImageData, 0, sizeof(ImageData));
	}
	// ---------------------------------------------------------------------------

	__fastcall IKI_img::~IKI_img()
	{
		_FreeMemoryArray(CameraSettings.DistortionFunction.Coefficient);
		_FreeMemoryArray(CameraSettings.TableDeadObjects.PixelData);
		_FreeMemoryArray(FilterData.DataPixels);
		_FreeMemoryArray(StarsData.StarsList);
		_FreeMemoryArray(StarsData.SimulatedFrame.StarRec);
		_FreeMemoryArray(ImageData.FrameData.Data);
		_FreeMemoryArray(ImageData.LinesData.Info);
		_FreeMemoryArray(ImageData.LinesData.Data);
		_FreeMemoryArray(ImageData.WindowsData.Info);
		_FreeMemoryArray(ImageData.WindowsData.Data);
		memset(&Georeferencing, 0, sizeof(Georeferencing));
		memset(&CameraSettings, 0, sizeof(CameraSettings));
		memset(&FilterData, 0, sizeof(FilterData));
		memset(&StarsData, 0, sizeof(StarsData));
		memset(&ImageData, 0, sizeof(ImageData));
	}
	// ---------------------------------------------------------------------------

	void WriteFormat(AnsiString filename)
	{
		AnsiString Value_String;
		TFileStream *FStream = new TFileStream(filename, fmCreate);
#pragma region Georeferencing
#pragma region datetime
		Value_String = FormatDateTime("dd.mm.yyyy hh:nn:ss.zzz", Georeferencing.DateTime);
		PrintString(FStream, datetime_id, datetime_name, Value_String);
#pragma end_region
#pragma region soft_vers
		Georeferencing.SoftwareVersion = VERSION_ID;
		PrintString(FStream, soft_vers_id, soft_vers_name, Georeferencing.SoftwareVersion);
#pragma end_region
#pragma region frame_num
		Value_String = IntToStr(Georeferencing.FrameNumber);
		PrintString(FStream, frame_num_id, frame_num_name, Value_String);
#pragma end_region
#pragma region protons_density
		Value_String = IntToStr(Georeferencing.ProtonsDensity);
		PrintString(FStream, protons_density_id, protons_density_name, Value_String);
#pragma end_region
#pragma region tracks_brightness
		Value_String = IntToStr(Georeferencing.TracksBrightness);
		PrintString(FStream, tracks_brightness_id, tracks_brightness_name, Value_String);
#pragma end_region
#pragma region moon_velocity
		if (Georeferencing.MoonVelocity != 0)
		{
			Value_String = IntToStr(Georeferencing.MoonVelocity);
			PrintString(FStream, moon_velocity_id, moon_velocity_name, Value_String);
		}
#pragma end_region
#pragma region moon_brightness
		if (Georeferencing.MoonBrightness != 0)
		{
			Value_String = IntToStr(Georeferencing.MoonBrightness);
			PrintString(FStream, moon_brightness_id, moon_brightness_name, Value_String);
		}
#pragma end_region
#pragma region bright_left
		Value_String = IntToStr(Georeferencing.BrightnessLeft);
		PrintString(FStream, bright_left_id, bright_left_name, Value_String);
#pragma end_region
#pragma region bright_right
		Value_String = IntToStr(Georeferencing.BrightnessRight);
		PrintString(FStream, bright_right_id, bright_right_name, Value_String);
#pragma end_region
#pragma region era
		Value_String = FloatToStrF(Georeferencing.Era, ffFixed, 8, 6);
		PrintString(FStream, era_id, era_name, Value_String);
#pragma end_region
#pragma region orbit_eccentricity
		Value_String = FloatToStrF(Georeferencing.OrbitEccentricity, ffFixed, 8, 3);
		PrintString(FStream, orbit_eccentricity_id, orbit_eccentricity_name, Value_String);
#pragma end_region
#pragma region length_semiaxis
		Value_String = FloatToStrF(Georeferencing.LengthSemiaxis, ffFixed, 8, 3);
		PrintString(FStream, length_semiaxis_id, length_semiaxis_name, Value_String);
#pragma end_region
#pragma region length_radius_vector
		Value_String = FloatToStrF(Georeferencing.LengthRadiusVector, ffFixed, 8, 3);
		PrintString(FStream, length_radius_vector_id, length_radius_vector_name, Value_String);
#pragma end_region
#pragma region longitude_node
		Value_String = ArcsecToStr(Georeferencing.LongitudeAscendingNode);
		PrintString(FStream, longitude_node_id, longitude_node_name, Value_String);
#pragma end_region
#pragma region inclination_orbit
		Value_String = ArcsecToStr(Georeferencing.InclinationOrbit);
		PrintString(FStream, inclination_orbit_id, inclination_orbit_name, Value_String);
#pragma end_region
#pragma region argument_perigee
		Value_String = ArcsecToStr(Georeferencing.ArgumentPerigee);
		PrintString(FStream, argument_perigee_id, argument_perigee_name, Value_String);
#pragma end_region
#pragma region argument_latitude
		Value_String = ArcsecToStr(Georeferencing.ArgumentLatitude);
		PrintString(FStream, argument_latitude_id, argument_latitude_name, Value_String);
#pragma end_region
#pragma region dev_angular_velocity
		Value_String = "";
		Value_String += FormatFloat("0.0######### ", Georeferencing.DeviceAngularVelocity[0]);
		Value_String += FormatFloat("0.0######### ", Georeferencing.DeviceAngularVelocity[1]);
		Value_String += FormatFloat("0.0#########", Georeferencing.DeviceAngularVelocity[2]);
		PrintString(FStream, dev_angular_velocity_id, dev_angular_velocity_name, Value_String);
#pragma end_region
#pragma region dev_angular_acceler
		Value_String = "";
		Value_String += FormatFloat("0.0######### ", Georeferencing.DeviceAngularAcceleration[0]);
		Value_String += FormatFloat("0.0######### ", Georeferencing.DeviceAngularAcceleration[1]);
		Value_String += FormatFloat("0.0#########", Georeferencing.DeviceAngularAcceleration[2]);
		PrintString(FStream, dev_angular_acceler_id, dev_angular_acceler_name, Value_String);
#pragma end_region
#pragma region proj_line_velo
		Value_String = "";
		Value_String += FormatFloat("0.0######### ", Georeferencing.ProjectionLinearVelocity[0]);
		Value_String += FormatFloat("0.0######### ", Georeferencing.ProjectionLinearVelocity[1]);
		Value_String += FormatFloat("0.0#########", Georeferencing.ProjectionLinearVelocity[2]);
		PrintString(FStream, proj_line_velo_id, proj_line_velo_name, Value_String);
#pragma end_region
#pragma region type_angle
		Value_String = "";
		Value_String += FormatFloat("0.0######### ", Georeferencing.OrientationAngles[0]);
		Value_String += FormatFloat("0.0######### ", Georeferencing.OrientationAngles[1]);
		Value_String += FormatFloat("0.0#########", Georeferencing.OrientationAngles[2]);
		PrintString(FStream, type_angle_id, type_angle_name, Value_String);
#pragma end_region
#pragma region type_quat
		if (Georeferencing.OrientationQuaternion[0] == 0 &&
			Georeferencing.OrientationQuaternion[1] == 0 &&
			Georeferencing.OrientationQuaternion[2] == 0 &&
			Georeferencing.OrientationQuaternion[3] == 0)
		{
		}
		else
		{
			Value_String = "";
			Value_String += FormatFloat("0.0######### ", Georeferencing.OrientationQuaternion[0]);
			Value_String += FormatFloat("0.0######### ", Georeferencing.OrientationQuaternion[1]);
			Value_String += FormatFloat("0.0######### ", Georeferencing.OrientationQuaternion[2]);
			Value_String += FormatFloat("0.0#########", Georeferencing.OrientationQuaternion[3]);
			PrintString(FStream, type_quat_id, type_quat_name, Value_String);
		}
#pragma end_region
#pragma region type_matr
		Value_String = "";
		for (int i = 0; i < 3; i++)
		{
			Value_String += "{";
			Value_String += FormatFloat("0.0######### ", Georeferencing.OrientationMatrix[i][0]);
			Value_String += FormatFloat("0.0######### ", Georeferencing.OrientationMatrix[i][1]);
			Value_String += FormatFloat("0.0#########} ", Georeferencing.OrientationMatrix[i][2]);
		}
		PrintString(FStream, type_matr_id, type_matr_name, Value_String);
#pragma end_region
#pragma region year_aber_app
		Value_String = BoolToString(Georeferencing.YearAberrationApply);
		PrintString(FStream, year_aber_app_id, year_aber_app_name, Value_String);
#pragma end_region
#pragma region satel_aber_app
		Value_String = BoolToString(Georeferencing.SatelliteAberrationApply);
		PrintString(FStream, satel_aber_app_id, satel_aber_app_name, Value_String);
#pragma end_region
#pragma region preces_nutat_app
		Value_String = BoolToString(Georeferencing.PrecessionNutationApply);
		PrintString(FStream, preces_nutat_app_id, preces_nutat_app_name, Value_String);
#pragma end_region
#pragma region photon_noise_app
		Value_String = BoolToString(Georeferencing.PhotonNoiseApply);
		PrintString(FStream, photon_noise_app_id, photon_noise_app_name, Value_String);
#pragma end_region
#pragma region distortion_app
		Value_String = BoolToString(Georeferencing.DistortionApply);
		PrintString(FStream, distortion_app_id, distortion_app_name, Value_String);
#pragma end_region

#pragma end_region
#pragma region CameraSettings
#pragma region device_type
		if (CameraSettings.DataSource != "")
			PrintString(FStream, device_type_id, device_type_name, CameraSettings.DataSource);
#pragma end_region
#pragma region device_sn
		if (CameraSettings.SNDevice != 0)
		{
			Value_String = FormatFloat("#####0", CameraSettings.SNDevice);
			PrintString(FStream, device_sn_id, device_sn_name, Value_String);
		}
#pragma end_region
#pragma region inner_radius
		Value_String = IntToStr(CameraSettings.InnerRadius);
		PrintString(FStream, inner_radius_id, inner_radius_name, Value_String);
#pragma end_region
#pragma region outer_radius
		Value_String = IntToStr(CameraSettings.OuterRadius);
		PrintString(FStream, outer_radius_id, outer_radius_name, Value_String);
#pragma end_region
#pragma region resolution_acp
		if (CameraSettings.ResolutionACP)
		{
			Value_String = FormatFloat("#####0", CameraSettings.ResolutionACP);
			PrintString(FStream, resolution_acp_id, resolution_acp_name, Value_String);
		}
#pragma end_region
#pragma region ev_acp
		if (CameraSettings.EV_ACP != 0)
		{
			Value_String = FloatToStrF(CameraSettings.EV_ACP, ffFixed, 8, 3);
			PrintString(FStream, ev_acp_id, ev_acp_name, Value_String);
		}
#pragma end_region
#pragma region pixel_size
		if (CameraSettings.PixelSize != 0)
		{
			Value_String = FloatToStrF(CameraSettings.PixelSize, ffFixed, 8, 5);
			PrintString(FStream, pixel_size_id, pixel_size_name, Value_String);
		}
#pragma end_region
#pragma region focal_length
		if (CameraSettings.FocalLength != 0) {
			Value_String = FloatToStrF(CameraSettings.FocalLength, ffFixed, 8, 3);
			PrintString(FStream, focal_length_id, focal_length_name, Value_String);
		}
#pragma end_region
#pragma region diametr_entrace_pupil
		if (CameraSettings.DiameterEntrancePupil != 0)
		{
			Value_String = FloatToStrF(CameraSettings.DiameterEntrancePupil, ffFixed, 8, 3);
			PrintString(FStream, diametr_entrace_pupil_id, diametr_entrace_pupil_name, Value_String);
		}
#pragma end_region
#pragma region dark_level
		Value_String = FloatToStrF(CameraSettings.DarkLevel, ffFixed, 8, 3);
		PrintString(FStream, dark_level_id, dark_level_name, Value_String);
#pragma end_region
#pragma region dark_sko
		Value_String = FloatToStrF(CameraSettings.DarkSKO, ffFixed, 8, 3);
		PrintString(FStream, dark_sko_id, dark_sko_name, Value_String);
#pragma end_region
#pragma region limit_magn
		if (CameraSettings.LimitingMagnitude != 0)
		{
			Value_String = FormatFloat("0.0#####", CameraSettings.LimitingMagnitude);
			PrintString(FStream, limit_magn_id, limit_magn_name, Value_String);
		}
#pragma end_region
#pragma region stars_dist
		if (CameraSettings.StarsDistribution != 0)
		{
			Value_String = FloatToStrF(CameraSettings.StarsDistribution, ffFixed, 8, 3);
			PrintString(FStream, stars_dist_id, stars_dist_name, Value_String);
		}
#pragma end_region
#pragma region function_distortion
		if (Georeferencing.DistortionApply)
		{
			Value_String = "";
			Value_String += FormatFloat("#####0 ", CameraSettings.DistortionFunction.PolynomialDegree);
			Value_String += FormatFloat("#####0", CameraSettings.DistortionFunction.CoefficientsCount);
			PrintString(FStream, function_distortion_id, function_distortion_name, Value_String);
			for (int i = 0; i < CameraSettings.DistortionFunction.CoefficientsCount; i++)
			{
				Value_String = "";
				Value_String += FormatFloat("#0.0########### ", CameraSettings.DistortionFunction.Coefficient[i].X);
				Value_String += FormatFloat("#0.0###########", CameraSettings.DistortionFunction.Coefficient[i].Y);
				print_AnsiString(FStream, Value_String);
			}
		}
#pragma end_region
#pragma region dead_pix
		if (CameraSettings.TableDeadObjects.RecordsCount != 0)
		{
			Value_String = FormatFloat("#####0", CameraSettings.TableDeadObjects.RecordsCount);
			PrintString(FStream, dead_pix_id, dead_pix_name, Value_String);
			for (int i = 0; i < CameraSettings.TableDeadObjects.RecordsCount; i++)
			{
				Value_String = "";
				Value_String += FormatFloat("####0.0##### ", CameraSettings.TableDeadObjects.PixelData[i].X_coordinate);
				Value_String += FormatFloat("####0.0##### ", CameraSettings.TableDeadObjects.PixelData[i].Y_coordinate);
				Value_String += FormatFloat("####0.0##### ", CameraSettings.TableDeadObjects.PixelData[i].BrightnessObj);
				Value_String += FormatFloat("#####0", CameraSettings.TableDeadObjects.PixelData[i].CoefficientObj);
				print_AnsiString(FStream, Value_String);
			}
		}
#pragma end_region

#pragma region matrixtemp
		Value_String = FloatToStrF(CameraSettings.MatrixTemp, ffFixed, 10, 7);
		PrintString(FStream, matrixtemp_id, matrixtemp_name, Value_String);
#pragma end_region

#pragma region frequency
		if (CameraSettings.Frequency != 0)
		{
			Value_String = FormatFloat("#####0", CameraSettings.Frequency);
			PrintString(FStream, frequency_id, frequency_name, Value_String);
		}
#pragma end_region

#pragma end_region
#pragma region FilterData
#pragma region bottomright
		if (FilterData.FilteredPixelsCount != 0)
		{
			Value_String = BoolToString(FilterData.BottomRight);
			PrintString(FStream, bottomright_id, bottomright_name, Value_String);
		}
#pragma end_region
		if (FilterData.FilteredPixelsCount != 0)
		{
			Value_String = FormatFloat("#####0 ", FilterData.FilteredPixelsCount) + FilterData.TagFilter;
			PrintString(FStream, filter_pix_id, filter_pix_name, Value_String);
			for (int i = 0; i < FilterData.FilteredPixelsCount; i++)
			{
				Value_String = "";
				Value_String += FormatFloat("#####0 ", FilterData.DataPixels[i].X_coordinate);
				Value_String += FormatFloat("#####0 ", FilterData.DataPixels[i].Y_coordinate);
				Value_String += FormatFloat("#####0", FilterData.DataPixels[i].BrightnessPixel);
				print_AnsiString(FStream, Value_String);
			}
		}
#pragma end_region
#pragma region starsinfo
#pragma region rezstat
		Value_String = IntToHex(StarsData.RezStat, 4);
		PrintString(FStream, rezstat_id, rezstat_name, Value_String);
#pragma end_region
#pragma region epsilon
		Value_String = FormatFloat("0.0#####", StarsData.Epsilon);
		PrintString(FStream, epsilon_id, epsilon_name, Value_String);
#pragma end_region
#pragma region m_X_Y_Cyr
		if (StarsData.m_X == 0 && StarsData.m_Y == 0 && StarsData.m_Cur == 0) {
		}
		else
		{
			Value_String = "";
			Value_String += FormatFloat("0.0##### ", StarsData.m_X);
			Value_String += FormatFloat("0.0##### ", StarsData.m_Y);
			Value_String += FormatFloat("0.0#####", StarsData.m_Cur);
			PrintString(FStream, m_X_Y_Cyr_id, m_X_Y_Cyr_name, Value_String);
		}
#pragma end_region
#pragma region sizelocallist
		Value_String = FormatFloat("#####0", StarsData.SizeLocalList);
		PrintString(FStream, sizelocallist_id, sizelocallist_name, Value_String);
#pragma end_region
#pragma region local_recog
		if (StarsData.LocalizedCount != 0)
		{
			Value_String = "";
			Value_String += FormatFloat("#####0 ", StarsData.LocalizedCount);
			Value_String += FormatFloat("#####0", StarsData.RecognizedCount);
			PrintString(FStream, local_recog_id, local_recog_name, Value_String);
			for (int i = 0; i < StarsData.SizeLocalList; i++)
			{
				Value_String = "";
				Value_String += FormatFloat("#####0.0#### ", StarsData.StarsList[i].BrightnessObject);
				Value_String += FormatFloat("#####0.0#### ", StarsData.StarsList[i].PixelsCount);
				Value_String += FormatFloat("#####0.0#### ", StarsData.StarsList[i].X);
				Value_String += FormatFloat("#####0.0#### ", StarsData.StarsList[i].Y);
				if (StarsData.StarsList[i].NumberStar == 0)
					goto endlist;
				else
					Value_String += FormatFloat("#####0 ", StarsData.StarsList[i].NumberStar);
				Value_String += FormatFloat("0.0 ", StarsData.StarsList[i].Mv);
				Value_String += (AnsiString)StarsData.StarsList[i].Sp[0] + (AnsiString)StarsData.StarsList[i].Sp[1] + " ";
				Value_String += FormatFloat("#####0.0#### ", StarsData.StarsList[i].DX);
				Value_String += FormatFloat("#####0.0####", StarsData.StarsList[i].DY);
			endlist: print_AnsiString(FStream, Value_String);
			}
		}
#pragma end_region
#pragma region sizstarlist
		Value_String = FormatFloat("#####0", StarsData.SimulatedFrame.SizeStarList);
		PrintString(FStream, sizestarlist_id, sizestarlist_name, Value_String);
#pragma end_region
#pragma region rec
		if (StarsData.SimulatedFrame.SizeStarList != 0)
		{
			Value_String = FormatFloat("#####0", StarsData.SimulatedFrame.strrec);
			PrintString(FStream, rec_id, rec_name, Value_String);
			for (int i = 0; i < StarsData.SimulatedFrame.SizeStarList; i++)
			{
				Value_String = "";
				Value_String += FormatFloat("###0 ", StarsData.SimulatedFrame.StarRec[i].Ns);
				Value_String += FormatFloat("#####0 ", StarsData.SimulatedFrame.StarRec[i].Is);
				Value_String += FormatFloat("#####0.0#### ", StarsData.SimulatedFrame.StarRec[i].Xs);
				Value_String += FormatFloat("#####0.0#### ", StarsData.SimulatedFrame.StarRec[i].Ys);
				Value_String += FormatFloat("0.0 ", StarsData.SimulatedFrame.StarRec[i].Mv);
				Value_String += (AnsiString)StarsData.SimulatedFrame.StarRec[i].Sp[0] + (AnsiString)StarsData.SimulatedFrame.StarRec[i].Sp[1];
				print_AnsiString(FStream, Value_String);
			}
		}
#pragma end_region
#pragma region rec_orient_angl
		if (StarsData.RecognizedOrientationAngles[0] == 0 &&
			StarsData.RecognizedOrientationAngles[1] == 0 &&
			StarsData.RecognizedOrientationAngles[2] == 0)
		{
		}
else
		{
			Value_String = "";
			Value_String += FormatFloat("0.0######### ", StarsData.RecognizedOrientationAngles[0]);
			Value_String += FormatFloat("0.0######### ", StarsData.RecognizedOrientationAngles[1]);
			Value_String += FormatFloat("0.0#########", StarsData.RecognizedOrientationAngles[2]);
			PrintString(FStream, rec_orient_angl_id, rec_orient_angl_name, Value_String);
		}
#pragma end_region
#pragma region rec_orient_quat
		if (StarsData.RecognizedOrientationQuaternion[0] == 0 &&
			StarsData.RecognizedOrientationQuaternion[1] == 0 &&
			StarsData.RecognizedOrientationQuaternion[2] == 0 &&
			StarsData.RecognizedOrientationQuaternion[3] == 0)
		{
		}
		else
		{
			Value_String = "";
			for (int i = 0; i < 4; i++)
				Value_String += FormatFloat("0.0######### ", StarsData.RecognizedOrientationQuaternion[i]);
			PrintString(FStream, rec_orient_quat_id, rec_orient_quat_name, Value_String);
		}
#pragma end_region
#pragma region rec_orient_matr
		if (StarsData.RecognizedOrientationMatrix[0][0] == 0 &&
			StarsData.RecognizedOrientationMatrix[0][1] == 0 &&
			StarsData.RecognizedOrientationMatrix[0][2] == 0 &&
			StarsData.RecognizedOrientationMatrix[1][0] == 0 &&
			StarsData.RecognizedOrientationMatrix[1][1] == 0 &&
			StarsData.RecognizedOrientationMatrix[1][2] == 0 &&
			StarsData.RecognizedOrientationMatrix[2][0] == 0 &&
			StarsData.RecognizedOrientationMatrix[2][1] == 0 &&
			StarsData.RecognizedOrientationMatrix[2][2] == 0)
		{
		}
		else
		{
			Value_String = "";
			for (int i = 0; i < 3; i++)
			{
				Value_String += "{";
				Value_String += FormatFloat("0.0######### ", StarsData.RecognizedOrientationMatrix[i][0]);
				Value_String += FormatFloat("0.0######### ", StarsData.RecognizedOrientationMatrix[i][1]);
				Value_String += FormatFloat("0.0#########} ", StarsData.RecognizedOrientationMatrix[i][2]);
			}
			PrintString(FStream, rec_orient_matr_id, rec_orient_matr_name, Value_String);
		}
#pragma end_region
#pragma region rec_angular_vel
		if (StarsData.RecognizedAngularVelocity[0] == 0 &&
			StarsData.RecognizedAngularVelocity[1] == 0 &&
			StarsData.RecognizedAngularVelocity[2] == 0)
		{
		}
else
		{
			Value_String = "";
			Value_String += FormatFloat("0.0######### ", StarsData.RecognizedAngularVelocity[0]);
			Value_String += FormatFloat("0.0######### ", StarsData.RecognizedAngularVelocity[1]);
			Value_String += FormatFloat("0.0#########", StarsData.RecognizedAngularVelocity[2]);
			PrintString(FStream, rec_angular_vel_id, rec_angular_vel_name, Value_String);
		}
#pragma end_region
#pragma end_region
#pragma region ImageData
		Value_String = FormatFloat("0.0#####", ImageData.ExposureTime);
		PrintString(FStream, exp_time_id, exp_time, Value_String);
#pragma region stringframe
		if (ImageData.FrameData.FrameWidth != 0 && ImageData.FrameData.FrameHeight != 0)
		{
			Value_String = FormatFloat("#####0", ImageData.FrameData.DataType);
			PrintString(FStream, stringframe_type_id, stringframe_type, Value_String);
			Value_String = FormatFloat("#####0", ImageData.FrameData.DegreeBinning);
			PrintString(FStream, stringframe_deg_bin_id, stringframe_deg_bin, Value_String);
			Value_String = FormatFloat("#####0", ImageData.FrameData.FrameWidth);
			PrintString(FStream, stringframe_width_id, stringframe_width, Value_String);
			Value_String = FormatFloat("#####0", ImageData.FrameData.FrameHeight);
			PrintString(FStream, stringframe_height_id, stringframe_height, Value_String);
			Value_String = FormatFloat("#####0", ImageData.FrameData.SizeData);
			PrintString(FStream, stringframe_SizeData_id, stringframe_SizeData, Value_String);
		}
#pragma end_region
#pragma region stringlines
		if (ImageData.LinesData.LinesCount != 0)
		{
			Value_String = FormatFloat("#####0", ImageData.LinesData.LinesCount);
			PrintString(FStream, stringlines_count_id, stringlines_count, Value_String);
			Value_String = FormatFloat("#####0", ImageData.LinesData.DataType);
			PrintString(FStream, stringlines_type_id, stringlines_type, Value_String);
			Value_String = FormatFloat("#####0", ImageData.LinesData.DegreeBinning);
			PrintString(FStream, stringlines_deg_bin_id, stringlines_deg_bin, Value_String);
			Value_String = FormatFloat("#####0", ImageData.LinesData.LinesWidth);
			PrintString(FStream, stringlines_width_id, stringlines_width, Value_String);
			Value_String = FormatFloat("#####0", ImageData.LinesData.LinesHeight);
			PrintString(FStream, stringlines_height_id, stringlines_height, Value_String);
			Value_String = FormatFloat("#####0", ImageData.LinesData.SizeData);
			PrintString(FStream, stringlines_SizeData_id, stringlines_SizeData, Value_String);
			Value_String = "";
			PrintString(FStream, stringlines_info_id, stringlines_info, Value_String);
			for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
			{
				Value_String = "";
				Value_String += FormatFloat("#####0 ", ImageData.LinesData.Info[i].Y_FirstString);
				Value_String += FormatFloat("#####0", ImageData.LinesData.Info[i].CountString);
				print_AnsiString(FStream, Value_String);
			}
		}
#pragma end_region

#pragma region stringwindow
		if (ImageData.WindowsData.SizeWindowList != 0)
		{
			Value_String = FormatFloat("#####0", ImageData.WindowsData.SizeWindowList);
			PrintString(FStream, stringwindow_sizewindowlist_id, stringwindow_sizewindowlist, Value_String);
			Value_String = FormatFloat("#####0", ImageData.WindowsData.WindowCount);
			PrintString(FStream, stringwindow_count_id, stringwindow_count, Value_String);
			Value_String = FormatFloat("#####0", ImageData.WindowsData.DataType);
			PrintString(FStream, stringwindow_type_id, stringwindow_type, Value_String);
			Value_String = FormatFloat("#####0", ImageData.WindowsData.DegreeBinning);
			PrintString(FStream, stringwindow_deg_bin_id, stringwindow_deg_bin, Value_String);
			Value_String = FormatFloat("#####0", ImageData.WindowsData.SizeData);
			PrintString(FStream, stringwindow_SizeData_id, stringwindow_SizeData, Value_String);
			Value_String = BoolToString(ImageData.WindowsData.BottomRight);
			PrintString(FStream, stringwindow_flag_id, stringwindow_flag, Value_String);
			Value_String = "";
			PrintString(FStream, stringwindow_info_id, stringwindow_info, Value_String);
			for (int i = 0; i < ImageData.WindowsData.SizeWindowList; i++)
			{
				Value_String = "";
				Value_String += FormatFloat("#####0 ", ImageData.WindowsData.Info[i].WindowWidth);
				Value_String += FormatFloat("#####0 ", ImageData.WindowsData.Info[i].WindowHeight);
				Value_String += FormatFloat("#####0 ", ImageData.WindowsData.Info[i].X);
				Value_String += FormatFloat("#####0 ", ImageData.WindowsData.Info[i].Y);
				Value_String += FormatFloat("#####0 ", ImageData.WindowsData.Info[i].Limit);
				Value_String += FormatFloat("#####0 ", ImageData.WindowsData.Info[i].ObjCount);
				Value_String += FormatFloat("0.0##### ", ImageData.WindowsData.Info[i].SKO);
				Value_String += FormatFloat("0.0##### ", ImageData.WindowsData.Info[i].Average);
				Value_String += FormatFloat("#####0 ", ImageData.WindowsData.Info[i].ZipX);
				Value_String += FormatFloat("#####0", ImageData.WindowsData.Info[i].ZipY);
				print_AnsiString(FStream, Value_String);
			}
		}
#pragma end_region
#pragma region videoframe
		if (ImageData.FrameData.FrameWidth != 0 && ImageData.FrameData.FrameHeight != 0)
		{
			AnsiString _endstr = "\r\n";
			Value_String = "";
			PrintString(FStream, videoframe_id, videoframe_name, Value_String);
			switch (ImageData.FrameData.DataType)
			{
			case 0: FStream->Write((byte*) ImageData.FrameData.Data, ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight * MassType[ImageData.FrameData.DataType]); break;
			case 1: FStream->Write((unsigned short*)ImageData.FrameData.Data, ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight * MassType[ImageData.FrameData.DataType]); break;
			case 2: FStream->Write((float*) ImageData.FrameData.Data, ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight * MassType[ImageData.FrameData.DataType]); break;
			}
			FStream->Write(_endstr.c_str(), _endstr.Length());
		}
#pragma end_region
#pragma region videolines
		if (ImageData.LinesData.LinesCount != 0)
		{
			AnsiString _endstr = "\r\n";
			Value_String = "";
			PrintString(FStream, videolines_id, videolines_name, Value_String);
			switch (ImageData.LinesData.DataType)
			{
			case 0: FStream->Write((byte*) ImageData.LinesData.Data, ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight * MassType[ImageData.LinesData.DataType]); break;
			case 1: FStream->Write((unsigned short*)ImageData.LinesData.Data, ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight * MassType[ImageData.LinesData.DataType]); break;
			case 2: FStream->Write((float*) ImageData.LinesData.Data, ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight * MassType[ImageData.LinesData.DataType]); break;
			}
			FStream->Write(_endstr.c_str(), _endstr.Length());
		}
#pragma end_region
#pragma region videowindow
		if (ImageData.WindowsData.SizeWindowList != 0)
		{
			AnsiString _endstr = "\r\n";
			Value_String = "";
			PrintString(FStream, videowindow_id, videowindow_name, Value_String);
			switch (ImageData.WindowsData.DataType)
			{
			case 0: FStream->Write((byte*) ImageData.WindowsData.Data, ImageData.WindowsData.SizeData * MassType[ImageData.WindowsData.DataType]); break;
			case 1: FStream->Write((unsigned short*)ImageData.WindowsData.Data, ImageData.WindowsData.SizeData * MassType[ImageData.WindowsData.DataType]); break;
			case 2: FStream->Write((float*) ImageData.WindowsData.Data, ImageData.WindowsData.SizeData * MassType[ImageData.WindowsData.DataType]); break;
			}
			FStream->Write(_endstr.c_str(), _endstr.Length());
		}
#pragma end_region
#pragma end_region
		_FreeMemory(FStream);
	}
	// ---------------------------------------------------------------------------

	void Clear_Memory(void)
	{
		_FreeMemoryArray(CameraSettings.DistortionFunction.Coefficient);
		_FreeMemoryArray(CameraSettings.TableDeadObjects.PixelData);
		_FreeMemoryArray(FilterData.DataPixels);
		_FreeMemoryArray(StarsData.StarsList);
		_FreeMemoryArray(StarsData.SimulatedFrame.StarRec);
		_FreeMemoryArray(ImageData.FrameData.Data);
		_FreeMemoryArray(ImageData.LinesData.Info);
		_FreeMemoryArray(ImageData.LinesData.Data);
		_FreeMemoryArray(ImageData.WindowsData.Info);
		_FreeMemoryArray(ImageData.WindowsData.Data);
		memset(&Georeferencing, 0, sizeof(Georeferencing));
		memset(&CameraSettings, 0, sizeof(CameraSettings));
		memset(&FilterData, 0, sizeof(FilterData));
		memset(&StarsData, 0, sizeof(StarsData));
		memset(&ImageData, 0, sizeof(ImageData));
	}
	// ---------------------------------------------------------------------------

	bool ReadFormat(AnsiString filename, bool Need_Erase)
	{
		AnsiString buferstr;
		int count_find_pos;
		int pos_index;
		unsigned int start;
		unsigned int sizedata;
		double buf[10] = {0};
		char* end_str = new char[2];
		char* bufer = new char[80];
		char* Data_buf = new char[10240]; // ���� � ������� ���� ������ ��� �� �������� � �� ������� ������ ��� ������ ������ ������
		TFileStream* FStream = new TFileStream(filename, fmOpenRead | fmShareExclusive);

		if (Need_Erase) // ���� true �� ������� ����� �������
				Clear_Memory();
		while (FStream->Position != FStream->Size)
		{
			FStream->Read(&start, sizeof(start));
			FStream->Read(&bufer[0], 80); // buferstr = AnsiString(bufer).SubString(1, 80); // "��������� ����������� ������"
			FStream->Read(&sizedata, sizeof(sizedata));
			FStream->Read(&Data_buf[0], sizedata);
			FStream->Read(&end_str[0], 2);
			if (end_str[0] != '\r' && end_str[1] != '\n')
			{
				_FreeMemoryArray(end_str);
				_FreeMemoryArray(bufer);
				_FreeMemoryArray(Data_buf);
				_FreeMemory(FStream);
				return false;
			}
			buferstr = AnsiString(Data_buf).SubString(1, sizedata);
			switch (start)
			{
#pragma region Georeferencing
			case datetime_id: Georeferencing.DateTime = StrToDateTime(buferstr); break;
			case soft_vers_id: Georeferencing.SoftwareVersion = buferstr; break;
			case frame_num_id: Georeferencing.FrameNumber = StrToInt(buferstr); break;
			case protons_density_id: Georeferencing.ProtonsDensity = StrToInt(buferstr); break;
			case tracks_brightness_id: Georeferencing.TracksBrightness = StrToInt(buferstr); break;
			case moon_velocity_id: Georeferencing.MoonVelocity = StrToInt(buferstr); break;
			case moon_brightness_id: Georeferencing.MoonBrightness = StrToInt(buferstr); break;
			case bright_left_id: Georeferencing.BrightnessLeft = StrToInt(buferstr); break;
			case bright_right_id: Georeferencing.BrightnessRight = StrToInt(buferstr); break;
			case era_id: Georeferencing.Era = (float) StrToFloat(buferstr); break;
			case orbit_eccentricity_id: Georeferencing.OrbitEccentricity = (double) StrToFloat(buferstr); break;
			case length_semiaxis_id: Georeferencing.LengthSemiaxis = (double) StrToFloat(buferstr); break;
			case length_radius_vector_id: Georeferencing.LengthRadiusVector = (double) StrToFloat(buferstr); break;
			case longitude_node_id: Georeferencing.LongitudeAscendingNode = (double) StrToArcsec(buferstr); break;
			case inclination_orbit_id: Georeferencing.InclinationOrbit = (double) StrToArcsec(buferstr); break;
			case argument_perigee_id: Georeferencing.ArgumentPerigee = (double) StrToArcsec(buferstr); break;
			case argument_latitude_id: Georeferencing.ArgumentLatitude = (double) StrToArcsec(buferstr); break;
			case dev_angular_velocity_id: StringToFloatMass(Georeferencing.DeviceAngularVelocity, 3, buferstr); break;
			case dev_angular_acceler_id: StringToFloatMass(Georeferencing.DeviceAngularAcceleration, 3, buferstr); break;
			case proj_line_velo_id: StringToFloatMass(Georeferencing.ProjectionLinearVelocity, 3, buferstr); break;
			case type_angle_id: StringToFloatMass(Georeferencing.OrientationAngles, 3, buferstr); break;
			case type_quat_id: StringToFloatMass(Georeferencing.OrientationQuaternion, 4, buferstr); break;
			case type_matr_id: StringToFloatMass3D(Georeferencing.OrientationMatrix, 3, 3, buferstr); break;
			case year_aber_app_id: Georeferencing.YearAberrationApply = StringToBool(buferstr); break;
			case satel_aber_app_id: Georeferencing.SatelliteAberrationApply = StringToBool(buferstr); break;
			case preces_nutat_app_id: Georeferencing.PrecessionNutationApply = StringToBool(buferstr); break;
			case photon_noise_app_id: Georeferencing.PhotonNoiseApply = StringToBool(buferstr); break;
			case distortion_app_id: Georeferencing.DistortionApply = StringToBool(buferstr); break;
#pragma end_region
#pragma region CameraSettings
			case device_type_id: CameraSettings.DataSource = buferstr; break;
			case device_sn_id: CameraSettings.SNDevice = StrToInt(buferstr); break;
			case inner_radius_id: CameraSettings.InnerRadius = StrToInt(buferstr); break;
			case outer_radius_id: CameraSettings.OuterRadius = StrToInt(buferstr); break;
			case resolution_acp_id: CameraSettings.ResolutionACP = StrToInt(buferstr); break;
			case ev_acp_id: CameraSettings.EV_ACP = (float) StrToFloat(buferstr); break;
			case pixel_size_id: CameraSettings.PixelSize = (float) StrToFloat(buferstr); break;
			case focal_length_id: CameraSettings.FocalLength = (float) StrToFloat(buferstr); break;
			case diametr_entrace_pupil_id: CameraSettings.DiameterEntrancePupil = (float) StrToFloat(buferstr); break;
			case dark_level_id: CameraSettings.DarkLevel = (float) StrToFloat(buferstr); break;
			case dark_sko_id: CameraSettings.DarkSKO = (float) StrToFloat(buferstr); break;
			case limit_magn_id: CameraSettings.LimitingMagnitude = (float) StrToFloat(buferstr); break;
			case stars_dist_id: CameraSettings.StarsDistribution = (float) StrToFloat(buferstr); break;
			case function_distortion_id:
				{
					StringToFloatMass(buf, 2, buferstr);
					CameraSettings.DistortionFunction.PolynomialDegree = (WORD)buf[0];
					CameraSettings.DistortionFunction.CoefficientsCount = (WORD)buf[1];
					_FreeMemoryArray(CameraSettings.DistortionFunction.Coefficient);
					CameraSettings.DistortionFunction.Coefficient = new coefonaxis[CameraSettings.DistortionFunction.CoefficientsCount];
					for (int i = 0; i < CameraSettings.DistortionFunction.CoefficientsCount; i++)
					{
						FStream->Read(&sizedata, sizeof(unsigned int));
						FStream->Read(&Data_buf[0], sizedata);
						FStream->Read(&end_str[0], 2);
						buferstr = AnsiString(Data_buf).SubString(1, sizedata);
						StringToFloatMass(buf, 2, buferstr);
						CameraSettings.DistortionFunction.Coefficient[i].X = buf[0];
						CameraSettings.DistortionFunction.Coefficient[i].Y = buf[1];
					}
					break;
				}
			case dead_pix_id:
				{
					StringToFloatMass(buf, 1, buferstr);
					CameraSettings.TableDeadObjects.RecordsCount = (WORD)buf[0];
					_FreeMemoryArray(CameraSettings.TableDeadObjects.PixelData);
					CameraSettings.TableDeadObjects.PixelData = new infopix[CameraSettings.TableDeadObjects.RecordsCount];
					for (int i = 0; i < CameraSettings.TableDeadObjects.RecordsCount; i++)
					{
						FStream->Read(&sizedata, sizeof(unsigned int));
						FStream->Read(&Data_buf[0], sizedata);
						FStream->Read(&end_str[0], 2);
						buferstr = AnsiString(Data_buf).SubString(1, sizedata);
						StringToFloatMass(buf, 4, buferstr);
						CameraSettings.TableDeadObjects.PixelData[i].X_coordinate = (float) buf[0];
						CameraSettings.TableDeadObjects.PixelData[i].Y_coordinate = (float) buf[1];
						CameraSettings.TableDeadObjects.PixelData[i].BrightnessObj = (float) buf[2];
						CameraSettings.TableDeadObjects.PixelData[i].CoefficientObj = (unsigned short)buf[3];
					}
					break;
				}
			case matrixtemp_id: CameraSettings.MatrixTemp = (float) StrToFloat(buferstr); break;
			case frequency_id: CameraSettings.Frequency = (unsigned short)StrToInt(buferstr); break;
#pragma end_region
#pragma region FilterData
			case bottomright_id: FilterData.BottomRight = StringToBool(buferstr); break;
			case filter_pix_id:
				{
					pos_index = buferstr.AnsiPos(" ");
					FilterData.FilteredPixelsCount = (WORD) StrToInt(buferstr.SubString(0, pos_index - 1));
					FilterData.TagFilter = buferstr.SubString(pos_index + 1, buferstr.Length() - pos_index);
					_FreeMemoryArray(FilterData.DataPixels);
					FilterData.DataPixels = new filterdata[FilterData.FilteredPixelsCount];
					for (int i = 0; i < FilterData.FilteredPixelsCount; i++)
					{
						FStream->Read(&sizedata, sizeof(unsigned int));
						FStream->Read(&Data_buf[0], sizedata);
						FStream->Read(&end_str[0], 2);
						buferstr = AnsiString(Data_buf).SubString(1, sizedata);
						StringToFloatMass(buf, 3, buferstr);
						FilterData.DataPixels[i].X_coordinate = (unsigned short) buf[0];
						FilterData.DataPixels[i].Y_coordinate = (unsigned short) buf[1];
						FilterData.DataPixels[i].BrightnessPixel = (unsigned short) buf[2];
					}
					break;
				}
#pragma end_region
#pragma region starsinfo
			case rezstat_id: StarsData.RezStat = (WORD) StrToInt("0x"+buferstr); break;
			case epsilon_id: StarsData.Epsilon = (float) StrToFloat(buferstr); break;
			case m_X_Y_Cyr_id:
				{
					StringToFloatMass(buf, 3, buferstr);
					StarsData.m_X = (float) buf[0];
					StarsData.m_Y = (float) buf[1];
					StarsData.m_Cur = (float) buf[2];
					break;
				}
			case sizelocallist_id: StarsData.SizeLocalList = StrToInt(buferstr); break;
			case local_recog_id:
				{
					AnsiString str;
					StringToFloatMass(buf, 2, buferstr);
					StarsData.LocalizedCount = (int)buf[0];
					StarsData.RecognizedCount = (int)buf[1];
					_FreeMemoryArray(StarsData.StarsList);
					StarsData.StarsList = new starinfo[StarsData.SizeLocalList];
					memset(StarsData.StarsList, 0, sizeof(starinfo)*StarsData.SizeLocalList);
					for (int i = 0; i < StarsData.SizeLocalList; i++)
					{
						FStream->Read(&sizedata, sizeof(unsigned int));
						FStream->Read(&Data_buf[0], sizedata);
						for (int k = 0; k < sizedata; k++)
							if (Data_buf[k] == '\0')
								Data_buf[k] = '.';

						FStream->Read(&end_str[0], 2);
						buferstr = AnsiString(Data_buf).SubString(1, sizedata);
						count_find_pos = 0;
						for (int j = 0; j < 9; j++)
						{
							if (buferstr.Length() != 0)
							{
								pos_index = buferstr.AnsiPos(" ");
								if (pos_index != 0)
								{
									switch (count_find_pos)
									{
									case 0: StarsData.StarsList[i].BrightnessObject = (float) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
									case 1: StarsData.StarsList[i].PixelsCount = (float) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
									case 2: StarsData.StarsList[i].X = (float) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
									case 3: StarsData.StarsList[i].Y = (float) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
									case 4: StarsData.StarsList[i].NumberStar = (WORD) StrToInt(buferstr.SubString(0, pos_index - 1)); break;
									case 5: StarsData.StarsList[i].Mv = (float) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
									case 6:
										{
											str = buferstr.SubString(0, pos_index - 1);
											Move(str.c_str(), &StarsData.StarsList[i].Sp[0], 2);
											break;
										}
									case 7: StarsData.StarsList[i].DX = (float) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;

									}
									buferstr = buferstr.SubString(pos_index + 1, buferstr.Length() - pos_index);
									count_find_pos++;
								}
								else
									StarsData.StarsList[i].DY = (float) StrToFloat(buferstr);
							}
							else
								break;
						}
					}
					break;
				}
			case sizestarlist_id: StarsData.SimulatedFrame.SizeStarList = (WORD)StrToInt(buferstr); break;
 			case rec_id:
				{
					StarsData.SimulatedFrame.strrec = (WORD) StrToInt(buferstr);
					_FreeMemoryArray(StarsData.SimulatedFrame.StarRec);
					StarsData.SimulatedFrame.StarRec = new STARREC[StarsData.SimulatedFrame.SizeStarList];
					for (int i = 0; i < StarsData.SimulatedFrame.SizeStarList; i++)
					{
						FStream->Read(&sizedata, sizeof(unsigned int));
						FStream->Read(&Data_buf[0], sizedata);
						FStream->Read(&end_str[0], 2);
						buferstr = AnsiString(Data_buf).SubString(1, sizedata);
						count_find_pos = 0;
						for (int j = 0; j < 6; j++)
						{
							pos_index = buferstr.AnsiPos(" ");
							if (pos_index != 0)
							{
								switch (count_find_pos)
								{
								case 0: StarsData.SimulatedFrame.StarRec[i].Ns = (WORD) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
								case 1: StarsData.SimulatedFrame.StarRec[i].Is = (unsigned long) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
								case 2: StarsData.SimulatedFrame.StarRec[i].Xs = (float) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
								case 3: StarsData.SimulatedFrame.StarRec[i].Ys = (float) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
								case 4: StarsData.SimulatedFrame.StarRec[i].Mv = (float) StrToFloat(buferstr.SubString(0, pos_index - 1)); break;
								}
								buferstr = buferstr.SubString(pos_index + 1, buferstr.Length() - pos_index);
								count_find_pos++;
							}
							else
								Move(buferstr.c_str(), &StarsData.SimulatedFrame.StarRec[i].Sp[0], 2);
						}
					}
					break;
				}
			case rec_orient_angl_id: StringToFloatMass(StarsData.RecognizedOrientationAngles, 3, buferstr); break;
			case rec_orient_quat_id: StringToFloatMass(StarsData.RecognizedOrientationQuaternion, 4, buferstr); break;
			case rec_orient_matr_id: StringToFloatMass3D(StarsData.RecognizedOrientationMatrix, 3, 3, buferstr); break;
			case rec_angular_vel_id: StringToFloatMass(StarsData.RecognizedAngularVelocity, 3, buferstr); break;
#pragma end_region
#pragma region videoframe
			case exp_time_id: ImageData.ExposureTime = (float)StrToFloat(buferstr); break;
			case stringframe_type_id: ImageData.FrameData.DataType = (unsigned int)StrToInt(buferstr); break;
			case stringframe_deg_bin_id: ImageData.FrameData.DegreeBinning = (unsigned int)StrToInt(buferstr); break;
			case stringframe_width_id: ImageData.FrameData.FrameWidth = (unsigned int)StrToInt(buferstr); break;
			case stringframe_height_id: ImageData.FrameData.FrameHeight = (unsigned int)StrToInt(buferstr); break;
			case stringframe_SizeData_id: ImageData.FrameData.SizeData = (unsigned int)StrToInt(buferstr); break;

			case stringlines_count_id: ImageData.LinesData.LinesCount = (unsigned short)StrToInt(buferstr); break;
			case stringlines_type_id: ImageData.LinesData.DataType = (unsigned int)StrToInt(buferstr); break;
			case stringlines_deg_bin_id: ImageData.LinesData.DegreeBinning = (unsigned int)StrToInt(buferstr); break;
			case stringlines_width_id: ImageData.LinesData.LinesWidth = (unsigned int)StrToInt(buferstr); break;
			case stringlines_height_id: ImageData.LinesData.LinesHeight = (unsigned int)StrToInt(buferstr); break;
			case stringlines_SizeData_id: ImageData.LinesData.SizeData = (unsigned int)StrToInt(buferstr); break;
			case stringlines_info_id:
				{
					_FreeMemoryArray(ImageData.LinesData.Info);
					ImageData.LinesData.Info = new dataline[ImageData.LinesData.LinesCount];
					for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
					{
						FStream->Read(&sizedata, sizeof(unsigned int));
						FStream->Read(&Data_buf[0], sizedata);
						FStream->Read(&end_str[0], 2);
						buferstr = AnsiString(Data_buf).SubString(1, sizedata);
						StringToFloatMass(buf, 2, buferstr);
						ImageData.LinesData.Info[i].Y_FirstString = (int) buf[0];
						ImageData.LinesData.Info[i].CountString = (int) buf[1];
					}
					break;
				}
			case stringwindow_sizewindowlist_id: ImageData.WindowsData.SizeWindowList  = (unsigned short)StrToInt(buferstr); break;
			case stringwindow_count_id: ImageData.WindowsData.WindowCount = (unsigned short)StrToInt(buferstr); break;
			case stringwindow_type_id: ImageData.WindowsData.DataType = (unsigned int)StrToInt(buferstr); break;
			case stringwindow_deg_bin_id: ImageData.WindowsData.DegreeBinning = (unsigned int)StrToInt(buferstr); break;
			case stringwindow_SizeData_id: ImageData.WindowsData.SizeData = (unsigned int)StrToInt(buferstr); break;
			case stringwindow_flag_id: ImageData.WindowsData.BottomRight = StringToBool(buferstr); break;
			case stringwindow_info_id:
				{
					_FreeMemoryArray(ImageData.WindowsData.Info);
					ImageData.WindowsData.Info = new datawindow[ImageData.WindowsData.SizeWindowList];
					for (int i = 0; i < ImageData.WindowsData.SizeWindowList; i++)
					{
						FStream->Read(&sizedata, sizeof(unsigned int));
						FStream->Read(&Data_buf[0], sizedata);
						FStream->Read(&end_str[0], 2);
						buferstr = AnsiString(Data_buf).SubString(1, sizedata);
						StringToFloatMass(buf, 10, buferstr);
						ImageData.WindowsData.Info[i].WindowWidth = (unsigned int) buf[0];
						ImageData.WindowsData.Info[i].WindowHeight = (unsigned int) buf[1];
						ImageData.WindowsData.Info[i].X = (unsigned int) buf[2];
						ImageData.WindowsData.Info[i].Y = (unsigned int) buf[3];
						ImageData.WindowsData.Info[i].Limit = (unsigned int) buf[4];
						ImageData.WindowsData.Info[i].ObjCount = (unsigned int) buf[5];
						ImageData.WindowsData.Info[i].SKO = (float) buf[6];
						ImageData.WindowsData.Info[i].Average = (float) buf[7];
						ImageData.WindowsData.Info[i].ZipX = (unsigned short) buf[8];
						ImageData.WindowsData.Info[i].ZipY = (unsigned short) buf[9];
					}
					break;
				}
			case stringframe_id:
				{
					StringToFloatMass(buf, 5, buferstr);
					ImageData.ExposureTime = (float) buf[0];
					ImageData.FrameData.DataType = (unsigned int) buf[1];
					ImageData.FrameData.DegreeBinning = (unsigned int) buf[2];
					ImageData.FrameData.FrameWidth = (unsigned int) buf[3];
					ImageData.FrameData.FrameHeight = (unsigned int) buf[4];
					break;
				}
			case stringlines_id:
				{
					StringToFloatMass(buf, 6, buferstr);
					ImageData.ExposureTime = (float) buf[0];
					ImageData.LinesData.LinesCount = (unsigned short) buf[1];
					ImageData.LinesData.DataType = (unsigned int) buf[2];
					ImageData.LinesData.DegreeBinning = (unsigned int) buf[3];
					ImageData.LinesData.LinesWidth = (unsigned int) buf[4];
					ImageData.LinesData.LinesHeight = (unsigned int) buf[5];
					_FreeMemoryArray(ImageData.LinesData.Info);
					ImageData.LinesData.Info = new dataline[ImageData.LinesData.LinesCount];
					for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
					{
						FStream->Read(&sizedata, sizeof(unsigned int));
						FStream->Read(&Data_buf[0], sizedata);
						FStream->Read(&end_str[0], 2);
						buferstr = AnsiString(Data_buf).SubString(1, sizedata);
						StringToFloatMass(buf, 2, buferstr);
						ImageData.LinesData.Info[i].Y_FirstString = (int) buf[0];
						ImageData.LinesData.Info[i].CountString = (int) buf[1];
					}
					break;
				}
			case stringwindow_id:
				{
					StringToFloatMass(buf, 5, buferstr);
					ImageData.ExposureTime = (float) buf[0];
					ImageData.WindowsData.WindowCount = (unsigned short) buf[1];
					ImageData.WindowsData.DataType = (unsigned int) buf[2];
					ImageData.WindowsData.DegreeBinning = (unsigned int) buf[3];
					ImageData.WindowsData.SizeData = (unsigned int) buf[4];
					_FreeMemoryArray(ImageData.WindowsData.Info);
					ImageData.WindowsData.Info = new datawindow[ImageData.WindowsData.WindowCount];
					for (int i = 0; i < ImageData.WindowsData.WindowCount; i++)
					{
						FStream->Read(&sizedata, sizeof(unsigned int));
						FStream->Read(&Data_buf[0], sizedata);
						FStream->Read(&end_str[0], 2);
						buferstr = AnsiString(Data_buf).SubString(1, sizedata);
						StringToFloatMass(buf, 8, buferstr);
						ImageData.WindowsData.Info[i].WindowWidth = (unsigned int) buf[0];
						ImageData.WindowsData.Info[i].WindowHeight = (unsigned int) buf[1];
						ImageData.WindowsData.Info[i].X = (unsigned int) buf[2];
						ImageData.WindowsData.Info[i].Y = (unsigned int) buf[3];
						ImageData.WindowsData.Info[i].Limit = (unsigned int) buf[4];
						ImageData.WindowsData.Info[i].ObjCount = (unsigned int) buf[5];
						ImageData.WindowsData.Info[i].SKO = (float) buf[6];
						ImageData.WindowsData.Info[i].Average = (float) buf[7];
					}
					break;
				}
			case videoframe_id:
				{
					int size = ImageData.FrameData.FrameHeight * ImageData.FrameData.FrameWidth;
					_FreeMemoryArray(ImageData.FrameData.Data);
					switch (ImageData.FrameData.DataType)
					{
					case 0: ImageData.FrameData.Data = (void*)new byte[size]; break;
					case 1: ImageData.FrameData.Data = (void*)new unsigned short[size]; break;
					case 2: ImageData.FrameData.Data = (void*)new float[size]; break;
					}
					if (ImageData.FrameData.SizeData != 0)
						FStream->Read(ImageData.FrameData.Data, size * MassType[ImageData.FrameData.DataType]);
					FStream->Read(&end_str[0], 2);
					break;
				}
			case videolines_id:
				{
					int size = ImageData.LinesData.LinesHeight * ImageData.LinesData.LinesWidth;
					_FreeMemoryArray(ImageData.LinesData.Data);
					switch (ImageData.LinesData.DataType)
					{
					case 0: ImageData.LinesData.Data = (void*)new byte[size]; break;
					case 1: ImageData.LinesData.Data = (void*)new unsigned short[size]; break;
					case 2: ImageData.LinesData.Data = (void*)new float[size]; break;
					}
					if (ImageData.LinesData.SizeData != 0)
						FStream->Read(ImageData.LinesData.Data, size * MassType[ImageData.LinesData.DataType]);
					FStream->Read(&end_str[0], 2);
					break;
				}
			case videowindow_id:
				{
					_FreeMemoryArray(ImageData.WindowsData.Data);
					switch (ImageData.WindowsData.DataType)
					{
					case 0: ImageData.WindowsData.Data = (void*)new byte[ImageData.WindowsData.SizeData]; break;
					case 1: ImageData.WindowsData.Data = (void*)new unsigned short[ImageData.WindowsData.SizeData]; break;
					case 2: ImageData.WindowsData.Data = (void*)new float[ImageData.WindowsData.SizeData]; break;
					}
					if (ImageData.WindowsData.SizeData != 0)
						FStream->Read(ImageData.WindowsData.Data, ImageData.WindowsData.SizeData * MassType[ImageData.WindowsData.DataType]);
					FStream->Read(&end_str[0], 2);
					break;
				}
#pragma end_region
			}
		}
		_FreeMemoryArray(end_str);
		_FreeMemoryArray(bufer);
		_FreeMemoryArray(Data_buf);
		_FreeMemory(FStream);
		return true;
	}
	// ---------------------------------------------------------------------------

	unsigned char* ZipStructure(unsigned int &SizeInfo)
	{
		unsigned int position = 0;
		unsigned int size_id; // ������ ������, ��� ������� ��������

		unsigned int whenwhere_size =
			sizeof(Georeferencing.DateTime)+
			sizeof(size_id) + // ������ ������������ ������,
			Georeferencing.SoftwareVersion.Length()+ // ��� �����
			sizeof(Georeferencing.FrameNumber)+
			sizeof(Georeferencing.ProtonsDensity)+
			sizeof(Georeferencing.TracksBrightness)+
			sizeof(Georeferencing.MoonVelocity)+
			sizeof(Georeferencing.MoonBrightness)+
			sizeof(Georeferencing.BrightnessLeft)+
			sizeof(Georeferencing.BrightnessRight)+
			sizeof(Georeferencing.Era)+
			sizeof(Georeferencing.OrbitEccentricity)+
			sizeof(Georeferencing.LengthSemiaxis)+
			sizeof(Georeferencing.LengthRadiusVector)+
			sizeof(Georeferencing.LongitudeAscendingNode)+
			sizeof(Georeferencing.InclinationOrbit)+
			sizeof(Georeferencing.ArgumentPerigee)+
			sizeof(Georeferencing.ArgumentLatitude)+
			sizeof(Georeferencing.DeviceAngularVelocity[3])+
			sizeof(Georeferencing.DeviceAngularAcceleration[3])+
			sizeof(Georeferencing.ProjectionLinearVelocity[3])+
			sizeof(Georeferencing.OrientationAngles[3])+
			sizeof(Georeferencing.OrientationQuaternion[4])+
			sizeof(Georeferencing.OrientationMatrix[3][3])+
			sizeof(Georeferencing.YearAberrationApply)+
			sizeof(Georeferencing.SatelliteAberrationApply)+
			sizeof(Georeferencing.PrecessionNutationApply)+
			sizeof(Georeferencing.PhotonNoiseApply)+
			sizeof(Georeferencing.DistortionApply);

		unsigned int devicetype_size =
			sizeof(size_id) +
			CameraSettings.DataSource.Length()+
			sizeof(CameraSettings.SNDevice)+
			sizeof(CameraSettings.InnerRadius)+
			sizeof(CameraSettings.OuterRadius)+
			sizeof(CameraSettings.ResolutionACP)+
			sizeof(CameraSettings.EV_ACP)+
			sizeof(CameraSettings.PixelSize)+
			sizeof(CameraSettings.FocalLength)+
			sizeof(CameraSettings.DiameterEntrancePupil)+
			sizeof(CameraSettings.DarkLevel)+
			sizeof(CameraSettings.DarkSKO)+
			sizeof(CameraSettings.LimitingMagnitude)+
			sizeof(CameraSettings.StarsDistribution)+
			sizeof(CameraSettings.DistortionFunction.PolynomialDegree)+
			sizeof(CameraSettings.DistortionFunction.CoefficientsCount)+
			sizeof(coefonaxis) * CameraSettings.DistortionFunction.CoefficientsCount +
			sizeof(CameraSettings.TableDeadObjects.RecordsCount)+
			sizeof(infopix) * CameraSettings.TableDeadObjects.RecordsCount;

		unsigned int filterframe_size =
			sizeof(size_id) +
			FilterData.TagFilter.Length()+
			sizeof(FilterData.FilteredPixelsCount)+
			sizeof(FilterData.BottomRight)+
			sizeof(filterdata) * FilterData.FilteredPixelsCount;

		unsigned int starsinfo_size =
			sizeof(StarsData.RezStat)+
			sizeof(StarsData.Epsilon)+
			sizeof(StarsData.m_X)+sizeof(StarsData.m_Y)+sizeof(StarsData.m_Cur)+
			sizeof(StarsData.LocalizedCount)+
			sizeof(StarsData.RecognizedCount)+
            sizeof(StarsData.SizeLocalList)+
			sizeof(starinfo) * StarsData.LocalizedCount +
			sizeof(StarsData.SimulatedFrame.SizeStarList) +
			sizeof(StarsData.SimulatedFrame.strrec)+
			sizeof(STARREC) * StarsData.SimulatedFrame.strrec +
			sizeof(StarsData.RecognizedOrientationAngles)+
			sizeof(StarsData.RecognizedOrientationQuaternion)+
			sizeof(StarsData.RecognizedOrientationMatrix);

		unsigned int videodata_size =
			sizeof(ImageData.ExposureTime)+
			sizeof(ImageData.FrameData.DataType)+
			sizeof(ImageData.FrameData.DegreeBinning)+
			sizeof(ImageData.FrameData.FrameWidth)+
			sizeof(ImageData.FrameData.FrameHeight) +
			(MassType[ImageData.FrameData.DataType] * ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight)+

			sizeof(ImageData.LinesData.LinesCount)+
			sizeof(ImageData.LinesData.DataType)+
			sizeof(ImageData.LinesData.DegreeBinning)+
			sizeof(ImageData.LinesData.LinesWidth)+
			sizeof(ImageData.LinesData.LinesHeight)+
			sizeof(dataline) * ImageData.LinesData.LinesCount +
			(MassType[ImageData.LinesData.DataType] * ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight)+

			sizeof(ImageData.WindowsData.SizeWindowList)+
			sizeof(ImageData.WindowsData.WindowCount)+
			sizeof(ImageData.WindowsData.DataType)+
			sizeof(ImageData.WindowsData.DegreeBinning)+
			sizeof(ImageData.WindowsData.SizeData)+
			sizeof(ImageData.WindowsData.BottomRight) +
			(sizeof(datawindow) * ImageData.WindowsData.SizeWindowList) +
			(MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData);

		SizeInfo = whenwhere_size + devicetype_size + filterframe_size + starsinfo_size + videodata_size;
		unsigned char* Array = new unsigned char[SizeInfo];
		memset(Array, 0, SizeInfo);

		Move(&Georeferencing.DateTime, &Array[position], sizeof(Georeferencing.DateTime)); position += sizeof(Georeferencing.DateTime);
		size_id = Georeferencing.SoftwareVersion.Length();
		Move(&size_id, &Array[position], sizeof(size_id)); position += sizeof(size_id); // ������ ����� ������
		Move(Georeferencing.SoftwareVersion.c_str(), &Array[position], size_id); position += size_id;
		Move(&Georeferencing.FrameNumber, &Array[position], sizeof(Georeferencing.FrameNumber)); position += sizeof(Georeferencing.FrameNumber);
		Move(&Georeferencing.ProtonsDensity, &Array[position], sizeof(Georeferencing.ProtonsDensity)); position += sizeof(Georeferencing.ProtonsDensity);
		Move(&Georeferencing.TracksBrightness, &Array[position], sizeof(Georeferencing.TracksBrightness)); position += sizeof(Georeferencing.TracksBrightness);
		Move(&Georeferencing.MoonVelocity, &Array[position], sizeof(Georeferencing.MoonVelocity)); position += sizeof(Georeferencing.MoonVelocity);
		Move(&Georeferencing.MoonBrightness, &Array[position], sizeof(Georeferencing.MoonBrightness)); position += sizeof(Georeferencing.MoonBrightness);
		Move(&Georeferencing.BrightnessLeft, &Array[position], sizeof(Georeferencing.BrightnessLeft)); position += sizeof(Georeferencing.BrightnessLeft);
		Move(&Georeferencing.BrightnessRight, &Array[position], sizeof(Georeferencing.BrightnessRight)); position += sizeof(Georeferencing.BrightnessRight);
		Move(&Georeferencing.Era, &Array[position], sizeof(Georeferencing.Era)); position += sizeof(Georeferencing.Era);
		Move(&Georeferencing.OrbitEccentricity, &Array[position], sizeof(Georeferencing.OrbitEccentricity)); position += sizeof(Georeferencing.OrbitEccentricity);
		Move(&Georeferencing.LengthSemiaxis, &Array[position], sizeof(Georeferencing.LengthSemiaxis)); position += sizeof(Georeferencing.LengthSemiaxis);
		Move(&Georeferencing.LengthRadiusVector, &Array[position], sizeof(Georeferencing.LengthRadiusVector)); position += sizeof(Georeferencing.LengthRadiusVector);
		Move(&Georeferencing.LongitudeAscendingNode, &Array[position], sizeof(Georeferencing.LongitudeAscendingNode)); position += sizeof(Georeferencing.LongitudeAscendingNode);
		Move(&Georeferencing.InclinationOrbit, &Array[position], sizeof(Georeferencing.InclinationOrbit)); position += sizeof(Georeferencing.InclinationOrbit);
		Move(&Georeferencing.ArgumentPerigee, &Array[position], sizeof(Georeferencing.ArgumentPerigee)); position += sizeof(Georeferencing.ArgumentPerigee);
		Move(&Georeferencing.ArgumentLatitude, &Array[position], sizeof(Georeferencing.ArgumentLatitude)); position += sizeof(Georeferencing.ArgumentLatitude);
		Move(&Georeferencing.DeviceAngularVelocity, &Array[position], sizeof(Georeferencing.DeviceAngularVelocity)); position += sizeof(Georeferencing.DeviceAngularVelocity);
		Move(&Georeferencing.DeviceAngularAcceleration, &Array[position], sizeof(Georeferencing.DeviceAngularAcceleration)); position += sizeof(Georeferencing.DeviceAngularAcceleration);
		Move(&Georeferencing.ProjectionLinearVelocity, &Array[position], sizeof(Georeferencing.ProjectionLinearVelocity)); position += sizeof(Georeferencing.ProjectionLinearVelocity);
		Move(&Georeferencing.OrientationAngles, &Array[position], sizeof(Georeferencing.OrientationAngles)); position += sizeof(Georeferencing.OrientationAngles);
		Move(&Georeferencing.OrientationQuaternion, &Array[position], sizeof(Georeferencing.OrientationQuaternion)); position += sizeof(Georeferencing.OrientationQuaternion);
		Move(&Georeferencing.OrientationMatrix, &Array[position], sizeof(Georeferencing.OrientationMatrix)); position += sizeof(Georeferencing.OrientationMatrix);
		Move(&Georeferencing.YearAberrationApply, &Array[position], sizeof(Georeferencing.YearAberrationApply)); position += sizeof(Georeferencing.YearAberrationApply);
		Move(&Georeferencing.SatelliteAberrationApply, &Array[position], sizeof(Georeferencing.SatelliteAberrationApply)); position += sizeof(Georeferencing.SatelliteAberrationApply);
		Move(&Georeferencing.PrecessionNutationApply, &Array[position], sizeof(Georeferencing.PrecessionNutationApply)); position += sizeof(Georeferencing.PrecessionNutationApply);
		Move(&Georeferencing.PhotonNoiseApply, &Array[position], sizeof(Georeferencing.PhotonNoiseApply)); position += sizeof(Georeferencing.PhotonNoiseApply);
		Move(&Georeferencing.DistortionApply, &Array[position], sizeof(Georeferencing.DistortionApply)); position += sizeof(Georeferencing.DistortionApply);

		size_id = CameraSettings.DataSource.Length();
		Move(&size_id, &Array[position], sizeof(size_id)); position += sizeof(size_id); // ������ ����� ������
		Move(CameraSettings.DataSource.c_str(), &Array[position], size_id); position += size_id;
		Move(&CameraSettings.SNDevice, &Array[position], sizeof(CameraSettings.SNDevice)); position += sizeof(CameraSettings.SNDevice);
		Move(&CameraSettings.InnerRadius, &Array[position], sizeof(CameraSettings.InnerRadius)); position += sizeof(CameraSettings.InnerRadius);
		Move(&CameraSettings.OuterRadius, &Array[position], sizeof(CameraSettings.OuterRadius)); position += sizeof(CameraSettings.OuterRadius);
		Move(&CameraSettings.ResolutionACP, &Array[position], sizeof(CameraSettings.ResolutionACP)); position += sizeof(CameraSettings.ResolutionACP);
		Move(&CameraSettings.EV_ACP, &Array[position], sizeof(CameraSettings.EV_ACP)); position += sizeof(CameraSettings.EV_ACP);
		Move(&CameraSettings.PixelSize, &Array[position], sizeof(CameraSettings.PixelSize)); position += sizeof(CameraSettings.PixelSize);
		Move(&CameraSettings.FocalLength, &Array[position], sizeof(CameraSettings.FocalLength)); position += sizeof(CameraSettings.FocalLength);
		Move(&CameraSettings.DiameterEntrancePupil, &Array[position], sizeof(CameraSettings.DiameterEntrancePupil)); position += sizeof(CameraSettings.DiameterEntrancePupil);
		Move(&CameraSettings.DarkLevel, &Array[position], sizeof(CameraSettings.DarkLevel)); position += sizeof(CameraSettings.DarkLevel);
		Move(&CameraSettings.DarkSKO, &Array[position], sizeof(CameraSettings.DarkSKO)); position += sizeof(CameraSettings.DarkSKO);
		Move(&CameraSettings.LimitingMagnitude, &Array[position], sizeof(CameraSettings.LimitingMagnitude)); position += sizeof(CameraSettings.LimitingMagnitude);
		Move(&CameraSettings.StarsDistribution, &Array[position], sizeof(CameraSettings.StarsDistribution)); position += sizeof(CameraSettings.StarsDistribution);
		Move(&CameraSettings.DistortionFunction.PolynomialDegree, &Array[position], sizeof(CameraSettings.DistortionFunction.PolynomialDegree)); position += sizeof(CameraSettings.DistortionFunction.PolynomialDegree);
		Move(&CameraSettings.DistortionFunction.CoefficientsCount, &Array[position], sizeof(CameraSettings.DistortionFunction.CoefficientsCount)); position += sizeof(CameraSettings.DistortionFunction.CoefficientsCount);
		for (int i = 0; i < CameraSettings.DistortionFunction.CoefficientsCount; i++)
		{
			Move(&CameraSettings.DistortionFunction.Coefficient[i], &Array[position], sizeof(coefonaxis)); position += sizeof(coefonaxis);
		}
		Move(&CameraSettings.TableDeadObjects.RecordsCount, &Array[position], sizeof(CameraSettings.TableDeadObjects.RecordsCount)); position += sizeof(CameraSettings.TableDeadObjects.RecordsCount);
		for (int i = 0; i < CameraSettings.TableDeadObjects.RecordsCount; i++)
		{
			Move(&CameraSettings.TableDeadObjects.PixelData[i], &Array[position], sizeof(infopix)); position += sizeof(infopix);
		}

		size_id = FilterData.TagFilter.Length();
		Move(&size_id, &Array[position], sizeof(size_id)); position += sizeof(size_id); // ������ ����� ������
		Move(FilterData.TagFilter.c_str(), &Array[position], size_id); position += size_id;
		Move(&FilterData.FilteredPixelsCount, &Array[position], sizeof(FilterData.FilteredPixelsCount)); position += sizeof(FilterData.FilteredPixelsCount);
		Move(&FilterData.BottomRight, &Array[position], sizeof(FilterData.BottomRight)); position += sizeof(FilterData.BottomRight);
		for (int i = 0; i < FilterData.FilteredPixelsCount; i++)
		{
			Move(&FilterData.DataPixels[i], &Array[position], sizeof(filterdata)); position += sizeof(filterdata);
		}

		Move(&StarsData.RezStat, &Array[position], sizeof(StarsData.RezStat)); position += sizeof(StarsData.RezStat);
		Move(&StarsData.Epsilon, &Array[position], sizeof(StarsData.Epsilon)); position += sizeof(StarsData.Epsilon);
		Move(&StarsData.m_X, &Array[position], sizeof(StarsData.m_X)); position += sizeof(StarsData.m_X);
		Move(&StarsData.m_Y, &Array[position], sizeof(StarsData.m_Y)); position += sizeof(StarsData.m_Y);
		Move(&StarsData.m_Cur, &Array[position], sizeof(StarsData.m_Cur)); position += sizeof(StarsData.m_Cur);

		Move(&StarsData.SizeLocalList , &Array[position], sizeof(StarsData.SizeLocalList)); position += sizeof(StarsData.SizeLocalList);
		Move(&StarsData.LocalizedCount, &Array[position], sizeof(StarsData.LocalizedCount)); position += sizeof(StarsData.LocalizedCount);
		Move(&StarsData.RecognizedCount, &Array[position], sizeof(StarsData.RecognizedCount)); position += sizeof(StarsData.RecognizedCount);
		for (int i = 0; i < StarsData.SizeLocalList; i++)
		{
			Move(&StarsData.StarsList[i], &Array[position], sizeof(starinfo)); position += sizeof(starinfo);
		}
	    Move(&StarsData.SimulatedFrame.SizeStarList, &Array[position], sizeof(StarsData.SimulatedFrame.SizeStarList)); position += sizeof(StarsData.SimulatedFrame.SizeStarList);
		Move(&StarsData.SimulatedFrame.strrec, &Array[position], sizeof(StarsData.SimulatedFrame.strrec)); position += sizeof(StarsData.SimulatedFrame.strrec);
		for (int i = 0; i < StarsData.SimulatedFrame.strrec; i++)
		{
			Move(&StarsData.SimulatedFrame.StarRec[i], &Array[position], sizeof(starinfo)); position += sizeof(STARREC);
		}
		Move(&StarsData.RecognizedOrientationAngles, &Array[position], sizeof(StarsData.RecognizedOrientationAngles)); position += sizeof(StarsData.RecognizedOrientationAngles);
		Move(&StarsData.RecognizedOrientationQuaternion, &Array[position], sizeof(StarsData.RecognizedOrientationQuaternion)); position += sizeof(StarsData.RecognizedOrientationQuaternion);
		Move(&StarsData.RecognizedOrientationMatrix, &Array[position], sizeof(StarsData.RecognizedOrientationMatrix)); position += sizeof(StarsData.RecognizedOrientationMatrix);

		Move(&ImageData.ExposureTime, &Array[position], sizeof(ImageData.ExposureTime)); position += sizeof(ImageData.ExposureTime);
		Move(&ImageData.FrameData.DataType, &Array[position], sizeof(ImageData.FrameData.DataType)); position += sizeof(ImageData.FrameData.DataType);
		Move(&ImageData.FrameData.DegreeBinning, &Array[position], sizeof(ImageData.FrameData.DegreeBinning)); position += sizeof(ImageData.FrameData.DegreeBinning);
		Move(&ImageData.FrameData.FrameWidth, &Array[position], sizeof(ImageData.FrameData.FrameWidth)); position += sizeof(ImageData.FrameData.FrameWidth);
		Move(&ImageData.FrameData.FrameHeight, &Array[position], sizeof(ImageData.FrameData.FrameHeight)); position += sizeof(ImageData.FrameData.FrameHeight);
		Move(ImageData.FrameData.Data, &Array[position], (MassType[ImageData.FrameData.DataType] * ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight)); position += (MassType[ImageData.FrameData.DataType] * ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight);

		Move(&ImageData.LinesData.LinesCount, &Array[position], sizeof(ImageData.LinesData.LinesCount)); position += sizeof(ImageData.LinesData.LinesCount);
		Move(&ImageData.LinesData.DataType, &Array[position], sizeof(ImageData.LinesData.DataType)); position += sizeof(ImageData.LinesData.DataType);
		Move(&ImageData.LinesData.DegreeBinning, &Array[position], sizeof(ImageData.LinesData.DegreeBinning)); position += sizeof(ImageData.LinesData.DegreeBinning);
		Move(&ImageData.LinesData.LinesWidth, &Array[position], sizeof(ImageData.LinesData.LinesWidth)); position += sizeof(ImageData.LinesData.LinesWidth);
		Move(&ImageData.LinesData.LinesHeight, &Array[position], sizeof(ImageData.LinesData.LinesHeight)); position += sizeof(ImageData.LinesData.LinesHeight);
		for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
		{
			Move(&ImageData.LinesData.Info[i], &Array[position], sizeof(dataline)); position += sizeof(dataline);
		}
		Move(ImageData.LinesData.Data, &Array[position], (MassType[ImageData.LinesData.DataType] * ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight)); position += (MassType[ImageData.LinesData.DataType] * ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight);

		Move(&ImageData.WindowsData.SizeWindowList, &Array[position], sizeof(ImageData.WindowsData.SizeWindowList)); position += sizeof(ImageData.WindowsData.SizeWindowList);
		Move(&ImageData.WindowsData.WindowCount, &Array[position], sizeof(ImageData.WindowsData.WindowCount)); position += sizeof(ImageData.WindowsData.WindowCount);
		Move(&ImageData.WindowsData.DataType, &Array[position], sizeof(ImageData.WindowsData.DataType)); position += sizeof(ImageData.WindowsData.DataType);
		Move(&ImageData.WindowsData.DegreeBinning, &Array[position], sizeof(ImageData.WindowsData.DegreeBinning)); position += sizeof(ImageData.WindowsData.DegreeBinning);
		Move(&ImageData.WindowsData.SizeData, &Array[position], sizeof(ImageData.WindowsData.SizeData)); position += sizeof(ImageData.WindowsData.SizeData);
		Move(&ImageData.WindowsData.BottomRight, &Array[position], sizeof(ImageData.WindowsData.BottomRight)); position += sizeof(ImageData.WindowsData.BottomRight);
		for (int i = 0; i < ImageData.WindowsData.SizeWindowList; i++)
		{
			Move(&ImageData.WindowsData.Info[i], &Array[position], sizeof(datawindow)); position += sizeof(datawindow);
		}
		Move(ImageData.WindowsData.Data, &Array[position], (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData)); position += (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData);
		return Array;
	}
	// ---------------------------------------------------------------------------

	void UnZipStructure(unsigned char* Array, unsigned int SizeInfo)
	{
		unsigned int position = 0;
		unsigned int size_id; // ������ ������, ��� ������� ��������

		Move(&Array[position], &Georeferencing.DateTime, sizeof(Georeferencing.DateTime)); position += sizeof(Georeferencing.DateTime);
		Move(&Array[position], &size_id, sizeof(size_id)); position += sizeof(size_id); // ������ ����� ������
		Move(&Array[position], Georeferencing.SoftwareVersion.c_str(), size_id); position += size_id;
		Move(&Array[position], &Georeferencing.FrameNumber, sizeof(Georeferencing.FrameNumber)); position += sizeof(Georeferencing.FrameNumber);
		Move(&Array[position], &Georeferencing.ProtonsDensity, sizeof(Georeferencing.ProtonsDensity)); position += sizeof(Georeferencing.ProtonsDensity);
		Move(&Array[position], &Georeferencing.TracksBrightness, sizeof(Georeferencing.TracksBrightness)); position += sizeof(Georeferencing.TracksBrightness);
		Move(&Array[position], &Georeferencing.MoonVelocity, sizeof(Georeferencing.MoonVelocity)); position += sizeof(Georeferencing.MoonVelocity);
		Move(&Array[position], &Georeferencing.MoonBrightness, sizeof(Georeferencing.MoonBrightness)); position += sizeof(Georeferencing.MoonBrightness);
		Move(&Array[position], &Georeferencing.BrightnessLeft, sizeof(Georeferencing.BrightnessLeft)); position += sizeof(Georeferencing.BrightnessLeft);
		Move(&Array[position], &Georeferencing.BrightnessRight, sizeof(Georeferencing.BrightnessRight)); position += sizeof(Georeferencing.BrightnessRight);
		Move(&Array[position], &Georeferencing.Era, sizeof(Georeferencing.Era)); position += sizeof(Georeferencing.Era);
		Move(&Array[position], &Georeferencing.OrbitEccentricity, sizeof(Georeferencing.OrbitEccentricity)); position += sizeof(Georeferencing.OrbitEccentricity);
		Move(&Array[position], &Georeferencing.LengthSemiaxis, sizeof(Georeferencing.LengthSemiaxis)); position += sizeof(Georeferencing.LengthSemiaxis);
		Move(&Array[position], &Georeferencing.LengthRadiusVector, sizeof(Georeferencing.LengthRadiusVector)); position += sizeof(Georeferencing.LengthRadiusVector);
		Move(&Array[position], &Georeferencing.LongitudeAscendingNode, sizeof(Georeferencing.LongitudeAscendingNode)); position += sizeof(Georeferencing.LongitudeAscendingNode);
		Move(&Array[position], &Georeferencing.InclinationOrbit, sizeof(Georeferencing.InclinationOrbit)); position += sizeof(Georeferencing.InclinationOrbit);
		Move(&Array[position], &Georeferencing.ArgumentPerigee, sizeof(Georeferencing.ArgumentPerigee)); position += sizeof(Georeferencing.ArgumentPerigee);
		Move(&Array[position], &Georeferencing.ArgumentLatitude, sizeof(Georeferencing.ArgumentLatitude)); position += sizeof(Georeferencing.ArgumentLatitude);
		Move(&Array[position], &Georeferencing.DeviceAngularVelocity, sizeof(Georeferencing.DeviceAngularVelocity)); position += sizeof(Georeferencing.DeviceAngularVelocity);
		Move(&Array[position], &Georeferencing.DeviceAngularAcceleration, sizeof(Georeferencing.DeviceAngularAcceleration)); position += sizeof(Georeferencing.DeviceAngularAcceleration);
		Move(&Array[position], &Georeferencing.ProjectionLinearVelocity, sizeof(Georeferencing.ProjectionLinearVelocity)); position += sizeof(Georeferencing.ProjectionLinearVelocity);
		Move(&Array[position], &Georeferencing.OrientationAngles, sizeof(Georeferencing.OrientationAngles)); position += sizeof(Georeferencing.OrientationAngles);
		Move(&Array[position], &Georeferencing.OrientationQuaternion, sizeof(Georeferencing.OrientationQuaternion)); position += sizeof(Georeferencing.OrientationQuaternion);
		Move(&Array[position], &Georeferencing.OrientationMatrix, sizeof(Georeferencing.OrientationMatrix)); position += sizeof(Georeferencing.OrientationMatrix);
		Move(&Array[position], &Georeferencing.YearAberrationApply, sizeof(Georeferencing.YearAberrationApply)); position += sizeof(Georeferencing.YearAberrationApply);
		Move(&Array[position], &Georeferencing.SatelliteAberrationApply, sizeof(Georeferencing.SatelliteAberrationApply)); position += sizeof(Georeferencing.SatelliteAberrationApply);
		Move(&Array[position], &Georeferencing.PrecessionNutationApply, sizeof(Georeferencing.PrecessionNutationApply)); position += sizeof(Georeferencing.PrecessionNutationApply);
		Move(&Array[position], &Georeferencing.PhotonNoiseApply, sizeof(Georeferencing.PhotonNoiseApply)); position += sizeof(Georeferencing.PhotonNoiseApply);
		Move(&Array[position], &Georeferencing.DistortionApply, sizeof(Georeferencing.DistortionApply)); position += sizeof(Georeferencing.DistortionApply);

		Move(&Array[position], &size_id, sizeof(size_id)); position += sizeof(size_id); // ������ ����� ������
		Move(&Array[position], CameraSettings.DataSource.c_str(), size_id); position += size_id;
		Move(&Array[position], &CameraSettings.SNDevice, sizeof(CameraSettings.SNDevice)); position += sizeof(CameraSettings.SNDevice);
		Move(&Array[position], &CameraSettings.InnerRadius, sizeof(CameraSettings.InnerRadius)); position += sizeof(CameraSettings.InnerRadius);
		Move(&Array[position], &CameraSettings.OuterRadius, sizeof(CameraSettings.OuterRadius)); position += sizeof(CameraSettings.OuterRadius);
		Move(&Array[position], &CameraSettings.ResolutionACP, sizeof(CameraSettings.ResolutionACP)); position += sizeof(CameraSettings.ResolutionACP);
		Move(&Array[position], &CameraSettings.EV_ACP, sizeof(CameraSettings.EV_ACP)); position += sizeof(CameraSettings.EV_ACP);
		Move(&Array[position], &CameraSettings.PixelSize, sizeof(CameraSettings.PixelSize)); position += sizeof(CameraSettings.PixelSize);
		Move(&Array[position], &CameraSettings.FocalLength, sizeof(CameraSettings.FocalLength)); position += sizeof(CameraSettings.FocalLength);
		Move(&Array[position], &CameraSettings.DiameterEntrancePupil, sizeof(CameraSettings.DiameterEntrancePupil)); position += sizeof(CameraSettings.DiameterEntrancePupil);
		Move(&Array[position], &CameraSettings.DarkLevel, sizeof(CameraSettings.DarkLevel)); position += sizeof(CameraSettings.DarkLevel);
		Move(&Array[position], &CameraSettings.DarkSKO, sizeof(CameraSettings.DarkSKO)); position += sizeof(CameraSettings.DarkSKO);
		Move(&Array[position], &CameraSettings.LimitingMagnitude, sizeof(CameraSettings.LimitingMagnitude)); position += sizeof(CameraSettings.LimitingMagnitude);
		Move(&Array[position], &CameraSettings.StarsDistribution, sizeof(CameraSettings.StarsDistribution)); position += sizeof(CameraSettings.StarsDistribution);
		Move(&Array[position], &CameraSettings.DistortionFunction.PolynomialDegree, sizeof(CameraSettings.DistortionFunction.PolynomialDegree)); position += sizeof(CameraSettings.DistortionFunction.PolynomialDegree);
		Move(&Array[position], &CameraSettings.DistortionFunction.CoefficientsCount, sizeof(CameraSettings.DistortionFunction.CoefficientsCount)); position += sizeof(CameraSettings.DistortionFunction.CoefficientsCount);
		_FreeMemoryArray(CameraSettings.DistortionFunction.Coefficient);
		if (CameraSettings.DistortionFunction.CoefficientsCount > 0)
		{
			CameraSettings.DistortionFunction.Coefficient = new coefonaxis[CameraSettings.DistortionFunction.CoefficientsCount];
			for (int i = 0; i < CameraSettings.DistortionFunction.CoefficientsCount; i++)
			{
				Move(&Array[position], &CameraSettings.DistortionFunction.Coefficient[i], sizeof(coefonaxis)); position += sizeof(coefonaxis);
			}
		}

		Move(&Array[position], &CameraSettings.TableDeadObjects.RecordsCount, sizeof(CameraSettings.TableDeadObjects.RecordsCount)); position += sizeof(CameraSettings.TableDeadObjects.RecordsCount);
		_FreeMemoryArray(CameraSettings.TableDeadObjects.PixelData);
		if (CameraSettings.TableDeadObjects.RecordsCount > 0)
		{
			CameraSettings.TableDeadObjects.PixelData = new infopix[CameraSettings.TableDeadObjects.RecordsCount];
			for (int i = 0; i < CameraSettings.TableDeadObjects.RecordsCount; i++)
			{
				Move(&Array[position], &CameraSettings.TableDeadObjects.PixelData[i], sizeof(infopix)); position += sizeof(infopix);
			}
		}

		Move(&Array[position], &size_id, sizeof(size_id)); position += sizeof(size_id); // ������ ����� ������
		Move(&Array[position], FilterData.TagFilter.c_str(), size_id); position += size_id;
		Move(&Array[position], &FilterData.FilteredPixelsCount, sizeof(FilterData.FilteredPixelsCount)); position += sizeof(FilterData.FilteredPixelsCount);
		Move(&Array[position], &FilterData.BottomRight, sizeof(FilterData.BottomRight)); position += sizeof(FilterData.BottomRight);
		_FreeMemoryArray(FilterData.DataPixels);
		if (FilterData.FilteredPixelsCount > 0)
		{
			FilterData.DataPixels = new filterdata[FilterData.FilteredPixelsCount];
			for (int i = 0; i < FilterData.FilteredPixelsCount; i++)
			{
				Move(&Array[position], &FilterData.DataPixels[i], sizeof(filterdata)); position += sizeof(filterdata);
			}
		}

		Move(&Array[position], &StarsData.RezStat, sizeof(StarsData.RezStat)); position += sizeof(StarsData.RezStat);
		Move(&Array[position], &StarsData.Epsilon, sizeof(StarsData.Epsilon)); position += sizeof(StarsData.Epsilon);
		Move(&Array[position], &StarsData.m_X, sizeof(StarsData.m_X)); position += sizeof(StarsData.m_X);
		Move(&Array[position], &StarsData.m_Y, sizeof(StarsData.m_Y)); position += sizeof(StarsData.m_Y);
		Move(&Array[position], &StarsData.m_Cur, sizeof(StarsData.m_Cur)); position += sizeof(StarsData.m_Cur);
		Move(&Array[position], &StarsData.SizeLocalList , sizeof(StarsData.SizeLocalList)); position += sizeof(StarsData.SizeLocalList);
		Move(&Array[position], &StarsData.LocalizedCount, sizeof(StarsData.LocalizedCount)); position += sizeof(StarsData.LocalizedCount);
		Move(&Array[position], &StarsData.RecognizedCount, sizeof(StarsData.RecognizedCount)); position += sizeof(StarsData.RecognizedCount);
		_FreeMemoryArray(StarsData.StarsList);
		if (StarsData.SizeLocalList > 0)
		{
			StarsData.StarsList = new starinfo[StarsData.SizeLocalList];
			for (int i = 0; i < StarsData.SizeLocalList; i++)
			{
				Move(&Array[position], &StarsData.StarsList[i], sizeof(starinfo)); position += sizeof(starinfo);
			}
		}
		Move(&Array[position], &StarsData.SimulatedFrame.SizeStarList, sizeof(StarsData.SimulatedFrame.SizeStarList)); position += sizeof(StarsData.SimulatedFrame.SizeStarList);
		Move(&Array[position], &StarsData.SimulatedFrame.strrec, sizeof(StarsData.SimulatedFrame.strrec)); position += sizeof(StarsData.SimulatedFrame.strrec);
		_FreeMemoryArray(StarsData.SimulatedFrame.StarRec);
		if (StarsData.SimulatedFrame.strrec > 0)
		{
			StarsData.SimulatedFrame.StarRec = new STARREC[StarsData.SimulatedFrame.strrec];
			for (int i = 0; i < StarsData.SimulatedFrame.strrec; i++)
			{
				Move(&Array[position], &StarsData.SimulatedFrame.StarRec[i], sizeof(starinfo)); position += sizeof(STARREC);
			}
		}
		Move(&Array[position], &StarsData.RecognizedOrientationAngles, sizeof(StarsData.RecognizedOrientationAngles)); position += sizeof(StarsData.RecognizedOrientationAngles);
		Move(&Array[position], &StarsData.RecognizedOrientationQuaternion, sizeof(StarsData.RecognizedOrientationQuaternion)); position += sizeof(StarsData.RecognizedOrientationQuaternion);
		Move(&Array[position], &StarsData.RecognizedOrientationMatrix, sizeof(StarsData.RecognizedOrientationMatrix)); position += sizeof(StarsData.RecognizedOrientationMatrix);

		Move(&Array[position], &ImageData.ExposureTime, sizeof(ImageData.ExposureTime)); position += sizeof(ImageData.ExposureTime);
		Move(&Array[position], &ImageData.FrameData.DataType, sizeof(ImageData.FrameData.DataType)); position += sizeof(ImageData.FrameData.DataType);
		Move(&Array[position], &ImageData.FrameData.DegreeBinning, sizeof(ImageData.FrameData.DegreeBinning)); position += sizeof(ImageData.FrameData.DegreeBinning);
		Move(&Array[position], &ImageData.FrameData.FrameWidth, sizeof(ImageData.FrameData.FrameWidth)); position += sizeof(ImageData.FrameData.FrameWidth);
		Move(&Array[position], &ImageData.FrameData.FrameHeight, sizeof(ImageData.FrameData.FrameHeight)); position += sizeof(ImageData.FrameData.FrameHeight);

		_FreeMemoryArray(ImageData.FrameData.Data);
		size_id = ImageData.FrameData.FrameWidth * ImageData.FrameData.FrameHeight;
		if (size_id > 0)
		{
			switch (ImageData.FrameData.DataType)
			{
			case 0: ImageData.FrameData.Data = (void*)new byte[size_id]; break;
			case 1: ImageData.FrameData.Data = (void*)new unsigned short[size_id]; break;
			case 2: ImageData.FrameData.Data = (void*)new float[size_id]; break;
			}
			Move(&Array[position], ImageData.FrameData.Data, (MassType[ImageData.FrameData.DataType] * size_id)); position += (MassType[ImageData.FrameData.DataType] * size_id);
		}
		Move(&Array[position], &ImageData.LinesData.LinesCount, sizeof(ImageData.LinesData.LinesCount)); position += sizeof(ImageData.LinesData.LinesCount);
		Move(&Array[position], &ImageData.LinesData.DataType, sizeof(ImageData.LinesData.DataType)); position += sizeof(ImageData.LinesData.DataType);
		Move(&Array[position], &ImageData.LinesData.DegreeBinning, sizeof(ImageData.LinesData.DegreeBinning)); position += sizeof(ImageData.LinesData.DegreeBinning);
		Move(&Array[position], &ImageData.LinesData.LinesWidth, sizeof(ImageData.LinesData.LinesWidth)); position += sizeof(ImageData.LinesData.LinesWidth);
		Move(&Array[position], &ImageData.LinesData.LinesHeight, sizeof(ImageData.LinesData.LinesHeight)); position += sizeof(ImageData.LinesData.LinesHeight);
		for (int i = 0; i < ImageData.LinesData.LinesCount; i++)
		{
			Move(&Array[position], &ImageData.LinesData.Info[i], sizeof(dataline)); position += sizeof(dataline);
		}
		_FreeMemoryArray(ImageData.LinesData.Data);
		size_id = ImageData.LinesData.LinesWidth * ImageData.LinesData.LinesHeight;
		if (size_id > 0)
		{
			switch (ImageData.LinesData.DataType)
			{
			case 0: ImageData.LinesData.Data = (void*)new byte[size_id]; break;
			case 1: ImageData.LinesData.Data = (void*)new unsigned short[size_id]; break;
			case 2: ImageData.LinesData.Data = (void*)new float[size_id]; break;
			}
			Move(&Array[position], ImageData.LinesData.Data, (MassType[ImageData.LinesData.DataType] * size_id)); position += (MassType[ImageData.LinesData.DataType] * size_id);
		}
		Move(&Array[position], &ImageData.WindowsData.SizeWindowList, sizeof(ImageData.WindowsData.SizeWindowList)); position += sizeof(ImageData.WindowsData.SizeWindowList);
		Move(&Array[position], &ImageData.WindowsData.WindowCount, sizeof(ImageData.WindowsData.WindowCount)); position += sizeof(ImageData.WindowsData.WindowCount);
		Move(&Array[position], &ImageData.WindowsData.DataType, sizeof(ImageData.WindowsData.DataType)); position += sizeof(ImageData.WindowsData.DataType);
		Move(&Array[position], &ImageData.WindowsData.DegreeBinning, sizeof(ImageData.WindowsData.DegreeBinning)); position += sizeof(ImageData.WindowsData.DegreeBinning);
		Move(&Array[position], &ImageData.WindowsData.SizeData, sizeof(ImageData.WindowsData.SizeData)); position += sizeof(ImageData.WindowsData.SizeData);
		Move(&Array[position], &ImageData.WindowsData.BottomRight, sizeof(ImageData.WindowsData.BottomRight)); position += sizeof(ImageData.WindowsData.BottomRight);
		for (int i = 0; i < ImageData.WindowsData.SizeWindowList; i++)
		{
			Move(&Array[position], &ImageData.WindowsData.Info[i], sizeof(datawindow)); position += sizeof(datawindow);
		}
		_FreeMemoryArray(ImageData.WindowsData.Data);
		if (ImageData.WindowsData.SizeData > 0)
		{
			switch (ImageData.WindowsData.DataType)
			{
			case 0: ImageData.WindowsData.Data = (void*)new byte[ImageData.WindowsData.SizeData]; break;
			case 1: ImageData.WindowsData.Data = (void*)new unsigned short[ImageData.WindowsData.SizeData]; break;
			case 2: ImageData.WindowsData.Data = (void*)new float[ImageData.WindowsData.SizeData]; break;
			}
			Move(&Array[position], ImageData.WindowsData.Data, (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData)); position += (MassType[ImageData.WindowsData.DataType] * ImageData.WindowsData.SizeData);
		}
	}
	// ---------------------------------------------------------------------------
#pragma end_region
};

#endif
