/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "creditswindow.h"
#include "ceguiutility.h"
#include "OgreConfigFile.h"
#include "OgreDataStream.h"
#include "sumwarshelper.h"

#include "config.h"

#include "creditswindow_content.inc"

CreditsWindow::CreditsWindow(Document* doc, const std::string& ceguiSkinName)
	: Window (doc)
	, m_ceguiSkinName (ceguiSkinName)
{
	DEBUG ("Creating CreditsWindow using the cegui skin: [%s]", ceguiSkinName.c_str ());

	std::stringstream ss;
	//ss << ceguiSkinName << "/FrameWindow";
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::FrameWindow* creditsframe = (CEGUI::FrameWindow*) win_mgr.createWindow(
		CEGUIUtility::getWidgetWithSkin (ceguiSkinName, "FrameWindow"), "CreditsWindow");
	
	creditsframe->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.1f))); //0.0/0.8
	CEGUIUtility::setWidgetSizeRel (creditsframe, 0.5f, 0.8f);
	creditsframe->setProperty("FrameEnabled","false");
	creditsframe->setProperty("TitlebarEnabled","false");
	creditsframe->setProperty("CloseButtonEnabled","false");
	creditsframe->setAlpha(0.4);
	
	m_window = creditsframe;
	
	ss.str ("");
	//ss << ceguiSkinName << "/ScrollablePaneNoBar";
	CEGUI::ScrollablePane* pane = static_cast<CEGUI::ScrollablePane*> (win_mgr.createWindow(
		CEGUIUtility::getWidgetWithSkin (ceguiSkinName, "ScrollablePaneNoBar"), "CreditsPane"));
	CEGUIUtility::addChildWidget (creditsframe, pane);
	pane->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));
	CEGUIUtility::setWidgetSizeRel (pane, 1.0f, 1.0f);
	pane->setInheritsAlpha(false);
	pane->setContentPaneAutoSized(true);
	
	ss.str ("");
	//ss << ceguiSkinName << "/StaticText";

	CEGUI::Window* credits;
	credits = win_mgr.createWindow (CEGUIUtility::getWidgetWithSkin (ceguiSkinName, "StaticText"), "CreditsText");
	CEGUIUtility::addChildWidget (pane, credits);
	credits->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
	CEGUIUtility::setWidgetSizeRel (credits, 1.0f, 1.0f);
	credits->setProperty("FrameEnabled", "false");
	credits->setProperty("BackgroundEnabled", "true");
	credits->setProperty("HorzFormatting", "HorzCentred");

	Ogre::DataStreamPtr mem_stream(OGRE_NEW Ogre::MemoryDataStream((void*)authors_content.c_str(), authors_content.length(), false, true));
	mem_stream->seek(0);

	Ogre::ConfigFile cf;
	cf.load(mem_stream, "=", false);

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
		content.push_back(std::string(" ") + LINE_ENDING);
	}
	
	credits->setFont("DejaVuSerif-12");
	
	std::string added;
	CEGUI::UVector2 sz = CEGUIUtility::getWidgetSize (credits);
	sz.d_y = CEGUIUtility::getWindowSizeForText(content, credits->getFont(), added).d_y;
	CEGUIUtility::setWidgetSize (credits, sz);
	CEGUIUtility::setWidgetMaxSize (credits, CEGUI::UVector2(CEGUI::UDim(5.0,5.0), CEGUI::UDim(5.0,5.0)));
	credits->setText(added);
	if (credits->isPropertyPresent ("BackgroundColours"))
	{
		credits->setProperty("BackgroundColours", "tl:99000000 tr:99000000 bl:99000000 br:99000000");
	}
	else if (credits->isPropertyPresent ("BackgroundColour"))
	{
		credits->setProperty("BackgroundColour", "99000000");
	}
	credits->setAlpha(0.9);
	
	
	updateTranslation();
	
	m_shown_timer.start();
}

void CreditsWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	std::string widgetName = CEGUIUtility::getNameForWidget("CreditsWindow");
	CEGUI::FrameWindow* wtext = (CEGUI::FrameWindow*) (CEGUIUtility::getWindowForLoadedLayout(m_window, widgetName));

	
	const CEGUI::Font* fnt = wtext->getFont();
	
	// Set Size of the Window automatically
	CEGUI::UVector2 credSize = CEGUIUtility::getWidgetSize (wtext);
	//CEGUIUtility::Rectf isize = wtext->getUnclippedInnerRect ();
	//float height = PixelAligned(CEGUIUtility::fitTextToWindow(wtext->getText(), isize.getWidth(), CEGUIUtility::WordWrapCentred, fnt).lines * fnt->getLineSpacing());
	credSize.d_y = CEGUIUtility::getWindowSizeForText(wtext->getText().c_str(), fnt).d_y; //CEGUI::UDim(0.0, height);
	
	// FIXME: this factor is a hack to achieve the right window size. Size computation is not correct yet
	credSize.d_y.d_scale *= 1.2;
	CEGUIUtility::setWidgetSize (wtext, credSize);
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
		float alltime = 40000;	// time for scrolling
		float pos = (m_shown_timer.getTime()-starttime)/alltime;
		
		if (pos > 1.0)
			pos = 1.0;
		
		if (pos < 0.0)
			pos = 0.0;
		
		// credits scrolling
		CEGUI::ScrollablePane* pane  = static_cast<CEGUI::ScrollablePane*>(CEGUIUtility::getWindow ("CreditsPane"));

		pane->setVerticalScrollPosition(pos);
	}
}

