	#include "ImageProcessingFcns.h"

  void changeContrast(int ConstrastCoefficient, TImage* ImageToCorrect)
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

		AverageBrightness /= Width * Height;
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

		for(int i = 0;i < Height;i ++)
		{
			BitmapLine = (TRGBTriple*)ImageToCorrect->Picture->Bitmap->ScanLine[i];
			for(int j = 0;j < Width;j++)
			{
			   BitmapLine[j].rgbtBlue = Palette[BitmapLine[j].rgbtBlue]  ;
			   BitmapLine[j].rgbtGreen = Palette[BitmapLine[j].rgbtGreen] ;
			   BitmapLine[j].rgbtRed = Palette[BitmapLine[j].rgbtRed] ;
			}
		}

		ImageToCorrect->Repaint();


  }

std::unique_ptr <TBitmap> changeContrast(int ContrastCoefficient, FragmentData& FData)
  {
	int FragmentSize = FData.SizeX * FData.SizeY;
	if(FData.min == 0 && FData.max == 0 && FData.mean == 0)
	{
		FData.min = *std::min_element(FData.RawFragment, FData.RawFragment + FragmentSize - 1);
		FData.max = *std::max_element(FData.RawFragment, FData.RawFragment + FragmentSize - 1);

		if (FData.min == FData.max) throw std::logic_error("Минимальная яркость равна максимальной");

		int sum = 0;
		for (int i = 0; i < FragmentSize; i++)
		{
				sum += FData.RawFragment[i];
		}
		FData.mean = sum / FragmentSize;
	}

	std::unique_ptr <TBitmap> Fragment (new TBitmap());
	Fragment->PixelFormat = pf24bit;
	Fragment->Width =  FData.SizeX;
	Fragment->Height = FData.SizeY;


	TRGBTriple *BitmapLine; // структура, хранящая RBG
	for (unsigned int currentColumn = 0; currentColumn < FData.SizeY; currentColumn++)
	{
		// указатель на currentColumn строку Bitmap
		BitmapLine = (TRGBTriple*) Fragment->ScanLine[currentColumn];
		for (unsigned int currentRow = 0, adress = currentColumn * FData.SizeX; currentRow < FData.SizeX; currentRow++, adress += currentRow)
		{
			int ContrastValue =
			(((FData.RawFragment[adress] - FData.mean) * ContrastCoefficient + FData.mean - FData.min) * 256) / (FData.max - FData.min);
			if (ContrastValue < 0) ContrastValue = 0;
			if (ContrastValue > 255) ContrastValue = 255;

			BitmapLine[currentRow].rgbtBlue = ContrastValue;
			BitmapLine[currentRow].rgbtGreen = ContrastValue;
			BitmapLine[currentRow].rgbtRed = ContrastValue;
		}
	}

	return Fragment;
  }

 std::unique_ptr<TBitmap> createFragmentBitmap(FragmentData& FData)
  {
	  return changeContrast(1, FData);
  }


  void resizeBitmap(unsigned int Width, unsigned int Height, TBitmap* BitmapToScale)
  {
		std::unique_ptr <TBitmap> TemporaryBitmap(new TBitmap());
		TemporaryBitmap->Width = Width;
		TemporaryBitmap->Height = Height;
		TRect TheRect =  Rect(0 , 0 , Width, Height);
		TemporaryBitmap->Canvas->StretchDraw(TheRect, BitmapToScale);
		BitmapToScale->Assign(TemporaryBitmap.get());
  }

  void writePixelValue(FragmentData& FData,TBitmap* Bitmap, unsigned short PixelSize, unsigned short ToCenter, unsigned short FontSize)
  {

	for (unsigned int currentColumn = 0, PixelY = 0; currentColumn < FData.SizeY; currentColumn++, PixelY += PixelSize)
	{
		unsigned int PixelX = 0;
		for (unsigned int currentRow = 0, adress = currentColumn * FData.SizeX; currentRow < FData.SizeX; currentRow++, adress += currentRow)
		{
			int PixelValue =
			((FData.RawFragment[adress] - FData.min) * 256) / (FData.max - FData.min);
			if (PixelValue < 0) PixelValue = 0;
			if (PixelValue > 255) PixelValue = 255;

			Bitmap->Canvas->Font->Size = FontSize;
			Bitmap->Canvas->Brush->Color = (TColor)RGB(PixelValue, PixelValue, PixelValue); //PixelValue;
			if(PixelValue < 128) Bitmap->Canvas->Font->Color = clWhite;
			else  Bitmap->Canvas->Font->Color = clBlack;


			TRect TheRect = Rect(PixelX, PixelY, PixelX + PixelSize, PixelY + PixelSize);

			Bitmap->Canvas->TextRect(TheRect, PixelX + ToCenter, PixelY + ToCenter, IntToStr(FData.RawFragment[adress]));
			PixelX += PixelSize;

		}
	}


  }



