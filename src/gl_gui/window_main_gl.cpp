#include "window_main_gl.h"


extern Document* global_doc;

void WindowMain::ReshapeFunc (int w, int h) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45, (double)w/(double)h, 1, 2000);
	glViewport (0, 0, w, h);
	glMatrixMode (GL_MODELVIEW);	// Sicherheit halber
	glLoadIdentity ();				// Modelierungsmatrix einstellen
}

void WindowMain::SetCamera() {

	//gluLookAt (0,0,1.2*FIELD_DIM, 0,0,0, 0, 1, 0);
	glMatrixMode( GL_PROJECTION  );
	glLoadIdentity();
	gluOrtho2D( -9, 9, -5, 5);
}

void WindowMain::KeyboardUpFunc (unsigned char key, int x, int y)
{
	DEBUG5("key up %i",key);

			// shift
	if (key == 60)
	{
		global_doc->getGUIState()->m_shift_hold = false;
	}


}

void WindowMain::KeyboardFunc (unsigned char key, int xin, int yin)
{
	DEBUG5("key down %i",key);

	int n =0;
	WorldObject::TypeInfo::ObjectSubtype i;
	if (global_doc->getLocalPlayer()!=0)
		i = global_doc->getLocalPlayer()->getTypeInfo()->m_subtype;

	if (i=="warrior")
		n=32;
	if (i=="mage")
		n=96;
	if (i=="archer")
		n=64;
	if (i=="priest")
		n=128;

	if (key >=97 && key < 123)
	{

		global_doc->setLeftAction((Action::ActionType) (key - 97+n));
	}

	if (key >=65 && key < 90)
	{
		global_doc->setRightAction((Action::ActionType) (key - 65+n));
	}

	if (key >=49 && key <59)
	{
		global_doc->setLeftAction((Action::ActionType) (key -49+4));
	}


	// shift
	if (key == 60)
	{
		global_doc->getGUIState()->m_shift_hold = true;
	}



	if (key == 43)
	{
		global_doc ->Document::onButtonPartyAccept(0);
	}

	// ESC
	if (key == 27)
	{
		global_doc ->onKeyPress(27);
	}

}

void WindowMain::SpecialKeyboardUpFunc (int key, int x, int y)
{

}

void WindowMain::SpecialKeyboardFunc (int key, int x, int y)
{
	DEBUG5("skey down %i",key);
	// F1-F4 - Party management
	if (key >=1 && key <= 4)
	{
		global_doc ->Document::onButtonPartyApply(key - 1);
	}

}

void WindowMain::TimerFunc(int dummy)
{
	if (global_doc->getState() == Document::SHUTDOWN)
		exit(0);
	
	global_doc->update(25);

	glutTimerFunc(25,TimerFunc,0);
	glutPostRedisplay();
}

void WindowMain::MouseFunc (int button, int state, int xin, int yin)
{
	DEBUG5("mouse %i  %i %i",button,xin,yin);

	float x=xin,y=yin;

	// Transformieren der Koordinaten
	float fielddim = 50;
	float  x_dim = 9;
	float y_dim = 5;
	x=(x/fielddim)-x_dim;
	y=y_dim-(y/fielddim);
	if (state == GLUT_DOWN)
	{
		if (global_doc->getLocalPlayer() ==0)
			return;
		
		float dx,dy;
		dx = global_doc->getLocalPlayer()->getGeometry()->m_shape.m_coordinate_x;
		dy = global_doc->getLocalPlayer()->getGeometry()->m_shape.m_coordinate_y;
		
		if (button==0)
		{

			global_doc->getGUIState()->m_left_mouse_hold=true;
			global_doc->onLeftMouseButtonClick(x+dx, y+dy);

			DEBUG5("Left Button press");
		}


		if (button==2)
		{
			global_doc->getGUIState()->m_right_mouse_hold=true;
			global_doc->onRightMouseButtonClick(x+dx, y+dy);

			DEBUG5("right Button press");

		}
	}
	else
	{
		if (button==0)
		{
			DEBUG5("Button release");
			global_doc->getGUIState()->m_left_mouse_hold=false;
			global_doc->getGUIState()->m_clicked_object_id=0;
		}

		if (button==2)
		{
			DEBUG5("Right Button release");
			global_doc->getGUIState()->m_right_mouse_hold=false;
			global_doc->getGUIState()->m_clicked_object_id=0;
		}
	}
}

