#ifndef TEXTFILEEDITWINDOW_H
#define TEXTFILEEDITWINDOW_H

#include <CEGUI/CEGUIWindow.h>


class TextFileEditWindow : public CEGUI::Window
{
public:
    TextFileEditWindow(const CEGUI::String& type, const CEGUI::String& name);
	void save();
	void load(CEGUI::String fileName);

protected:
	CEGUI::MultiLineEditbox *m_textEditBox;
public:
	static CEGUI::String WidgetTypeName;
};

#endif // TEXTFILEEDITWINDOW_H
