#include "worldloader.h"
#include "world.h"

#include <iostream>



void WorldLoader::loadEvent( TiXmlNode* node, Event *ev, TriggerType &type)
{
	ElementAttrib attrib;
	
	TiXmlNode* child;
	TiXmlText* text;
	attrib.parseElement(node->ToElement());
	attrib.getString("trigger", type);
	
	std::string once;
	attrib.getString("once", once,"false");
	ev->setOnce(once=="true");

	for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
	{
		if (!strcmp(child->Value(), "Condition"))
		{
			text = child->FirstChild()->ToText();
			if (text != 0)
			{
				ev->setCondition(text->Value());
			}
		}
		else if (!strcmp(child->Value(), "Effect"))
		{
			text = child->FirstChild()->ToText();
			if (text != 0)
			{
				ev->setEffect(text->Value());
			}
		}
		else if (child->Type()!=TiXmlNode::COMMENT)
		{
			DEBUG("unexpected element of <Event>: %s",child->Value());
		}
	}
}


bool WorldLoader::loadRegionData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadRegions(&doc);
		return true;
	}
	else
	{
		ERRORMSG("Failed to load file %s", pFilename);
		return false;
	}
}

bool WorldLoader::loadRegions(TiXmlNode* node)
{
	TiXmlNode* child;
	RegionData* rdata=0;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "Region"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		rdata = new RegionData;
		
		int id;
		attr.getInt("id",id,-1);
		rdata->m_id = id;
		attr.getString("name",rdata->m_name);
		DEBUG5("Region %s %i",rdata->m_name.c_str(), id);
		World::getWorld()->registerRegionData(rdata, rdata->m_id);
		loadRegion(node,rdata);
		
	}
	else
	{
		if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "World"))
		{
			ElementAttrib attr;
			attr.parseElement(node->ToElement());
			
			attr.getString("start_region",World::getWorld()->getPlayerStartLocation().first,World::getWorld()->getPlayerStartLocation().first);
			
			attr.getString("start_location",World::getWorld()->getPlayerStartLocation().second,World::getWorld()->getPlayerStartLocation().second);
			
		}
		
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadRegions(child);
		}
	}
	return true;
}

