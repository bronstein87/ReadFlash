#include "StatLib.h"
#include "math.h"

#define sqr(x) (x)*(x)
double Maximum (double *massive1, double *massive2, int N)
{
	double max;
	max=massive1[0];
	for (int i=0; i<N; i++)
	{
		if (massive1[i]>max)
			max=massive1[i];
	}
	for (int i=0; i<N; i++)
	{
		if (massive2[i]>max)
			max=massive2[i];
	}
	return max;
}

double Minimum (double *massive1, double *massive2, int N)
{
	double min;
	min=massive1[0];
	for (int i=0; i<N; i++)
	{
		if (massive1[i]<min)
			min=massive1[i];
	}
	for (int i=0; i<N; i++)
	{
		if (massive2[i]<min)
			min=massive2[i];
	}
	return min;
}

double Maximum (double *massive1, int N)
{
	double max;
	max=massive1[0];
	for (int i=0; i<N; i++)
	{
		if (massive1[i]>max)
			max=massive1[i];
	}
	return max;
}

double Minimum (double *massive1, int N)
{
	double min;
	min=massive1[0];
	for (int i=0; i<N; i++)
	{
		if (massive1[i]<min)
			min=massive1[i];
	}

	return min;
}

double noise(double sigma)
{
	int j; 
	double r[12];double n=0; 
		
	for (j=0;j<12;j++)
		{
			r[j]=-0.5 + double(rand())/32767;
			n=n+r[j];
		}
//		fprintf(f1, "%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f\n", r[0], r[1], r[2], r[3], r[4], r[5],n);
		n=n*sigma;
		return n;
}

void LineInterpolation (double *t, double *t_in, double *x, double *x_in, int N, int N_in)
{
	FILE* 	f;
	f = fopen( "f_in1.dat", "w" );

	int i,j;

	j=0;
	for (i=0; i<N; i++)
	{
		
		while ((t_in[j]<t[i+1])&&(i<N)&&(j<N_in))
		{
			
			x_in[j]= x[i]+(x[i+1]-x[i])*(t_in[j]-t[i])/(t[i+1]-t[i]);

			fprintf(f, "t[i]=    %f  x[i]=    %f\n", t[i], x[i]);
			fprintf(f, "t_in[j]= %f  x_in[j]= %f\n\n", t_in[j], x_in[j]);
			j++;
		}
	}	
	fclose(f);

}
	
void SquareInterpolation (double *t, double *t_in, double *x, double *x_in, int N, int N_in)
{
	FILE* 	f;
	f = fopen( "f_in2.dat", "w" );

	int i,j;

	j=0;
	for (i=1; i<N; i=i+2)
	{
		if (i==N-1)
			{
				i=i-1;
			}

		while ((t_in[j]<=t[i+1])&&(j<N_in))
		{
			
			x_in[j]= (t_in[j]-t[i])*(t_in[j]-t[i+1])/((t[i-1]-t[i])*(t[i-1]-t[i+1]))*x[i-1]+
				(t_in[j]-t[i-1])*(t_in[j]-t[i+1])/((t[i]-t[i-1])*(t[i]-t[i+1]))*x[i]+
				(t_in[j]-t[i-1])*(t_in[j]-t[i])/((t[i+1]-t[i-1])*(t[i+1]-t[i]))*x[i+1];

			if (t_in[j]>=t[i])
			{
				fprintf(f, "t[i]=  %f  x[i]=  %f\n", t[i], x[i]);
			}
			else
			{
				fprintf(f, "t[i]=  %f  x[i]=  %f\n", t[i-1], x[i-1]);
			}
			fprintf(f, "t_in[j]= %f  x_in[j]= %f\n\n", t_in[j], x_in[j]);
			j++;
		}
	}	
	fclose(f);

}

double IntError(double *x, double *y, int N)
{
	FILE* 	f;
	f = fopen( "interp.dat", "w" );
	
	 double sum=0;
	 int i;

	 for (i=0;i<N;i++)
	 {
		 sum=sum+pow((x[i]-y[i]),2);
		 fprintf (f, "x=%f y=%f dx=%f\n",x[i],y[i],pow((x[i]-y[i]),2));
	 }
	 fclose(f);
	 return sum;
}

