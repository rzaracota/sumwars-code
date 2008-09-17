#include "objectloader.h"

#include "warrior.h"
#include "mage.h"
#include "archer.h"
#include "priest.h"
#include "fixedobject.h"
#include "monster.h"
#include "goblin.h"


int ObjectLoader::generateObjects(TiXmlElement* pElement, string element)
{
	if ( !pElement ) return 0;
	
	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	
	if (element == "Monster" && pAttrib)
	{
		if (m_monster_data == 0)
		{
			m_monster_data = new MonsterBasicData;
			m_monster_data->m_layer = (WorldObject::Geometry::LAYER_BASE | WorldObject::Geometry::LAYER_AIR);
		}
		
		while (element == "Monster" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "type"))
			{
				if (!strcmp(pAttrib->Value(), "MONSTER"))
					m_monster_data->m_type_info.m_type = WorldObject::TypeInfo::TYPE_MONSTER;
			}
			else if (!strcmp(pAttrib->Name(), "subtype"))
			{
				if (!strcmp(pAttrib->Value(), "GOBLIN"))
					m_monster_data->m_type_info.m_subtype = "goblin";
					//m_monster_data->m_type_info.m_subtype = WorldObject::TypeInfo::SUBTYPE_GOBLIN;
			}
			else if (!strcmp(pAttrib->Name(), "fraction"))
			{
				if (!strcmp(pAttrib->Value(), "MONSTER"))
					m_monster_data->m_type_info.m_fraction = WorldObject::TypeInfo::FRAC_MONSTER;
			}
			else if (!strcmp(pAttrib->Name(), "category"))
			{
				if (!strcmp(pAttrib->Value(), "GOBLIN"))
					m_monster_data->m_type_info.m_category = WorldObject::TypeInfo::GOBLIN;
			}
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Dropslots" && pAttrib)
	{
		while (element == "Dropslots" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "p0") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_size_prob[0] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "p1") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_size_prob[1] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "p2") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_size_prob[2] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "p3") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_size_prob[3] = static_cast<float>(dval);
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Dropslot0" && pAttrib)
	{
		while (element == "Dropslot0" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "min_level") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_min_level[0] = ival;
			else if (!strcmp(pAttrib->Name(), "max_level") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_max_level[0] = ival;
			else if (!strcmp(pAttrib->Name(), "magic_prob") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_magic_prob[0] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "magic_power") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_magic_power[0] = static_cast<float>(dval);
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Dropslot1" && pAttrib)
	{
		while (element == "Dropslot1" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "min_level") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_min_level[1] = ival;
			else if (!strcmp(pAttrib->Name(), "max_level") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_max_level[1] = ival;
			else if (!strcmp(pAttrib->Name(), "magic_prob") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_magic_prob[1] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "magic_power") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_magic_power[1] = static_cast<float>(dval);
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "BasicAttributes" && pAttrib)
	{
		for (int k=0; k<6; k++)
			m_monster_data->m_base_attr.m_abilities[k]=0;
		
		while (element == "BasicAttributes" && pAttrib)
		{
			/*printf( "%s: value=[%s]", pAttrib->Name(), pAttrib->Value());*/
			
			/*cout << "<<< " << element << " >>>" << endl;
			if (pAttrib)
				cout << "+++ " << pAttrib->Name() << " +++" << endl;*/
			
			if (!strcmp(pAttrib->Name(), "max_experience") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_max_experience = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "level") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_level = static_cast<char>(ival);
			else if (!strcmp(pAttrib->Name(), "max_health") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_max_health = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "armor") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_armor = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "block") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_block = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "attack") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_attack = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "strength") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_strength = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dexterity") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_dexterity = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "magic_power") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_magic_power = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "willpower") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_willpower = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "resistances_physical") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_resistances[Damage::PHYSICAL] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "resistances_air") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_resistances[Damage::AIR] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "resistances_ice") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_resistances[Damage::ICE] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "resistances_fire") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_resistances[Damage::FIRE] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "resistances_cap_physical") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_resistances_cap[Damage::PHYSICAL] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "resistances_cap_air") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_resistances_cap[Damage::AIR] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "resistances_cap_ice") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_resistances_cap[Damage::ICE] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "resistances_cap_fire") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_resistances_cap[Damage::FIRE] = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "walk_speed") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_walk_speed = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "attack_speed") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_attack_speed = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "step_length") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_step_length = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "abilities0") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_abilities[0] = ival;
			else if (!strcmp(pAttrib->Name(), "abilities1") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_abilities[1] = ival;
			else if (!strcmp(pAttrib->Name(), "abilities2") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_abilities[2] = ival;
			else if (!strcmp(pAttrib->Name(), "abilities3") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_abilities[3] = ival;
			else if (!strcmp(pAttrib->Name(), "abilities4") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_abilities[4] = ival;
			else if (!strcmp(pAttrib->Name(), "abilities5") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_abilities[5] = ival;
			else if (!strcmp(pAttrib->Name(), "attack_range") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_attack_range = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "special_flags") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_special_flags = ival;
			else if (!strcmp(pAttrib->Name(), "immunity") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_monster_data->m_base_attr.m_immunity = static_cast<char>(ival);
			/*
			else if (!strcmp(pAttrib->Name(), "ability") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
			{
				// Enum name der Aktion
				std::string aname = pAttrib->Name();
				
				// Suche in der Liste der registrierten Aktionen
				map<std::string, Action::ActionType>::iterator it;
				it = Action::m_enum_string_to_type.find(aname);
				
				if (it != Action::m_enum_string_to_type.end())
				{
					// in der Liste gefunden
					// Nummer der Aktion
					int anr =it->second;
					// Bit setzen
					m_monster_data->m_base_attr.m_abilities[anr /32] |= (1 << (anr %32));
					
				}
			}
			*/
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Geometry" && pAttrib)
	{
		while (element == "Geometry" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "radius") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_monster_data->m_radius = static_cast<float>(dval);
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "AI" && pAttrib)
	{
		while (element == "AI" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "ai_sight_range") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_monster_data->m_ai_sight_range = static_cast<float>(dval);
			
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	return i;
}


void ObjectLoader::searchXml(TiXmlNode* pParent)
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
		num = generateObjects(pParent->ToElement(), pParent->Value());
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
		searchXml(pChild);
		//DEBUG("Searching Document");
		//cout << "Immer:  " << pChild->Value() << endl;
		//if (pChild->Type() == TiXmlNode::ELEMENT)
			//cout << "Element:" << pChild->Value() << endl;
		if ( !strcmp(pChild->Value(), "Monster") && pChild->Type() == TiXmlNode::ELEMENT) //|| pChild->NextSibling() == 0) || monster_data == 0)
		{
			m_monster_data->m_drop_slots[0].init(m_size_prob, m_min_level[0], m_max_level[0], m_magic_prob[0], m_magic_power[0]);
			m_monster_data->m_drop_slots[1].init(m_size_prob, m_min_level[1], m_max_level[1], m_magic_prob[1], m_magic_power[1]);
			//registerMonster(WorldObject::TypeInfo::SUBTYPE_GOBLIN, m_monster_data); FIXME
			m_monster_list->push_back(m_monster_data);
			m_monster_data = 0;
			DEBUG("Object loaded");
		}
	}
}


list<MonsterBasicData*>* ObjectLoader::loadObjects(const char* pFilename) // TODO Noch nicht benutzen!
{
	for (int i=0; i<4; i++)
	{
		m_size_prob[i] = 0.0;
	}
	for (int i=0; i<2; i++)
	{
		m_min_level[i] = 0;
		m_max_level[i] = 0;
		m_magic_prob[i] = 0.0;
		m_magic_power[i] = 0.0;
	}
	m_monster_data = 0;
	m_monster_list = new list<MonsterBasicData*>;
	
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	
	if (loadOkay)
	{
		DEBUG("Loading %s", pFilename);
		searchXml(&doc);
		DEBUG("Loading %s finished", pFilename);
		return m_monster_list;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return 0;
	}
}


