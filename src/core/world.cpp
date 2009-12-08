/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "world.h"
#include "player.h"
#include "mapgenerator.h"
#include "spawnpoint.h"
#include "eventsystem.h"
#include "dialogue.h"

#include "itemfactory.h"
#include "objectfactory.h"
#include "networkstruct.h"
#include "worldloader.h"
#include "templateloader.h"
#include "objectloader.h"

#include "OgreResourceGroupManager.h"

World* World::m_world=0;
int World::m_version = 13;

void  World::createWorld(bool server, int port, bool cooperative, int max_players)
{
	if (m_world != 0)
		delete m_world;
	
	m_world = new World(server, cooperative, max_players);
	m_world->init(port);
}

/**
 * Constructors/Destructors
 */
/**
 *
 */
 World::World(bool server, bool cooperative, int max_players)
{
	m_server = server;
	m_cooperative = cooperative;

	// diverse Initialisierungen

	m_player_slots = new WorldObjectMap;
	m_players = new WorldObjectMap;;


	if (server)
	{
		m_max_nr_players =max_players;
	}
	else
	{
		m_max_nr_players =8;
	}
	m_parties.resize(m_max_nr_players);
	for (int i =0;i<m_max_nr_players;i++)
	{
		m_parties[i].init(i);
	}
	
	m_local_player =0;

	m_events = new NetEventList;

}


bool World::init(int port)
{
	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	std::string file;
	
	EventSystem::init();
	Dialogue::init();
	
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("lua","*.lua");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;
		EventSystem::doFile(file.c_str());
	}
	
	// Aktionen initialisieren
	Action::init();
	
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("abilities","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;

		Action::loadAbilityData(file.c_str());

	}
	
	ObjectFactory::init();
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("monsters","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;

		ObjectLoader::loadMonsterData(file.c_str());

	}
	
	// TODO: funktioniert nur, solange kein LUA Code enthalten
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("projectiles","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;

		ObjectLoader::loadProjectileData(file.c_str());
	}

	// feste Objekte Initialisieren
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("objects","*.xml");
	for (it = files->begin(); it != files->end(); ++it)
	{
		file = it->archive->getName();
		file += "/";
		file += it->filename;

		ObjectLoader::loadObjectData(file.c_str());

	}

	if (m_server)
	{
		WorldLoader worldloader;
		std::list<RegionData*> region_list;
		files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("world","*.xml");
		
		for (it = files->begin(); it != files->end(); ++it)
		{
			file = it->archive->getName();
			file += "/";
			file += it->filename;
			//worldloader.loadRegionData(file.c_str(), region_list);
			worldloader.loadRegionData(file.c_str());
		}

		/*
		std::list<RegionData*>::iterator rt;
		for (rt = region_list.begin(); rt != region_list.end();rt++)
		{
			registerRegionData(*rt, (*rt)->m_id);
		}
		*/
		
		files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("npc","*.xml");
		for (it = files->begin(); it != files->end(); ++it)
		{
			file = it->archive->getName();
			file += "/";
			file += it->filename;
			worldloader.loadNPCData(file.c_str());
		}
		
		files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("quests","*.xml");
		for (it = files->begin(); it != files->end(); ++it)
		{
			file = it->archive->getName();
			file += "/";
			file += it->filename;
			worldloader.loadQuestsData(file.c_str());
		}
		
		
		
		DEBUG("server");
		m_network = new ServerNetwork(m_max_nr_players);
		
		

	}
	else
	{
		DEBUG("client");
		m_network = new ClientNetwork();
	}

	if( m_network->init( port )!=NET_OK )
	{
		ERRORMSG( "Error occured in network" );
		return false;
	}

	m_timer[0] =0;
	m_timer[1] =0;
	m_timer[2] =0;
	
	// Regionen aus XML Laden
	

	return true;
}


bool World::createRegion(short region)
{


	DEBUG5("creating region %i",region);
	int type = 1;
	if (type==1)
	{
		RegionData* rdata;
		std::map<int, RegionData*>::iterator it;
		it = m_region_data.find(region);
		if (it == m_region_data.end())
		{
			ERRORMSG("no data for region %i",region);
		}
		rdata = it->second;

		srand(1000);
		//srand((unsigned int)time((time_t *)NULL));
		Region* reg = MapGenerator::createRegion(rdata);
		DEBUG5("region created %p for id %i",reg,region);

		if (reg == 0)
		{
			return false;
		}
		
		insertRegion(reg,region);
		return true;
	}
	else if(type==2)
	{
		Region* reg = new Region(25,25,region,"region0");
		insertRegion(reg,region);


		// Umgebungen erstellen
		reg->insertEnvironment(0.6,"meadow");
		reg->insertEnvironment(1.0,"hills");

		reg->addLocation("entry_south",Vector(10,10));
		// Objekte anlegen


		reg->createObject( "fence1", Vector(4,2));
		reg->createObject("fence2", Vector(12,2));
		reg->createObject( "fence3", Vector(20,2));
		reg->createObject("fence4", Vector(28,2));
		reg->createObject("fence5", Vector(3,22));

		reg->createObject("smallWall2", Vector(7.5,22));
		reg->createObject("wall1", Vector(11,22));
		reg->createObject("wall2", Vector(17,22));
		reg->createObject("wall3", Vector(23,22));
		reg->createObject( "smallWall1", Vector(26.5,22));

		reg->createObject( "tree1", Vector(9,13));
		reg->createObject("tree2", Vector(10,2));
		reg->createObject( "tree3", Vector(1,2));

		reg->createObject("$tree", Vector(1,8));

	}
	return true;
}


void World::acceptLogins()
{
	int login_slot;

	while((login_slot=static_cast<ServerNetwork*>(m_network)->popNewLoginSlot()) != -1 )
	{
		INFO( "user login detected: %i",  login_slot  );
		m_logins.push_back(login_slot);


	}
}


void World::updateLogins()
{
	std::list<int>::iterator i;
	Packet* data;
	PackageHeader header;
	DEBUG5("update logins");
	for (i=m_logins.begin();i!=m_logins.end();)
	{
		DEBUG5("testing slot %i",(*i));
		if (m_network->numberSlotMessages((*i) )>0)
		{
			DEBUG5("got Package");
			m_network->popSlotMessage(data,(*i));
			CharConv cv(data);
			header.fromString(&cv);
			if (header.m_content == PTYPE_C2S_SAVEGAME)
			{
				DEBUG5("got savegame from slot %i",(*i));
				handleSavegame(&cv,*i);
				i = m_logins.erase(i);

			}
			else
			{
				DEBUG("unknown type %i",header.m_content);
			}
			m_network->deallocatePacket(data);
		}
		else
		{
			i++;
		}

	}
	DEBUG5("done");

}

void World::deleteWorld()
{
	delete m_world;
	m_world=0;
}

World::~World()
{
	
	delete m_network;

	
	std::map<int, RegionData*>::iterator it;
 	
	for (it = m_region_data.begin(); it != m_region_data.end(); ++it)
	{
		delete it->second;
	}
	
	std::map<int,Region*>::iterator rit;
	for (rit = m_regions.begin(); rit != m_regions.end(); rit++)
	{
		delete rit->second;
	}
	
	std::map<std::string, Quest*>::iterator qit;
	for (qit = m_quests.begin(); qit != m_quests.end(); ++qit)
	{
		delete qit->second;
	}


	//delete[] m_parties;
	delete m_player_slots;
	delete m_players;
	delete m_events;
	
	ObjectFactory::cleanupObjectData();
	Action::cleanup();
	Dialogue::cleanup();
	EventSystem::cleanup();
}

Region* World::getRegion(int rid)
{
	std::map<int,Region*>::iterator it;
	it = m_regions.find(rid);

	if (it != m_regions.end())
	{
		return it->second;
	}
	return 0;
}

int World::getRegionId(std::string name)
{
	std::map<std::string,int>::iterator it;
	it = m_region_name_id.find(name);

	if (it != m_region_name_id.end())
	{
		return it->second;
	}
	return -1;
}

void World::insertRegion(Region* region, int rnr)
{
	DEBUG5("inserting region %i %s %p",rnr, region->getName().c_str(),region);
	m_regions.insert(std::make_pair(rnr,region));

}

Fraction::Relation World::getRelation(Fraction::Id frac, Fraction::Id frac2)
{
	if (frac > frac2)
		return getRelation(frac2,frac);
	
	// Sonderfaelle
	if (frac == Fraction::HOSTILE_TO_ALL || frac2 == Fraction::HOSTILE_TO_ALL)
		return Fraction::HOSTILE;
	
	if (frac == Fraction::NEUTRAL_TO_ALL || frac2 == Fraction::NEUTRAL_TO_ALL)
		return Fraction::NEUTRAL;
	
	if (frac == frac2)
		return Fraction::ALLIED;
	
	if (frac == Fraction::NOFRACTION)
		return Fraction::NEUTRAL;
	
	if (frac>=  Fraction::MONSTER && frac2>=  Fraction::MONSTER)
	{
		// Beziehung Nicht-Spieler - Nicht-Spieler
		Fraction* fraction = getFraction(frac);
		return fraction->getRelation(frac2);
	}
	else if (frac<  Fraction::MONSTER)
	{
	
		// Beziehung zwischen Spieler und Nicht-Spieler Partei
		Fraction* fraction = getFraction(frac2);
		if (fraction ==0)
			return Fraction::NEUTRAL;
		
		return fraction->getRelation(Fraction::PLAYER);
	}
	else
	{
		// Beziehung zwischen Spielern
		return std::min(m_parties[frac].getRelations()[frac2], m_parties[frac2].getRelations()[frac]);
	}
	
	
}

Fraction::Relation World::getRelation(Fraction::Id frac, WorldObject* wo)
{
	Fraction::Id f = wo->getFraction();
	return getRelation(frac,f);
}

