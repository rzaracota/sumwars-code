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
