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

#include "OgreResourceGroupManager.h"

World* World::m_world=0;

/**
 * Constructors/Destructors
 */
/**
 *
 */
 World::World(bool server, bool cooperative)
{
	m_server = server;
	m_cooperative = cooperative;

	// diverse Initialisierungen

	m_player_slots = new WorldObjectMap;
	m_players = new WorldObjectMap;;

	// Baum fuer die Handelsvorgaenge anlegen
	m_trades = new std::map<int, Trade* >;
	m_trades->clear();


	m_max_nr_players =8;
	m_parties = new Party[m_max_nr_players];
	for (int i =0;i<m_max_nr_players;i++)
	{
		m_parties[i].init(i);
	}

	m_local_player =0;

	m_events = new NetEventList;

}


bool World::init()
{
	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	std::string file;
	
	EventSystem::init();
	Dialogue::init();

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
			worldloader.loadRegionData(file.c_str(), region_list);
		}

		std::list<RegionData*>::iterator rt;
		for (rt = region_list.begin(); rt != region_list.end();rt++)
		{
			registerRegionData(*rt, (*rt)->m_id);
		}
		
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
		
		files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("lua","*.lua");
		for (it = files->begin(); it != files->end(); ++it)
		{
			file = it->archive->getName();
			file += "/";
			file += it->filename;
			EventSystem::doFile(file.c_str());
		}
		
		DEBUG("server");
		m_network = new ServerNetwork(m_max_nr_players);
		
		

	}
	else
	{
		DEBUG("client");
		m_network = new ClientNetwork();
	}

	if( m_network->init( REQ_PORT )!=NET_OK )
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


void World::createRegion(short region)
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

		Region* reg = MapGenerator::createRegion(rdata);
		DEBUG5("region created %p for id %i",reg,region);

		// Debugging
		reg->setReviveLocation("region0","entry_south");
		insertRegion(reg,region);

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


		// Tiles Setzen
		for (int i=1;i<5;i++)
		{
			reg->setTile(TILE_GRASS,5,i);
		}

		for (int i=7;i<10;i++)
		{
			reg->setTile(TILE_WATER,i,6);
		}


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


		/*
		WorldObject* wo=0;
		wo = new Spawnpoint("goblins", World::getWorld()->getValidId());
		reg->insertObject(wo, Vector(15,10));


		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		reg->insertObject(wo, Vector(13,8));




		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "gob_dog");
		reg->insertObject(wo, Vector(14.2,8.2));

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		reg->insertObject(wo, Vector(14.5,6.7));

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		reg->insertObject(wo, Vector(14.8,5.7));

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		reg->insertObject(wo, Vector(14,10.9));

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		reg->insertObject(wo, Vector(17,8.2));



		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		reg->insertObject(wo, Vector(18.5,7));

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		reg->insertObject(wo, Vector(17.2,6.6));

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		reg->insertObject(wo, Vector(19.9,6.9));
		*/

/*
		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertObject(wo, Vector(16.2,3),rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertObject(wo, Vector(19.2,3.1),rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertObject(wo, Vector(6.5,6.4),rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		insertObject(wo, Vector(8.5,6.4),rid);
		*/

	}
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

	std::map<int, Trade* >::iterator j;
	for (j=m_trades->begin(); j!=m_trades->end();j++)
	{
		delete j->second;
	}
	delete m_trades;
	
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


	delete[] m_parties;
	delete m_player_slots;
	delete m_players;
	delete m_events;
	
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

WorldObject::Relation World::getRelation(WorldObject::Fraction frac, WorldObject::Fraction frac2)
{
	if (frac > frac2)
		return getRelation(frac2,frac);
	
	if (frac == WorldObject::FRAC_HOSTILE_TO_ALL || frac2 == WorldObject::FRAC_HOSTILE_TO_ALL)
		return WorldObject::HOSTILE;
	
	if (frac == frac2)
		return WorldObject::ALLIED;
	
	if (frac == WorldObject::NOFRACTION)
		return WorldObject::NEUTRAL;
	
	if (frac<=  WorldObject::FRAC_MONSTER && frac2<=  WorldObject::FRAC_MONSTER)
	{
		// Beziehung zwischen zwei Nicht -Spieler Parteien
		return WorldObject::NEUTRAL;
	}
	else if (frac<=  WorldObject::FRAC_MONSTER)
	{
	
		// Beziehung zwischen Spieler und Nicht-Spieler Partei
		WorldObject::Relation data[7] = {WorldObject::NEUTRAL, WorldObject::ALLIED, WorldObject::HOSTILE,WorldObject::HOSTILE,  WorldObject::NEUTRAL, WorldObject::HOSTILE,WorldObject::HOSTILE};
		
		return data[frac];
	}
	else
	{
		// Beziehung zwischen Spielern
		return std::min(m_parties[frac - WorldObject::FRAC_PLAYER_PARTY].getRelations()[frac2- WorldObject::FRAC_PLAYER_PARTY], m_parties[frac2 - WorldObject::FRAC_PLAYER_PARTY].getRelations()[frac- WorldObject::FRAC_PLAYER_PARTY]);
	}
	
	
}