Fraction::Relation World::getRelation(Fraction::Type fractionname1, Fraction::Type fractionname2)
{
	Fraction::Id id1 = getFractionId(fractionname1);
	Fraction::Id id2 = getFractionId(fractionname2);
	
	if (id1 == Fraction::NOFRACTION)
	{
		ERRORMSG("Fraction %s does not exist",fractionname1.c_str());
		return Fraction::NEUTRAL;
	}
	
	if (id2 == Fraction::NOFRACTION)
	{
		ERRORMSG("Fraction %s does not exist",fractionname2.c_str());
		return Fraction::NEUTRAL;
	}
	
	return getRelation(id1,id2);
}

void World::setRelation(Fraction::Id frac, Fraction::Id frac2, Fraction::Relation relation)
{
	Fraction::Id id1 = frac, id2 = frac2;
	
	// id1 muss die kleinere ID sind, aber keine der PseudoIds fuer Player, Hostile, Neutral
	if (id1 > id2)
		std::swap(id1,id2);
	
	if (id1 < Fraction::MONSTER)
	{
		if (id2 >= Fraction::MONSTER)
		{
			std::swap(id1,id2);
		}
		else
		{
			// Spieler - Spieler relation kann nicht mit dieser Funktion gesetzt werden
			return;
		}
	}
	
	Fraction * fraction = getFraction(id1);
	if (fraction !=0 )
	{
		fraction->setRelation(id2,relation);
		
		NetEvent event;
		event.m_id = id1;
		event.m_data = id2;
		event.m_type = NetEvent::FRACTION_RELATION_CHANGED;
		
		insertNetEvent(event);
	}
	else
	{
		ERRORMSG("Fraction %i does not exist",id1);
	}
}

void World::setRelation(Fraction::Type fractionname1, Fraction::Type fractionname2, Fraction::Relation relation)
{
	Fraction::Id id1 = getFractionId(fractionname1);
	Fraction::Id id2 = getFractionId(fractionname2);
	
	if (id1 == Fraction::NOFRACTION)
	{
		ERRORMSG("Fraction %s does not exist",fractionname1.c_str());
		return;
	}
	
	if (id2 == Fraction::NOFRACTION)
	{
		ERRORMSG("Fraction %s does not exist",fractionname2.c_str());
		return;
	}
	
	
	setRelation(id1,id2,relation);
	
}


Party* World::getEmptyParty()
{
	int i;
	for (i=0;i<m_max_nr_players;i++)
	{
		if (m_parties[i].getNrMembers()==0)
			return &(m_parties[i]);
	}
	return 0;
}





bool World::insertPlayer(WorldObject* player, int slot)
{
	if (slot != NOSLOT)
	{
		m_player_slots->insert(std::make_pair(slot,player));
	}
	m_players->insert(std::make_pair(player->getId(),player));

	// Spieler in Party einfuegen
	if (m_server)
	{
		Party* p;
		if (World::getWorld()->isCooperative())
		{
			// Spieler zur ersten (einzigen) Party hinzufuegen
			p = World::getWorld()->getParty(0);
		}
		else
		{
			p = World::getWorld()->getEmptyParty();
			if (p==0)
			{
				ERRORMSG("cant open new party");
			}
			p->clear();
			DEBUG("opened Party %i",p->getId());
		}
		p->addMember(player->getId());
	}

	return true;
}


bool World::insertPlayerIntoRegion(WorldObject* player, short region, LocationName loc)
{
	DEBUG5("try to enter region %i",region); 
	Region* reg = getRegion(region);

	// Testen ob alle Daten vorhanden sind
	int data_missing =0;
	if (reg ==0)
	{
		data_missing =1;
	}

	if (player->getRegionId() != region)
	{
		DEBUG5("changing region %i %i",player->getRegionId(),region);
		player->setRegionId(region);
		player->setState(WorldObject::STATE_INACTIVE,false);
	}
	
	if (player->getState() != WorldObject::STATE_ENTER_REGION)
	{
		// Spieler kann noch nicht in die Region eingefuegt werden

		if (m_server)
		{
			// Ort zwischenspeichern
			m_region_enter_loc[player->getId()] = loc;

			// Wenn die Region noch nicht existiert: erzeugen
			if (data_missing !=0)
			{
				
				if (m_region_data.count(region) == 0)
				{
					// fuer die Region gibt es keine Daten
					// zurueck zum allerersten Startpunkt
					if (region == getRegionId(m_player_start_location.first) && loc ==m_player_start_location.second)
						return false;
					
					static_cast<Player*>(player)->setRevivePosition(m_player_start_location);
					bool succ = insertPlayerIntoRegion(player, getRegionId(m_player_start_location.first), m_player_start_location.second);
					
					return succ;
				}
				bool succ = createRegion(region);
				if (succ == false)
				{
					static_cast<Player*>(player)->setRevivePosition(m_player_start_location);
					bool succ = insertPlayerIntoRegion(player, getRegionId(m_player_start_location.first), m_player_start_location.second);
					return succ;
				}
			}

			// ein lokaler Spieler kann jetzt in die erzeugte region
			// bei einem Client muss dieser erst noch die Daten anfordern die ihm fehlen
			if (player == m_local_player)
			{
				// Spieler in die Region einfuegen
				player->setState(WorldObject::STATE_ENTER_REGION,false);
				DEBUG5("player can enter region");
			}
			else
			{
				if (player->getState() !=WorldObject::STATE_REGION_DATA_WAITING)
				{
					// den Client benachrichtigen, dass sein Spieler die Region gewechselt hat
					CharConv msg;
					PackageHeader header;
					header.m_content = PTYPE_S2C_REGION_CHANGED;
					header.m_number =region;
					header.toString(&msg);

					WorldObjectMap::iterator it;
					for (it = m_player_slots->begin(); it != m_player_slots->end(); it++)
					{
						if (it->second == player)
						{
							m_network->pushSlotMessage(msg.getBitStream(), it->first);
						}
					}

				}
				// Auf Datenanfrage seitens des Client warten
				player->setState(WorldObject::STATE_REGION_DATA_WAITING,false);
				DEBUG("waiting for a client data request");
			}


		}
		else
		{
			// Clientseite
			if (player == m_local_player)
			{
				// Server nach den fehlenden Informationen fragen
				// wird vom Server initiiert
			}
			else
			{
				DEBUG5("player %i region %i data %i",player->getId(), region, data_missing);
				if (data_missing != 0)
				{
					// Region unbekannt, ignorieren
					player->setState(WorldObject::STATE_INACTIVE,false);
				}
				else
				{
					// Spieler in die Region einfuegen
					player->setState(WorldObject::STATE_ENTER_REGION,false);
				}
			}

		}

		// auf Serverseite muss jetzt eventuell auf die Erzeugung der Region gewartet werden
		if (m_server)
		{
			return true;
		}
	}

	if (player->getState() == WorldObject::STATE_ENTER_REGION)
	{
		// Daten sind vollständig
		Vector pos = player->getShape()->m_center;

		if (m_server)
		{
			if (!reg->hasLocation(m_region_enter_loc[player->getId()]))
			{
				DEBUG("location %s does not exist",m_region_enter_loc[player->getId()].c_str());
				m_region_enter_loc[player->getId()] = m_player_start_location.second;
				
				static_cast<Player*>(player)->setRevivePosition(m_player_start_location);
				short id = getRegionId(m_player_start_location.first);
				insertPlayerIntoRegion(player, id, m_player_start_location.second);
				return true;
			}
			
			
			pos = reg->getLocation(m_region_enter_loc[player->getId()] );
			m_region_enter_loc.erase(player->getId());
		}
		reg->getFreePlace(player->getShape(),player->getLayer() , pos, player);
		reg->insertObject(player, pos,player->getShape()->m_angle);
		DEBUG5("entry position %f %f",pos.m_x, pos.m_y);

		player->setState(WorldObject::STATE_ACTIVE,false);

		// bisheriges Kommando abbrechen
		static_cast<Creature*>(player)->clearCommand(false);
		static_cast<Creature*>(player)->getAction()->m_type = "noaction";

		if (m_server)
		{
			NetEvent event;
			event.m_type = NetEvent::PLAYER_CHANGED_REGION;
			event.m_id = player->getId();
			event.m_data =region ;

			insertNetEvent(event);
		}

		DEBUG5("player %i %p entered region %i %p",player->getId(),player, region,getRegion(region));


	}
	return true;
}

