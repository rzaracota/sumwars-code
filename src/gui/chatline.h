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
		
	private:
		/**
		 * \fn bool onSendMessage(const CEGUI::EventArgs& evt)
		 * \brief Behandelt das Senden einer Nachricht
		 * \param evt CEGUI Event Parameter
		 */
		bool onSendMessage(const CEGUI::EventArgs& evt);
};

#endif
