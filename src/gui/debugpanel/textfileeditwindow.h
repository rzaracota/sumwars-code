#ifndef TEXTFILEEDITWINDOW_H
#define TEXTFILEEDITWINDOW_H

#include <CEGUI/CEGUIWindow.h>


class TextFileEditWindow : public CEGUI::Window
{
public:
    TextFileEditWindow(const CEGUI::String& type, const CEGUI::String& name);
	void close();
	void save();
	bool load(const CEGUI::String &m_filePath);
	CEGUI::String getFilepath() { return m_filePath; };
	void setFilepath(CEGUI::String path);
	
	virtual bool handleTextChanged(const CEGUI::EventArgs& e);
protected:
	CEGUI::String m_filePath;
	CEGUI::MultiLineEditbox *m_textEditBox;
	bool m_isDirty;
public:
	static CEGUI::String WidgetTypeName;
};

#endif // TEXTFILEEDITWINDOW_H
