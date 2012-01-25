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
#include "../NeuralNet/NeuralNetwork.h"
#include "../NeuralNet/ChangeContainer.h"

using namespace std;

// Initial size of graphics window.
const int WIDTH  = 1200;
const int HEIGHT = 600;

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

void GenerateShapes(Rect* bounds, int index);


NeuralNetwork * nnet;

char* filename = "nnet.bin";

vector<DrawableObject*> objects[8];
vector<Rect*> rectangles;
vector<ChangeContainer*> changes;

float inputArray[16];

float randFloat()
{
	return (float)rand() / RAND_MAX;
}

float origArray[8];

void GenerateMutations()
{
	for (int i = 0; i < changes.size(); ++i)
	{
		delete changes[i];
	}
	changes.clear();
	for (int j = 0; j < 8; ++j)
	{
		for (int i = 0; i < objects[j].size(); ++i)
		{
			delete objects[j][i];
		}
		objects[j].clear();
	}
	for (int i = 0; i < 8; ++i)
	{
		ChangeContainer* changeContainer = nnet->Mutate(400);
		changes.push_back(changeContainer);
		nnet->ApplyChanges(changeContainer);
		GenerateShapes(rectangles[i], i);
		nnet->Revert(changeContainer);
	}
}

void setup()
{
	srand(time(NULL));
	if (!(nnet = NeuralNetwork::LoadFromFile(filename)))
		nnet = new NeuralNetwork(8, 80, 10, 50);
	float fw = (float)width;
	float fh = (float)height;
	for (int i = 0; i < 16; ++i)
	{
		origArray[i] = randFloat();
		inputArray[i] = origArray[i];
	}
	for (int i = 0; i < 4; ++i)
	{
		Vec2 extents(300.0f, 300.0f);
		Vec4 colour(0.7f, 0.7f, 0.7f, 0.0f);
		Rect* rect = new Rect(Vec2(150.0f + (i * 300.0f), 150.0f), extents, colour); 
		rectangles.push_back(rect);
		rect = new Rect(Vec2(150.0f + (i * 300.0f), 450.0f), extents, colour);
		rectangles.push_back(rect);
	}
	GenerateMutations();
}

void display ()
{
 	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);
	for (int j = 0; j < 8; ++j)
	{
		//rectangles[j]->SetScissor();
		for (int i = 0; i < objects[j].size(); ++i)
		{
			objects[j][i]->Draw();
		}
		rectangles[j]->DrawBorder();
	}
	glDisable(GL_SCISSOR_TEST);
	glutSwapBuffers();
}

void GenerateShapes(Rect* bounds, int index)
{
	float* output = nnet->Compute(origArray);	
	Vec2 offset = bounds->GetCentre() - bounds->GetExtents();
	float width = bounds->GetExtents()[0] * 2.0f;
	float height = bounds->GetExtents()[1] * 2.0f;
	for (int i = 0; i < 10; ++i)
	{
		int ptrOffs = i * 8;
		objects[index].push_back(new Rect(Vec2(output[ptrOffs] * width,output[ptrOffs + 1] * height) + offset, Vec2(output[ptrOffs + 2] * width, output[ptrOffs + 3] * height), \
		Vec4(output[ptrOffs + 4], output[ptrOffs + 5], output[ptrOffs + 6], output[ptrOffs + 7])));
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

void mouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i < rectangles.size(); ++i)
		{
			if (rectangles[i]->Intersects(Vec2((float)x, (float)y)))
			{
				nnet->ApplyChanges(changes[i]);
				GenerateMutations();
				break;
			}
		}
	}
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
	glOrtho(0.0, (double)width, (double)height, 0.0, 0.0, 1.0);	
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
	case 27:
		exit(0);
		break;
	case 'm':
		nnet->ApplyChanges(nnet->Mutate(3000));
		GenerateMutations();
		break;
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

void clean()
{
	nnet->SaveToFile(filename);
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
	glutMouseFunc(mouseClick);
	glutIdleFunc(idle);

	// OpenGL initialization
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	// Display help.
	help();

	// Enter GLUT loop.

	setup();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	atexit(clean);
	glutMainLoop();
}
