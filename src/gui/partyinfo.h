#ifndef PARTYINFO_H
#define PARTYINFO_H

#include "window.h"

/**
 * \class PartyInfo
 * \brief Fenster Charakter Info
 */
class PartyInfo : public Window
{
	public:
		/**
	 * \fn PartyInfo (Document* doc)
	 * \brief Konstruktor
		 */
		PartyInfo (Document* doc);
	
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
		 * \fn bool onAcceptMemberButtonClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Bewerber aktzeptieren
		 */
		bool onAcceptMemberButtonClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onRejectMemberButtonClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Bewerber ablehnen
		 */
		bool onRejectMemberButtonClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onApplyButtonClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button bewerben
		 */
		bool onApplyButtonClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onKickMemberButtonClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Partymitglied kicken
		 */
		bool onKickMemberButtonClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onLeavePartyButtonClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Party verlassen
		 */
		bool onLeavePartyButtonClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onPeaceButtonClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Frieden anbieten
		 */
		bool onPeaceButtonClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onDeclareWarButtonClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Button Krieg erklären
		 */
		bool onDeclareWarButtonClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \var int m_player_ids[8]
		 * \brief IDs der Spieler die sich hinter den Buttons befinden
		 */
		int m_player_ids[8];
};

#endif

