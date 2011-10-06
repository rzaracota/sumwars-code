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

#ifndef CHARCREATE_H
#define CHARCREATE_H

#include "window.h"


/**
 * \class CharCreate
 * \brief Fenster Charakter erstellen
 */
class CharCreate : public Window
{
	public:
		/**
	 * \fn CharCreate (Document* doc)
	 * \brief Konstruktor
		 */
		CharCreate (Document* doc);
	
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
		
		/**
		 * \fn void updateClassList()
		 * \brief aktualisiert die Liste der Spielerklassen
		 */
		void updateClassList();
		
	private:
	/**
	 * \fn bool onClassSelected(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Auswahl einer Klasse
	 */
	bool onClassSelected(const CEGUI::EventArgs& evt);
	
	/**
	 * \fn bool onLookSelected(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Auswahl eines Aussehens
	 */
	bool onLookSelected(const CEGUI::EventArgs& evt);
	
	/**
	 * \fn bool onButtonAbort(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Druck auf Knopf Abbrechen
	 */
	bool onButtonAbort(const CEGUI::EventArgs& evt);
	
	/**
	 * \fn bool onButtonCharCreate(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Druck auf Knopf Charakter erstellen
	 */
	bool onButtonCharCreate(const CEGUI::EventArgs& evt);
	
};

#endif
