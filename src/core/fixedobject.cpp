/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Daniel Erler

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

#include "fixedobject.h"

//Constructors/Destructors
FixedObject::FixedObject( int id) : WorldObject(id)
{
	bool tmp=FixedObject::init();
	if (!tmp)
	{
		DEBUG("Initialisierung des Fixed Objects fehlgeschlagen!");
	}
}



FixedObject::FixedObject( int id,  TypeInfo::ObjectSubtype object_subtype)  : WorldObject( id)
{
	getTypeInfo()->m_subtype = object_subtype;
	bool tmp=FixedObject::init();
	if (!tmp)
	{
		DEBUG("Initialisierung des Fixed Objects fehlgeschlagen!");
	}
}

//Methods
bool FixedObject::init()
{
	//eigene Initialisierung
	getTypeInfo()->m_type = TypeInfo::TYPE_FIXED_OBJECT;

	return true;
}

bool FixedObject::update(float time)
{
	return true;
}

