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
	}
}

//######################  RegionData (world.xml)  ##############################

//int ObjectLoader::generateFixedObjectData(TiXmlElement* pElement, string element, std::list<FixedObjectData*>* object_list, std::list<string>* subtype_list)
int WorldLoader::generateRegionData(TiXmlNode* pParent, TiXmlElement* pElement, std::string element, std::list<RegionData*> &region_list)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	
	if (element == "Event")
	{
		TriggerType type;
		Event* ev = new Event;
		loadEvent(pParent, ev,type);
		m_region_data->addEvent(type,ev);
	}
	
	ElementAttrib attr;
	
	if (element == "World")
	{
		DEBUG5("World");
		attr.parseElement(pElement);
		std::string startreg, startloc;
		attr.getString("start_region",startreg);
		attr.getString("start_location",startloc);
		
		World::getWorld()->getPlayerStartLocation() = std::make_pair(startreg,startloc);
		
	}
	
	if (element == "Region")
	{
		DEBUG5("Region");
		
		if (m_region_data == 0)
		{
			m_region_data = new RegionData;
			m_region_data->m_ground_material = "";
			m_region_data->m_region_template = "";
				
		}
		
		while (element == "Region" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "id") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_region_data->m_id = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "name"))
				m_region_data->m_name = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Settings")
	{
		DEBUG5("Settings");
		while (element == "Settings" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "dimx") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_region_data->m_dimx = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "dimy") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_region_data->m_dimy = static_cast<short>(ival);
			else if (!strcmp(pAttrib->Name(), "area_percent") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_region_data->m_area_percent = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "complexity") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_region_data->m_complexity = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "granularity") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_region_data->m_granularity = ival;
			else if (!strcmp(pAttrib->Name(), "ground"))
				m_region_data->m_ground_material = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "region_template"))
				m_region_data->m_region_template = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "ExitDirections")
	{
		DEBUG5("ExitDirections");
		while (element == "ExitDirections" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "north"))
			{
				if (!strcmp(pAttrib->Value(), "true"))
					m_region_data->m_exit_directions[NORTH] = true;
				else
					m_region_data->m_exit_directions[NORTH] = false;
			}
			else if (!strcmp(pAttrib->Name(), "south"))
			{
				if (!strcmp(pAttrib->Value(), "true"))
					m_region_data->m_exit_directions[SOUTH] = true;
				else
					m_region_data->m_exit_directions[SOUTH] = false;
			}
			else if (!strcmp(pAttrib->Name(), "west"))
			{
				if (!strcmp(pAttrib->Value(), "true"))
					m_region_data->m_exit_directions[WEST] = true;
				else
					m_region_data->m_exit_directions[WEST] = false;
			}
			else if (!strcmp(pAttrib->Name(), "east"))
			{
				if (!strcmp(pAttrib->Value(), "true"))
					m_region_data->m_exit_directions[EAST] = true;
				else
					m_region_data->m_exit_directions[EAST] = false;
			}

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Exits")
	{
		DEBUG5("Exits");
		while (element == "Exits" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Exit")
	{
		DEBUG5("Exit");
		while (element == "Exit" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "destination_location"))
				m_temp_exit.destination_location = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "destination_region"))
				m_temp_exit.destination_region = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "name"))
				m_temp_exit.exit_name = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Shape")
	{
		DEBUG5("Shape");
		while (element == "Shape" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "shape"))
			{
				if (!strcmp(pAttrib->Value(), "RECT"))
					m_temp_exit.type = Shape::RECT;
				else
					m_temp_exit.type = Shape::CIRCLE;
			}
			else if (!strcmp(pAttrib->Name(), "extent_x") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_exit.extent_x = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "extent_y") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_exit.extent_y = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "radius") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_exit.radius = static_cast<float>(dval);

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "NamedObjectGroups")
	{
		DEBUG5("NamedObjectGroups");
		while (element == "NamedObjectGroups" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "NameObjectGroup")
	{
		DEBUG5("NameObjectGroup");
		m_temp_name_object_group.init();
		while (element == "NameObjectGroup" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "object_group"))
				m_temp_name_object_group.group_name = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "name"))
				m_temp_name_object_group.name = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "priority") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_temp_name_object_group.prio = ival;

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "ObjectGroups")
	{
		DEBUG5("ObjectGroups");
		while (element == "ObjectGroups" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "ObjectGroup")
	{
		DEBUG5("ObjectGroup");
		m_temp_object_group.init();
		while (element == "ObjectGroup" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "object_group"))
				m_temp_object_group.group_name = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "priority") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_temp_object_group.prio = ival;
			else if (!strcmp(pAttrib->Name(), "number") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_temp_object_group.number = ival;
			else if (!strcmp(pAttrib->Name(), "probability") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_object_group.probability = static_cast<float>(dval);

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Environments")
	{
		DEBUG5("Environments");
		while (element == "Environments" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Environment")
	{
		DEBUG5("Environment");
		while (element == "Environment" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "height") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_environment.maxheight = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "name"))
				m_temp_environment.env = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "SpawnGroups")
	{
		DEBUG5("SpawnGroups");
		while (element == "SpawnGroups" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "SpawnGroup")
	{
		DEBUG5("SpawnGroup");
		m_spawn_group.init();
		while (element == "SpawnGroup" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "group_name"))
				m_spawn_group.group_name = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "number") && pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)
				m_spawn_group.number = ival;

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "ReviveLocation")
	{
		DEBUG5("ReviveLocation");
		while (element == "ReviveLocation" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "location"))
				m_temp_revive_location = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	return i;
}


