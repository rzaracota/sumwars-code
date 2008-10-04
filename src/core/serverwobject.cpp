/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf

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

#include "serverwobject.h"

//Constructors/Destructors
ServerWObject::ServerWObject(World* world, int id) : WorldObject(id)
{
	bool tmp=init();
	if (tmp!=true)
	{
	DEBUG("Initialisierung des ServerWObjects fehlgeschlagen");
	}
	m_world=world;

}

//Methods
 bool  ServerWObject::init ()
{
	m_destroyed=false;
	return true;
}


bool ServerWObject::moveTo(float x, float y)
{
	if (m_world==0)
	{
		WorldObject::Geometry* geom = getGeometry();
		geom->m_shape.m_coordinate_x=x;
		geom->m_shape.m_coordinate_y=y;
		return true;
	}
	else
	{
		return m_world->moveObject(this, x,y);
	}
}

Region* ServerWObject::getRegion()
{
	return m_world->getRegion(m_grid_location.m_region);
}

bool  ServerWObject::destroy()
{
	DEBUG5("destroy");
	return true;
}




