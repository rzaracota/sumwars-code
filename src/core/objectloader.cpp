

#include "scene.h"
#include "player.h"
#include "fixedobject.h"
#include "monster.h"
#include "objectfactory.h"
#include "objectloader.h"
#include "graphicmanager.h"
#include "projectile.h"


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
		std::string type,subtype, race, fraction,layer;
		attr.getString("type",data->m_type,"MONSTER");
		attr.getString("subtype",subtype);
		attr.getString("race",race);
		attr.getString("fraction",fraction,"MONSTER");
		attr.getString("layer",layer,"NORMAL");
		attr.getString("name",data->m_name,subtype);
		data->m_subtype = subtype;
		
		DEBUG5("monster: %s %s %s %s",type.c_str(), subtype.c_str(), race.c_str(), fraction.c_str());
		
		// String in enums umwandeln

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
	
		data->m_race = race;
		
		if (layer == "NORMAL")
			data->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
		else if (layer == "BASE")
			data->m_layer = WorldObject::LAYER_BASE;
		else if (layer == "AIR")
			data->m_layer = WorldObject::LAYER_AIR;
		
		// Datenfelder nullen
		data->m_base_attr.m_immunity =0;
		data->m_base_attr.m_special_flags =0;
		
		
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
					
					DEBUG5("mesh %s %s",subtype.c_str(), mesh.c_str());
					GraphicManager::registerGraphicMapping(subtype, mesh);
				}
				else if (!strcmp(child->Value(), "RenderInfo"))
				{
					std::string mesh;
					attr.getString("name",mesh);
					
					DEBUG5("mesh %s %s",subtype.c_str(), mesh.c_str());
					GraphicManager::registerGraphicMapping(subtype, mesh);
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
					
					loadCreatureBaseAttr(child,data->m_base_attr);
				
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
					attr.getFloat("ai_warn_range",data-> m_ai_vars.m_warn_radius,4);
					
				}
				else if (child->Type()!=TiXmlNode::COMMENT)
				{
					DEBUG("unexpected element of <Monster>: %s",child->Value());
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
			else if (child->Type()!=TiXmlNode::COMMENT)
			{
				DEBUG("unexpected element of <MonsterGroup>: %s",child->Value());
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

bool  ObjectLoader::loadCreatureBaseAttr(TiXmlNode* node, CreatureBaseAttr& basattr)
{
	ElementAttrib attr;
	attr.parseElement(node->ToElement());
	
	// Basisattribute setzen
	attr.getFloat("experience",basattr.m_max_experience,0);
	short level;
	attr.getShort("level",level,1);
	basattr.m_level = level;
	attr.getFloat("max_health",basattr.m_max_health,1);
	attr.getShort("armor",basattr.m_armor,0);
	attr.getShort("block",basattr.m_block,0);
	attr.getShort("attack",basattr.m_attack,1);
	attr.getShort("power",basattr.m_power,1);
	attr.getShort("strength",basattr.m_strength,1);
	attr.getShort("dexterity",basattr.m_dexterity,1);
	attr.getShort("magic_power",basattr.m_magic_power,1);
	attr.getShort("willpower",basattr.m_willpower,1);
	attr.getShort("resistances_fire",basattr.m_resistances[Damage::FIRE],0);
	attr.getShort("resistances_ice",basattr.m_resistances[Damage::ICE],0);
	attr.getShort("resistances_air",basattr.m_resistances[Damage::AIR],0);
	attr.getShort("resistances_physical",basattr.m_resistances[Damage::PHYSICAL],0);
	attr.getShort("resistances_fire",basattr.m_resistances[Damage::FIRE],0);
	attr.getShort("resistances_ice",basattr.m_resistances[Damage::ICE],0);
	attr.getShort("resistances_air",basattr.m_resistances[Damage::AIR],0);
	attr.getShort("resistances_physical",basattr.m_resistances[Damage::PHYSICAL],0);
	attr.getShort("walk_speed",basattr.m_walk_speed,2000);	
	attr.getShort("attack_speed",basattr.m_attack_speed,1500);	
	attr.getFloat("step_length",basattr.m_step_length,1);
	attr.getFloat("attack_range",basattr.m_attack_range,1);

					
	// Schleife ueber die Elemente von BasicAttributes
	TiXmlNode* child = node;
	for (TiXmlNode* child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
	{
		attr.parseElement(child2->ToElement());
						
		if (!strcmp(child2->Value(), "Ability"))
		{
			std::string anim,act;
			Action::ActionType action;
			attr.getString("type",act);
			basattr.m_abilities[act].m_timer = 0;
							
		}
		else if (!strcmp(child2->Value(), "Immunity"))
		{
			std::string type;
			attr.getString("type",type);
							
							// string -> enum
			if (type == "blind")
				basattr.m_immunity |= 1<<Damage::BLIND;
			else if (type ==  "poisoned")
				basattr.m_immunity |= 1<<Damage::POISONED;
			else if (type ==  "berserk")
				basattr.m_immunity |= 1<<Damage::BERSERK;
			else if (type ==  "confused")
				basattr.m_immunity |= 1<<Damage::CONFUSED;
			else if (type ==  "mute")
				basattr.m_immunity |= 1<<Damage::MUTE;
			else if (type ==  "paralyzed")
				basattr.m_immunity |= 1<<Damage::PARALYZED;
			else if (type ==  "frozen")
				basattr.m_immunity |= 1<<Damage::FROZEN;
			else if (type ==  "burning")
				basattr.m_immunity |= 1<<Damage::BURNING;
							
		}
		else if (!strcmp(child2->Value(), "SpecialFlag"))
		{
			std::string type;
			attr.getString("type",type);
							
							// string -> enum
			/*
			if (type== "noflags")
				basattr.m_special_flags |= Damage::NOFLAGS;
			else if (type ==  "unblockable")
				basattr.m_special_flags |= Damage::UNBLOCKABLE;
			else if (type ==  "ignore_armor")
				basattr.m_special_flags |= Damage::IGNORE_ARMOR;
			else if (type == "extra_human_dmg")
				basattr.m_special_flags |= Damage::EXTRA_HUMAN_DMG;
			else if (type == "extra_demon_dmg")
				basattr.m_special_flags |= Damage::EXTRA_DEMON_DMG;
			else if (type ==  "extra_undead_dmg")
				basattr.m_special_flags |= Damage::EXTRA_UNDEAD_DMG;
			else if (type == "extra_dwarf_dmg")
				basattr.m_special_flags |= Damage::EXTRA_DWARF_DMG;
			else if (type == "extra_drake_dmg")
				basattr.m_special_flags |= Damage::EXTRA_DRAKE_DMG;
			else if (type == "extra_fairy_dmg")
				basattr.m_special_flags |= Damage::EXTRA_FAIRY_DMG;
			else if (type == "extra_goblin_dmg")
				basattr.m_special_flags |= Damage::EXTRA_GOBLIN_DMG;
			else if (type == "extra_animal_dmg")
				basattr.m_special_flags |= Damage::EXTRA_ANIMAL_DMG;
			else if (type == "extra_summoned_dmg")
				basattr.m_special_flags |= Damage::EXTRA_SUMMONED_DMG;
			*/
		}
		else if (child2->Type()!=TiXmlNode::COMMENT)
		{
			DEBUG("unexpected element of <BasicAttributes>: %s",child2->Value());
		}
	}
	
	return true;
}

bool ObjectLoader::loadPlayerData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadPlayer(&doc);
		return true;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return false;
	}
}

bool ObjectLoader::loadPlayer(TiXmlNode* node)
{
	TiXmlNode* child;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "PlayerClass"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		PlayerBasicData* data = new PlayerBasicData;
		attr.getString("type",data->m_subtype);
		attr.getString("name",data->m_name,data->m_subtype);
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			attr.parseElement(child->ToElement());
			if (!strcmp(child->Value(), "BasicAttributes"))
			{
				loadCreatureBaseAttr(child, data->m_base_attr);
			}
			else if (!strcmp(child->Value(), "BasicAbility"))
			{
				attr.getString("type",data->m_base_ability);
			}
			else if (!strcmp(child->Value(), "Look"))
			{
				std::string gender;
				std::string look;
				attr.getString("name",look);
				attr.getString("gender",gender);
				Scene::registerPlayerLook(data->m_subtype,look, gender=="male");
			}
			else if (!strcmp(child->Value(), "SkilltreeTabs"))
			{
				attr.getString("tab1",data->m_tabnames[0]);
				attr.getString("tab2",data->m_tabnames[1]);
				attr.getString("tab3",data->m_tabnames[2]);
			}
			else if (!strcmp(child->Value(), "Item"))
			{
				GameObject::Subtype type;
				attr.getString("type",type);
				data->m_start_items.push_back(type);
			}
			else if (!strcmp(child->Value(), "LearnableAbility"))
			{
				int id = data->m_learnable_abilities.size();
				LearnableAbility& ablt = data->m_learnable_abilities[id];
				ablt.m_id = id;
				attr.getString("type",ablt.m_type);
				attr.getInt("skilltree_tab",ablt.m_skilltree_tab);
				attr.getFloat("skilltree_posx",ablt.m_skilltree_position.m_x);
				attr.getFloat("skilltree_posy",ablt.m_skilltree_position.m_y);
				attr.getShort("required_level",ablt.m_req_level);
				
				for (TiXmlNode* child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					attr.parseElement(child2->ToElement());
					if (!strcmp(child2->Value(), "RequiredAbility"))
					{
						Action::ActionType atype;
						attr.getString("type",atype);
						ablt.m_req_abilities.push_back(atype);
					}
					else if (child2->Type()!=TiXmlNode::COMMENT)
					{
						DEBUG("unexpected element of <LearnableAbility>: %s",child2->Value());
					}
					
				}
			}
			else if (child->Type()!=TiXmlNode::COMMENT)
			{
				DEBUG("unexpected element of <PlayerClass>: %s",child->Value());
			}
		}
		ObjectFactory::registerPlayer(data->m_subtype,data);
		
	}
	else
	{
		// rekursiv durchmustern
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadPlayer(child);
		}
	}
	
	return true;
}

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
		GameObject::Subtype subtype;
		attr.getString("subtype",subtype);
		std::string layer;
		attr.getString("layer",layer, "NORMAL");
		
		if (layer == "NORMAL")
			data->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
		else if (layer == "BASE")
			data->m_layer = WorldObject::LAYER_BASE;
		else if (layer == "AIR")
			data->m_layer = WorldObject::LAYER_AIR;
		else if (layer == "NOCOLLISION")
			data->m_layer = WorldObject::LAYER_NOCOLLISION;
		
		// Schleife ueber die Elemente von Object
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			if (child->Type()==TiXmlNode::ELEMENT && !strcmp(child->Value(), "Mesh"))
			{
				attr.parseElement(child->ToElement());
				std::string mesh;
				attr.getString("file",mesh);
				
				GraphicManager::registerGraphicMapping(subtype, mesh);
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
			else if (child->Type()!=TiXmlNode::COMMENT)
			{
				DEBUG5("unexpected element of <Object>: %s",child->Value());
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

bool ObjectLoader::loadProjectileData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadProjectile(&doc);
		return true;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return false;
	}
}


