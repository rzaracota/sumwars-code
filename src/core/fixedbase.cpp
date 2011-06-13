#include "fixedbase.h"
#include "worldobject.h"


void FixedObjectData::writeToXML(TiXmlNode* node)
{
	TiXmlElement* elem = node->ToElement();
	if (elem == 0) 
		return;
	
	if (m_layer == (WorldObject::LAYER_BASE | WorldObject::LAYER_AIR))
	{
		elem->SetAttribute("layer","NORMAL");
	}
	else if (m_layer ==  WorldObject::LAYER_BASE)
	{
		elem->SetAttribute("layer","BASE");
	}
	else if (m_layer ==  WorldObject::LAYER_AIR)
	{
		elem->SetAttribute("layer","AIR");
	}
	else if (m_layer ==  WorldObject::LAYER_NOCOLLISION)
	{
		elem->SetAttribute("layer","NOCOLLISION");
	}
	
	TiXmlElement* geom_node =	elem->FirstChildElement("Geometry");
	if (geom_node == 0)
	{
		geom_node = new TiXmlElement("Geometry");
		elem->LinkEndChild(geom_node);
	}
	
	if (m_shape.m_type == Shape::CIRCLE)
	{
		geom_node->SetAttribute("type","CIRCLE");
		geom_node->SetDoubleAttribute("radius",m_shape.m_radius);
		geom_node->RemoveAttribute("extent_x");
		geom_node->RemoveAttribute("extent_y");
	}
	else
	{
		geom_node->SetAttribute("type","RECT");
		geom_node->SetDoubleAttribute("extent_x",m_shape.m_extent.m_x);
		geom_node->SetDoubleAttribute("extent_y",m_shape.m_extent.m_y);
		geom_node->RemoveAttribute("radius");	
	}
}