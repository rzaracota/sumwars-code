#include "templateloader.h"

/*
#include "warrior.h"
#include "mage.h"
#include "archer.h"
#include "priest.h"
#include "fixedobject.h"
#include "monster.h"
*/

//#include <iostream>

//######################  RegionData (world.xml)  ##############################

//int ObjectLoader::generateFixedObjectData(TiXmlElement* pElement, string element, std::list<FixedObjectData*>* object_list, std::list<string>* subtype_list)
int TemplateLoader::generateRegionData(TiXmlElement* pElement, std::string element, std::list<RegionData*> &region_list)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	
	if (element == "World")
	{
		DEBUG5("World");
		/*if (m_object_data == 0)
		{
			m_object_data = new FixedObjectData;
		}*/
		
		while (element == "World" && pAttrib)
		{
			/*if (!strcmp(pAttrib->Name(), "subtype"))
				m_subtype = pAttrib->Value();*/

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Region")
	{
		DEBUG5("Region");
		
		if (m_region_data == 0)
		{
			m_region_data = new RegionData;
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
			if (!strcmp(pAttrib->Name(), "region"))
				m_temp_revive_location.region = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "location"))
				m_temp_revive_location.location = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	return i;
}


//void ObjectLoader::searchFixedObjectData(TiXmlNode* pParent, std::list<FixedObjectData*>* object_list, std::list<string>* subtype_list)
void TemplateLoader::searchRegionData(TiXmlNode* pParent, std::list<RegionData*> &region_list)
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
		num = generateRegionData(pParent->ToElement(), pParent->Value(), region_list);
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
		else if ( !strcmp(pChild->Value(), "SpawnGroups") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_region_data->addSpawnGroup(m_spawn_group.group_name, m_spawn_group.number);
			DEBUG5("SpawnGroups loaded");
		}
		else if ( !strcmp(pChild->Value(), "ReviveLocation") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_region_data->m_revive_location.first = m_temp_revive_location.region;
			m_region_data->m_revive_location.second = m_temp_revive_location.location;
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


//bool ObjectLoader::loadFixedObjectData(const char* pFilename, std::list<FixedObjectData*>* &object_list, std::list<string>* &subtype_list)
bool TemplateLoader::loadRegionData(const char* pFilename, std::list<RegionData*> &region_list)
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


//#####################  ObjectTemplate (obj_templates)  #######################

int TemplateLoader::generateObjectTemplate(TiXmlElement* pElement, std::string element, std::list<ObjectTemplate*> &object_template_list, std::list<std::string> &name_list)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	
	if (element == "ObjectTemplate")
	{
		DEBUG5("ObjectTemplate");
		
		if (m_object_template == 0)
		{
			m_object_template = new ObjectTemplate;
		}
		
		while (element == "ObjectTemplate" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "name"))
				name_list.push_back(pAttrib->Value());
			else if (!strcmp(pAttrib->Name(), "type"))
			{
				if (!strcmp(pAttrib->Value(), "NONE"))
					m_object_template->m_type = WorldObject::TypeInfo::TYPE_NONE;
				else if (!strcmp(pAttrib->Value(), "PLAYER"))
					m_object_template->m_type = WorldObject::TypeInfo::TYPE_PLAYER;
				else if (!strcmp(pAttrib->Value(), "MONSTER"))
					m_object_template->m_type = WorldObject::TypeInfo::TYPE_MONSTER;
				else if (!strcmp(pAttrib->Value(), "TRADER"))
					m_object_template->m_type = WorldObject::TypeInfo::TYPE_TRADER;
				else if (!strcmp(pAttrib->Value(), "FIXED_OBJECT"))
					m_object_template->m_type = WorldObject::TypeInfo::TYPE_FIXED_OBJECT;
			}

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Environment")
	{
		DEBUG5("Environment");
		while (element == "Environment" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "name"))
				m_current_environment_name = pAttrib->Value();

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Object")
	{
		DEBUG5("Object");
		while (element == "Object" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "name"))
				m_object_template->addObject(m_current_environment_name,pAttrib->Value());

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	return i;
}


void TemplateLoader::searchObjectTemplate(TiXmlNode* pParent, std::list<ObjectTemplate*> &object_template_list, std::list<std::string> &name_list)
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
		num = generateObjectTemplate(pParent->ToElement(), pParent->Value(), object_template_list, name_list);
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
		searchObjectTemplate(pChild, object_template_list, name_list);

		if ( !strcmp(pChild->Value(), "ObjectTemplate") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			object_template_list.push_back(m_object_template);
			m_object_template = 0;
			DEBUG5("ObjectTemplate loaded");
		}
	}
}


