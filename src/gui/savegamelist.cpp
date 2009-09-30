#include "OgreResourceGroupManager.h"
#include "savegamelist.h"


SavegameList::SavegameList (Document* doc)
	:Window(doc)
{
	DEBUG4("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	

	// Rahmen fuer das Menue Savegame auswaehlen
	CEGUI::FrameWindow* save_menu = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "SavegameMenu");
	m_window = save_menu;
	
	save_menu->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f))); //0.0/0.8
	save_menu->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 1.0f))); //1.0/0.2
	save_menu->setProperty("FrameEnabled","false");
	save_menu->setProperty("TitlebarEnabled","false");
	save_menu->setProperty("CloseButtonEnabled","false");

	// Bestandteile der Kontrollleiste hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;

	// Label Savegame waehlen
	label = win_mgr.createWindow("TaharezLook/StaticText", "SavegameChooseLabel");
	save_menu->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.1f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.1f)));
	
	CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.createWindow("TaharezLook/MultiColumnList", "SavegameList");
	save_menu->addChildWindow(savelist);
	savelist->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.2f)));
	savelist->setSize(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.6f)));

	savelist->setSelectionMode(CEGUI::MultiColumnList::RowSingle);
	savelist->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged, CEGUI::Event::Subscriber(&SavegameList::onSavegameChosen, this));

	savelist->addColumn("Name",0,CEGUI::UDim(0.5,0));
	savelist->addColumn("Klasse",1,CEGUI::UDim(0.3,0));
	savelist->addColumn("Level",2,CEGUI::UDim(0.2,0));

	// Button Savegame akzeptieren
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SelectSavegameButton"));
	save_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.92f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.05f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SavegameList::onSavegameSelected, this));
	btn->setWantsMultiClickEvents(false);
	
	// Button neu
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "NewCharButton"));
	save_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim( 0.92f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.6f), cegui_reldim( 0.05f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SavegameList::onNewCharClicked, this));
	btn->setWantsMultiClickEvents(false);
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "DeleteCharButton"));
	save_menu->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.35f), cegui_reldim( 0.83f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.6f), cegui_reldim( 0.05f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SavegameList::onDeleteCharClicked, this));
	btn->setWantsMultiClickEvents(false);
	
	updateTranslation();
}

void SavegameList::update()
{	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.getWindow("SavegameList");
	savelist->resetList();
	savelist->setSortDirection( CEGUI::ListHeaderSegment::None);
	
	// Liste aller Files im Save Ordner der Form *.sav
	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("Savegame","*.sav");
	
	std::fstream file;
	char bin;
	int n=0;
	char lev;
	std::string cl;
	
	std::string name;
	std::string classname;
	std::ostringstream stream;
	std::string filename;
	PlayerLook look;
	int version;
	// iterieren ueber die Files
	unsigned char* data;
	for (it = files->begin(); it!= files->end();++it)
	{
		filename = it->archive->getName();
		filename += "/";
		filename += it->filename;
		DEBUG("file found %s",filename.c_str());
		//File oeffnen

		file.open(filename.c_str(),std::ios::in| std::ios::binary);
		if (file.is_open())
		{
			savelist->addRow();

			file.get(bin);
			
			CharConv* save;
			data =0;
			if (bin == '0')
			{
				save = new CharConv(&file);
			}
			else
			{
				int len;
				file.read((char*) &len,4);
				data = new unsigned char[len];
				file.read((char*) data,len);
			
				save = new CharConv(data,len);
			}
			
			save->fromBuffer(version);
			save->setVersion(version);
			
			save->fromBuffer(classname);
			save->fromBuffer(name);
			look.fromString(save);
			save->fromBuffer(lev);
			
			stream.str("");
			stream << (int) lev;
			savelist->setItem(new ListItem(stream.str()),2,n);
			savelist->setItem(new ListItem(classname),1,n);
			savelist->setItem(new StrListItem(name,filename),0,n);
			n++;

			file.close();
			
			delete save;
			if (data != 0)
				 delete data;
		}
	}
	
	
}

void SavegameList::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Window* label;

	label = win_mgr.getWindow("SavegameChooseLabel");
	label->setText((CEGUI::utf8*) gettext("Savegame"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("SelectSavegameButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("NewCharButton"));
	btn->setText((CEGUI::utf8*) gettext("Create Character"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("DeleteCharButton"));
	btn->setText((CEGUI::utf8*) gettext("Delete Character"));
	
	CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.getWindow("SavegameList");
	savelist->getHeaderSegmentForColumn(0).setText((CEGUI::utf8*) gettext("Name"));
	savelist->getHeaderSegmentForColumn(1).setText((CEGUI::utf8*) gettext("Class"));
	savelist->getHeaderSegmentForColumn(2).setText((CEGUI::utf8*) gettext("Level"));
	
}

bool SavegameList::onSavegameChosen(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.getWindow("SavegameList");

	CEGUI::ListboxItem * itm = savelist->getFirstSelectedItem();


	if (itm !=0)
	{
		StrListItem * sitm = (StrListItem *) itm;
		m_document->setSaveFile(sitm->m_data);
		DEBUG5("selected Savegame %s", sitm->m_data.c_str());
	}

	return true;
}

bool SavegameList::onSavegameSelected(const CEGUI::EventArgs& evt)
{
	if (m_document->getSaveFile() != "")
	{
		DEBUG("savegame accepted %s", m_document->getSaveFile().c_str());
		m_document->getGUIState()->m_shown_windows = Document::START_MENU;
		m_document->setModified(Document::WINDOWS_MODIFIED);
	}
	return true;
}

bool SavegameList::onNewCharClicked(const CEGUI::EventArgs& evt)
{
	m_document->onCreateNewCharButton();
	return true;
}


bool SavegameList::onDeleteCharClicked(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.getWindow("SavegameList");

	CEGUI::ListboxItem * itm = savelist->getFirstSelectedItem();


	if (itm !=0)
	{
		StrListItem * sitm = (StrListItem *) itm;
		DEBUG("delete Savegame %s", sitm->m_data.c_str());
		
		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
		CEGUI::FrameWindow* message = (CEGUI::FrameWindow*) win_mgr.getWindow("DeleteChar");
	
		message->setVisible(true);
		message->setModalState(true);
	}

	return true;
}

bool SavegameList::onDeleteCharConfirmClicked(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::FrameWindow* message = (CEGUI::FrameWindow*) win_mgr.getWindow("DeleteChar");
	
	message->setVisible(false);
	message->setModalState(false);
	
	remove(m_document->getSaveFile().c_str());
	m_document->setSaveFile("");
	
	update();
	return true;
}

bool SavegameList::onDeleteCharAbortClicked(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::FrameWindow* message = (CEGUI::FrameWindow*) win_mgr.getWindow("DeleteChar");
	
	message->setVisible(false);
	message->setModalState(false);
	return true;
}