/**
int FindIntervals(vector<double>& x, vector<double>& y, vector <int>::iterator pN0, vector <int>::iterator pNk, int* Nint) 
{
	vector<double>::iterator p;
	vector <int> N0; vector <int> Nk;
	int i;
	const double proc=20;
	bool IsCorB=false;
	bool IsCorT=false;
	double min, max,extr;//,tconst,tvar;

	FILE* ff;
	ff=fopen("extr.txt","wt");

	

	min=Minimum(&y[0], y.size());
	max=Maximum(&y[0], y.size());
	extr=max-min;

	fprintf(ff,"%lf %lf %lf \n", min, max, extr);
	for (p=y.begin(),i=0; p!=y.end();p++,i++)
	{
		
		if (*p>(max-extr*proc/100))
		{
			if (IsCorT==false) 
			{
				IsCorT=true;
				if (N0.size()!=0) Nk.push_back(i-1);
				
			}
//			else
//			{
//				tconst=tconst+x[i]-x[i-1];
//			}
		}
		else
		{
			if (IsCorT==true)
			{
				IsCorT=false;
				N0.push_back(i);
			}
			else
			{
				//����� ����� ������� ����������
				if (N0.size()==0) N0.push_back(i);
			}
		}

		if (*p<(min+extr*proc/100))
		{
			if (IsCorB==false) 
			{
				IsCorB=true;
				if (N0.size()!=0) Nk.push_back(i-1);
				
			}
//			else
//			{
//				tconst=tconst+x[i]-x[i-1];
//			}
		}
		else 
		{
			if (IsCorB==true)
			{
				IsCorB=false;
				N0.push_back(i);
			}
			else
			{
				//����� ����� ������� ����������
				if (N0.size()==0) N0.push_back(i);
			}
		}		
	}	

	for (i=0;i<Nk.size();i++)
		fprintf(ff,"%ld %ld \n ", N0[i], Nk[i]);

	fclose(ff);

	pN0=&N0[0];
	pNk=&Nk[0];
	*(Nint)=Nk.size();
	
//	N0.clear();
//	Nk.clear();
return 0;
}
/**/
int MNK1pow (double *x, double *y, int n0, int nk, double *a,double *b)
{
	double x1=0,x2=0,y1=0,y1x1=0,aa,bb;
	int n=nk-n0+1,i;

	for (i=n0; i<=nk;i++)
	{
		x1=x1+x[i];
		x2=x2+pow((x[i]),2);
				
		y1=y1+y[i];
		y1x1=y1x1+(y[i])*(x[i]);
	}
	bb=(+y1x1-x1*y1/n)/(-x1*x1/n+x2);
	aa=y1/n-bb*x1/n;
	*(a)= aa;
	*(b)= bb;
	
	if (fabs(-x1*x1/n+x2)>0.001) return 0;
	else return (-1);
}

int MNK2pow (double *x, double *y, int n0, int nk, double *a,double *b,double *c)
{
	double x1=0,x2=0,x3=0,x4=0,y1=0,y1x1=0,y1x2=0, det, da, db, dc;
//	vector<double>::iterator px;
//	vector<double>::iterator py;
	int n=nk-n0+1,i;

	for (i=n0; i<=nk;i++)
	{
		x1=x1+x[i]-x[n0];
		x2=x2+pow((x[i]-x[n0]),2);
		x3=x3+pow((x[i]-x[n0]),3);
		x4=x4+pow((x[i]-x[n0]),4);
		
		y1=y1+y[i];
		y1x1=y1x1+(y[i])*(x[i]-x[n0]);
		y1x2=y1x2+(y[i])*pow((x[i]-x[n0]),2);
/*
		x1=x1+*(x);
		x2=x2+pow(*(x),2);
		x3=x3+pow(*(x),3);
		x4=x4+pow(*(x),4);
		
		y1=y1+y[i];
		y1x1=y1x1+(*y)*(*(x));
		y1x2=y1x2+(*y)*pow(*(x),2);
*/
	}

	det=double(n)*x2*x4+2*x1*x2*x3-x2*x2*x2-double(n)*x3*x3-x1*x1*x4;

	da=y1*x2*x4+y1x1*x3*x2+x1*y1x2*x3-y1x2*x2*x2-y1*x3*x3-y1x1*x1*x4;

	db=double(n)*y1x1*x4+y1x2*x1*x2+y1*x2*x3-y1x1*x2*x2-double(n)*y1x2*x3-y1*x1*x4;

	dc=double(n)*x2*y1x2+y1*x1*x3+y1x1*x1*x2-y1*x2*x2-double(n)*y1x1*x3-x1*x1*y1x2;

	*(a)= da/det;
	*(b)= db/det;
	*(c)= dc/det;

	if (fabs(det)>0.001) return 0;
	else return (-1);
}

