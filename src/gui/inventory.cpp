#include "inventory.h"

Inventory::Inventory (Document* doc)
	: ItemWindow(doc)
{
	DEBUGX("setup inventory");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	// Rahmen fuer Inventar Fenster
	CEGUI::FrameWindow* inventory = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "Inventory");
	m_window = inventory;
	
	inventory->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	inventory->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.85f)));
	inventory->setProperty("FrameEnabled","false");
	inventory->setProperty("TitlebarEnabled","false");
	inventory->setProperty("CloseButtonEnabled","false");
	inventory->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	inventory->setWantsMultiClickEvents(false);
	inventory->setAlpha(0.7);

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	
	label = win_mgr.createWindow("TaharezLook/StaticImage", "CharacterImage");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "false");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.02)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.4f)));
	label->setProperty("Image", "set:character image:character_img"); 
	label->setInheritsAlpha (false);
	label->setID(Equipement::NONE);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
	//label->setAlpha(1.0);

	int i,j;

	// Label fuer grosse Items
	std::ostringstream outStream;
	for (i=0;i<5;i++)
	{
		outStream.str("");
		outStream << "BigItem" << i<< "Label";
		label = win_mgr.createWindow("TaharezLook/StaticImage", outStream.str());
		inventory->addChildWindow(label);
		label->setProperty("FrameEnabled", "true");
		label->setProperty("BackgroundEnabled", "true");
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f+i*0.18f), cegui_reldim( 0.47f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.09f)));
		label->setID(Equipement::BIG_ITEMS+i);
		label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
		label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
		label->setWantsMultiClickEvents(false);
	}

	// Label fuer mittlere Items
	for (j=0;j<2;j++)
	{
		for (i=0;i<7;i++)
		{
			outStream.str("");
			outStream << "MediumItem" << j*7+i<< "Label";
			label = win_mgr.createWindow("TaharezLook/StaticImage", outStream.str());
			inventory->addChildWindow(label);
			label->setProperty("FrameEnabled", "true");
			label->setProperty("BackgroundEnabled", "true");
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f+i*0.13f), cegui_reldim( 0.58f+0.085*j)));
			label->setSize(CEGUI::UVector2(cegui_reldim(0.115f), cegui_reldim( 0.075f)));
			label->setID(Equipement::MEDIUM_ITEMS+j*7+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
			label->setWantsMultiClickEvents(false);
		}
	}
	// Label fuer kleine Items
	for (j=0;j<3;j++)
	{
		for (i=0;i<10;i++)
		{
			outStream.str("");
			outStream << "SmallItem" << j*10+i<< "Label";
			label = win_mgr.createWindow("TaharezLook/StaticImage", outStream.str());
			inventory->addChildWindow(label);
			label->setProperty("FrameEnabled", "true");
			label->setProperty("BackgroundEnabled", "true");
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.02f+i*0.096f), cegui_reldim( 0.75f+0.065*j)));
			label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.056f)));
			label->setID(Equipement::SMALL_ITEMS+j*10+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
			label->setWantsMultiClickEvents(false);
		}
	}

	// Label Ruestung
	label = win_mgr.createWindow("TaharezLook/StaticImage", "ArmorItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.35f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Equipement::ARMOR);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
	label->setWantsMultiClickEvents(false);


	// Label Waffe
	label = win_mgr.createWindow("TaharezLook/StaticImage", "WeaponItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.14f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Equipement::WEAPON);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
	label->setWantsMultiClickEvents(false);
	
	// Label Helm
	label = win_mgr.createWindow("TaharezLook/StaticImage", "HelmetItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.07f)));
	label->setID(Equipement::HELMET);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
	label->setWantsMultiClickEvents(false);
	
	// Label Schild
	label = win_mgr.createWindow("TaharezLook/StaticImage", "ShieldItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.14f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	label->setID(Equipement::SHIELD);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
	label->setWantsMultiClickEvents(false);
	
	// Label Handschuhe
	label = win_mgr.createWindow("TaharezLook/StaticImage", "GlovesItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.83f), cegui_reldim( 0.35f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.07f)));
	label->setID(Equipement::GLOVES);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*)this));
	label->setWantsMultiClickEvents(false);
	
	// Ring links
	label = win_mgr.createWindow("TaharezLook/StaticImage", "RingLeftItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.85f), cegui_reldim( 0.26f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.06f)));
	label->setID(Equipement::RING_LEFT);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*)this));
	label->setWantsMultiClickEvents(false);
	
	// Ring rechts
	label = win_mgr.createWindow("TaharezLook/StaticImage", "RingRightItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.26f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.06f)));
	label->setID(Equipement::RING_RIGHT);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*)this));
	label->setWantsMultiClickEvents(false);
	
	// Amulett
	label = win_mgr.createWindow("TaharezLook/StaticImage", "AmuletItemLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.85f), cegui_reldim( 0.06f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.06f)));
	label->setID(Equipement::AMULET);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*)this));
	label->setWantsMultiClickEvents(false);
	
	// Button Ausruestung umschalten
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SwapEquipButton"));
	inventory->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.77f), cegui_reldim( 0.04f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.07f), cegui_reldim( 0.05f)));
	btn->setText("1");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Inventory::onSwapEquipClicked, this));

	
	// Label Gold
	label = win_mgr.createWindow("TaharezLook/StaticText", "GoldLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.94f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim( 0.05f)));
	
	// Label Gold (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "GoldValueLabel");
	inventory->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.22f), cegui_reldim( 0.94f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.05f)));
	label->setText("0");
	
	// Label drop Gold
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "GoldDropButton"));
	inventory->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.94f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim( 0.05f)));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&Inventory::onDropGoldClicked, this));
	
	
	// Label drop Gold (Wert)
	CEGUI::Editbox* box;
	box = static_cast<CEGUI::Editbox*>(win_mgr.createWindow("TaharezLook/Editbox", "GoldDropValueBox"));
	inventory->addChildWindow(box);
	box->setPosition(CEGUI::UVector2(cegui_reldim(0.67f), cegui_reldim( 0.94f)));
	box->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.05f)));
	box->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&Inventory::onDropGoldClicked,  this));
	box->setText("0");
	
	updateTranslation();
}

