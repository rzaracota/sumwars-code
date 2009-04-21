#ifndef MESSAGEBOXES_H
#define MESSAGEBOXES_H


#include "window.h"


/**
 * \class SaveExitWindow
 * \brief Fenster Spiel beenden
 */
class SaveExitWindow : public Window
{
	public:
	/**
	 * \fn SaveExitWindow (Document* doc
	 * \brief Konstruktor
	 */
		SaveExitWindow (Document* doc);
	
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
		 * \fn bool onExitGameConfirmed(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Spiel beenden
		 */
		bool onExitGameConfirmed(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onExitGameAborted(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Spiel beenden
		 */
		bool onExitGameAborted(const CEGUI::EventArgs& evt);
};


#endif