//void ObjectLoader::searchFixedObjectData(TiXmlNode* pParent, std::list<FixedObjectData*>* object_list, std::list<string>* subtype_list)
void WorldLoader::searchRegionData(TiXmlNode* pParent, std::list<RegionData*> &region_list)
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
		num = generateRegionData(pParent, pParent->ToElement(), pParent->Value(), region_list);
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
	/*
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling() )
	{
		if (pChild->Type() == TiXmlNode::ELEMENT)
		{
			std::cout << "### " << pChild->Value() << std::endl;
			for ( pChild2 = pChild->FirstChild(); pChild2 != 0; pChild2= pChild2->NextSibling() )
			{
				if (pChild2->Type() == TiXmlNode::ELEMENT)
				{
					std::cout << "### " << pChild2->Value() << std::endl;
					if (pChild2->Value() == "Region")
					{
						RegionData* rdata = new RegionData;
						loadRegionData(rdata, pChild2);
					}
				}
			}
		}
	}*/
	
	/*if (element=="Region")
	{
	RegionData* rdata = new RegionData;
	loadRegionData(rdata,pElement);
	// hier dann id und Name einlesen
	}*/
	
	
	for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		searchRegionData(pChild, region_list);

		if ( !strcmp(pChild->Value(), "NameObjectGroup") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_region_data->addNamedObjectGroupTemplate(m_temp_name_object_group.group_name, m_temp_name_object_group.name, m_temp_name_object_group.prio);
			DEBUG5("NameObjectGroup loaded");
		}
		else if ( !strcmp(pChild->Value(), "ObjectGroup") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_region_data->addObjectGroupTemplate(m_temp_object_group.group_name, m_temp_object_group.prio, m_temp_object_group.number, m_temp_object_group.probability);
			DEBUG5("ObjectGroup loaded");
		}
		else if ( !strcmp(pChild->Value(), "Environment") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_region_data->addEnvironment(m_temp_environment.maxheight, m_temp_environment.env);
			DEBUG5("Environment loaded");
		}
		else if ( !strcmp(pChild->Value(), "SpawnGroup") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_region_data->addSpawnGroup(m_spawn_group.group_name, m_spawn_group.number);
			DEBUG5("SpawnGroups loaded");
		}
		else if ( !strcmp(pChild->Value(), "ReviveLocation") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_region_data->m_revive_location = m_temp_revive_location;
			DEBUG5("ReviveLocation loaded");
		}
		else if ( !strcmp(pChild->Value(), "Region") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			region_list.push_back(m_region_data);
			m_region_data = 0;
			DEBUG5("Region loaded");
		}
		else if ( !strcmp(pChild->Value(), "Exit") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			RegionExit temp_exit;
			
			temp_exit.m_destination_location = m_temp_exit.destination_location;
			temp_exit.m_destination_region = m_temp_exit.destination_region;
			temp_exit.m_exit_name = m_temp_exit.exit_name;
			temp_exit.m_shape.m_type = m_temp_exit.type;
			
			if (m_temp_exit.type == Shape::RECT)
				temp_exit.m_shape.m_extent = Vector(m_temp_exit.extent_x,m_temp_exit.extent_y);
			else
				temp_exit.m_shape.m_radius = m_temp_exit.radius;
			
			m_region_data->addExit(temp_exit);
			
			DEBUG5("Exit loaded");
		}
		/*else if ( !strcmp(pChild->Value(), "World") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			DEBUG5("World loaded");
		}*/
	}
}



bool WorldLoader::loadRegionData(const char* pFilename, std::list<RegionData*> &region_list)
{
	m_region_data = 0;
	
	/*object_list = new std::list<FixedObjectData*>;
	subtype_list = new std::list<std::string>;*/

	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		DEBUG5("Loading %s", pFilename);
		searchRegionData(&doc, region_list);
		DEBUG5("Loading %s finished", pFilename);
		//return m_object_list;
		return true;
	}
	else
	{
		DEBUG5("Failed to load file %s", pFilename);
		return false;
	}
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
		DEBUG("Failed to load file %s", pFilename);
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
			
			if (child->Type()==TiXmlNode::ELEMENT && !strcmp(child->Value(), "Trade"))
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
		DEBUG("Failed to load file %s", pFilename);
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
			
			if (!strcmp(child->Value(), "Description"))
			{
				text = child->FirstChild()->ToText();
				quest->setDescription(text->Value());
			}
			
			if (!strcmp(child->Value(), "NPC"))
			{
				loadNPC(child);
			}
			
			if (!strcmp(child->Value(), "Region"))
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
				}
				
			}
		}
	}
}

