/*
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

#ifndef WORLDMAP_H
#define WORLDMAP_H


#include "window.h"

/**
 * \class Worldmap
 * \brief Fenster Weltkarte
 */
class Worldmap : public Window
{
	public:
		/**
		* \fn Worldmap (Document* doc)
		* \brief Konstruktor
		*/
		Worldmap (Document* doc);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update();
		
		/**
		 * \fn virtual void updateTranslation
		 * \brief aktualisiert die Uebersetzungen
		 */
		virtual void updateTranslation();
		
	private:
		
		/**
		 * \fn bool onWaypointClicked(const CEGUI::EventArgs& evt);
		 * \brief Behandelt anklicken eines Wegpunktes
		 */
		bool onWaypointClicked(const CEGUI::EventArgs& evt);
		
};


#endif

