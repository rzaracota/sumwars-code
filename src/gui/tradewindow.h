#ifndef TRADEWINDOW_H
#define TRADEWINDOW_H

#include "itemwindow.h"

/**
 * \class TradeWindow
 * \brief Fenster den Handel mit NPCs
 */
class TradeWindow : public ItemWindow
{
	public:
		/**
	 * \fn TradeWindow (Document* doc)
	 * \brief Konstruktor
		 */
		TradeWindow (Document* doc);
	
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
		 * \fn bool onTradeItemHover(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Eintritt der Maus in Bereich ueber einem Item
		 */
		bool onTradeItemHover(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onItemMouseButtonPressed(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Druecken der linken Maustaste ueber einen Item
		 */
		bool onTradeItemMouseButtonPressed(const CEGUI::EventArgs& evt);

		/**
		 * \fn bool onItemMouseButtonReleased(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Loslassen der linken Maustaste ueber einen Item
		 */
		bool onTradeItemMouseButtonReleased(const CEGUI::EventArgs& evt);
		
		
		/**
		 * \fn bool onTradeNextItems(const CEGUI::EventArgs& evt);
		 * \brief Behandelt Druck auf einen der Buttons zum weiterschalten
		 */
		bool onTradeNextItems(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onCloseTrade(const CEGUI::EventArgs& evt);
		 * \brief Behandelt Druck auf einen der Buttons zum weiterschalten
		 */
		bool onCloseTrade(const CEGUI::EventArgs& evt);
		
		/**
		 * \var int m_big_sheet
		 * \brief Seite der grossen Items die angezeigt wird
		 */
		int m_big_sheet;
		
		/**
		 * \var int m_medium_sheet
		 * \brief Seite der mittleren Items die angezeigt wird
		 */
		int m_medium_sheet;
		
		/**
		 * \var int m_small_sheet
		 * \brief Seite der kleinen Items die angezeigt wird
		 */
		int m_small_sheet;
		
};

#endif

