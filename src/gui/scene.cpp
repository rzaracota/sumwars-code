#include "scene.h"

#define USE_OBJECTLOADER
#define USE_ITEMLOADER

std::map<Projectile::ProjectileType, RenderInfo> Scene::m_projectile_render_info;

std::map<WorldObject::TypeInfo::ObjectSubtype, RenderInfo> Scene::m_object_render_info;

std::map<Item::Subtype, RenderInfo> Scene::m_item_render_info;

std::map<Tile, RenderInfo> Scene::m_tile_render_info;

std::map<WorldObject::TypeInfo::ObjectSubtype, std::map<Action::ActionType, std::vector<std::string> > > Scene::m_object_animations;

std::map<PlayerLook, RenderInfo> Scene::m_player_render_info;

std::multimap< WorldObject::TypeInfo::ObjectSubtype, std::pair<bool, PlayerLook> > Scene::m_player_look;


Scene::Scene(Document* doc,Ogre::RenderWindow* window)
{
	m_document = doc;
	m_window = window;
	m_scene_manager = Ogre::Root::getSingleton().getSceneManager("DefaultSceneManager");


	// Kamera anlegen
	m_camera = m_scene_manager->createCamera("camera");
	m_camera->setPosition(Ogre::Vector3(0, 1500, 1500));
	m_camera->lookAt(Ogre::Vector3(0,0,0));
	m_camera->setNearClipDistance(5);


	// Viewport anlegen, Hintergrundfarbe schwarz
	m_viewport = m_window->addViewport(m_camera);
	m_viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	m_camera->setAspectRatio(Ogre::Real(m_viewport->getActualWidth()) / Ogre::Real(m_viewport->getActualHeight()));

	m_objects = new std::map<int,std::string>;

	m_drop_items = new std::map<int,std::string>;

	m_projectiles = new std::map<int,std::string>;

	registerMeshes();

	m_region_id = -1;

}

Scene::~Scene()
{
	clearObjects();
	delete m_objects;

	delete m_drop_items;

	delete m_projectiles;
}

void Scene::loadItemData(std::string file)
{
	// Item Meshes aus XML-Datei Laden
	ItemLoader* itemloader = 0;
	itemloader = new ItemLoader;

	std::list<ItemMeshData*>* item_mesh_list;
	item_mesh_list = itemloader->loadItemMeshData(file.c_str());

	if (item_mesh_list != 0)
	{
		// Daten auslesen und registrieren
		std::list<ItemMeshData*>::iterator iter = item_mesh_list->begin();
		while (iter != item_mesh_list->end())
		{
			registerItem((*iter)->m_subtype, (*iter)->m_mesh);
			*iter++;
		}

		// Liste aus Speicher loeschen
		iter = item_mesh_list->begin();
		while (iter != item_mesh_list->end())
		{
			delete *iter;
			*iter++;
		}
	}

	delete item_mesh_list;
	item_mesh_list = 0;
	delete itemloader;
	itemloader = 0;
}

void Scene::loadMonsterData(std::string file)
{
	// Monster Meshes aus XML-Datei Laden
	ObjectLoader* objectloader = 0;
	objectloader = new ObjectLoader;

	std::list<MonsterMeshData*>* monster_mesh_list;
	monster_mesh_list = objectloader->loadMonsterMeshData(file.c_str());

	if (monster_mesh_list != 0)
	{
		// Daten auslesen und registrieren
		std::list<MonsterMeshData*>::iterator iter = monster_mesh_list->begin();
		while (iter != monster_mesh_list->end())
		{
			registerObject((*iter)->m_subtype, (*iter)->m_mesh, "");
			registerObjectAnimations((*iter)->m_subtype,(*iter)->m_animations); 
			*iter++;
		}

		// Liste aus Speicher loeschen
		iter = monster_mesh_list->begin();
		while (iter != monster_mesh_list->end())
		{
			delete *iter;
			*iter++;
		}
	}

	delete monster_mesh_list;
	monster_mesh_list = 0;
	delete objectloader;
	objectloader = 0;
}

void Scene::loadFixedObjectData(std::string file)
{
	// FixedObject Meshes aus XML-Datei Laden
	ObjectLoader* objectloader = 0;
	objectloader = new ObjectLoader;

	std::list<FixedObjectMeshData*>* fixed_object_mesh_list;
	fixed_object_mesh_list = objectloader->loadFixedObjectMeshData(file.c_str());

	if (fixed_object_mesh_list != 0)
	{
		// Daten auslesen und registrieren
		std::list<FixedObjectMeshData*>::iterator iter = fixed_object_mesh_list->begin();
		while (iter != fixed_object_mesh_list->end())
		{
			registerObject((*iter)->m_subtype, (*iter)->m_mesh, "");
			*iter++;
		}

		// Liste aus Speicher loeschen
		iter = fixed_object_mesh_list->begin();
		while (iter != fixed_object_mesh_list->end())
		{
			delete *iter;
			*iter++;
		}
	}

	delete fixed_object_mesh_list;
	fixed_object_mesh_list = 0;
	delete objectloader;
	objectloader = 0;
}