bool WorldLoader::loadRegion(TiXmlNode* node, RegionData* rdata)
{
	
	TiXmlNode* child;
	TiXmlNode* child2;
	for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
	{
		if (child->Type()==TiXmlNode::ELEMENT)
		{
			ElementAttrib attr;
			attr.parseElement(child->ToElement());
			
			if (!strcmp(child->Value(), "Settings"))
			{
				int dimx,dimy;
				attr.getInt("dimx",dimx,16);
				attr.getInt("dimy",dimy,16);
				rdata->m_dimx = dimx;
				rdata->m_dimy = dimy;
				attr.getString("region_template",rdata->m_region_template);
				attr.getString("ground",rdata->m_ground_material);
				attr.getFloat("area_percent",rdata->m_area_percent,0.5f);
				attr.getFloat("complexity",rdata->m_complexity,0.5f);
				attr.getInt("granularity",rdata->m_granularity,8);
			}
			else if (!strcmp(child->Value(), "ExitDirections"))
			{
				string boolstr;
				attr.getString("north",boolstr,"false");
				rdata->m_exit_directions[NORTH] = (boolstr =="true");
				attr.getString("south",boolstr,"false");
				rdata->m_exit_directions[SOUTH] = (boolstr =="true");
				attr.getString("west",boolstr,"false");
				rdata->m_exit_directions[WEST] = (boolstr =="true");
				attr.getString("east",boolstr,"false");
				rdata->m_exit_directions[EAST] = (boolstr =="true");
					
			}
			else if (!strcmp(child->Value(), "Exits"))
			{
				for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					
					if (!strcmp(child2->Value(), "Exit"))
					{
						RegionExit exit;
						attr.parseElement(child2->ToElement());
						attr.getString("name",exit.m_exit_name);
						attr.getString("destination_region",exit.m_destination_region);
						attr.getString("destination_location",exit.m_destination_location);
						
						TiXmlNode* child3;
						for ( child3 = child2->FirstChild(); child3 != 0; child3 = child3->NextSibling())
						{
							
							if (!strcmp(child3->Value(), "Shape"))
							{
								attr.parseElement(child3->ToElement());
								std::string shape;
								attr.getString("shape",shape,"CIRCLE");
								if (shape == "RECT")
								{
									exit.m_shape.m_type = Shape::RECT;
									attr.getFloat("extent_x",exit.m_shape.m_extent.m_x,0);
									attr.getFloat("extent_y",exit.m_shape.m_extent.m_y,0);
								}
								else
								{
									exit.m_shape.m_type = Shape::CIRCLE;
									attr.getFloat("radius",exit.m_shape.m_radius,0);
								}
							}
							else if (child3->Type()!=TiXmlNode::COMMENT)
							{
								DEBUG("unexpected element of <Exit>: %s",child3->Value());
							}
						}
						rdata->addExit(exit);
					}
					else if (child2->Type()!=TiXmlNode::COMMENT)
					{
						DEBUG("unexpected element of <Exits>: %s",child2->Value());
					}
				}
			}
			else if (!strcmp(child->Value(), "Environments"))
			{
				for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					if (!strcmp(child2->Value(), "Environment"))
					{
						std::string envname;
						float height;
						attr.parseElement(child2->ToElement());
						attr.getString("name",envname);
						attr.getFloat("height",height);
						rdata->addEnvironment(height,envname);
					}
					else if (child2->Type()!=TiXmlNode::COMMENT)
					{
						DEBUG("unexpected element of <Environments>: %s",child2->Value());
					}
				}
			}
			else if (!strcmp(child->Value(), "ReviveLocation"))
			{
				attr.getString("location",rdata->m_revive_location);
			}
			else if (!strcmp(child->Value(), "Waypoint"))
			{
				WaypointInfo winfo;
				winfo.m_id = rdata->m_id;
				winfo.m_name = rdata->m_name;
				attr.getFloat("world_x",winfo.m_world_coord.m_x);
				attr.getFloat("world_y",winfo.m_world_coord.m_y);
				
				rdata->m_has_waypoint = true;
				World::getWorld()->addWaypoint(winfo.m_id,winfo);
			}
			else if (!strcmp(child->Value(), "Event"))
			{
				TriggerType type;
				Event* ev = new Event();
				loadEvent(child, ev,type);
				World::getWorld()->addEvent(rdata->m_name,type,ev);
			}
 			else if (!strcmp(child->Value(), "NamedObjectGroups"))
			{
				for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					if ((!strcmp(child2->Value(), "NameObjectGroup")) || (!strcmp(child2->Value(), "NamedObjectGroup")))
					{
						attr.parseElement(child2->ToElement());
						std::string group_name, name;
						int prio;
						attr.getString("name",name);
						attr.getString("object_group",group_name);
						attr.getInt("priority",prio);
						
						DEBUG5("named object group %s %s %i",group_name.c_str(), name.c_str(), prio);
						
						rdata->addNamedObjectGroupTemplate(group_name,name,prio);
					}
					else if (child2->Type()!=TiXmlNode::COMMENT)
					{
						DEBUG("unexpected element of <NamedObjectGroups>: %s",child2->Value());
					}
				}
			}
			else if (!strcmp(child->Value(), "ObjectGroups"))
			{
				for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					if (!strcmp(child2->Value(), "ObjectGroup"))
					{
						attr.parseElement(child2->ToElement());
						std::string group_name;
						int prio,number;
						float prob;
						attr.getString("object_group",group_name);
						attr.getInt("priority",prio,0);	
						attr.getInt("number",number,1);	
						attr.getFloat("probability",prob,1.0f);	
						
						DEBUG5("object group %s  number %i prio %i",group_name.c_str(), number, prio);
						
						rdata->addObjectGroupTemplate(group_name,prio, number, prob);
					}
					else if (child2->Type()!=TiXmlNode::COMMENT)
					{
						DEBUG("unexpected element of <ObjectGroups>: %s",child2->Value());
					}
				}
			}
			else if (!strcmp(child->Value(), "SpawnGroups"))
			{
				for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					if (!strcmp(child2->Value(), "SpawnGroup"))
					{
						attr.parseElement(child2->ToElement());
						std::string group_name;
						int number;
						attr.getInt("number",number,1);
						attr.getString("group_name",group_name);
						
						rdata->addSpawnGroup(group_name,number);
					}
					else if (child2->Type()!=TiXmlNode::COMMENT)
					{
						DEBUG("unexpected element of <SpawnGroup>: %s",child2->Value());
					}
				}
			}
			else if (!strcmp(child->Value(), "AmbientLight"))
			{
				attr.getFloat("red",rdata->m_ambient_light[0],0.2f);
				attr.getFloat("green",rdata->m_ambient_light[1],0.2f);
				attr.getFloat("blue",rdata->m_ambient_light[2],0.2f);
			}
			else if (!strcmp(child->Value(), "HeroLight"))
			{
				attr.getFloat("red",rdata->m_hero_light[0],1.0f);
				attr.getFloat("green",rdata->m_hero_light[1],1.0f);
				attr.getFloat("blue",rdata->m_hero_light[2],1.0f);
			}
			else if (!strcmp(child->Value(), "DirectionalLight"))
			{
				attr.getFloat("red",rdata->m_directional_light[0],0.3f);
				attr.getFloat("green",rdata->m_directional_light[1],0.3f);
				attr.getFloat("blue",rdata->m_directional_light[2],0.3f);
			}
			else if (child->Type()!=TiXmlNode::COMMENT)
			{
				DEBUG("unexpected element of <Region>: %s",child->Value());
			}
		}
	}
	return true;
}

