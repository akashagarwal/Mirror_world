#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

// Function Declarations
void drawScene();
void update(int value);
void drawBox(float,float);
void drawBall(float rad);
void drawTriangle();
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void handleMotion(int,int);
void handlePassiveMotion(int,int);
void drawLine(float x1,float y1, float x2, float y2,float,float,float);
void cal_procenter();
bool find_intersection(float,float,float,float,float,float,float,float);
bool find_side(float,float,float,float,float,float);
void findline();
// Global Variables


float m_r=5;
int m_t=10;
float xint,yint; // variables used to store output of intersection fn if it exists
float box_x;
float box_y;
float sc;
float theta = 0.0f; 
int i,no;
float ww,wh;
float a,b,c,d;
char *filename;
FILE *f;
int gaze_flag;
GLfloat matrix[16]; 
float xold,yold;
int x_curr,y_curr; //current co-ord's
int stipple_count=0;
int stipple_flag=-1;
int select_flag=0;
int select_total;
int random_flag;
FILE * pFile;
char line[100];
//Global variables used to store the co-ord's of projectors, mirrors and blocks

int nopro;
float pro[20][8];
bool prof[20];
int nomir;
float mir[20][4];
bool mirf[20];
int noblo;
float blo[20][4];
bool blof[20];
int nobou;
float bou[4][4];

int nocp;
float cp[100][3];

struct l_ray
{
	float p[2];
	struct l_ray * next;
};

struct l_ray * ray[20][100];


float ppx,ppy;

void find_next(l_ray * temp);
float find_distance(l_ray *t1,float x,float y);


int main(int argc, char *argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	//	int windowWidth = w * 2 / 3;
	//	int windowHeight = h * 2 / 3;

	gaze_flag=-1;
	random_flag=-1;

	pFile = fopen (argv[1] , "r");

	findline();
	sscanf(line,"%f",&ww);
	sscanf(line,"%f",&wh);

	nobou=4;
	bou[0][0]=-(ww/2);
	bou[0][1]=-(wh/2);
	bou[0][2]=(ww/2);
	bou[0][3]=-(wh/2);	
	bou[1][0]=(ww/2);
	bou[1][1]=-(wh/2);
	bou[1][2]=(ww/2);
	bou[1][3]=(wh/2);
	bou[2][0]=-(ww/2);
	bou[2][1]=(wh/2);
	bou[2][2]=(ww/2);
	bou[2][3]=(wh/2);
	bou[3][0]=-(ww/2);
	bou[3][1]=-(wh/2);
	bou[3][2]=-(ww/2);
	bou[3][3]=(wh/2);


	findline();
	sscanf(line,"%d",&nopro);
	for(i=0;i<nopro;i++)
	{
	findline();
		sscanf(line,"%f %f %f %f",&pro[i][0],&pro[i][1],&pro[i][2],&pro[i][3]);
	findline();
		sscanf(line,"%f",&pro[i][4]);
	findline();
		sscanf(line,"%f",&pro[i][5]);
		prof[i]=0;
	}
//	printf("%d\n",nopro);
	findline();
	sscanf(line,"%d",&noblo);
	for(i=0;i<noblo;i++)
	{
	findline();
		sscanf(line,"%f %f %f %f",&blo[i][0],&blo[i][1],&blo[i][2],&blo[i][3]);
		blof[i]=0;
	}
//	printf("%d\n",noblo);
	findline();
	sscanf(line,"%d",&nomir);
	for(i=0;i<nomir;i++)
	{
	findline();
		sscanf(line,"%f %f %f %f",&mir[i][0],&mir[i][1],&mir[i][2],&mir[i][3]);
		mirf[i]=0;
	}
	select_total=nopro+noblo+nomir;
	//	printf("%d %d\n",select_total,select_flag);
	//	blof[0]=1;
	/*
	   if (find_intersection(bou[0][0],bou[0][1],bou[0][2],bou[0][3],bou[1][0],bou[1][1],bou[1][2],bou[1][3])==1)
	   printf("%f %f\n",xint,yint);
	   else printf("NO\n");

	   if (find_intersection(0,0,5,5,1,0,0,1)==1)
	   printf("%f %f\n",xint,yint);
	   else printf("NO\n");

	 */

	cal_procenter();
	//	printf("%f %f\n",pro[0][6],pro[0][7]);
	glutInitWindowSize(ww, wh);
	glutInitWindowPosition((w - ww) / 2, (h - wh) / 2);


	glutCreateWindow("ASSIGNMENT 1");  // Setup the window
	initRendering();

	// Register callbacks
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseclick);
	glutReshapeFunc(handleResize);
	glutMotionFunc(handleMotion);
	glutPassiveMotionFunc(handlePassiveMotion);
	glutTimerFunc(10, update, 0);

	glutMainLoop();
	return 0;
}

