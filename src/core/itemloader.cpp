#include "itemloader.h"

int ItemLoader::generateItemBasicData(TiXmlElement* pElement, string element)
{
	if ( !pElement ) return 0;
	
	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	
	if (element == "Item" && pAttrib)
	{
		if (m_item_data == 0)
		{
			m_item_data = new ItemBasicData;
			//Folgendes wird schon im Konstruktor von ItemBasicData gesetzt
			//m_item_data->m_useup_effect = 0;
			//m_item_data->m_equip_effect = 0;
			//m_item_data->m_weapon_attr = 0;
			for (int i=0; i<31; i++)
				m_item_data->m_modchance[i] = m_weapon_mod[i];
		}
		
		while (element == "Item" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "type"))
			{
				if (!strcmp(pAttrib->Value(), "armor"))
					m_item_data->m_type = Item::ARMOR;
				else if (!strcmp(pAttrib->Value(), "helmet"))
					m_item_data->m_type = Item::HELMET;
				else if (!strcmp(pAttrib->Value(), "gloves"))
					m_item_data->m_type = Item::GLOVES;
				else if (!strcmp(pAttrib->Value(), "weapon"))
					m_item_data->m_type = Item::WEAPON;
				else if (!strcmp(pAttrib->Value(), "shield"))
					m_item_data->m_type = Item::SHIELD;
				else if (!strcmp(pAttrib->Value(), "potion"))
					m_item_data->m_type = Item::POTION;
				else if (!strcmp(pAttrib->Value(), "ring"))
					m_item_data->m_type = Item::RING;
				else if (!strcmp(pAttrib->Value(), "amulet"))
					m_item_data->m_type = Item::AMULET;
				else if (!strcmp(pAttrib->Value(), "gold_type"))
					m_item_data->m_type = Item::GOLD_TYPE;
			}
			else if (!strcmp(pAttrib->Name(), "subtype"))
			{
				m_item_data->m_subtype = pAttrib->Value();
			}
			else if (!strcmp(pAttrib->Name(), "size"))
			{
				if (!strcmp(pAttrib->Value(), "gold"))
					m_item_data->m_size = Item::GOLD;
				else if (!strcmp(pAttrib->Value(), "small"))
					m_item_data->m_size = Item::SMALL;
				else if (!strcmp(pAttrib->Value(), "medium"))
					m_item_data->m_size = Item::MEDIUM;
				else if (!strcmp(pAttrib->Value(), "big"))
					m_item_data->m_size = Item::BIG;
			}
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "UseupEffect" && pAttrib)
	{
		if (m_item_data->m_useup_effect == 0)
			m_item_data->m_useup_effect = new CreatureDynAttrMod;
		
		while (element == "UseupEffect" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "EquipEffect" /*&& pAttrib*/)
	{
		if (m_item_data->m_equip_effect == 0)
			m_item_data->m_equip_effect = new CreatureBaseAttrMod;
		
		while (element == "EquipEffect" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "WeaponAttribute" && pAttrib)
	{
		if (m_item_data->m_weapon_attr == 0)
			m_item_data->m_weapon_attr = new WeaponAttr;
		
		while (element == "WeaponAttribute" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "damage_min_physical") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "damage_max_physical") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "damage_min_air") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_min_damage[Damage::AIR] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "damage_max_air") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_max_damage[Damage::AIR] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "damage_min_ice") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_min_damage[Damage::ICE] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "damage_max_ice") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_max_damage[Damage::ICE] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "damage_min_fire") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_min_damage[Damage::FIRE] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "damage_max_fire") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_max_damage[Damage::FIRE] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "damage_attack") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_attack = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "damage_power") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_damage.m_power = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "attack_range") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_attack_range = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "two_handed"))
			{
				if (!strcmp(pAttrib->Value(), "yes"))
					m_item_data->m_weapon_attr->m_two_handed = true;
				else
					m_item_data->m_weapon_attr->m_two_handed = false;
			}
			else if (!strcmp(pAttrib->Name(), "dattack_speed") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_weapon_attr->m_dattack_speed = static_cast<short>(ival);
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Attribute" && pAttrib)
	{
	
		while (element == "Attribute" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "level_requirement") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_level_req = static_cast<char>(ival);
			else if (!strcmp(pAttrib->Name(), "character_requirement") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_char_req = static_cast<char>(ival);
			else if (!strcmp(pAttrib->Name(), "price") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_price = ival;
			else if (!strcmp(pAttrib->Name(), "min_enchant") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
			{
				m_item_data->m_min_enchant = static_cast<float>(dval);
			}
			else if (!strcmp(pAttrib->Name(), "max_enchant") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
			{
				m_item_data->m_max_enchant = static_cast<float>(dval);
			}
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	return i;
}


void ItemLoader::searchItemBasicData(TiXmlNode* pParent)
{
	if ( !pParent ) return;
	
	TiXmlNode* pChild;
	TiXmlText* pText;
	
	int t = pParent->Type();
	int num;
	
	switch ( t )
	{
	case TiXmlNode::ELEMENT:
		//printf( "Element [%s]", pParent->Value() );
		num = generateItemBasicData(pParent->ToElement(), pParent->Value());
		/*switch(num)
		{
			case 0:  printf( " (No attributes)"); break;
			case 1:  printf( "%s1 attribute", getIndentAlt(indent)); break;
			default: printf( "%s%d attributes", getIndentAlt(indent), num); break;
		}*/
		break;
	/*
	case TiXmlNode::TEXT:
		pText = pParent->ToText();
		printf( "Text: [%s]", pText->Value() );
		break;
	*/
	default:
		break;
	}
	
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		searchItemBasicData(pChild);
		
		if ( !strcmp(pChild->Value(), "Item") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_item_list->push_back(m_item_data);
			m_item_data = 0;
			DEBUG5("Item loaded");
		}
	}
}


list<ItemBasicData*>* ItemLoader::loadItemBasicData(const char* pFilename)
{
	// Standard Modifikator Verteilung fuer Waffen
	for (int i=0; i<31; i++)
	{
		m_weapon_mod[i] = 0;
	}
	m_weapon_mod[ItemFactory::DEXTERITY_MOD] = 0.1;
	m_weapon_mod[ItemFactory::MAGIC_POWER_MOD] = 0.05;
	m_weapon_mod[ItemFactory::STRENGTH_MOD] = 0.1;
	m_weapon_mod[ItemFactory::DAMAGE_PHYS_MOD] = 0.15;
	m_weapon_mod[ItemFactory::DAMAGE_FIRE_MOD] = 0.10;
	m_weapon_mod[ItemFactory::DAMAGE_ICE_MOD] = 0.10;
	m_weapon_mod[ItemFactory::DAMAGE_AIR_MOD] = 0.10;
	m_weapon_mod[ItemFactory::ATTACK_MOD] = 0.1;
	m_weapon_mod[ItemFactory::POWER_MOD] = 0.1;
	m_weapon_mod[ItemFactory::DAMAGE_MULT_PHYS_MOD] = 0.1;
	
	m_item_data = 0;
	m_item_list = new list<ItemBasicData*>;
	
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	
	if (loadOkay)
	{
		DEBUG5("Loading %s", pFilename);
		searchItemBasicData(&doc);
		DEBUG5("Loading %s finished", pFilename);
		return m_item_list;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return 0;
	}
}

//##############################################################################

int ItemLoader::generateDropChanceData(TiXmlElement* pElement, string element)
{
	if ( !pElement ) return 0;
	
	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	
	if (element == "Item" && pAttrib)
	{
		if (m_item_data == 0)
		{
			m_drop_chance_data = new DropChanceData;
		}
		
		while (element == "Item" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "type"))
			{
				if (!strcmp(pAttrib->Value(), "armor"))
					m_drop_chance_data->m_type = Item::ARMOR;
				else if (!strcmp(pAttrib->Value(), "helmet"))
					m_drop_chance_data->m_type = Item::HELMET;
				else if (!strcmp(pAttrib->Value(), "gloves"))
					m_drop_chance_data->m_type = Item::GLOVES;
				else if (!strcmp(pAttrib->Value(), "weapon"))
					m_drop_chance_data->m_type = Item::WEAPON;
				else if (!strcmp(pAttrib->Value(), "shield"))
					m_drop_chance_data->m_type = Item::SHIELD;
				else if (!strcmp(pAttrib->Value(), "potion"))
					m_drop_chance_data->m_type = Item::POTION;
				else if (!strcmp(pAttrib->Value(), "ring"))
					m_drop_chance_data->m_type = Item::RING;
				else if (!strcmp(pAttrib->Value(), "amulet"))
					m_drop_chance_data->m_type = Item::AMULET;
				else if (!strcmp(pAttrib->Value(), "gold_type"))
					m_drop_chance_data->m_type = Item::GOLD_TYPE;
			}
			else if (!strcmp(pAttrib->Name(), "subtype"))
			{
				m_drop_chance_data->m_subtype = pAttrib->Value();
			}
			else if (!strcmp(pAttrib->Name(), "size"))
			{
				if (!strcmp(pAttrib->Value(), "gold"))
					m_drop_chance_data->m_size = Item::GOLD;
				else if (!strcmp(pAttrib->Value(), "small"))
					m_drop_chance_data->m_size = Item::SMALL;
				else if (!strcmp(pAttrib->Value(), "medium"))
					m_drop_chance_data->m_size = Item::MEDIUM;
				else if (!strcmp(pAttrib->Value(), "big"))
					m_drop_chance_data->m_size = Item::BIG;
			}
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "DropChance" && pAttrib)
	{
		while (element == "DropChance" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "level") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_drop_chance_data->m_level = ival;
			else if (!strcmp(pAttrib->Name(), "probability") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_drop_chance_data->m_probability = static_cast<float>(dval);
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	return i;
}


void ItemLoader::searchDropChanceData(TiXmlNode* pParent)
{
	if ( !pParent ) return;
	
	TiXmlNode* pChild;
	TiXmlText* pText;
	
	int t = pParent->Type();
	int num;
	
	switch ( t )
	{
	case TiXmlNode::ELEMENT:
		//printf( "Element [%s]", pParent->Value() );
		num = generateDropChanceData(pParent->ToElement(), pParent->Value());
		/*switch(num)
		{
			case 0:  printf( " (No attributes)"); break;
			case 1:  printf( "%s1 attribute", getIndentAlt(indent)); break;
			default: printf( "%s%d attributes", getIndentAlt(indent), num); break;
		}*/
		break;
	/*
	case TiXmlNode::TEXT:
		pText = pParent->ToText();
		printf( "Text: [%s]", pText->Value() );
		break;
	*/
	default:
		break;
	}
	
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		searchDropChanceData(pChild);
		
		if ( !strcmp(pChild->Value(), "Item") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_drop_chance_data_list->push_back(m_drop_chance_data);
			m_drop_chance_data = 0;
			DEBUG5("DropChance loaded");
		}
	}
}


list<DropChanceData*>* ItemLoader::loadDropChanceData(const char* pFilename)
{
	m_drop_chance_data = 0;
	m_drop_chance_data_list = new list<DropChanceData*>;
	
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	
	if (loadOkay)
	{
		DEBUG5("Loading %s", pFilename);
		searchDropChanceData(&doc);
		DEBUG5("Loading %s finished", pFilename);
		return m_drop_chance_data_list;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return 0;
	}
}






ItemLoader::~ItemLoader()
{
	for (list<DropChanceData*>::iterator iter = m_drop_chance_data_list->begin(); iter != m_drop_chance_data_list->end(); iter++)
	{
		delete (*iter);
	}
	delete m_drop_chance_data_list;
	m_drop_chance_data_list = 0;
}