void World::handleSavegame(CharConv *cv, int slot)
{
	DEBUG5("got savegame from slot %i",slot);
	
	Player* pl =0;
	pl= new Player;
	pl->fromSavegame(cv, (slot == LOCAL_SLOT) );
	
	// Spieler ist lokal
	if (slot == LOCAL_SLOT)
	{
		m_local_player = pl;

		if (!m_server)
		{
			// Savegame dem Server senden
			// Savegame an den Server senden
			PackageHeader header;
			header.m_content = PTYPE_C2S_SAVEGAME; 	// Savegame von Client zu Server
			header.m_number =1;
			CharConv save;
			header.toString(&save);

			save.toBuffer((char*) cv->getBitStream()->GetData(),(cv->writeBits()+7)/8);
			m_network->pushSlotMessage(save.getBitStream());
		}
	}

	// Spieler zur Welt hinzufuegen
	if (pl!=0)
	{
		DEBUG5("insert player");
		insertPlayer(pl,slot);
		// Daten aus dem Savegame laden

		pl->setState(WorldObject::STATE_ACTIVE,false);
		pl->recalcDamage();
		
		// Auf Serverseite Spieler gleich in Region einfuegen, auf Clientseite auf Informationen warten
		if (isServer())
		{
			RegionLocation regloc = pl->getRevivePosition();
		
			short id = getRegionId(regloc.first);
			if (id != -1)
			{
				insertPlayerIntoRegion(pl,id, regloc.second);
			}
			else
			{
				pl->setRevivePosition(m_player_start_location);
				insertPlayerIntoRegion(pl, getRegionId(m_player_start_location.first), m_player_start_location.second);
			}
		}
		else
		{
			
		}


		if (m_server)
		{
			WorldObjectMap::iterator it;

			if (slot != LOCAL_SLOT)
			{
				DEBUG5("sending player data ");
				// Daten zur Initialisierung
				PackageHeader header3;
				header3.m_content =PTYPE_S2C_INITIALISATION;
				header3.m_number =1;

				CharConv msg2;
				header3.toString(&msg2);

				// die eigene ID auf Serverseite
				msg2.toBuffer(pl->getId());

				// die Fraktion auf Serverseite
				msg2.toBuffer<char>(pl->getFraction());


				m_network->pushSlotMessage(msg2.getBitStream(),slot);


				// Dem Spieler Informationen ueber alle anderen Spieler in der Welt senden
				PackageHeader header;
				header.m_content = PTYPE_S2C_PLAYER;		// Spielerdaten vom Server zum Client
				header.m_number = m_player_slots->size()-1;	// alle Spieler bis auf den eigenen

				CharConv msg;
				header.toString(&msg);

				// Informationen ueber die Spieler
				for (it = m_player_slots->begin(); it != m_player_slots->end(); ++it)
				{
					// Nur senden, wenn es nicht der eigene Spieler ist
					if (it->first != slot)
					{
						DEBUG5("writing player slot %i",slot);
						it->second->toString(&msg);
					}
				}

				// Nachricht an den Client senden
				if (header.m_number>0)
				{
					m_network->pushSlotMessage(msg.getBitStream(),slot);
				}
			}

			// Nachricht von dem neuen Spieler an alle anderen Spieler senden
			// ausser dem Spieler selbst und dem Server
			PackageHeader header2;
			header2.m_content = PTYPE_S2C_PLAYER;	// Spielerdaten vom Server zum Client
			header2.m_number = 1;					// der neue Spieler

			CharConv msg2;
			header2.toString(&msg2);
			pl->toString(&msg2);

			for (it = m_player_slots->begin(); it != m_player_slots->end(); ++it)
			{
				if (it->first != slot && it->first != LOCAL_SLOT)
				{
					m_network->pushSlotMessage(msg2.getBitStream(),it->first);
				}
			}
			
			// Nachricht ueber die Wegpunkte
			if (slot != LOCAL_SLOT)
			{
				std::map<short,WaypointInfo>& winfos =getWaypointData();
				std::map<short,WaypointInfo>::iterator lt;
				
				PackageHeader header3;
				header3.m_content = PTYPE_S2C_WAYPOINTS;	// Spielerdaten vom Server zum Client
				header3.m_number = winfos.size();					// der neue Spieler
	
				CharConv msg3;
				header3.toString(&msg3);
				
				
				for (lt = winfos.begin(); lt != winfos.end(); ++lt)
				{
					msg3.toBuffer(lt->first);
					msg3.toBuffer(lt->second.m_name);
					msg3.toBuffer(lt->second.m_id);
					msg3.toBuffer(lt->second.m_world_coord.m_x);
					msg3.toBuffer(lt->second.m_world_coord.m_y);
					DEBUG5("sending waypoint info %i %s %f %f",lt->first, lt->second.m_name.c_str(), lt->second.m_world_coord.m_x,lt->second.m_world_coord.m_y);
				}
				
				std::map<std::string, int>::iterator rnt;
				msg3.toBuffer<int>(m_region_name_id.size());
				for (rnt = m_region_name_id.begin(); rnt != m_region_name_id.end(); ++rnt)
				{
					msg3.toBuffer(rnt->first);
					msg3.toBuffer(rnt->second);
				}
				
				m_network->pushSlotMessage(msg3.getBitStream(),slot);
			
				// Informationen ueber Parties senden
				PackageHeader header4;
				header4.m_content =PTYPE_S2C_PARTY;

				for (int i=0; i< m_max_nr_players; i++)
				{
					if (m_parties[i].getNrMembers() > 0)
					{
						DEBUG("sending data for party %i",i);
						header4.m_number =i;
						CharConv msg3;
						header4.toString(&msg3);

						m_parties[i].toString(&msg3);

						m_network->pushSlotMessage(msg3.getBitStream(),slot);
					}
				}
			
				// Informationen ueber Parties senden
				PackageHeader header5;
				header5.m_number =1;
				header5.m_content =PTYPE_S2C_QUEST;

				std::map< std::string, Quest * >::iterator it;
				
				for (it = m_quests.begin(); it != m_quests.end(); ++it)
				{
					
						DEBUG5("sending data for quest %s",it->second->getName().c_str());
						
						CharConv msg5;
						header5.toString(&msg5);

						it->second->toString(&msg5);
						m_network->pushSlotMessage(msg5.getBitStream(),slot);
					
				}
				
				PackageHeader header6;
				header6.m_number =1;
				header6.m_content =PTYPE_S2C_FRACTION;

				std::map< Fraction::Id, Fraction* >::iterator ft;
				
				for (ft = m_fractions.begin(); ft != m_fractions.end(); ++ft)
				{
					
					DEBUG5("sending data for Fraction %s",ft->second->getType().c_str());
						
					CharConv msg6;
					header6.toString(&msg6);

					ft->second->toString(&msg6);
					m_network->pushSlotMessage(msg6.getBitStream(),slot);
					
				}
			}
			
		}
	}


}


void World::handleCommand(ClientCommand* comm, int slot, float delay)
{
	DEBUG5("Kommando (%f %f) button: %i id: %i action: %s",comm->m_goal.m_x,comm->m_goal.m_y,comm->m_button, comm->m_id,comm->m_action.c_str());


	// Wenn man sich nicht auf Serverseite befindet
	if (!m_server)
	{
		// Kommando an den Server senden
		CharConv cv;

		// Header anlegen
		PackageHeader header;
		header.m_content = PTYPE_C2S_COMMAND; 	// Daten von Client zu Server
		header.m_number = 1;


		// Header in den Puffer schreiben
		header.toString(&cv);
		// Kommando in den Puffer schreiben
		comm->toString(&cv);

		/*
		timeval tv;
		gettimeofday(&tv, NULL);
		DEBUG("timestamp %i system time %i",cv.getTimestamp(),tv.tv_usec/1000);

		*/
	 	// Datenpaket zum Server senden
		getNetwork()->pushSlotMessage(cv.getBitStream());
	}

	Player* pl = static_cast<Player*> ((*m_player_slots)[slot]);
	if (pl == 0)
	{
		ERRORMSG("no player in slot %i",slot);
	}
	else
	{
		pl->onClientCommand(comm,delay);
	}


}

void World::handleMessage(std::string msg, int slot)
{

	std::string smsg = msg;

	// als Server: Nachricht an alle Spieler versenden
	// als Client: Nachricht an den Server senden
	if (m_server && msg[0]!='$')
	{
		
		// Name des Senders an die Nachricht haengen
		smsg = "";
		if (m_player_slots->count(slot)>0)
		{
			Player* pl = static_cast<Player*>((*m_player_slots)[slot]);
			if (pl != 0)
			{
				smsg += "[";
				smsg += pl->getName();
				
				if (pl->getSpeakText().m_text == "" && pl->getDialogue() == 0)
				{
					CreatureSpeakText text;
					text.m_text = msg;
					text.m_time = msg.size()*100 + 1000;
					text.m_in_dialogue = false;
					pl->speakText(text);
				}
				smsg += "] ";
			}
		}
		smsg += msg;

		CharConv cv;

		// Header anlegen
		PackageHeader header;
		header.m_content = PTYPE_S2C_MESSAGE; 	// Daten von Server zum Client
		header.m_number = smsg.size();

		header.toString(&cv);
		cv.toBuffer((char*) smsg.c_str(),smsg.size());

		// Nachricht an alle Spieler mit ausser dem Sender
		WorldObjectMap::iterator it;
		for (it = m_player_slots->begin(); it != m_player_slots->end(); ++it)
		{
			if (it->first != slot && it->first!=LOCAL_SLOT)
			{
				getNetwork()->pushSlotMessage(cv.getBitStream(),it->first);
			}
		}
	}
	else
	{
		CharConv cv;

		// Header anlegen
		PackageHeader header;
		header.m_content = PTYPE_C2S_MESSAGE; 	// Daten von Client zu Server
		header.m_number = msg.size();

		header.toString(&cv);
		cv.toBuffer((char*) msg.c_str(),msg.size());

		getNetwork()->pushSlotMessage(cv.getBitStream());

		smsg = "[";
		smsg += m_local_player->getName();
		smsg += "] ";
		smsg += msg;
	}

	// Nachricht einfuegen
	static_cast<Player*>(m_local_player)->addMessage(smsg);

	if (msg[0] == '$')
	{
		// Cheatcode eingegeben
		Player* pl = 0;
		if (m_player_slots->count(slot)>0)
		{
			pl = static_cast<Player*>((*m_player_slots)[slot]);
		}

		std::stringstream stream;
		stream << msg;

			// $ lesen
		char dummy;
		stream >> dummy;

		std::string obj;
		stream >> obj;
		if (obj == "item")
		{
			// Item erzeugen
			std::string name="";
			int val =0;
			stream >> name >> val;

			Item::Type type = ItemFactory::getBaseType(name);

			Item* itm = ItemFactory::createItem(type, name,0,val);
			if (itm != 0)
			{
				pl->insertItem(itm,true);
			}
		}
		else if (obj == "get" && pl != 0)
		{
			EventSystem::setRegion(m_local_player->getRegion());
			std::string member;
			stream >> member;
			std::stringstream instr;
			instr << "return getObjectValue(";
			instr << m_local_player->getId();
			instr << ", \"" << member << "\")";
			EventSystem::doString((char*) instr.str().c_str());
			std::string ret = EventSystem::getReturnValue();
			if (ret != "")
			{
				static_cast<Player*>(m_local_player)->addMessage(ret);
			}
		}
		else if (obj == "set" && pl != 0)
		{
			EventSystem::setRegion(m_local_player->getRegion());
			std::string member, val;
			stream >> member >> val;
			std::stringstream instr;
			instr << "setObjectValue(";
			instr << m_local_player->getId();
			instr << ", \"" << member << "\","<<val<<")";
			EventSystem::doString((char*) instr.str().c_str());
		}
		else
		{
			EventSystem::setRegion(m_local_player->getRegion());
			std::string instr = "";
			instr += msg.substr(1);
			std::string ret;
			
			EventSystem::doString((char*) instr.c_str());
			do
			{
				ret = EventSystem::getReturnValue();
				if (ret != "")
				{
					DEBUG5("return value %s",ret.c_str());
					static_cast<Player*>(m_local_player)->addMessage(ret);
				}
			}
			while (ret != "");
		}
	}
}