// Function to draw objects on the screen
void drawScene()
{
	int i;
	/*	 glDisable(GL_DEPTH_TEST);

		 glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
		 glLoadIdentity();                           // Reset The Projection Matrix

		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 glOrtho(0,500,500,0,0,1);
		 glMatrixMode(GL_MODELVIEW);
		 glLoadIdentity();
		 glPushMatrix();

	// Draw Box
	//	glTranslatef(0.0f, 0.0f, -5.0f);
	glColor3f(0.4f, 0.4f, 0.4f);
	drawBox(box_x,box_y);

	 */
	/*

	 */
	// Setup a 2D projection
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0, ww, wh, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	//Displacement trick for exact pixelization
	glTranslatef(0.375,0.375, 0.0f);

	glTranslatef(ww/2,wh/2,0.0f);
	glScalef(1.0f,-1.0f,0.0f);

	glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
	glPushMatrix();
	/*
	   glPushMatrix();
	   glLoadIdentity ();
	//		glOrtho (0, ww, wh, 0, 0, 1);
	//		glTranslatef(0.375,0.375, 0.0f);
	glTranslatef(mir[0][0],mir[0][1],0);
	glRotatef(theta, 0, 0, 1.0f);
	glTranslatef(-mir[0][0],-mir[0][1],0);
	glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
	xold=mir[0][2];
	yold=mir[0][3];
	mir[0][2]=mir[0][2]*matrix[0]+mir[0][3]*matrix[4]+matrix[12];
	mir[0][3]=mir[0][2]*matrix[1]+mir[0][3]*matrix[5]+matrix[13];
	 */
	/*
	   xold=mir[0][2];
	   yold=mir[0][3];
	   float x,y,r;
	   x = atan2((mir[0][3]-mir[0][1]),(mir[0][2]-mir[0][0]));
	//	x=atan(sl);
	y=DEG2RAD(theta);
	r=(mir[0][3]-mir[0][1])*(mir[0][3]-mir[0][1]) + (mir[0][2]-mir[0][0])*(mir[0][2]-mir[0][0]);
	r=sqrt(r);
	//	printf("%f\n",y);
	mir[0][2]=mir[0][0] + r*cos(x+y);
	mir[0][3]=mir[0][1] + r*sin(x+y);
	 */
	for(i=0;i<noblo;i++)
	{	if (blof[i]==1)
		{
			/*
			//			printf("%f %f\n",blo[i][2],blo[i][3]);
			xold=blo[i][2];
			yold=blo[i][3];
			float x,y,r;
			x = atan2((blo[i][3]-blo[i][1]),(blo[i][2]-blo[i][0]));
			//	x=atan(sl);
			y=DEG2RAD(theta);
			r=(blo[i][3]-blo[i][1])*(blo[i][3]-blo[i][1]) + (blo[i][2]-blo[i][0])*(blo[i][2]-blo[i][0]);
			r=sqrt(r);
			//	printf("%f\n",y);
			blo[i][2]=blo[i][0] + r*cos(x+y);
			blo[i][3]=blo[i][1] + r*sin(x+y);
			//			printf("%f %f\n",blo[i][2],blo[i][3]);
			 */
		}

		//	printf("%d %f %f\n",i,pro[i][6],pro[i][7]);
		if(blof[i]==1)
			glLineWidth(6);
		else
			glLineWidth(3);

		drawLine(blo[i][0],blo[i][1],blo[i][2],blo[i][3],1.0,0.0,0.0);
	}
	for(i=0;i<nomir;i++)
	{
		/*
		   if (mirf[i]==1)
		   {	
		   xold=mir[i][2];
		   yold=mir[i][3];
		   float x,y,r;
		   x = atan2((mir[i][3]-mir[i][1]),(mir[i][2]-mir[i][0]));
		//	x=atan(sl);
		y=DEG2RAD(theta);
		r=(mir[i][3]-mir[i][1])*(mir[i][3]-mir[i][1]) + (mir[i][2]-mir[i][0])*(mir[i][2]-mir[i][0]);
		r=sqrt(r);
		//	printf("%f\n",y);
		mir[i][2]=mir[i][0] + r*cos(x+y);
		mir[i][3]=mir[i][1] + r*sin(x+y);
		}
		 */
		if (i==0)
		{
			//		glPushMatrix();
			//		glTranslatef(mir[0][0],mir[0][1],0);
			//		glRotatef(theta, 0, 0, 1.0f);
			//		glTranslatef(-mir[0][0],-mir[0][1],0);


			//		mir[0][3]=mir[0][3]*matrix[0]+mir[0][4]*matrix[1]+matrix[3];
			//printf("ttt %f\n",mir[0][3]);
		}
		if (mirf[i]==1)
			glLineWidth(6);
		else
			glLineWidth(3);
		drawLine(mir[i][0],mir[i][1],mir[i][2],mir[i][3],0.0,0.0,1.0);
		if (i==0)
		{
			//		glPopMatrix();
		}
	}
	for(i=0;i<nopro;i++)
	{
		/*
		   if (prof[i]==1)
		   {	
		   xold=pro[i][2];
		   yold=pro[i][3];
		   float x,y,r;
		   x = atan2((pro[i][3]-pro[i][1]),(pro[i][2]-pro[i][0]));
		//	x=atan(sl);
		y=DEG2RAD(theta);
		r=(pro[i][3]-pro[i][1])*(pro[i][3]-pro[i][1]) + (pro[i][2]-pro[i][0])*(pro[i][2]-pro[i][0]);
		r=sqrt(r);
		//	printf("%f\n",y);
		pro[i][2]=pro[i][0] + r*cos(x+y);
		pro[i][3]=pro[i][1] + r*sin(x+y);
		}
		 */
		//	printf("%d %f %f\n",i,pro[i][6],pro[i][7]);
		if(prof[i]==1)
			glLineWidth(6);
		else
			glLineWidth(3);
		drawLine(pro[i][0],pro[i][1],pro[i][2],pro[i][3],0.0,1.0,0.0);
	}
	cal_procenter();
	for(i=0;i<nopro;i++)
	{
		if(stipple_flag==1)
		{
			if(stipple_count == 16)
				stipple_count = 0;
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(5,0x00ff << stipple_count++);
		}

		glBegin(GL_POINTS);
		glVertex2f(pro[i][6],pro[i][7]);
		glEnd();
		int j;
		float c1,c2,c3;
		c1=1;
		c2=0;
		c3=0;
		for(j=0;j<pro[i][5];j++)
		{
			if(2*j<pro[i][5])
			{
				c1-=(2.0/pro[i][5]);
				c2+=(2.0/pro[i][5]);
			}
			else
			{
				c2-=(2.0/pro[i][5]);
				c3+=(2.0/pro[i][5]);
			}
		//	printf("%f %f\n",c1,c3);
			glColor3f(c1,c2,c3);
			l_ray * temp;
			temp=ray[i][j];
			while(temp!=NULL && temp->next!=NULL)
			{
				glLineWidth(1);
				drawLine(temp->p[0],temp->p[1],temp->next->p[0],temp->next->p[1],c1,c2,c3);
				temp=temp->next;
			}
		}
		glDisable(GL_LINE_STIPPLE);
	}
	/*
	   for(i=0;i<nomir;i++)
	   {
	   if(mirf[i]==1)
	   {
	   mir[i][2]=xold;
	   mir[i][3]=yold;
	   }
	   }
	   for(i=0;i<noblo;i++)
	   {

	   if(blof[i]==1)
	   {
	   blo[i][2]=xold;
	   blo[i][3]=yold;
	   }

	   }

	   for(i=0;i<nopro;i++)
	   {
	   if(prof[i]==1)
	   {
	   pro[i][2]=xold;
	   pro[i][3]=yold;
	   }
	   }

	 */
	/*	for(i=0;i<4;i++)
		{
		for(j=0;j<4;j++)
		printf("%f ",matrix[i*4+j]);
		printf("\n");
		} */


	//	cal_procenter();
	glPopMatrix();
	glutSwapBuffers();
}

