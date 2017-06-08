	#include "ImageProcessingFcns.h"

  void changeContrast(int ConstrastCoefficient,TImage* ImageToCorrect)
  {
		ImageToCorrect->Picture->Bitmap->PixelFormat= pf24bit;
		int AverageBrightness = 0;
		int Width = ImageToCorrect->Picture->Bitmap->Width;
		int Height = ImageToCorrect->Picture->Bitmap->Height;
		TRGBTriple* BitmapLine;
		for(int i = 0;i < Height;i ++)
		{
			BitmapLine = (TRGBTriple*)ImageToCorrect->Picture->Bitmap->ScanLine[i];

			for(int j = 0;j < Width;j++)
			{
			   AverageBrightness +=
			   (BitmapLine[j].rgbtRed * 0.299 +
			   BitmapLine[j].rgbtGreen * 0.587 +
			   BitmapLine[j].rgbtBlue * 0.114);
			}
		}

		AverageBrightness /= Width*Height;
		double CorrectionCoefficient = 1.0 + ConstrastCoefficient / 100.0;

		std::vector<int> Palette;

		for (int i = 0; i < 256; i++)
		{
			int delta = i - AverageBrightness;  // отклонение от средней яркости
			int temp  = AverageBrightness + CorrectionCoefficient *delta;

			if (temp < 0)
			temp = 0;

			if (temp >= 255)
			temp = 255;

			Palette.push_back(temp);
		}

		for(int i=0;i<Height;i++)
		{
			BitmapLine = (TRGBTriple*)ImageToCorrect->Picture->Bitmap->ScanLine[i];
			for(int j=0;j<Width;j++)
			{
			   BitmapLine[j].rgbtBlue = Palette[BitmapLine[j].rgbtBlue]  ;
			   BitmapLine[j].rgbtGreen = Palette[BitmapLine[j].rgbtGreen] ;
			   BitmapLine[j].rgbtRed = Palette[BitmapLine[j].rgbtRed] ;
			}
		}

		ImageToCorrect->Repaint();


  }
