/*
	Summoning Wars - main.cpp
	Copyright (C) 2007  Michael Kempf

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "window_main_gl.h"



Document* global_doc=NULL;


int main (int argc, char *argv[])
{

    WindowMain* win_main = NULL;

	string save;
	if (argc>=2)
	{
		save = argv[1];
	}
	else
	{
		save = "../save/warrior.sav";
	}

	// Globales Document anlegen
	global_doc = new Document(Document::GL_GUI);
	global_doc->setSaveFile(save);
	global_doc->serverConnect();

	//Gtk::Main mn( argc,argv );
	//Gtk::GL::init( argc,argv );



	if ( true )
	{
		//win_main=new WindowMain(global_doc);
		win_main=new WindowMain();

		DEBUG("starting main window");

		//Gtk::Main::run( *win_main );

		delete win_main;
	}

	delete global_doc;

	return 0;
}