bool  WorldLoader::loadNPCData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadNPC(&doc);
		return true;
	}
	else
	{
		ERRORMSG("Failed to load file %s", pFilename);
		return false;
	}
}

void  WorldLoader::loadNPC( TiXmlNode* node)
{
	TiXmlNode* child;
	
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "NPC"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		std::string refname;
		std::string topic;
		std::string start_op;
		Event* ev;
		TriggerType dummy;
		NPCTrade::TradeObject tradeobj;
		attr.getString("refname",refname,"global");
		
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			if (child->Type()==TiXmlNode::ELEMENT && !strcmp(child->Value(), "Topic"))
			{
				attr.parseElement(child->ToElement());
				attr.getString("name",topic,"topic");
				attr.getString("start_option",start_op);
				
				DEBUG5("found Topic %s for %s",topic.c_str(),refname.c_str());
				
				ev = new Event;
				// Topic kann mit der selben Funktion geladen werden wie Events
				loadEvent(child,ev,dummy);
				
				Dialogue::getTopicList(refname).addTopic(topic,ev);
				if (start_op != "")
				{
					Dialogue::getTopicList(refname).addStartTopic(start_op, topic);
				}
			}
			else if (child->Type()==TiXmlNode::ELEMENT && !strcmp(child->Value(), "Trade"))
			{
				attr.parseElement(child->ToElement());
				float refreshtime,price_factor;
				attr.getFloat("refresh_time", refreshtime,3600000.0f);
				attr.getFloat("price_factor",price_factor,1.5f);
				
				DEBUG5("trade conditions %f %f",refreshtime, price_factor);
				NPCTrade& tradeinfo = Dialogue::getNPCTrade(refname);
				
				tradeinfo.m_cost_multiplier = price_factor;
				tradeinfo.m_refresh_time = refreshtime;
				
				// TradeObject einlesen
				for (TiXmlNode* child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					attr.parseElement(child2->ToElement());
					
					attr.getString("subtype", tradeobj.m_subtype);
					attr.getInt("number", tradeobj.m_number,1);
					attr.getInt("number_magical",tradeobj.m_number_magical,0);
					attr.getFloat("min_enchant", tradeobj.m_min_enchant,0);
					attr.getFloat("max_enchant", tradeobj.m_min_enchant,0);
					
					tradeinfo.m_trade_objects.push_back(tradeobj);
					DEBUG5("new trade Object for %s : %s %i %i %f %f",refname.c_str(),tradeobj.m_subtype.c_str(), tradeobj.m_number, tradeobj.m_number_magical, tradeobj.m_min_enchant, tradeobj.m_min_enchant);
				}
			}
			else if (child->Type()!=TiXmlNode::COMMENT)
			{
				DEBUG("unexpected element of <NPC>: %s",child->Value());
			}
		}
	}
	else
	{
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadNPC(child);
		}
	}
}

bool WorldLoader::loadQuestsData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadQuests(&doc);
		return true;
	}
	else
	{
		ERRORMSG("Failed to load file %s", pFilename);
		return false;
	}
}

void WorldLoader::loadQuests(TiXmlNode* node)
{
	TiXmlNode* child;
	Quest* qu;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "Quest"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		std::string tabname,name;
		attr.getString("table_name",tabname);
		attr.getString("name",name);
		
		qu = new Quest(name,tabname);
		
		loadQuest(node,qu);
		World::getWorld()->addQuest(tabname,qu);
	}
	else
	{
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadQuests(child);
		}
	}
}


void WorldLoader::loadQuest(TiXmlNode* node, Quest* quest)
{
	TiXmlNode* child;
	TiXmlText* text;
	for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
	{
		if (child->Type()==TiXmlNode::ELEMENT)
		{
			if (!strcmp(child->Value(), "Init"))
			{
				text = child->FirstChild()->ToText();
				quest->setInit(text->Value());
				quest->init();
			}
			else if (!strcmp(child->Value(), "Description"))
			{
				text = child->FirstChild()->ToText();
				quest->setDescription(text->Value());
			}
			else if (!strcmp(child->Value(), "NPC"))
			{
				loadNPC(child);
			}
			else if (!strcmp(child->Value(), "Region"))
			{
				TriggerType type;
				std::string rname;
				Event* ev;
				TiXmlNode* child2;
				
				ElementAttrib attr;
				attr.parseElement(child->ToElement());
				attr.getString("name",rname);
				DEBUG5("event for region %s",rname.c_str());
				
				for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					if (child2->Type()==TiXmlNode::ELEMENT && !strcmp(child2->Value(), "Event"))
					{
						ev = new Event;
						loadEvent(child2, ev,type);
						World::getWorld()->addEvent(rname,type,ev);
					}
					else if (child2->Type()!=TiXmlNode::COMMENT)
					{
						DEBUG("unexpected element of <Region>: %s",child2->Value());
					}
				}
				
			}
			else if (child->Type()!=TiXmlNode::COMMENT)
			{
				DEBUG("unexpected element of <Quest>: %s",child->Value());
			}
		}
	}
}

