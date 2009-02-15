#ifndef LISTITEM_H
#define LISTITEM_H

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
		StrListItem(const CEGUI::String& text, std::string data) : ListItem(text)
		{
			setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
			m_data = data;
		}

		std::string m_data;
};

#endif
