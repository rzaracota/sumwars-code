#include "server.h"

Server::Server()
	: m_logins()
{
	m_world =0;
	m_network=0;
	m_stop = false;
}

Server::~Server()
{
}

void Server::init(int max_slots)
{
	ObjectFactory::init();

	m_network = new ServerNetwork(max_slots);
	if( m_network->init( REQ_PORT )!=NET_OK )
	{
		ERRORMSG( "Error occured in network" );
		return ;
	}

	long sec;
	time(&sec);
	srand(sec);

	m_world=new World();
	m_world->setNetwork(m_network);

	ObjectFactory::m_world = m_world;

	int gen_type=2	;

	if (gen_type==1)
	{

	}
	else if(gen_type==2)
	{
		Region* reg = new Region(25,25);
		short rid = m_world->insertRegion(reg);




		// Objekte anlegen
		ServerWObject* wo=0,*wo2=0;
		Creature* cr =0;
		CreatureBaseAttr* basm =0;
		WorldObject::Geometry* wob=0;
		Shape* sp=0;
		Spawnpoint* spawn=0;

		// Tiles Setzen
		for (int i=1;i<5;i++)
		{
			reg->setTile(TILE_GRASS,5,i);
		}

		for (int i=7;i<10;i++)
		{
			reg->setTile(TILE_WATER,i,6);
		}

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "fence");
		m_world->insertSWObject(wo, 4,6,rid);


		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "tree");
		m_world->insertSWObject(wo, 9,13,rid);


		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 13,8,rid);


		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "gob_dog");
		m_world->insertSWObject(wo, 14.2,8.2,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 14.5,6.7,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 14.8,5.7,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 14,10.9,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 17,8.2,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "lich");
		m_world->insertSWObject(wo, 7,5,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 18.5,7,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 17.2,6.6,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 19.9,6.9,rid);

/*
		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 16.2,3,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 19.2,3.1,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 6.5,6.4,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_MONSTER, "goblin");
		m_world->insertSWObject(wo, 8.5,6.4,rid);
		*/
/*

		Spawnpoint::SpawnedMonsterGroup mg("goblin",1.0,1);
		spawn = new Spawnpoint(m_world,1);
		spawn->setRespawnTime(10000);
		spawn->addMonsterGroup(mg);
		m_world->insertSWObject(spawn,9,9,rid);
*/
/*
		int i,j;
		for (i=1;i<48;i++)
		{
			for (int j=1;j<48;j++)
			{
				reg->setTile(TILE_GRASS,i,j);
			}
		}

		for (i=0;i<49;i++)
		{
			reg->setTile(TILE_WATER,i,0);
			reg->setTile(TILE_WATER,i,49);
			reg->setTile(TILE_WATER,49,i);
			reg->setTile(TILE_WATER,0,i);

		}

		Spawnpoint::SpawnedMonsterGroup gob("goblin",0.6,6);
		Spawnpoint::SpawnedMonsterGroup gob2("goblin",0.5,3);
		Spawnpoint::SpawnedMonsterGroup lich("lich",0.7,1);
		Spawnpoint::SpawnedMonsterGroup dog("gob_dog",0.4,4);

		spawn = new Spawnpoint(m_world,1);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob);
		m_world->insertSWObject(spawn,9,9,rid);

		spawn = new Spawnpoint(m_world,2);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob);
		m_world->insertSWObject(spawn,6,18,rid);

		spawn = new Spawnpoint(m_world,3);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob2);
		spawn->addMonsterGroup(dog);
		m_world->insertSWObject(spawn,22,14,rid);

		spawn = new Spawnpoint(m_world,4);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob);
		spawn->addMonsterGroup(lich);
		m_world->insertSWObject(spawn,16,25,rid);

		spawn = new Spawnpoint(m_world,5);
		spawn->setRespawnTime(100000);
		spawn->addMonsterGroup(gob2);
		spawn->addMonsterGroup(lich);
		spawn->addMonsterGroup(dog);
		m_world->insertSWObject(spawn,30,27,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "tree");
		m_world->insertSWObject(wo, 24,26,rid);


		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "tree");
		m_world->insertSWObject(wo, 10,13,rid);

		wo = ObjectFactory::createObject(WorldObject::TypeInfo::TYPE_FIXED_OBJECT, "tree");
		m_world->insertSWObject(wo, 20,18,rid);
*/

	}
	else if(gen_type==3)
	{


			//MapGenerator::createMap(m_world);
		printf("World  created\n");

	}
}

