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

//int ObjectLoader::generateFixedObjectData(TiXmlElement* pElement, string element, std::list<FixedObjectData*>* object_list, std::list<string>* subtype_list)
int TemplateLoader::generateRegionData(TiXmlElement* pElement, std::string element, std::list<RegionData*>* &region_list)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	double dval;
/*
World
Region
Settings
ExitDirections
Exits
Exit
Shape
NamedObjectGroups
NameObjectGroup
NameObjectGroup
NameObjectGroup
ObjectGroups
ObjectGroup
ObjectGroup
ObjectGroup
Environments
Environment
Environment
SpawnGroups
SpawnGroup
SpawnGroup
SpawnGroup
*/
	std::cout << element << std::endl;

	if (element == "World")
	{
		DEBUG("World");
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
		DEBUG("Region");
		while (element == "Region" && pAttrib)
		{
			/*if (!strcmp(pAttrib->Name(), "layer"))
			{
				if (!strcmp(pAttrib->Value(), "NORMAL"))
					m_object_data->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
				else if (!strcmp(pAttrib->Value(), "GROUND"))
					m_object_data->m_layer = WorldObject::LAYER_BASE;
			}*/

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Settings")
	{
		DEBUG("Settings");
		while (element == "Settings" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "ExitDirections")
	{
		DEBUG("ExitDirections");
		while (element == "ExitDirections" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Exits")
	{
		DEBUG("Exits");
		while (element == "Exits" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Exit")
	{
		DEBUG("Exit");
		while (element == "Exit" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Shape")
	{
		DEBUG("Shape");
		while (element == "Shape" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "NamedObjectGroups")
	{
		DEBUG("NamedObjectGroups");
		while (element == "NamedObjectGroups" && pAttrib)
		{
			/*if (!strcmp(pAttrib->Name(), "shape"))
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
				m_radius = static_cast<float>(dval);*/

			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "NameObjectGroup")
	{
		DEBUG("NameObjectGroup");
		while (element == "NameObjectGroup" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "ObjectGroups")
	{
		DEBUG("ObjectGroups");
		while (element == "ObjectGroups" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "ObjectGroup")
	{
		DEBUG("ObjectGroup");
		while (element == "ObjectGroup" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Environments")
	{
		DEBUG("Environments");
		while (element == "Environments" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "Environment")
	{
		DEBUG("Environment");
		while (element == "Environment" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "SpawnGroups")
	{
		DEBUG("SpawnGroups");
		while (element == "SpawnGroups" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	if (element == "SpawnGroup")
	{
		DEBUG("SpawnGroup");
		while (element == "SpawnGroup" && pAttrib)
		{
			i++;
			pAttrib=pAttrib->Next();
		}
	}
	
	return i;
}


//void ObjectLoader::searchFixedObjectData(TiXmlNode* pParent, std::list<FixedObjectData*>* object_list, std::list<string>* subtype_list)
void TemplateLoader::searchRegionData(TiXmlNode* pParent, std::list<RegionData*>* &region_list)
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

		if ( !strcmp(pChild->Value(), "World") && pChild->Type() == TiXmlNode::ELEMENT)
		{
			/*if (m_object_data->m_shape.m_type == Shape::RECT)
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
			m_object_data = 0;*/
			DEBUG("World loaded");
		}
	}
}


//bool ObjectLoader::loadFixedObjectData(const char* pFilename, std::list<FixedObjectData*>* &object_list, std::list<string>* &subtype_list)
bool TemplateLoader::loadRegionData(const char* pFilename, std::list<RegionData*>* &region_list)
{
	/*m_object_data = 0;
	
	object_list = new std::list<FixedObjectData*>;
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
		DEBUG("Failed to load file %s", pFilename);
		return false;
	}
}

