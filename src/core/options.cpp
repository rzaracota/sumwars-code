#include "options.h"

#include "sound.h"
#include "music.h"
#include "gettext.h"

#include "OISKeyboard.h"

#include "../tinyxml/tinyxml.h"
#include "elementattrib.h"

Options::Options()
{
	init();
}

void Options::init()
{
	// Fixed shortkeys
	m_shortkey_map[OIS::KC_ESCAPE] =  CLOSE_ALL;
	m_shortkey_map[OIS::KC_RETURN] =  SHOW_CHATBOX_NO_TOGGLE;
	m_shortkey_map[OIS::KC_TAB] =  SHOW_MINIMAP;
	m_shortkey_map[OIS::KC_F10] =  SHOW_OPTIONS;

	m_shortkey_map[OIS::KC_1] =  USE_POTION;
	m_shortkey_map[OIS::KC_2] =  (ShortkeyDestination) (USE_POTION+1);
	m_shortkey_map[OIS::KC_3] =  (ShortkeyDestination) (USE_POTION+2);
	m_shortkey_map[OIS::KC_4] =  (ShortkeyDestination) (USE_POTION+3);
	m_shortkey_map[OIS::KC_5] =  (ShortkeyDestination) (USE_POTION+4);
	m_shortkey_map[OIS::KC_6] =  (ShortkeyDestination) (USE_POTION+5);
	m_shortkey_map[OIS::KC_7] =  (ShortkeyDestination) (USE_POTION+6);
	m_shortkey_map[OIS::KC_8] =  (ShortkeyDestination) (USE_POTION+7);
	m_shortkey_map[OIS::KC_9] =  (ShortkeyDestination) (USE_POTION+8);
	m_shortkey_map[OIS::KC_0] =  (ShortkeyDestination) (USE_POTION+9);

	// special keys that may not be used as shortkeys
	m_special_keys.insert(OIS::KC_ESCAPE);
	m_special_keys.insert(OIS::KC_LSHIFT);
	m_special_keys.insert(OIS::KC_TAB);
	m_special_keys.insert(OIS::KC_RETURN);
	m_special_keys.insert(OIS::KC_BACK);
	m_special_keys.insert(OIS::KC_UP);
	m_special_keys.insert(OIS::KC_DOWN);
	m_special_keys.insert(OIS::KC_LEFT);
	m_special_keys.insert(OIS::KC_RIGHT);
	m_special_keys.insert(OIS::KC_CAPITAL);
	m_special_keys.insert(OIS::KC_0);
	m_special_keys.insert(OIS::KC_1);
	m_special_keys.insert(OIS::KC_2);
	m_special_keys.insert(OIS::KC_3);
	m_special_keys.insert(OIS::KC_4);
	m_special_keys.insert(OIS::KC_5);
	m_special_keys.insert(OIS::KC_6);
	m_special_keys.insert(OIS::KC_7);
	m_special_keys.insert(OIS::KC_8);
	m_special_keys.insert(OIS::KC_9);

	m_difficulty = NORMAL;
	m_text_speed = 1.0;
	m_enemy_highlight_color = "red";
}

Options* Options::getInstance()
{
	static Options m_instance;
	return &m_instance;
}

void Options::setToDefaultOptions()
{
	init();

	// shortkeys that may be changed
	m_shortkey_map[OIS::KC_I] = SHOW_INVENTORY ;
	m_shortkey_map[OIS::KC_C] =  SHOW_CHARINFO;
	m_shortkey_map[OIS::KC_T] =  SHOW_SKILLTREE;
	m_shortkey_map[OIS::KC_P] =  SHOW_PARTYMENU;
	m_shortkey_map[OIS::KC_M] =  SHOW_CHATBOX;
	m_shortkey_map[OIS::KC_Q] =  SHOW_QUESTINFO;
	m_shortkey_map[OIS::KC_W] =  SWAP_EQUIP;
	m_shortkey_map[OIS::KC_LMENU] =  SHOW_ITEMLABELS;


	setSoundVolume(1.0);
	setMusicVolume(1.0);

	setMaxNumberPlayers(8);
	setPort(5331);
	setServerHost("127.0.0.1");
}

