#ifndef SAVEGAMELIST_H
#define SAVEGAMELIST_H

#include "Ogre.h"

#include "window.h"

/**
 * \class ListItem
 * \brief Eintrag einer CEGUI Auswahlliste
 */
class ListItem : public CEGUI::ListboxTextItem
{
	public:
		ListItem(const CEGUI::String& text) : ListboxTextItem(text)
		{
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
		}
};

/**
 * \class SaveListItem
 * \brief Eintrag der Auswahlliste der Savegames
 */
class SaveListItem : public ListItem
{
	public:
		SaveListItem(const CEGUI::String& text, std::string file) : ListItem(text)
		{
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
			m_filename = file;
		}

		std::string m_filename;
};



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
		 */
		SavegameList (Document* doc);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update();
		
	private:
		/**
		 * \fn bool onSavegameChosen(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Auswahl eines Savegames in der Liste
		 */
		bool onSavegameChosen(const CEGUI::EventArgs& evt);

		/**
		 * \fn bool onSavegameSelected(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Auswahl eines Savegames
		 */
		bool onSavegameSelected(const CEGUI::EventArgs& evt);

};

#endif
