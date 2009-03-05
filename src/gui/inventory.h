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
		/**
	 * \fn Inventory (Document* doc)
	 * \brief Konstruktor
		 */
		Inventory (Document* doc);
	
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
		 * \fn bool onSwapEquipClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Mausklick auf Ausruestung wechseln
		 */
		bool onSwapEquipClicked(const CEGUI::EventArgs& evt);
};

#endif

