#include "filebrowser.h"
#include "listitem.h"

#include <list>
#include <fstream>

#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Exception.h"

using namespace CEGUI;

FileBrowser::~FileBrowser()
{
	m_winManager->destroyWindow(m_rootWindow);
}

void FileBrowser::init(CEGUI::String defaultDir, FileBrowserType type, bool visible)
{
	m_currentPath = Poco::Path::current();

	m_type = type;
	m_guiSystem = System::getSingletonPtr();
	m_winManager = WindowManager::getSingletonPtr();
	m_gameScreen = m_winManager->getWindow("GameScreen");

	m_rootWindow = m_winManager->loadWindowLayout("FileBrowser.layout");
	m_gameScreen->addChildWindow(m_rootWindow);
	m_rootWindow->setVisible(visible);

	m_acceptBtn = static_cast<PushButton*>(m_rootWindow->getChild("Ok"));
	m_cancelBtn = static_cast<PushButton*>(m_rootWindow->getChild("Cancel"));
	CEGUI::PushButton *btn = static_cast<PushButton*>(m_rootWindow->getChild("PopDir"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(&FileBrowser::handlePopDirectory, this));

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
	m_pathBox->setText(m_currentPath.toString());

	m_fileNameBox = static_cast<Editbox*>(m_rootWindow->getChild("FileName"));
	
	m_browserBox = static_cast<MultiColumnList*>(m_rootWindow->getChild("Browser"));
	m_browserBox->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged, Event::Subscriber(&FileBrowser::handleSelectionChanged, this));
	m_browserBox->subscribeEvent(CEGUI::MultiColumnList::EventMouseDoubleClick, CEGUI::Event::Subscriber(&FileBrowser::handleBrowserDblClick, this));
	m_browserBox->addColumn("", 0, CEGUI::UDim(0.8, 0));
	m_browserBox->addColumn("Type", 1, CEGUI::UDim(0.2, 0));
	m_browserBox->setSelectionMode(CEGUI::MultiColumnList::RowSingle);
	
	fillBrowser();
}

void FileBrowser::destroy()
{
	m_winManager->destroyWindow(m_rootWindow);
}


void FileBrowser::fillBrowser()
{
	m_dirs.clear();
	m_files.clear();
	m_browserBox->resetList();

	Poco::DirectoryIterator it(m_currentPath);
	Poco::DirectoryIterator end;

	while (it != end)
	{
		try
		{
			if (it->isFile())
				m_files.push_back(it.name());
			else
				m_dirs.push_back(it.name());
			++it;
		}
		catch (Poco::Exception& exc)
		{
			std::cout << exc.displayText() << std::endl;
			++it;
		}
	}


	m_dirs.sort();
	m_files.sort();

	std::list<std::string>::iterator sit;
	for (sit=m_dirs.begin(); sit!=m_dirs.end(); ++sit)
	{
		int id = m_browserBox->addRow();
		StrListItem* ite1 = new StrListItem(*sit, "2");
		StrListItem* ite2 = new StrListItem("Directory", "4");
		m_browserBox->setItem(ite1, 0, id);
		m_browserBox->setItem(ite2, 1, id);
	}
	for (sit=m_files.begin(); sit!=m_files.end(); ++sit)
	{
		int id = m_browserBox->addRow();
		StrListItem* ite1 = new StrListItem(*sit, "f2");
		StrListItem* ite2 = new StrListItem("File", "f4");
		m_browserBox->setItem(ite1, 0, id);
		m_browserBox->setItem(ite2, 1, id);
	}
}


bool FileBrowser::handleBrowserDblClick(const CEGUI::EventArgs &e)
{
	try
	{
		m_currentPath.pushDirectory(m_browserBox->getFirstSelectedItem()->getText().c_str());
		m_pathBox->setText(m_currentPath.toString());
		fillBrowser();
	}
	catch (Poco::Exception& exc)
	{
		std::cout << exc.displayText() << std::endl;
		return true;
	}
	return true;
}

bool FileBrowser::handlePopDirectory(const CEGUI::EventArgs &e)
{
	try
	{
		m_currentPath.popDirectory();
		m_pathBox->setText(m_currentPath.toString());
		fillBrowser();
	}
	catch (Poco::Exception& exc)
	{
		std::cout << exc.displayText() << std::endl;
		return true;
	}
	return true;
}

bool FileBrowser::handleSelectionChanged(const CEGUI::EventArgs& e)
{
	/*if(m_browserBox->getFirstSelectedItem() != 0)
	{
		CEGUI::String selectedItem = m_browserBox->getFirstSelectedItem()->getText();
		CEGUI::String dir = m_pathBox->getText();
		
		if(fileExists((dir + "/" + selectedItem).c_str()))
			m_fileNameBox->setText(selectedItem);
		else
			m_fileNameBox->setText("");
	}*/
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
	return "";
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
