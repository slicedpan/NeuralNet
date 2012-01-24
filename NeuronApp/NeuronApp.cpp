// Skeleton program for GLUT applications.

// Link with: opengl32.lib, glu32.lib, glut32.lib.

#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include <vector>
#include "DrawableObject.h"
#include "Rect.h"
#include "NeuralNetwork.h"

using namespace std;

// Initial size of graphics window.
const int WIDTH  = 1200;
const int HEIGHT = 800;

// Current size of window.
int width  = WIDTH;
int height = HEIGHT;

// Mouse positions, normalized to [0,1].
double xMouse = 0.5;
double yMouse = 0.5;

// Bounds of viewing frustum.
double nearPlane =  5;
double farPlane  = 15;

// Viewing angle.
double fovy = 40.0;

// Variables.
double alpha = 0;                                  // Set by idle function.
double beta = 0;                                   // Set by mouse X.
double ldistance = - (farPlane - nearPlane) / 2;    // Set by mouse Y.

// This function is called to display the scene.

NeuralNetwork * nnet;

vector<DrawableObject*> objects;

float inputArray[16];

float randFloat()
{
	return (float)rand() / RAND_MAX;
}

float origArray[8];

void setup()
{
	srand(time(NULL));
	nnet = new NeuralNetwork(8, 8, 7, 250);
		float fw = (float)width;
	float fh = (float)height;
	for (int i = 0; i < 16; ++i)
	{
		origArray[i] = randFloat();
		inputArray[i] = origArray[i];
	}
}

void display ()
{
 	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->Draw();
	}
	glutSwapBuffers();
}

void GenerateShape()
{
	float* output = nnet->Compute(inputArray);
	float fw = (float)width;
	float fh = (float)height;
	objects.push_back(new Rect(Vec2(output[0] * fw,output[1] * fh), Vec2(output[2] * fw, output[3] * fh), Vec4(output[4], output[5], output[6], output[7])));
	
	for (int i = 0; i < 8; ++i)
	{
		inputArray[i] = output[i];

	}
	
}

void GenerateShapes()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		delete objects[i];
	}
	for (int i = 0; i < 8; ++i)
	{
		inputArray[i] = origArray[i];
	}
	objects.clear();
	for (int i = 0; i < 100; ++i)
	{
		GenerateShape();
	}
}

// This function is called when there is nothing else to do.
void idle ()
{
   const double STEP = 0.1;
   const double ALL_ROUND = 360;
   alpha += STEP;
   if (alpha > ALL_ROUND)
      alpha -= ALL_ROUND;

   // Display normalized coordinates in title bar.

	glutPostRedisplay();
}

void mouseMovement (int mx, int my)
{
   // Normalize mouse coordinates.
   xMouse = double(mx) / double(width);
   yMouse = 1 - double(my) / double(height);

   // Redisplay image.
   glutPostRedisplay();
}

// Respond to window resizing, preserving proportions.
// Parameters give new window size in pixels.
void reshapeMainWindow (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (double)width, (double)height,0.0, 0.0, 1.0);	
}

// Display help.
void help()
{
   cout << 
      "'h'   display help" << endl <<
      endl;
}

// Respond to graphic character keys.
// Parameters give key code and mouse coordinates.
void graphicKeys (unsigned char key, int x, int y)
{
	switch (key)
	{
   case 'h':
      help();
      break;
   case 'm':
	   nnet->Mutate();
	   GenerateShapes();
	   break;
   case 'b':
	   nnet->Revert();
	   GenerateShapes();
	   break;
   case 'g':
	   nnet->ReApply();
	   GenerateShapes();
	   break;
	case 27:
		exit(0);
   default:
      cout << key << endl;
	}
}

// Respond to function keys.
// Parameters give key code and mouse coordinates.
void functionKeys (int key, int x, int y)
{
   switch (key)
   {
   case GLUT_KEY_F1:
      cout << "F1" << endl;
      break;
   case GLUT_KEY_UP:
      cout << "Up" << endl;
      break;
   }
}


void main (int argc, char **argv)
{
	// GLUT initialization.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("GLUT Skeleton Program");

	// Register call backs.
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeMainWindow);
	glutKeyboardFunc(graphicKeys);
	glutSpecialFunc(functionKeys);
	glutMotionFunc(mouseMovement);
	glutIdleFunc(idle);

	// OpenGL initialization
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	// Display help.
	help();

	// Enter GLUT loop.

	setup();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutMainLoop();
}
