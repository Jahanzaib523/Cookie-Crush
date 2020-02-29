//============================================================================
// Name        : ascii-art.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <cmath>
#include "CImg.h"
using namespace std;
using namespace cimg_library;


int const size=100;
int const w_size=100+100/10;
#define WIDTH 480	
#define HEIGHT 720
float array[250][230];
float imgArray[HEIGHT][WIDTH];
float asci_avg[10][10];
float img_avg[size][w_size];
float asci_max=0;
float asci_min=0;
float img_max=0;
float img_min=0;

void min_max(float asci_avg[][10],float img_avg[][w_size]);
void printing_structure(float img_avg[][w_size], float asci_avg[][10]);
char characters(float asci_avg[10][10], float var);

void ReadAsciiCharactersImage(float array[][230]) {

	CImg<unsigned char> img("ascii2.png");
	int k = 0;
	for (int i = 0; i < 250; i++)
		for (int j = 0; j < 230; ++j)
			array[i][j] = img[k++];

}
void ReadImage(string imgname, float imgArray[][WIDTH]) {
	CImg<unsigned char> img(imgname.c_str());
	int k = 0;
	for (int i = 0; i < img.height(); i++)
		for (int j = 0; j < img.width(); ++j)
			imgArray[i][j] = img[k++];

}
void finding_avg(float array[250][230],float imgArray[][WIDTH]) 
{
	for(int m=0;m<10;m++)
	{
		for(int i=0;i<10;i++)
		{
			float sum=0;
			for(int j=0;j<25;j++)
			{
				for(int k=0;k<23;k++)
				{
					sum=sum+array[m*25+j][i*23+k];
				}
			}
			asci_avg[m][i]=sum/(23*25);
		}
	}
	
			

	for(int m=0;m<size;m++)
	{
		for(int i=0;i<w_size;i++)
		{
			float sum=0;
			for(int j=0;j<HEIGHT/size;j++)
			{
				for(int k=0;k<WIDTH/size;k++)
				{
					sum=sum+imgArray[m*(HEIGHT/size)+j][i*(WIDTH/size)+k];
				}
			}
			img_avg[m][i]=sum/((WIDTH/size)*(HEIGHT/size));
		}
	}
	min_max(asci_avg,img_avg);
}
void min_max(float asci_avg[][10],float img_avg[][w_size])
{
	img_max=img_avg[0][0];
	img_min=img_avg[0][0];
	asci_max=asci_avg[0][0];
	asci_min=asci_avg[0][0];
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<w_size;j++)
		{
			if(img_avg[i][j]>img_max)
				img_max=img_avg[i][j];
			if(img_avg[i][j]<img_min)
				img_min=img_avg[i][j];
		}
	}

	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			if(asci_avg[i][j]>asci_max)
				asci_max=asci_avg[i][j];
			if(asci_avg[i][j]<asci_min)
				asci_min=asci_avg[i][j];

		}
	}
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<w_size;j++)
		{
			img_avg[i][j]=((img_avg[i][j]-img_min)/(img_max-img_min));
		}
	}
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			asci_avg[i][j]=((asci_avg[i][j]-asci_min)/(asci_max-asci_min));
		}
	}
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			cout<<asci_avg[i][j]<<" ";
		}
		cout<<"\n";
	}

	printing_structure(img_avg, asci_avg);
}
void printing_structure(float img_avg[][w_size], float asci_avg[][10])
{	
	float var,min;
	float chk;
	float b;
	for(int m=0;m<size;m++)
	{	
		for(int k=0;k<w_size;k++)
		{
			chk=img_avg[0][0]-asci_avg[0][0];
			b=chk;
			if ((img_avg[0][0]-asci_avg[0][0])<0)
			{	chk=(-1*chk);
				b=chk;
			}
			min=b;
			for(int i=0;i<10;i++)
			{
				for(int j=0;j<10;j++)
				{	
					if ((img_avg[m][k]-asci_avg[i][j])<0)
					{	
						chk=img_avg[m][k]-asci_avg[i][j];
						chk=(-1*chk);
						b=chk;
					}
					else if ((img_avg[m][k]-asci_avg[i][j])>0)
					{	
						chk=img_avg[m][k]-asci_avg[i][j];
						b=chk;
					}
					if (b<min)
					{
						var=asci_avg[i][j];
						min=b;
					}
				}
			}
			cout<<characters(asci_avg,var);
		
		}
		cout<<"\n";
	}
}
char characters(float asci_avg[10][10], float var)
{
	int sum=32;
	for(int i=0;i<10;i++)
	{
		for (int j=0;j<10;j++)
		{
			if (var==asci_avg[i][j])
				return sum;
			else 
				sum=sum+1;
		}
	}
}
int main(int argc, char *argv[]) {

	int winsize = 5;
	if (argc < 2 || argc > 3) {
		cerr << "\nProgram require at least one input \n "
				"Usage: ascii-art <filename> <window-size (by default 5> \n"
				" " << endl;
		return 0;
	}
	string imname = argv[1]; // variable to store given image name
	if (argc == 3)
		winsize = atoi(argv[2]);

	// Your code goes here, write auxliary functions and
	// call them here
	ReadAsciiCharactersImage(array);
	ReadImage(imname,imgArray);
	finding_avg(array,imgArray);
}	
