#include "messageboxes.h"

SaveExitWindow::SaveExitWindow (Document* doc)
	:Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	
	CEGUI::FrameWindow* save_exit = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "SaveExitWindow");
	m_window = save_exit;
	
	
	save_exit->setPosition(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.25f))); //0.0/0.8
	save_exit->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.2f))); //1.0/0.2
	save_exit->setProperty("FrameEnabled","false");
	save_exit->setProperty("TitlebarEnabled","false");
	save_exit->setProperty("CloseButtonEnabled","false");
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "GameExitConfirmButton"));
	save_exit->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.6f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.3f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SaveExitWindow ::onExitGameConfirmed, this));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "GameExitAbortButton"));
	save_exit->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.60f), cegui_reldim( 0.6f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.3f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SaveExitWindow ::onExitGameAborted, this));
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "SaveExitLabel");
	save_exit->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim(0.1f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.3f)));	
	
	
	updateTranslation();
}

void SaveExitWindow::update()
{
}

void SaveExitWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "GameExitConfirmButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "GameExitAbortButton"));
	btn->setText((CEGUI::utf8*) gettext("Abort"));
	
	label = win_mgr.getWindow("SaveExitLabel");
	label->setText((CEGUI::utf8*) gettext("Save and Exit?"));
}


bool SaveExitWindow::onExitGameConfirmed(const CEGUI::EventArgs& evt)
{
	m_document->onButtonSaveExitConfirm();
	return true;
}

bool SaveExitWindow::onExitGameAborted(const CEGUI::EventArgs& evt)
{
	m_document->onButtonSaveExitAbort();
	return true;
}

