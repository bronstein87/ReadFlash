
#include "ReadFlashFormat.h"

double SumSquare(float *vect, int num)
{
	double res=0;
	for (int i = 0; i < num; i++) {
		res+=vect[i]*vect[i];
	}

	return res;
}

float GetFloat(float ptr)
{
unsigned short ss[2];
unsigned short *sh, buf_s;

  sh=(unsigned short *)&ptr;
  ss[0]=*sh;
  ss[1]=*(sh+1);
  buf_s=ss[0];
  ss[0]=ss[1];
  ss[1]=buf_s;
  return *(float*)ss;
}

int GetInt(int ptr)
{
unsigned short ss[2];
unsigned short *sh, buf_s;

  sh=(unsigned short *)&ptr;
  ss[0]=*sh;
  ss[1]=*(sh+1);
  buf_s=ss[0];
  ss[0]=ss[1];
  ss[1]=buf_s;
  return *(int*)ss;
}

void ChangeWordSingleReg(struct DataSingleReg &data)
{
	data.T = GetInt(data.T);
	data.Tbshv = GetInt(data.Tbshv);
	data.Tpr_sec = GetInt(data.Tpr_sec);
	data.Tpr_msec = GetInt(data.Tpr_msec);
}

void ChangeWordPix(struct DataPixHeader &data)
{
	data.T = GetInt(data.T);
	data.Tbshv = GetInt(data.Tbshv);
	data.Tpr_sec = GetInt(data.Tpr_sec);
	data.Tpr_msec = GetInt(data.Tpr_msec);
}

void ChangeWordFrag(struct DataFragHeader &data)
{
	data.Tpr_sec = GetInt(data.Tpr_sec);
	data.Tpr_msec = GetInt(data.Tpr_msec);
	data.NumPix = GetInt(data.NumPix);
}

void ChangeWordNO(struct DataNO &data)
{
	data.T = GetInt(data.T);
	data.Tbshv = GetInt(data.Tbshv);
	data.Tpr_sec = GetInt(data.Tpr_sec);
	data.Tpr_msec = GetInt(data.Tpr_msec);
	data.Temp=GetFloat(data.Temp);

	for (int i = 0; i < 4; i++)
		data.QuatBoart[i]=GetFloat(data.QuatBoart[i]);

	for (int k = 0; k < 2; k++)
		for (int i = 0; i < data.NumL[k]; i++)
			for (int j = 0; j < 4; j++)
				data.RsLocT[k][i][j]=GetFloat(data.RsLocT[k][i][j]);

	for (int i = 0; i < MaxDetHO; i++) {
		for (int j = 0; j < 4; j++) {
			data.RsLoc[i][j]=GetFloat(data.RsLoc[i][j]);
			data.Coord[i][j]=GetFloat(data.Coord[i][j]);
			data.D_cos[i][j]=GetFloat(data.D_cos[i][j]);
		}
		data.Dx[i]=GetFloat(data.Dx[i]);
		data.Dy[i]=GetFloat(data.Dy[i]);
	}

	data.m_cur=GetFloat(data.m_cur);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			data.M_ornt[i][j]=GetFloat(data.M_ornt[i][j]);
		}
	}
	for (int i = 0; i < 4; i++)
		data.Quat[i]=GetFloat(data.Quat[i]);

	data.TimeExp=GetFloat(data.TimeExp);
	for (int i = 0; i < 3; i++)
		data.Wop[i]=GetFloat(data.Wop[i]);
	data.EPOCH=GetFloat(data.EPOCH);
}

