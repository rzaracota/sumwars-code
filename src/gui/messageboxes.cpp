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

MessageQuestionWindow::MessageQuestionWindow (Document* doc, std::string name,std::string question, std::string button1,CEGUI::Event::Subscriber subscriber1,  std::string button2, CEGUI::Event::Subscriber subscriber2)
	:Window(doc)
{
	m_button1 = button1;
	m_button2 = button2;
	m_question = question;
	
	std::string wname = name;
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	
	CEGUI::FrameWindow* message = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", wname);
	m_window = message;
	
	
	message->setPosition(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.25f))); //0.0/0.8
	message->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.2f))); //1.0/0.2
	message->setProperty("FrameEnabled","false");
	message->setProperty("TitlebarEnabled","false");
	message->setProperty("CloseButtonEnabled","false");
	message->setVisible(false);
	
	wname = name;
	wname += "Button1";
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", wname));
	message->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.6f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.3f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, subscriber1);
	
	wname = name;
	wname += "Button2";
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", wname));
	message->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.60f), cegui_reldim( 0.6f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.3f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, subscriber2);
	
	wname = name;
	wname += "Label";
	label = win_mgr.createWindow("TaharezLook/StaticText", wname);
	message->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim(0.1f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.3f)));	
	
	m_name = name;
	updateTranslation();
	
}

void MessageQuestionWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	std::string wname = m_name;
	wname += "Button1";
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( wname));
	btn->setText((CEGUI::utf8*) gettext(m_button1.c_str()));
	
	wname = m_name;
	wname += "Button2";
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( wname));
	btn->setText((CEGUI::utf8*) gettext(m_button2.c_str()));
	
	wname = m_name;
	wname += "Label";
	label = win_mgr.getWindow(wname);
	label->setText((CEGUI::utf8*) gettext(m_question.c_str()));
}

void MessageQuestionWindow::setQuestion(std::string question)
{
	m_question = question;
	updateTranslation();
}

ErrorDialogWindow::ErrorDialogWindow (Document* doc)
:Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	
	CEGUI::FrameWindow* error_dialog = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "ErrorDialogWindow");
	m_window = error_dialog;
	m_error = "Network connection timed out";
	
	error_dialog->setPosition(CEGUI::UVector2(cegui_reldim(0.3f), cegui_reldim( 0.25f))); //0.0/0.8
	error_dialog->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.2f))); //1.0/0.2
	error_dialog->setProperty("FrameEnabled","false");
	error_dialog->setProperty("TitlebarEnabled","false");
	error_dialog->setProperty("CloseButtonEnabled","false");
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "ErrorDialogConfirmButton"));
	error_dialog->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.6f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.3f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ErrorDialogWindow ::onConfirmed, this));
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "ErrorDialogLabel");
	error_dialog->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim(0.1f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.75f), cegui_reldim( 0.3f)));
	
	updateTranslation();
}

void ErrorDialogWindow::update()
{
}

void ErrorDialogWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "ErrorDialogConfirmButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
	
	label = win_mgr.getWindow("ErrorDialogLabel");
	label->setText((CEGUI::utf8*) gettext(m_error.c_str()));
}


bool ErrorDialogWindow::onConfirmed(const CEGUI::EventArgs& evt)
{
	m_document->onButtonErrorDialogConfirm();
	return true;
}

void ErrorDialogWindow::setError(std::string error)
{
	m_error = error;
	updateTranslation();
}

