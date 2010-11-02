#include "creditswindow.h"
#include "ceguiutility.h"
#include "OgreConfigFile.h"

CreditsWindow::CreditsWindow(Document* doc)
	:Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::FrameWindow* creditsframe = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "CreditsWindow");
	
	creditsframe->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.1f))); //0.0/0.8
	creditsframe->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.8f))); //1.0/0.2
	creditsframe->setProperty("FrameEnabled","false");
	creditsframe->setProperty("TitlebarEnabled","false");
	creditsframe->setProperty("CloseButtonEnabled","false");
	creditsframe->setAlpha(0.4);
	
	m_window = creditsframe;
	
	CEGUI::ScrollablePane* pane = static_cast<CEGUI::ScrollablePane*> (win_mgr.createWindow("TaharezLook/ScrollablePaneNoBar", "CreditsPane"));
	creditsframe->addChildWindow(pane);
	pane->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));
	pane->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
	pane->setInheritsAlpha(false);
	pane->setContentPaneAutoSized(true);
	
	CEGUI::Window* credits;
	credits = win_mgr.createWindow("TaharezLook/StaticText", "CreditWindow");
	pane->addChildWindow(credits);
	credits->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
	credits->setSize(CEGUI::UVector2(cegui_reldim(1.00f), cegui_reldim( 1.0f)));
	credits->setProperty("FrameEnabled", "false");
	credits->setProperty("BackgroundEnabled", "true");
	credits->setProperty("HorzFormatting", "HorzCentred");

	Ogre::ConfigFile cf;
#ifndef __APPLE__
	cf.load("authors.txt");
#else
    Ogre::String path;
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char resPath[PATH_MAX];
    CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)resPath, PATH_MAX);
    CFRelease(resourcesURL);
    path = resPath;
    cf.load(path + "/authors.txt");
#endif

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	std::list<std::string> content;

	std::string secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		secName = secName.erase(0,2) + LINE_ENDING;
		content.push_back(CEGUIUtility::getColourizedString(CEGUIUtility::Red, secName, CEGUIUtility::White).c_str());
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			content.push_back(archName + LINE_ENDING);
		}
	}
	
	std::string added;
	CEGUI::UVector2 sz = credits->getSize();
	sz.d_y = CEGUIUtility::getWindowSizeForText(content, credits->getFont(), added).d_y;
	credits->setSize(sz);
	credits->setText(added);
	credits->setProperty("BackgroundColours", "tl:99000000 tr:99000000 bl:99000000 br:99000000");
	credits->setAlpha(0.9);
	credits->setFont("DejaVuSerif-12");
	
	updateTranslation();
	
	m_shown_timer.start();
}

void CreditsWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* wtext = win_mgr.getWindow("CreditWindow");
	
	
	CEGUI::Font* fnt = wtext->getFont();
	
	// Set Size of the Window automatically
	CEGUI::UVector2 size = wtext->getSize();
	CEGUI::Rect isize = wtext->getUnclippedInnerRect ();
	//float height = PixelAligned(CEGUIUtility::fitTextToWindow(wtext->getText(), isize.getWidth(), CEGUIUtility::WordWrapCentred, fnt).lines * fnt->getLineSpacing());
	//float height = CEGUIUtility::getWindowSizeForText(wtext->getText().c_str(), fnt);
	size.d_y = CEGUIUtility::getWindowSizeForText(wtext->getText().c_str(), fnt).d_y; //CEGUI::UDim(0.0, height);
	wtext->setSize(size);
	
}

void CreditsWindow::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	if (!m_window->isVisible())
	{
		m_shown_timer.start();
	}
	else
	{
		float starttime = 2000;	// time before scrolling starts
		float alltime = 20000;	// time for scrolling
		float pos = (m_shown_timer.getTime()-starttime)/alltime;
		
		if (pos > 1.0)
			pos = 1.0;
		
		
		// credits scrolling
		CEGUI::Window* wtext = win_mgr.getWindow("CreditWindow");
		CEGUI::ScrollablePane* pane  = static_cast<CEGUI::ScrollablePane*>(win_mgr.getWindow("CreditsPane"));
		
		CEGUI::Font* fnt = wtext->getFont();

		pane->setVerticalScrollPosition(pos);
	}
}