void Scene::registerMeshes()
{
	// Meshes fuer Objekte registrieren
	registerPlayerLook("warrior","warrior_m",true);
	registerPlayerLook("mage","mage_m",true);
	registerPlayerLook("priest","priest_f",false);
	registerPlayerLook("archer","archer_f",false);
	
	registerPlayer("warrior_m","warrior_m.mesh");
	registerPlayer("mage_m","mage_m.mesh");
	registerPlayer("priest_f","priest_f.mesh");
	registerPlayer("archer_f","archer_f.mesh");
	
	
	// Spieler
	registerObject("warrior","warrior_m.mesh","");
	//registerAttachedMesh("warrior","itemRightHand","sword.mesh");
	registerObject("mage","mage_m.mesh","");	// TODO
	registerObject("priest","priest_f.mesh",""); // TODO
	registerObject("archer","archer_f.mesh",""); // TODO

	// Monster
	registerObject("gob_dog","gobDog.mesh","");
	registerObject("lich","lich.mesh","");

	// feste Objekte

	registerObject("smallWall1","smallWall1.mesh","");
	registerObject("smallWall2","smallWall2.mesh","");
	registerObject("stones3","stones3.mesh","");

	// Geschosse
	registerProjectile(Projectile::ARROW,"arrow.mesh","");
	registerProjectile(Projectile::MAGIC_ARROW,"","Magic_Arrow");
	registerProjectile(Projectile::FIRE_BOLT,"","Firebolt");
	registerProjectile(Projectile::FIRE_BALL,"","Fireball");
	registerProjectile(Projectile::FIRE_WALL,"","Firewall");
	registerProjectile(Projectile::FIRE_WAVE,"","Firewave");
	registerProjectile(Projectile::ICE_BOLT,"","Icebolt");
	registerProjectile(Projectile::BLIZZARD,"","Blizzard");
	registerProjectile(Projectile::ICE_RING,"","Icering");
	registerProjectile(Projectile::FREEZE,"","Freeze");
	registerProjectile(Projectile::LIGHTNING,"","Lightning");
	registerProjectile(Projectile::THUNDERSTORM,"","Thunderstorm");
	registerProjectile(Projectile::CHAIN_LIGHTNING,"","Chainlightning");
	registerProjectile(Projectile::STATIC_SHIELD,"","Static_Shield");
	registerProjectile(Projectile::FIRE_ARROW,"arrow.mesh","Fire_Arrow");
	registerProjectile(Projectile::ICE_ARROW ,"arrow.mesh","Ice_Arrow");
	registerProjectile(Projectile::WIND_ARROW,"arrow.mesh","Wind_Arrow");
	registerProjectile(Projectile::GUIDED_ARROW,"arrow.mesh","Guided_Arrow");
	registerProjectile(Projectile::EXPLOSION,"","Explosion");
	registerProjectile(Projectile::FIRE_EXPLOSION,"","Fire_Explosion");
	registerProjectile(Projectile::ICE_EXPLOSION,"","Ice_Explosion");
	registerProjectile(Projectile::WIND_EXPLOSION,"","Wind_Explosion");
	registerProjectile(Projectile::LIGHT_BEAM,"","Lightbeam");
	registerProjectile(Projectile::ELEM_EXPLOSION,"","ElemExplosion");
	registerProjectile(Projectile::ACID,"","Acid");
	registerProjectile(Projectile::DIVINE_BEAM,"","Divine_Beam");
	registerProjectile(Projectile::HYPNOSIS,"","Hypnosis");


	registerItem("long_sw","sword.mesh");
	registerItem("wood_bow","shortbow.mesh");
	registerItem("long_bow","shortbow.mesh");
	registerItem("fire_bow","shortbow.mesh");
	registerItem("battle_axe","battleaxe.mesh");
	registerItem("holy_flail","warhammer.mesh");
	registerItem("ice_wand","basicWand.mesh");
	registerItem("ice_staff","basicStaff.mesh");

	registerItem("leath_arm","armor.mesh");
	registerItem("tiled_arm","armor.mesh");
	registerItem("heavy_arm","armor.mesh");

	registerItem("wood_sh","buckler.mesh");
	registerItem("iron_sh","shield.mesh");

	registerItem("steel_hlm","helmet.mesh");
	registerItem("divine_hlm","helmet.mesh");

	registerItem("heal_1","potion.mesh");
	registerItem("heal_2","potion.mesh");
	registerItem("heal_bl","potion.mesh");
	registerItem("heal_fr","potion.mesh");
}

void Scene::registerObject(WorldObject::TypeInfo::ObjectSubtype subtype, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_object_render_info.insert(std::make_pair(subtype,RenderInfo(mesh,particle_system,scaling_factor)));
}

void Scene::registerObjectAnimations(WorldObject::TypeInfo::ObjectSubtype subtype, std::map<Action::ActionType, std::vector<std::string> > &animations)
{
	if (!animations.empty())
	{
		m_object_animations[subtype] = animations;
	}
}

void Scene::registerAttachedMesh(WorldObject::TypeInfo::ObjectSubtype subtype, std::string bone, std::string mesh)
{
    // Renderinformationen suchen
    std::map<WorldObject::TypeInfo::ObjectSubtype, RenderInfo>::iterator it = m_object_render_info.find(subtype);
    if (it != m_object_render_info.end())
    {
        // gefunden
        it->second.m_extra_meshes.push_back(std::make_pair(bone,mesh));
    }
}

void Scene::registerPlayer(PlayerLook look, std::string mesh)
{
	m_player_render_info.insert(std::make_pair(look, RenderInfo(mesh, "")));
}

void Scene::registerPlayerLook(WorldObject::TypeInfo::ObjectSubtype subtype, PlayerLook look, bool male )
{
	m_player_look.insert(std::make_pair(subtype, std::make_pair(male,look)));
}

void Scene::registerItem(Item::Subtype subtype, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_item_render_info.insert(std::make_pair(subtype,RenderInfo(mesh,particle_system,scaling_factor)));
}

void Scene::registerTile(Tile tile, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_tile_render_info.insert(std::make_pair(tile,RenderInfo(mesh,particle_system,scaling_factor)));
}


void Scene::registerProjectile(Projectile::ProjectileType type, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_projectile_render_info.insert(std::make_pair(type,RenderInfo(mesh,particle_system,scaling_factor)));
}

RenderInfo  Scene::getObjectRenderInfo(WorldObject::TypeInfo::ObjectSubtype subtype)
{
	std::map<WorldObject::TypeInfo::ObjectSubtype, RenderInfo>::iterator i= m_object_render_info.find(subtype);
	if (i != m_object_render_info.end())
	{
		return i->second;
	}
	else
	{
		// Standardmesh
		return RenderInfo("goblin.mesh","");
	}
}

RenderInfo Scene::getPlayerRenderInfo(PlayerLook look)
{
	std::map<PlayerLook, RenderInfo>::iterator i= m_player_render_info.find(look);
	if (i != m_player_render_info.end())
	{
		return i->second;
	}
	else
	{
		// Standardmesh
		return RenderInfo("warrior_m.mesh","");
	}
}

