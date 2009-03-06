#include "tradewindow.h"

TradeWindow::TradeWindow (Document* doc)
	: ItemWindow(doc)
{
	DEBUG5("setup trade");

	m_big_sheet =0;
	m_medium_sheet =0;
	m_small_sheet =0;
	
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	// Rahmen fuer Inventar Fenster
	CEGUI::FrameWindow* trade = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "TradeWindow");
	m_window = trade;
	
	trade->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
	trade->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.85f)));
	trade->setProperty("FrameEnabled","false");
	trade->setProperty("TitlebarEnabled","false");
	trade->setProperty("CloseButtonEnabled","false");
	trade->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	trade->setWantsMultiClickEvents(false);
	trade->setAlpha(0.7);

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;

	int i,j;

	// Label fuer grosse Items
	std::ostringstream outStream;
	
	for (j=0;j<3;j++)
	{
		for (i=0;i<5;i++)
		{
			outStream.str("");
			outStream << "TraderBigItem" << j*5+i<< "Label";
			label = win_mgr.createWindow("TaharezLook/StaticImage", outStream.str());
			trade->addChildWindow(label);
			label->setProperty("FrameEnabled", "true");
			label->setProperty("BackgroundEnabled", "true");
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f+i*0.18f), cegui_reldim( 0.02f +j*0.1f)));
			label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.09f)));
			label->setID(Equipement::BIG_ITEMS+j*5+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&TradeWindow::onTradeItemMouseButtonPressed, this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&TradeWindow::onTradeItemMouseButtonReleased,  this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&TradeWindow::onTradeItemHover,  this));
		}
	}
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "TradeBigPrevButton"));
	trade->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.32f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.05f)));
	btn->setID(0);
	btn->setText("<<");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TradeWindow::onTradeNextItems, this));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "TradeBigNextButton"));
	trade->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.85f), cegui_reldim( 0.32f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.05f)));
	btn->setID(1);
	btn->setText(">>");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TradeWindow::onTradeNextItems, this));

	
	for (j=0;j<3;j++)
	{
		for (i=0;i<7;i++)
		{
			outStream.str("");
			outStream << "TraderMediumItem" << j*7+i<< "Label";
			label = win_mgr.createWindow("TaharezLook/StaticImage", outStream.str());
			trade->addChildWindow(label);
			label->setProperty("FrameEnabled", "true");
			label->setProperty("BackgroundEnabled", "true");
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f+i*0.13f), cegui_reldim( 0.40f+0.085*j)));
			label->setSize(CEGUI::UVector2(cegui_reldim(0.115f), cegui_reldim( 0.075f)));
			label->setID(Equipement::MEDIUM_ITEMS+j*7+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&TradeWindow::onTradeItemMouseButtonPressed, this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&TradeWindow::onTradeItemMouseButtonReleased,  this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&TradeWindow::onTradeItemHover,  this));
		}
	}
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "TradeMediumPrevButton"));
	trade->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.05f)));
	btn->setID(2);
	btn->setText("<<");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TradeWindow::onTradeNextItems, this));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "TradeMediumNextButton"));
	trade->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.85f), cegui_reldim( 0.65f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.05f)));
	btn->setID(3);
	btn->setText(">>");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TradeWindow::onTradeNextItems, this));
	
	for (j=0;j<3;j++)
	{
		for (i=0;i<10;i++)
		{
			outStream.str("");
			outStream << "TraderSmallItem" << j*10+i<< "Label";
			label = win_mgr.createWindow("TaharezLook/StaticImage", outStream.str());
			trade->addChildWindow(label);
			label->setProperty("FrameEnabled", "true");
			label->setProperty("BackgroundEnabled", "true");
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.02f+i*0.096f), cegui_reldim( 0.72f+0.065*j)));
			label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.056f)));
			label->setID(Equipement::SMALL_ITEMS+j*10+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&TradeWindow::onTradeItemMouseButtonPressed, this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&TradeWindow::onTradeItemMouseButtonReleased,  this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&TradeWindow::onTradeItemHover,  this));
		}
	}
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "TradeSmallPrevButton"));
	trade->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.92f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.05f)));
	btn->setID(4);
	btn->setText("<<");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TradeWindow::onTradeNextItems, this));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "TradeSmallNextButton"));
	trade->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.85f), cegui_reldim( 0.92f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.05f)));
	btn->setID(5);
	btn->setText(">>");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TradeWindow::onTradeNextItems, this));
	
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "TradeCloseButton"));
	trade->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.45f), cegui_reldim( 0.94f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.05f)));
	btn->setID(5);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TradeWindow::onCloseTrade, this));
	
	updateTranslation();
}

void TradeWindow::update()
{
	
}

void TradeWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("TradeCloseButton"));
	btn->setText(gettext("OK"));
}


bool TradeWindow::onTradeItemHover(const CEGUI::EventArgs& evt)
{
	return true;
}

bool TradeWindow::onTradeItemMouseButtonPressed(const CEGUI::EventArgs& evt)
{
	return true;
}

bool TradeWindow::onTradeItemMouseButtonReleased(const CEGUI::EventArgs& evt)
{
	return true;
}

bool TradeWindow::onTradeNextItems(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
		static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	if (id ==0) 
	{
		m_big_sheet--;
	}
	else if (id ==1) 
	{
		m_big_sheet++;
	}
	else if (id ==2) 
	{
		m_medium_sheet--;
	}
	else  if (id ==3) 
	{
		m_medium_sheet++;
	}
	else  if (id ==4) 
	{
		m_small_sheet--;
	}
	else  if (id ==5) 
	{
		m_small_sheet++;
	}
	
	return true;
}

bool TradeWindow::onCloseTrade(const CEGUI::EventArgs& evt)
{
	m_document->onButtonCloseTrade();
	return true;
	
}



