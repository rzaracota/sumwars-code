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

/**
 * \file action.h
 * \defgroup action \ Aktionen
 * \ingroup command
 * \brief Stellt einzelne Aktionen zur Realisierung eines Kommandos zur Verf&uuml;gung
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Action
 */
#ifndef SERVERACTION_H
#define SERVERACTION_H

#include <string>
#include "action.h"

/**
 * \class Action
 * \brief Beschreibt eine Aktion, welche eine Figur ausf&uuml;hren kann. Eine Aktion ist dabei die kleinste Einheit einer Handlung.
 */

/**
 * \struct ServerAction
 * \brief Genauere Beschreibung einer Aktion auf Serverseite
 */
struct ServerAction : public Action
{
//Public stuff
	
public:
	

	
	
	
	//Fields
	//Constructors
	/**
	 * \fn Action(ActionType type, float goal_coordinate_x, float goal_coordinate_y, int goal_object_id, float time)
	 * \brief Legt ein neues Action Objekt an
	 * \param type initialisiert den Typ der Aktion
	 * \param goal_coordinate_x initialisiert die x-Koordinate des Zielpunkts
	 * \param goal_coordinate_y initialisiert die y-Koordinate des Zielpunkts
	 * \param goal_object_id initialisiert ID des Zielobjekts
	 * \param time initialisiert Gesamtdauer der Aktion
	 */
	ServerAction(ActionType type, float goal_coordinate_x, float goal_coordinate_y, int goal_object_id, float time) : Action(type)
	{
		m_goal_coordinate_x= goal_coordinate_x;
		m_goal_coordinate_y = goal_coordinate_y;
		m_goal_object_id= goal_object_id;
		m_time = time;
		m_elapsed_time =0;
	
	}
	
	/**
	 * \fn ServerAction() : Action()
	 * \brief Legt eine leere Aktion an
	 */
	ServerAction() : Action()
	{
		
	}
	
	//Fields	
	/**
	 * \var m_type;
	 * \brief Typ der vorhergehenden Aktion
	 */
	ActionType m_prev_type;
	
	/**
	 * \var m_goal_coordinate_x;
	 * \brief x-Koordinate des Zieles
	 */
	float m_goal_coordinate_x;
	
	/**
	 * \var m_goal_coordinate_y;
	 * \brief y-Koordinate des Zieles
	 */
	float m_goal_coordinate_y;
	
	/**
	 * \var m_goal_object_id;
	 * \brief ID des Zielobjekts
	 */
	int  m_goal_object_id;
	
	

	

	
};



#endif //SERVERACTION_H
