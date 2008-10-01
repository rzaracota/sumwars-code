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
			if (!strcmp(pAttrib->Name(), "dhealth") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_useup_effect->m_dhealth = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "dstatus_mod_immune_time0") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_useup_effect->m_dstatus_mod_immune_time[0] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "dstatus_mod_immune_time1") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_useup_effect->m_dstatus_mod_immune_time[1] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "dstatus_mod_immune_time2") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_useup_effect->m_dstatus_mod_immune_time[2] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "dstatus_mod_immune_time3") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_useup_effect->m_dstatus_mod_immune_time[3] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "dstatus_mod_immune_time4") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_useup_effect->m_dstatus_mod_immune_time[4] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "dstatus_mod_immune_time5") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_useup_effect->m_dstatus_mod_immune_time[5] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "dstatus_mod_immune_time6") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_useup_effect->m_dstatus_mod_immune_time[6] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "dstatus_mod_immune_time7") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_useup_effect->m_dstatus_mod_immune_time[7] = static_cast<float>(dval);
			
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
			if (!strcmp(pAttrib->Name(), "darmor") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_darmor = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dblock") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dblock = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dmax_health") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dmax_health = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "dattack") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dattack = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dstrength") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dstrength = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "ddexterity") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_ddexterity = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dmagic_power") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dmagic_power = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dwillpower") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dwillpower = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dresistances_physical") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dresistances[Damage::PHYSICAL] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dresistances_air") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dresistances[Damage::AIR] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dresistances_ice") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dresistances[Damage::ICE] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dresistances_fire") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dresistances[Damage::FIRE] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dresistances_cap_physical") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dresistances_cap[Damage::PHYSICAL] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dresistances_cap_air") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dresistances_cap[Damage::AIR] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dresistances_cap_ice") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dresistances_cap[Damage::ICE] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dresistances_cap_fire") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dresistances_cap[Damage::FIRE] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dwalk_speed") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dwalk_speed = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dattack_speed") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_dattack_speed = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "xspecial_flags") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_xspecial_flags = ival;
			else if (!strcmp(pAttrib->Name(), "time") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_time = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "xabilities0") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_xabilities[0] = ival;
			else if (!strcmp(pAttrib->Name(), "xabilities1") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_xabilities[1] = ival;
			else if (!strcmp(pAttrib->Name(), "xabilities2") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_xabilities[2] = ival;
			else if (!strcmp(pAttrib->Name(), "xabilities3") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_xabilities[3] = ival;
			else if (!strcmp(pAttrib->Name(), "xabilities4") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_xabilities[4] = ival;
			else if (!strcmp(pAttrib->Name(), "xabilities5") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_xabilities[5] = ival;
			else if (!strcmp(pAttrib->Name(), "ximmunity") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_item_data->m_equip_effect->m_ximmunity = static_cast<char>(ival);
			
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

//##############################################################################

int ItemLoader::generateItemMeshData(TiXmlElement* pElement, string element)
{
	if ( !pElement ) return 0;
	
	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	
	if (element == "Item" && pAttrib)
	{
		if (m_item_mesh_data == 0)
		{
			m_item_mesh_data = new ItemMeshData;
		}
		
		while (element == "Item" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "subtype"))
				m_item_mesh_data->m_subtype = pAttrib->Value();
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Mesh" && pAttrib)
	{
		while (element == "Mesh" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "file"))
				m_item_mesh_data->m_mesh = pAttrib->Value();
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	return i;
}


void ItemLoader::searchItemMeshData(TiXmlNode* pParent)
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
		num = generateItemMeshData(pParent->ToElement(), pParent->Value());
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
		searchItemMeshData(pChild);
		
		if ( !strcmp(pChild->Value(), "Item") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_item_mesh_list->push_back(m_item_mesh_data);
			m_item_mesh_data = 0;
			DEBUG5("Item Mesh loaded");
		}
	}
}


list<ItemMeshData*>* ItemLoader::loadItemMeshData(const char* pFilename)
{
	m_item_mesh_data = 0;
	m_item_mesh_list = new list<ItemMeshData*>;
	
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	
	if (loadOkay)
	{
		DEBUG5("Loading %s", pFilename);
		searchItemMeshData(&doc);
		DEBUG5("Loading %s finished", pFilename);
		return m_item_mesh_list;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return 0;
	}
}




ItemLoader::~ItemLoader()
{
	// FIXME Löschen der Listen erfolgt nach dem Aufruf der Methoden manuell
	//       in der rufenden Funktion. Dies eventuell hier implementieren.
	//       Daten in m_item_list (list<ItemBasicData*>) duerfen nicht geloescht
	//       werden!
}

