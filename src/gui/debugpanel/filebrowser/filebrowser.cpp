#include "filebrowser.h"
#include <dirent.h>
#include <list>
#include <fstream>

#include  <stdio.h>
#include  <stdlib.h>

using namespace CEGUI;

FileBrowser::~FileBrowser()
{
	m_winManager->destroyWindow(m_rootWindow);
}

void FileBrowser::init(CEGUI::String defaultDir, FileBrowserType type, bool visible)
{
	m_type = type;
	m_guiSystem = System::getSingletonPtr();
	m_winManager = WindowManager::getSingletonPtr();
	m_gameScreen = m_winManager->getWindow("GameScreen");

	m_rootWindow = m_winManager->loadWindowLayout("FileBrowser.layout");
	m_gameScreen->addChildWindow(m_rootWindow);
	m_rootWindow->setVisible(visible);

	m_acceptBtn = static_cast<PushButton*>(m_rootWindow->getChild("Ok"));
	m_cancelBtn = static_cast<PushButton*>(m_rootWindow->getChild("Cancel"));
	
	m_rootWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, Event::Subscriber(&FileBrowser::handleCloseWindow, this));
	
	switch(m_type)
	{
		case FB_TYPE_OPEN_FILE:
			m_rootWindow->setText("Select file");
			m_acceptBtn->setText("Open");
			break;
		case FB_TYPE_SAVE_FILE:
			m_rootWindow->setText("Select file");
			m_acceptBtn->setText("Save");
			break;
		case FB_TYPE_SELECT_DIRECTORY:
			m_rootWindow->setText("Select directory");
			m_acceptBtn->setText("Select");
			break;
		default:
			break;
	}
	
	m_pathBox = static_cast<Editbox*>(m_rootWindow->getChild("CurrentPath"));
	m_pathBox->setText(defaultDir);

	m_fileNameBox = static_cast<Editbox*>(m_rootWindow->getChild("FileName"));
	
	m_browserBox = static_cast<MultiColumnList*>(m_rootWindow->getChild("Browser"));
	m_browserBox->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged, Event::Subscriber(&FileBrowser::handleSelectionChanged, this));
	m_browserBox->subscribeEvent(CEGUI::MultiColumnList::EventMouseDoubleClick, CEGUI::Event::Subscriber(&FileBrowser::handleBrowserDblClick, this));
	m_browserBox->addColumn("", 0, CEGUI::UDim(0.8, 0));
	m_browserBox->addColumn("Type", 1, CEGUI::UDim(0.2, 0));
	m_browserBox->setSelectionMode(CEGUI::MultiColumnList::RowSingle);
	
	fillBrowser(defaultDir);

}

void FileBrowser::destroy()
{
	m_winManager->destroyWindow(m_rootWindow);
}


void FileBrowser::fillBrowser(CEGUI::String inDir)
{
	m_dirs.clear();
	m_files.clear();
	m_browserBox->resetList();

	DIR *dir;
	struct dirent *ent;

	CEGUI::ListboxTextItem *item;

	std::list<std::string> m_dirs;
	std::list<std::string> m_files;
	std::list<std::string>::iterator it;

	/* open directory stream */
	dir = opendir (inDir.c_str());
	if (dir != NULL)
	{

		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL)
		{
			switch (ent->d_type)
			{
			case DT_REG:
				m_files.push_back(ent->d_name);
				//printf ("%*.*s\n", ent->d_namlen, ent->d_namlen, ent->d_name);
				break;

			case DT_DIR:
				m_dirs.push_back(ent->d_name);
				//printf ("%s (dir)\n", ent->d_name);
				break;

			default:
				break;
				//printf ("%s:\n", ent->d_name);
			}
		}

		closedir (dir);
	}

	m_dirs.sort();
	m_files.sort();

	for (it=m_dirs.begin(); it!=m_dirs.end(); ++it)
	{
		if(*it != ".")
		{
			m_dirs.insert(m_dirs.begin(), ".");
			m_dirs.insert(m_dirs.begin(), "..");
		}

		item = new ListboxTextItem(*it);
		int id = m_browserBox->addRow(item, 0);
		
		item = new ListboxTextItem("d");
		m_browserBox->addRow(item, 1, id);
	}
	for (it=m_files.begin(); it!=m_files.end(); ++it)
	{
		item = new ListboxTextItem(*it);
		int id = m_browserBox->addRow(item, 0);
		
		item = new ListboxTextItem("f");
		m_browserBox->addRow(item, 1, id);
	}
}


bool FileBrowser::handleBrowserDblClick(const CEGUI::EventArgs &e)
{
	CEGUI::String newDir;
	ListboxItem *selectedDir = m_browserBox->getFirstSelectedItem();
	CEGUI::String oldDir = m_pathBox->getText();
	
#ifdef WIN32
	CEGUI::String dirDelemiter = "\\";
#else
	CEGUI::String dirDelemiter = "/";
#endif

	if(selectedDir->getText() == "..")
	{
		if(oldDir.find_last_of(dirDelemiter) == oldDir.size())
			oldDir = oldDir.erase(oldDir.size()-1, CEGUI::String::npos);

		int pos = oldDir.find_last_of(dirDelemiter);
		newDir = oldDir.erase(pos, oldDir.size()- pos);
	}
	else
		if(oldDir.find_last_of(dirDelemiter) == oldDir.size()-1)
			newDir = oldDir + selectedDir->getText();
		else
			newDir = oldDir +  dirDelemiter + selectedDir->getText();

	m_pathBox->setText(newDir);

	fillBrowser(newDir + dirDelemiter);

	return true;
}

bool FileBrowser::handleSelectionChanged(const CEGUI::EventArgs& e)
{
	if(m_browserBox->getFirstSelectedItem() != 0)
	{
		CEGUI::String selectedItem = m_browserBox->getFirstSelectedItem()->getText();
		CEGUI::String dir = m_pathBox->getText();
		
		if(fileExists((dir + "/" + selectedItem).c_str()))
			m_fileNameBox->setText(selectedItem);
		else
			m_fileNameBox->setText("");
	}
	return true;
}


CEGUI::String FileBrowser::getCurrentSelected()
{
	CEGUI::String currentDir = m_pathBox->getText() + "/";
	CEGUI::String currentSelectedItemText = m_browserBox->getFirstSelectedItem()->getText();
		
	switch(m_type)
	{
		case FB_TYPE_OPEN_FILE:
			if(fileExists((currentDir + currentSelectedItemText).c_str()))
				return currentDir + currentSelectedItemText;
			else
				break;
		case FB_TYPE_SAVE_FILE:
			return currentDir + currentSelectedItemText;
		case FB_TYPE_SELECT_DIRECTORY:
			return currentDir;
		default:
			return "";
	}
}

bool FileBrowser::handleCloseWindow(const CEGUI::EventArgs& e)
{
	m_rootWindow->hide();
	return true;
}

bool FileBrowser::fileExists(const char *strFilename) 
{
/*#ifdef WIN32
	CEGUI::String dirDelemiter = "\\";
#else
	CEGUI::String dirDelemiter = "/";
#endif
	CEGUI::String temp(strFilename);
	
	int pos = temp.find_last_of(dirDelemiter);
	*/
	std::cout << strFilename << std::endl;
	struct stat stFileInfo;
	stat(strFilename,&stFileInfo);
	if(stFileInfo.st_mode == S_IFDIR)
		std::cout << "dir " << stFileInfo.st_mode << std::endl;
	else
		std::cout << "file " << stFileInfo.st_mode << std::endl;
	
	return true;
}