void ChangeWordSLEZH(struct DataSLEZH &data)
{
	data.T = GetInt(data.T);
	data.Tbshv = GetInt(data.Tbshv);
	data.Tpr_sec = GetInt(data.Tpr_sec);
	data.Tpr_msec = GetInt(data.Tpr_msec);
	data.Temp=GetFloat(data.Temp);

	data.mean=GetFloat(data.mean);
	data.sigma=GetFloat(data.sigma);

	for (int i = 0; i < MaxStarCad; i++)
		for (int j = 0; j < 2; j++)
			data.XYcProg[i][j]=GetFloat(data.XYcProg[i][j]);

		for (int i = 0; i < data.NumLoc  ; i++)
			for (int j = 0; j < 4; j++)
				data.RsLocT[i][j]=GetFloat(data.RsLocT[i][j]);

	for (int i = 0; i < MaxOrnt; i++) {
		for (int j = 0; j < 2; j++)
			data.MeanFrag[i][j]=GetFloat(data.MeanFrag[i][j]);
		for (int j = 0; j < 4; j++) {
			data.XYc[i][j]=GetFloat(data.XYc[i][j]);
			data.RsLoc[i][j]=GetFloat(data.RsLoc[i][j]);
			data.Coord[i][j]=GetFloat(data.Coord[i][j]);
			data.D_cos[i][j]=GetFloat(data.D_cos[i][j]);
		}
		data.Dx[i]=GetFloat(data.Dx[i]);
		data.Dy[i]=GetFloat(data.Dy[i]);
	}

	data.m_cur=GetFloat(data.m_cur);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			data.M_prog[i][j]=GetFloat(data.M_prog[i][j]);
			data.M_ornt[i][j]=GetFloat(data.M_ornt[i][j]);
		}
	}
	for (int i = 0; i < 4; i++)
		data.Quat[i]=GetFloat(data.Quat[i]);

	data.TimeExp=GetFloat(data.TimeExp);
	for (int i = 0; i < 3; i++)
		data.Wop[i]=GetFloat(data.Wop[i]);
	data.EPOCH=GetFloat(data.EPOCH);
}

