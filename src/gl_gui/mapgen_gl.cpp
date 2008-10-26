
#include "mapgen_gl.h"
#include "unionfindtree.h"
#include "mapgenerator.h"

using namespace std;

float point_x, point_y;

const int DIMX = 32;
const int DIMY = 32;

const int CENTERX = DIMX/2;
const int CENTERY = DIMY/2;


RegionData rdata;
MapGenerator::MapData mdata;



//
//	Resize CALLBACK Funktion - wird aufgerufen, wenn sich die 
//	Fenstergroesse aendert
//		w,h: neue Breite und Hoehe des Fensters
//
void ReshapeFunc (int w, int h) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45, (double)w/(double)h, 1, 2000);
	glViewport (0, 0, w, h);
	glMatrixMode (GL_MODELVIEW);	// Sicherheit halber 
	glLoadIdentity ();				// Modelierungsmatrix einstellen
	globMouse.ScreenX = w;			
	globMouse.ScreenY = h;
}

//
//	Maus Button CALLBACK Funktion
//		button - Welche Taste betaetigt bzw. losgelassen wurde
//		state  - Status der State (GL_DOWN, GL_UP)
//		x, y   - Fensterkoordinaten des Mauszeigers
//
void MouseFunc (int button, int state, int x, int y) {
	globMouse.LastState = (state == GLUT_DOWN) ? button : -1;
	switch (button) {
		// Linke Taste - mit Rotation verknuepft
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				globMouse.OldX = x;
				globMouse.OldY = y;
				point_y=DIMX*(globMouse.ScreenY-y)*1.0/globMouse.ScreenY-0.5;
				point_x=DIMY*(x-0.5*(globMouse.ScreenX-globMouse.ScreenY))*1.0/globMouse.ScreenY-0.5;
				
				//printf("x: %i/ %i\n",x,globMouse.ScreenX);
				//printf("y: %i/ %i\n",y,globMouse.ScreenY);
				
				}
			break;
		// Mittlere Taste - mit Skalierung verknuepft
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
				globMouse.OldY = y;
			break;
		// Rechte Taste - reserviert fuer Menue
		case GLUT_RIGHT_BUTTON:
			break;
		}
}


void SetCamera() {
	
	//gluLookAt (0,0,1.2*DIMY, 0,0,0, 0, 1, 0);
	
	glMatrixMode( GL_PROJECTION  );
	glLoadIdentity();
	gluOrtho2D( -CENTERX, CENTERX, -CENTERY, CENTERY);
	//gluOrtho2D(0,0,DIMX,DIMY);

}

//
//	Tastatur CALLBACK Funktion
//		key: Wert der Taste die gedrueckt wurde
//		x,y: Position des Mauskursors auf dem Viewport
//
void KeyboardFunc (unsigned char key, int x, int y) {
	switch (key) {
		// Escape
		case 27:
			exit (0);
			break;
			
		case 'i':
		case 'I':
			globMouse.MoveX = 0;
			globMouse.MoveY = 0;
			glutPostRedisplay ();
			break;
			
		
		}
}


void TimerFunc(int dummy) {

	
	glutTimerFunc(200000,TimerFunc,0);
	glutPostRedisplay();
}


