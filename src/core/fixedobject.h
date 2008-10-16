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
/**
 * \file fixedobject.h
 * \defgroup Fixedobject \ Fixes Objekt
 * \ingroup worldobject
 * \brief Stellt ein unbewegliches Objekt dar
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse FixedObject
 */

#ifndef FIXEDOBJECT_H
#define FIXEDOBJECT_H
#include <string>

#include "worldobject.h"

struct FixedObjectData
{
	short m_layer;
	
	Shape m_shape;
};
	
	



/**
 * \class FixedObject
 * \brief Dieses Objekt stellt ein fixes Objekt dar. Es kann seine Position in der Welt nicht ver&auml;ndern.
 */
class FixedObject : public WorldObject {
/**
 * Public stuff
 */
public:
	
	/**
	 * \fn FixedObject(World* world, int id)
	 * \param world Zeiger auf die Welt
	 * \param id ID des Objekts
	 * \brief Konstruktor
	 */
	FixedObject(World* world, int id);
	
	/**
	 * \fn FixedObject(World* world,  int id, TypeInfo::ObjectSubtype object_subtype)
	 * \brief Konstruktor
	 */
	FixedObject(World* world,  int id, TypeInfo::ObjectSubtype object_subtype);
	
	/**
	* \fn virtual bool init()
	* \brief Initialisierung von unbewegten Gegenständen
	* \return bool Gibt an, ob die Initialisierung erfolgreich war
	*/
	virtual bool init ();
	
	/**
	 * \fn virtual bool  update (float time)
	 * \brief aktualisiert das Objekt nach dem eine bestimmte Zeit vergangen ist
	 * \param time vergangene Zeit in ms
	 */
	virtual bool  update (float time);


};
#endif //FIXEDOBJECT_H

