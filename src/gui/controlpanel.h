
#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "itemwindow.h"

/**
 * \class ControlPanel
 * \brief Fenster fuer die Steuerleiste
 */
class ControlPanel : public ItemWindow
{
	public:
		/**
	 * \fn ControlPanel (Document* doc)
	 * \brief Konstruktor
		 */
		ControlPanel (Document* doc);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update();
		
		/**
		 * \fn void onButtonCharInfoClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf CharakterInfo Button
		 */
		bool onButtonCharInfoClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn void onButtonSkilltreeClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Skilltree Button
		 */
		bool onButtonSkilltreeClicked(const CEGUI::EventArgs& evt);
		
	private:
		/**
		* \fn bool onButtonSaveExitClicked(const CEGUI::EventArgs& evt)
		* \brief Behandelt Klick auf Speichern/Beenden
		*/
		bool onButtonSaveExitClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn void onButtonInventoryClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Inventar Button
		 */
		bool onButtonInventoryClicked(const CEGUI::EventArgs& evt);

		/**
		 *\fn bool onButtonPartyClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Party Button
		 */
		bool onButtonPartyClicked(const CEGUI::EventArgs& evt);

		/**
		 * \fn void onButtonOpenChatClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Chat oeffnen Button
		 */
		bool onButtonOpenChatClicked(const CEGUI::EventArgs& evt);
		
		
		
		
};

#endif