WorldObject::Relation World::getRelation(WorldObject::Fraction frac, WorldObject* wo)
{
	WorldObject::Fraction f = wo->getFraction();
	return getRelation(frac,f);
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


Trade* World::getTrade ( int id)
{
	std::map<int,Trade*>::iterator iter;

	//Handel suchen
	iter = m_trades->find(id);

	// Testen ob gefunden
	if (iter == m_trades->end())
	{
		// keins gefunden, NULL ausgeben
		return 0;
	}
	else
	{
		// Zeiger auf Objekt ausgeben
		return iter->second;
	}
}


int World::newTrade(int trader1_id, int trader2_id)
{
	// zufällige ID erzeugen;
	int id = 0;
	Trade* trade;
	while (id == 0)
	{
		id=rand();
		trade=getTrade(id);
		if (trade!=0)
		{
			id=0;
		}
	}

	// Neues Trade Objekt anlegen
	trade = new Trade(id, trader1_id,trader2_id);
	if (trade ==0)
	{
		return 0;
	}

	m_trades->insert(std::make_pair(id,trade));
	// ID ausgeben
	return id;
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

	Region* reg = getRegion(region);

	// Testen ob alle Daten vorhanden sind
	int data_missing =0;
	if (reg ==0)
	{
		data_missing =1;
	}

	player->getGridLocation()->m_region = region;


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
				createRegion(region);
			}

			// ein lokaler Spieler kann jetzt in die erzeugte region
			// bei einem Client muss dieser erst noch die Daten anfordern die ihm fehlen
			if (player == m_local_player)
			{
				// Spieler in die Region einfuegen
				player->setState(WorldObject::STATE_ENTER_REGION);
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
				player->setState(WorldObject::STATE_REGION_DATA_WAITING);
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
					player->setState(WorldObject::STATE_INACTIVE);
				}
				else
				{
					// Spieler in die Region einfuegen
					player->setState(WorldObject::STATE_ENTER_REGION);
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
			pos = reg->getLocation(m_region_enter_loc[player->getId()] );
			m_region_enter_loc.erase(player->getId());
		}
		reg->getFreePlace(player->getShape(),player->getLayer() , pos, player);
		reg->insertObject(player, pos,player->getShape()->m_angle);
		DEBUG5("entry position %f %f",pos.m_x, pos.m_y);

		player->setState(WorldObject::STATE_ACTIVE);

		// bisheriges Kommando abbrechen
		static_cast<Creature*>(player)->clearCommand(false);
		static_cast<Creature*>(player)->getAction()->m_type = Action::NOACTION;

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
	pl= new Player(getValidId(),"");
	pl->fromSavegame(cv);
	
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

		pl->setState(WorldObject::STATE_ACTIVE);

		// Debugging: Region, Koordinaten setzen
		pl->getGridLocation()->m_region =0;
		pl->getShape()->m_center = Vector(9,10);

		insertPlayerIntoRegion(pl,pl->getGridLocation()->m_region, "entry_south");


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

			if (slot != LOCAL_SLOT)
			{
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
			}
		}
	}


}