int MNK2 (double *x, double *y, int n, double *a,double *b,double *c)
{
	double xh,x1=0,x2=0,x3=0,x4=0,y1=0,y1x1=0,y1x2=0, det, da, db, dc;

	for (int i=0; i<n;i++)
	{
		xh=x[i];
		x1+=xh;
		xh*=x[i];
		x2+=xh;
		xh*=x[i];
		x3+=xh;
		xh*=x[i];
		x4+=xh;
		
		xh=y[i];
		y1+=xh;
		xh*=x[i];
		y1x1+=xh;
		xh*=x[i];
		y1x2+=xh;
/*
		x1=x1+*(x);
		x2=x2+pow(*(x),2);
		x3=x3+pow(*(x),3);
		x4=x4+pow(*(x),4);
		
		y1=y1+y[i];
		y1x1=y1x1+(*y)*(*(x));
		y1x2=y1x2+(*y)*pow(*(x),2);
*/
	}

	det=double(n)*x2*x4+2*x1*x2*x3-x2*x2*x2-double(n)*x3*x3-x1*x1*x4;

	da=y1*x2*x4+y1x1*x3*x2+x1*y1x2*x3-y1x2*x2*x2-y1*x3*x3-y1x1*x1*x4;

	db=double(n)*y1x1*x4+y1x2*x1*x2+y1*x2*x3-y1x1*x2*x2-double(n)*y1x2*x3-y1*x1*x4;

	dc=double(n)*x2*y1x2+y1*x1*x3+y1x1*x1*x2-y1*x2*x2-double(n)*y1x1*x3-x1*x1*y1x2;

	*(a)= da/det;
	*(b)= db/det;
	*(c)= dc/det;

	if (fabs(det)>0.001) return 0;
	else return (-1);
}

int MNK(double *x, double *y, int M,int N, double *a)
{
	double *sx,*sy,xh;
	double **Matr,**Matrh,det;
	int r=(M+1)*(M+1);
	Matr=new double *[M+1];
	Matrh=new double *[M+1];
	for (int i=0;i<M+1;i++) 
	{
		Matr[i]=new double [M+1]; 
		Matrh[i]=new double [M+1]; 
	}
	sx=new double [2*M+1];
	sy=new double [M+1];
	FILE *filem;
	for (int i=0;i<2*M+1;i++){
		sx[i]=0;
		if (i<M+1) sy[i]=0;	
	}
	filem=fopen("new_mnk.txt","wt");
	sx[0]=N;
	for (int j=0;j<N;j++){
		xh=1;
		sy[0]+=y[j];
		for (int i=1;i<2*M+1;i++){
			xh*=x[j];
			sx[i]+=xh;
			if (i<M+1) sy[i]+=y[j]*xh;	
		}
	}

	for (int i=0;i<(M+1);i++)
	{
		for (int j=0;j<(M+1);j++)
		{
//			Matr[i*(M+1)+j]=sx[i+j];
//			fprintf(filem,"%lf  ",Matr[i*(M+1)+j]);
			Matr[i][j]=sx[i+j];
			fprintf(filem,"%lf  ",Matr[i][j]);
		}
		fprintf(filem,"\n");
	}
	fprintf(filem,"\n");
	for (int j=0;j<M+1;j++)
	{
		for (int i=0;i<M+1;i++)
		{
			Matrh[i][j]=Matr[i][j];
		}
	}	
	det=BareissDeterminant(Matr, M+1);
	fprintf(filem,"%lf \n ",det);


	for (int j=0;j<M+1;j++)
	{
		for (int k=0;k<M+1;k++)
		{
			for (int i=0;i<M+1;i++)
			{
				Matr[i][k]=Matrh[i][k];
			}
		}
		for (int i=0;i<M+1;i++)
		{
			Matr[i][j]=sy[i];
		}	
		a[j]=BareissDeterminant(Matr, M+1);
		a[j]/=det;
		fprintf(filem,"%lf \n ",a[j]);
	}
			
/**
	for (int i=1;i<(M+1);i++)
	{
		for (int j=0;j<(M+1);j++)
		{
			Matr[i*(M+1)+j]-=Matr[0*(M+1)+j]*(Matr[i*(M+1)+0]/Matr[0]);
			fprintf(filem,"%lf  ",Matr[i*(M+1)+j]);
		}
		fprintf(filem,"\n");
	}
/**/
	fprintf(filem,"\n");

	delete sx;
	delete sy;
	for (int i=0;i<M+1;i++) 
	{
		delete Matr[i];
		delete Matrh[i];
	}
	delete Matr;
	delete Matrh;
	fclose(filem);

	if (fabs(det)>0.001) return 0;
	else return (-1);
}


