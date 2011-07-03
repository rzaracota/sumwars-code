#ifndef INVENTORY_H
#define INVENTORY_H

#include "itemwindow.h"

/**
 * \class Inventory
 * \brief Fenster das Inventar
 */
class Inventory : public ItemWindow
{
	public:
		enum State
		{
			StateSmall = 0,
			StateMedium,
			StateBig
		};


		/**
	 * \fn Inventory (Document* doc)
	 * \brief Konstruktor
		 */
		Inventory (Document* doc);
	
		/**
		 * \fn virtual void update()
		 * \brief Updates the window contents
		 */
		virtual void update();

		/**
		 * \fn virtual void updateTranslation
		 * \brief Updates the translations
		 */
		virtual void updateTranslation();

		/**
		 * \fn virtual void setState
		 * \brief Sets the Inventory state
		 */
		void setState(State s);

	private:
		/**
		 * \fn bool onSwapEquipClicked(const CEGUI::EventArgs& evt)
		 * \brief Handles clicks to the equipment swap button
		 */
		bool onSwapEquipClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onDropGoldClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Mausklick auf Gold fallen lassen
		 */
		bool onDropGoldClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onSwitchTabClicked(const CEGUI::EventArgs& evt)
		 * \brief Handles clicks to the switch tab button
		 */
		bool onSwitchTabClicked(const CEGUI::EventArgs& evt);

		/**
		 * \fn bool createAnimations()
		 * \brief Creates All animations used by the Inventory GUI
		 */
		void createAnimations();
};

#endif

