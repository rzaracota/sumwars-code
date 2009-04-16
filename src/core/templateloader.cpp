#include "templateloader.h"

/*
#include "warrior.h"
#include "mage.h"
#include "archer.h"
#include "priest.h"
#include "fixedobject.h"
#include "monster.h"
*/

#include <iostream>


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
				m_object_template->m_type = pAttrib->Value();
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
		m_temp_group_object.prob_angle = true;
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