RenderInfo  Scene::getItemRenderInfo(Item::Subtype subtype)
{
	std::map<Item::Subtype, RenderInfo>::iterator i= m_item_render_info.find(subtype);
	if (i != m_item_render_info.end())
	{
		return i->second;
	}
	else
	{
		// Standardmesh
		return RenderInfo("potion.mesh","");
	}
}

RenderInfo  Scene::getProjectileRenderInfo(Projectile::ProjectileType type)
{
	std::map<Projectile::ProjectileType, RenderInfo>::iterator i= m_projectile_render_info.find(type);
	if (i != m_projectile_render_info.end())
	{
		return i->second;
	}
	else
	{
		// Standardmesh
		return RenderInfo("arrow.mesh","");
	}
}


std::pair<float,float> Scene::getProjection(Vector pos)
{
	Ogre::Vector4 ipos(pos.m_x*50,0,pos.m_y*50,1);
	Ogre::Vector4 projpos;
	projpos = m_camera->getProjectionMatrix()*m_camera->getViewMatrix()*ipos;

	return std::make_pair(0.5 + 0.5*projpos.x / projpos.w, 0.5 - 0.5*projpos.y / projpos.w);
}

void Scene::update(float ms)
{
	DEBUG5("update scene");

	// Spielerobjekt
	Player* player = m_document->getLocalPlayer();

	if (player ==0)
		return;
	if (player->getRegion() ==0)
		return;

	// Nummer der region in der sich der Spieler befindet
	short region_nr = player->getGridLocation()->m_region;




	if (region_nr != m_region_id)
	{
		// Spieler hat eine neue Region betreten
		if (player->getState() == WorldObject::STATE_ACTIVE)
		{
			// Szene komplett neu aufbauen
			createScene();

			m_document->setModified(m_document->getModified() & ~Document::REGION_MODIFIED);

			m_region_id = region_nr;

		}
		else
		{
			// Spieler ist der Region noch nicht aktiviert
			return;
		}
	}

	// Koordinaten des Spielers
	Vector pos = player->getShape()->m_center;

	// Kamera auf Spieler ausrichten
	float r= player->getCamera().m_distance*50;
	float theta = player->getCamera().m_theta * 3.14159 / 180;
	float phi = player->getCamera().m_phi * 3.14159 / 180;
	
	if (player->getRegion()->getCutsceneMode())
	{
		RegionCamera::Position& cam =player->getRegion()->getCamera().m_position;
		pos = cam.m_focus;
		r = cam.m_distance*50;
		phi = cam.m_phi* 3.14159 / 180;
		theta = cam.m_theta* 3.14159 / 180;
	}
	
	m_camera->setPosition(Ogre::Vector3(pos.m_x*50 + r*cos(theta)*cos(phi), r*sin(theta), pos.m_y*50 - r*cos(theta)*sin(phi)));
	m_camera->lookAt(Ogre::Vector3(pos.m_x*50,70,pos.m_y*50));

	Ogre::Light* light= m_scene_manager->getLight("HeroLight");
	light->setPosition(Ogre::Vector3(pos.m_x*50,1000,pos.m_y*50));

	// alle Objekte aktualisieren
	updateObjects();


	DEBUG5("update items");
	// alle Items aktualisieren
	updateItems();


	updateProjectiles();

}

void  Scene::updateObjects()
{
	Timer timer;
	timer.start();

	DEBUG5("update objects");
	// Spielerobjekt
	Player* player = m_document->getLocalPlayer();

	// aktuelle Liste der Objekte holen
	float x,y;
	x = player->getShape()->m_center.m_x;
	y = player->getShape()->m_center.m_y;

	std::list<WorldObject*> objs;
	WorldObject* obj;
	std::list<WorldObject*>::iterator it;
	Shape s;
	s.m_center = player->getShape()->m_center;
	s.m_type = Shape::RECT;
	s.m_extent = Vector(20,20);

	//player->getRegion()->getObjectsInShape(&s,&objs, WorldObject::LAYER_ALL,WorldObject::CREATURE);
	//player->getRegion()->getObjectsInShape(&s,&objs, WorldObject::LAYER_ALL,WorldObject::DEAD);
	player->getRegion()->getObjectsInShape(&s,&objs);

	// Liste der aktuell in der Szene vorhanden Objekte durchmustern
	std::map<int, string>::iterator it2;
	int id;
	for (it2 = m_objects->begin();it2 != m_objects->end();)
	{
		id = it2->first;
		if (player->getRegion()->getObject(id) == 0)
		{
			// Objekt existiert nicht in der Liste im Dokument

			// Objekt loeschen
			deleteObject(it2->second);

			// aus der Liste der Objekte entfernen
			m_objects->erase(it2++);
		}
		else
		{
			++it2;
		}
	}


	// aller Objekte im Dokument durchmustern
	for (it = objs.begin();it != objs.end(); ++it)
	{
		obj = *it;
		
		if (obj->getState() == WorldObject::STATE_STATIC)
			continue;
		
		
		// Darstellung fuer das Objekt aktualisieren
		updateObject(obj);
	}

}

void Scene::updateItems()
{
	Player* player = m_document->getLocalPlayer();

	// Liste der aktuell in der Szene vorhandenen Items durchmustern
	std::map<int,DropItem*>* itms;
	std::map<int,DropItem*>::iterator it;
	itms = player->getRegion()->getDropItems();
	DropItem* di;

	std::map<int, string>::iterator it2;
	int id;

	for (it2= m_drop_items->begin(); it2 != m_drop_items->end(); )
	{

		id= it2->first;
		if (itms->find(id) == itms->end())
		{
			// Item existiert nicht in der Liste im Dokument

			// Item loeschen
			deleteItem(it2->second);

			// aus der Liste der Items entfernen
			m_drop_items->erase(it2++);
		}
		else
		{
			++it2;
		}

	}

	// Liste der Objekte im Dokument durchmustern
	for (it = itms->begin();it != itms->end(); ++it)
	{
		di = (it->second);

		// Name des Items
		std::string name = di->getNameId();
		std::string node_name = name + "Node";
		if (!m_scene_manager->hasSceneNode(node_name))
		{
			// Item existiert noch nicht in der Szene

			createItem(di,name);

		}
		else
		{
			// Koordinaten des Objektes
			float x=di->m_x;
			float y=di->m_y;
			float z = di->m_height;

			// Ortsvektor des Objektes
			Ogre::Vector3 vec(x*25,z*50,y*25);

			// an die richtige Stelle verschieben
			Ogre::SceneNode* itm_node = m_scene_manager->getSceneNode(node_name);
			itm_node->setPosition(vec);

			float angle = di->m_angle_z;
			itm_node->setDirection(cos(angle),0,sin(angle),Ogre::Node::TS_WORLD);

			angle = di->m_angle_x;
			itm_node->pitch(Ogre::Radian(angle));

		}
	}
}