bool Options::readFromFile(const std::string& filename)
{

	TiXmlDocument doc(filename.c_str());
	bool loadOkay = doc.LoadFile();

	ElementAttrib attr;

	if (loadOkay)
	{
		TiXmlNode* child;
		TiXmlNode* child2;
		TiXmlNode* root;

		root = doc.FirstChild();
		root = root->NextSibling();
		if (root->Type() == TiXmlNode::ELEMENT && !strcmp(root->Value(), "Options"))
		{
			for ( child = root->FirstChild(); child != 0; child = child->NextSibling())
			{
				if (child->Type()==TiXmlNode::ELEMENT)
				{
					attr.parseElement(child->ToElement());
					std::string env,defstr,objname;
					if (!strcmp(child->Value(), "Shortkeys"))
					{
						for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
						{
							if (child2->Type()==TiXmlNode::ELEMENT && !strcmp(child2->Value(), "Shortkey"))
							{
								attr.parseElement(child2->ToElement());

								int key, target;
								attr.getInt("key",key);
								attr.getInt("target",target);
								if (key != 0 && target != 0)
								{
									setShortkey(key, (ShortkeyDestination) target);
								}
							}
						}
					}
					else if (!strcmp(child->Value(), "Music"))
					{
						float volume;
						attr.getFloat("volume", volume, 1.0f);
						setMusicVolume(volume);
					}
					else if (!strcmp(child->Value(), "Sound"))
					{
						float volume;
						attr.getFloat("volume", volume, 1.0f);
						setSoundVolume(volume);
					}
					else if (!strcmp(child->Value(), "Language"))
					{
						std::string locale;
						attr.getString("locale",locale);
						setLocale(locale);
					}
					else if (!strcmp(child->Value(), "Gameplay"))
					{
						int diff;
						attr.getInt("difficulty",diff);
						setDifficulty( static_cast<Difficulty>(diff));
						float text_speed;
						attr.getFloat("text_speed",text_speed);
						setTextSpeed( text_speed);
					}
					else if (!strcmp(child->Value(), "Network"))
					{
						std::string host;
						int port;
						int max_players;
						attr.getString("host",host,"127.0.0.1");
						attr.getInt("port",port,5331);
						attr.getInt("max_players",max_players,8);

						setPort(port);
						setServerHost(host);
						setMaxNumberPlayers(max_players);
					}
					else if (!strcmp(child->Value(), "Graphic"))
					{
						std::string color = getEnemyHighlightColor();
						attr.getString("ehl_color", color);
						setEnemyHighlightColor(color);
					}
					else if (child->Type()!=TiXmlNode::COMMENT)
					{
						WARNING("unexpected element in options.xml: %s",child->Value());
					}
				}
			}
		}  // if root == <Options>
		else
		{
			setToDefaultOptions();
			return false;
		}
	}  // if (loadOkay)
	else
	{
		setToDefaultOptions();
		return false;
	}
	return true;
}

bool Options::writeToFile(const std::string& filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement* root;
	root = new TiXmlElement("Options");
	doc.LinkEndChild(root);

	TiXmlElement * element;
	element = new TiXmlElement( "Shortkeys" );
	root->LinkEndChild(element);

	TiXmlElement * subele;
	ShortkeyMap::iterator it;
	for (it = m_shortkey_map.begin(); it != m_shortkey_map.end(); ++it)
	{
		subele = new TiXmlElement("Shortkey");
		subele->SetAttribute("key",it->first);
		subele->SetAttribute("target",it->second);
		element->LinkEndChild(subele);
	}

	element = new TiXmlElement( "Gameplay" );
	root->LinkEndChild(element);
	element->SetAttribute("difficulty",getDifficulty());
	element->SetDoubleAttribute("text_speed",getTextSpeed());

	element = new TiXmlElement( "Music" );
	root->LinkEndChild(element);
	element->SetDoubleAttribute("volume",getMusicVolume());

	element = new TiXmlElement( "Sound" );
	element->SetDoubleAttribute("volume",getSoundVolume());
	root->LinkEndChild(element);

	element = new TiXmlElement("Graphic");
	root->LinkEndChild(element);
	element->SetAttribute("ehl_color", getEnemyHighlightColor().c_str());

	element = new TiXmlElement( "Language" );
	root->LinkEndChild(element);
	element->SetAttribute("locale",getLocale().c_str());

	element = new TiXmlElement( "Network" );
	root->LinkEndChild(element);
	element->SetAttribute("host",getServerHost().c_str());
	element->SetAttribute("port",getPort());
	element->SetAttribute("max_players",getMaxNumberPlayers());

	doc.SaveFile( filename.c_str());
	return true;
}

KeyCode Options::getMappedKey(ShortkeyDestination sd)
{
	std::map<KeyCode, ShortkeyDestination>::iterator it;
	for (it=m_shortkey_map.begin(); it!= m_shortkey_map.end();++it)
	{
		if (it->second == sd)
		{
			return it->first;
		}
	}

	return 0;
}

ShortkeyDestination Options::getMappedDestination(KeyCode key)
{
	std::map<KeyCode, ShortkeyDestination>::iterator it;
	it = m_shortkey_map.find(key);

	if (it != m_shortkey_map.end())
		return it->second;

	return NO_KEY;
}

bool Options:: setShortkey(KeyCode key,ShortkeyDestination dest)
{
	if (isSpecialKey(key))
		return false;

	// key that was mapped to the action dest so far
	KeyCode oldkey = getMappedKey(dest);
	// delete mapping
	if (oldkey != 0)
	{
		m_shortkey_map.erase(oldkey);
	}

	// create new mapping
	m_shortkey_map[key]=dest;
	return true;
}

void Options::setSoundVolume(float vol)
{
	SoundSystem::setSoundVolume(vol);
}

float Options::getSoundVolume()
{
	return SoundSystem::getSoundVolume();
}

void Options::setMusicVolume(float vol)
{
	MusicManager::instance().setMusicVolume(vol);
}

float Options::getMusicVolume()
{
	return MusicManager::instance().getMusicVolume();
}

std::string Options::getLocale()
{
	const char* locale = Gettext::getLocale();

	std::string locstr = "";
	if (locale != 0)
	{
		locstr = locale;
	}
	if (locstr == "")
		locstr = "#default#";

	return locstr;
}

void Options::setLocale(const std::string& locale)
{
	if (locale!= "#default#" && locale != "")
	{
		Gettext::setLocale(locale.c_str());
	}
}

