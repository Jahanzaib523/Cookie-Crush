//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================
#ifndef COOKIE_CRUSH_CPP
#define COOKIE_CRUSH_CPP

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include "util.h"
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10

#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // Cookie radius in pixels...

int width = 930, height = 660; // i have set my window size to be 800 x 600
int dball = 0; // difference between cookies center of 0 pixels
int byoffset = bradius + dball; // 2 * bradius + dball; // board yoffset
int value=0;
float score = 0;
int setcolor, psetcolor;
int pres=0;
enum GameState { // Use to check different states of game...
	Ready, Shot, Over, RemoveCluster
};
GameState gamestate = Ready;
int timing=120;
const int ncookies = 6;
int ex=0,yy=0;
enum Cookies {
	CK_BISCUIT, CK_BURGER, CK_CROISSONT, CK_CUPCAKE, CK_DONUT, CK_STAR
};
GLuint texture[ncookies];
GLuint tid[ncookies];
string tnames[] = { "biscuit.png", "burger.png", "croissont.png", "cupcake.png",
		"donut.png", "star.png" };
GLuint mtid[ncookies];
int cwidth = 60, cheight = 60; // 60x60 pixels cookies width & height...

void RegisterTextures()
/*Function is used to load the textures from the files and display*/
// Please do not change it...
{
	// allocate a texture name
	glGenTextures(ncookies, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);
	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*) &length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < ncookies; ++i) {

		// Read current cookie

		ifile.read((char*) &data[0], sizeof(char) * length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture( GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
				wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
				wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps( GL_TEXTURE_2D, 3, cwidth, cheight, GL_RGB,
		GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}

void DrawCookie(const Cookies &cname, int sx, int sy, int cwidth ,
		int cheight )
		/*Draws a specfic cookie at given position coordinate
		 * sx = position of x-axis from left-bottom
		 * sy = position of y-axis from left-bottom
		 * cwidth= width of displayed cookie in pixels
		 * cheight= height of displayed cookiei pixels.
		 * */
		{
	float fwidth = (float) cwidth / width * 2, fheight = (float) cheight
			/ height * 2;
	float fx = (float) sx / width * 2 - 1, fy = (float) sy / height * 2 - 1;

	glPushMatrix();
	glEnable( GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, mtid[cname]);
//	glTranslatef(0, 0, 0);
//	glRotatef(-M_PI / 2, 0, 0, 1);
	glBegin( GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}

float Distance(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
void Pixels2Cell(int px, int py, int & cx, int &cy) {

}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
		{

}

int locate[10][15]; // 2D array declared for location of candies
void cookietype()    // function is used to initialize the array with integers
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<15;j++)
		{
			locate[i][j]=GetRandInRange(0,6);   //assigning random values to 2D array
		}
	}	
		
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */

void MouseClicked(int button, int state, int x, int y) {
	int d,g=0;
	int j=(x-15)/60,i=(660-y-15)/60;
	pres=0;
	if (button == GLUT_LEFT_BUTTON && timing >=0) // dealing only with left button
	{
		value=value+1;
		g++;
			cout << "Left Mouse Button Pressed at Coordinates X=" << x << " Y= "<< height - y << endl;

			if(yy/60==i-1 && ex/60==j)
			{
				cout << "Left Up Mouse Button Pressed at Coordinates X=" << x << " Y= "<< height - y << endl;
				d=locate[yy/60][ex/60];
				locate[yy/60][ex/60]=locate[i][j];  // swaping cookies
				locate[i][j]=d;		
				pres=1;          // storing which key is pressed 
						
			}	
			glutPostRedisplay();    // redisplaying cookies after swapping
			
			if(yy/60==i+1 && ex/60==j )
			{
				cout << "Left Down Mouse Button Pressed at Coordinates X=" << x << " Y= "<< height - y << endl;
				d=locate[yy/60][ex/60];
				locate[yy/60][ex/60]=locate[i][j];  // swaping cookies
				locate[i][j]=d;
				pres=2;          // storing which key is pressed 
			}
			glutPostRedisplay();	 // redisplaying cookies after swapping

			if(yy/60==i && ex/60==j-1 )
			{
				cout << "Left Left Mouse Button Pressed at Coordinates X=" << x << " Y= "<< height - y << endl;
				d=locate[yy/60][ex/60];
				locate[yy/60][ex/60]=locate[i][j];  // swaping cookies
				locate[i][j]=d;		
				pres=3;          // storing which key is pressed 
						
			}	
			glutPostRedisplay();     // redisplaying cookies after swapping
			if(yy/60==i && ex/60==j+1)
			{
				cout << "Left Right Mouse Button Pressed at Coordinates X=" << x << " Y= "<< height - y << endl;
				d=locate[yy/60][ex/60];
				locate[yy/60][ex/60]=locate[i][j];  // swaping cookies
				locate[i][j]=d;
				pres=4;          // storing which key is pressed 
			}
			glutPostRedisplay();    // redisplaying cookies after swapping
	
	ex=x-15;
	yy=660-y-15;
	}
}

int s;
void movingdown()  // this fuction has almost all the combinations for the crushing of cookies
{			// this functiion converts cookies into blank spaces  and then drop cookies 
	int d,r=0 ; 
	for(int w=0;w<3;w++)
	{	
		s=0;
		r++;
		for(int i=0;i<10;i++)        //removing three similar candies
		{ 	
			for(int j=0;j<15;j++)
			{	
				if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+3][j] && 					locate[i][j]==locate[i+4][j] && locate[i][j]==locate[i+2][j+1] && locate[i][j]==locate[i+2][j+2] && 					i<6 && j<13  )
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+3][j]=-1;
					locate[i+4][j]=-1;
					locate[i+2][j+1]=-1;	
					locate[i+2][j+2]=-1;	
					s=s+1;
				}
					
		
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+3][j] && 					locate[i][j]==locate[i+4][j] && locate[i][j]==locate[i+2][j-1] && locate[i][j]==locate[i+2][j-2]&& 					i<6 && j>1  )
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+3][j]=-1;
					locate[i+4][j]=-1;
					locate[i+2][j-1]=-1;	
					locate[i+2][j-2]=-1;	
					s=s+1;
				}
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+2][j+1] && 					locate[i][j]==locate[i+2][j+2] && locate[i][j]==locate[i+2][j-1] && locate[i][j]==locate[i+2][j-2] && 					i<8 && j<13 && j>1 )
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+2][j+2]=-1;
					locate[i+2][j+1]=-1;
					locate[i+2][j-1]=-1;	
					locate[i+2][j-2]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i][j+1] && locate[i][j]==locate[i][j+2] && locate[i][j]==locate[i][j+3] && 					locate[i][j]==locate[i][j+4] && locate[i][j]==locate[i+1][j+2] && locate[i][j]==locate[i+2][j+2] && 					i<8 && j<11 && j>1  )
				{
					locate[i][j]=-1;
					locate[i][j+4]=-1;
					locate[i][j+3]=-1;
					locate[i][j+2]=-1;
					locate[i][j+1]=-1;
					locate[i+1][j+2]=-1;	
					locate[i+2][j+2]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+3][j] && 					locate[i][j]==locate[i+2][j+1] && locate[i][j]==locate[i+2][j+2] && i<7 && j<13   )
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+3][j]=-1;
					locate[i+2][j+1]=-1;	
					locate[i+2][j+2]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+3][j] && 					 locate[i][j]==locate[i+2][j-1] && locate[i][j]==locate[i+2][j-2] && i<7  && j>1 )
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+3][j]=-1;
					locate[i+2][j-1]=-1;	
					locate[i+2][j-2]=-1;	
					s=s+1;
				}
		
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+2][j+1] && 					locate[i][j]==locate[i+2][j-1] && locate[i][j]==locate[i+2][j-2] && i<8 && j<14 && j>1 )
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+2][j+1]=-1;
					locate[i+2][j-1]=-1;	
					locate[i+2][j-2]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i][j+1] && locate[i][j]==locate[i][j+2] && locate[i][j]==locate[i][j+3] && 					 locate[i][j]==locate[i+1][j+2] && locate[i][j]==locate[i+2][j+2]  && i<8 && j<12)
				{
					locate[i][j]=-1;
					locate[i][j+3]=-1;
					locate[i][j+2]=-1;
					locate[i][j+1]=-1;
					locate[i+1][j+2]=-1;	
					locate[i+2][j+2]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+3][j] && 					locate[i][j]==locate[i+1][j+1] && locate[i][j]==locate[i+1][j+2]  && i<7 && j<13)
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+3][j]=-1;
					locate[i+1][j+1]=-1;	
					locate[i+1][j+2]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+3][j] && 					locate[i][j]==locate[i+1][j-1] && locate[i][j]==locate[i+1][j-2]  && i<7 && j>1)
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+3][j]=-1;
					locate[i+1][j-1]=-1;	
					locate[i+1][j-2]=-1;	
					s=s+1;
				}		
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+2][j+1] && 					locate[i][j]==locate[i+2][j+2] && locate[i][j]==locate[i+2][j-1] && i<8 && j<13 &&j>0)
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+2][j+2]=-1;
					locate[i+2][j+1]=-1;
					locate[i+2][j-1]=-1;		
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i][j+1] && locate[i][j]==locate[i][j+2] && locate[i][j]==locate[i][j+3] && 					 locate[i][j]==locate[i+1][j+1] && locate[i][j]==locate[i+2][j+1]  && i<8 && j<12)
				{
					locate[i][j]=-1;
					locate[i][j+3]=-1;
					locate[i][j+2]=-1;
					locate[i][j+1]=-1;
					locate[i+1][j+1]=-1;	
					locate[i+2][j+1]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && 					locate[i][j]==locate[i+2][j-1] && locate[i][j]==locate[i+2][j-2]  && i<8 && j>1)
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+2][j-1]=-1;	
					locate[i+2][j-2]=-1;	
					s=s+1;
				}	
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && 					locate[i][j]==locate[i+1][j-1] && locate[i][j]==locate[i+1][j-2]  && i<8 && j>1)
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i][j-1]=-1;	
					locate[i][j-2]=-1;	
					s=s+1;
				}	
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+2][j+1] && 					locate[i][j]==locate[i+2][j+2] && i<8 && j<13)
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+2][j+2]=-1;
					locate[i+2][j+1]=-1;	
					s=s+1;
				}
			else if (locate[i][j]==locate[i][j+1] && locate[i][j]==locate[i][j+2] && 					 locate[i][j]==locate[i+1][j+1] && locate[i][j]==locate[i+2][j+1]  && i<8 && j<13)
				{
					locate[i][j]=-1;
					locate[i][j+2]=-1;
					locate[i][j+1]=-1;
					locate[i+1][j]=-1;	
					locate[i+2][j]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+2][j-1] 						&& locate[i][j]==locate[i+2][j+1] && i<8 && j<14 && j>0)
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+2][j+1]=-1;	
					locate[i+2][j-1]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+1][j-2] 						&& locate[i][j]==locate[i+1][j-1]  && i<8 && j>1)
				{
					locate[i][j]=-1;
					locate[i+1][j-1]=-1;
					locate[i+1][j-2]=-1;
					locate[i+2][j]=-1;	
					locate[i+1][j]=-1;	
					s=s+1;
				}

			else if (locate[i][j]==locate[i+2][j+1] && locate[i][j]==locate[i+2][j-1] && locate[i][j]==locate[i+1][j] 						&& locate[i][j]==locate[i+2][j]  && i<8 && j<14 && j>0)
				{
					locate[i][j]=-1;
					locate[i+2][j+1]=-1;
					locate[i+2][j-1]=-1;
					locate[i+1][j]=-1;	
					locate[i+2][j]=-1;	
					s=s+1;
				}
	
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+1][j+1] 						&& locate[i][j]==locate[i+1][j+2] && i<8 && j<13 )
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+1][j+1]=-1;	
					locate[i+1][j+2]=-1;	
					s=s+1;
				}
			else if (locate[i][j]==locate[i][j+1] && locate[i][j]==locate[i][j+2] && locate[i][j]==locate[i][j+3] && 					locate[i][j]==locate[i][j+4] && j<11)
				{
					locate[i][j]=-1;
					locate[i][j+1]=-1;
					locate[i][j+2]=-1;
					locate[i][j+3]=-1;
					locate[i][j+4]=-1;
					s=s+1;
				}
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+3][j] && 					locate[i][j+4]==locate[i][j] && i<6 )
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+3][j]=-1;
					locate[i+4][j]=-1;
					s=s+1;
				}
			else if (locate[i][j]==locate[i][j+1] && locate[i][j]==locate[i][j+2] && locate[i][j]==locate[i][j+3] && j<12 )
				{
					locate[i][j]=-1;
					locate[i][j+1]=-1;
					locate[i][j+2]=-1;
					locate[i][j+3]=-1;
					s=s+1;
				}
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && locate[i][j]==locate[i+3][j] && i<7)
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					locate[i+3][j]=-1;
					s=s+1;
				}
			else if (locate[i][j]==locate[i][j+1] && locate[i][j]==locate[i][j+2] && j<13  )
				{
					locate[i][j]=-1;
					locate[i][j+1]=-1;
					locate[i][j+2]=-1;
					s=s+1;
					
				}
			else if (locate[i][j]==locate[i+1][j] && locate[i][j]==locate[i+2][j] && i<8 )
				{
					locate[i][j]=-1;
					locate[i+1][j]=-1;
					locate[i+2][j]=-1;
					s=s+1;
					
				}
			}
		}
		
