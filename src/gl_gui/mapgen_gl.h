#ifndef CGPROJECT_H
#define CGPROJECT_H




#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>

#ifdef _WIN32
#include "C:\MinGW\include\GL\glut.h"
#else
#include <GL/glut.h>
#endif



#ifndef	M_PI				// for Windows
#define M_PI 3.14159265358979323846
#endif

#ifndef SGN					// Signum
#define SGN(y) (((y) < 0) ? -1 : ((y) > 0))
#endif

#ifndef MIN
#define MIN(a,b) ((a) > (b))? (b) : (a)
#endif

#ifndef MAX
#define MAX(a,b) ((a) > (b))? (a) : (b)
#endif


//////////////////////////////////////////////////////////////
//	Fenster Initialisierung
//////////////////////////////////////////////////////////////
//! Die Startposition des Fensters
#define WIN_POS_X	0
#define WIN_POS_Y	0



//! Breite des Fensters
#define WIN_WIDTH	1024
//! Hoehe des Fensters
#define WIN_HEIGHT	768


// Uncomment, wenn man Stencil Buffer braucht
//#define NEED_STENCIL

// Uncomment, wenn man Accumulation Buffer braucht
//#define NEED_ACCUM


//////////////////////////////////////////////////////////////
//	OpenGL Darstellungsmodus
//////////////////////////////////////////////////////////////
//! Default OpenGL Modus:  RGBA mit double Buffering und
//! depth buffer (Z-Buffer)
//! Verwendung der Stencil und Accumulation Buffers
#if defined NEED_STENCIL && defined NEED_ACCUM
#define USED_MODUS	GLUT_RGBA |  GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL | GLUT_ACCUM
#elif defined NEED_STENCIL
#define USED_MODUS	GLUT_RGBA |  GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL
#elif defined NEED_ACCUM
#define USED_MODUS	GLUT_RGBA |  GLUT_DEPTH | GLUT_DOUBLE | GLUT_ACCUM
#else
#define USED_MODUS	GLUT_RGBA |  GLUT_DEPTH | GLUT_DOUBLE
#endif


/////////////////////////////////////////////////////////////
//	Texturdefinition
/////////////////////////////////////////////////////////////
#define MAX_TEXTURES 	50
#define READ_TEXTURES	0

/////////////////////////////////////////////////////////////
//	Menuelemente
/////////////////////////////////////////////////////////////
#define MENU_OPEN			"Open"
#define MENU_SHADE			"Shading"
#define MENU_EXIT			"Exit"

enum MENU_IDs {
	ID_MENU_OPEN = 1,
 ID_MENU_SHADE,
 ID_MENU_EXIT
};

#include "matrix2d.h"

//////////////////////////////////////////////////////////////
//	Programanhaengige Konstanten und Variablen
/////////////////////////////////////////////////////////////
#define PROG_NAME	"Gradient"

int MainWin;		// Identifier vom Hauptfenster
int MainMenu;		// Identifier vom Hauptmenu

//! Struktur fuer Bearbeitung der Mausevents
struct MouseStruct {
	int LastState;
	int OldX;
	int OldY;
	int ScreenX;
	int ScreenY;
	int MoveX;
	int MoveY;
	double Radius;
} globMouse;



/////////////////////////////////////////////////////////////
//	Funktionen und Prozeduren
/////////////////////////////////////////////////////////////
void DisplayFunc ();
void ReshapeFunc (int,int);
void MouseFunc (int,int,int,int);
void SetCamera();
void KeyboardFunc (unsigned char,int,int);
void TimerFunc(int dummy);




#endif