void ConvertDataSLEZH(struct DataSLEZH data, struct CadrInfo &mCadr)
{
	mCadr.IsBinary=true;
//	mCadr.IsReverse=true;

	mCadr.ImageHeight=1024;
	mCadr.ImageWidth=1024;
	mCadr.MatrixTemp=data.Temp;
	mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;

	mCadr.CountLocalObj=data.NumLoc;
	mCadr.CountDeterObj=data.NumDet;
	mCadr.CountStars=data.NumProgFrag;
	mCadr.CountWindows=data.NumFrag;

	ObjectsInfo objInfo;
	mCadr.SizeObjectsList=mCadr.CountLocalObj;
	for (int i=0; i<mCadr.SizeObjectsList; i++) {
		objInfo.X=data.RsLocT[i][0];
		objInfo.Y=data.RsLocT[i][1];
		objInfo.Bright=data.RsLocT[i][2];
		objInfo.Square  =abs(data.RsLocT[i][3]);
		objInfo.StarID=0;
		objInfo.Mv=0;
		objInfo.Sp[0]='_';
		objInfo.Sp[1]='_';
		objInfo.Dx=0;
		objInfo.Dy=0;
		mCadr.ObjectsList.push_back(objInfo) ;
	}

	if ((!data.res_stat)&&(data.NumDet))
	{
		for (int i=0; i<mCadr.CountDeterObj; i++) {
			int iloc=data.MasRes[i][0];
			int ifrag=data.MasRes[i][1];
			mCadr.ObjectsList[iloc].StarID=(unsigned int)data.XYc[ifrag][2];
			mCadr.ObjectsList[iloc].Mv=data.XYc[ifrag][3];
			mCadr.ObjectsList[iloc].Dx=data.Dx[i];
			mCadr.ObjectsList[iloc].Dy=data.Dy[i];
		}

		double SumX, SumY;
		SumX=SumSquare(data.Dx, data.NumDet);
		SumY=SumSquare(data.Dy, data.NumDet);
		mCadr.MeanErrorX=sqrtm(SumX/(double)(data.NumDet-1));
		mCadr.MeanErrorY=sqrtm(SumY/(double)(data.NumDet-1));
		mCadr.MeanErrorXY=data.m_cur;//sqrtm((SumX+SumY)/(double)(2*data.NumDet-1));
	}

	mCadr.CountBlock=data.CountBlock;
	mCadr.CountLines=data.EndBufFrag>>mCadr.IsBinary;

	LinesInfo linesInfo;

	for (int i=0; i<mCadr.CountBlock; i++) {
		linesInfo.Start=data.TabTakeAway[i][0]>>mCadr.IsBinary;
		linesInfo.Height=data.TabTakeAway[i][1]>>mCadr.IsBinary;
		mCadr.LinesList.push_back(linesInfo);
	}

	StarsInfo starsInfo;
	mCadr.SizeStarsList=mCadr.CountStars;
	for (int i = 0; i < mCadr.SizeStarsList; i++) {
		starsInfo.X=data.XYcProg[i][0];
		starsInfo.Y=data.XYcProg[i][1];
		mCadr.StarsList.push_back(starsInfo);
//		mCadr.StarsList[i].Mv=data.XYcProg[i][2];
//		mCadr.StarsList[i].StarID=data.XYcProg[i][2];
	}

	WindowsInfo winInfo;
	mCadr.SizeWindowsList=mCadr.CountWindows;
	for (int i = 0; i < mCadr.SizeWindowsList; i++) {
		winInfo.Xstart = data.FragYX[i][1]-data.PfragXY0[i][0]+1;
		winInfo.Ystart = data.YendLoc[i]-data.PfragXY0[i][1]+1;
		winInfo.Width = data.PfragXY0[i][0];
		winInfo.Height = data.PfragXY0[i][1];
		winInfo.ZipX = data.ZipXY[i][0];
		winInfo.ZipY = data.ZipXY[i][1];
		winInfo.Mean = data.MeanFrag[i][0];
		winInfo.Sigma = data.MeanFrag[i][1];
		winInfo.Level = data.ThFrag[i];
		winInfo.CountObj = data.ObjFrag[i];
		winInfo.StarID = data.XYc[i][2];
		winInfo.Mv = data.XYc[i][3];
		mCadr.WindowsList.push_back(winInfo);
	}

	if (!data.res_stat) {
		mCadr.IsOrient=true;
		for (int i=0; i<3; i++) {
			for (int j=0; j<3; j++) {
				mCadr.MatrixOrient[i][j]=data.M_ornt[i][j];
			}
		}

		for (int i=0; i<4; i++) {
			mCadr.QuatOrient[i]=data.Quat[i];
		}

		for (int i=0; i<3; i++) {
			mCadr.OmegaOrient[i]=data.Wop[i];
		}
	}
	else mCadr.IsOrient=false;
}