// Function to handle all calculations in the scene
// updated evry 10 milliseconds
void update(int value) 
{
	//	printf("in update %d %d\n",x_curr,y_curr);
	//	cal_procenter();
	//	printf("%d\n",random_flag);
	if (random_flag==1)
	{
		int r,m;
		float xx,yy,theta,rr;
		r=rand()%nopro;
		m=rand()%4;
		theta=rand()%2;
		//		printf("%d %d\n",r,m);
		if(m==0 && pro[r][0]<ww/2 && pro[r][2]<ww/2)
		{
			pro[r][0]+=5;
			pro[r][2]+=5;
		}
		if(m==1 && pro[r][0]>(-ww/2) && pro[r][2]>(-ww/2)) 
		{
			pro[r][0]-=5;
			pro[r][2]-=5;
		}
		if(m==2 && pro[r][1]<wh/2 && pro[r][3]<wh/2)
		{
			pro[r][1]+=5;
			pro[r][3]+=5;
		}
		if(m==3 && pro[r][1]>(-wh/2) && pro[r][3]>(-wh/2)) 
		{
			pro[r][1]-=5;
			pro[r][3]-=5;
		}
		xx = atan2((pro[r][3]-pro[r][1]),(pro[r][2]-pro[r][0]));
		//	x=atan(sl);
		yy=DEG2RAD(theta);
		rr=(pro[r][3]-pro[r][1])*(pro[r][3]-pro[r][1]) + (pro[r][2]-pro[r][0])*(pro[r][2]-pro[r][0]);
		rr=sqrt(rr);
		//	printf("%f\n",y);
		pro[r][2]=pro[r][0] + rr*cos(xx+yy);
		pro[r][3]=pro[r][1] + rr*sin(xx+yy);
		//	

		r=rand()%noblo;
		m=rand()%4;
		theta=rand()%5;
		if(m==0 && blo[r][0]<ww/2 && blo[r][2]<ww/2)
		{
			blo[r][0]+=5;
			blo[r][2]+=5;
		}
		if(m==1 && blo[r][0]>(-ww/2) && blo[r][2]>(-ww/2))
		{
			blo[r][0]-=5;
			blo[r][2]-=5;
		}
		if(m==2 && blo[r][1]<wh/2 && blo[r][3]<wh/2)
		{
			blo[r][1]+=5;
			blo[r][3]+=5;
		}
		if(m==3 && blo[r][1]>(-wh/2) && blo[r][3]>(-wh/2))
		{
			blo[r][1]-=5;
			blo[r][3]-=5;
		}
		xx = atan2((blo[r][3]-blo[r][1]),(blo[r][2]-blo[r][0]));
		//	x=atan(sl);
		yy=DEG2RAD(theta);
		rr=(blo[r][3]-blo[r][1])*(blo[r][3]-blo[r][1]) + (blo[r][2]-blo[r][0])*(blo[r][2]-blo[r][0]);
		rr=sqrt(rr);
		//	printf("%f\n",y);
		blo[r][2]=blo[r][0] + rr*cos(xx+yy);
		blo[r][3]=blo[r][1] + rr*sin(xx+yy);

		r=rand()%nomir;
		m=rand()%4;
		theta=rand()%5;
		if(m==0 && mir[r][0]<ww/2 && mir[r][2]<ww/2)
		{
			mir[r][0]+=5;
			mir[r][2]+=5;
		}
		if(m==1 && mir[r][0]>(-ww/2) && mir[r][2]>(-ww/2))
		{
			mir[r][0]-=5;
			mir[r][2]-=5;
		}
		if(m==2 && mir[r][1]<wh/2 && mir[r][3]<wh/2)
		{
			mir[r][1]+=5;
			mir[r][3]+=5;
		}
		if(m==3 && mir[r][1]>(-wh/2) && mir[r][3]>(-wh/2))
		{
			mir[r][1]-=5;
			mir[r][3]-=5;
		}
		xx = atan2((mir[r][3]-mir[r][1]),(mir[r][2]-mir[r][0]));
		//	x=atan(sl);
		yy=DEG2RAD(theta);
		rr=(mir[r][3]-mir[r][1])*(mir[r][3]-mir[r][1]) + (mir[r][2]-mir[r][0])*(mir[r][2]-mir[r][0]);
		rr=sqrt(rr);
		//	printf("%f\n",y);
		mir[r][2]=mir[r][0] + rr*cos(xx+yy);
		mir[r][3]=mir[r][1] + rr*sin(xx+yy);

		theta=0;
	}
	glutTimerFunc(50, update, 0);
}

