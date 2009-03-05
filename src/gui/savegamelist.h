#ifndef SAVEGAMELIST_H
#define SAVEGAMELIST_H

#include "Ogre.h"

#include "window.h"

#include "listitem.h"


/**
 * \class SavegameList
 * \brief Fenster Charakter Info
 */
class SavegameList : public Window
{
	public:
		/**
		 * \fn SavegameList (Document* doc)
		 * \brief Konstruktor
		 * \param doc Dokument
		 */
		SavegameList (Document* doc);
	
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
		 * \fn bool onSavegameChosen(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Auswahl eines Savegames in der Liste
		 */
		bool onSavegameChosen(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onNewCharClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Auswahl eines Savegames in der Liste
		 */
		bool onNewCharClicked(const CEGUI::EventArgs& evt);

		/**
		 * \fn bool onSavegameSelected(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Auswahl eines Savegames
		 */
		bool onSavegameSelected(const CEGUI::EventArgs& evt);

};

#endif