void ConvertDataNO(struct DataNO data, struct CadrInfo &mCadr, int NC)
{
	mCadr.IsBinary=true;
//	mCadr.IsReverse=true;

	mCadr.ImageHeight=1024;
	mCadr.ImageWidth=1024;
	mCadr.MatrixTemp=data.Temp;
	mCadr.Time=data.Tpr_sec+data.Tpr_msec/1000.;
	mCadr.CountLocalObj=data.NumLoc[NC];
	mCadr.CountDeterObj=data.NumDet;

	ObjectsInfo objInfo;
	mCadr.ObjectsList.clear();      //!!!!!!!!!!!!!!!!
	mCadr.SizeObjectsList=mCadr.CountLocalObj;
	for (int i=0; i<mCadr.SizeObjectsList; i++) {
		objInfo.X=data.RsLocT[NC][i][0];
		objInfo.Y=data.RsLocT[NC][i][1];
		objInfo.Bright=data.RsLocT[NC][i][2];
		objInfo.Square  = abs(data.RsLocT[NC][i][3]);
		objInfo.StarID=0;
		objInfo.Mv=0;
		objInfo.Sp[0]='_';
		objInfo.Sp[1]='_';
		objInfo.Dx=0;
		objInfo.Dy=0;
		mCadr.ObjectsList.push_back(objInfo);
	}

	if (NC) {
		if ((!data.res_stat)&&(data.NumDet))
		{
			for (int i=0; i<data.NumDet; i++) {
				int iloc=data.MasRes[i][0];
		//			mCadr.ObjectsList[iloc].StarID=(unsigned int)data.XYc[ifrag][2];
		//			mCadr.ObjectsList[iloc].Mv=data.XYc[ifrag][3];
					mCadr.ObjectsList[iloc].Dx=data.Dx[i];
					mCadr.ObjectsList[iloc].Dy=data.Dy[i];
			}

			double SumX, SumY;
			SumX=SumSquare(data.Dx, data.NumDet);
			SumY=SumSquare(data.Dy, data.NumDet);
			mCadr.MeanErrorX=sqrtm(SumX/(double)(data.NumDet-1));
			mCadr.MeanErrorY=sqrtm(SumY/(double)(data.NumDet-1));
			mCadr.MeanErrorXY=data.m_cur;//sqrtm((SumX+SumY)/(double)(2*data.NumDet-1));
		}
	}

	if ((NC)&&(!data.res_stat)) {
		mCadr.IsOrient=true;
		for (int i=0; i<3; i++) {
			for (int j=0; j<3; j++) {
				mCadr.MatrixOrient[i][j]=data.M_ornt[i][j];
			}
		}

		for (int i=0; i<4; i++) {
			mCadr.QuatOrient[i]=data.Quat[i];
		}

		for (int i=0; i<3; i++) {
			mCadr.OmegaOrient[i]=data.Wop[i];
		}
	}
	else mCadr.IsOrient=false;

	mCadr.CountStars = 0;
	mCadr.SizeStarsList = 0;
	mCadr.CountWindows = 0;
	mCadr.SizeWindowsList = 0;
	mCadr.CountLines = 0;
	mCadr.CountBlock = 0;
}

void PrintReg(FILE *ftxt, unsigned char *mas)
{
  fprintf(ftxt,"\n\n//---------------Регистры-----------------------//\n");
  fprintf(ftxt,"Регистры:\n");
  for (int i=0; i<COUNT_REG; i++)
  {
	fprintf(ftxt,"%d 0x%02x\n",i+1,mas[i]);
  }
  fprintf(ftxt,"//----------------------------------------------//\n");
}

void PrintSingleReg(FILE *ftxt, struct DataSingleReg data)
{
  fprintf(ftxt,"\n\n//---------------Ошибка чтения регистра-----------------------//\n");
  fprintf(ftxt,"Полукомплект №:  %02d\n",(data.SerNum&0x8000+1)>>15);
  fprintf(ftxt,"Заводской номер: %02d\n",data.SerNum&0x7FFF);
  fprintf(ftxt,"Запись регистров № %d\n",data.CntRecord);
  fprintf(ftxt,"Время привязки: Tep=%ld, Tbshv=%ld\n",data.T,data.Tbshv);
  fprintf(ftxt,"Время привязки: Ts=%ld, Tms=%ld\n",data.Tpr_sec,data.Tpr_msec);
  fprintf(ftxt,"Количество сбоев регистров: %d\n",data.CntErr);
  fprintf(ftxt,"Номер регистра: %d\n",data.Nreg);
  fprintf(ftxt,"Значение регистра: %d\n",data.RegVal);
  fprintf(ftxt,"//----------------------------------------------//\n");
}

void PrintDataPix(FILE *ftxt, struct DataPixHeader data, int NumCadr)
{
fprintf(ftxt,"\n\n//----Пиксели выше порога на %d-м кадре---------//\n",NumCadr);
if (NumCadr==1) {
  fprintf(ftxt,"Полукомплект №:  %02d\n",(data.SerNum&0x8000+1)>>15);
  fprintf(ftxt,"Заводской номер: %02d\n",data.SerNum&0x7FFF);
  fprintf(ftxt,"Запись пикселей № %d\n",data.CntRecord);
}
fprintf(ftxt,"Время привязки: Tep=%ld, Tbshv=%ld\n",data.T,data.Tbshv);
fprintf(ftxt,"Время привязки: Ts=%ld, Tms=%ld\n",data.Tpr_sec,data.Tpr_msec);
fprintf(ftxt,"Время экспозиции: %d\n",data.StarExp);
fprintf(ftxt,"Число пикселей: %d\n",data.NumPix);
fprintf(ftxt,"//----------------------------------------------//\n");
}