int World::getValidId()
{
	// zufällige ID erzeugen;
	static int j=1;
	return j++;
}


int World::getValidProjectileId()
{
	// zufällige ID erzeugen;
	return getValidId();
}


void World::update(float time)
{

	// Timer weiterzaehlen und Limits feststellen
	static float timer_max[6] = {200,500,1000,2000,5000,10000};
	for (int i=0; i<6; i++)
	{
		m_timer[i] += time;
		m_timer_limit[i] = false;
		if (m_timer[i] > timer_max[i])
		{
			m_timer[i] -= timer_max[i];
			m_timer_limit[i] = true;
		}
	}

	DEBUG5("update %f",time);
	std::map<int,Region*>::iterator rit;
	for (rit = m_regions.begin(); rit != m_regions.end(); rit++)
	{
		rit->second->update(time);
	}
	
	if (m_timer_limit[1])
	{
		// Parties aktualisieren
		for (unsigned int i=0; i<m_parties.size(); ++i)
		{
			m_parties[i].updateMinimaps();	
		}
	}


	m_network->update();


	if (m_server)
	{
		updateLogins();
		acceptLogins();
		
		// Lua update
		if (getLocalPlayer() !=0)
		{
			std::string chunk = EventSystem::getPlayerVarString(getLocalPlayer()->getId());
			EventSystem::doString(chunk.c_str());
			EventSystem::clearPlayerVarString(getLocalPlayer()->getId());
			
			std::stringstream stream;
			stream << "updatePlayerVars("<<getLocalPlayer()->getId()<<");";
			EventSystem::doString(stream.str().c_str());
		}
	}

	updatePlayers();

	m_events->clear();

	std::map<int,Region*>::iterator rrit;
	for (rrit = m_regions.begin(); rrit != m_regions.end(); rrit++)
	{
		rrit->second->getNetEvents()->clear();
	}

	m_network->update();



}