bool TemplateLoader::loadObjectTemplate(const char* pFilename, std::list<ObjectTemplate*> &object_template_list, std::list<std::string> &name_list)
{
	m_object_template = 0;
	
	/*object_list = new std::list<FixedObjectData*>;
	subtype_list = new std::list<std::string>;*/

	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		DEBUG5("Loading %s", pFilename);
		searchObjectTemplate(&doc, object_template_list, name_list);
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


//####################  ObjectGroupTemplate (og_template)  #####################

int TemplateLoader::generateObjectGroupTemplate(TiXmlElement* pElement, std::string element, std::list<ObjectGroupTemplate*> &object_group_template_list, std::list<std::string> &name_list)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	double dval;
	
	if (element == "ObjectGroupTemplate")
	{
		DEBUG5("ObjectGroupTemplate");
		
		if (m_object_group_template == 0)
		{
			m_object_group_template = new ObjectGroupTemplate;
		}
		
		while (element == "ObjectGroupTemplate" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "name"))
				name_list.push_back(pAttrib->Value());

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Shape")
	{
		DEBUG5("Shape");
		while (element == "Shape" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "type"))
			{
				if (!strcmp(pAttrib->Value(), "RECT"))
					m_temp_shape.type = Shape::RECT;
				else
					m_temp_shape.type = Shape::CIRCLE;
			}
			else if (!strcmp(pAttrib->Name(), "angle") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_shape.angle = (static_cast<float>(dval))*PI/180;
			else if (!strcmp(pAttrib->Name(), "extent_x") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_shape.extent_x = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "extent_y") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_shape.extent_y = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "radius") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_shape.radius = static_cast<float>(dval);

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Waypoint")
	{
		DEBUG5("Waypoint");
		while (element == "Waypoint" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "pos_x") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_waypoint.pos_x = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "pos_y") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_waypoint.pos_y = static_cast<float>(dval);

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "ObjectContent")
	{
		DEBUG5("ObjectContent");
		while (element == "ObjectContent" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Object")
	{
		DEBUG5("Object");
		while (element == "Object" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "center_x") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_group_object.center_x = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "center_y") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_group_object.center_y = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "angle") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_group_object.angle = (static_cast<float>(dval))*PI/180;
			else if (!strcmp(pAttrib->Name(), "subtype"))
				m_temp_group_object.type = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "probability") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_group_object.probability = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "prob_angle"))
			{
				if (!strcmp(pAttrib->Value(), "true"))
					m_temp_group_object.prob_angle = true;
				else
					m_temp_group_object.prob_angle = false;
			}

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Location")
	{
		DEBUG5("Location");
		while (element == "Location" && pAttrib)
		{
			if (!strcmp(pAttrib->Name(), "name"))
				m_temp_location.name = pAttrib->Value();
			else if (!strcmp(pAttrib->Name(), "pos_x") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_location.pos_x = static_cast<float>(dval);
			else if (!strcmp(pAttrib->Name(), "pos_y") && pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS)
				m_temp_location.pos_y = static_cast<float>(dval);

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	return i;
}


void TemplateLoader::searchObjectGroupTemplate(TiXmlNode* pParent, std::list<ObjectGroupTemplate*> &object_group_template_list, std::list<std::string> &name_list)
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
		num = generateObjectGroupTemplate(pParent->ToElement(), pParent->Value(), object_group_template_list, name_list);
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
		searchObjectGroupTemplate(pChild, object_group_template_list, name_list);

		if ( !strcmp(pChild->Value(), "Shape") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_object_group_template->getShape()->m_type = m_temp_shape.type;
			m_object_group_template->getShape()->m_angle = m_temp_shape.angle;

			if (m_temp_shape.type == Shape::RECT)
				m_object_group_template->getShape()->m_extent = Vector(m_temp_shape.extent_x,m_temp_shape.extent_y);
			else
				m_object_group_template->getShape()->m_radius = m_temp_shape.radius;

			DEBUG5("Shape loaded");
		}
		else if ( !strcmp(pChild->Value(), "Waypoint") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_object_group_template->addWaypoint(Vector(m_temp_waypoint.pos_x,m_temp_waypoint.pos_y));
			DEBUG5("Waypoint loaded");
		}
		else if ( !strcmp(pChild->Value(), "Object") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			ObjectGroupTemplate::GroupObject temp_group_object;
			
			temp_group_object.m_angle = m_temp_group_object.angle;
			temp_group_object.m_center = Vector(m_temp_group_object.center_x, m_temp_group_object.center_y);
			temp_group_object.m_prob_angle = m_temp_group_object.prob_angle;
			temp_group_object.m_probability = m_temp_group_object.probability;
			temp_group_object.m_type = m_temp_group_object.type;

			m_object_group_template->addObject(temp_group_object);

			DEBUG5("Object loaded");
		}
		else if ( !strcmp(pChild->Value(), "Location") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			m_object_group_template->addLocation(m_temp_location.name, Vector(m_temp_location.pos_x,m_temp_location.pos_y));
			DEBUG5("Location loaded");
		}
		else if ( !strcmp(pChild->Value(), "ObjectGroupTemplate") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			object_group_template_list.push_back(m_object_group_template);
			m_object_group_template = 0;
			DEBUG5("ObjectGroupTemplate loaded");
		}
	}
}


bool TemplateLoader::loadObjectGroupTemplate(const char* pFilename, std::list<ObjectGroupTemplate*> &object_group_template_list, std::list<std::string> &name_list)
{
	m_object_group_template = 0;
	
	/*object_list = new std::list<FixedObjectData*>;
	subtype_list = new std::list<std::string>;*/

	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		DEBUG5("Loading %s", pFilename);
		searchObjectGroupTemplate(&doc, object_group_template_list, name_list);
		DEBUG5("Loading %s finished", pFilename);
		return true;
	}
	else
	{
		DEBUG5("Failed to load file %s", pFilename);
		return false;
	}
}