void Server::loop()
{
	float  dt=0.f;
	int i=0;
	float s=0;

	Timer timer;

	while( !m_stop )
	{
		DEBUG5("new_login_player");
		updateLogins();
		acceptLogins();

		DEBUG5("update");
		timer.start();
		m_world->update( 25.f );
		dt = timer.getTime();
		s += dt;
		if (dt >5)
		{
            DEBUG5("update zeit in ms: %f",dt);
		}
		i++;
		if (i==100)
		{
			DEBUG5("durchschnittliche updatezeit is ms: %f",s/100);
			s=0;
			i=0;
		}

		if( 25.f-dt>0.f )
		{
#ifdef WIN32
			Sleep( (int)(25.f-dt) );
#else
			usleep( (int)((25.f-dt)*1000) );
#endif
		}
		else
		{
			//WARNING("server is too slow!");
		}

		// Debugging
		//usleep( 1000000 );

		DEBUG5("network->update()");
		for( int i=0 ; i<1 ; i++ )
			m_network->update();

	}

    DEBUG("server thread end");
}

void Server::stop()
{
 	m_stop = true;
 	m_network->kill();

    pthread_join(m_server_thread,NULL);

	if( m_world )
	{
		delete m_world;
	}

	if (m_network)
	{

		delete m_network;
	}
}

void Server::acceptLogins()
{
	int login_slot;

	while((login_slot=m_network->popNewLoginSlot()) != -1 )
	{
		INFO( "user login detected: %i",  login_slot  );
		m_logins.push_back(login_slot);


	}
}

void Server::updateLogins()
{
	list<int>::iterator i;
	Packet* data;
	int lend;
	ClientHeader header;
	DEBUG5("update logins");
	for (i=m_logins.begin();i!=m_logins.end();)
	{
		DEBUG5("testing slot %i",(*i));
		if (m_network->numberSlotMessages((*i) )>0)
		{
			DEBUG5("got Package");
			m_network->popSlotMessage( (*i),data);
			CharConv cv(data);
			header.fromString(&cv);
			if (header.m_content == PTYPE_C2S_SAVEGAME)
			{
				DEBUG("got savegame from slot %i",(*i));
				char binsave;
				cv.fromBuffer<char>(binsave);
				short version;
				cv.fromBuffer<short>(version);
				int len;
				cv.fromBuffer<int>(len);
				WorldObject::TypeInfo::ObjectSubtype ot;
				char tmp[11];
				tmp[10] = '\0';
				cv.fromBuffer(tmp,10);
				for (int u=0;u<10;u++)
					printf("%i ",tmp[u]);
				ot = tmp;
				Player* pl =0;

				if (ot == "warrior")
				{
					DEBUG5("warrior");
					pl=new Warrior( m_world, m_world->getValidId() );
				}
				else if (ot == "mage")
				{
					DEBUG5("mage");
					pl=new Mage( m_world, m_world->getValidId() );
				}
				else if (ot == "archer")
				{
					DEBUG5("archer");
					pl=new Archer( m_world, m_world->getValidId() );
				}
				else if (ot == "priest")
				{
					DEBUG5("priest");
					pl=new Priest( m_world, m_world->getValidId() );
				}
				else
				{
					ERRORMSG("invalid savegame type: %s",ot.c_str());
					pl=new Warrior( m_world, m_world->getValidId() );
				}

				if (pl!=0)
				{
					DEBUG5("adress of players %p",pl);
					//pl->fromSavegame(bp2);
					pl->setNetworkSlot( (*i) );
					m_world->insertSWObject(pl,6,11,0);
					pl->getDynAttr()->m_health=150;

					//pl->getDynAttr()->m_status_mod_time[Damage::CONFUSED] = 100000;
					//pl->getDynAttr()->m_status_mod_time[Damage::BERSERK] = 100000;

				}
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

void* Server::startServer(void* arg)
{
	StartArgs* argp = (StartArgs*) arg;

	argp->m_server->loop();
}

void Server::run()
{
	StartArgs* arg = new StartArgs;
	arg->m_server = this;

	pthread_create( &m_server_thread, NULL, &Server::startServer, arg );
}




