/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Alexander Boehm

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <unistd.h>
#include "debug.h"
#include "network.h"
#include "worldobject.h"
#include "worldobjectselector.h"
#include "constants.h"
#include "worldobjecttype.h"
#include "player.h"
#include "command.h"
#include "fixedobject.h"
#include "constants.h"
#include "monster.h"
#include "clientstructures.h"
#include "user.h"
#include "goblin.h"
#include "warrior.h"
#include "fountain.h"
#include "trader.h"
#include "trade.h"
#include "spawnpoint.h"
#include "database.h"
#include "chest.h"
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>


World *g_world=NULL;

int main( int argc, char** argv )
{
	
	if( !db_init( argv[1], argv[2], argv[3], argv[4] ) )
	{
		ERROR( "No database available" );
		return false;
	}
	
	g_world=new World();
	db_set_users_in_world( false );

	g_world->saveData();
	
	delete g_world;
	db_kill();
	return 0;
}
	
	
