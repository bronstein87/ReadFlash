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
	bool IsBinary, IsReverse;   //������� �����������
	unsigned short CountPixFilter;       //����� �������� ���� ������
	unsigned short ImageWidth, ImageHeight;  //������ � ������ �����������
	unsigned short StatOrient;  //������ ������� ������ (0 - �������)
	unsigned short CountStars;  //����� ���������������� �����
	std::vector <StarsInfo>  StarsList;  //������ ���������������� �����
	unsigned short CountLocalObj, CountDeterObj;    //����� �������������� � ������������ ��������
	std::vector <ObjectsInfo> ObjectsList;    //������ �������������� ��������
	unsigned short CountWindows;  //����� �������������� ����
    unsigned short InfoCountWindows;  //����� ����� ����, ����� �� ������� �� ������ � ����
	std::vector <WindowsInfo> WindowsList; //������ �������������� ����
	unsigned short CountLines, CountBlock;  //����� ����������� ����� � ������ � �������
	std::vector <LinesInfo> LinesList;  //������ ������
	double QuatOrient[4], MatrixOrient[3][3], AnglesOrient[3];   //����������, ������� � ���� ����������
	double OmegaOrient[3], MatrixProg[3][3]; //������� ��������, �������������� ������� ����������
	double MatrixTemp;     //����������� ����-�������

};





#endif
