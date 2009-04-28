#ifndef LISTITEM_H
#define LISTITEM_H

#include "CEGUI.h"

/**
 * \class ListItem
 * \brief Eintrag einer CEGUI Auswahlliste
 */
class ListItem : public CEGUI::ListboxTextItem
{
	public:
		ListItem(const CEGUI::String& text, unsigned int id=0) : ListboxTextItem(text,id)
		{
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
			setAutoDeleted (true);
		}
};


/**
 * \class StrListItem
 * \brief Eintrag der Auswahlliste der Savegames
 */
class StrListItem : public ListItem
{
	public:
		StrListItem(const CEGUI::String& text, std::string data, unsigned int id=0) : ListItem(text,id)
		{
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
			m_data = data;
		}

		std::string m_data;
};

#endif
