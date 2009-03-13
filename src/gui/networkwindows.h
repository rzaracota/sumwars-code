#ifndef NETWORKWINDOW_H
#define NETWORKWINDOW_H


#include "window.h"


/**
 * \class HostGameWindow
 * \brief Fenster Spiel hosten
 */
class HostGameWindow : public Window
{
	public:
	/**
	 * \fn HostGameWindow (Document* doc
	 * \brief Konstruktor
	 */
		HostGameWindow (Document* doc);
	
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
		 * \fn bool onStartHostGame(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Start eines Multiplayer Spiels als Host
		 */
		bool onStartHostGame(const CEGUI::EventArgs& evt);
};


/**
 * \class JoinGameWindow
 * \brief Fenster Spiel hosten
 */
class JoinGameWindow : public Window
{
	public:
	/**
	 * \fn JoinGameWindow (Document* doc
	 * \brief Konstruktor
	 */
		JoinGameWindow (Document* doc);
	
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
	 * \fn bool onStartJoinGame(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Start eines Multiplayer Spiels als Host
		 */
		bool onStartJoinGame(const CEGUI::EventArgs& evt);
};
#endif

