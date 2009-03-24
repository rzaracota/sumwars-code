#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "Ogre.h"

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

