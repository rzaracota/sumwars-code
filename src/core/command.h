/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Chris Drechsler

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
 * \file command.h
 * \defgroup command \ Kommandos
 * \brief Stellt Kommandos zur Steuerung der Figuren zur Verf&uuml;gung
 * \author Chris Drechsler
 * \version 1.0
 * \date 2007/06/06
 * \note Beinhaltet die Klasse Command
 */
#ifndef COMMAND_H
#define COMMAND_H



#include <string>
#include "action.h"


/**
 * \class Command
 * \brief Beschreibt ein Kommando, welches eine Figur erhalten kann. Zur Ausf&uuml;hrung des Kommandos wird die Anweisung in mehrere Aktionen unterteilt, die hintereinander ausgef&uuml;hrt werden.
 */
class Command {
/**
 * Public stuff
 */
public:
	/**
	 * Fields
	 */
	/**
	 * Constructors
	 */
	/**
	 * \fn Command(CommandType type, float goal_coordinate_x, float goal_coordinate_y, int goal_object_id)
	 * \brief Konstruktor
	 * \param type initialisiert den Typ des Kommandos
	 * \param goal_coordinate_x initialisiert die x-Koordinate des Zielpunkts
	 * \param goal_coordinate_y initialisiert die y-Koordinate des Zielpunkts
	 * \param goal_object_id initialisiert ID des Zielobjekts
	 * Legt ein neues Command Objekt an
	 */
	Command(Action::ActionType type, float goal_coordinate_x, float goal_coordinate_y, int goal_object_id)
	{
		m_type = type;
		m_goal_coordinate_x= goal_coordinate_x;
		m_goal_coordinate_y = goal_coordinate_y;
		m_goal_object_id= goal_object_id;
	
	}

	/**
	 * \fn Command()
	 * \brief Legt ein leeres Kommando an
	 */
	Command()
	{
		m_type = Action::NOACTION;
	}
	
	/**
	 * Fields
	 */
	/**
	 * \var m_type
	 * \brief Typ des Kommandos
	 */
	 Action::ActionType m_type;
	 
	/**
	 * \var m_goal_coordinate_x
	 * \brief x-Koordinate des Zieles
	 */
	float m_goal_coordinate_x;
	/**
	 * \var m_goal_coordinate_y
	 * \brief y-Koordinate des Zieles
	 */
	float m_goal_coordinate_y;
	/**
	 * \var m_goal_object_id
	 * \brief ID des Zielobjekts
	 */
	int  m_goal_object_id;
	
	/**
	 * \var float m_range
	 * \brief Reichweite der Aktion
	 */
	float m_range;
	
	/**
	 * \var m_damage_mult;
	 * \brief Multiplikator auf den Schaden der die Aktion abschließt
	 */
	float m_damage_mult;
	
	/**
	 * Constructors
	 */
	/**
	 * Accessor Methods
	 */
	/**
	 * Operations
	 */
};
#endif //COMMAND_H
