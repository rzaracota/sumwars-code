#ifndef CHATLINE_H
#define CHATLINE_H

#include "window.h"

/**
 * \class ChatLine
 * \brief Fenster Charakter Info
 */
class ChatLine : public Window
{
	public:
		/**
	 * \fn ChatLine (Document* doc)
	 * \brief Konstruktor
		 */
		ChatLine (Document* doc);
	
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
		 * \fn bool onSendMessage(const CEGUI::EventArgs& evt)
		 * \brief Behandelt das Senden einer Nachricht
		 * \param evt CEGUI Event Parameter
		 */
		bool onSendMessage(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onKeyPress(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Tastendruck in diesem Fenster
		 */
		bool onKeyPress(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn void setHistoryLine()
		 * \brief Setzt die Chatzeile mit einer Nachricht aus der History
		 */
		void setHistoryLine();
		
		/**
		 * \var int m_history_line
		 * \brief Zeile der History die gerade angezeigt wird
		 */
		int m_history_line;
		
		/**
		 * \var std::list<std::string> m_history
		 * \brief Liste der früher abgesendeten Befehle
		 */
		std::list<std::string> m_history;
};

#endif
