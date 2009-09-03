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
#include "objectfactory.h"
#include "elementattrib.h"


bool TemplateLoader::loadObjectTemplateData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadObjectTemplate(&doc);
		return true;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return false;
	}
}

bool TemplateLoader::loadObjectTemplate(TiXmlNode* node)
{
	TiXmlNode* child, *child2;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "ObjectTemplate"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		ObjectTemplate* templ = new ObjectTemplate;
		attr.getString("type",templ->m_type);
		
		std::string name;
		attr.getString("name",name);
		
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			if (child->Type()==TiXmlNode::ELEMENT)
			{
				attr.parseElement(child->ToElement());
				std::string env,defstr,objname;
				if (!strcmp(child->Value(), "Environment"))
				{
					attr.getString("name",env);
					attr.getString("default",defstr,"false");
					
					if (defstr == "true")
					{
						templ->m_default_environment = env;
						DEBUG5("default environment for %s is %s",name.c_str(), env.c_str());
					}
					
					for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
					{
						if (child2->Type()==TiXmlNode::ELEMENT)
						{
							if (!strcmp(child2->Value(), "Object"))
							{
								attr.parseElement(child2->ToElement());
								attr.getString("name",objname);
								
								templ->addObject (env,objname);
								DEBUG5("added object %s to generic object %s for environment %s",objname.c_str(), name.c_str(), env.c_str());
								
							}
							else if (child2->Type()!=TiXmlNode::COMMENT)
							{
								DEBUG("unexpected element of <Environment>: %s",child->Value());
							}
						}
					}
				}
				else if (child->Type()!=TiXmlNode::COMMENT)
				{
					DEBUG("unexpected element of <ObjectTemplate>: %s",child->Value());
				}
				
			}
		}
		DEBUG5("found object template %s",name.c_str());
		
		ObjectFactory::registerObjectTemplate(name,templ);
	}
	else
	{
	// rekursiv durchmustern
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadObjectTemplate(child);
		}
	}

	return true;
}

bool TemplateLoader::loadObjectGroupTemplateData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadObjectGroupTemplate(&doc);
		return true;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return false;
	}
}


bool TemplateLoader::loadObjectGroupTemplate(TiXmlNode* node)
{
	TiXmlNode* child, *child2;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "ObjectGroupTemplate"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		std::string name;
		attr.getString("name",name);
		
		ObjectGroupTemplate* templ = new ObjectGroupTemplate;
		
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			if (child->Type()==TiXmlNode::ELEMENT)
			{
				attr.parseElement(child->ToElement());
				std::string env,defstr,objname;
				
				if (!strcmp(child->Value(), "Shape"))
				{
					std::string shape;
					attr.getString("type",shape,"CIRCLE");
					if (shape == "RECT")
					{
						templ->getShape()->m_type = Shape::RECT;
						attr.getFloat("extent_x",templ->getShape()->m_extent.m_x,0);
						attr.getFloat("extent_y",templ->getShape()->m_extent.m_y,0);
					}
					else
					{
						templ->getShape()->m_type = Shape::CIRCLE;
						attr.getFloat("radius",templ->getShape()->m_radius,0);
					}
				}
				else if (!strcmp(child->Value(), "WayPoint"))
				{
					Vector pos;
					attr.getFloat("pos_x",pos.m_x);
					attr.getFloat("pos_y",pos.m_y);
					templ->addWaypoint (pos);
					DEBUG5("waypoint in %s at %f %f",name.c_str(), pos.m_x,pos.m_y);
				}
				else if (!strcmp(child->Value(), "ObjectContent"))
				{
					for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
					{
						if (child2->Type()==TiXmlNode::ELEMENT)
						{
							if (!strcmp(child2->Value(), "Object"))
							{
								ObjectGroupTemplate::GroupObject obj;
								
								attr.parseElement(child2->ToElement());
								std::string prob_angle;
								
								attr.getFloat("center_x",obj.m_center.m_x);
								attr.getFloat("center_y",obj.m_center.m_y);
								attr.getString("subtype",obj.m_type);
								attr.getString("name",obj.m_name);
								attr.getFloat("angle",obj.m_angle,0.0);
								attr.getFloat("height",obj.m_height,0.0);
								attr.getFloat("probability",obj.m_probability,1.0);
								attr.getString("prob_angle",prob_angle);
								obj.m_prob_angle = (prob_angle == "true");
								
								obj.m_angle *= 3.14159 / 180.0;
								if (obj.m_height!=0)
								{
									DEBUG5("object %s height %f",name.c_str(), obj.m_height);
								}
								
								DEBUG5("object for %s: %s at %f %f angle %f prob %f",name.c_str(),obj.m_type.c_str(), obj.m_center.m_x, obj.m_center.m_y, obj.m_angle, obj.m_probability);
								
								templ->addObject (obj);
							}
							else if (child2->Type()!=TiXmlNode::COMMENT)
							{
								DEBUG("unexpected element of <ObjectContent>: %s",child->Value());
							}
							
						}
					}
				}
				else if (!strcmp(child->Value(), "Locations"))
				{
					for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
					{
						if (child2->Type()==TiXmlNode::ELEMENT)
						{
							if (!strcmp(child2->Value(), "Location"))
							{
								std::string lname;
								Vector pos;
								
								attr.parseElement(child2->ToElement());
								
								attr.getFloat("pos_x",pos.m_x);
								attr.getFloat("pos_y",pos.m_y);
								attr.getString("name",lname);
								
								templ->addLocation(lname,pos);
								DEBUG5("location for %s: %s at %f %f",name.c_str(),lname.c_str(), pos.m_x, pos.m_y);
							}
							else if (child2->Type()!=TiXmlNode::COMMENT)
							{
								DEBUG("unexpected element of <Locations>: %s",child->Value());
							}
						}
					}
				}
				else if (child->Type()!=TiXmlNode::COMMENT)
				{
					DEBUG("unexpected element of <ObjectGroupTemplate>: %s",child->Value());
				}
			}
		}
		
		ObjectFactory::registerObjectGroupTemplate(name,templ);
	}
	else
	{
		// rekursiv durchmustern
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadObjectGroupTemplate(child);
		}
	}

	return true;
}