void Scene::updateObject(WorldObject* obj)
{
	std::string name = obj->getNameId();
	DEBUG5("handle obj %s",name.c_str());
	std::string node_name = name + "Node";

	// Ogre::Entity des Objektes
	Ogre::Entity* obj_ent;
	Ogre::SceneNode* node;

	if (!m_scene_manager->hasSceneNode(node_name))
	{
		// Objekt existiert noch nicht in der Szene

		

		//Objekt anlegen
		createObject(obj,name);
	}


	// Koordinaten des Objektes
	float x=obj->getShape()->m_center.m_x;
	float y=obj->getShape()->m_center.m_y;

	// Ortsvektor des Objektes
	Ogre::Vector3 vec(x*50,0,y*50);


	// an die richtige Stelle verschieben
	node = m_scene_manager->getSceneNode(node_name);
	node->setPosition(vec);

	float angle = obj->getShape()->m_angle;
	// Objekt drehen
	node->setDirection(cos(angle),0,sin(angle),Ogre::Node::TS_WORLD);

	// Statusmods anpassen
	std::ostringstream num("");
	std::string mod_name;



	// Animation anpassen
	// Status der Animation
	Ogre::AnimationState* anim;
    Ogre::AnimationStateSet* anim_set;

	DEBUG5("animation");
	// Aktion des Objeks
	Creature* cr=0;
	if (obj->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
	{
		cr = static_cast<Creature*>(obj);
		Action::ActionType act = cr->getAction()->m_type;
		Action::ActionInfo* aci = Action::getActionInfo(act);
		float perc = cr->getAction()->m_elapsed_time / cr->getAction()->m_time  ;
		
		if (cr->getState() == WorldObject::STATE_DEAD)
		{
			act = Action::DIE;
			perc = 0.99;
		}
		// Name der Animation ermitteln
		
		// Wenn in der Datenbank ein Satz Animationen fuer die Aktion enthalten diesen verwenden
		// (sonst den Standardsatz
		std::vector<std::string>* animations = &(aci->m_animation[cr->getActionEquip()]);
		if (m_object_animations.count(cr->getTypeInfo()->m_subtype) != 0)
		{
			animations = &(m_object_animations[cr->getTypeInfo()->m_subtype][act]);
		}
		
		std::string anim_name = "";
		if (!animations->empty())
		{
			anim_name = (*animations)[cr->getAction()->m_animation_number % animations->size()];
		}

		obj_ent = m_scene_manager->getEntity(name);
		if (obj_ent == 0)
		{
				ERRORMSG("object %s not found",name.c_str());
		}

		if (anim_name != "")
		{
			// status der Animation setzen
			//Iterator ueber alle aktiven Animationen
			anim_set = obj_ent->getAllAnimationStates();

			// Testen ob das Objekt animiert ist
			if (anim_set != 0)
			{

				Ogre::ConstEnabledAnimationStateIterator anim_it = anim_set->getEnabledAnimationStateIterator();

				//deaktiviert alle animationen, die gerade nicht verwendet werden
				while(anim_it.hasMoreElements())
				{
					anim = anim_it.getNext();
					//TODO
					//if(anim->getAnimationName!=anim_name)
						anim->setEnabled(false);
				}

				try
				{
					if (anim_set->hasAnimationState(anim_name))
					{
						anim = obj_ent->getAnimationState(anim_name);
						anim->setEnabled(true);

						// prozentsatz zu dem die Animation fortgeschritten ist
						
						DEBUG5("setting animation %s to %f",anim_name.c_str(),perc);
						anim->setTimePosition(perc*anim->getLength());
					}
				}
				catch (Ogre::Exception e)
				{
					ERRORMSG(e.what());
				}
			}
		}
		else
		{
			//keine Animation
		}
	}

	DEBUG5("extra meshes");
    // angehaengte Meshes an die verwendeten Items anpassen
    if (obj->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
    {
        // Objekt ist ein Spieler
        Player* cmp = static_cast<Player*>(obj);
        //Equipement* equ =  cmp->getEquipement();

        // Schleife ueber alle angehaengten Entities
        Ogre::Entity* attch_ent;

        // Knochen an den ein Mesh angehaengt ist
        std::string bone;

        // Name des neuen Meshes an einem Knochen
        std::string old_ent_name;
        std::string new_ent_name;

        Item* itm;
        Item::Subtype itmsubtype;

        Ogre::Node* node;

       Ogre::Entity::ChildObjectListIterator it = obj_ent->getAttachedObjectIterator();

		// mappt Namen von Knochen auf die daran anzuhaengenen Meshes
        std::map<std::string, std::string> goal_atch;
        std::map<std::string, std::string>::iterator jt;

        itm = cmp->getWeapon();
        if (itm !=0)
        {
            itmsubtype = itm->m_subtype;
            new_ent_name = getItemRenderInfo(itmsubtype).m_mesh;

			// einige Waffen werden in der Linken Hand getragen
			if (itm->m_weapon_attr->m_weapon_type == WeaponAttr::BOW || itm->m_weapon_attr->m_weapon_type == WeaponAttr::CROSSBOW)
			{
				goal_atch.insert(std::make_pair("itemLeftHand",new_ent_name));
			}
			else
			{
            	goal_atch.insert(std::make_pair("itemRightHand",new_ent_name));
			}
        }

		itm = cmp->getShield();
		if (itm !=0)
		{
			itmsubtype = itm->m_subtype;
			new_ent_name = getItemRenderInfo(itmsubtype).m_mesh;

			goal_atch.insert(std::make_pair("itemLeftHand",new_ent_name));
		}

        // Schleife ueber die aktuell angehaengten Meshes
        while (it.hasMoreElements())
        {
            bone = it.peekNextKey();
            bone = bone.substr(name.size());
            attch_ent = static_cast<Ogre::Entity*>(it.getNext());
            old_ent_name = attch_ent->getMesh()->getName();
            DEBUG5("bone name %s",bone.c_str());
            jt = goal_atch.find(bone);

            if (jt != goal_atch.end())
            {
                // es ist bereits ein Objekt an den Knochen gehaengt

                if (old_ent_name != jt->second)
                {
                    // Es soll ein anderes Mesh angehaengt werden als aktuell angehaengt ist
                    DEBUG5("replaced mesh %s by %s at bone %s",old_ent_name.c_str(),jt->second.c_str(),bone.c_str());


                    // altes Mesh entfernen, neues anhaengen
                    obj_ent->detachObjectFromBone(attch_ent);
                    m_scene_manager->destroyEntity(attch_ent);

                    attch_ent = m_scene_manager->createEntity(name+bone,jt->second);
                    obj_ent->attachObjectToBone(bone,attch_ent);

                    node = attch_ent->getParentNode();
                    node->setInheritScale(false);


                }

                // Zielzustand erreicht
                goal_atch.erase(jt);

            }
            else
            {
                // Objekt befindet sich im Zielzustand nicht am Knochen

                DEBUG5("removed mesh %s from bone %s",old_ent_name.c_str(),bone.c_str());
                // Mesh entfernen
                obj_ent->detachObjectFromBone(attch_ent);
                m_scene_manager->destroyEntity(attch_ent);
            }
        }

        // Behandeln der Meshes die an keinem Knochen gefunden wurden
        for (jt = goal_atch.begin(); jt != goal_atch.end();++jt)
        {
            bone = jt->first;
            DEBUG5("attached mesh %s at bone %s",jt->second.c_str(),bone.c_str());
            attch_ent = m_scene_manager->createEntity(name+bone,jt->second);

            obj_ent->attachObjectToBone(bone,attch_ent);
            node = attch_ent->getParentNode();
            node->setInheritScale(false);


        }
    }
	DEBUG5("particle");
	//zeigt an ob ein Partikelsystem sichtbar is


	// Ermitteln welche Partikelsysteme vorhanden sind
	std::string modnames[8]  = {"Blind", "Poison", "Berserk", "Confuse", "Mute", "Paralyze", "Frozen", "Burning"};
	std::string effectnames[1] = {"Hit"};

	std::list<Ogre::MovableObject*> rm_obj;
	std::list<Ogre::MovableObject*>::iterator it;

	std::list<Ogre::MovableObject*> attach_obj;

	int i;
	Ogre::ParticleSystem *mod_part;
	if (cr !=0)
	{
		// Feld das angibt, welche Mods gesetzt sind
		float * status_mods = cr->getDynAttr()->m_status_mod_time;
		std::string mod;

		Ogre::MovableObject* obj;


		// Schleife ueber die Statusmods
		for (i=0;i<NR_STATUS_MODS;i++)
		{
			mod_part =0;

			// Schleife ueber die angehaengten Objekte
			for (int j=0; j< node->numAttachedObjects(); j++)
			{
				obj = node->getAttachedObject(j);

				// pruefen ob das angehaengte Objekt das Partikelsystem fuer den aktuellen Statusmod ist
				if (obj->getMovableType()== "ParticleSystem")
				{
					mod = Ogre::any_cast<std::string>(obj->getUserAny());
					if (mod == modnames[i])
					{
						mod_part = static_cast<Ogre::ParticleSystem*> (obj);
						break;
					}
				}
			}

			if (mod_part !=0 && status_mods[i]<=0)
			{
				// Partikelsystem ist vorhanden, aber der zugehoerige Mod nicht aktiv
				// Partikelsystem entfernen und in den Pool verschieben

				rm_obj.push_back(mod_part);
				putBackParticleSystem(mod_part);
			}

			if (mod_part ==0 && status_mods[i]>0)
			{
				// Partikelsystem ist nicht vorhanden, aber der zugehoerige Mod ist aktiv
				// Partikelsystem aus dem Pool entnehmen und an den Knoten anfuegen
				mod_part = getParticleSystem(modnames[i]);
				attach_obj.push_back(mod_part);

			}
		}


		// weitere Effekte anpassen
		float* effects =  cr->getDynAttr()->m_effect_time;

		for (i=0;i<NR_EFFECTS;i++)
		{
			mod_part =0;

			// Schleife ueber die angehaengten Objekte
			for (int j=0; j< node->numAttachedObjects(); j++)
			{
				obj = node->getAttachedObject(j);

				// pruefen ob das angehaengte Objekt das Partikelsystem fuer den aktuellen Statusmod ist
				if (obj->getMovableType()== "ParticleSystem")
				{
					mod = Ogre::any_cast<std::string>(obj->getUserAny());
					if (mod == effectnames[i])
					{
						mod_part = static_cast<Ogre::ParticleSystem*> (obj);
						break;
					}
				}
			}

			if (mod_part !=0 && effects[i]<=0)
			{
				// Partikelsystem ist vorhanden, aber der zugehoerige Effekt nicht aktiv
				// Partikelsystem entfernen und in den Pool verschieben

				rm_obj.push_back(mod_part);
				putBackParticleSystem(mod_part);

			}

			if (mod_part ==0 && effects[i]>0)
			{
				// Partikelsystem ist nicht vorhanden, aber der zugehoerige Effekt ist aktiv
				// Partikelsystem aus dem Pool entnehmen und an den Knoten anfuegen
				mod_part = getParticleSystem(effectnames[i]);
				attach_obj.push_back(mod_part);

			}
		}

		for (it = rm_obj.begin(); it != rm_obj.end(); ++it)
		{
			node->detachObject(*it);
		}

		for (it = attach_obj.begin(); it != attach_obj.end(); ++it)
		{
			node->attachObject(*it);
		}

	}

	// Sound aktualisieren
	if (obj->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
	{
		std::string sname= name;
		sname += ":weapon";
		SoundObject* sobj = SoundSystem::getSoundObject(sname);
		if (sobj !=0)
		{
			Player* pl = static_cast<Player*>(obj);
			Action::ActionType act = pl->getAction()->m_type;
			Action::ActionType baseact = Action::getActionInfo(act)->m_base_action;

			if ((baseact == Action::ATTACK || baseact == Action::HOLY_ATTACK || baseact == Action::RANGE_ATTACK|| baseact == Action::MAGIC_ATTACK) && pl->getAction()->m_elapsed_time <200)
			{
				SoundTarget target = "";
				if (pl->getWeapon() != 0)
				{
					target = pl->getWeapon()->m_subtype;
					target += ":attack";
				}

				SoundName sound;
				sound = SoundSystem::getSoundName(target);
				sobj->setSound(sound);

			}

		}
	}

	if (obj->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
	{
		// Sound durch Aktion
		std::string actsoundname = name;
		actsoundname += ":action";
		SoundObject* sobj = SoundSystem::getSoundObject(actsoundname);
		if (sobj!=0)
		{
			Creature* cr = static_cast<Creature*>(obj);
			if (cr->getAction()->m_elapsed_time < 200)
			{
				Action::ActionType act = cr->getAction()->m_type;
				SoundTarget target = cr->getTypeInfo()->m_subtype;

				if (obj->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
				{
					target = "hero";
				}

				target += ":";
				target += Action::getActionInfo(act)->m_enum_name;

				SoundName sound;
				sound = SoundSystem::getSoundName(target);
				if (act == Action::DIE)
				{
					sobj->stop();
				}
				sobj->setSound(sound);
			}
		}

		// Sound durch Wirkung von aussen
		std::string passoundname = name;
		passoundname += ":passive";
		SoundObject* sobj2 = SoundSystem::getSoundObject(passoundname);
		float* effects =  cr->getDynAttr()->m_effect_time;


		if (effects[CreatureDynAttr::BLEEDING]> 50)
		{
			SoundTarget target = cr->getTypeInfo()->m_subtype;

			if (obj->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
			{
				target = "hero";
			}

			target += ":hit";
			SoundName sound;
			sound = SoundSystem::getSoundName(target);
			sobj2->setSound(sound);
		}

	}

}

void Scene::deleteObject(std::string name)
{
	std::string sname= name;
	sname += ":weapon";
	if (SoundSystem::getSoundObject(sname)!=0)
	{
		SoundSystem::deleteSoundObject(sname);
	}

	std::string actsoundname = name;
	actsoundname += ":action";
	if (SoundSystem::getSoundObject(actsoundname)!=0)
	{
		SoundSystem::deleteSoundObject(actsoundname);
	}

	std::string passoundname = name;
	passoundname += ":passive";
	if (SoundSystem::getSoundObject(passoundname)!=0)
	{
		SoundSystem::deleteSoundObject(passoundname);
	}

	std::string node_name = name + "Node";

	DEBUG5("deleting object %s",name.c_str());


	destroySceneNode(node_name);

}

void Scene::createObject(WorldObject* obj,std::string& name, bool is_static)
{
	if (!is_static)
	{
		// in die Liste der Objekte einfuegen
		m_objects->insert(std::make_pair(obj->getId(),name));
	}
	
	Timer timer;
	timer.start();

	DEBUG5("creating object %s",name.c_str());
	std::string node_name = name + "Node";

	// Koordinaten des Objektes
	float x=obj->getShape()->m_center.m_x;
	float y=obj->getShape()->m_center.m_y;

	// Ortsvektor des Objektes
	Ogre::Vector3 vec(x*50,0,y*50);

	// Node anlegen
	Ogre::SceneNode* obj_node;
	obj_node =m_scene_manager->getRootSceneNode()->createChildSceneNode(node_name,vec);

	float angle = obj->getShape()->m_angle;
	// Objekt drehen
	obj_node->setDirection(cos(angle),0,sin(angle),Ogre::Node::TS_WORLD);

	// Informationen zum Rendern anfordern
	RenderInfo ri;
	Player* pl = dynamic_cast<Player*>(obj);
	if (pl != 0 )
	{
		ri = getPlayerRenderInfo(pl->getPlayerLook());
	}
	else
	{
		ri = getObjectRenderInfo(obj->getTypeInfo()->m_subtype);
	}

	// Je nach Typ das richtige Mesh benutzen
	Ogre::Entity* obj_ent;
	obj_ent = m_scene_manager->createEntity(name, ri.m_mesh);

    obj_node->attachObject(obj_ent);

    // weitere Meshes anfuegen
    std::list<std::pair<std::string,std::string> >::iterator it;
    // Entity fuer ein angefuegtes Mesh
    Ogre::Entity* extr_m_ent;

    Ogre::Node* node;
    for (it = ri.m_extra_meshes.begin(); it !=ri.m_extra_meshes.end();++it)
    {
            extr_m_ent = m_scene_manager->createEntity(name + it->first,it->second);
            obj_ent->attachObjectToBone(it->first,extr_m_ent);

            node = extr_m_ent->getParentNode();
            node->setInheritScale(false);

    }

	// Fuer Spieler ein extra Soundobjekt fuer die Waffe anlegen
	if (obj->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
	{
		std::string sname= name;
		sname += ":weapon";
		SoundSystem::createSoundObject(sname);
	}

	if (!is_static)
	{
		std::string actsoundname = name;
		actsoundname += ":action";
		SoundSystem::createSoundObject(actsoundname);

		std::string passoundname = name;
		passoundname += ":passive";
		SoundSystem::createSoundObject(passoundname);
	}

}

void Scene::createItem(DropItem* di, std::string& name)
{
	std::string node_name = name + "Node";

	// SoundObjekt anlegen
	SoundObject* obj = SoundSystem::createSoundObject(name);
	if (di->m_height > 0)
	{
		SoundTarget target = di->m_item->m_subtype;
		target += ":drop";
		SoundName sound = SoundSystem::getSoundName(target);
		obj->setSound(sound);
		obj->play();
	}
	DEBUG5("created item %s",name.c_str());
	// Ortsvektor des Items
	Ogre::Vector3 vec(di->m_x*25,0,di->m_y*25);

	// in die Liste einfuegen
	m_drop_items->insert(std::make_pair(di->m_item->m_id,name));

	// Node anlegen
	Ogre::SceneNode* obj_node;
	obj_node =m_scene_manager->getRootSceneNode()->createChildSceneNode(node_name,vec);


	// Informationen zum Rendern anfordern
	RenderInfo ri = getItemRenderInfo(di->m_item->m_subtype);

	// Je nach Typ das richtige Mesh benutzen
	Ogre::Entity* ent;
	ent = m_scene_manager->createEntity(name, ri.m_mesh);

	// Objekt drehen
	float angle = di->m_angle_z;
	obj_node->setDirection(cos(angle),0,sin(angle),Ogre::Node::TS_WORLD);

	obj_node->attachObject(ent);

}

void Scene::deleteItem(std::string name)
{
	SoundSystem::deleteSoundObject(name);

	std::string node_name = name + "Node";

	DEBUG5("deleting item %s",name.c_str());

	destroySceneNode(node_name);

}

void Scene::updateProjectiles()
{
	Player* player = m_document->getLocalPlayer();

	std::map<int,Projectile*>* projectiles;
	std::map<int,Projectile*>::iterator it;
	projectiles = player->getRegion()->getProjectiles();
	Projectile* pr;

	std::map<int, string>::iterator it2;
	int id;

	// Liste der aktuell in der Szene vorhandenen Projektile durchmustern
	for (it2= m_projectiles->begin(); it2 != m_projectiles->end(); )
	{

		id= it2->first;
		if (projectiles->find(id) == projectiles->end())
		{
			// Projektil existiert nicht in der Liste im Dokument

			// loeschen
			deleteProjectile(it2->second);

			// aus der Liste der Projektile entfernen
			m_projectiles->erase(it2++);
		}
		else
		{
			++it2;
		}

	}

	// Liste der Projektile im Dokument durchmustern
	for (it = projectiles->begin();it != projectiles->end(); ++it)
	{
		pr = (it->second);

		std::ostringstream out_stream("");
		out_stream << "projectile:"<<pr->getId();
		std::string name = out_stream.str();
		std::string node_name = name + "Node";
		if (!m_scene_manager->hasSceneNode(node_name))
		{
			// Projektil existiert noch nicht in der Szene
			// erzeugen
			createProjectile(pr,name);

		}

		// Ortsvektor des Projektils
		Ogre::Vector3 vec(pr->getShape()->m_center.m_x*50,50,pr->getShape()->m_center.m_y*50);

		// an die richtige Stelle verschieben
		m_scene_manager->getSceneNode(node_name)->setPosition(vec);

		// Objekt drehen
		float angle = pr->getShape()->m_angle;
		m_scene_manager->getSceneNode(node_name)->setDirection(cos(angle),0,sin(angle),Ogre::Node::TS_WORLD);

		if (pr->getTimer()<200)
		{
			SoundTarget target = SoundSystem::getProjectileSound(pr->getType());
			SoundName sound;
			sound = SoundSystem::getSoundName(target);
			SoundObject* obj = SoundSystem::getSoundObject(name);

			if (sound =="")
			{
				DEBUG("no sound for target %s",target.c_str());
			}
			if (obj !=0)
			{

				obj->setSound(sound);
			}
		}

	}
}

void Scene::createProjectile(Projectile* pr, std::string& name)
{
	// SoundObjekt anlegen
	SoundSystem::createSoundObject(name);


	DEBUG5("creating projectile %s",name.c_str());

	std::string node_name = name + "Node";

	// Ortsvektor des Projektils
	Ogre::Vector3 vec(pr->getShape()->m_center.m_x*50,50,pr->getShape()->m_center.m_y*50);

	// in die Liste einfuegen
	m_projectiles->insert(std::make_pair(pr->getId(),name));

	DEBUG5("speed %f %f",pr->getSpeed().m_x, pr->getSpeed().m_y);
	DEBUG5("angle %f ",pr->getShape()->m_angle*180/3.14);

	// Node anlegen
	Ogre::SceneNode* obj_node;
	obj_node =m_scene_manager->getRootSceneNode()->createChildSceneNode(node_name,vec);

	Projectile::ProjectileType type = pr->getType();
	Ogre::Entity *ent =0;
	//Partikelsystem anlegen
	std::string particle_name = name + "Particle";
	Ogre::ParticleSystem *part = 0;

	RenderInfo ri = getProjectileRenderInfo(type);

	if (ri.m_mesh != "")
	{
		ent = m_scene_manager->createEntity(name,ri.m_mesh);
		obj_node->attachObject(ent);
	}

	if (ri.m_particle_system != "")
	{
		part = getParticleSystem(ri.m_particle_system);
		obj_node->attachObject(part);
	}


}

void Scene::deleteProjectile(std::string name)
{
	SoundSystem::deleteSoundObject(name);

	std::string node_name = name + "Node";

	DEBUG5("deleting projectile %s",name.c_str());

	destroySceneNode(node_name);
}

Ogre::ParticleSystem* Scene::getParticleSystem(std::string type)
{
	// Im Pool nach einem passenden Partikelsystem suchen
	std::multimap<std::string, Ogre::ParticleSystem*>::iterator it;
	it = m_particle_system_pool.find(type);

	Ogre::ParticleSystem* part=0;
	static int count =0;

	if (it == m_particle_system_pool.end())
	{
		// Kein Partikelsystem gefunden
		// neues erzeugen
		std::ostringstream name;
		name << "ParticleSystem"<<count;
		count ++;

		part = m_scene_manager->createParticleSystem(name.str(), type);
		part->setUserAny(Ogre::Any(type));
		part->setKeepParticlesInLocalSpace(true);
		DEBUG5("created particlesystem %p %s for type %s",part, name.str().c_str(), type.c_str());
	}
	else
	{
		// Partikelsystem aus dem Pool nehmen
		part = it->second;
		m_particle_system_pool.erase(it);
		DEBUG5("took particlesystem %s for type %s",part->getName().c_str(), type.c_str());
	}

	part->clear();
	return part;
}

void Scene::putBackParticleSystem(Ogre::ParticleSystem* part)
{
	// Typ des Partikelsystems ermitteln
	std::string type;
	type = Ogre::any_cast<std::string>(part->getUserAny());

	DEBUG5("put back particlesystem %p %s for type %s",part, part->getName().c_str(), type.c_str());

	m_particle_system_pool.insert(std::make_pair(type,part));
}

void Scene::destroySceneNode(std::string& node_name)
{

	Ogre::SceneNode* node = m_scene_manager->getSceneNode(node_name);

	if (node==0)
		return;

	// Partikelsysteme werden nicht geloescht sondern wieder in den Pool eingefuegt

	std::list<Ogre::MovableObject*> rm_obj;
	std::list<Ogre::MovableObject*>::iterator jt;

	std::string mod;

	// Schleife ueber die angehaengten Objekte
	Ogre::MovableObject* obj;
	for (int j=0; j< node->numAttachedObjects(); j++)
	{
		obj = node->getAttachedObject(j);

		// pruefen ob das angehaengte Objekt ein Partikelsystem ist
		if (obj->getMovableType()== "ParticleSystem")
		{
			putBackParticleSystem(static_cast<Ogre::ParticleSystem*> (obj) );
			rm_obj.push_back(obj);
		}
	}

	for (jt = rm_obj.begin(); jt != rm_obj.end(); ++jt)
	{
		node->detachObject(*jt);
	}



	// Iterator ueber die angehaengten Objekte
	Ogre::SceneNode::ObjectIterator it = node->getAttachedObjectIterator();

	std::string name;
	std::list<Ogre::MovableObject*> objects;
	std::list<Ogre::MovableObject*>::iterator i;

	while (it.hasMoreElements())
	{
		// Name des aktuellen Objektes
		name = it.peekNextKey();
		// Zeiger auf aktuelles Objekt
		obj = it.peekNextValue();

		DEBUG5("deleting object %s",name.c_str());

		objects.push_back(obj);

		it.moveNext();
	}

	for (i=objects.begin(); i!=objects.end();i++)
	{
		node->detachObject(*i);
		m_scene_manager->destroyMovableObject((*i));
	}
	// Knoten entfernen
	m_scene_manager->destroySceneNode(node_name);
}


void Scene::clearObjects()
{
	m_objects->clear();
	m_drop_items->clear();
	m_projectiles->clear();
}

void Scene::createScene()
{
	DEBUG5("create Scene");

	// alle Partikelsystem loeschen
	std::multimap<std::string, Ogre::ParticleSystem*>::iterator kt;
	for (kt = m_particle_system_pool.begin(); kt !=  m_particle_system_pool.end(); ++kt)
	{
		DEBUG5("destroy particle system %s",kt->second->getName().c_str());
		m_scene_manager->destroyParticleSystem(kt->second);
	}
	m_particle_system_pool.clear();

	// alle bisherigen Objekte aus der Szene loeschen
	m_scene_manager->clearScene();
	clearObjects();
	SoundSystem::clearObjects();

    m_scene_manager->setAmbientLight(Ogre::ColourValue(0.4,0.4,0.4));
    Ogre::Light *light = m_scene_manager->createLight("HeroLight");
    light->setType(Ogre::Light::LT_SPOTLIGHT);
    light->setDiffuseColour(0.8, 0.8, 0.8);
    light->setSpecularColour(0.5, 0.5, 0.5);
    light->setDirection(0,-1,0);

    light = m_scene_manager->createLight("RegionLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDiffuseColour(0.6, 0.6, 0.6);
    light->setSpecularColour(1.0, 1.0, 1.0);
    light->setDirection(Ogre::Vector3(400,1000,300));


	// Liste der statischen Objekte
	std::list<WorldObject*> stat_objs;

	Region* region = m_document->getLocalPlayer()->getRegion();

	Shape s;
	s.m_center = Vector(0,0);
	s.m_type = Shape::RECT;
	s.m_extent = Vector(10000,10000);

	region->getObjectsInShape(&s,&stat_objs, WorldObject::LAYER_ALL,WorldObject::FIXED);
	std::list<WorldObject*>::iterator it;
	std::string name;
	for (it = stat_objs.begin(); it !=stat_objs.end();++it)
	{
		name = (*it)->getNameId();

		DEBUG5("create static object %s",name.c_str());

		// Objekt in der Szene erzeugen
		createObject((*it),name, ((*it)->getState() == WorldObject::STATE_STATIC));

	}

	// Tiles einfuegen
	insertTiles();

}

void  Scene::insertTiles()
{
	/*
	// Matrix der Tiles
	Matrix2d<char>* mat = m_document->getRegionData()->m_tiles;


	// Matrix durchgehen
	Tile tile;
	ostringstream out_stream;
	for (int i =0;i<m_document->getRegionData()->m_dimx*2-1;i++)
	{

		for (int j =0;j<m_document->getRegionData()->m_dimy*2-1;j++)
		{
			tile = (Tile) *(mat->ind(i,j));
			if (tile ==TILE_NOTHING)
			{
				continue;
			}
			// Name des Tiles
			out_stream.str("");
			out_stream << "Tile:"<<i<<":"<<j;
			string tname = out_stream.str();


			// Entity des Tiles
			Ogre::Entity *ent = m_scene_manager->createEntity(tname, "Tile");

			// TODO: richtiges Material auswaehlen
			ent->setMaterialName("grass1");

			// Node des Tiles
			Ogre::SceneNode* obj_node;
			obj_node = m_scene_manager->getRootSceneNode()->createChildSceneNode(tname+"Node",Ogre::Vector3(i*100,0,j*100));

			// Objekt an den Knoten haengen
			obj_node->attachObject(ent);

		}
	}
	*/
}