void World::updatePlayers()
{
	// Schleife ueber die Spieler
	WorldObjectMap::iterator it;
	Player* pl;
	int slot = LOCAL_SLOT;
	if (m_local_player !=0)
	for (it = m_player_slots->begin(); it != m_player_slots->end(); )
	{
		slot = it->first;
		pl = static_cast<Player*>(it->second);

		// feststellen, ob ein Spieler das Spiel verlassen hat
		// ggf NetEvent erstellen
		if (m_server && slot != LOCAL_SLOT &&
				  (m_network->getSlotStatus( slot )!=NET_CONNECTED || pl->getState() == WorldObject::STATE_QUIT))
		{
			NetEvent event;
			event.m_type = NetEvent::PLAYER_QUIT;
			event.m_id = pl->getId();

			insertNetEvent(event);

			if (pl->getRegion() !=0)
			{
				pl->getRegion()->deleteObject(pl);
			}
			m_players->erase( pl->getId());
			m_player_slots->erase(it++);

			// Spieler aus seiner Party entfernen
			pl->getParty()->removeMember(pl->getId());

			// aus allen Partys als Bewerber loeschen
			for (int i=0; i<m_max_nr_players; i++)
			{
				if (getParty(i)->getCandidates().count(pl->getId()) ==1)
				{
					getParty(i)->removeCandidate(pl->getId());
				}
			}


			DEBUG5("player %i has quit",pl->getId());

			delete pl;
			continue;
		}
		
		// Spielern die auf Daten zur aktuellen Region warten, Daten senden
		if (pl->getState() == WorldObject::STATE_REGION_DATA_REQUEST)
		{
			DEBUG("send data request to server");
			// Client wartet auf Daten zur Region
			pl->setState(WorldObject::STATE_REGION_DATA_WAITING,false);

			// fehlende Daten zur Region anfordern
			PackageHeader header;
			header.m_content = PTYPE_C2S_DATA_REQUEST; 	// Data Request von Client zu Server
			header.m_number =1;

			ClientDataRequest datareq;
			datareq.m_data = ClientDataRequest::REGION_ALL;
			datareq.m_id = pl->getRegionId();
			datareq.m_region_id = pl->getRegionId();

			CharConv msg;
			header.toString(&msg);
			datareq.toString(&msg);

			m_network->pushSlotMessage(msg.getBitStream());
		}

		// Spieler, deren Regionen komplett geladen wurden aktivieren
		if (m_server)
		{
			
			if (pl->getState() == WorldObject::STATE_ENTER_REGION && pl->getRegion() !=0 )
			{
				// TODO: Ort angeben
				insertPlayerIntoRegion(pl,pl->getRegionId(), "entry_south");
			}
		}


		// Wenn aktuelle Instanz Server ist:
		// Daten von allen verbundenen Client annehmen und verarbeiten
		if (m_server && slot != LOCAL_SLOT)
		{
			
			
			// Nachrichten fuer die Spieler abholen und Verteilen
			PackageHeader headerp;
			Packet* data;
			CharConv* cv;

			// Schleife ueber die Nachrichten
			while (m_network->numberSlotMessages( slot )>0)
			{
				m_network->popSlotMessage( data ,slot);

				cv = new CharConv(data);
				if (cv->getDelay()>1000)
				{
					DEBUG("got packet with delay %f",cv->getDelay());
				}

				headerp.fromString(cv);

				// Kommando bearbeiten
				if (headerp.m_content ==  PTYPE_C2S_COMMAND)
				{
					// Kommando Daten erhalten
					ClientCommand com;

					// Spielerobjekt die Daten senden
					com.fromString(cv);

					handleCommand(&com,slot,cv->getDelay());
				}

				// Datenanfrage bearbeiten
				if (headerp.m_content == PTYPE_C2S_DATA_REQUEST)
				{
					// Datenanfrage erhalten
					ClientDataRequest req;
					req.fromString(cv);

					handleDataRequest(&req,slot);
				}

				if (headerp.m_content == PTYPE_C2S_MESSAGE)
				{
					char* buf = new char[headerp.m_number+1];
					buf[headerp.m_number] = 0;
					cv->fromBuffer(buf, headerp.m_number);

					handleMessage(buf,slot);

					delete buf;
				}

				delete cv;
			}

		}

		if (m_server)
		{
			// NetEvents fuer die Spieler generieren
			if (pl->getNetEventMask() !=0)
			{
				DEBUG5(" send player event id %i data %i",pl->getId(), pl->getNetEventMask());
				NetEvent event;
				event.m_type = NetEvent::OBJECT_STAT_CHANGED;
				event.m_data = pl->getNetEventMask();
				event.m_id = pl->getId();
				insertNetEvent(event);
				
				pl->clearNetEventMask();
				
				
			}
		}

		++it;
	}

	if (!m_server)
	{
		// Daten vom Server empfangen und verarbeiten
		if (m_network->getSlotStatus()!=NET_CONNECTED)
		{
			// disconnect
			if (m_network->getSlotStatus() == NET_TIMEOUT)
			{
				m_local_player->setState(GameObject::STATE_QUIT,false);
			}
		}
		else
		{
			PackageHeader headerp;
			Packet* data;
			CharConv* cv;

			// Nachrichten vom Server empfangen
			while (m_network->numberSlotMessages()>0)
			{
				m_network->popSlotMessage( data ,slot);

				cv = new CharConv(data);

				headerp.fromString(cv);

				if (headerp.m_content == PTYPE_S2C_PLAYER)
				{
					for (int n=0; n< headerp.m_number;n++)
					{
						// Daten zu Spielern erhalten
						// Typ Spieler (schon bekannt)
						GameObject::Type tmp;
						cv->fromBuffer(tmp);

						// Subtyp
						GameObject::Subtype subt;
						cv->fromBuffer(subt);

						int id;
						cv->fromBuffer(id);
						WorldObject* player;

						DEBUG("got data for player %s id %i",subt.c_str(),id);

						// Spieler entweder neu anlegen oder aus den existierenden herraussuchen
						if (m_players->count(id)==0)
						{
							// Spieler existiert noch nicht
							player = ObjectFactory::createObject("PLAYER", subt,id);
							insertPlayer(player);
						}
						else
						{
							player = (*m_players)[id];
						}

						// Daten aktualisieren
						player->fromString(cv);

					}

				}
				else if (headerp.m_content == PTYPE_S2C_REGION)
				{
					// Daten zu einer Region erhalten
					DEBUG("got data for region %i",headerp.m_number);
					short dimx, dimy;

					// Groesse der Region
					cv->fromBuffer(dimx);
					cv->fromBuffer(dimy);

					Region* reg = getRegion(headerp.m_number);
					if (reg ==0)
					{
						reg = new Region(dimx,dimy,headerp.m_number,"");
					}
					else
					{
						// Daten loeschen?
					}

					// Daten schreiben
					reg->setRegionData(cv,m_players);

					// lokalen Spieler fuer die Region freischalten
					m_local_player->setState(WorldObject::STATE_ENTER_REGION,false);

					// Region einfuegen
					if (getRegion(headerp.m_number) ==0)
					{
						insertRegion(reg, headerp.m_number);
					}
				}
				else if (headerp.m_content == PTYPE_S2C_INITIALISATION)
				{
					int id;
					cv->fromBuffer(id);
					DEBUG("ID at server %i",id);
					m_players->clear();
					m_local_player->setId(id);

					char frac;
					cv->fromBuffer(frac);
					m_local_player->setFraction((Fraction::Id) frac);
					DEBUG("fraction %i",frac);

					insertPlayer(m_local_player, LOCAL_SLOT);
				}
				else if (headerp.m_content == PTYPE_S2C_WAYPOINTS)
				{
					DEBUG5("got waypoints");
					std::map<short,WaypointInfo>& winfos = World::getWorld()->getWaypointData();
					winfos.clear();
					
					WaypointInfo wi;
					short regid;
					for (int i=0; i<headerp.m_number; i++)
					{
						cv->fromBuffer(regid);
						cv->fromBuffer(wi.m_name);
						cv->fromBuffer(wi.m_id);
						cv->fromBuffer(wi.m_world_coord.m_x);
						cv->fromBuffer(wi.m_world_coord.m_y);	
						
						winfos[regid] = wi;
						DEBUG5("got waypoint info %i %s %f %f",regid, wi.m_name.c_str(), wi.m_world_coord.m_x,wi.m_world_coord.m_y);
					}
					
					int nrregions;
					cv->fromBuffer(nrregions);
					m_region_name_id.clear();
					
					std::string regname;
					int id;
					for (int i = 0; i < nrregions; i++)
					{
						cv->fromBuffer(regname);
						cv->fromBuffer(id);
						m_region_name_id.insert(std::make_pair(regname,id));
					}
					
				}
				else if (headerp.m_content == PTYPE_S2C_PARTY)
				{
					int id = headerp.m_number;
					getParty(id)->fromString(cv);
				}				
				else if (headerp.m_content == PTYPE_S2C_QUEST)
				{
					std::string name,tabname;
					cv->fromBuffer(name);
					cv->fromBuffer(tabname);
					DEBUG5("got data for quest %s %s",name.c_str(), tabname.c_str());
					
					Quest* qu = new Quest(name,tabname);
					qu->fromString(cv);
					qu->init();
					addQuest(tabname,qu);
				}
				else if (headerp.m_content == PTYPE_S2C_FRACTION)
				{
					Fraction::Id id;
					Fraction::Type type;
					cv->fromBuffer(id);
					cv->fromBuffer(type);
					
					Fraction* frac = new Fraction(id, type);
					frac->fromString(cv);
					m_fractions[id] = frac;
					
					
				}
				else if (headerp.m_content == PTYPE_S2C_REGION_CHANGED)
				{
					DEBUG5("notified that region changed to %i",headerp.m_number);
					// der lokale Spieler hat die Region gewechselt
					// fehlende Daten anfordern
					if (m_local_player->getRegion()!=0)
					{
						m_local_player->getRegion()->deleteObject(m_local_player);
					}
					m_local_player->setState(WorldObject::STATE_REGION_DATA_REQUEST,false);
					m_local_player->setRegionId(headerp.m_number);
					DEBUG5("state %i",m_local_player->getState());
	
				}
				else if (headerp.m_content == PTYPE_S2C_EVENT)
				{
					Region* reg = m_local_player->getRegion();

					bool ret;
					for (int n=0; n< headerp.m_number;n++)
					{
						ret = processNetEvent(reg,cv);

						if (ret == false)
						{
							break;

						}
					}

				}
				else if (headerp.m_content == PTYPE_S2C_MESSAGE)
				{
					char* buf = new char[headerp.m_number+1];
					buf[headerp.m_number] = 0;
					cv->fromBuffer(buf, headerp.m_number);

					static_cast<Player*>(m_local_player)->addMessage(buf);

					delete buf;
				}
				else if (headerp.m_content == PTYPE_S2C_LUA_CHUNK)
				{
					std::string chunk;
					cv->fromBuffer(chunk);
					DEBUG5("got lua chunk %s",chunk.c_str());
					EventSystem::doString(chunk.c_str());
				}
				else if (headerp.m_content == PTYPE_S2C_DATA_CHECK)
				{
					if (m_local_player->getRegion() !=0)
					{
						m_local_player->getRegion() ->	checkRegionData(cv);
					}
				}
				else if (headerp.m_content == PTYPE_C2S_MESSAGE)
				{
					
				}
				else
				{
					DEBUG("got package with unknown type %i", headerp.m_content);
				}
				
				delete cv;
			}

		}
	}




	if (m_server)
	{
		// Nachrichten ueber die NetEvents zur den Clients senden
		Region* reg;
		NetEventList::iterator lt;
		for (it = m_player_slots->begin(); it != m_player_slots->end(); ++it)
		{
			slot = it->first;
			pl = static_cast<Player*>(it->second);

			if (slot != LOCAL_SLOT)
			{
				// Anzahl der NetEvents

				reg = pl->getRegion();
				if (pl->getState() != WorldObject::STATE_ACTIVE && pl->getState() != WorldObject::STATE_DEAD && pl->getState() != WorldObject::STATE_DIEING && pl->getState() != WorldObject::STATE_INACTIVE)
				{
					reg =0;
				}

				CharConv* msg;
				PackageHeader header;
				header.m_content = PTYPE_S2C_EVENT;
				header.m_number =1;



				// globale NetEvents
				for (lt = m_events->begin(); lt != m_events->end(); ++lt)
				{
					msg = new CharConv;
					DEBUG5(" send global event %i id %i data %i",lt->m_type,lt->m_id, lt->m_data);

					header.toString(msg);
					bool ret = writeNetEvent(reg,&(*lt),msg);
					if (ret)
					{
						m_network->pushSlotMessage(msg->getBitStream(),slot);
					}
					delete msg;
				}

				// NetEvents der Region in der der Spieler ist
				bool ret;
				if (reg !=0)
				{
					for (lt = reg->getNetEvents()->begin(); lt != reg->getNetEvents()->end(); ++lt)
					{
						msg = new CharConv;
						DEBUG5(" send local event %i id %i data %i",lt->m_type,lt->m_id, lt->m_data);

						header.toString(msg);
						ret = writeNetEvent(reg,&(*lt),msg);

						if (ret)
						{
							m_network->pushSlotMessage(msg->getBitStream(),slot);
						}
						delete msg;
					}
				}

				// Lua Chunk
				std::string chunk = EventSystem::getPlayerVarString(pl->getId());
				if (chunk != "")
				{
					msg = new CharConv;
					header.m_content = PTYPE_S2C_LUA_CHUNK;
					DEBUG5("send lua chunk: %s",chunk.c_str());
					
					header.toString(msg);
					msg->toBuffer(chunk);
					m_network->pushSlotMessage(msg->getBitStream(),slot);
					
					EventSystem::clearPlayerVarString(pl->getId());
					delete msg;
				}
				
				// Daten abgleichen
				if (pl->getState() == WorldObject::STATE_ACTIVE && pl->getRegion() !=0 && m_timer_limit[3])
				{
					CharConv cv;
					PackageHeader header;
					header.m_content = PTYPE_S2C_DATA_CHECK;
				
					header.toString(&cv);
				
					pl->getRegion()->getRegionCheckData(&cv);
				
					m_network->pushSlotMessage(cv.getBitStream(),slot);
				}
			}
			
			
		}
	}	
}

