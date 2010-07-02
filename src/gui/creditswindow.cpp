#include "creditswindow.h"

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
	creditsframe->setAlpha(0.0);
	
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
	credits->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.00f)));
	credits->setSize(CEGUI::UVector2(cegui_reldim(1.00f), cegui_reldim( 1.0f)));
	credits->setProperty("FrameEnabled", "false");
	credits->setProperty("BackgroundEnabled", "true");
	credits->setProperty("HorzFormatting", "HorzCentred");
	credits->setMaxSize(CEGUI::UVector2(cegui_reldim(1.00f), cegui_reldim(10.0f)));
	
	std::string content = "Programming: \
			\n Hans Wulf (Lastmerlin) \
			\n fusion44 \
			\n Nicholas Cosens (gnemo) \
            \n\nConversion to Mac: \
            \n David Athay (trapdoor) \
            \n Ben Anderman (crimson_penguin) \
			\n\nGraphics: \
			\n Andreas Schönefeldt \
			\n Steffen Schönefeldt (Nori) \
			\n psycho \
			\n Francesco Miglietta (hal9000 ) \
			\n\n Scripting: \
			\n Steffen Schönefeldt (Nori) \
			\n Hans Wulf (Lastmerlin) \
			\n\nStory: \
			\n Steffen Schönefeldt (Nori) \
			\n Andreas Schönefeldt \
			\n\nSounds: \
			\n Michael Kempf (Hangman) \
			\n artisticdude \
			\n\n Translation: \
			\n Steffen Schönefeldt (Nori) \
			\n Michael Kempf (Hangman) \
			\n kroni \
			\n Brian Jeffears (getter77) \
			\n Nicholas Cosens (gnemo) \
			\n\n Build system: \
			\n Michael Kempf (Hangman) \
			\n fusion44 \
			\n\nWebsite: \
			\n Andreas Schönefeldt \
			\n Michael Kempf (Hangman)";
	
	credits->setText((CEGUI::utf8*) content.c_str());
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
	float height = PixelAligned(fnt->getFormattedLineCount(wtext->getText(), isize, CEGUI::WordWrapCentred) * fnt->getLineSpacing());
	size.d_y = CEGUI::UDim(0.0, height);
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
		
		if (pos < 0)
			pos = 0;
		
		if (pos > 1.0)
			pos = 1.0;
		
		
		// credits scrolling
		CEGUI::Window* wtext = win_mgr.getWindow("CreditWindow");
		CEGUI::ScrollablePane* pane  = static_cast<CEGUI::ScrollablePane*>(win_mgr.getWindow("CreditsPane"));
		
		CEGUI::Font* fnt = wtext->getFont();
		
		pane->setVerticalScrollPosition(pos);
	}
}

