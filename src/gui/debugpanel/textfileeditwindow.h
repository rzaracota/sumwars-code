#ifndef TEXTFILEEDITWINDOW_H
#define TEXTFILEEDITWINDOW_H

#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/CEGUIEvent.h>
#include "filebrowser/filebrowser.h"

#include "Poco/File.h"

class TextFileEditWindow : public CEGUI::Window
{
public:
    TextFileEditWindow(const CEGUI::String& type, const CEGUI::String& name);
	void close();
	void save();
	bool load(const CEGUI::String &m_file);
	CEGUI::String getFilepath() { return m_file.path(); };
	void setFilepath(CEGUI::String path);
	
	virtual bool handleTextChanged(const CEGUI::EventArgs& e);
	virtual bool handleCharacterKey(const CEGUI::EventArgs& e);
	virtual bool handleFileBrowserSaveClicked(const CEGUI::EventArgs& e);
	virtual bool handleFileBrowserCancelClicked(const CEGUI::EventArgs& e);
	
protected:
	CEGUI::Event::Connection m_handleTextChangedConnection;
	Poco::File m_file;
	CEGUI::MultiLineEditbox *m_textEditBox;
	bool m_isDirty;
public:
	FileBrowser *m_fb;
	static CEGUI::String WidgetTypeName;
private:
	void getNewFileName();
};

#endif // TEXTFILEEDITWINDOW_H
