
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
	updateItemTooltip(id);
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

void ItemWindow::updateItemTooltip(unsigned int pos)
{
	Item* item = m_document->getLocalPlayer()->getEquipement()->getItem(pos);

	DEBUG5("setting tool tip for item at %i",pos);
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	std::ostringstream out_stream;
	out_stream.str("");
	if (pos == Equipement::ARMOR)
		out_stream << "ArmorItemLabel";
	if (pos == Equipement::WEAPON || pos == Equipement::WEAPON2)
		out_stream << "WeaponItemLabel";
	if (pos == Equipement::HELMET)
		out_stream << "HelmetItemLabel";
	if (pos == Equipement::SHIELD || pos == Equipement::SHIELD2)
		out_stream << "ShieldItemLabel";
	if (pos == Equipement::GLOVES)
		out_stream << "GlovesItemLabel";
	if (pos == Equipement::RING_LEFT)
		out_stream << "RingLeftItemLabel";
	if (pos == Equipement::RING_RIGHT)
		out_stream << "RingRightItemLabel";
	if (pos == Equipement::AMULET)
		out_stream << "AmuletItemLabel";
	if (pos>= Equipement::BIG_ITEMS && pos <  Equipement::MEDIUM_ITEMS)
	{
		out_stream << "BigItem"<<pos-Equipement::BIG_ITEMS<<"Label";
	}
	if (pos>= Equipement::MEDIUM_ITEMS && pos < Equipement::SMALL_ITEMS)
	{
		out_stream << "MediumItem"<<pos-Equipement::MEDIUM_ITEMS<<"Label";
	}
	if (pos>= Equipement::SMALL_ITEMS)
	{
		out_stream << "SmallItem"<<pos-Equipement::SMALL_ITEMS<<"Label";
	}
	label = win_mgr.getWindow(out_stream.str());

	std::string msg;
	
	if (item ==0)
	{
		msg = "";
	}
	else
	{
		msg =item->getDescription();
	}
	label->setTooltipText(msg);

	DEBUG5("Label: %s \ndescription: \n%s",out_stream.str().c_str(),msg.c_str());

	if (pos>= Equipement::SMALL_ITEMS && pos< Equipement::SMALL_ITEMS+10)
	{
		// Item befindet sich im Guertel
		out_stream.str("");
		out_stream << "InventoryItem"<<pos-Equipement::SMALL_ITEMS;
		label = win_mgr.getWindow(out_stream.str());
		label->setTooltipText(msg);
	}

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

