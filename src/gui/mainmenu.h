#ifndef MAINMENU_H
#define MAINMENU_H

#include "window.h"

/**
 * \class MainMenu
 * \brief Fenster Charakter Info
 */
class MainMenu : public Window
{
	public:
		/**
	 * \fn MainMenu (Document* doc)
	 * \brief Konstruktor
		 */
		MainMenu (Document* doc);
	
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
		 * \fn bool onStartSinglePlayer(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Start eines Einzelspielerspieles
		 */
		bool onStartSinglePlayer(const CEGUI::EventArgs& evt);

		/**
		 * \fn bool onStartMultiPlayer(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Start eines Einzelspielerspieles
	 	*/
		bool onStartMultiPlayer(const CEGUI::EventArgs& evt);

		/**
		 * \fn bool onStartMultiPlayerHost(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Start eines Einzelspielerspieles
		 */
		bool onStartMultiPlayerHost(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onShowCredits(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Credits
		 */
		bool onShowCredits(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onQuitGamePlayerHost(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Start eines Einzelspielerspieles
		 */
		bool onQuitGameHost(const CEGUI::EventArgs& evt);
};

#endif