void PrintDataFrag(FILE *ftxt, struct DataFragHeader data)
{
fprintf(ftxt,"\n\n//-----------Массив фрагментов--------------//\n");
fprintf(ftxt,"Полукомплект №:  %02d\n",(data.SerNum&0x8000+1)>>15);
fprintf(ftxt,"Заводской номер: %02d\n",data.SerNum&0x7FFF);
fprintf(ftxt,"Запись фрагментов № %d\n",data.CntRecord);
fprintf(ftxt,"Время привязки: Ts=%ld, Tms=%ld\n",data.Tpr_sec,data.Tpr_msec);
fprintf(ftxt,"Число пикселей: %ld\n",data.NumPix);
fprintf(ftxt,"//----------------------------------------------//\n");
}

void PrintDataNO(FILE *ftxt, struct DataNO data)
{
fprintf(ftxt,"\n\n//---------------Режим НО/ТО--------------------//\n");
fprintf(ftxt,"Полукомплект №:  %02d\n",(data.SerNum&0x8000+1)>>15);
fprintf(ftxt,"Заводской номер: %02d\n",data.SerNum&(0x7FFF));
fprintf(ftxt,"Запись в режиме НО/ТО № %d\n",data.CntRecord);
fprintf(ftxt,"Время привязки: Tep=%ld, Tbshv=%ld\n",data.T,data.Tbshv);
fprintf(ftxt,"Время привязки: Ts=%ld, Tms=%ld\n",data.Tpr_sec,data.Tpr_msec);
fprintf(ftxt,"Температура: %f\n",data.Temp);
fprintf(ftxt,"Время экспозиции: %d\n",data.StarExp);
fprintf(ftxt,"Статус решения: 0x%04x\n",data.res_stat);
fprintf(ftxt,"Априорная информация: %d\n",data.NumApr);
fprintf(ftxt,"Количество совпадающих объектов: %d\n",data.NumStore);

fprintf(ftxt,"Априорный кватернион: %f; %f; %f; %f;\n",
		data.QuatBoart[0],data.QuatBoart[1],
		data.QuatBoart[2],data.QuatBoart[3]);
fprintf(ftxt,"Количество зафиксированных объектов: %d/%d\n",data.NumL[0],data.NumL[1]);

fprintf(ftxt,"RsLocT[0]\n");
for (int i=0; i<data.NumL[0];i++)
{
  fprintf(ftxt,"%4d %10.4f %10.4f %10.2f %8.0f\n",i+1,
		  data.RsLocT[0][i][0],data.RsLocT[0][i][1],
		  data.RsLocT[0][i][2],data.RsLocT[0][i][3]);
}
fprintf(ftxt,"RsLocT[1]\n");
for (int i=0; i<data.NumL[1];i++)
{
  fprintf(ftxt,"%4d %10.4f %10.4f %10.2f %8.0f\n",i+1,
		  data.RsLocT[1][i][0],data.RsLocT[1][i][1],
		  data.RsLocT[1][i][2],data.RsLocT[1][i][3]);
}
fprintf(ftxt,"Общее число объектов: %d/%d\n",data.NumLoc[0],data.NumLoc[1]);

fprintf(ftxt,"Координаты выбранных объектов:\n");
fprintf(ftxt,"RsLoc\n");
for (int i=0; i<MaxDetHO;i++)
{
  fprintf(ftxt,"%4d %10.4f %10.4f %10.2f %8.0f\n",i+1,
		  data.RsLoc[i][0],data.RsLoc[i][1],
		  data.RsLoc[i][2],data.RsLoc[i][3]);
}
fprintf(ftxt,"Количество распознанных объектов: NumDet=%d\n",data.NumDet);
fprintf(ftxt,"Максимальное смещение по осям гистограммы: %d/%d\n",data.dxmaxH,data.dymaxH);
fprintf(ftxt,"Максимум гистораммы: %d\n",data.maxH);

fprintf(ftxt,"MasRes\n");
for (int i=0; i<MaxDetHO;i++)
{
  fprintf(ftxt,"%4d %4d %4d\n",i+1, data.MasRes[i][0], data.MasRes[i][1]);  //НЕ меняем местами
}

fprintf(ftxt,"Coord\n");
for (int i=0; i<MaxDetHO;i++)
{
  fprintf(ftxt,"%4d %12.8f %12.8f %12.8f %10.2f\n",i+1,
		  data.Coord[i][0],data.Coord[i][1],
		  data.Coord[i][2],data.Coord[i][3]);
}

fprintf(ftxt,"D_cos\n");
for (int i=0; i<MaxDetHO;i++)
{
  fprintf(ftxt,"%4d %12.8f %12.8f %12.8f %8.0f\n",i+1,
		  data.D_cos[i][0],data.D_cos[i][1],
		  data.D_cos[i][2],data.D_cos[i][3]);
}

fprintf(ftxt,"Dx&Dy\n");
for (int i=0; i<MaxDetHO;i++)
{
  fprintf(ftxt,"%4d %10.6f %10.6f\n",i+1, data.Dx[i],data.Dy[i]);
}

fprintf(ftxt,"Ошибка единицы веса: %f\n",data.m_cur);
fprintf(ftxt,"Матрица ориентации:\n");
fprintf(ftxt,"%16.8f %16.8f %16.8f\n",data.M_ornt[0][0],data.M_ornt[0][1],data.M_ornt[0][2]);
fprintf(ftxt,"%16.8f %16.8f %16.8f\n",data.M_ornt[1][0],data.M_ornt[1][1],data.M_ornt[1][2]);
fprintf(ftxt,"%16.8f %16.8f %16.8f\n",data.M_ornt[2][0],data.M_ornt[2][1],data.M_ornt[2][2]);

fprintf(ftxt,"Кватернион ориентации:%f %f %f %f\n",
		data.Quat[0],data.Quat[1],
		data.Quat[2],data.Quat[3]);

fprintf(ftxt,"Время между кадрами: %f\n",data.TimeExp);
fprintf(ftxt,"Угловая скорость, угл.с./с: %f %f %f\n",
		data.Wop[0]*RTS,data.Wop[1]*RTS,data.Wop[2]*RTS);
fprintf(ftxt,"Эпоха наблюдения: %12.8f\n",data.EPOCH);

fprintf(ftxt,"Линейные скорости, км/с:\n");
fprintf(ftxt,"%f %f %f\n",GetFloat(data.Vline[0]),GetFloat(data.Vline[1]),
						  GetFloat(data.Vline[2]));
fprintf(ftxt,"//----------------------------------------------//\n");
}

