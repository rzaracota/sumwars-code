#include "inventory.h"

Inventory::Inventory (Document* doc)
	: ItemWindow(doc)
{
	DEBUG5("setup inventory");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	// Rahmen fuer Inventar Fenster
	CEGUI::FrameWindow* inventory = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "Inventory");
	m_window = inventory;
	
	inventory->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	inventory->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.78f)));
	inventory->setProperty("FrameEnabled","false");
	inventory->setProperty("TitlebarEnabled","false");
	inventory->setProperty("CloseButtonEnabled","false");
	inventory->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	inventory->setWantsMultiClickEvents(false);
	inventory->setAlpha(0.7);

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;

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
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f+i*0.18f), cegui_reldim( 0.5f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
		label->setID(Equipement::BIG_ITEMS+i);
		label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
		label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
		label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
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
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f+i*0.13f), cegui_reldim( 0.61f+0.09*j)));
			label->setSize(CEGUI::UVector2(cegui_reldim(0.115f), cegui_reldim( 0.08f)));
			label->setID(Equipement::MEDIUM_ITEMS+j*7+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
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
			label->setPosition(CEGUI::UVector2(cegui_reldim(0.02f+i*0.096f), cegui_reldim( 0.79f+0.07*j)));
			label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.06f)));
			label->setID(Equipement::SMALL_ITEMS+j*10+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
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



	/*
	try
	{
	numlabel = (NumberedWindow*) win_mgr.createWindow("Taharez/NumberedStaticImage", "ArmorItemLabel");
	inventory->addChildWindow(numlabel);
	numlabel->setProperty("FrameEnabled", "true");
	numlabel->setProperty("BackgroundEnabled", "true");
	numlabel->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.35f)));
	numlabel->setSize(CEGUI::UVector2(cegui_reldim(0.13f), cegui_reldim( 0.1f)));
	numlabel->setText("Ruestung");
	DEBUG("NumberedWindow2 created");
}
	catch (CEGUI::UnknownObjectException e)
	{
	ERRORMSG("Error message: %s",e.getMessage().c_str());
}
	*/

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

	// Button Ausruestung umschalten
	btn = static_cast<CEGUI::PushButton*>(win_mgr.createWindow("TaharezLook/Button", "SwapEquipButton"));
	inventory->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.06f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.07f)));
	btn->setText("1");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Inventory::onSwapEquipClicked, this));

}

void Inventory::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* img;
	std::ostringstream out_stream;

	Player* player = m_document->getLocalPlayer();


	// Label Waffe
	out_stream.str("");
	if (player->getWeapon()!=0)
	{
		out_stream << getItemImage(player->getWeapon()->m_subtype);
	}
	img =  win_mgr.getWindow("WeaponItemLabel");
	if (img->getProperty("Image")!=out_stream.str())
	{
		img->setProperty("Image", out_stream.str());
	}



	// Label Ruestung
	out_stream.str("");
	Equipement* equ = player->getEquipement();
	if (equ->getItem(Equipement::ARMOR)!=0)
	{
		out_stream << getItemImage(equ->getItem(Equipement::ARMOR)->m_subtype);
	}
	img =  win_mgr.getWindow("ArmorItemLabel");
	if (img->getProperty("Image")!=out_stream.str())
	{
		img->setProperty("Image", out_stream.str());
	}

	// Label Helm
	out_stream.str("");
	if (equ->getItem(Equipement::HELMET)!=0)
	{
		out_stream << getItemImage(equ->getItem(Equipement::HELMET)->m_subtype);
	}
	img =  win_mgr.getWindow("HelmetItemLabel");
	if (img->getProperty("Image")!=out_stream.str())
	{
		img->setProperty("Image", out_stream.str());
	}

	// Label Schild
	out_stream.str("");
	if (player->getShield()!=0)
	{
		out_stream << getItemImage(player->getShield()->m_subtype);
	}
	img =  win_mgr.getWindow("ShieldItemLabel");
	if (img->getProperty("Image")!=out_stream.str())
	{
		img->setProperty("Image", out_stream.str());
	}

	// Label Handschuhe
	out_stream.str("");
	if (equ->getItem(Equipement::GLOVES)!=0)
	{
		out_stream << getItemImage(equ->getItem(Equipement::GLOVES)->m_subtype);
	}
	img =  win_mgr.getWindow("GlovesItemLabel");
	if (img->getProperty("Image")!=out_stream.str())
	{
		img->setProperty("Image", out_stream.str());
	}

	// Label Ring links
	out_stream.str("");
	if (equ->getItem(Equipement::RING_LEFT)!=0)
	{
		out_stream << getItemImage(equ->getItem(Equipement::RING_LEFT)->m_subtype);
	}
	img =  win_mgr.getWindow("RingLeftItemLabel");
	if (img->getProperty("Image")!=out_stream.str())
	{
		img->setProperty("Image", out_stream.str());
	}

	// Label Ring rechts
	out_stream.str("");
	if (equ->getItem(Equipement::RING_RIGHT)!=0)
	{
		out_stream << getItemImage(equ->getItem(Equipement::RING_RIGHT)->m_subtype);
	}
	img =  win_mgr.getWindow("RingRightItemLabel");
	if (img->getProperty("Image")!=out_stream.str())
	{
		img->setProperty("Image", out_stream.str());
	}

	// Label Amulet
	out_stream.str("");
	if (equ->getItem(Equipement::AMULET)!=0)
	{
		out_stream << getItemImage(equ->getItem(Equipement::AMULET)->m_subtype);
	}
	img =  win_mgr.getWindow("AmuletItemLabel");
	if (img->getProperty("Image")!=out_stream.str())
	{
		img->setProperty("Image", out_stream.str());
	}

	int i;
	Item* it;
	// Inventar: grosse Items
	for (i=0;i<5;i++)
	{
		out_stream.str("");
		out_stream << "BigItem" << i<< "Label";
		img =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::BIG_ITEMS+i);
		out_stream.str("");
		if (it!=0)
		{
			out_stream<< getItemImage(it->m_subtype);
		}
		if (img->getProperty("Image")!=out_stream.str())
		{
			img->setProperty("Image", out_stream.str());
		}
	}

	// Inventar: mittlere Items
	for (i=0;i<14;i++)
	{
		out_stream.str("");
		out_stream << "MediumItem" << i<< "Label";
		img =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::MEDIUM_ITEMS+i);
		out_stream.str("");
		if (it!=0)
		{
			out_stream<< getItemImage(it->m_subtype);
		}
		if (img->getProperty("Image")!=out_stream.str())
		{
			img->setProperty("Image", out_stream.str());
		}
	}

	// Inventar: kleine Items
	for (i=0;i<30;i++)
	{
		out_stream.str("");
		out_stream << "SmallItem" << i<< "Label";
		img =  win_mgr.getWindow(out_stream.str().c_str());
		it = equ->getItem(Equipement::SMALL_ITEMS+i);
		out_stream.str("");
		if (it!=0)
		{
			out_stream<< getItemImage(it->m_subtype);
		}
		if (img->getProperty("Image")!=out_stream.str())
		{
			img->setProperty("Image", out_stream.str());
		}
	}
}

bool Inventory::onSwapEquipClicked(const CEGUI::EventArgs& evt)
{
	m_document->onSwapEquip();
	return true;
}