bool World::writeNetEvent(Region* region,NetEvent* event, CharConv* cv)
{
	event->toString(cv);

	DEBUG5("sending event %i  id %i  data %i",event->m_type, event->m_id, event->m_data);
	

	WorldObject* object;
	Projectile* proj;
	if (region !=0)
	{
		if (event->m_type == NetEvent::OBJECT_CREATED)
		{
			
			object =region->getObject(event->m_id);
			DEBUG5("object created %s %i",object->getSubtype().c_str(), object->getId());
			object->toString(cv);
		}
		else if (event->m_type == NetEvent::OBJECT_STAT_CHANGED)
		{
			if (m_players->count(event->m_id) ==1)
			{
				object = (*m_players)[event->m_id];
			}
			else
			{
				object =region->getObject(event->m_id);
			}

			if (object !=0)
			{
				
				object->writeNetEvent(event,cv);
			}
			else
				return false;

		}
		else if (event->m_type == NetEvent::PROJECTILE_CREATED)
		{
			proj = region->getProjectile(event->m_id);
			proj->toString(cv);
			proj->clearNetEventMask();
		}
		else if (event->m_type == NetEvent::PROJECTILE_STAT_CHANGED)
		{
			proj = region->getProjectile(event->m_id);
			if (proj !=0)
			{
				proj->writeNetEvent(event,cv);
			}
			else
				return false;
		}
		else if (event->m_type == NetEvent::ITEM_DROPPED)
		{
			DropItem* di;
			di = region->getDropItem(event->m_id);
			if (di !=0)
			{
				
				di->toString(cv);
				di->clearNetEventMask();
			}
			else
				return false;
		}
		else if (event->m_type == NetEvent::TRADER_INVENTORY_REFRESH)
		{
			Creature* npc = dynamic_cast<Creature*>(region->getObject(event->m_id));
			if (npc ==0 || npc->getEquipement() ==0)
				return false;
			
			npc->getEquipement()->toStringComplete(cv);
		}
		else if (event->m_type == NetEvent::TRADER_ITEM_BUY)
		{
			Creature* npc = dynamic_cast<Creature*>(region->getObject(event->m_id));
			if (npc ==0 || npc->getEquipement() ==0)
				return false;
			
			Item* item = npc->getEquipement()->getItem(event->m_data);
			if (item ==0)
				return false;
			
			DEBUG5("sending item %i of trader %i",event->m_data,npc->getId());
			
			item->toStringComplete(cv);
		}
		else if (event->m_type == NetEvent::REGION_CAMERA)
		{
			region->getCamera().toString(cv);
		}
		else if (event->m_type == NetEvent::DIALOGUE_CREATED)
		{
			Dialogue* dia = region->getDialogue(event->m_id);
			if (dia != 0)
			{
				dia->toString(cv);
			}
		}
		else if (event->m_type == NetEvent::DIALOGUE_STAT_CHANGED)
		{
			Dialogue* dia = region->getDialogue(event->m_id);
			if (dia != 0)
			{
				dia->toString(cv);
			}
		}
		else if (event->m_type == NetEvent::DAMAGE_VISUALIZER_CREATED)
		{
			std::map<int,DamageVisualizer>& dmgvis = region->getDamageVisualizer();
			std::map<int,DamageVisualizer>::iterator it= dmgvis.find(event->m_id);
			if (it != dmgvis.end())
			{
				cv->toBuffer(it->second.m_number);
				cv->toBuffer(it->second.m_time);
				cv->toBuffer(it->second.m_size);
				cv->toBuffer(it->second.m_position.m_x);
				cv->toBuffer(it->second.m_position.m_y);	
			}
		}
	}
	else
	{
		// Spieler kann man auch ohne Region aktualisieren
		if (event->m_type == NetEvent::OBJECT_STAT_CHANGED && m_players->count(event->m_id) ==1)
		{
			
			object = (*m_players)[event->m_id];
			object->writeNetEvent(event,cv);
		}
	}

	if (event->m_type == NetEvent::PLAYER_CHANGED_REGION)
	{

		object = (*m_players)[event->m_id];
		if (object !=0)
		{
			cv->toBuffer(object->getShape()->m_center.m_x);
			cv->toBuffer(object->getShape()->m_center.m_y);
		}
		else
		{
			return false;
		}

	}

	if (event->m_type == NetEvent::PLAYER_ITEM_SWAP)
	{
		object = (*m_players)[event->m_id];
		if (object != 0)
		{
			cv->toBuffer(static_cast<Player*>(object)->isUsingSecondaryEquip());
		}
		else
		{
			return false;
		}
	}
	
	if (event->m_type == NetEvent::PLAYER_ITEM_INSERT)
	{
		object = (*m_players)[event->m_id];
		if (object != 0)
		{
			if (event->m_data != Equipement::GOLD)
			{
				if (static_cast<Player*>(object)->getEquipement()->getItem(event->m_data) ==0)
				{
					ERRORMSG("no item at pos %i (player %i)",event->m_data,object->getId());
					return false;
				}
				cv->toBuffer<short>((short) event->m_data);
				static_cast<Player*>(object)->getEquipement()->getItem(event->m_data)->toStringComplete(cv);
				DEBUG5("player item insert");
			}
			else
			{
				cv->toBuffer<int>(static_cast<Player*>(object)->getEquipement()->getGold());
			}
		}
		else
			return false;

	}

	if (event->m_type ==  NetEvent::ITEM_REMOVED)
	{
		DEBUG5("removing item %i",event->m_id);
	}

	if (event->m_type == NetEvent::PLAYER_PARTY_CHANGED)
	{
		object = (*m_players)[event->m_id];
		if (object != 0)
		{
			cv->toBuffer<char>(static_cast<Player*>(object)->getParty()->getId());
			DEBUG5("player %i changed party to %i",event->m_id, static_cast<Player*>(object)->getParty()->getId());
		}
		else
			return false;
	}

	
	if (event->m_type == NetEvent::FRACTION_RELATION_CHANGED)
	{
		Fraction::Relation rel = getRelation(event->m_id, event->m_data);
		cv->toBuffer<char>(rel);
	}
	
	if (event->m_type == NetEvent::PLAYER_PARTY_CANDIDATE)
	{
		object = (*m_players)[event->m_id];
		if (object != 0)
		{
			cv->toBuffer<char>(static_cast<Player*>(object)->getCandidateParty());
			DEBUG5("player %i candidate for party %i",event->m_id, static_cast<Player*>(object)->getCandidateParty());
		}
		else
			return false;
	}

	if (event->m_type == NetEvent::PARTY_RELATION_CHANGED)
	{
		DEBUG("party %i changed relation to %i to %i",event->m_data, event->m_id, getParty(event->m_data)->getRelations()[event->m_id]);
		cv->toBuffer<char>(getParty(event->m_data)->getRelations()[event->m_id]);
	}
	


	return true;
}


bool World::processNetEvent(Region* region,CharConv* cv)
{
	NetEvent event;
	event.fromString(cv);


	DEBUG5("got event %i  id %i  data %i",event.m_type, event.m_id, event.m_data);

	WorldObject* object;
	Creature* cr;
	Projectile* proj;
	Dialogue* dia;
 	Item* item;
	int id;
	bool mode;
	std::string subtype;
	char type;
	bool success;

	// Objekt suchen dass zu dem NetEvent gehoert
	// Spieler werden aus der Spielerliste gesucht
	// andere Objekte aus der Region
	if (event.m_type ==  NetEvent::OBJECT_STAT_CHANGED ||
		   event.m_type ==  NetEvent::OBJECT_DESTROYED)
	{
		if (m_players->count(event.m_id) ==1)
		{
			object = (*m_players)[event.m_id];
		}
		else
		{
			if (region !=0)
			{
				object =region->getObject(event.m_id);
				if (object ==0)
					return false;
			}
			else
			{
				return false;
			}
		}
	}

	switch(event.m_type)
	{
		case NetEvent::OBJECT_CREATED:
			if (region !=0)
			{
				region->createObjectFromString(cv, m_players);
			}
			else
			{
				return false;
			}
			break;

		case NetEvent::OBJECT_STAT_CHANGED:

			if (m_players->count(event.m_id) ==1)
			{
				object = (*m_players)[event.m_id];
			}
			else
			{
				if (region !=0)
				{
					object =region->getObject(event.m_id);
					if (object ==0)
						return false;
				}
				else
					return false;
			}
			object->processNetEvent(&event,cv);
			break;


		case NetEvent::OBJECT_DESTROYED:
			if (m_players->count(event.m_id) ==1)
			{
				object = (*m_players)[event.m_id];
			}
			else
			{
				if (region !=0)
				{
					object =region->getObject(event.m_id);
					if (object ==0)
						return false;
				}
				else
					return false;
			}
			object->destroy();
			success = true;
			if (region !=0)
			{
				success = region->deleteObject(object);
			}
			if (success)
			{
				delete object;
			}

			break;

		case NetEvent::PROJECTILE_CREATED:
			if (region !=0)
			{
				region->createProjectileFromString(cv);
			}
			else
			{
				return false;
			}
			break;

		case NetEvent::PROJECTILE_STAT_CHANGED:
			if (region !=0)
			{
				proj = region->getProjectile(event.m_id);
				if (proj !=0)
				{
					proj->processNetEvent(&event,cv);
				}
				else
				{
					DEBUG("projectile %i for event does not exist",event.m_id);
					return false;
				}
			}
			else
			{
				return false;
			}
			break;
		case NetEvent::PROJECTILE_DESTROYED:
			if (region !=0)
			{
				proj = region->getProjectile(event.m_id);
				if (proj != 0)
				{
					region->deleteProjectile(proj);
					delete proj;
				}
			}
			else
			{
				return false;
			}
			break;
			
		case NetEvent::TRADER_INVENTORY_REFRESH:
			
			cr = dynamic_cast<Creature*>(region->getObject(event.m_id));
			if (cr ==0)
				return false;
			
			if (cr->getEquipement() ==0)
			{
				cr->setEquipement(new Equipement(100,100,100));
			}
			cr->getEquipement()->clear();
			
			cr->getEquipement()->fromStringComplete(cv);
			break;
			
		case NetEvent::TRADER_ITEM_BUY:

			cr = dynamic_cast<Creature*>(region->getObject(event.m_id));
			if (cr ==0 || cr->getEquipement() ==0)
				return false;
			
			// Item erstellen
			cv->fromBuffer(type);
			cv->fromBuffer(subtype);
			cv->fromBuffer(id);
			item = ItemFactory::createItem((Item::Type) type,subtype,id);
			item->fromStringComplete(cv);
			
			cr->getEquipement()->swapItem(item, event.m_data);
			if (item !=0)
				delete item;
			
			break;
			
		case NetEvent::TRADER_ITEM_SELL:
			cr = dynamic_cast<Creature*>(region->getObject(event.m_id));
			if (cr ==0 || cr->getEquipement() ==0)
				return false;
			
			item =0;
			cr->getEquipement()->swapItem(item, event.m_data);
			if (item !=0)
				delete item;
			break;

		case NetEvent::PLAYER_CHANGED_REGION:
			DEBUG5("received event player %i changed region %i",event.m_id, event.m_data);

			if (m_players->count (event.m_id)>0)
			{
				object = (*m_players)[event.m_id];

				// Spieler aus seiner bisherigen Region entfernen
				if (object->getRegion() !=0 && object != m_local_player)
				{
					object->getRegion()->deleteObject(object);
				}
				object->setRegionId( event.m_data);

				cv->fromBuffer(object->getShape()->m_center.m_x);
				cv->fromBuffer(object->getShape()->m_center.m_y);
				insertPlayerIntoRegion(object,event.m_data);
			}
			break;

		case NetEvent::PLAYER_QUIT:
			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];
				if (object != m_local_player)
				{
					if (object->getRegion() !=0)
					{
						object->getRegion()->deleteObject(object);
					}
					m_players->erase( object->getId());

					WorldObjectMap::iterator it;
					for (it = m_player_slots->begin(); it != m_player_slots->end(); ++it)
					{
						if (it->second == object)
						{
							m_player_slots->erase(it);
							break;
						}

					}

					delete object;
				}
			}
			break;

		case NetEvent::ITEM_DROPPED:
			if (region !=0)
			{
				region->createItemFromString(cv);
			}
			else
			{
				return false;
			}
			break;

		case NetEvent::ITEM_REMOVED:
			if (region !=0)
			{
				DEBUG5("remove item %i",event.m_id);
				region->deleteItem(event.m_id,true);
			}
			else
			{
				return false;
			}
			break;

		case NetEvent::PLAYER_ITEM_REMOVE:
			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];
				
				Item* item =0;
				static_cast<Player*>(object)->getEquipement()->swapItem(item,event.m_data);

				if (item !=0)
					delete item;
			
			}
			else
			{
				return false;
			}
			break;
			
		case NetEvent::PLAYER_ITEM_SWAP:
			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];
				bool sec;
				cv->fromBuffer(sec);
				
				static_cast<Player*>(object)->setUsingSecondaryEquip(sec);
			}
			break;

		case NetEvent::PLAYER_ITEM_INSERT:
			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];
				
				if (event.m_data != Equipement::GOLD)
				{
					static_cast<Player*>(object)->readItemComplete(cv);
				}
				else
				{
					int gold;
					cv->fromBuffer(gold);
					static_cast<Player*>(object)->getEquipement()->setGold(gold);
				}
				
			}
			else
			{
				return false;
			}
			break;
			
		case NetEvent::PLAYER_WAYPOINT_DISCOVERED:
			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];
				static_cast<Player*>(object)->addWaypoint(event.m_data,false);
			}
			break;

		case NetEvent::PLAYER_PARTY_CHANGED:
			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];

				static_cast<Player*>(object)->getParty()->removeMember(object->getId());
				char id;
				cv->fromBuffer(id);
				DEBUG5("player %i changed party to %i",object->getId(),id);
				World::getWorld()->getParty( id )->addMember(object->getId());

			}
			else
			{
				return false;
			}
			break;

		case NetEvent::PLAYER_PARTY_CANDIDATE:
			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];
				// bisherige Bewerbung entfernen
				id = static_cast<Player*>(object)->getCandidateParty();
				if (id >= 0)
				{
					getParty(id)->removeCandidate(object->getId());
				}
				char id;
				cv->fromBuffer(id);
				DEBUG5("player %i candidate for party  %i",object->getId(),id);
				World::getWorld()->getParty( id )->addCandidate(object->getId());

			}
			else
			{
				return false;
			}
			break;
			
		case NetEvent::FRACTION_RELATION_CHANGED:
			char tmp;
			cv->fromBuffer(tmp);
			setRelation(event.m_id, event.m_data, (Fraction::Relation) tmp);
			DEBUG("set relation %i %i to %i",event.m_id, event.m_data, (int) tmp);
			
			break;
			
		case NetEvent::PARTY_RELATION_CHANGED:
			char rel;
			cv->fromBuffer(rel);
			World::getWorld()->getParty( event.m_data )->setRelation(event.m_id, (Fraction::Relation) rel);
			DEBUG5("party %i changed relation to %i to %i",event.m_data, event.m_id, rel);
			break;
			
			
		case NetEvent::REGION_CUTSCENE:
			mode = (bool) event.m_data;
			if (region !=0)
			{
				region ->setCutsceneMode(mode);
			}
			break;
			
		case NetEvent::REGION_CAMERA:
			if (region !=0)
			{
				region->getCamera().fromString(cv);
			}
			break;
			
		case NetEvent::DIALOGUE_CREATED:
			if (region !=0)
			{
				region->createDialogueFromString(cv);
			}
			else
			{
				return false;
			}
			break;
			
		case NetEvent::DIALOGUE_DESTROYED:
			if (region !=0)
			{
				dia = region->getDialogue(event.m_id);
				if (dia != 0)
				{
					region->deleteDialogue(dia);
				}
			}
			else
			{
				return false;
			}
			break;
			
		case NetEvent::DIALOGUE_STAT_CHANGED:
			if (region !=0)
			{
				dia = region->getDialogue(event.m_id);
				if (dia != 0)
				{
					int id;
					cv->fromBuffer(id);
					dia->fromString(cv);
				}
			}
			else
			{
				return false;
			}
			break;
			
		case NetEvent::DAMAGE_VISUALIZER_CREATED:
			if (region != 0)
			{
				DamageVisualizer& dmgvis = region->getDamageVisualizer()[event.m_id];
				cv->fromBuffer(dmgvis.m_number);
				cv->fromBuffer(dmgvis.m_time);
				dmgvis.m_time -= cv->getDelay();
				cv->fromBuffer(dmgvis.m_size);
				cv->fromBuffer(dmgvis.m_position.m_x);
				cv->fromBuffer(dmgvis.m_position.m_y);	
				
			}
			break;
			
		default:
			ERRORMSG("unknown event type %i",event.m_type);

	}

	return true;
}