void PrintDataSLEZH(FILE *ftxt,  struct DataSLEZH data)
{
fprintf(ftxt,"\n\n//---------------Режим СЛЕЖЕНИЕ--------------------//\n");
fprintf(ftxt,"Полукомплект №:  %02d\n",(data.SerNum&0x8000+1)>>15);
fprintf(ftxt,"Заводской номер: %02d\n",data.SerNum&0x7FFF);
fprintf(ftxt,"Запись в режиме слежения № %d\n",data.CntRecord);
fprintf(ftxt,"Время привязки: Tep=%ld, Tbshv=%ld\n",data.T,data.Tbshv);
fprintf(ftxt,"Время привязки: Ts=%ld, Tms=%ld\n",data.Tpr_sec,data.Tpr_msec);
fprintf(ftxt,"Температура: %f\n",data.Temp);
fprintf(ftxt,"Время экспозиции: %d\n",data.StarExp);
fprintf(ftxt,"Статус решения: 0x%04x\n",data.res_stat);
fprintf(ftxt,"Статистика по кадру: mean=%f, sigma=%f\n",data.mean, data.sigma);

fprintf(ftxt,"Прогнозируемая матрица ориентации: \n");
fprintf(ftxt,"%16.8f %16.8f %16.8f\n",data.M_prog[0][0], data.M_prog[0][1], data.M_prog[0][2]);
fprintf(ftxt,"%16.8f %16.8f %16.8f\n",data.M_prog[1][0], data.M_prog[1][1], data.M_prog[1][2]);
fprintf(ftxt,"%16.8f %16.8f %16.8f\n",data.M_prog[2][0], data.M_prog[2][1], data.M_prog[2][2]);

fprintf(ftxt,"Ближайший сектор каталога: %d\n",data.NumSec);
fprintf(ftxt,"Число прогнозируемых звезд: %d\n",data.NumProgFrag);

//XYcProg
fprintf(ftxt,"XYcProg\n");
for (int i=0; i<MaxStarCad;i++)
{
	fprintf(ftxt,"%4d %10.4f %10.4f\n",i+1,
			data.XYcProg[i][0], data.XYcProg[i][1]);
}
fprintf(ftxt,"Число фрагментов на кадре: %d\n",data.NumFrag);
fprintf(ftxt,"XYc\n");
for (int i=0; i<MaxOrnt;i++)
{
	fprintf(ftxt,"%4d %10.4f %10.4f %8.0f %8.2f\n",i+1,
		  data.XYc[i][0],data.XYc[i][1],
		  data.XYc[i][2],data.XYc[i][3]);
}

fprintf(ftxt,"MeanFrag&ThFrag\n");
for (int i=0; i<MaxOrnt;i++)
{
	fprintf(ftxt,"%4d %10.4f %10.4f %6d\n",i+1, data.MeanFrag[i][0],
			data.MeanFrag[i][1], data.ThFrag[i]);
}

fprintf(ftxt,"Максимальный порог на исходных фрагментах: %d\n",data.ThMax);

fprintf(ftxt,"Количество блоков в буфере: %d\n",data.CountBlock);
fprintf(ftxt,"Количество строк в буфере: %d\n",data.EndBufFrag);

fprintf(ftxt,"TabTakeAway\n");
for (int i=0; i<8;i++)
{
 fprintf(ftxt,"%4d %6d %6d\n",i+1, data.TabTakeAway[i][0],data.TabTakeAway[i][1]); //НЕ меняем местами
}
fprintf(ftxt,"PfragXY0\n");
for (int i=0; i<MaxOrnt;i++)
{
 fprintf(ftxt,"%4d %6d %6d\n",i+1, data.PfragXY0[i][0],data.PfragXY0[i][1]);  //НЕ меняем местами
}

fprintf(ftxt,"PfragXY\n");
for (int i=0; i<MaxOrnt;i++)
{
 fprintf(ftxt,"%4d %6d %6d\n",i+1, data.PfragXY[i][0],data.PfragXY[i][1]);  //НЕ меняем местами
}

fprintf(ftxt,"ZipXY\n");
for (int i=0; i<MaxOrnt;i++)
{
 fprintf(ftxt,"%4d %6d %6d\n",i+1, data.ZipXY[i][0],data.ZipXY[i][1]);   //НЕ меняем местами
}

fprintf(ftxt,"FragYX\n");
for (int i=0; i<MaxOrnt;i++)
{
 fprintf(ftxt,"%4d %6d %6d\n",i+1, data.FragYX[i][0],data.FragYX[i][1]);  //НЕ меняем местами
}

fprintf(ftxt,"YendLoc\n");
for (int i=0; i<MaxOrnt;i++)
{
 fprintf(ftxt,"%4d %6d\n",i+1, data.YendLoc[i]);
}

fprintf(ftxt,"ObjFrag\n");
for (int i=0; i<MaxOrnt;i++)
{
 fprintf(ftxt,"%4d %6d\n",i+1, data.ObjFrag[i]);
}

fprintf(ftxt,"Количество локализованных объектов: %d\n",data.NumLoc);
fprintf(ftxt,"Количество объектов в обработке: %d\n",data.NumStore);
fprintf(ftxt,"Количество распознанных объектов: %d\n",data.NumDet);

fprintf(ftxt,"RsLocT\n");
for (int i=0; i<data.NumLoc;i++)
{
  fprintf(ftxt,"%4d %10.4f %10.4f %10.2f %8.0f\n",i+1,
		  data.RsLocT[i][0],data.RsLocT[i][1],
		  data.RsLocT[i][2],data.RsLocT[i][3]);
}

fprintf(ftxt,"Координаты выбранных объектов:\n");
fprintf(ftxt,"RsLoc\n");
for (int i=0; i<MaxOrnt;i++)
{
  fprintf(ftxt,"%4d %10.4f %10.4f %10.2f %8.0f\n",i+1,
		  data.RsLoc[i][0],data.RsLoc[i][1],
		  data.RsLoc[i][2],data.RsLoc[i][3]);
}

fprintf(ftxt,"Максимальное смещение по осям гистограммы: %d/%d\n",data.dxmaxH,data.dymaxH);
fprintf(ftxt,"Максимум гистораммы: %d\n",data.maxH);

fprintf(ftxt,"MasRes\n");
for (int i=0; i<MaxOrnt;i++)
{
  fprintf(ftxt,"%4d %4d %4d\n",i+1, data.MasRes[i][1], data.MasRes[i][0]);  //меняем местами
}

fprintf(ftxt,"Coord\n");
for (int i=0; i<MaxOrnt;i++)
{
  fprintf(ftxt,"%4d %12.8f %12.8f %12.8f %10.2f\n",i+1,
		  data.Coord[i][0],data.Coord[i][1],
		  data.Coord[i][2],data.Coord[i][3]);
}

fprintf(ftxt,"D_cos\n");
for (int i=0; i<MaxOrnt;i++)
{
  fprintf(ftxt,"%4d %12.8f %12.8f %12.8f %8.0f\n",i+1,
		  data.D_cos[i][0],data.D_cos[i][1],
		  data.D_cos[i][2],data.D_cos[i][3]);
}

fprintf(ftxt,"Dx&Dy\n");
for (int i=0; i<MaxOrnt;i++)
{
  fprintf(ftxt,"%4d %10.6f %10.6f\n",i+1, data.Dx[i],data.Dy[i]);
}

fprintf(ftxt,"Ошибка единицы веса: %f\n",data.m_cur);
fprintf(ftxt,"Матрица ориентации:\n");
fprintf(ftxt,"%16.8f %16.8f %16.8f\n",data.M_ornt[0][0],data.M_ornt[0][1],data.M_ornt[0][2]);
fprintf(ftxt,"%16.8f %16.8f %16.8f\n",data.M_ornt[1][0],data.M_ornt[1][1],data.M_ornt[1][2]);
fprintf(ftxt,"%16.8f %16.8f %16.8f\n",data.M_ornt[2][0],data.M_ornt[2][1],data.M_ornt[2][2]);

fprintf(ftxt,"Кватернион ориентации:%f %f %f %f\n",
		data.Quat[0],data.Quat[1],
		data.Quat[2],data.Quat[3]);

fprintf(ftxt,"Время между кадрами: %f\n",data.TimeExp);
fprintf(ftxt,"Угловая скорость, угл.с./с: %f %f %f\n",
		data.Wop[0]*RTS,data.Wop[1]*RTS,data.Wop[2]*RTS);
fprintf(ftxt,"Эпоха наблюдения: %12.8f\n",data.EPOCH);

fprintf(ftxt,"Линейные скорости, км/с:\n");
fprintf(ftxt,"%f %f %f\n",GetFloat(data.Vline[0]),GetFloat(data.Vline[1]),
						  GetFloat(data.Vline[2]));
fprintf(ftxt,"//----------------------------------------------//\n");
}