void drawLine(float x1,float y1, float x2, float y2,float a,float b,float c)
{
	//	printf("%d\n",(int)(b * 100));
	/*
	   if (((int)(b * 100))!=84)
	   glLineWidth(3);
	   else
	   glLineWidth(1);
	//	glEnable(GL_LINE_SMOOTH);
	 */
	glBegin(GL_LINES);
	glColor3f(a,b,c);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd();
}

void drawBox(float x,float y) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-x, -y);
	glVertex2f(x, -y);
	glVertex2f(x, y);
	glVertex2f(-x, y);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawBall(float rad) {

	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) {
		glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
	}
	glEnd();
}

void drawTriangle() {

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
}

// Initializing some openGL 3D rendering options
void initRendering() {

	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	glClearColor(0, 0, 0, 0.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) 
{

	if (key == 27) {
		exit(0);     // escape key is pressed
	}
	int ii;
	if (key == 'm')
	{
		int v=nomir;
		mir[v][0]=0;
		mir[v][1]=0;
		mir[v][2]=100;
		mir[v][3]=10;
		for(ii=0;ii<nopro;ii++)
			prof[ii]=0;
		for(ii=0;ii<nomir;ii++)
			mirf[ii]=0;
		for(ii=0;ii<noblo;ii++)
			blof[ii]=0;
		mirf[v]=1;
		nomir++;
	}
		if (key == 'b')
	{
		int v=noblo;
		blo[v][0]=0;
		blo[v][1]=0;
		blo[v][2]=100;
		blo[v][3]=10;
		for(ii=0;ii<nopro;ii++)
			prof[ii]=0;
		for(ii=0;ii<nomir;ii++)
			mirf[ii]=0;
		for(ii=0;ii<noblo;ii++)
			blof[ii]=0;
		blof[v]=1;
		noblo++;
	}
			if (key == 'p')
	{
		int v=nopro;
		pro[v][0]=0;
		pro[v][1]=0;
		pro[v][2]=100;
		pro[v][3]=10;
		pro[v][4]=20;
		pro[v][5]=10;
		for(ii=0;ii<nopro;ii++)
			prof[ii]=0;
		for(ii=0;ii<nomir;ii++)
			mirf[ii]=0;
		for(ii=0;ii<noblo;ii++)
			blof[ii]=0;
		prof[v]=1;
		nopro++;
	}
	
	if(key=='f' || key=='F')
	{
		m_t+=5;
		m_r+=2;
	}
	if(key=='s' || key=='S')
	{
		if(m_t>5)
			m_t-=5;
		if(m_r>2)
			m_r-=2;
	}
	if (key=='g' || key=='G')
		gaze_flag=gaze_flag*(-1);
	//	printf("%d\n",gaze_flag);
	if (key=='l'|| key=='L')
		theta += m_r;
	if (key=='r' || key=='R')
		theta -= m_r;
	if (key=='q' || key == 'Q')
	{
//		int ii;
		for(ii=0;ii<nopro;ii++)
			prof[ii]=0;
		for(ii=0;ii<nomir;ii++)
			mirf[ii]=0;
		for(ii=0;ii<noblo;ii++)
			blof[ii]=0;
		FILE * wf;
		wf=fopen("state_at_exit","w");
		fprintf(wf,"# All numbers are in floating point except for number of items\n");
		fprintf(wf,"%f %f \t # width (x) and height (y) of the world\n",ww,wh);
		fprintf(wf,"%d \t # number of Projectors\n",nopro);
		for(ii=0;ii<nopro;ii++)
		{
			fprintf(wf,"%f %f %f %f \t # Projector %d line: x1 y1 x2 y2\n",pro[ii][0],pro[ii][1],pro[ii][2],pro[ii][3],ii+1);
			fprintf(wf,"%f \t # Distance(d) to the point light source\n",pro[ii][4]);
			fprintf(wf,"%d \t # Number of pixels in the Projector\n",(int)pro[ii][5]);
		}
		fprintf(wf,"%d \t # number of Blocks\n",noblo);
		for(ii=0;ii<noblo;ii++)
		{
			fprintf(wf,"%f %f %f %f \t # BLocks %d line: x1 y1 x2 y2\n",blo[ii][0],blo[ii][1],blo[ii][2],blo[ii][3],ii+1);
		}
		fprintf(wf,"%d \t # number of Mirrors\n",nomir);
		for(ii=0;ii<nomir;ii++)
		{
			fprintf(wf,"%f %f %f %f \t # Mirror %d line: x1 y1 x2 y2\n",mir[ii][0],mir[ii][1],mir[ii][2],mir[ii][3],ii+1);
		}
	}


	float xx,yy,r;
	int c=0,nf;
	nf=0;
	for(ii=0;ii<noblo;ii++)
	{
		if (blof[ii]==1)
		{
			c=ii;
			nf=1;
		}
	}
	if (nf==1)
	{
		xx = atan2((blo[c][3]-blo[c][1]),(blo[c][2]-blo[c][0]));
		//	x=atan(sl);
		yy=DEG2RAD(theta);
		r=(blo[c][3]-blo[c][1])*(blo[c][3]-blo[c][1]) + (blo[c][2]-blo[c][0])*(blo[c][2]-blo[c][0]);
		r=sqrt(r);
		//	printf("%f\n",y);
		blo[c][2]=blo[c][0] + r*cos(xx+yy);
		blo[c][3]=blo[c][1] + r*sin(xx+yy);
	}
	//
	nf=0;
	for(ii=0;ii<nomir;ii++)
	{
		if (mirf[ii]==1)
		{
			nf=1;
			c=ii;
		}
	}
	if (nf==1)
	{
		xx = atan2((mir[c][3]-mir[c][1]),(mir[c][2]-mir[c][0]));
		//	x=atan(sl);
		yy=DEG2RAD(theta);
		r=(mir[c][3]-mir[c][1])*(mir[c][3]-mir[c][1]) + (mir[c][2]-mir[c][0])*(mir[c][2]-mir[c][0]);
		r=sqrt(r);
		//	printf("%f\n",y);
		mir[c][2]=mir[c][0] + r*cos(xx+yy);
		mir[c][3]=mir[c][1] + r*sin(xx+yy);
		//	
	}
	nf=0;
	for(ii=0;ii<nopro;ii++)
	{
		if (prof[ii]==1)
		{
			c=ii;
			nf=1;
		}
	}
	if (nf==1)
	{
		xx = atan2((pro[c][3]-pro[c][1]),(pro[c][2]-pro[c][0]));
		//	x=atan(sl);
		yy=DEG2RAD(theta);
		r=(pro[c][3]-pro[c][1])*(pro[c][3]-pro[c][1]) + (pro[c][2]-pro[c][0])*(pro[c][2]-pro[c][0]);
		r=sqrt(r);
		//	printf("%f\n",y);
		pro[c][2]=pro[c][0] + r*cos(xx+yy);
		pro[c][3]=pro[c][1] + r*sin(xx+yy);
		//	
	}
	theta=0;
	if (key=='t' || key== 'T')
		stipple_flag=stipple_flag*(-1);
	if (key=='c' || key=='C')
	{
		//		printf("c pressed\n");
		if (select_flag==select_total)
			select_flag=0;
		if (select_flag<nopro)
		{
			int fff=select_flag;
			for(i=0;i<nopro;i++)
			{
				if (i==fff)
					prof[i]=1;
				else
					prof[i]=0;
			}
			for(i=0;i<nomir;i++)
			{
				mirf[i]=0;
			}
			for(i=0;i<noblo;i++)
			{
				blof[i]=0;
			}
		}
		else if(select_flag<(nopro+nomir))
		{
			int fff;
			fff=select_flag-nopro;	
			for(i=0;i<nopro;i++)
			{
				prof[i]=0;
			}
			for(i=0;i<nomir;i++)
			{
				if (i==fff)
					mirf[i]=1;
				else
					mirf[i]=0;
			}
			for(i=0;i<noblo;i++)
			{
				blof[i]=0;
			}
		}
		else
		{
			int fff;
			fff=select_flag-(nopro+nomir);
			for(i=0;i<nopro;i++)
			{
				prof[i]=0;
			}
			for(i=0;i<nomir;i++)
			{
				mirf[i]=0;
			}
			for(i=0;i<noblo;i++)
			{
				if (i==fff)
					blof[i]=1;
				else
					blof[i]=0;
			}

		}
		select_flag++;
	}
	if (key=='z' || key=='Z')
	{

		random_flag=random_flag*(-1);
		//	printf("random_flag %d\n",random_flag);
	}
}

void handleKeypress2(int key, int x, int y)
{
	//	printf("%d %d\n",x,y);

	if (key == GLUT_KEY_LEFT)
	{
		for(i=0;i<nomir;i++)
		{
			if(mirf[i]==1 && mir[i][0]>(-ww/2) && mir[i][2]>(-ww/2))
			{
				mir[i][0]=mir[i][0]-m_t;
				mir[i][2]=mir[i][2]-m_t;
			}
		}
		for(i=0;i<noblo;i++)
		{
			if((blof[i]==1) && blo[i][0]>(-ww/2) && blo[i][2]>(-ww/2))
			{
				blo[i][0]=blo[i][0]-m_t;
				blo[i][2]=blo[i][2]-m_t;
			}
		}
		for(i=0;i<nopro;i++)
		{
			if(prof[i]==1 && pro[i][0]>(-ww/2) && pro[i][2]>(-ww/2))
			{
				pro[i][0]=pro[i][0]-m_t;
				pro[i][2]=pro[i][2]-m_t;
			}
		}
	}
	if (key == GLUT_KEY_RIGHT)
	{
		for(i=0;i<nomir;i++)
		{
			if(mirf[i]==1 && mir[i][0]<ww/2 && mir[i][2]<ww/2)
			{
				mir[i][0]=mir[i][0]+m_t;
				mir[i][2]=mir[i][2]+m_t;
			}
		}
		for(i=0;i<nopro;i++)
		{
			if(prof[i]==1 && pro[i][0]<ww/2 && pro[i][2]<ww/2)
			{
				pro[i][0]=pro[i][0]+m_t;
				pro[i][2]=pro[i][2]+m_t;
			}
		}
		for(i=0;i<noblo;i++)
		{
			if(blof[i]==1 && blo[i][0]<ww/2 && blo[i][2]<ww/2)
			{
				blo[i][0]=blo[i][0]+m_t;
				blo[i][2]=blo[i][2]+m_t;
			}
		}
	}
	if (key == GLUT_KEY_UP)
	{
		for(i=0;i<nomir;i++)
		{
			if(mirf[i]==1 && mir[i][1]<wh/2 && mir[i][3]<wh/2)
			{
				mir[i][1]=mir[i][1]+m_t;
				mir[i][3]=mir[i][3]+m_t;
			}
		}
		for(i=0;i<nopro;i++)
		{
			if(prof[i]==1 && pro[i][1]<wh/2 && pro[i][3]<wh/2)
			{
				pro[i][1]=pro[i][1]+m_t;
				pro[i][3]=pro[i][3]+m_t;
			}
		}
		for(i=0;i<noblo;i++)
		{
			if(blof[i]==1 && blo[i][1]<wh/2 && blo[i][3]<wh/2)
			{
				blo[i][1]=blo[i][1]+m_t;
				blo[i][3]=blo[i][3]+m_t;
			}
		}
	}
	if (key == GLUT_KEY_DOWN)
	{
		for(i=0;i<nomir;i++)
		{
			if(mirf[i]==1 && mir[i][1]>(-wh/2) && mir[i][3]>(-wh/2))
			{
				mir[i][1]=mir[i][1]-m_t;
				mir[i][3]=mir[i][3]-m_t;
			}
		}
		for(i=0;i<nopro;i++)
		{
			if(prof[i]==1 && pro[i][1]>(-wh/2) && pro[i][3]>(-wh/2))
			{
				pro[i][1]=pro[i][1]-m_t;
				pro[i][3]=pro[i][3]-m_t;
			}
		}
		for(i=0;i<noblo;i++)
		{
			if(blof[i]==1 && blo[i][1]>(-wh/2) && blo[i][3]>(-wh/2))
			{
				blo[i][1]=blo[i][1]-m_t;
				blo[i][3]=blo[i][3]-m_t;
			}
		}
	} 
}

void handleMouseclick(int button, int state, int x, int y)
{
	x-=ww/2;
	y-=wh/2;
	y=y*(-1);

	x_curr=x;
	y_curr=y;
	//	printf("%d %d\n",x,y);
	if (state == GLUT_DOWN)
	{
	}
}

void handleMotion(int x,int y)
{
	x-=ww/2;
	y-=wh/2;
	y=y*(-1);
	x_curr=x;
	y_curr=y;
	//	printf("%d %d\n",x,y);
}
void handlePassiveMotion(int x,int y)
{
	x-=ww/2;
	y-=wh/2;
	y=y*(-1);
	x_curr=x;
	y_curr=y;
	//	printf("%d %d\n",x,y);
}
void cal_procenter()
{
	//	xold=mir[0][2];
	//	yold=mir[0][3];
	//	mir[0][2]=mir[0][2]*matrix[0]+mir[0][3]*matrix[4]+matrix[12];
	//	mir[0][3]=mir[0][2]*matrix[1]+mir[0][3]*matrix[5]+matrix[13];
	int i;
	float x3,y3,m,c,k;
	//	printf("%d\n",nopro);
	for(i=0;i<nopro;i++)
	{
		x3=(pro[i][0]+pro[i][2])/2;
		y3=(pro[i][1]+pro[i][3])/2;
		int flag=0;
		if(pro[i][2]==pro[i][0])
		{
			pro[i][6]=x3-pro[i][4];
			pro[i][7]=y3;
			flag=1;
		}
		if(pro[i][3]==pro[i][1])
		{
			pro[i][6]=x3;
			pro[i][7]=y3-pro[i][4];
			flag=1;
		}
		if (flag==0)
		{
			m=(pro[i][3]-pro[i][1])/(pro[i][2]-pro[i][0]);
			//		printf("In fn: %f %f %f\n",x3,y3,m);
			c=-(1/m);
			k=sqrt((pro[i][4]*pro[i][4])/(1+(c*c)));
			pro[i][6]=-k+x3;
			pro[i][7]=-c*(-pro[i][6]+x3)+y3;
		}
		int j;
		//		printf("in fn %f\n",pro[i][5]);
		for(j=0;j<pro[i][5];j++)
		{
			//		printf("%f %d\n",pro[i][5],j);
			ray[i][j]=(l_ray *)malloc(sizeof(l_ray));
			l_ray * temp = ray[i][j];
			temp->p[0]=pro[i][6];
			temp->p[1]=pro[i][7];
			temp->next=(l_ray *)malloc(sizeof(l_ray));
			temp=temp->next;
			temp->p[0]=pro[i][0]+(((pro[i][2]-pro[i][0])/(pro[i][5]-1))*j);
			temp->p[1]=pro[i][1]+(((pro[i][3]-pro[i][1])/(pro[i][5]-1))*j);
			if (gaze_flag==1)
			{
				temp->next=(l_ray *)malloc(sizeof(l_ray));
				float rrx,rry,tslop;
				tslop = atan2((y_curr-temp->p[1]),(x_curr-temp->p[0]));
				//	tslop=atan(tslop);
				rrx = temp->p[0]+2*cos(tslop);
				rry = temp->p[1]+2*sin(tslop);
				temp=temp->next;
				temp->p[0]=rrx;
				temp->p[1]=rry;
			}
			temp->next=NULL;
			//		printf("%f %f\n",ray[i][j]->next->p[0],ray[i][j]->next->p[1]);		

			l_ray * temp2=ray[i][j];
			if (gaze_flag==1)
				find_next(temp2->next);
			else 
				find_next(temp2);

		}
	}
	//	mir[0][2]=xold;
	//	mir[0][3]=yold;
}

void find_next(l_ray * temp)
{

	int i;
	nocp=1;
	//	printf("find next called\n");
	temp->next->next=(l_ray *)malloc(sizeof(l_ray));
	for(i=0;i<nobou;i++)
	{
		//		printf("in f %d %d\n",nobou,i);
		if(find_intersection(bou[i][0],bou[i][1],bou[i][2],bou[i][3],temp->p[0],temp->p[1],temp->next->p[0],temp->next->p[1])==1)
		{
			//		printf("YES %d\n",i);
			if( find_distance(temp,xint,yint)>find_distance(temp->next,xint,yint) && find_distance(temp->next,temp->p[0],temp->p[1])<find_distance(temp,xint,yint))
			{

				if (((xint>=bou[i][0] && xint<=bou[i][2])||(xint<=bou[i][0] && xint>=bou[i][2])) &&  ((yint>=bou[i][1] && yint<=bou[i][3])|| (yint<=bou[i][1] && yint>=bou[i][3])))
				{
					cp[nocp][0]=xint;
					cp[nocp][1]=yint;
					cp[nocp][2]=-1;
					nocp++;
					/*
					   printf("%d\n",i);
					   temp=temp->next->next;
					   temp->p[0]=xint;
					   temp->p[1]=yint;
					   temp->next=NULL;
					   return;
					 */
				}
			}
		}
	}
	for(i=0;i<nomir;i++)
	{
		if(find_intersection(mir[i][0],mir[i][1],mir[i][2],mir[i][3],temp->p[0],temp->p[1],temp->next->p[0],temp->next->p[1])==1)
		{
			if( find_distance(temp,xint,yint)>find_distance(temp->next,xint,yint) && find_distance(temp,xint,yint)>1 && find_distance(temp->next,xint,yint)>1 && find_distance(temp->next,temp->p[0],temp->p[1])<find_distance(temp,xint,yint))// && xint>=mir[i][0] && xint<=mir[i][2] && yint>=mir[i][1] && yint<=mir[i][3])
			{
				{
					if (((xint>=mir[i][0] && xint<=mir[i][2])||(xint<=mir[i][0] && xint>=mir[i][2])) &&  ((yint>=mir[i][1] && yint<=mir[i][3])|| (yint<=mir[i][1] && yint>=mir[i][3])))
					{
						cp[nocp][0]=xint;
						cp[nocp][1]=yint;
						cp[nocp][2]=i;
						nocp++;
					}
				}
			}
		}
	}
	for(i=0;i<noblo;i++)
	{
		if(find_intersection(blo[i][0],blo[i][1],blo[i][2],blo[i][3],temp->p[0],temp->p[1],temp->next->p[0],temp->next->p[1])==1)
		{
			//			printf("int e %d\n",i);
			if( find_distance(temp,xint,yint)>find_distance(temp->next,xint,yint) && find_distance(temp->next,temp->p[0],temp->p[1])<find_distance(temp,xint,yint))
			{
				//				printf("bb %d\n",i);
				if (((xint>=blo[i][0] && xint<=blo[i][2])||(xint<=blo[i][0] && xint>=blo[i][2])) &&  ((yint>=blo[i][1] && yint<=blo[i][3])|| (yint<=blo[i][1] && yint>=blo[i][3])))
				{
					//					printf("blo %d %f %f\n",i,xint,yint);
					cp[nocp][0]=xint;
					cp[nocp][1]=yint;
					cp[nocp][2]=-1;
					nocp++;
				}
			}
		}
	}
	float min=1000000.0;
	int min_ind=-1;
	for(i=1;i<nocp;i++)
	{
		float t_d;
		t_d=find_distance(temp->next,cp[i][0],cp[i][1]);
		if (t_d<min)
		{
			//	printf("%f %d\n",min,i);
			min=t_d;
			min_ind=i;
		}
	}
	int id;
	id = (int) cp[min_ind][2];
	//	printf("%d %d %f %f\n",min_ind,nocp,cp[min_ind][0],cp[min_ind][1]);
	l_ray * t3;
	t3=temp;
	temp=temp->next->next;
	temp->p[0]=cp[min_ind][0];
	temp->p[1]=cp[min_ind][1];
	/*
	   if (cp[min_ind][2]==1)
	   {
	   temp->next=(l_ray *)malloc(sizeof(l_ray));
	   temp=temp->next;
	   temp->p[0]=0;
	   temp->p[1]=0;
	   }
	 */
	temp->next=NULL;
	//	printf("jefj \n");
	if (id!=-1)
	{
		//		printf("%d %d\n",id,find_side(mir[id][0],mir[id][1],mir[id][2],mir[id][3],t3->next->p[0],t3->next->p[1]));
		if (find_side(mir[id][0],mir[id][1],mir[id][2],mir[id][3],t3->next->p[0],t3->next->p[1]) == 1)
		{
			float aa,cc;
			aa= (mir[id][3]-mir[id][1])/(mir[id][2]-mir[id][0]);
			cc = mir[id][1] - aa*mir[id][0];
			//			dd = (t3->next->p[0] + (t3->next->p[1]-cc)*aa)/(1 + aa*aa);
			//	printf("%f %f\n",dd,aa);
			l_ray * t4=(l_ray *)malloc(sizeof(l_ray));
			//			t4->p[0]=2*dd - t3->next->p[0];
			//			t4->p[1]=2*dd*aa - t3->next->p[1] + 2*cc;

			t4->p[0] = (2*t3->next->p[1] - 2*cc -t3->next->p[0]*(aa-1/aa))/(aa+1/aa);	
			t4->p[1]= aa*(t4->p[0] + t3->next->p[0] ) + 2*cc - t3->next->p[1];
			ppx=t4->p[0];
			ppy=t4->p[1];
			//	drawLine(ppx,ppy,0,0,0.0f,0.0f,0.0f);
			t4->next=t3->next->next;
			//		drawLine(t4->p[0],t4->p[1],t4->next->p[0],t4->next->p[1],0.0,0.84,0.0);
			//	printf("%f %f\n",ppx,ppy);
			find_next(t4);
		}
	}

}

float find_distance(l_ray *t1,float x,float y)
{
	float ans;
	ans=((y-t1->p[1])*(y-t1->p[1]))+((x-t1->p[0])*(x-t1->p[0]));
	return sqrt(ans);
}

bool find_intersection(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
	float c1,c2,m1,m2;

	if(abs(x2-x1)<0.0001 && abs(x3-x4)<0.0001)
		return 0;

	if(abs(x2-x1)<0.0001)
	{
		m2=(y4-y3)/(x4-x3);	
		c2=y3-(m2*x3);
		xint=x2;
		yint=(m2*xint)+c2;

		return 1;
	}
	if(abs(x3-x4)<0.0001)
	{
		m1=(y2-y1)/(x2-x1);
		c1=y1-(m1*x1);
		xint=x3;
		yint=(m1*xint)+c1;
		return 1;
	}
	m1=(y2-y1)/(x2-x1);
	m2=(y4-y3)/(x4-x3);

	//printf("in fn: %f %f\n",m1,m2);
	if (m1==m2)
		return 0;
	c1=y1-(m1*x1);
	c2=y3-(m2*x3);
	xint=(c2-c1)/(m1-m2);
	yint=(m1*xint)+c1;
	float t1,t2,t3;
	t1=(y3-yint)*(y3-yint) + (x3-xint)*(x3-xint);
	t2=(y3-y4)*(y3-y4) + (x3-x4)*(x3-x4);
	t3=(y4-yint)*(y4-yint) + (x4-xint)*(x4-xint);	
	t1=t1*100;
	t2=t2*100;
	t3=t3*100;
	int f1,f2,f3;
	f1 = (int)t1;
	f2 = (int)t2;
	f3 = (int)t3;
	if (f1+f3-f2<1)
		return 0;

	return 1;
}

bool find_side(float x1,float y1,float x2,float y2,float a,float b)
{
	float ans;
	ans=0;
	ans += ( x1* (y2-b));
	ans -= ( y1 * (x2-a));
	ans += (x2*b) - (y2*a);
	if (ans<0)
		return 1;
	else 
		return 0;
}

void findline()
{
	    // to find a line between the files !

	    while(fgets(line,sizeof line,pFile)){
		       //     printf("%s",s);
		           if(strcmp(line,"\n")!=0&&line[0]!='#')
				               break;
			       }
}
