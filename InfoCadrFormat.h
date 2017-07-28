#ifndef InfoCadr
#define InfoCadr
#include <vector>

struct StarsInfo // ��������������
{
	float X, Y, Bright;
	unsigned long StarID;
	float Mv;
	char Sp[2];
};

struct ObjectsInfo  // ����������� �������
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
	bool IsBinary, IsReverse;   //������� �����������, ������� ������� �����������
	bool IsOrient; //������� ����������� ����������
	unsigned short CountPixFilter;       //����� �������� ���� ������
	unsigned short ImageWidth, ImageHeight;  //������ � ������ �����������
	unsigned short StatOrient;  //������ ������� ������ (0 - �������)
	double MeanBright, SigmaBright;

	unsigned short CountStars;  //����� ���������������� �����
	unsigned short CountWindows;  //����� �������������� ����
	unsigned short CountLocalObj, CountDeterObj;    //����� �������������� � ������������ ��������

	unsigned short SizeStarsList;
	std::vector <StarsInfo>  StarsList;  //������ ���������������� �����

	unsigned short SizeObjectsList;
	std::vector <ObjectsInfo> ObjectsList;    //������ �������������� ��������

	unsigned short SizeWindowsList;
	std::vector <WindowsInfo> WindowsList; //������ �������������� ����

	unsigned short CountLines, CountBlock;  //����� ����������� ����� � ������ � �������
	std::vector <LinesInfo> LinesList;  //������ ������

	double QuatOrient[4], MatrixOrient[3][3], AnglesOrient[3];   //����������, ������� � ���� ����������
	double OmegaOrient[3], MatrixProg[3][3]; //������� ��������, �������������� ������� ����������
	double MatrixTemp;     //����������� ����-�������
	double MeanErrorX, MeanErrorY, MeanErrorXY;
};





#endif
