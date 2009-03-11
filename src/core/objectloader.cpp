

#include "scene.h"
#include "player.h"
#include "fixedobject.h"
#include "monster.h"
#include "objectfactory.h"
#include "objectloader.h"
#include "scene.h"



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
		attr.getString("name",data->m_name,subtype);
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
					attr.getFloat("ai_sight_range",data-> m_ai_vars.m_sight_range,12);
					attr.getFloat("ai_shoot_range",data-> m_ai_vars.m_shoot_range,data-> m_ai_vars.m_sight_range);
					attr.getFloat("ai_action_range",data-> m_ai_vars.m_action_range,data-> m_ai_vars.m_sight_range);
					attr.getFloat("ai_randaction_prob",data-> m_ai_vars.m_randaction_prob,0);
					attr.getFloat("ai_chase_range",data-> m_ai_vars.m_chase_distance,20);
					
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

// neuer FixedObject loader
bool ObjectLoader::loadFixedObjectData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadFixedObject(&doc);
		return true;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return false;
	}
}

bool ObjectLoader::loadFixedObject(TiXmlNode* node)
{
	
	TiXmlNode* child;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "Object"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		FixedObjectData* data = new FixedObjectData;
		WorldObject::TypeInfo::ObjectSubtype subtype;
		attr.getString("subtype",subtype);
		std::string layer;
		attr.getString("layer",layer, "NORMAL");
		
		if (layer == "NORMAL")
			data->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
		else if (layer == "BASE")
			data->m_layer = WorldObject::LAYER_BASE;
		else if (layer == "AIR")
			data->m_layer = WorldObject::LAYER_AIR;
		
		// Schleife ueber die Elemente von Object
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			if (child->Type()==TiXmlNode::ELEMENT && !strcmp(child->Value(), "Mesh"))
			{
				attr.parseElement(child->ToElement());
				std::string mesh;
				attr.getString("file",mesh);
				
				Scene::registerObject(subtype,mesh);
			}
			else if (child->Type()==TiXmlNode::ELEMENT && !strcmp(child->Value(), "Geometry"))
			{
				attr.parseElement(child->ToElement());
				std::string shape;
				attr.getString("shape",shape,"CIRCLE");
				if (shape == "RECT")
				{
					data->m_shape.m_type = Shape::RECT;
					attr.getFloat("extent_x",data->m_shape.m_extent.m_x,0);
					attr.getFloat("extent_y",data->m_shape.m_extent.m_y,0);
				}
				else
				{
					data->m_shape.m_type = Shape::CIRCLE;
					attr.getFloat("radius",data->m_shape.m_radius,0);
				}
			}
		}
		ObjectFactory::registerFixedObject(subtype,data);
	}
	else
	{
		// rekursiv durchmustern
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadFixedObject(child);
		}
	}	
	return true;
}