bool ObjectLoader::loadProjectile(TiXmlNode* node)
{
	TiXmlNode* child;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "Projectile"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		ProjectileBasicData* data = new ProjectileBasicData;
		data->m_flags =0;
		
		attr.getString("subtype",data->m_subtype);
		attr.getFloat("radius",data->m_radius,0.1);
		attr.getFloat("lifetime",data->m_lifetime);
		attr.getInt("counter",data->m_counter);
		attr.getString("new_projectile_type",data->m_new_projectile_type);
		attr.getString("implementation",data->m_implementation);
		
		// Schleife ueber die Elemente von Object
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			if (child->Type()==TiXmlNode::ELEMENT)
			{
				attr.parseElement(child->ToElement());
				if (!strcmp(child->Value(), "Mesh"))
				{
					std::string mesh;
					attr.getString("file",mesh);
					
					DEBUG5("mesh %s %s",data->m_subtype.c_str(), mesh.c_str());
					GraphicManager::registerGraphicMapping(data->m_subtype, mesh);
				}
				else if (!strcmp(child->Value(), "RenderInfo"))
				{
					std::string mesh;
					attr.getString("name",mesh);
					
					DEBUG5("mesh %s %s",data->m_subtype.c_str(), mesh.c_str());
					GraphicManager::registerGraphicMapping(data->m_subtype, mesh);
				}
				else if (!strcmp(child->Value(), "Flag"))
				{
					std::string flag;
					attr.getString("name",flag);
					if (flag == "explodes")
					{
						data->m_flags |= Projectile::EXPLODES;
					}
					else if (flag == "multi_explodes")
					{
						data->m_flags |= Projectile::MULTI_EXPLODES;
					}
					else if (flag == "piercing")
					{
						data->m_flags |= Projectile::PIERCING;
								
					}
					else if (flag == "bouncing")
					{
						data->m_flags |= Projectile::BOUNCING;
					}
					else if (flag == "prob_bouncing")
					{
						data->m_flags |= Projectile::PROB_BOUNCING;
					}
				}
				else if (!strcmp(child->Value(), "Effect"))
				{
					Action::loadHybridImplementation(child,data->m_effect);
				}
			}
		}
	
		ObjectFactory::registerProjectile(data->m_subtype,data);
	}
	else
	{
		// rekursiv durchmustern
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadProjectile(child);
		}
	}	
	return true;
}
