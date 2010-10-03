
#include "itemwindow.h"
#include "tooltipmanager.h"

std::map<Item::Subtype, std::string> ItemWindow::m_item_images;


ItemWindow::ItemWindow (Document* doc)
	: Window(doc)
{
	
}

bool ItemWindow::onItemHover(const CEGUI::EventArgs& evt)
{

	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	
	Player* player = m_document->getLocalPlayer();
	Item* itm;
	if (id == Equipement::WEAPON)
	{
		itm = player->getWeapon();
	}
	else if (id == Equipement::SHIELD)
	{
		itm = player->getShield();
	}
	else
	{
		itm = player->getEquipement()->getItem(id);
	}
	
	float factor = 1.0;
	Creature* npc = player->getTradePartner();
	if (npc != 0)
	{
		NPCTrade& tradeinfo = Dialogue::getNPCTrade(npc->getRefName());
		factor = tradeinfo.m_pay_multiplier;
	}
	updateItemWindowTooltip(we.window,itm ,player,-1,factor);
	return true;
}


bool ItemWindow::onItemMouseButtonPressed(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();

	if (we.button == CEGUI::LeftButton)
	{
		m_document->onItemLeftClick((short) id);
	}

	if (we.button == CEGUI::RightButton)
	{
		m_document->onItemRightClick((short) id);
	}


	return true;
}

bool ItemWindow::onItemMouseButtonReleased(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	if (we.button == CEGUI::LeftButton)
	{
		DEBUGX("left button released on Item %i",id);
	}
	return true;
}


std::string ItemWindow::getItemImage(Item::Subtype type)
{
	std::map<Item::Subtype, std::string>::iterator it;
	it = m_item_images.find(type);

	if (it != m_item_images.end())
	{
		return it->second;
	}

	return "set: noMedia.png image: full_image";
}


void ItemWindow::updateItemWindow(CEGUI::Window* img, Item* item, Player* player, int gold)
{
	std::string imgname="";
	if (item != 0)
	{
		imgname= getItemImage(item->m_subtype); 
	}
	
	if (img->getProperty("Image")!=imgname)
	{
		img->setProperty("Image", imgname);
	}
	
	std::string propold = img->getProperty("BackgroundColours").c_str();
	std::string propnew = "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000";
	if (item !=0)
	{
		// rot wenn Spieler Item nicht verwenden kann
		// oder es bezahlen muss und nicht genug Geld hat
		if (!player->checkItemRequirements(item).m_overall)
		{
			propnew = "tl:FFAA5555 tr:FFAA5555 bl:FFAA5555 br:FFAA5555";
		}
		else if (gold>=0 && gold<item->m_price)
		{
			propnew = "tl:FFAA5555 tr:FFAA5555 bl:FFAA5555 br:FFAA5555";
		}
		else if (item->m_rarity == Item::MAGICAL)
		{
			propnew = "tl:FF5555AA tr:FF5555AA bl:FF5555AA br:FF5555AA";
		}
	}
	
	if (propold != propnew)
	{
		img->setProperty("BackgroundColours", propnew); 
	}
	
}

void ItemWindow::updateItemWindowTooltip(CEGUI::Window* img, Item* item, Player* player, int gold,float price_factor)
{
	TooltipManager *tMgr = TooltipManager::getSingletonPtr();

	if ( item == 0 )
		return;

	CEGUI::Font *font = img->getTooltip()->getFont();
	std::string msg;
	ItemRequirementsMet irm = player->checkItemRequirements ( item );
	std::list<std::string> l = item->getDescriptionAsStringList ( price_factor, irm );
	l.push_front ( "Hovered:\n" );
	std::ostringstream out_stream;

	tMgr->createTooltip ( img, l, 0, font, Tooltip::Main );

	Item *currentEqItem = 0;
	Item *currentEqItemOffhand = 0;

	switch ( item->m_type )
	{
		case Item::NOITEM:
			break;

		case Item::ARMOR:
			currentEqItem = player->getEquipement()->getItem ( Equipement::ARMOR );
			break;

		case Item::HELMET:
			currentEqItem = player->getEquipement()->getItem ( Equipement::HELMET );
			break;

		case Item::GLOVES:
			currentEqItem = player->getEquipement()->getItem ( Equipement::GLOVES );
			break;

		case Item::WEAPON:
			currentEqItem = player->getEquipement()->getItem ( Equipement::WEAPON );
			currentEqItemOffhand = player->getEquipement()->getItem ( Equipement::WEAPON2 );
			break;

		case Item::SHIELD:
			currentEqItem = player->getEquipement()->getItem ( Equipement::SHIELD );
			break;

		case Item::POTION:
			break;

		case Item::RING:
			currentEqItem = player->getEquipement()->getItem ( Equipement::RING_LEFT );
			currentEqItemOffhand = player->getEquipement()->getItem ( Equipement::RING_RIGHT );
			break;

		case Item::AMULET:
			currentEqItem = player->getEquipement()->getItem ( Equipement::AMULET );
			break;

		case Item::GOLD_TYPE:
			break;

		default:
			break;

	}

	if ( currentEqItem )
	{
		l = currentEqItem->getDescriptionAsStringList ( price_factor );
		tMgr->createTooltip ( img, l, 0, font, Tooltip::Comparision );

		if ( currentEqItemOffhand )
		{
			l = currentEqItemOffhand->getDescriptionAsStringList ( price_factor );
			tMgr->createTooltip ( img, l, 0, font, Tooltip::Comparision );
		}
	}
}

void ItemWindow::registerItemImage(Item::Subtype type, std::string image)
{
	m_item_images[type] = image;
}
