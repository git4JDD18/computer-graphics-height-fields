#include "stdafx.h"
#include <pic.h>
#include <windows.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <GL/glut.h>


int g_iMenuId;

int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;


typedef float GLfloat;
int type;
int i;
int j;
int MARK =0;
int imageCounter;
GLfloat x, y, h, h2;
GLfloat diffuseMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };



typedef enum { ROTATE, TRANSLATE, SCALE } CONTROLSTATE;

CONTROLSTATE g_ControlState = ROTATE;

/* state of the world */
float g_vLandRotate[3] = {1.0, 1.0, 1.0};
float g_vLandTranslate[3] = {-0.50, -1.0, 0.5};
//float g_vLandTranslate[3] = {-1.0, -1.0, 0.0};
float g_vLandScale[3] = {1.5, 1.5, 1.5};

/* see <your pic directory>/pic.h for type Pic */
Pic * g_pHeightData;
Pic * inputImage2;

void color(float *c, int x, int y)
{
	
    int channel = inputImage2->bpp;
	if(channel == 3)
	{
	  c[0] = PIC_PIXEL(inputImage2, x, y, 0)/255.0;
	  c[1] = PIC_PIXEL(inputImage2, x, y, 1)/255.0;
	  c[2] = PIC_PIXEL(inputImage2, x, y, 2)/255.0;
	}
	else if(channel == 1)
	{ 
	  float temp = PIC_PIXEL(inputImage2, x, y, 0)/255.;
		c[0] = temp;
		c[1] = temp;
		c[2] = temp;
	}
   
}
/* Write a screenshot to the specified filename */
void saveScreenshot (char *filename)
{
  int i, j;
  Pic *in = NULL;

  if (filename == NULL)
    return;

  /* Allocate a picture buffer */
  in = pic_alloc(640, 480, 3, NULL);

  printf("File to save to: %s\n", filename);

  for (i=479; i>=0; i--) {
    glReadPixels(0, 479-i, 640, 1, GL_RGB, GL_UNSIGNED_BYTE,
                 &in->pix[i*in->nx*in->bpp]);
  }

  if (jpeg_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");

  pic_free(in);
}

//saves screenshot 
void shot()
{
	char frame[30];

	sprintf(frame,"%.3d.jpg",imageCounter);

	if (imageCounter<=300)
	imageCounter++;
	else
	return;

	saveScreenshot(frame);

}


/*void single_light()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = {25.0};
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lmodel_ambient[] = {0.1, 0.1, 0.1, 1.0};

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

}*/

void double_light_source()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = {5.0};
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lmodel_ambient[] = {0.1, 0.1, 0.1, 1.0};

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);


	GLfloat light1_ambient[] = {0.3, 0.3, 0.2, 0.2};
	GLfloat light1_diffuse[] = {2.0, 0.0, 0.0, 1.0};
	GLfloat light1_specular[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat light1_position[] = {-2.0, 2.0, 1.0, 1.0};
	GLfloat spot_direction[] = {-1.0, -1.0, 0.0};
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	
}



void myinit()
{
	/* setup gl view here */
	
	double_light_source();
	
	//glClearColor (0.0, 0.0, 0.0, 0.0);
	
	//glEnable(GL_DEPTH_TEST);
	/* glShadeModel (GL_FLAT); */
	//glShadeModel (GL_SMOOTH);

	
}


//Function reshape window
void Reshape(int w, int h)
{
	
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60.0, (GLfloat) w/ (GLfloat) h, 0.01, 1000.0);
	glMatrixMode(GL_MODELVIEW);

}


void triangles()
{

	//glBegin(GL_TRIANGLE_STRIP);
	for(i = 0; i < 511; i++)
	{
		x =(GLfloat) i;
		for(j = 0; j < 512; j++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			//x =(GLfloat) i;
			y =(GLfloat) j;
			h = (GLfloat) PIC_PIXEL(g_pHeightData, i, j, 0);
			
			glColor3f(h, ((1.0)*(h/32)), (1.0*(h/32)));
			//glColor3f(0.0, h/512 ,h/512);
			glVertex3f(x/512, y/512, h/512);


			h = (GLfloat) PIC_PIXEL(g_pHeightData, i+1, j, 0);
			glColor3f(h, ((1.0)*(h/32)), (1.0*(h/32)));
			//glColor3f(0.0,1.0,0.0);
			glVertex3f((x+1)/512, y/512, h/512);

			/*h = (GLfloat) PIC_PIXEL(g_pHeightData, i, j+1, 0);
			glColor3f(0.0, h/256, h/256);
			glVertex3f(x/256, (y+1)/256, h/256);

			h = (GLfloat) PIC_PIXEL(g_pHeightData, i+1, j+1, 0);
			glColor3f(0.0, h/256, h/256);
			glVertex3f((x+1)/256, (y+1)/256, h/256);

			*/

			//glEnd();
		}
		 glEnd();
	}

}