//
//	Anfang des OpenGL Programmes
//
int main (int argc,char **argv) {
	// Fensterinitialisierung
	long sec;
	time(&sec);
	srand(time(NULL));

	glutInit (&argc,argv);
	glutInitWindowSize (960,960);
	glutInitWindowPosition (WIN_POS_X,WIN_POS_Y);
	glutInitDisplayMode (USED_MODUS);
	MainWin = glutCreateWindow (PROG_NAME);

	// OpenGL Initialisierungen
	glEnable (GL_DEPTH_TEST);
	
	
	// Callback funktion
	glutDisplayFunc (DisplayFunc);
	glutReshapeFunc (ReshapeFunc);
	glutMouseFunc (MouseFunc);
	glutKeyboardFunc (KeyboardFunc);
	//glutIgnoreKeyRepeat(1);

	// Initiale Mauseigenschaften
	globMouse.LastState = -1;
	globMouse.MoveX 	= 0;
	globMouse.MoveY 	= WIN_HEIGHT/8;
	globMouse.Radius 	= 4;

	
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
	glLineWidth(2);
	//glEnable (GL_LIGHTING);
	glEnable (GL_NORMALIZE);
	

	rdata.m_dimx = DIMX;
	rdata.m_dimy = DIMY;
	
	rdata.m_complexity = 0.4;
	rdata.m_granularity = 8;
	rdata.m_area_percent = 0.35;
	rdata.m_id = 0;
	rdata.m_name = "region0";
	
	rdata.addEnvironment(0.6,"meadow");
	rdata.addEnvironment(1.0,"hills");
	
	rdata.m_exit_directions[NORTH] = true;
	rdata.m_exit_directions[SOUTH] = true;
	rdata.m_exit_directions[WEST] = false;
	rdata.m_exit_directions[EAST] = true;
	
	rdata.addObjectGroupTemplate("trees3",4,1,1.0);
	rdata.addObjectGroupTemplate("trees2",2,3,0.7);
	rdata.addObjectGroupTemplate("trees1",1,10,0.5);
	std::cout << rdata.m_object_groups.size() << "\n";
	
	rdata.addNamedObjectGroupTemplate("test2","ort1",4);
	rdata.addNamedObjectGroupTemplate("test1","ort2",3);
	rdata.addNamedObjectGroupTemplate("test1","ort3",2);
	
	
	RegionExit exit;
	exit.m_shape.m_type = Shape::RECT;
	exit.m_shape.m_extent = Vector(4,4);
	exit.m_exit_name = "exit_south";
	exit.m_destination_region = "region1";
	exit.m_destination_location = "entry_north";
	
	rdata.addExit(exit);
	
	ObjectFactory::init();
	ObjectFactory::loadFixedObjectData("../data/objects/objects.xml");
	
	MapGenerator::createMapData(&mdata,&rdata);
	MapGenerator::createBaseMap(&mdata,&rdata);
	MapGenerator::createTemplateMap(&mdata,&rdata);
	MapGenerator::insertGroupTemplates(&mdata,&rdata);
	MapGenerator::createBorder(&mdata,&rdata);
	
	/*
	ObjectGroupTemplate* ogl = ObjectFactory::getObjectGroupTemplate("test2");
	Vector pl;
	for (int i=0; i<5; i++)
		MapGenerator::getTemplatePlace(&mdata,ogl->getShape(),pl);
	*/
	
	glutTimerFunc(20,TimerFunc,0);
	// Die Hauptschleife
	glutMainLoop ();
	
	printf("program started\n");
	return 0;
}


///////////////////////////////////////////////////////////////////////
//
//	Modellierungsfunktionen
//
///////////////////////////////////////////////////////////////////////

