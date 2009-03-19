#include "chatline.h"

ChatLine::ChatLine (Document* doc)
	: Window(doc)
{
	m_history_line = 0;
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// Rahmen fuer CharInfo Fenster
	CEGUI::FrameWindow* chat = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "ChatWindow");
	m_window = chat;
	
	
	chat->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.68f)));
	chat->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 0.2f)));
	chat->setProperty("FrameEnabled","false");
	chat->setProperty("TitlebarEnabled","false");
	chat->setProperty("CloseButtonEnabled","false");
	chat->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	chat->setWantsMultiClickEvents(false);
	chat->setAlpha(0.7);
	
	CEGUI::Editbox* chatline;
	
	chatline = static_cast<CEGUI::Editbox*>(win_mgr.createWindow("TaharezLook/Editbox", "Chatline"));
 	chat->addChildWindow(chatline);
	chatline->setPosition(CEGUI::UVector2(cegui_reldim(0.00f), cegui_reldim( 0.8f)));
	chatline->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 0.20f)));
	chatline->setWantsMultiClickEvents(false);
	chatline->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&ChatLine::onSendMessage,  this));
	chatline->subscribeEvent(CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber(&ChatLine::onKeyPress,  this));
	
	chatline->setText("");
	
	
	CEGUI::MultiLineEditbox* chat_content;
	
	chat_content = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.createWindow("TaharezLook/MultiLineEditbox", "ChatContent"));
	chat->addChildWindow(chat_content);
	chat_content->setPosition(CEGUI::UVector2(cegui_reldim(0.00f), cegui_reldim( 0.0f)));
	chat_content->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 0.78f)));
	chat_content->setWantsMultiClickEvents(false);
	chat_content->setReadOnly(true);
	chat_content->setText("");
	
	updateTranslation();
	
}

void ChatLine::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	Player* pl = m_document->getLocalPlayer();
	
	CEGUI::MultiLineEditbox* chat_content;
	chat_content =  static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("ChatContent"));
	
	if (chat_content->getText() != pl->getMessages())
	{
		chat_content->setText(pl->getMessages());
		CEGUI::Scrollbar* sb = chat_content->getVertScrollbar();
		if (sb !=0)
		{
			sb->setScrollPosition(sb->getDocumentSize());
		}
	}
}

void ChatLine::updateTranslation()
{

}

void ChatLine::setHistoryLine()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Editbox* line = static_cast<CEGUI::Editbox*>(win_mgr.getWindow( "Chatline"));
	
	if (m_history_line == 0)
	{
		line->setText("");
	}
	else
	{
		// Zeile aus der History suchen
		std::list<std::string>::iterator it;
		it = m_history.begin();
		for (int i=1; i<m_history_line && it != m_history.end(); ++i)
		{
			++it;
		}
		
		if (it != m_history.end())
		{
			line->setText(it->c_str());
		}
		else
		{
			line->setText(m_history.back().c_str());
		}
	}
}

bool ChatLine::onSendMessage(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	
	CEGUI::Editbox* line = static_cast<CEGUI::Editbox*>(we.window);
	
	m_document->sendChatMessage(line->getText().c_str());
	
	// einfuegen in History
	m_history.push_front(line->getText().c_str());
	if (m_history.size() >10)
	{
		m_history.pop_back();
	}
	m_history_line = 0;
	
	line->setText("");
	
	return true;
}

bool ChatLine::onKeyPress(const CEGUI::EventArgs& evt)
{
	const CEGUI::KeyEventArgs &kevt = static_cast<const CEGUI::KeyEventArgs&>(evt);
	
	if (kevt.scancode == CEGUI::Key::ArrowUp)
	{
		if (m_history_line<(int) m_history.size())
		{
			m_history_line++;
		}
		setHistoryLine();
		return true;
	}
	else if (kevt.scancode == CEGUI::Key::ArrowDown)
	{
		
		if (m_history_line>0)
		{
			m_history_line--;
		}
		setHistoryLine();
		return true;
	}
	
	return false;
}
