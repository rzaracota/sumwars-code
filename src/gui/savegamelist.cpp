
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
	label->setText("Savegame auswaehlen");

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
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.85f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.10f)));
	btn->setText("Ok");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SavegameList::onSavegameSelected, this));
	btn->setWantsMultiClickEvents(false);
}

void SavegameList::update()
{		
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.getWindow("SavegameList");
	savelist->resetList();
	// Liste aller Files im Save Ordner der Form *.sav
	Ogre::StringVectorPtr files = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Savegame","*.sav");
	
	std::ifstream file;
	char head[50];
	int n=0;
	string name;
	char lev;
	std::string cl;
	string classname;
	std::ostringstream stream;
	char* bp;
	// iterieren ueber die Files
	for (Ogre::StringVector::iterator it = files->begin(); it!= files->end();++it)
	{
		DEBUG5("file found %s",it->c_str());
		//File oeffnen

		file.open(("../save/"+(*it)).c_str(),std::ios::in| std::ios::binary);
		if (file.is_open())
		{
			savelist->addRow();

			// Daten einlesen
			for (int i=0;i<50;i++)
			{
				file.get(head[i]);
			}

			// nicht benoetigte Headerdaten ueberspringen
			bp = head+7;

			char ctmp[11];
			ctmp[10] = '\0';
			strncpy(ctmp,bp,10);
			cl = ctmp;

			// Level einlesen
			bp +=10;
			DEBUG5("bp pointer: %i",bp-head);
			lev = *(bp+32);
			DEBUG5("level: %i",lev);
			//hexwrite(bp,33);
			stream.str("");
			stream << (int) lev;
			savelist->setItem(new ListItem(stream.str()),2,n);

			// Klasse einlesen
			if (cl=="warrior")
				classname = "Krieger";
			if (cl=="mage")
				classname = "Magier";
			if (cl=="archer")
				classname = "Bogenschuetze";
			if (cl=="priest")
				classname = "Priester";
			savelist->setItem(new ListItem(classname),1,n);

			name = bp;
			savelist->setItem(new SaveListItem(name,(*it)),0,n);
			n++;

			file.close();
		}
	}
	
	
}

bool SavegameList::onSavegameChosen(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::MultiColumnList* savelist = (CEGUI::MultiColumnList*) win_mgr.getWindow("SavegameList");

	CEGUI::ListboxItem * itm = savelist->getFirstSelectedItem();


	if (itm !=0)
	{
		SaveListItem * sitm = (SaveListItem *) itm;
		m_document->setSaveFile(sitm->m_filename);
		DEBUG("selected Savegame %s", sitm->m_filename.c_str());
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

