

#include "stdafx.h"
#include<conio.h>
#include "string.h"
#include "stdio.h"
#include<stdlib.h>
#include<cstdlib>
#include<time.h>
#include<math.h>
#include<windows.h>
#pragma comment(lib,"Winmm.lib")


#define fs 320
#define sc 30
#define P 12
#define pii 22.0/7.0
#define size 60000
double tw[]={0.0,1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
double arr[size][P+1];
double cb[33][P+1];
int us=0;
#define ep 0.03
#define deltaa 0.00001

#define rs 1000
#define N 5
#define M 32
long double a[rs][N+1];
long double b[rs][N+1];
long double g[rs][N+1];
long double dl[rs][N+1];
long double zai[rs][N+1][N+1];
int shai[rs][N+1];
int _q[rs];
long double A[N+1][N+1];
long double B[N+1][M+1];
long double PI[N+1];
long double _A[N+1][N+1];
long double _B[N+1][M+1];
long double _PI[N+1];
int O[rs];
int T;


short int waveIn[16025 * 3];


long double avgEnergy(int l, int r,long double arr[])
{
	long double energy = 0;
	for(int i = l; i<=r; i++)
		energy += arr[i]*arr[i];
	return energy/(r-l+1);
}

double abs(double x,double y)
{
	if(x>y)
		return x-y;
	return y-x;
}

int maxx(int x,int y)
{
	if(x>y)
	return x;
	return y;
}

void dc_shift(long double arr[])
{
	long double avg,count=0;
	int n=sizeof(arr);
	for(int i=0;i<n;i++)
		count+=arr[i];
	avg=count/n;
	for(int i=0;i<n;i++)
		arr[i]=arr[i]-avg;
}

void normalize(long double arr[])
{
	long double max_val=0,min_val=0;
	int n=sizeof(arr);
	for(int i=0;i<n;i++)
	{
		if(arr[i]>max_val)
		max_val=arr[i];
		if(arr[i]<min_val)
		min_val=arr[i];
	}
	max_val=maxx(max_val,0-min_val);
	for(int i=0;i<n;i++)
	arr[i]=(arr[i]*5000)/max_val;
}

void computeRis(long double arr[],long double R[])
{
	for(int i=0;i<=P;i++)
	{
		long double temp=0;
		for(int j=0;j<fs-i;j++)
			temp+=arr[j]*arr[j+i];
		R[i]=temp;
	}
}

void computeAis(long double R[],long double A[])
{
	long double E[P+1],k[P+1],a[P+1][P+1];
	E[0]=R[0];
	long double summation;
	for(int i=1;i<=P;i++)
	{
		summation=0;
		for(int j=1;j<=i-1;j++)
			summation+=a[j][i-1]*R[i-j];
		k[i]=(R[i]-summation)/E[i-1];
		a[i][i]=k[i];
		for(int j=1;j<=i-1;j++)
		a[j][i]=a[j][i-1]-k[i]*a[i-j][i-1];
		E[i]=(1-(k[i]*k[i]))*E[i-1];
	}
	for(int i=1;i<=12;i++)
		A[i]=a[i][P];
}

void computeCis(long double R[],long double A[],long double C[])
{
	C[0]=log(R[0]*R[0]);
	long double summation;
	for(int i=1;i<=12;i++)
	{
		summation=0;
		for(int k=1;k<=i-1;k++)
			summation+=(k*C[k]*A[i-k])/i;
		C[i]=A[i]+summation;
	}
}

void hamming(long double arr[])
{
	long double w;
	for(int i=0;i<fs;i++)
	{
		w=0.54-(0.46*cos((2*pii*i)/(fs-1)));
		arr[i]=arr[i]*w;
	}
}

void raisedSine(long double arr[])
{
	long double w;
	for(int i=1;i<=P;i++)
	{
		w=1+((P/2)*sin(pii*i/P));
		arr[i]=w*arr[i];
	}
}

long double Energy(long double arr[])
{
	long double e=0;
	for(int i=0;i<sizeof(arr);i++)
		e+=arr[i]*arr[i];
	return e;
}

void loadCodebook()
{
	FILE *fp;
	fp=fopen("./Output_Training/cb.txt","r");
	for(int i=1;i<=32;i++)
	{
		for(int j=1;j<=P;j++)
		{
			fscanf(fp,"%Lf ",&cb[i][j]);
		}
	}
}

int t_distIndx(long double C[])
{
	long double min_dis=INT_MAX,dis;
	int index=1;
	for(int i=1;i<=32;i++)
	{
		dis=0;
		for(int j=1;j<=P;j++)
		dis+=tw[j]*pow((C[j]-cb[i][j]),2);
		if(dis<min_dis)
		{
			min_dis=dis;
			index=i;
		}
	}
	return index;
}


long double fwd()
{
	for(int i=1;i<=N;i++)
		a[1][i]=PI[i]*B[i][O[1]];
	for(int t=1;t<=T-1;t++)
	{
		for(int j=1;j<=N;j++)
		{
			long double temp=0;
			for(int i=1;i<=N;i++)
				temp+=a[t][i]*A[i][j];
			a[t+1][j]=temp*B[j][O[t+1]];
		}
	}
	long double probability=0;
	for(int i=1;i<=N;i++)
		probability+=a[T][i];
	return probability;
}


void readModel(int digit)
{
	FILE *fpt;
	char ch[40];
	sprintf(ch,"Trained_Model/244101025_%d_PI.txt",digit);
	fpt=fopen(ch,"r");
	for(int i=1;i<=N;i++)
		fscanf(fpt,"%lf",&PI[i]);
	fclose(fpt);
	sprintf(ch,"Trained_Model/244101025_%d_A.txt",digit);
	fpt=fopen(ch,"r");
	for(int i=1;i<=N;i++)
	{
		for(int j=1;j<=N;j++)
			fscanf(fpt,"%lf",&A[i][j]);
	}
	fclose(fpt);
	sprintf(ch,"Trained_Model/244101025_%d_B.txt",digit);
	fpt=fopen(ch,"r");
	for(int i=1;i<=N;i++)
	{
		for(int j=1;j<=M;j++)
			fscanf(fpt,"%lf",&B[i][j]);
	}
	fclose(fpt);
}


int getDigit()
{
	long double probability=0,calc_probability=0;
	int index=0;
	for(int i=0;i<=9;i++)
	{
		readModel(i);
		calc_probability=fwd();
		if(calc_probability>probability)
		{
			probability=calc_probability;
			index=i;
		}
	}
	return index;
}

void PlayRecord()
{
 const int NUMPTS = 16025 * 3;   // 3 seconds
 int sampleRate = 16025;  
 // 'short int' is a 16-bit type; I request 16-bit samples below
    // for 8-bit capture, you'd    use 'unsigned char' or 'BYTE' 8-bit types
 HWAVEIN  hWaveIn;
 WAVEFORMATEX pFormat;
 pFormat.wFormatTag=WAVE_FORMAT_PCM;     // simple, uncompressed format
 pFormat.nChannels=1;                    //  1=mono, 2=stereo
 pFormat.nSamplesPerSec=sampleRate;      // 44100
 pFormat.nAvgBytesPerSec=sampleRate*2;   // = nSamplesPerSec * n.Channels * wBitsPerSample/8
 pFormat.nBlockAlign=2;                  // = n.Channels * wBitsPerSample/8
 pFormat.wBitsPerSample=16;              //  16 for high quality, 8 for telephone-grade
 pFormat.cbSize=0;
 // Specify recording parameters
 waveInOpen(&hWaveIn, WAVE_MAPPER,&pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);
 WAVEHDR      WaveInHdr;
 // Set up and prepare header for input
 WaveInHdr.lpData = (LPSTR)waveIn;
 WaveInHdr.dwBufferLength = NUMPTS*2;
 WaveInHdr.dwBytesRecorded=0;
 WaveInHdr.dwUser = 0L;
 WaveInHdr.dwFlags = 0L;
 WaveInHdr.dwLoops = 0L;
 waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
 HWAVEOUT hWaveOut;
 printf("\nPlaying...\n");
 waveOutOpen(&hWaveOut, WAVE_MAPPER, &pFormat, 0, 0, WAVE_FORMAT_DIRECT);
 waveOutWrite(hWaveOut, &WaveInHdr, sizeof(WaveInHdr)); // Playing the data
 Sleep(3 * 1000); //Sleep for as long as there was recorded
 waveInClose(hWaveIn);
 waveOutClose(hWaveOut);
}

void StartRecord()
{
 const int NUMPTS = 16025 * 3;   // 3 seconds
 int sampleRate = 16025;  
 // 'short int' is a 16-bit type; I request 16-bit samples below
 // for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit     types
 HWAVEIN      hWaveIn;
 MMRESULT result;
 WAVEFORMATEX pFormat;
 pFormat.wFormatTag=WAVE_FORMAT_PCM;     // simple, uncompressed format
 pFormat.nChannels=1;                    //  1=mono, 2=stereo
 pFormat.nSamplesPerSec=sampleRate;      // 8.0 kHz, 11.025 kHz, 22.05 kHz, and 44.1 kHz
 pFormat.nAvgBytesPerSec=sampleRate*2;   // =  nSamplesPerSec × nBlockAlign
 pFormat.nBlockAlign=2;                  // = (nChannels × wBitsPerSample) / 8
 pFormat.wBitsPerSample=16;              //  16 for high quality, 8 for telephone-grade
 pFormat.cbSize=0;
 // Specify recording parameters
 result = waveInOpen(&hWaveIn, WAVE_MAPPER,&pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);
 WAVEHDR      WaveInHdr;
 // Set up and prepare header for input
 WaveInHdr.lpData = (LPSTR)waveIn;
 WaveInHdr.dwBufferLength = NUMPTS*2;
 WaveInHdr.dwBytesRecorded=0;
 WaveInHdr.dwUser = 0L;
 WaveInHdr.dwFlags = 0L;
 WaveInHdr.dwLoops = 0L;
 waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
 // Insert a wave input buffer
 result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
 // Commence sampling input
 result = waveInStart(hWaveIn);
 printf("\nRecording...");
 Sleep(3 * 1000);
 // Wait until finished recording
 waveInClose(hWaveIn);
 //PlayRecord();
}



int predictDigit()
{
	char ch[100];
	long double arr[60000],frame_arr[fs+1];
	int index,framecount=0,predicted_digit;
	long double count;
	long double R[P+1],A[P+1],C[P+1];
	loadCodebook();
	
	int k=0;
	for(int i=0;i<16025*2;i++)
		arr[k++]=waveIn[i];

	dc_shift(arr);
	normalize(arr);
	framecount=0;
	
	for(int x=0;x+fs<k;x+=320)
	{
		
		index=0;
		for(int y=x;y<x+fs;y++)
		{
			frame_arr[index]=arr[y];
			index++;
		}
		hamming(frame_arr);
		computeRis(frame_arr,R);
		computeAis(R,A);
		computeCis(R,A,C);
		raisedSine(C);
		framecount++;
		O[framecount]=t_distIndx(C);
	}
	T=framecount;
	predicted_digit=getDigit();
	return predicted_digit;
}


int liveTest()
{
	StartRecord();
	int x=predictDigit();
	return x;
}

int indx=-1;

int digitTesting(int digit)
{
	long double probability=0,calc_probability=0;
	int index=-1;
	for(int i=0;i<=9;i++)
	{
		readModel(i);
		calc_probability=fwd();
		if(calc_probability>probability)
		{
			probability=calc_probability;
			index=i;
		}
	}
	indx=index;
	if(index==digit)
		return 1;
	return 0;
}