double VAllon(vector <double>& t,vector <double>& vr,double Tint)
{
	vector <double> mu; double arw=0,t0=t[0],sum=0;
	FILE* f;
	int i,m=0;
	f=fopen("help_Al.txt", "at");
	fprintf (f,"%lf\n", Tint);
	for (i=0; i<vr.size()-1;i++)
	{
		if (fabs(t[i]-t0)>=(Tint-0.00001))
		{
			sum=sum/m;
			mu.push_back(sum);
			fprintf(f,"%lf %ld\n", sum,m);
			t0=t[i];
			sum=0; m=0;
		}
//  ����� �� ������� �������???
		sum=sum+vr[i];//*(t[i+1]-t[i]);
		m++;
	}

	for (i=0; i<mu.size()-1;i++)
		arw=arw+sqr(mu[i+1]-mu[i]);

	arw=arw/(2*(mu.size()));
	arw=sqrt(arw);

//	StatFunc(mu,&d,&sko,&d,&d);
	mu.clear();
	fclose(f);
	return arw;
}

int StatFunc(vector <double>& data, double *mx, double *sigmax, double *minimum,double *maximum)
{
	int N=data.size();
	int i;
	double min,max,sr=0,sko=0;

	min=data[0];
	max=data[0];
	for (i=0;i<N;i++)
	{
		if (data[i]<min) min=data[i];
		if (data[i]>max) max=data[i];
		sr=sr+data[i];
	}
	if (N!=0) sr=sr/N;

	for (i=0;i<N;i++)
	{
		sko=sko+sqr(data[i]-sr);
	}
	if (N>1) sko=sqrt(sko/(N-1));

	*(mx)=sr;
	*(sigmax)=sko;
	*(minimum)=min;
	*(maximum)=max;

	if (N>0) return 1;
	else return 0;
}

int StatFunc(double* data, int N, double *mx, double *sigmax, double *minimum,double *maximum)
{
	int i;
	double min,max,sr=0,sko=0;

	min=data[0];
	max=data[0];
	for (i=0;i<N;i++)
	{
		if (data[i]<min) min=data[i];
		if (data[i]>max) max=data[i];
		sr=sr+data[i];
	}
	if (N!=0) sr=sr/N;

	for (i=0;i<N;i++)
	{
		sko=sko+sqr(data[i]-sr);
	}
	if (N>1) sko=sqrt(sko/(N-1));

	*(mx)=sr;
	*(sigmax)=sko;
	*(minimum)=min;
	*(maximum)=max;

	if (N>0) return 1;
	else return 0;
}

double AngOutToFloat(int Ang)
{double AngFl;

	Ang=(Ang&0x3FFF);
	AngFl=double(Ang);
	return AngFl;
}

double TempOutToFloat(int Temp)
{
double TempFl;

	if (Temp&0x800)
	{	
		Temp=(Temp|0xFFFFF000);
		TempFl=(double)Temp;
	}
	else TempFl=double(Temp&0x0FFF);
	return TempFl;
}

template<class T> T BareissDeterminant(T **const M, int const N)
{
    T denom(1); //!
    int exchanges(0);
    
    for(int l1(0); l1<N-1; ++l1)
    { //��������� ��� ������ �������, ����� ���������
        int maxN( l1 );
        T maxValue( fabs( M[l1][l1] ) );
        for(int l2(l1+1); l2<N; ++l2)
        { //������ ������ � ������������ �� ������ ���������
            T const value( fabs( M[l2][l1] ) );
            if( value > maxValue ) { maxN=l2; maxValue=value; }
        }
        
        if( maxN > l1 )
        { //����� �����
            T *const temp(M[l1]); M[l1]=M[maxN]; M[maxN]=temp;
            ++exchanges;
        } else { //����� �� �����, �� ����� �������� �� ����
            if( maxValue == T(0) ) return maxValue;
        }

        T const value1( M[l1][l1] ); //!
          //!
        
        for(int l2(l1+1); l2<N; ++l2)
        { //������� ������ �� ���� �����������
            T const value2( M[l2][l1] ); //!
            M[l2][l1] = T(0);
            for(int c(l1+1); c<N; ++c) //!
                M[l2][c]=(M[l2][c]*value1-M[l1][c]*value2)/denom;
        }

        denom = value1; //!
    }
    //!
    if(exchanges%2) return -M[N-1][N-1]; else return M[N-1][N-1];
}


