
#ifndef CHARINFO_H
#define CHARINFO_H

#include "window.h"

/**
 * \class CharInfo
 * \brief Fenster Charakter Info
 */
class CharInfo : public Window
{
	public:
		/**
		* \fn CharInfo (Document* doc)
		* \brief Konstruktor
		*/
		CharInfo (Document* doc);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update();
		
	private:
		/**
		 * \fn bool onIncreaseAttributeButtonClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Attribut erhoehen
		 */
		bool onIncreaseAttributeButtonClicked(const CEGUI::EventArgs& evt);
};

#endif