void World::handleCommand(ClientCommand* comm, int slot, float delay)
{
	DEBUG5("Kommando (%f %f) button: %i id: %i action: %i",comm->m_goal.m_x,comm->m_goal.m_y,comm->m_button, comm->m_id,comm->m_action);


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
	if (m_server)
	{
		// Name des Senders an die Nachricht haengen
		smsg = "[";
		if (m_player_slots->count(slot)>0)
		{
			smsg += (*m_player_slots)[slot]->getName();
		}
		smsg += "] ";
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

	if (msg[0] == '$' && m_server)
	{
		// Cheatcode eingegeben
		Player* pl = static_cast<Player*>((*m_player_slots)[slot]);

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
				pl->insertItem(itm);
			}
		}
		else if (obj == "get")
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
		else if (obj == "set")
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
			EventSystem::doString((char*) instr.c_str());
			std::string ret = EventSystem::getReturnValue();
			if (ret != "")
			{
				static_cast<Player*>(m_local_player)->addMessage(ret);
			}
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
	float timer_max[3];
	timer_max[0] = 200; timer_max[1] = 500; timer_max[2] = 1000;
	for (int i=0; i<3; i++)
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

	// Durchmustern alle Handelsvorgänge
	std::map<int,Trade*>::iterator iter2;
	Trade* trade=0;

	for (iter2 =m_trades->begin(); iter2!=m_trades->end();)
	{
		trade = iter2->second;
		DEBUG5("Trades behandeln %p\n\n",trade);
		// Testen ob der Handel gelöscht werden kann

		if (trade->getFinished() == true)
		{
			if (trade->getSuccessful() == true)
			{
				// Handel war erfolgreich, löschen wenn beide Spieler ihre Gegenstaende erhalten haben
				if (trade->getGotItems(0)==true && trade->getGotItems(1)==true)
				{
					m_trades->erase(iter2++);
					delete trade;
					continue;
				}
			}
			else
			{
				// Handel war nicht erfolgreich, sofort löschen
				m_trades->erase(iter2++);
				delete trade;
				continue;
			}
		}
		++iter2;
	}
	DEBUG5("Trades behandeln abgeschlossen");

	m_network->update();


	if (m_server)
	{
		updateLogins();
		acceptLogins();
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
				if (m_parties[i].getCandidates().count(pl->getId()) ==1)
				{
					m_parties[i].removeCandidate(pl->getId());
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
			pl->setState(WorldObject::STATE_REGION_DATA_WAITING);

			// fehlende Daten zur Region anfordern
			PackageHeader header;
			header.m_content = PTYPE_C2S_DATA_REQUEST; 	// Data Request von Client zu Server
			header.m_number =1;

			ClientDataRequest datareq;
			datareq.m_data = ClientDataRequest::REGION_ALL;
			datareq.m_id = pl->getGridLocation()->m_region;

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
				insertPlayerIntoRegion(pl,pl->getGridLocation()->m_region, "entry_south");
				pl->setState(WorldObject::STATE_ACTIVE);
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

					/*
					timeval tv;
					gettimeofday(&tv, NULL);
					DEBUG("timestamp %i delay %i  system time %i",cv->getTimestamp(), cv->getDelay(),tv.tv_usec/1000);
					*/

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
						char tmp;
						cv->fromBuffer(tmp);

						// Subtyp
						char subt[11];
						subt[10] ='\0';
						cv->fromBuffer(subt,10);

						int id;
						cv->fromBuffer(id);
						WorldObject* player;

						DEBUG("got data for player %s id %i",subt,id);

						// Spieler entweder neu anlegen oder aus den existierenden herraussuchen
						if (m_players->count(id)==0)
						{
							// Spieler existiert noch nicht
							player = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_PLAYER, std::string(subt),id);
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

				if (headerp.m_content == PTYPE_S2C_REGION)
				{
					// Daten zu einer Region erhalten
					DEBUG("got data for region %i",headerp.m_number);
					short dimx, dimy;

					// Groesse der Region
					cv->fromBuffer(dimx);
					cv->fromBuffer(dimy);

					// TODO: Name setzen
					Region* reg = getRegion(headerp.m_number);
					if (reg ==0)
					{
						reg = new Region(dimx,dimy,headerp.m_number,"");
					}
					else
					{
						// Daten loeschen
					}

					// Daten schreiben
					reg->setRegionData(cv,m_players);

					// lokalen Spieler fuer die Region freischalten
					m_local_player->setState(WorldObject::STATE_ENTER_REGION);

					// Region einfuegen
					if (getRegion(headerp.m_number) ==0)
					{
						insertRegion(reg, headerp.m_number);
					}
				}

				if (headerp.m_content == PTYPE_S2C_INITIALISATION)
				{
					int id;
					cv->fromBuffer(id);
					DEBUG("ID at server %i",id);
					m_players->clear();
					m_local_player->setId(id);

					char frac;
					cv->fromBuffer(frac);
					m_local_player->setFraction((WorldObject::Fraction) frac);
					DEBUG("fraction %i",frac);

					insertPlayer(m_local_player, LOCAL_SLOT);
				}

				if (headerp.m_content == PTYPE_S2C_PARTY)
				{
					int id = headerp.m_number;
					m_parties[id].fromString(cv);
				}

				if (headerp.m_content == PTYPE_S2C_REGION_CHANGED)
				{
					DEBUG("notified that region changed to %i",headerp.m_number);
					// der lokale Spieler hat die Region gewechselt
					// fehlende Daten anfordern
					if (m_local_player->getRegion()!=0)
					{
						m_local_player->getRegion()->deleteObject(m_local_player);
					}
					m_local_player->setState(WorldObject::STATE_REGION_DATA_REQUEST);
					m_local_player->getGridLocation()->m_region = headerp.m_number;
				}

				if (headerp.m_content == PTYPE_S2C_EVENT)
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

				if (headerp.m_content == PTYPE_S2C_MESSAGE)
				{
					char* buf = new char[headerp.m_number+1];
					buf[headerp.m_number] = 0;
					cv->fromBuffer(buf, headerp.m_number);

					static_cast<Player*>(m_local_player)->addMessage(buf);

					delete buf;
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
				if (pl->getState() != WorldObject::STATE_ACTIVE && pl->getState() != WorldObject::STATE_DEAD && pl->getState() != WorldObject::STATE_DIEING)
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
					writeNetEvent(reg,&(*lt),msg);
					m_network->pushSlotMessage(msg->getBitStream(),slot);
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
			DEBUG5("object created %s %i",object->getTypeInfo()->m_subtype.c_str(), object->getId());
			object->toString(cv);
		}

		if (event->m_type == NetEvent::OBJECT_STAT_CHANGED)
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

		if (event->m_type == NetEvent::PROJECTILE_CREATED)
		{
			proj = region->getProjectile(event->m_id);
			proj->toString(cv);
		}

		if (event->m_type == NetEvent::PROJECTILE_STAT_CHANGED)
		{
			proj = region->getProjectile(event->m_id);
			if (proj !=0)
			{
				proj->writeNetEvent(event,cv);
			}
			else
				return false;
		}

		if (event->m_type == NetEvent::ITEM_DROPPED)
		{
			DropItem* di;
			di = region->getDropItem(event->m_id);
			if (di !=0)
			{
				cv->toBuffer(di->m_x);
				cv->toBuffer(di->m_y);
				di->m_item->toString(cv);
			}
			else
				return false;
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

	if (event->m_type == NetEvent::PLAYER_ITEM_EQUIPED)
	{
		object = (*m_players)[event->m_id];
		if (object != 0)
		{
			cv->toBuffer<short>((short) event->m_data);
			static_cast<Player*>(object)->getEquipement()->getItem(event->m_data)->toString(cv);


		}
		else
			return false;

	}

	if (event->m_type == NetEvent::PLAYER_ITEM_PICKED_UP)
	{
		object = (*m_players)[event->m_id];
		if (object != 0)
		{
			if (event->m_data != Equipement::GOLD)
			{
				if (static_cast<Player*>(object)->getEquipement()->getItem(event->m_data) ==0)
					ERRORMSG("no item at pos %i",event->m_data);
				cv->toBuffer<short>((short) event->m_data);
				static_cast<Player*>(object)->getEquipement()->getItem(event->m_data)->toStringComplete(cv);
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
		DEBUG("removing item %i",event->m_id);
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
	Projectile* proj;
	int id;
	bool mode;

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
			if (region !=0)
			{
				region->deleteObject(object);
			}
			delete object;

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
				object->getGridLocation()->m_region = event.m_data;

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

		case NetEvent::PLAYER_NOITEM_EQUIPED:

			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];
				if (object != m_local_player)
				{
					Item* item =0;
					static_cast<Player*>(object)->getEquipement()->swapItem(item,event.m_data);

					if (item !=0)
						delete item;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			break;


		case NetEvent::PLAYER_ITEM_EQUIPED:
			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];
				if (object != m_local_player)
				{
					static_cast<Player*>(object)->readItem(cv);
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			break;

		case NetEvent::PLAYER_ITEM_PICKED_UP:
			if (m_players->count(event.m_id)>0)
			{
				object = (*m_players)[event.m_id];
				if (object == m_local_player)
				{
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
			}
			else
			{
				return false;
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
		case NetEvent::PARTY_RELATION_CHANGED:
			char rel;
			cv->fromBuffer(rel);
			World::getWorld()->getParty( event.m_data )->setRelation(event.m_id, (WorldObject::Relation) rel);
			DEBUG("party %i changed relation to %i to %i",event.m_data, event.m_id, rel);
			break;
			
		case NetEvent::REGION_CUTSCENE:
			mode = (bool) event.m_data;
			if (region !=0)
			{
				region ->setCutsceneMode(mode);
			}

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

	if (request->m_data <= ClientDataRequest::REGION_ALL)
	{
		DEBUG5("Daten zur Region %i gefordert",request->m_id);
		Region* region = getRegion(request->m_id);

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

			player->setState(WorldObject::STATE_ENTER_REGION);

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

	// Alle Objekte in dem Gebiet suchen
	getRegion(pathinfo->m_region)->getObjectsInShape(&s, &ret,pathinfo->m_layer);
	WorldObject* wo=0;
	pathinfo->m_block->clear();

	// durchmustern der Objekte
	for (it = ret.begin(); it!=ret.end(); ++it)
	{
		wo = *it;
		// eigenes Objekt nicht mit einbeziehen
		if (wo->getId() != pathinfo->m_id)
		{
			DEBUG5("found obstacle %i",wo->getId());
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
			if (wo->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_MONSTER || wo->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
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

