#include <Controls.hpp>
#include <vcl.h>
#include <vector>
#include <algorithm>

struct FragmentData
{
	FragmentData():RawFragment(NULL),SizeX(0),SizeY(0), min(0),max(0),mean(0) { }
	~FragmentData() { delete [] RawFragment;};
	FragmentData(const FragmentData&);
	FragmentData& operator=(const FragmentData&);
	unsigned short* RawFragment;
	int SizeX;
	int SizeY;
	int min;
	int max;
	int mean;

};

	FragmentData::FragmentData(const FragmentData& CopyData)
	{
		SizeX = CopyData.SizeX;
		SizeY = CopyData.SizeY;
		min = CopyData.min;
		max = CopyData.max;
		mean = CopyData.mean;

		if(SizeX != 0 && SizeY != 0)
		{
			int FragmentSize = SizeX * SizeY;
			RawFragment = new unsigned short [FragmentSize];
			for (int i = 0; i < FragmentSize; i++)
			{
				RawFragment[i] = CopyData.RawFragment[i];
			}
		}
	}
	FragmentData& FragmentData::operator=(const FragmentData& CopyData)
	{
		if(this != & CopyData)
		{
			SizeX = CopyData.SizeX;
			SizeY = CopyData.SizeY;
			min = CopyData.min;
			max = CopyData.max;
			mean = CopyData.mean;

			if(SizeX != 0 && SizeY != 0)
			{
				delete [] RawFragment;
				int FragmentSize = SizeX * SizeY;
				RawFragment = new unsigned short [FragmentSize];
				for (int i = 0; i < FragmentSize; i++)
				{
					RawFragment[i] = CopyData.RawFragment[i];
				}
			}

		}
		return *this;

	}


// функция для изменения контраста изображения
// алгоритм смотри здесь: https://habrahabr.ru/post/139428/
void changeContrast(int ConstrastCoefficient,TImage* ImageToCorrect);
std::unique_ptr<TBitmap> changeContrast(int ContrastCoefficient, FragmentData& FData);
std::unique_ptr<TBitmap> createFragmentBitmap(FragmentData& FData);
void resizeBitmap(unsigned int Width, unsigned int Height, TBitmap* BitmapToScale);
void writePixelValue(FragmentData& FData,TBitmap* Bitmap, unsigned short PixelSize,unsigned short ToCenter, unsigned short FontSize);