/* if user clicks two cookies and no combination is built by the swapping 
   then this part swap them back */
		if (s==0 && r==1)
			{
					if(pres==1)
					{
						
						d=locate[yy/60][ex/60];
						locate[yy/60][ex/60]=locate[yy/60-1][ex/60];
						locate[yy/60-1][ex/60]=d;	
						pres=0;	
						
					}
					glutPostRedisplay();
					if(pres==2)
					{
						d=locate[yy/60][ex/60];
						locate[yy/60][ex/60]=locate[yy/60+1][ex/60];
						locate[yy/60+1][ex/60]=d;	
						pres=0;		
						
					}
					glutPostRedisplay();
					if(pres==3)
					{
						d=locate[yy/60][ex/60];
						locate[yy/60][ex/60]=locate[yy/60][ex/60-1];
						locate[yy/60][ex/60-1]=d;	
						pres=0;
					}
					glutPostRedisplay();
					if(pres==4)
					{
						d=locate[yy/60][ex/60];
						locate[yy/60][ex/60]=locate[yy/60][ex/60+1];
						locate[yy/60][ex/60+1]=d;	
						pres=0;
					}
					glutPostRedisplay();	
				}		
		
// this part is dropping cookies in the blank spaces					
	for(int m=0;m<10;m++)
	{
		for(int i=0;i<10;i++)        
		{
			int g=0;
			for(int j=0;j<15;j++)
			{
			g=0;
					
							
				if(locate[i][j]==-1 && locate[i][j]!=-2 && timing>0)
				{
					int d=0;
					for(int z=i;z<9;z++)
					{									
						d=locate[z][j];
						locate[z][j]=locate[z+1][j];
						locate[z+1][j]=d;
						
					}
					if (locate[9][j]==-1)
					{	locate[9][j]=GetRandInRange(0,6);
						g=g+1;
					}
					if (value!=0)
						score+=g;
				
				}			
				
			}		
		}	
	}
	
     }
	
}
void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
//#if 0
	glClearColor(0.8/*Red Component*/, 0.5/*Green Component*/,
			0.2/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

// #----------------- Write your code here ----------------------------#
	//write your drawing commands here or call your drawing functions...

	
	// draw cookies..;

	movingdown();
	int i=0;
	while (i<15)
	{
		int j=0;
		while (j<10)
		{
			DrawCookie(Cookies(locate[j][i]), i*60+15,j*60+15, 58, 58);	// displaying cookies on canvas	
			j=j+1;

		}
	i=i+1;
	}


	if (timing>=0){
		DrawString(width / 2 - 20, height - 20, width+600*60, height+15,     //displays remaining time 
			"Time " + Num2Str(timing), colors[RED]);
		DrawString(width / 2 - 20, height - 20, width-430, height+15,        //displays user score
			"Score " + Num2Str(score), colors[BLUE]);
		DrawString(width / 2 - 20, height - 20, width+175, height+15,        // displays name of game
			"COOKIE CRUSH", colors[PURPLE]);
		DrawString(width / 2 - 20, height - 20, width+70, height+600,        
			"", colors[WHITE]);
		

	}
	else
	{
		DrawString(width / 2 - 20, height - 20, width+600*60, height+15, 
			"Time Over", colors[RED]);
		DrawString(width / 2 - 20, height - 20, width+100, height+400,		// displays game end tag
			"GAME OVER ", colors[RED]);
		DrawString(width / 2 - 20, height - 20, width+60, height+600, 		//displays user score when game ended
			"Score " + Num2Str(score), colors[BLUE]);
		DrawString(width / 2 - 20, height - 20, width+175, height+15,
			"COOKIE CRUSH", colors[PURPLE]);
		DrawString(width / 2 - 20, height - 20, width+70, height+600,
			"", colors[WHITE]);
	}
	if (timing==0)   // making blank spaces at the back of string when game ends
	{
		for(int i=5;i<7;i++)
	{
		for(int j=6;j<9;j++)
		{	
			locate[i][j]=-2;
		}
	}
	}

// #----------------- Write your code till here ----------------------------#
	// do not modify below this
	glutSwapBuffers(); // do not modify this line..

}
/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {	
	/*glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	int d=0;
	int j=x/60,i=(660-y)/60;
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		if (j>0){		
		d=locate[i][j];
		locate[i][j]=locate[i][j-1];
		locate[i][j-1]=d;
		}
		glutPostRedisplay();
	} else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
		if (j<15){
		d=locate[i][j];
		locate[i][j]=locate[i][j+1];
		locate[i][j+1]=d;
		}
		glutPostRedisplay();
	} else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
		if (i<10){		
		d=locate[i][j];
		locate[i][j]=locate[i+1][j];
		locate[i+1][j]=d;
		}
		glutPostRedisplay();
	} else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
		if (i>0){		
		d=locate[i][j];
		locate[i][j]=locate[i-1][j];
		locate[i-1][j]=d;
		}
		glutPostRedisplay();
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/
	/*
	 glutPostRedisplay();
	 */
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */

void MouseMoved(int x, int y) {

	cout << "Current Mouse Coordinates X=" << x << " Y= " << height - y << endl;
	glutPostRedisplay();
}





void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC /* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
	
}

/*
 * This function is called after every 1000.0 milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
float dt = 0, lastframe = 0;
void Timer(int m) {
	dt = (m - lastframe) / 1000.0;
	lastframe = m;
	timing--;  		// subtracting 1 sec from the allocated time
	
	
	

	// dt is time elapsed between two frames..
	glutPostRedisplay();

	// implement your functionality here

	glutTimerFunc(1000, Timer, m + 1);
}

/*
 * our gateway main function
 * */

int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...
	//Write your code here for filling the canvas with different Cookies. You can use the GetCookie function for getting the random shaped cookies
	
	cookietype(); //function calling for generating 2D array
	glutInit(&argc, argv); // initialize the graphics library...

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Cookie Crush"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Load the textures from the file..;
	RegisterTextures();
	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	glutMouseFunc(MouseClicked);
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters	
	
	

//// This function tells the library to call our Timer function after 1000.0 milliseconds...
	glutTimerFunc(1000, Timer, 0);

//// now handle the control to library and it will call our registered functions when
//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */
