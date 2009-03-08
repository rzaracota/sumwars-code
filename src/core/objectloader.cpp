

#include "scene.h"
#include "player.h"
#include "fixedobject.h"
#include "monster.h"
#include "objectfactory.h"
#include "objectloader.h"


//##############################################################################

int ObjectLoader::generateFixedObjectData(TiXmlElement* pElement, string element, std::list<FixedObjectData*>* object_list, std::list<string>* subtype_list)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	double dval;

	if (element == "Object" && pAttrib)
	{
		if (m_object_data == 0)
		{
			m_object_data = new FixedObjectData;
		}

		while (element == "Object" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "subtype"))
				m_subtype = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Layer" && pAttrib)
	{
		while (element == "Layer" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "layer"))
			{
				if (!strcmp(pAttrib->Value(), "NORMAL"))
					m_object_data->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
				else if (!strcmp(pAttrib->Value(), "BASE"))
					m_object_data->m_layer = WorldObject::LAYER_BASE;
				else if (!strcmp(pAttrib->Value(), "AIR"))
					m_object_data->m_layer = WorldObject::LAYER_AIR;
			}

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Geometry" && pAttrib)
	{
		while (element == "Geometry" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "shape"))
			{
				if (!strcmp(pAttrib->Value(), "RECT"))
					m_object_data->m_shape.m_type = Shape::RECT;
				else if (!strcmp(pAttrib->Value(), "CIRCLE"))
					m_object_data->m_shape.m_type = Shape::CIRCLE;
			}
			else if (!strcmp(pAttrib->Name(), "extent_x") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_extent[0] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "extent_y") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_extent[1] = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "radius") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_radius = static_cast<float>(dval);

			i++;
			pAttrib=pAttrib->Next();
		}
	}

	return i;
}


void ObjectLoader::searchFixedObjectData(TiXmlNode* pParent, std::list<FixedObjectData*>* object_list, std::list<string>* subtype_list)
{
	if ( !pParent ) return;

	TiXmlNode* pChild;
//	TiXmlText* pText;

	int t = pParent->Type();
	int num;

	switch ( t )
	{
	case TiXmlNode::ELEMENT:
		//printf( "Element [%s]", pParent->Value() );
		num = generateFixedObjectData(pParent->ToElement(), pParent->Value(), object_list, subtype_list);
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
		searchFixedObjectData(pChild, object_list, subtype_list);

		if ( !strcmp(pChild->Value(), "Object") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			if (m_object_data->m_shape.m_type == Shape::RECT)
			{
				m_object_data->m_shape.m_extent = Vector(m_extent[0],m_extent[1]);
				m_object_data->m_shape.m_extent = Vector(m_extent[0],m_extent[1]);
			}
			else if (m_object_data->m_shape.m_type == Shape::CIRCLE)
			{
				m_object_data->m_shape.m_radius = m_radius;
			}
			object_list->push_back(m_object_data);
			subtype_list->push_back(m_subtype);
			m_object_data = 0;
			DEBUG5("Object loaded");
		}
	}
}


bool ObjectLoader::loadFixedObjectData(const char* pFilename, std::list<FixedObjectData*>* &object_list, std::list<string>* &subtype_list)
{
	m_object_data = 0;
	
	object_list = new std::list<FixedObjectData*>;
	subtype_list = new std::list<std::string>;

	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		DEBUG5("Loading %s", pFilename);
		searchFixedObjectData(&doc, object_list, subtype_list);
		DEBUG5("Loading %s finished", pFilename);
		//return m_object_list;
		return true;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return false;
	}
}

//##############################################################################

int ObjectLoader::generateFixedObjectMeshData(TiXmlElement* pElement, string element)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;

	if (element == "Object" && pAttrib)
	{
		if (m_fixed_object_mesh_data == 0)
		{
			m_fixed_object_mesh_data = new FixedObjectMeshData;
		}

		while (element == "Object" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "subtype"))
				m_fixed_object_mesh_data->m_subtype = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}

	if (element == "Mesh" && pAttrib)
	{
		while (element == "Mesh" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "file"))
				m_fixed_object_mesh_data->m_mesh = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}

	return i;
}


void ObjectLoader::searchFixedObjectMeshData(TiXmlNode* pParent)
{
	if ( !pParent ) return;

	TiXmlNode* pChild;
//	TiXmlText* pText;

	int t = pParent->Type();
	int num;

	switch ( t )
	{
	case TiXmlNode::ELEMENT:
		//printf( "Element [%s]", pParent->Value() );
		num = generateFixedObjectMeshData(pParent->ToElement(), pParent->Value());
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
		searchFixedObjectMeshData(pChild);

		if ( !strcmp(pChild->Value(), "Object") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_fixed_object_mesh_list->push_back(m_fixed_object_mesh_data);
			m_fixed_object_mesh_data = 0;
			DEBUG5("FixedObject Mesh loaded");
		}
	}
}


std::list<FixedObjectMeshData*>* ObjectLoader::loadFixedObjectMeshData(const char* pFilename)
{
	m_fixed_object_mesh_data = 0;
	m_fixed_object_mesh_list = new std::list<FixedObjectMeshData*>;

	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		DEBUG5("Loading %s", pFilename);
		searchFixedObjectMeshData(&doc);
		DEBUG5("Loading %s finished", pFilename);
		return m_fixed_object_mesh_list;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return 0;
	}
}




///// neuer Monsterloader

bool ObjectLoader::loadMonsterData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadMonster(&doc);
		return true;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return false;
	}
}

bool  ObjectLoader::loadMonster(TiXmlNode* node)
{
	TiXmlNode* child;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "Monster"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		MonsterBasicData* data = new MonsterBasicData;
		
		// Typinformationen auslesen
		std::string type,subtype, category, fraction,layer;
		attr.getString("type",type,"MONSTER");
		attr.getString("subtype",subtype);
		attr.getString("category",category);
		attr.getString("fraction",fraction,"MONSTER");
		attr.getString("layer",layer,"NORMAL");
		data->m_type_info.m_subtype = subtype;
		
		DEBUG5("monster: %s %s %s %s",type.c_str(), subtype.c_str(), category.c_str(), fraction.c_str());
		
		// String in enums umwandeln
		if (type == "NONE") 
			data->m_type_info.m_type = WorldObject::TypeInfo::TYPE_NONE;
		else if (type ==  "PLAYER")
			data->m_type_info.m_type = WorldObject::TypeInfo::TYPE_PLAYER;
		else if (type ==  "MONSTER")
			data->m_type_info.m_type = WorldObject::TypeInfo::TYPE_MONSTER;
		else if (type ==  "TRADER")
			data->m_type_info.m_type = WorldObject::TypeInfo::TYPE_TRADER;

		if (fraction ==  "NOFRACTION")
			data->m_fraction = WorldObject::NOFRACTION;
		else if (fraction == "HUMAN")
			data->m_fraction = WorldObject::FRAC_HUMAN;
		else if (fraction == "DEMON")
			data->m_fraction = WorldObject::FRAC_DEMON;
		else if (fraction == "UNDEAD")
			data->m_fraction = WorldObject::FRAC_UNDEAD;
		else if (fraction == "DWARF")
			data->m_fraction = WorldObject::FRAC_DWARF;
		else if (fraction == "SUMMONER")
			data->m_fraction = WorldObject::FRAC_SUMMONER;
		else if (fraction == "MONSTER")
			data->m_fraction = WorldObject::FRAC_MONSTER;
		else if (fraction == "HOSTILE_TO_ALL")
			data->m_fraction = WorldObject::FRAC_HOSTILE_TO_ALL;
		else if (fraction == "PLAYER_PARTY")
			data->m_fraction = WorldObject::FRAC_PLAYER_PARTY;
	
		
		if (category == "NOCATEGORY")
			data->m_category = WorldObject::NOCATEGORY;
		else if (category == "HUMAN")
			data->m_category = WorldObject::HUMAN;
		else if (category == "DEMON")
			data->m_category = WorldObject::DEMON;
		else if (category == "UNDEAD")
			data->m_category = WorldObject::UNDEAD;
		else if (category == "DWARF")
			data->m_category = WorldObject::DWARF;
		else if (category == "DRAKE")
			data->m_category = WorldObject::DRAKE;
		else if (category =="FAIRY")
			data->m_category = WorldObject::FAIRY;
		else if (category == "GOBLIN")
			data->m_category = WorldObject::GOBLIN;
		else if (category == "ANIMAL")
			data->m_category = WorldObject::ANIMAL;
		else if (category == "SUMMONED")
			data->m_category = WorldObject::SUMMONED;
		
		if (layer == "NORMAL")
			data->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
		else if (layer == "BASE")
			data->m_layer = WorldObject::LAYER_BASE;
		else if (layer == "AIR")
			data->m_layer = WorldObject::LAYER_AIR;
		
		// Datenfelder nullen
		data->m_base_attr.m_immunity =0;
		data->m_base_attr.m_special_flags =0;
		for (int i=0; i<6; i++)
			data->m_base_attr.m_abilities[i] =0;
		
		// Schleife ueber die Elemente von Monster
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			if (child->Type()==TiXmlNode::ELEMENT)
			{
				attr.parseElement(child->ToElement());
				
				if (!strcmp(child->Value(), "Mesh"))
				{
					std::string mesh;
					attr.getString("file",mesh);
					Scene::registerObject(subtype, mesh, "");
					
					DEBUG5("mesh %s %s",subtype.c_str(), mesh.c_str());
				}
				else if (!strcmp(child->Value(), "ExtraMesh"))
				{
					std::string bone,mesh;
					attr.getString("file",mesh);
					attr.getString("bone",bone);
					Scene::registerAttachedMesh(subtype,bone,mesh);	
				}
				else if (!strcmp(child->Value(), "Dropslots"))
				{
					int nr =0;
					for (TiXmlNode* child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
					{
						attr.parseElement(child2->ToElement());
						attr.getFloat("pbig",data->m_drop_slots[nr].m_size_probability[Item::BIG],0);
						attr.getFloat("pmedium",data->m_drop_slots[nr].m_size_probability[Item::MEDIUM],0);
						attr.getFloat("psmall",data->m_drop_slots[nr].m_size_probability[Item::SMALL],0);
						attr.getFloat("pgold",data->m_drop_slots[nr].m_size_probability[Item::GOLD],0);
						data->m_drop_slots[nr].m_size_probability[4] =1;
						for (int i=0; i<4; i++)
							data->m_drop_slots[nr].m_size_probability[4] -= data->m_drop_slots[nr].m_size_probability[i];
						
						attr.getInt("min_level",data->m_drop_slots[nr].m_min_level,0);
						attr.getInt("max_level",data->m_drop_slots[nr].m_max_level,0);
						attr.getInt("min_gold",data->m_drop_slots[nr].m_min_gold,1);
						attr.getInt("max_gold",data->m_drop_slots[nr].m_max_gold,1);
						attr.getFloat("magic_prob",data->m_drop_slots[nr].m_magic_probability,0);
						attr.getFloat("magic_power",data->m_drop_slots[nr].m_magic_power,0);
						
						DEBUG5("Dropslot %i %i %f %f", data->m_drop_slots[nr].m_min_level,data->m_drop_slots[nr].m_max_level,  data->m_drop_slots[nr].m_magic_probability, data->m_drop_slots[nr].m_magic_power);
						nr ++;
					}
					
				}
				else if (!strcmp(child->Value(), "BasicAttributes"))
				{
					// Basisattribute setzen
					attr.getFloat("experience",data->m_base_attr.m_max_experience,0);
					short level;
					attr.getShort("level",level,1);
					data->m_base_attr.m_level = level;
					attr.getFloat("max_health",data->m_base_attr.m_max_health,1);
					attr.getShort("armor",data->m_base_attr.m_armor,0);
					attr.getShort("block",data->m_base_attr.m_block,0);
					attr.getShort("armor",data->m_base_attr.m_attack,0);
					attr.getShort("strength",data->m_base_attr.m_strength,1);
					attr.getShort("dexterity",data->m_base_attr.m_dexterity,1);
					attr.getShort("magic_power",data->m_base_attr.m_magic_power,1);
					attr.getShort("willpower",data->m_base_attr.m_willpower,1);
					attr.getShort("resistances_fire",data->m_base_attr.m_resistances[Damage::FIRE],0);
					attr.getShort("resistances_ice",data->m_base_attr.m_resistances[Damage::ICE],0);
					attr.getShort("resistances_air",data->m_base_attr.m_resistances[Damage::AIR],0);
					attr.getShort("resistances_physical",data->m_base_attr.m_resistances[Damage::PHYSICAL],0);
					attr.getShort("resistances_fire",data->m_base_attr.m_resistances[Damage::FIRE],0);
					attr.getShort("resistances_ice",data->m_base_attr.m_resistances[Damage::ICE],0);
					attr.getShort("resistances_air",data->m_base_attr.m_resistances[Damage::AIR],0);
					attr.getShort("resistances_physical",data->m_base_attr.m_resistances[Damage::PHYSICAL],0);
					attr.getShort("walk_speed",data->m_base_attr.m_walk_speed,2000);	
					attr.getShort("attack_speed",data->m_base_attr.m_attack_speed,1500);	
					attr.getFloat("step_length",data->m_base_attr.m_step_length,1);
					attr.getFloat("attack_range",data->m_base_attr.m_attack_range,1);

					// Animationen
					std::map<Action::ActionType, std::vector<std::string> > animations;
					
					// Schleife ueber die Elemente von BasicAttributes
					for (TiXmlNode* child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
					{
						attr.parseElement(child2->ToElement());
						
						if (!strcmp(child2->Value(), "Ability"))
						{
							std::string anim,act;
							Action::ActionType action;
							attr.getString("type",act);
							action = Action::getActionType(act);
							data->m_base_attr.m_abilities[action/32] |= 1<<(action%32);
							
							for (TiXmlNode* child3 = child2->FirstChild(); child3 != 0; child3 = child3->NextSibling())
							{
								attr.parseElement(child3->ToElement());
								attr.getString("name",anim);
								if (anim != "")
								{
									animations[action].push_back(anim);
									
									DEBUG5("animation %s %s %s",subtype.c_str(),act.c_str(), anim.c_str());
								}
							}
							
						}
						else if (!strcmp(child2->Value(), "Immunity"))
						{
							std::string type;
							attr.getString("type",type);
							
							// string -> enum
							if (type == "blind")
								data->m_base_attr.m_immunity |= 1<<Damage::BLIND;
							else if (type ==  "poisoned")
								data->m_base_attr.m_immunity |= 1<<Damage::POISONED;
							else if (type ==  "berserk")
								data->m_base_attr.m_immunity |= 1<<Damage::BERSERK;
							else if (type ==  "confused")
								data->m_base_attr.m_immunity |= 1<<Damage::CONFUSED;
							else if (type ==  "mute")
								data->m_base_attr.m_immunity |= 1<<Damage::MUTE;
							else if (type ==  "paralyzed")
								data->m_base_attr.m_immunity |= 1<<Damage::PARALYZED;
							else if (type ==  "frozen")
								data->m_base_attr.m_immunity |= 1<<Damage::FROZEN;
							else if (type ==  "burning")
								data->m_base_attr.m_immunity |= 1<<Damage::BURNING;
							
						}
						else if (!strcmp(child2->Value(), "SpecialFlag"))
						{
							std::string type;
							attr.getString("type",type);
							
							// string -> enum
							if (type== "noflags")
								data->m_base_attr.m_special_flags |= Damage::NOFLAGS;
							else if (type ==  "unblockable")
								data->m_base_attr.m_special_flags |= Damage::UNBLOCKABLE;
							else if (type ==  "ignore_armor")
								data->m_base_attr.m_special_flags |= Damage::IGNORE_ARMOR;
							else if (type == "extra_human_dmg")
								data->m_base_attr.m_special_flags |= Damage::EXTRA_HUMAN_DMG;
							else if (type == "extra_demon_dmg")
								data->m_base_attr.m_special_flags |= Damage::EXTRA_DEMON_DMG;
							else if (type ==  "extra_undead_dmg")
								data->m_base_attr.m_special_flags |= Damage::EXTRA_UNDEAD_DMG;
							else if (type == "extra_dwarf_dmg")
								data->m_base_attr.m_special_flags |= Damage::EXTRA_DWARF_DMG;
							else if (type == "extra_drake_dmg")
								data->m_base_attr.m_special_flags |= Damage::EXTRA_DRAKE_DMG;
							else if (type == "extra_fairy_dmg")
								data->m_base_attr.m_special_flags |= Damage::EXTRA_FAIRY_DMG;
							else if (type == "extra_goblin_dmg")
								data->m_base_attr.m_special_flags |= Damage::EXTRA_GOBLIN_DMG;
							else if (type == "extra_animal_dmg")
								data->m_base_attr.m_special_flags |= Damage::EXTRA_ANIMAL_DMG;
							else if (type == "extra_summoned_dmg")
								data->m_base_attr.m_special_flags |= Damage::EXTRA_SUMMONED_DMG;
						}
					}
					
					Scene::registerObjectAnimations(subtype,animations);
				
				}
				else if (!strcmp(child->Value(), "Geometry"))
				{
					attr.getFloat("radius",data->m_radius);
				}
				else if (!strcmp(child->Value(), "AI"))
				{
					attr.getFloat("ai_sight_range",data-> m_ai_sight_range);
				}
				
			}
		}
		
		ObjectFactory::registerMonster(subtype,data);
		
	}
	else if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "MonsterGroup"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		MonsterGroup* data = new MonsterGroup;
		
		std::string name;
		attr.getString("name",name);
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			attr.parseElement(child->ToElement());
			if (!strcmp(child->Value(), "SubGroup"))
			{
				std::string subtype;
				int number;
				float prob;
				attr.getString("subtype",subtype);
				attr.getInt("number",number,1);
				attr.getFloat("prob",prob,1);
				
				data-> addMonsters(subtype,number,prob);
			}
		}
		
		ObjectFactory::registerMonsterGroup(name,data);
	}
	else
	{
		// rekursiv durchmustern
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadMonster(child);
		}
	}
	
	return true;
}
