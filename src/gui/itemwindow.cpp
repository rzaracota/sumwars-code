
#include "itemwindow.h"

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
	updateItemWindowTooltip(we.window,player->getEquipement()->getItem(id) ,player);
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
		DEBUG5("left button released on Item %i",id);
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
		if (!player->checkItemRequirements(item))
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

void ItemWindow::updateItemWindowTooltip(CEGUI::Window* img, Item* item, Player* player, int gold)
{
	std::string msg;
	
	if (item ==0)
	{
		msg = "";
	}
	else
	{
		msg =item->getDescription();
	}
	img->setTooltipText(msg);
}

void ItemWindow::registerItemImage(Item::Subtype type, std::string image)
{
	m_item_images[type] = image;
}