void points()
{
	for (i = 0; i < 511; i++)
	{
	
		for (j = 0; j < 512; j++)
		{
			glBegin(GL_POINTS);
			x = (GLfloat) i;
			y = (GLfloat) j;
			h = (GLfloat) PIC_PIXEL(g_pHeightData, i, j, 0);

			if(h < 50)
			{
				h = h/2.0;
				glColor3f(0.0, 0.0, 0.8);
				glVertex3f(x/512, y/512, h/512);
			}


			if(h <= 80)
			{
				glColor3f(0.0, 0.8, 0.0);
				glVertex3f(x/512, y/512, h/512);
			}


			if(h <= 120)
			{
				glColor3f(1.0, 0.5, 0.1);
				glVertex3f(x/512, y/512, h/512);
			}

			if(h > 120)
			{
				h = h* 1.5;
				glColor3f(h, ((1.0)*(h/16)), (1.0*(h/16)));
				//glColor3f(1.0, 1.0, 1.0);
				glVertex3f(x/512, y/512, h/512);
			
			}
		}
		glEnd();
	}
		
}

void Color_based_on_another_image()
{
	float c[3];
	for (i = 0; i < 511; i++)
	{
	
		for (j = 0; j < 512; j++)
		{
			glBegin(GL_POINTS);
			x = (GLfloat) i;
			y = (GLfloat) j;
			h = (GLfloat) PIC_PIXEL(g_pHeightData, i, j, 0);

			color(c,x,y);
			glColor3f(c[0],c[1],c[2]);
			//glVertex3f((GLfloat)i,(GLfloat)j,h);
			glVertex3f(x/512, y/512, h/512);
			glEnd();
		}

	}
/*	grey = PIC_PIXEL(g_pHeightData, n, m, 0);
  c2[0] = PIC_PIXEL(g_pHeightData, n, m, 0)/255.;
  c2[1] = PIC_PIXEL(g_pHeightData, n, m, 1)/255.;
  c2[2] = PIC_PIXEL(g_pHeightData, n, m, 2)/255.;
  glColor3f(c2[0],c2[1],c2[2]);
  height = (0.299*c2[0] + 0.587*c2[1] + 0.114*c2[2])*255.;
  glVertex3f((GLfloat)n,(GLfloat)m,height); 
  */


}


void lines()
{
	for (i = 0; i < 511; i++)
	{
		x = (GLfloat) i;
		glBegin(GL_LINES);
		for (j = 0; j < 512; j++)
		{
			y = (GLfloat) j;
			h = (GLfloat) PIC_PIXEL(g_pHeightData, i, j, 0);
			glColor3f(h/32, 0.0, h/128);
			glVertex3f(x/512, y/512, h/512);
	  
			h = (GLfloat) PIC_PIXEL(g_pHeightData, i+1, j, 0);
			glColor3f(0.0, h/512, 1.0);
			glVertex3f((x+1.0)/512, y/512, h/512);

		
		}
	glEnd();
	}	
}

void lines_wire_frame()
{
	// Push the GL attribute bits so that we don't wreck any settings
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	// Enable polygon offsets, and offset filled polygons forward by 2.5
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( -2.5f, -2.5f );
	// Set the render mode to be line rendering with a thick line width
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glLineWidth( 3.0f );
	// Set the colour to be white
	glColor3f( 1.0f, 1.0f, 1.0f );
	// Render the object
	for (i = 0; i < 511; i++)
	{
		x = (GLfloat) i;
		glBegin(GL_LINES);
		for (j = 0; j < 512; j++)
		{
			y = (GLfloat) j;
			h = (GLfloat) PIC_PIXEL(g_pHeightData, i, j, 0);
			//glColor3f(h, ((1.0)*(h/16)), (1.0*(h/16)));
			glColor3f(h/128,h/128, 0.0);
			glVertex3f(x/512, y/512, h/512);
	  
			h = (GLfloat) PIC_PIXEL(g_pHeightData, i+1, j, 0);
			glColor3f(0.0,h/128, 1.0 );
			glVertex3f((x+1.0)/512, y/512, h/512);

		
		}
		glEnd();
	}
	// Set the polygon mode to be filled triangles 
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_LIGHTING );
	// Set the colour to the background
	glColor3f( 0.0f, 0.0f, 0.0f );
	
	// Pop the state changes off the attribute stack
	// to set things back how they were
	glPopAttrib();


}