void World::handleDataRequest(ClientDataRequest* request, int slot )
{
	// Spieler von dem die Anfrage ausging
	WorldObject* player;

	// Spieler in dem betreffenden Slot aktivieren
	if (m_player_slots->count(slot)==0)
	{
		ERRORMSG("got request from an empty slot %i",slot);
		return;
	}
	else
	{
		player = (*m_player_slots)[slot];
	}
	
	Region* region = getRegion(request->m_region_id);
	if (request->m_data <= ClientDataRequest::REGION_ALL)
	{
		DEBUG5("Daten zur Region %i gefordert",request->m_id);
		
		if (region!=0)
		{
			// Daten der Region senden
			PackageHeader header;
			header.m_content = PTYPE_S2C_REGION;
			header.m_number =request->m_id;

			CharConv msg;
			header.toString(&msg);

			region->getRegionData(&msg);

			m_network->pushSlotMessage(msg.getBitStream(),slot);

			player->setState(WorldObject::STATE_ENTER_REGION,false);

		}

	}
	else if (request->m_data == ClientDataRequest::ITEM)
	{
		if (region!=0)
		{
			// Daten werden per Event aktualisiert
			NetEvent event;
			event.m_type = NetEvent::ITEM_DROPPED;
			event.m_id = request->m_id;

			region->insertNetEvent(event);
		}
	}
	else if (request->m_data == ClientDataRequest::OBJECT)
	{
		DEBUG("Daten zu Objekt %i gefordert",request->m_id);
		if (region!=0)
		{
			// Daten werden per Event aktualisiert
			NetEvent event;
			event.m_type = NetEvent::OBJECT_CREATED;
			event.m_id = request->m_id;

			region->insertNetEvent(event);
		}
	}
	else if (request->m_data == ClientDataRequest::PROJECTILE)
	{
		if (region!=0)
		{
			// Daten werden per Event aktualisiert
			NetEvent event;
			event.m_type = NetEvent::PROJECTILE_CREATED;
			event.m_id = request->m_id;

			region->insertNetEvent(event);
		}
	}
	
}

