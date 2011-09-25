#ifndef WINDOW_H
#define WINDOW_H


#include "document.h"
#include "gettext.h"

// CEGUI includes
#include "CEGUI/CEGUI.h"

/**
 * \fn class Window
 * \brief Wrapperklasse fuer ein CEGUI:: Window
 */
class Window
{
	public:
		/**
		* \fn Window(Document* doc)
		* \brief Konstruktor
		*/
		Window(Document* doc)
		{
			m_document = doc;
		}
		
		/**
		* \fn virtual ~Window()
		* \brief Destruktur
		*/
		virtual ~Window()
		{
			
		}
		
		/**
		 * \fn virtual void update() =0
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update() =0;
		
		/**
		 * \fn virtual void updateTranslation
		 * \brief aktualisiert die Uebersetzungen
		 */
		virtual void updateTranslation() =0;
		
		/**
		 * \fn bool consumeEvent(const CEGUI::EventArgs& evt)
		 * \brief Funktion die Events verbraucht, gibt immer true zurueck
		 */
		bool consumeEvent(const CEGUI::EventArgs& evt)
		{
			return true;
		}
		
		/**
		 * \fn CEGUI::Window* getCEGUIWindow()
		 * \brief Gibt das zugrunde liegende CEGUI Window aus
		 */
		CEGUI::Window* getCEGUIWindow()
		{
			return m_window;
		}
		
		/**
		 * \brief Returns the main document
		 */
		Document* getDocument()
		{
			return m_document;
		}
		
		
	protected:
		/**
		 * \var CEGUI::Window* m_window
		 * \brief das darunter liegende CEGUI Window
		 */
		CEGUI::Window* m_window;
		
		/**
		 * \var Document* m_document
		 * \brief Zeiger auf das Dokument
		 */
		Document* m_document;
		
		
	
};


#endif


