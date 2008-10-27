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
		save = "warrior.sav";
	}
	
	bool server = true;
	
	if (argc>=3)
	{
		if (std::string(argv[2]) == "-c")
		{
			server = false;
		}
	}

	ObjectFactory::init();
	ObjectFactory::loadFixedObjectData("../data/objects/objects.xml");
	
	// Globales Document anlegen
	global_doc = new Document();
	global_doc->setSaveFile(save);
	
	global_doc->installShortkey(27,Document::CLOSE_ALL);
	
	global_doc->startGame(server);

	//Gtk::Main mn( argc,argv );
	//Gtk::GL::init( argc,argv );

	

	if ( true )
	{
		
		win_main=new WindowMain();
		ObjectFactory::cleanup();
		delete win_main;
	}
		
	

	return 0;
}
