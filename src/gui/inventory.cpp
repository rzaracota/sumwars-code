#include "inventory.h"
#include "sound.h"

Inventory::Inventory (Document* doc)
	: ItemWindow(doc)
{
	DEBUGX("setup inventory");

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::FrameWindow* inventory = (CEGUI::FrameWindow*)win_mgr.loadWindowLayout( "inventory.layout" );
	m_window = inventory;
	inventory->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	inventory->setWantsMultiClickEvents(false);
	inventory->setPosition(CEGUI::UVector2(cegui_reldim(0.52f), cegui_reldim( 0.0f)));
	inventory->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.85f)));


	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	
	label = win_mgr.getWindow("CharacterImage");
	label->setProperty("Image", "set:character image:character_img"); 
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
		label = win_mgr.getWindow(outStream.str());
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
			label = win_mgr.getWindow(outStream.str());
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
			label = win_mgr.getWindow(outStream.str());
			label->setID(Equipement::SMALL_ITEMS+j*10+i);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
			label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
			label->setWantsMultiClickEvents(false);
		}
	}
	
	for (i=0;i<10;i++)
	{
		outStream.str("");
		outStream << "BeltShortcutLabel" << i;
		label = win_mgr.createWindow("TaharezLook/StaticText", outStream.str());
		inventory->addChildWindow(label);
		label->setPosition(CEGUI::UVector2(cegui_reldim(0.125f+i*0.0815f), cegui_reldim( 0.74f)));
		label->setSize(CEGUI::UVector2(cegui_reldim(0.086f), cegui_reldim( 0.056f)));
		label->setProperty("FrameEnabled", "false");
		label->setProperty("BackgroundEnabled", "false");
		std::stringstream stream;
		stream << (i+1)%10;
		label->setText(stream.str());
		label->setAlwaysOnTop(true);
		label->setMousePassThroughEnabled(true);
	}

	// Label Ruestung
	label = win_mgr.getWindow("ArmorItemLabel");
	label->setID(Equipement::ARMOR);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
	label->setWantsMultiClickEvents(false);


	// Label Waffe
	label = win_mgr.getWindow("WeaponItemLabel");
	label->setID(Equipement::WEAPON);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
	label->setWantsMultiClickEvents(false);
	
	// Label Helm
	label = win_mgr.getWindow("HelmetItemLabel");
	label->setID(Equipement::HELMET);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
	label->setWantsMultiClickEvents(false);
	
	// Label Schild
	label = win_mgr.getWindow("ShieldItemLabel");
	label->setID(Equipement::SHIELD);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*) this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*) this));
	label->setWantsMultiClickEvents(false);
	
	// Label Handschuhe
	label = win_mgr.getWindow("GlovesItemLabel");
	label->setID(Equipement::GLOVES);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*)this));
	label->setWantsMultiClickEvents(false);
	
	// Ring links
	label = win_mgr.getWindow("RingLeftItemLabel");
	label->setID(Equipement::RING_LEFT);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*)this));
	label->setWantsMultiClickEvents(false);
	
	// Ring rechts
	label = win_mgr.getWindow("RingRightItemLabel");
	label->setID(Equipement::RING_RIGHT);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*)this));
	label->setWantsMultiClickEvents(false);
	
	// Amulett
	label = win_mgr.getWindow("AmuletItemLabel");
	label->setID(Equipement::AMULET);
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonPressed, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Inventory::onItemMouseButtonReleased, (ItemWindow*)this));
	label->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&Inventory::onItemHover, (ItemWindow*)this));
	label->setWantsMultiClickEvents(false);
	
	// Button Ausruestung umschalten
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("SwapEquipButton"));
	btn->setText("1");
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Inventory::onSwapEquipClicked, this));

	// Label drop Gold
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("GoldDropButton"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&Inventory::onDropGoldClicked, this));
	
	
	// Label drop Gold (Wert)
	CEGUI::Editbox* box;
	box = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("GoldDropValueBox"));
	box->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&Inventory::onDropGoldClicked,  this));
	
	updateTranslation();

/*
	CEGUI::AnimationManager *amgr = CEGUI::AnimationManager::getSingletonPtr();
	CEGUI::Animation *anim = amgr->createAnimation("InventoryAnimation");
	anim->setDuration(0.5f);
    anim->setReplayMode(CEGUI::Animation::RM_Once);
    // now we define affector inside our Testing animation
    {
        // this affector changes YRotation and interpolates keyframes with float interpolator
        CEGUI::Affector* affector = anim->createAffector("YRotation", "float");
        // at 0.0 seconds, the animation should set YRotation to 10.0 degrees
        affector->createKeyFrame(0.0f, "90.0");
        // at 0.3 seconds, YRotation should be 0 degrees and animation should progress towards this in an accelerating manner
        affector->createKeyFrame(0.5f, "0.0", CEGUI::KeyFrame::P_QuadraticAccelerating);
    }

    // animation can have more than one affectors! lets define another affector that changes Alpha
    {
        // this affector will again use float interpolator
        CEGUI::Affector* affector = anim->createAffector("Alpha", "float");
        affector->createKeyFrame(0.0f, "0.0"); // at 0.0 seconds, set alpha to 0.5
        affector->createKeyFrame(0.5f, "0.9", CEGUI::KeyFrame::P_QuadraticDecelerating); // at 1.0 seconds, set alpha to 1.0, now decelerating!
    }
    
    CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	// after we instantiate the animation, we have to set it's target window
    instance->setTargetWindow(inventory);

	// at this point, you can start this instance and see the results
    instance->start();

	inventory->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&CEGUI::AnimationInstance::handleStart, instance));
*/
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
		if (!m_silent)
		{
			SoundSystem::playAmbientSound("sell_buy");
		}
	}
	
}

void Inventory::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
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