//
//	Display CALLBACK Funktion - wird immer aufgerufen, wenn der 
//	Inhalt des Viewports aktualisiert werden muss (z.B. nach
//	Aufruf der Funktion 'glutPostRedisplay ()').
//
void DisplayFunc () {
	// Buffer neu initialisieren
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	//glEnable(GL_CULL_FACE);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	SetCamera();
	
	float col[3];
	
	for (int i=0;i<DIMX/2;++i)
	{
		for (int j=0;j<DIMY/2;++j)
		{
			col[0] =1; col[1] =1; col[2] =1;
			if (*(mdata.m_base_map->ind(i,j)) == 0)
			{
				col[0] =0; col[1] =0; col[2] =0;
			}
			if (*(mdata.m_base_map->ind(i,j)) == 1)
			{
				col[0] =0.5; col[1] =0.5; col[2] =0.5;
			}
			
		
			glColor3f(col[0],col[1],col[2]);
			glBegin(GL_QUADS);
			
			glVertex3f(i*2-CENTERX,CENTERY-j*2,0);
			glVertex3f(i*2-CENTERX+2,CENTERY-j*2,0);
			glVertex3f(i*2-CENTERX+2,CENTERY-j*2-2,0);
			glVertex3f(i*2-CENTERX,CENTERY-j*2-2,0);
			
			
			glEnd();
			
		}
		
	}
	
	
	
	std::vector<int>::iterator it;
	for (it = mdata.m_template_places[3].begin(); it != mdata.m_template_places[3].end(); ++it)
	{
		int i,j;
		i = *it / 10000;
		j = *it % 10000;
		
		glColor3f(1,0,0);
		glBegin(GL_QUADS);
			
		glVertex3f(i-CENTERX,CENTERY-j,0.2);
		glVertex3f(i-CENTERX+1,CENTERY-j,0.2);
		glVertex3f(i-CENTERX+1,CENTERY-j-1,0.2);
		glVertex3f(i-CENTERX,CENTERY-j-1,0.2);
			
			
		glEnd();
		
	}
	
	
	
	for (int i=0;i<DIMX;++i)
	{
		for (int j=0;j<DIMY;++j)
		{
			col[0] =1; col[1] =1; col[2] =1;
			if (*(mdata.m_template_map->ind(i,j)) >= 1)
			{
				col[0] =*(mdata.m_template_map->ind(i,j)) * 0.1; col[1] =0; col[2] =0;
			
			
			
		
			glColor3f(col[0],col[1],col[2]);
			glBegin(GL_QUADS);
			
			glVertex3f(i-CENTERX,CENTERY-j,0.1);
			glVertex3f(i-CENTERX+1,CENTERY-j,0.1);
			glVertex3f(i-CENTERX+1,CENTERY-j-1,0.1);
			glVertex3f(i-CENTERX,CENTERY-j-1,0.1);
			
			
			glEnd();
			}
			
		}
		
	}
	
	
	
	/*
	std::list<std::pair<int,int> >::iterator it;
	int i,j;
	for (it = mdata.m_border.begin(); it != mdata.m_border.end(); ++it)
	{
		i = it->first;
		j = it->second;
	
		glColor3f(1,0,0);
		glBegin(GL_QUADS);
			
		glVertex3f(i*2-CENTERX,CENTERY-j*2,0.1);
		glVertex3f(i*2-CENTERX+2,CENTERY-j*2,0.1);
		glVertex3f(i*2-CENTERX+2,CENTERY-j*2-2,0.1);
		glVertex3f(i*2-CENTERX,CENTERY-j*2-2,0.1);
			
			
		glEnd();
	}
	*/
	
	Shape s;
	s.m_type = Shape::CIRCLE;
	s.m_radius = 1000;
	s.m_center = Vector(500,500);
	
	WorldObjectList res;
	mdata.m_region->getObjectsInShape(&s,&res);
	
	WorldObjectList::iterator wt;
	WorldObject* wo;
	float cx,cy;
	float x = CENTERX;
	float y = CENTERY;
	float ex,ey,r;
	
	list<int> wos;
	
	for (wt = res.begin(); wt != res.end(); ++wt)
	{
		glPushMatrix();

		wo = *wt;
		wos.push_back(wo->getId());

		cx = (wo->getShape()->m_center.m_x)/4;
		cy = (wo->getShape()->m_center.m_y)/4;

		glTranslatef(cx-x,y-cy,0);
		glRotatef( wo->getShape()->m_angle*180/3.141,0,0,1);
		if (wo->getShape()->m_angle !=0)
		{
			DEBUG5("angle %f",wo->getShape()->m_angle);
		}
		glColor3f(0,0,1);

		if (wo->getShape()->m_type == Shape::RECT)
		{


			ex = wo->getShape()->m_extent.m_x / 4;
			ey = wo->getShape()->m_extent.m_y / 4;


			glBegin(GL_QUADS);
			glVertex3f(-ex,-ey,0.5);
			glVertex3f(+ex,-ey,0.5);
			glVertex3f(+ex,+ey,0.5);
			glVertex3f(-ex,+ey,0.5);

			glEnd();
		}

		if (wo->getShape()->m_type == Shape::CIRCLE)
		{

			r = wo->getShape()->m_radius /4;

			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0, 0,0);

			for (float a=0;a<= 2 *3.2;a+=0.103)
			{
				glVertex3f(cos(a)*r, sin(a)*r,0.05);
			}
			glEnd();
		}

		glPopMatrix();
	}
	
	/*
	wos.sort();
	list<int>::iterator lt;
	for (lt = wos.begin(); lt != wos.end(); lt++)
	{
		std::cout << *lt <<" ";
	}
	std::cout << "\n\n";
	*/
	glFlush ();				// Daten an Server (fuer die Darstellung) 
							// schicken
	glutSwapBuffers();		// Buffers wechseln
}