WindowMain::WindowMain()
{
	int i=0;
	glutInit (&i,0);
	glutInitWindowSize (900,500);
	glutInitWindowPosition (0,0);
	glutInitDisplayMode (GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutCreateWindow ("test");

	// OpenGL Initialisierungen
	glEnable (GL_DEPTH_TEST);


	// Callback funktion
	glutDisplayFunc (DisplayFunc);
	glutReshapeFunc (ReshapeFunc);
	glutMouseFunc (MouseFunc);
	glutKeyboardFunc (KeyboardFunc);
	glutSpecialFunc (SpecialKeyboardFunc);
	glutKeyboardUpFunc(KeyboardUpFunc);
	glutSpecialUpFunc(SpecialKeyboardUpFunc);
	glutIgnoreKeyRepeat(1);

	glutTimerFunc(25,TimerFunc,0);

	// Die Hauptschleife
	glutMainLoop ();

}

void WindowMain::DisplayFunc ()
{
	SetCamera();

	// Buffer neu initialisieren
	glMatrixMode (GL_MODELVIEW);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();

	if (global_doc->getLocalPlayer() ==0)
		return;
	
	
	Region* region = global_doc->getLocalPlayer()->getRegion();
	if (region ==0)
		return;


	static float x = 0;
	static float y = 0;
	float cx,cy,ex,ey,r;


	glDisable(GL_BLEND);

	if (global_doc->getLocalPlayer()==0)
	{
		glFlush ();				// Daten an Server (fuer die Darstellung)
							// schicken
		glutSwapBuffers();		// Buffers wechseln
		return;
	}


	// Daten sperren
	global_doc->lock();


	x = global_doc->getLocalPlayer()->getGeometry()->m_shape.m_coordinate_x;
	y = global_doc->getLocalPlayer()->getGeometry()->m_shape.m_coordinate_y;


	int action_idx ;

	float c[3];

	// Tiles zeichnen
	/*
	Matrix2d<char>* mat = global_doc->getRegionData()->m_tiles;
	int dimx = global_doc->getRegionData()->m_dimx;
	int dimy = global_doc->getRegionData()->m_dimy;
	for (int i = max(0,(int) x/2-5);i<= min((int) x/2+5,2*dimx-1);i++)
	{
		for (int j = max(0,(int) y/2-4);j<= min((int) x/2+4,2*dimy-1);j++)
		{
			if (*(mat->ind(i,j)) == 0)
			{
				continue;
			}

			switch(*(mat->ind(i,j)))
			{
				case TILE_GRASS:
					c[0]=0;c[1]=0.4;c[2]=0;
					break;

				case TILE_CAVE:
					c[0]=0.4;c[1]=0.2;c[2]=0;
					break;

				case TILE_WATER:
					c[0]=0;c[1]=0;c[2]=0.6;
					break;



			}


			glPushMatrix();
			glTranslatef(i*2-x,j*2-y,0);

			glColor3f(c[0],c[1],c[2]);

			glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0,2,0);
			glVertex3f(2,2,0);
			glVertex3f(2,0,0);

			glEnd();

			glPopMatrix();

		}

	}
*/

	list<ServerWObject*> stat_objs;
	list<ServerWObject*> obj;
	list<ServerWObject*>::iterator it;
	Shape s;
	s.m_coordinate_x = x;
	s.m_coordinate_y = y;
	s.m_type = Shape::RECT;
	s.m_extent_x = 10;
	s.m_extent_y = 10;
	
	ServerWObject* wo, *cwo;
	Creature* cr;
	
	region->getSWObjectsInShape(&s,&stat_objs, WorldObject::Geometry::LAYER_ALL,WorldObject::FIXED);
	region->getSWObjectsInShape(&s,&obj, WorldObject::Geometry::LAYER_ALL,WorldObject::GROUP_ALL & ~WorldObject::FIXED);

	for (it = stat_objs.begin(); it !=stat_objs.end();++it)
	{
		glPushMatrix();

		wo = *it;

		cx = wo->getGeometry()->m_shape.m_coordinate_x;
		cy = wo->getGeometry()->m_shape.m_coordinate_y;

		glTranslatef(cx-x,cy-y,0);
		glRotatef( wo->getGeometry()->m_angle*180/3.141,0,0,1);
		if (wo->getGeometry()->m_angle !=0)
		{
			DEBUG5("angle %f",wo->getGeometry()->m_angle);
		}
		glColor3f(0,0,1);

		if (wo->getGeometry()->m_shape.m_type == Shape::RECT)
		{


			ex = wo->getGeometry()->m_shape.m_extent_x;
			ey = wo->getGeometry()->m_shape.m_extent_y;


			glBegin(GL_QUADS);
			glVertex3f(-ex,-ey,0.05);
			glVertex3f(+ex,-ey,0.05);
			glVertex3f(+ex,+ey,0.05);
			glVertex3f(-ex,+ey,0.05);

			glEnd();
		}

		if (wo->getGeometry()->m_shape.m_type == Shape::CIRCLE)
		{

			r = wo->getGeometry()->m_shape.m_radius;

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

	for (it = obj.begin(); it != obj.end(); ++it)
	{
		glPushMatrix();

		cwo = *it;
		//DEBUG("displaying obj %i",cwo->getId());
		c[0]=0;
		c[1]=0;
		c[2]=0;

		cx = cwo->getGeometry()->m_shape.m_coordinate_x;
		cy = cwo->getGeometry()->m_shape.m_coordinate_y;

		glTranslatef(cx-x,cy-y,0);
		glRotatef( cwo->getGeometry()->m_angle*180/3.141,0,0,1);
		if (wo->getGeometry()->m_angle !=0)
		{
			DEBUG5("angle %f",cwo->getGeometry()->m_angle);
		}

		switch( cwo->getTypeInfo()->m_type )
		{
			case WorldObject::TypeInfo::TYPE_PLAYER:
				if ( cwo->getTypeInfo()->m_subtype == "warrior" )
				{
					c[1]=1;
				}
				else if ( cwo->getTypeInfo()->m_subtype == "mage" )
				{
					c[1]=0.7;c[2]=0.5;
				}
				else if ( cwo->getTypeInfo()->m_subtype == "archer" )
				{
					c[1]=1;c[2]=0.5;c[0]=0.5;
				}
				else if ( cwo->getTypeInfo()->m_subtype == "priest" )
				{
					c[1]=1;c[2]=0;c[0]=0.5;
				}

				break;

			case WorldObject::TypeInfo::TYPE_MONSTER:
				c[0]=1;
				break;

			case WorldObject::TypeInfo::TYPE_FIXED_OBJECT:
				c[2]=1;

			default:

				break;
		}
		glColor3f(c[0],c[1],c[2]);
		if (cwo->getGeometry()->m_shape.m_type == Shape::RECT)
		{


			ex = cwo->getGeometry()->m_shape.m_extent_x;
			ey = cwo->getGeometry()->m_shape.m_extent_y;


			glBegin(GL_QUADS);
			glVertex3f(-ex,-ey,0.05);
			glVertex3f(+ex,-ey,0.05);
			glVertex3f(+ex,+ey,0.05);
			glVertex3f(-ex,+ey,0.05);

			glEnd();
		}

		if (cwo->getGeometry()->m_shape.m_type == Shape::CIRCLE)
		{

			r = cwo->getGeometry()->m_shape.m_radius;

			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0, 0, 0.05);

			for (float a=0;a<= 2 *3.2;a+=0.103)
			{
				glVertex3f(cos(a)*r, sin(a)*r,0.05);
			}
			glEnd();

			if (cwo->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
			{
				cr = static_cast<Creature*> (cwo);
				glColor3f(0,0,0);
				glBegin(GL_QUADS);
				glVertex3f(-0.8*r,-0.2*r,0.1);
				glVertex3f(+0.8*r,-0.2*r,0.1);
				glVertex3f(+0.8*r,+0.2*r,0.1);
				glVertex3f(-0.8*r,+0.2*r,0.1);
				glEnd();


				//DEBUG5("health percent %f",cwo->m_health_perc);
				float p = cr->getDynAttr()->m_health / cr->getBaseAttrMod()->m_max_health;
				float* status_mods = cr->getDynAttr()->m_status_mod_time;
				if (p<=0)
					p=0;
				glColor3f(c[0],c[1],c[2]);
				glBegin(GL_QUADS);
				glVertex3f(-0.8*r,-0.2*r,0.2);
				glVertex3f(-0.8*r+p*1.6*r,-0.2*r,0.2);
				glVertex3f(-0.8*r+p*1.6*r,+0.2*r,0.2);
				glVertex3f(-0.8*r,+0.2*r,0.2);

				glEnd();

				if (status_mods[0] >0)
				{
					glColor3f(1,1,1);
					glBegin(GL_QUADS);
					glVertex3f(-0.6*r,+0.6*r,0.2);
					glVertex3f(-0.3*r,+0.6*r,0.2);
					glVertex3f(-0.3*r,+0.3*r,0.2);
					glVertex3f(-0.6*r,+0.3*r,0.2);

					glEnd();
				}

				if (status_mods[1] >0)
				{
					glColor3f(0,0.6,0);
					glBegin(GL_QUADS);
					glVertex3f(-0.3*r,+0.6*r,0.2);
					glVertex3f(-0.0*r,+0.6*r,0.2);
					glVertex3f(-0.0*r,+0.3*r,0.2);
					glVertex3f(-0.3*r,+0.3*r,0.2);

					glEnd();
				}

				if (status_mods[2] >0)
				{
					glColor3f(0.7,0,0);
					glBegin(GL_QUADS);
					glVertex3f(+0.3*r,+0.6*r,0.2);
					glVertex3f(-0.0*r,+0.6*r,0.2);
					glVertex3f(-0.0*r,+0.3*r,0.2);
					glVertex3f(+0.3*r,+0.3*r,0.2);

					glEnd();
				}

				if (status_mods[3] >0)
				{
					glColor3f(0.5,0.5,0.5);
					glBegin(GL_QUADS);
					glVertex3f(+0.3*r,+0.6*r,0.2);
					glVertex3f(+0.6*r,+0.6*r,0.2);
					glVertex3f(+0.6*r,+0.3*r,0.2);
					glVertex3f(+0.3*r,+0.3*r,0.2);

					glEnd();
				}

				if (status_mods[4] >0)
				{
					glColor3f(0.7,0,0.7);
					glBegin(GL_QUADS);
					glVertex3f(-0.3*r,-0.6*r,0.2);
					glVertex3f(-0.6*r,-0.6*r,0.2);
					glVertex3f(-0.6*r,-0.3*r,0.2);
					glVertex3f(-0.3*r,-0.3*r,0.2);

					glEnd();
				}

				if (status_mods[5] >0)
				{
					glColor3f(1,1,0);
					glBegin(GL_QUADS);
					glVertex3f(-0.3*r,-0.6*r,0.2);
					glVertex3f(-0.0*r,-0.6*r,0.2);
					glVertex3f(-0.0*r,-0.3*r,0.2);
					glVertex3f(-0.3*r,-0.3*r,0.2);

					glEnd();
				}

				if (status_mods[6] >0)
				{
					glColor3f(0.6,0.6,1);
					glBegin(GL_QUADS);
					glVertex3f(+0.3*r,-0.6*r,0.2);
					glVertex3f(-0.0*r,-0.6*r,0.2);
					glVertex3f(-0.0*r,-0.3*r,0.2);
					glVertex3f(+0.3*r,-0.3*r,0.2);

					glEnd();
				}

				if (status_mods[7] >0)
				{
					glColor3f(1,0.8,0.3);
					glBegin(GL_QUADS);
					glVertex3f(+0.3*r,-0.6*r,0.2);
					glVertex3f(0.6*r,-0.6*r,0.2);
					glVertex3f(0.6*r,-0.3*r,0.2);
					glVertex3f(+0.3*r,-0.3*r,0.2);

					glEnd();
				}
			}




		}
		glPopMatrix();


	}

	list<DmgProjectile*> proj;
	list<DmgProjectile*>::iterator i2;
	
	region->getProjectilesOnScreen(x,y,&proj);
	Projectile* pr;
	for (i2=proj.begin(); i2 != proj.end(); ++i2)
	{
		pr = *i2;
		glPushMatrix();
		c[0]=0;
		c[1]=0;
		c[2]=0;

		cx = pr->getGeometry()->m_coordinate_x;
		cy = pr->getGeometry()->m_coordinate_y;
		r = pr->getGeometry()->m_radius;
		DEBUG5("draw projectile %f %f %f",cx,cy,r);

		glTranslatef(cx-x,cy-y,0);

		switch (pr->getType())
		{
			case Projectile::MAGIC_ARROW:
				c[0]=1;c[1]= 0.5;
				break;

			case Projectile::FIRE_BOLT:
			case Projectile::FIRE_WAVE:
			case Projectile::FIRE_BALL:
			case Projectile::FIRE_WALL:
			case Projectile::FIRE_ARROW:
			case Projectile::FIRE_EXPLOSION:
			case Projectile::EXPLOSION:
				c[0]=1;c[1]= 0.2;
				break;

			case Projectile::ICE_BOLT:
			case Projectile::ICE_RING:
			case Projectile::FREEZE:
			case Projectile::BLIZZARD:
			case Projectile::ICE_ARROW:
			case Projectile::ICE_EXPLOSION:

				c[0]=0.6;c[1]= 0.6;c[2]=1;
				break;

			case Projectile::LIGHTNING:
			case Projectile::THUNDERSTORM:
			case Projectile::CHAIN_LIGHTNING:
			case Projectile::WIND_EXPLOSION:
			case Projectile::WIND_ARROW:
				c[0]=1;c[1]= 1;c[2]=0.6;
				break;

			case Projectile::ARROW:
				c[0]=0.7;c[1]=0;c[2]=0;
				break;
			case Projectile::GUIDED_ARROW:
				c[0]=1;c[1]=1;c[2]=1;
				break;

			case Projectile::LIGHT_BEAM:
				c[0]=1;c[1]=1;c[2]=1;
				break;

			case Projectile::ACID:
				c[1]=0.6;
				break;

			case Projectile::DIVINE_BEAM:
				c[0]=1;c[1]= 1;c[2]=0.3;
				break;

			case Projectile::HYPNOSIS:
				c[0]=0.6;c[1]= 0.6;c[2]=0.6;
				break;

			case Projectile::ELEM_EXPLOSION:
				c[0]=0.1;c[1]= 0.1;

			case Projectile::STATIC_SHIELD:
				c[0]=1;c[1]=1;c[2]=2;



		}

		glColor3f(c[0],c[1],c[2]);
		r = pr->getGeometry()->m_radius;

		if (r<0.5)
		{
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0, 0,0.6);

			for (float a=0;a<= 2 *3.2;a+=0.103)
			{
				glVertex3f(cos(a)*r, sin(a)*r,0.6);
			}
			glEnd();
		}
		else
		{
			glBegin(GL_QUADS);
			{
				for (float a=0;a<= 2 *3.2;a+=0.1/r)
				{
					glVertex3f(cos(a)*r, sin(a)*r,+0.6);
					glVertex3f(cos(a+0.1/r)*r, sin(a+0.1/r)*r,+0.6);

					glVertex3f(cos(a+0.1/r)*(r-0.5), sin(a+0.1/r)*(r-0.5),+0.6);
					glVertex3f(cos(a)*(r-0.5), sin(a)*(r-0.5),+0.6);


				}
			}
			glEnd();
		}
		glPopMatrix();
	}


	r=0.5;
	glPushMatrix();
	glTranslatef(-8,-4,0);

	glColor3f(1,1,1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0,0.8);

	for (float a=0;a<=(1-global_doc->getLocalPlayer()->getTimerPercent(1))*6.3;a+=0.1)
	{
		glVertex3f(cos(a)*r, sin(a)*r,0.8);
	}
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(8,-4,0);

	glColor3f(1,1,1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0,0.8);

	for (float a=0;a<=(1-global_doc->getLocalPlayer()->getTimerPercent(2))*6.3;a+=0.1)
	{
		glVertex3f(cos(a)*r, sin(a)*r,0.8);
	}
	glEnd();
	glPopMatrix();


	// Daten entsperren
	global_doc->unlock();

	glFlush ();				// Daten an Server (fuer die Darstellung)
							// schicken
	glutSwapBuffers();		// Buffers wechseln

}