bool World::calcBlockmat(PathfindInfo * pathinfo)
{
	float sqs = pathinfo->m_base_size / pathinfo->m_quality;
	WorldObjectList ret;
	WorldObjectList::iterator it;
	int is,js;
	Shape s;
	s.m_center = pathinfo->m_center;
	s.m_type = Shape::RECT;
	s.m_extent.m_y =s.m_extent.m_x = 0.5*sqs * pathinfo->m_dim;

	// die beiden Ecken des Suchbereiches
	Vector c1 = pathinfo->m_center - s.m_extent;
	Vector c2 = pathinfo->m_center + s.m_extent;


	float hb = pathinfo->m_base_size * 0.5;

	DEBUG5("Calc blockmat %f %f", pathinfo->m_center.m_x,pathinfo->m_center.m_y);

	Shape s2;
	s2.m_type = Shape::CIRCLE;
	s2.m_radius = hb;

	Shape* wos=0;

	// Alle Objekte in dem Gebiet suchen (nur feste und lebende Objekte)
	getRegion(pathinfo->m_region)->getObjectsInShape(&s, &ret,pathinfo->m_layer, WorldObject::CREATURE | WorldObject::FIXED);
	WorldObject* wo=0;
	pathinfo->m_block->clear();

	// durchmustern der Objekte
	for (it = ret.begin(); it!=ret.end(); ++it)
	{
		wo = *it;
		// eigenes Objekt nicht mit einbeziehen
		if (wo->getId() != pathinfo->m_id)
		{
			if (wo->isCreature())
			{
				DEBUG5("found obstacle %i layer %i",wo->getId(),wo->getLayer());
			}
			wos = wo->getShape();
			DEBUG5("shape %i %f %f %f",wos->m_type,wos->m_radius,wos->m_extent.m_x,wos->m_extent.m_y);



			// Rand des Objektes mit X zeichnen

			// Mittelpunkt des Objektes im Grid
			js = (int) floor((wos->m_center.m_y - c1.m_y)/sqs);
			is = (int) floor((wos->m_center.m_x - c1.m_x)/sqs);
			is = std::max(std::min(is,pathinfo->m_dim-1),0);
			js = std::max(std::min(js,pathinfo->m_dim-1),0);

			// Form zum Testen auf Kollisionen
			s2.m_center.m_y = (js+0.5)*sqs+c1.m_y;
			s2.m_center.m_x = (is+0.5)*sqs+c1.m_x;


			DEBUG5("start %i %i %f %f",is,js,s2.m_center.m_x,s2.m_center.m_y);
			float dir[12][2] = {{sqs,sqs},{sqs,0},{sqs,-sqs},{0,-sqs},{-sqs,-sqs},{-sqs,0},{-sqs,sqs},{0,sqs},{sqs,sqs},{sqs,0},{sqs,-sqs},{0,-sqs}};
			int idir[12][2] = {{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1}};

			float x,y;
			// Vom Mittelpunkt des Hindernisses *nach unten* bis zum Rand gehen
			while (s2.m_center.m_y< c2.m_y && wos->intersects(s2))
			{
				s2.m_center.m_y += sqs;
				js++;
			}
			js--;
			s2.m_center.m_y -= sqs;

			// Wertigkeit des Hindernisses bestimmen
			char val = 9;
			if (wo->getType()== "MONSTER" || wo->getType() == "PLAYER")
			{
				if (wo->getSpeed().getLength() == 0)
				{
					// fest stehendes Lebewesen
					val = 6;
				}
				else
				{
					// sich bewegendes Lebewesen
					val = 4;
				}
			}
			// Hindernis eintragen
			if (*(pathinfo->m_block->ind(is,js))<val)
				*(pathinfo->m_block->ind(is,js)) = val;


			DEBUG5("start2 %i %i %f %f",is,js,s2.m_center.m_x,s2.m_center.m_y);

			// Hindernis dem Rand entlang umrunden
			// d ist ein Zaehler fuer die aktuelle Suchrichtung
			// wenn d 10 erreicht ist das Objekt einmal umrundet
			int d=1;
			while (d<10)
			{
				x = s2.m_center.m_x;
				y = s2.m_center.m_y;

				s2.m_center.m_x += dir[d][0];
				s2.m_center.m_y += dir[d][1];

				// solange Richtung weiterdrehen bis ein Punkt im Suchraum gefunden wird
				while ((d<=10) && (s2.m_center.m_x<c1.m_x || s2.m_center.m_y<c1.m_y ||
									s2.m_center.m_x>c2.m_x || s2.m_center.m_y>c2.m_y))
				{
					d++;
					s2.m_center.m_x = x+dir[d][0];
					s2.m_center.m_y = y+dir[d][1];
					DEBUG5("trying %f %f",s2.m_center.m_x,s2.m_center.m_y);
				}

				if (d==10)
					break;


				if (wos->intersects(s2))
				{
					// wenn in der aktuellen Richtung man im Objekt bleibt
					// Richtung testweise eins zurueck setzen
					// damit werden auch nicht konvexe Objekte korrekt behandelt
					DEBUG5("intersecting: %i %i %f %f",is+idir[d][0],js+idir[d][1],s2.m_center.m_x,s2.m_center.m_y);
					d--;
					s2.m_center.m_x = x+dir[d][0];
					s2.m_center.m_y = y+dir[d][1];
					if (s2.m_center.m_x<c1.m_x || s2.m_center.m_y<c1.m_y ||
									   s2.m_center.m_x> c2.m_x|| s2.m_center.m_y>c2.m_y || !wos->intersects(s2))
					{
						d++;
						s2.m_center.m_x = x+dir[d][0];
						s2.m_center.m_y = y+dir[d][1];
					}
				}
				else
				{
					// Suchrichtung solange weiter drehen bis ein zulaessiger Punkt im Objekt gefunden wurde
					DEBUG5("not intersecting: %i %i %f %f",is+idir[d][0],js+idir[d][1],s2.m_center.m_x,s2.m_center.m_y);
					do
					{
						d++;
						s2.m_center.m_x = x+dir[d][0];
						s2.m_center.m_y = y+dir[d][1];
					} while (d<=10 && (s2.m_center.m_x<c1.m_x || s2.m_center.m_y<c1.m_y ||
											s2.m_center.m_x>c2.m_x || s2.m_center.m_y>c2.m_y || !wos->intersects(s2)));

				}
				if (d<10)
				{
					// Hindernis eintragen
					DEBUG5("d: %i",d);
					is += idir[d][0];
					js += idir[d][1];
					DEBUG5("intersecting: %i %i %f %f",is,js,s2.m_center.m_x,s2.m_center.m_y);
					if (*(pathinfo->m_block->ind(is,js))<val)
						*(pathinfo->m_block->ind(is,js)) = val;

					/*
					std::cout << "  001122334455667788990011223344556677889900001122334455667788990011223344556677889900\n";

					for (int j=pathinfo->m_dim-1;j>=0;j--)
					{
					std::cout << j%10 << j%10;
					for (int i=0;i<pathinfo->m_dim;i++)
					{

					if (*(pathinfo->m_block->ind(i,j))!=0)
					{
					if (*(pathinfo->m_block->ind(i,j))=='X')
					std::cout << *(pathinfo->m_block->ind(i,j)) << *(pathinfo->m_block->ind(i,j));
					else
					std::cout <<(char) ( *(pathinfo->m_block->ind(i,j))+'0') << (char) (*(pathinfo->m_block->ind(i,j))+'0');
				}
					else
					std::cout << "  ";
				}
					std::cout << "\n";
				}
					std::cout << "---------------------\n";

					char c;
					std::cin >> c;
					*/
				}
			}
		}
	}

	/*
	std::cout << "  001122334455667788990011223344556677889900001122334455667788990011223344556677889900\n";

	for (int j=pathinfo->m_dim-1;j>=0;j--)
	{
	std::cout << j%10 << j%10;
	for (int i=0;i<pathinfo->m_dim;i++)
	{

	if (*(pathinfo->m_block->ind(i,j))!=0)
	{
	if (*(pathinfo->m_block->ind(i,j))=='X')
	std::cout << *(pathinfo->m_block->ind(i,j)) << *(pathinfo->m_block->ind(i,j));
	else
	std::cout <<(char) ( *(pathinfo->m_block->ind(i,j))+'0') << (char) (*(pathinfo->m_block->ind(i,j))+'0');
}
	else
	std::cout << "  ";
}
	std::cout << "\n";
}
	std::cout << "---------------------\n";
	*/

	return true;
}


bool World::calcPotential(PathfindInfo* pathinfo)
{
	//TODO: vordefinierte Felder benutzen

	float sqs = pathinfo->m_base_size / pathinfo->m_quality;
	float l = 0.5*sqs * pathinfo->m_dim;
	SearchField * sf = new SearchField(pathinfo->m_dim);
	sf->init(pathinfo->m_pot, pathinfo->m_block);
	int sx = (int) ((pathinfo->m_start.m_x - pathinfo->m_center.m_x + l) / sqs);
	int sy = (int) ((pathinfo->m_start.m_y - pathinfo->m_center.m_y + l) / sqs);
	DEBUG5("goal: %f %f",pathinfo->m_start.m_x,pathinfo->m_start.m_y);
	DEBUG5("Calc potential %i %i", sx,sy);

	sf->createPotential(sx,sy);
	delete sf;
	return true;

}



void World::calcPathDirection(PathfindInfo* pathinfo, Vector pos, Vector& dir)
{
	SearchField::getGradient(pathinfo, pos,dir);
}

void World::insertNetEvent(NetEvent &event)
{
	DEBUG5("new world Netevent %i %i type %i",event.m_id, event.m_data,event.m_type);
	m_events->push_back(event);

}


void  World::addEvent(RegionName rname, TriggerType trigger, Event* event)
{
	int id = getRegionId(rname);
	
	std::map<int, RegionData*>::iterator it;
	it =  m_region_data.find(id);

	if (it == m_region_data.end())
	{
		ERRORMSG("no data for region %s",rname.c_str());
		return;
	}
	
	DEBUG5("adding Event for Region %s",rname.c_str());
	it->second->addEvent(trigger,event);
}
		
WorldObject* World::getPlayer(int id)
{
	WorldObjectMap::iterator it;
	it = m_players->find(id);
	if (it != m_players->end())
	{
		return it->second;
	}
	return 0;
}

Party* World::getParty(int id)
{
	if (id >= Fraction::NEUTRAL_TO_ALL)
		return 0;
	
	if (id <0)
		return 0;
	
	if (id >= (int) m_parties.size())
		m_parties.resize(id+1);
		
	return &(m_parties[id]);
}


Fraction* World::getFraction(Fraction::Id id)
{
	std::map<Fraction::Id, Fraction*>::iterator it =  m_fractions.find(id);
	if (it == m_fractions.end())
		return 0;
	
	return it->second;
}

Fraction::Id World::getFractionId(Fraction::Type fractionname)
{
	
	if (fractionname == "player")
		return Fraction::PLAYER;
	
	if (fractionname.find("player_") != std::string::npos)
	{
		// Id aus dem String extrahieren
		int pos = fractionname.find("player_") + 7;
		
		std::stringstream stream;
		stream.str( fractionname.substr(pos));
		int nr;
		stream >> nr;
		return Fraction::PLAYER + nr;
	}
	
	if (fractionname == "DEFAULT" || fractionname=="default")
		return Fraction::DEFAULT;
	
	if (fractionname == "NEUTRAL_TO_ALL" || fractionname=="neutral_to_all")
		return Fraction::NEUTRAL_TO_ALL;
	
	if (fractionname == "HOSTILE_TO_ALL" || fractionname=="hostile_to_all")
		return Fraction::HOSTILE_TO_ALL;
	
	std::map<Fraction::Id, Fraction*>::iterator it;
	for (it = m_fractions.begin(); it != m_fractions.end(); ++it)
	{
		if (it->second->getType() == fractionname)
			return it->first;
	}
	
	return Fraction::NOFRACTION;
}
		
void World::createFraction(Fraction::Type name)
{
	// Fraktion mit dem gleichen Name oder gleicher Id zuerst loeschen
	Fraction::Id oldid = World::getFractionId(name);
	Fraction* frac = World::getFraction(oldid);
	if (frac != 0)
	{
		delete frac;
		m_fractions.erase(oldid);
		ERRORMSG("duplicate fraction with name %s",name.c_str());
	}
	

	Fraction::Id id = m_fractions.size() +1+ Fraction::MONSTER;
	
	frac = new Fraction(id, name);
	m_fractions[id] = frac;
}

