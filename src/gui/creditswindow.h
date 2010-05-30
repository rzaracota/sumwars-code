#pragma once


#include "window.h"


/**
 * \class CreditsWindow
 * \brief Window for displaying credits
 */
class CreditsWindow : public Window
{
	public:
		/**
		 * \fn  (Document* doc)
		 * \brief Konstruktor
		 */
		CreditsWindow (Document* doc);
	
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
		 * \brief Timer that counts the time, the Window is shown
		 */
		Timer m_shown_timer;
};