/* idle callback */

void display()
{
	/* draw 1x1 cube about origin */
	/* replace this code with your height field implementation */
	/* you may also want to precede it with your 
	rotation/translation/scaling */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glClear (GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0,0,3,0,0,-100, 0,1,0);

	glTranslatef(g_vLandTranslate[0], g_vLandTranslate[1], g_vLandTranslate[2]);

	glScalef(g_vLandScale[0], g_vLandScale[1], g_vLandScale[2]);


	glRotatef(g_vLandRotate[0], 0.0, 0.0, 1.0);
	glRotatef(g_vLandRotate[1], 0.0, 1.0, 0.0);
	glRotatef(g_vLandRotate[2], 1.0, 0.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
  
	//glRotatef(3.0,2.0,1.0,0.0);
	//glTranslatef(0.0,0.0,-6);
	//glRotatef(45.0,0.0,1.0,1.0);
	//glRotatef(45.0,1.0,1.0,1.0);
	//glTranslatef(-1.0,-1.5,0.0);
	//glScalef(2.0,3.0,2.0);


	/*glBegin(GL_POLYGON);

		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(-0.5, -0.5, 0.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-0.5, 0.5, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(0.5, 0.5, 0.0);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(0.5, -0.5, 0.0);

	glEnd();
	*/
	
	switch (type)
	{
		case 1:
			triangles();
			glFlush();
			break;

		case 2:
			lines();
			glFlush();
			break;

		case 3:
			points();
			glFlush();
			break;
			
		case 4:
			// Draw the 3d-image with triangles
			lines_wire_frame();
			glFlush();
			break;
		case 5:
			Color_based_on_another_image();
			glFlush();
			break;

		case 6:exit(0);
			break;
	}
	
		
	//triangles();
	//lines();
	//points();
	glFlush();
	glutSwapBuffers();

}


void menufunc(int value)
{
  switch (value)
  {
	case 1:
		type=1;
		glutPostRedisplay();
		break;

	case 2:
	    type=2;
		glutPostRedisplay();	
		break;

	case 3:
		type=3;
		glutPostRedisplay();	
		break;

	case 4:
		type=4;
		glutPostRedisplay();
		break;

	case 5:
		type=5;
		glutPostRedisplay();
		break;

	case 6:
	    exit(0);
		break;
	}

}


void doIdle()
{
  /* do some stuff... */
	shot();
	/* display result (do not forget this!) */
	glutPostRedisplay();
	
}

/* converts mouse drags into information about 
rotation/translation/scaling */
void mousedrag(int x, int y)
{
  int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
  
  switch (g_ControlState)
  {
    case TRANSLATE:  
      if (g_iLeftMouseButton)
      {
        g_vLandTranslate[0] += vMouseDelta[0]*1.0;
        g_vLandTranslate[1] -= vMouseDelta[1]*1.0;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandTranslate[2] += vMouseDelta[1]*0.01;
      }
      break;
    case ROTATE:
      if (g_iLeftMouseButton)
      {
        g_vLandRotate[0] += vMouseDelta[1];
        g_vLandRotate[1] += vMouseDelta[0];
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandRotate[2] += vMouseDelta[1];
      }
      break;
    case SCALE:
      if (g_iLeftMouseButton)
      {
        g_vLandScale[0] *= 1.0+vMouseDelta[0]*0.01;
        g_vLandScale[1] *= 1.0-vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandScale[2] *= 1.0-vMouseDelta[1]*0.01;
      }
      break;
  }
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mouseidle(int x, int y)
{
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mousebutton(int button, int state, int x, int y)
{

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
      g_iLeftMouseButton = (state==GLUT_DOWN);
	  if (state == GLUT_DOWN) 
	  {       /*  change red  */
            diffuseMaterial[0] += 0.1;
            if (diffuseMaterial[0] > 1.0)
               diffuseMaterial[0] = 0.0;
            glColor4fv(diffuseMaterial);
            glutPostRedisplay();
      }
      break;
    case GLUT_MIDDLE_BUTTON:
      g_iMiddleMouseButton = (state==GLUT_DOWN);
	  if (state == GLUT_DOWN) 
	  {       /*  change green  */
            diffuseMaterial[1] += 0.1;
            if (diffuseMaterial[1] > 1.0)
               diffuseMaterial[1] = 0.0;
            glColor4fv(diffuseMaterial);
            glutPostRedisplay();
       }
      break;
    case GLUT_RIGHT_BUTTON:
      g_iRightMouseButton = (state==GLUT_DOWN);
	  if (state == GLUT_DOWN) 
	  {      /*  change blue  */
            diffuseMaterial[2] += 0.1;
            if (diffuseMaterial[2] > 1.0)
               diffuseMaterial[2] = 0.0;
            glColor4fv(diffuseMaterial);
            glutPostRedisplay();
      }
      break;
  }
 
  switch(glutGetModifiers())
  {
    case GLUT_ACTIVE_CTRL:
      g_ControlState = TRANSLATE;
      break;
    case GLUT_ACTIVE_SHIFT:
      g_ControlState = SCALE;
      break;
    default:
      g_ControlState = ROTATE;
      break;
  }

  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}


/*void keyboard(unsigned char key, int x, int y)
{
	if((key == 'a')||(key == 'A'))
	{
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}
		
	if((key == 's')||(key == 'S'))
	{
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}
	if((key == 'd')||(key == 'd'))
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}
	if(key == 27)
		exit(0);
}*/

int _tmain(int argc, _TCHAR* argv[])
{
	// I've set the argv[1] to spiral.jpg.
	// To change it, on the "Solution Explorer",
	// right click "assign1", choose "Properties",
	// go to "Configuration Properties", click "Debugging",
	// then type your texture name for the "Command Arguments"
	if (argc<2)
	{  
		printf ("usage: %s heightfield.jpg\n", argv[0]);
		exit(1);
	}

	g_pHeightData = jpeg_read((char*)argv[1], NULL);
	printf("g_pHeightData [%s]",argv[1]);
	
	if (!g_pHeightData)
	{
	    printf ("error reading %s.\n", argv[1]);
	    exit(1);
	}

	inputImage2 = jpeg_read((char*)argv[2], NULL);
	printf("inputImage2 [%s]",argv[2]);
	
	if (!inputImage2)
	{
	    printf ("error reading %s.\n", argv[2]);
	    exit(2);
	}
	
	printf("%d,%d,%d\n",g_pHeightData->nx,g_pHeightData->ny,g_pHeightData->bpp);
	printf("%d",PIC_PIXEL(g_pHeightData,0,0,0));
	
	


	//system("PAUSE");
	glutInit(&argc,(char**)argv);
  
	/*
		create a window here..should be double buffered and use depth testing
  
	    the code past here will segfault if you don't have a window set up....
	    replace the exit once you add those calls.
	*/

	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(700, 600);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Height Field");
	/* double buffering for smooth animation */
		
		
	//exit(0);

	/* tells glut to use a particular display function to redraw */
	glutDisplayFunc(display);
  
	/* allow the user to quit using the right mouse button menu */
	g_iMenuId = glutCreateMenu(menufunc);
	glutSetMenu(g_iMenuId);

	glutAddMenuEntry("Triangles",1);
	glutAddMenuEntry("Lines",2);
	glutAddMenuEntry("Points",3);
	glutAddMenuEntry("Lines with WireFrame",4);
	glutAddMenuEntry("Color_based_on_another_image",5);
	glutAddMenuEntry("Quit",6);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	/* replace with any animate code */
	glutIdleFunc(doIdle);

	/* callback for mouse drags */
	glutMotionFunc(mousedrag);
	/* callback for idle mouse movement */
	glutPassiveMotionFunc(mouseidle);
	/* callback for mouse button changes */
	glutMouseFunc(mousebutton);

	/* do initialization */
	myinit();
	/* callback for reshape*/
	glutReshapeFunc(Reshape);
	 /* enable hidden-surface removal */
	//glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