void Inventory::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* img;
	CEGUI::Window* label;
	Item* it,*weapon;
	CEGUI::PushButton* btn;
	
	
	std::ostringstream out_stream;

	Player* player = m_document->getLocalPlayer();
	Equipement* equ = player->getEquipement();


	// Label Waffe
	// Label Schild
	img =  win_mgr.getWindow("WeaponItemLabel");
	it = player->getWeapon();
	updateItemWindow(img,it,player);
	weapon = it;
	bool two_hand = false;
	if (weapon !=0 && weapon->m_weapon_attr->m_two_handed)
	{
		two_hand = true;
	}


	// Label Ruestung
	img =  win_mgr.getWindow("ArmorItemLabel");
	it = equ->getItem(Equipement::ARMOR);
	updateItemWindow(img,it,player);

	// Label Helm
	img =  win_mgr.getWindow("HelmetItemLabel");
	it = equ->getItem(Equipement::HELMET);
	updateItemWindow(img,it,player);

	// Label Schild
	// bei Zweihandwaffen wird die Waffe eingeblendet
	float alpha = 1.0;
	img =  win_mgr.getWindow("ShieldItemLabel");
	it = player->getShield();
	if (two_hand && it ==0)
	{
		it = weapon;
		alpha = 0.7;
	}
	updateItemWindow(img,it,player);
	if (img->getAlpha() != alpha)
	{
		img->setAlpha(alpha);
	}
	

	// Label Handschuhe
	img =  win_mgr.getWindow("GlovesItemLabel");
	it = equ->getItem(Equipement::GLOVES);
	updateItemWindow(img,it,player);

	// Label Ring links
	img =  win_mgr.getWindow("RingLeftItemLabel");
	it = equ->getItem(Equipement::RING_LEFT);
	updateItemWindow(img,it,player);

	// Label Ring rechts
	img =  win_mgr.getWindow("RingRightItemLabel");
	it = equ->getItem(Equipement::RING_RIGHT);
	updateItemWindow(img,it,player);
	

	// Label Amulet
	img =  win_mgr.getWindow("AmuletItemLabel");
	it = equ->getItem(Equipement::AMULET);
	updateItemWindow(img,it,player);
	
	// Button fuer Zweitausruestung
	std::string text = "1";
	if (player->isUsingSecondaryEquip())
	{
		text="2";
	}
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("SwapEquipButton"));
	if (btn->getText() != text)
	{
		btn->setText(text);
	}
	
	int i;
	
	// Inventar: grosse Items
	for (i=0;i<5;i++)
	{
		out_stream.str("");
		out_stream << "BigItem" << i<< "Label";
		img =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::BIG_ITEMS+i);
		updateItemWindow(img,it,player);
	}

	// Inventar: mittlere Items
	for (i=0;i<14;i++)
	{
		out_stream.str("");
		out_stream << "MediumItem" << i<< "Label";
		img =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::MEDIUM_ITEMS+i);
		updateItemWindow(img,it,player);
	}

	// Inventar: kleine Items
	for (i=0;i<30;i++)
	{
		out_stream.str("");
		out_stream << "SmallItem" << i<< "Label";
		img =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::SMALL_ITEMS+i);
		updateItemWindow(img,it,player);
	}
	
	label =  win_mgr.getWindow("GoldValueLabel");
	out_stream.str("");
	out_stream << equ->getGold();
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}
	
}

void Inventory::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	label = win_mgr.getWindow("GoldLabel");
	label->setText((CEGUI::utf8*) gettext("Gold"));
	
	label = win_mgr.getWindow("GoldDropButton");
	label->setText((CEGUI::utf8*) gettext("Drop:"));
	
}



bool Inventory::onSwapEquipClicked(const CEGUI::EventArgs& evt)
{
	m_document->onSwapEquip();
	return true;
}

bool Inventory::onDropGoldClicked(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	
	CEGUI::Editbox* gold = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("GoldDropValueBox"));
	int val =0;
	std::stringstream stream;
	stream.str(gold->getText().c_str());
	stream >> val;
	
	if (val != 0)
	{
		gold->setText("0");
		gold->deactivate();
		m_document->dropGold(val);
	}
	return true;
}

