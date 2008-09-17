#include "scene.h"

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

	m_objects = new map<int,std::string>;

	m_drop_items = new map<int,std::string>;

	m_projectiles = new map<int,std::string>;

	registerMeshes();

}

Scene::~Scene()
{
	clearObjects();
	delete m_objects;

	delete m_drop_items;

	delete m_projectiles;
}

void Scene::registerMeshes()
{
	// Meshes fuer Objekte registrieren

	// Spieler
	registerObject("warrior","warrior.mesh","");
	registerAttachedMesh("warrior","itemRightHand","sword.mesh");
	registerObject("mage","warrior.mesh","");
	registerObject("priest","warrior.mesh",""); // TODO
	registerObject("archer","warrior.mesh",""); // TODO

	// Monster
	registerObject("goblin","goblin.mesh","");
	registerObject("gob_dog","gobDog.mesh","");
	registerObject("lich","lich.mesh","");

	// feste Objekte
	registerObject("tree","tree.mesh","");
	registerObject("fence","fence.mesh","");

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

	// Items
	registerItem("short_sw","sword.mesh");
	registerItem("long_sw","sword.mesh");
	registerItem("wood_bow","sword.mesh");
	registerItem("long_bow","sword.mesh");
	registerItem("fire_bow","sword.mesh");
	registerItem("battle_axe","greatAxe.mesh");
	registerItem("holy_flail","warhammer.mesh");
	registerItem("ice_wand","sword.mesh");

	registerItem("leath_arm","armor.mesh");
	registerItem("tiled_arm","armor.mesh");
	registerItem("heavy_arm","armor.mesh");

	registerItem("wood_sh","shield.mesh");
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
	m_object_render_info.insert(make_pair(subtype,RenderInfo(mesh,particle_system,scaling_factor)));
}

void Scene::registerAttachedMesh(WorldObject::TypeInfo::ObjectSubtype subtype, std::string bone, std::string mesh)
{
    // Renderinformationen suchen
    map<WorldObject::TypeInfo::ObjectSubtype, RenderInfo>::iterator it = m_object_render_info.find(subtype);
    if (it != m_object_render_info.end())
    {
        // gefunden
        it->second.m_extra_meshes.push_back(make_pair(bone,mesh));
    }
}
void Scene::registerItem(Item::Subtype subtype, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_item_render_info.insert(make_pair(subtype,RenderInfo(mesh,particle_system,scaling_factor)));
}

void Scene::registerTile(Tile tile, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_tile_render_info.insert(make_pair(tile,RenderInfo(mesh,particle_system,scaling_factor)));
}


void Scene::registerProjectile(Projectile::ProjectileType type, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_projectile_render_info.insert(make_pair(type,RenderInfo(mesh,particle_system,scaling_factor)));
}

RenderInfo  Scene::getObjectRenderInfo(WorldObject::TypeInfo::ObjectSubtype subtype)
{
	map<WorldObject::TypeInfo::ObjectSubtype, RenderInfo>::iterator i= m_object_render_info.find(subtype);
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

RenderInfo  Scene::getItemRenderInfo(Item::Subtype subtype)
{
	map<Item::Subtype, RenderInfo>::iterator i= m_item_render_info.find(subtype);
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
	map<Projectile::ProjectileType, RenderInfo>::iterator i= m_projectile_render_info.find(type);
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


void Scene::update(float ms)
{
	// Spielerobjekt
	ClientMPlayer* player = m_document->getMainPlayer();
	// Koordinaten des Spielers
	float x=player->getGeometry()->m_shape.m_coordinate_x;
	float y=player->getGeometry()->m_shape.m_coordinate_y;

	// Kamera auf Spieler ausrichten
	m_camera->setPosition(Ogre::Vector3(x*50, 1000, y*50+300));
	m_camera->lookAt(Ogre::Vector3(x*50,0,y*50));

	if (m_document->getModified() & Document::REGION_MODIFIED)
	{
		// Spieler hat eine neue Region betreten

		// Szene komplett neu aufbauen
		createScene();

		m_document->setModified(m_document->getModified() & ~Document::REGION_MODIFIED);
	}

	// alle Objekte aktualisieren
	updateObjects();

	// alle Items aktualisieren
	updateItems();

	updateProjectiles();
}

void  Scene::updateObjects()
{
	// aktuelle Liste der Objekte holen
	map<int, ClientWObject*>* objs;
	map<int, ClientWObject*>::iterator it;
	objs = m_document->getObjects();
	ClientWObject* obj;

	// Liste der aktuell in der Szene vorhanden Objekte durchmustern
	map<int, string>::iterator it2;
	int id;
	for (it2 = m_objects->begin();it2 != m_objects->end();)
	{
		id = it2->first;
		if (objs->find(id) == objs->end())
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
	for (it = objs->begin();it != objs->end(); ++it)
	{
		obj = it->second;
		// Darstellung fuer das Objekt aktualisieren
		updateObject(obj);
	}
}

void Scene::updateItems()
{
	// Liste der aktuell in der Szene vorhandenen Items durchmustern
	map<int,DropItem>* itms;
	map<int,DropItem>::iterator it;
	itms = m_document->getDropItems();
	DropItem* di;

	map<int, string>::iterator it2;
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
		di = &(it->second);

		// Name des Items
		std::string name = di->getNameId();
		std::string node_name = name + "Node";
		if (!m_scene_manager->hasSceneNode(node_name))
		{
			// Item existiert noch nicht in der Szene

			createItem(di,name);

		}
	}
}

void Scene::updateObject(ClientWObject* obj)
{
	std::string name = obj->getNameId();
	DEBUG5("handle obj %s",name.c_str());
	std::string node_name = name + "Node";

	// Ogre::Entity des Objektes
	Ogre::Entity* obj_ent;

	// Node des Objektes
	Ogre::SceneNode* obj_node;


	if (!m_scene_manager->hasSceneNode(node_name))
	{
		// Objekt existiert noch nicht in der Szene

		// in die Liste der Objekte einfuegen
		m_objects->insert(make_pair(obj->getId(),name));

		//Objekt anlegen
		createObject(obj,name);
	}


	// Koordinaten des Objektes
	float x=obj->getGeometry()->m_shape.m_coordinate_x;
	float y=obj->getGeometry()->m_shape.m_coordinate_y;

	// Ortsvektor des Objektes
	Ogre::Vector3 vec(x*50,0,y*50);


	// an die richtige Stelle verschieben
	m_scene_manager->getSceneNode(node_name)->setPosition(vec);

	float angle = obj->getGeometry()->m_angle;
	// Objekt drehen
	m_scene_manager->getSceneNode(node_name)->setDirection(cos(angle),0,sin(angle),Ogre::Node::TS_WORLD);

	// Statusmods anpassen
	std::ostringstream num("");
	std::string mod_name;
	int i;
	Ogre::ParticleSystem *mod_part;
	// Bitmaske welche Mods gesetzt sind
	char mask = obj->m_status_mods;

	// Animation anpassen
	// Status der Animation
	Ogre::AnimationState* anim;
    Ogre::AnimationStateSet* anim_set;

	// Aktion des Objeks
	Action::ActionType act = obj->getAction()->m_type;
	Action::ActionInfo* aci = Action::getActionInfo(act);

    // Name der Animation
	std::vector<std::string>& animations = aci->m_animation[obj->getAction()->m_action_equip];
	std::string anim_name = "";
	if (!animations.empty())
	{
		anim_name = animations[obj->getAction()->m_animation_number % animations.size()];
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
                    float perc = obj->getAction()->m_elapsed_time / obj->getAction()->m_time  ;
                    DEBUG5("setting animation %s to %f",anim_name.c_str(),perc);
                    anim->setTimePosition(perc);
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

    // angehaengte Meshes an die verwendeten Items anpassen
    if (obj->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
    {
        // Objekt ist ein Spieler
        ClientMPlayer* cmp = static_cast<ClientMPlayer*>(obj);
        Equipement* equ =  cmp->m_equipement;

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

        // Schleife ueber die angehaengten Meshes
        Ogre::Entity::ChildObjectListIterator it = obj_ent->getAttachedObjectIterator();
        // mappt Namen von Knochen auf die daran anzuhaengenen Meshes
        map<std::string, std::string> goal_atch;
        map<std::string, std::string>::iterator jt;

        itm = equ->getItem(Equipement::WEAPON);
        if (itm !=0)
        {
            itmsubtype = itm->m_subtype;
            new_ent_name = getItemRenderInfo(itmsubtype).m_mesh;
            goal_atch.insert(make_pair("itemRightHand",new_ent_name));
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
                    DEBUG("replaced mesh %s by %s at bone %s",old_ent_name.c_str(),jt->second.c_str(),bone.c_str());


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

                DEBUG("removed mesh %s from bone %s",old_ent_name.c_str(),bone.c_str());
                // Mesh entfernen
                obj_ent->detachObjectFromBone(attch_ent);
                m_scene_manager->destroyEntity(attch_ent);
            }
        }

        // Behandeln der Meshes die an keinem Knochen gefunden wurden
        for (jt = goal_atch.begin(); jt != goal_atch.end();++jt)
        {
            bone = jt->first;
            DEBUG("attached mesh %s at bone %s",jt->second.c_str(),bone.c_str());
            attch_ent = m_scene_manager->createEntity(name+bone,jt->second);

            obj_ent->attachObjectToBone(bone,attch_ent);
            node = attch_ent->getParentNode();
            node->setInheritScale(false);


        }
    }

	//zeigt an ob ein Partikelsystem sichtbar is
	bool vis;

	for (i=0;i<NR_STATUS_MODS;i++)
	{
		num.str("");
		num <<"mod"<< i;

		mod_name = name + num.str();

		mod_part = m_scene_manager->getParticleSystem(mod_name);
		vis = mod_part->isVisible();

		if ( ((mask & (1 << i)) !=0) ^ vis)
		{
			mod_part->setVisible(!vis);
		}
	}

	// weitere Effekte anpassen
	mask =  obj->m_effects;

	for (i=0;i<NR_EFFECTS;i++)
	{
		num.str("");
		num <<"effect"<< i;

		mod_name = name + num.str();

		mod_part = m_scene_manager->getParticleSystem(mod_name);
		vis = mod_part->isVisible();

		if ( ((mask & (1 << i)) !=0) ^ vis)
		{
			mod_part->setVisible(!vis);
		}
	}
}

void Scene::deleteObject(std::string name)
{
	std::string node_name = name + "Node";

	DEBUG("deleting object %s",name.c_str());

	destroySceneNode(node_name);

}

void Scene::createObject(WorldObject* obj,std::string& name, bool is_static)
{
	std::string node_name = name + "Node";

	// Koordinaten des Objektes
	float x=obj->getGeometry()->m_shape.m_coordinate_x;
	float y=obj->getGeometry()->m_shape.m_coordinate_y;

	// Ortsvektor des Objektes
	Ogre::Vector3 vec(x*50,0,y*50);

	// Node anlegen
	Ogre::SceneNode* obj_node;
	obj_node =m_scene_manager->getRootSceneNode()->createChildSceneNode(node_name,vec);

	float angle = obj->getGeometry()->m_angle;
	// Objekt drehen
	obj_node->setDirection(cos(angle),0,sin(angle),Ogre::Node::TS_WORLD);

	// Informationen zum Rendern anfordern
	RenderInfo ri = getObjectRenderInfo(obj->getTypeInfo()->m_subtype);

	// Je nach Typ das richtige Mesh benutzen
	Ogre::Entity* obj_ent;
	obj_ent = m_scene_manager->createEntity(name, ri.m_mesh);

    obj_node->attachObject(obj_ent);

    // weitere Meshes anfuegen
    std::list<pair<std::string,std::string> >::iterator it;
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


	// eventuelle Partikeleffekte einfuegen



	if (!is_static)
	{
		// Partikelsysteme anhaengen
		Ogre::ParticleSystem* part=0;

		// TODO: richtige Partikelsystem benutzen
		// blind
		part = m_scene_manager->createParticleSystem(name+"mod0", "Blind");
		obj_node->attachObject(part);

		// vergiftet
		part = m_scene_manager->createParticleSystem(name+"mod1", "Poison");
		obj_node->attachObject(part);

		//  Berserker
		part = m_scene_manager->createParticleSystem(name+"mod2", "Berserk");
		obj_node->attachObject(part);

		// verwirrt
		part = m_scene_manager->createParticleSystem(name+"mod3", "Confuse");
		obj_node->attachObject(part);

		// stumm
		part = m_scene_manager->createParticleSystem(name+"mod4", "Mute");
		obj_node->attachObject(part);

		// gelaehmt
		part = m_scene_manager->createParticleSystem(name+"mod5", "Paralyze");
		obj_node->attachObject(part);

		// eingefroren
		part = m_scene_manager->createParticleSystem(name+"mod6", "Frozen");
		obj_node->attachObject(part);

		// brennend
		part = m_scene_manager->createParticleSystem(name+"mod7", "Burning");
		obj_node->attachObject(part);

		// Partikelsysteme fuer weitere Effekte
		// Bluteffekt
		part = m_scene_manager->createParticleSystem(name+"effect0", "Hit");
		obj_node->attachObject(part);
	}

}

void Scene::createItem(DropItem* di, std::string& name)
{
	std::string node_name = name + "Node";

	DEBUG5("created item %s",name.c_str());
	// Ortsvektor des Items
	Ogre::Vector3 vec(di->m_x*25,0,di->m_y*25);

	// in die Liste einfuegen
	m_drop_items->insert(make_pair(di->getId(),name));

	// Node anlegen
	Ogre::SceneNode* obj_node;
	obj_node =m_scene_manager->getRootSceneNode()->createChildSceneNode(node_name,vec);


	// Informationen zum Rendern anfordern
	RenderInfo ri = getItemRenderInfo(di->m_item->m_subtype);

	// Je nach Typ das richtige Mesh benutzen
	Ogre::Entity* ent;
	ent = m_scene_manager->createEntity(name, ri.m_mesh);

	obj_node->attachObject(ent);

}

void Scene::deleteItem(std::string name)
{
	std::string node_name = name + "Node";

	DEBUG("deleting item %s",name.c_str());

	destroySceneNode(node_name);

}

void Scene::updateProjectiles()
{
	map<int,Projectile*>* projectiles;
	map<int,Projectile*>::iterator it;
	projectiles = m_document->getProjectiles();
	Projectile* pr;

	map<int, string>::iterator it2;
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
		Ogre::Vector3 vec(pr->getGeometry()->m_coordinate_x*50,50,pr->getGeometry()->m_coordinate_y*50);

		// an die richtige Stelle verschieben
		m_scene_manager->getSceneNode(node_name)->setPosition(vec);

		// Objekt drehen
		float angle = pr->getGeometry()->m_angle;
		m_scene_manager->getSceneNode(node_name)->setDirection(cos(angle),0,sin(angle),Ogre::Node::TS_WORLD);


	}
}

void Scene::createProjectile(Projectile* pr, std::string& name)
{
	DEBUG5("creating projectile %s",name.c_str());

	std::string node_name = name + "Node";

	// Ortsvektor des Projektils
	Ogre::Vector3 vec(pr->getGeometry()->m_coordinate_x*50,50,pr->getGeometry()->m_coordinate_y*50);

	// in die Liste einfuegen
	m_projectiles->insert(make_pair(pr->getId(),name));

	// Node anlegen
	Ogre::SceneNode* obj_node;
	obj_node =m_scene_manager->getRootSceneNode()->createChildSceneNode(node_name,vec);

	Projectile::ProjectileType type = pr->getType();
	Ogre::Entity *ent =0;
	//Partikelsystem Feuer anlegen
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
		part = m_scene_manager->createParticleSystem(particle_name, ri.m_particle_system);
		obj_node->attachObject(part);
	}


	/*
	switch (type)
	{
		case Projectile::ARROW:
			// Entity erzeugen
			ent = m_scene_manager->createEntity(name,"arrow.mesh");
			obj_node->attachObject(ent);
		break;
		case Projectile::MAGIC_ARROW:
			part = m_scene_manager->createParticleSystem(particle_name, "Magic_Arrow");
			obj_node->attachObject(part);
		break;
		case Projectile::FIRE_BOLT:
			part = m_scene_manager->createParticleSystem(particle_name, "Firebolt");
			obj_node->attachObject(part);
			break;
		case Projectile:: FIRE_BALL:
			part = m_scene_manager->createParticleSystem(particle_name, "Fireball");
			obj_node->attachObject(part);
		break;
		case Projectile:: FIRE_WALL:
			part = m_scene_manager->createParticleSystem(particle_name, "Firewall");
			obj_node->attachObject(part);
			break;
		case Projectile::FIRE_WAVE:
			part = m_scene_manager->createParticleSystem(particle_name, "Firewave");
			obj_node->attachObject(part);
			break;
		case Projectile::ICE_BOLT:
			part = m_scene_manager->createParticleSystem(particle_name, "Icebolt");
			obj_node->attachObject(part);
			break;
		case Projectile:: BLIZZARD:
			part = m_scene_manager->createParticleSystem(particle_name, "Blizzard");
			obj_node->attachObject(part);
			break;
		case Projectile::ICE_RING:
			part = m_scene_manager->createParticleSystem(particle_name, "Icering");
			obj_node->attachObject(part);
			break;
		case Projectile::FREEZE:
			part = m_scene_manager->createParticleSystem(particle_name, "Freeze");
			obj_node->attachObject(part);
			break;
		case Projectile::LIGHTNING:
			part = m_scene_manager->createParticleSystem(particle_name, "Lightning");
			obj_node->attachObject(part);
			break;
		case Projectile::THUNDERSTORM:
			part = m_scene_manager->createParticleSystem(particle_name, "Thunderstorm");
			obj_node->attachObject(part);
			break;
		case Projectile::CHAIN_LIGHTNING:
			part = m_scene_manager->createParticleSystem(particle_name, "Chainlightning");
			obj_node->attachObject(part);
			break;
		case Projectile::STATIC_SHIELD:
			part = m_scene_manager->createParticleSystem(particle_name, "Static_Shield");
			obj_node->attachObject(part);
			break;
		case Projectile::FIRE_ARROW:
			ent = m_scene_manager->createEntity(name,"arrow.mesh");
			obj_node->attachObject(ent);
			part = m_scene_manager->createParticleSystem(particle_name, "Fire_Arrow");
			obj_node->attachObject(part);
			break;
		case Projectile::ICE_ARROW:
			ent = m_scene_manager->createEntity(name,"arrow.mesh");
			obj_node->attachObject(ent);
			part = m_scene_manager->createParticleSystem(particle_name, "Ice_Arrow");
			obj_node->attachObject(part);
			break;
		case Projectile::WIND_ARROW:
			ent = m_scene_manager->createEntity(name,"arrow.mesh");
			obj_node->attachObject(ent);
			part = m_scene_manager->createParticleSystem(particle_name, "Wind_Arrow");
			obj_node->attachObject(part);
			break;
		case Projectile::GUIDED_ARROW:
			ent = m_scene_manager->createEntity(name,"arrow.mesh");
			obj_node->attachObject(ent);
			part = m_scene_manager->createParticleSystem(particle_name, "Guided_Arrow");
			obj_node->attachObject(part);
			break;
		case Projectile::EXPLOSION:
			part = m_scene_manager->createParticleSystem(particle_name, "Explosion");
			obj_node->attachObject(part);
			break;
		case Projectile::FIRE_EXPLOSION:
			part = m_scene_manager->createParticleSystem(particle_name, "Fire_Explosion");
			obj_node->attachObject(part);
			break;
		case Projectile::ICE_EXPLOSION:
			part = m_scene_manager->createParticleSystem(particle_name, "Ice_Explosion");
			obj_node->attachObject(part);
			break;
		case Projectile::WIND_EXPLOSION:
			part = m_scene_manager->createParticleSystem(particle_name, "Wind_Explosion");
			obj_node->attachObject(part);
			break;
		case Projectile::LIGHT_BEAM:
			part = m_scene_manager->createParticleSystem(particle_name, "Lightbeam");
			obj_node->attachObject(part);
			break;
		case Projectile::ELEM_EXPLOSION:
			part = m_scene_manager->createParticleSystem(particle_name, "ElemExplosion");
			obj_node->attachObject(part);
			break;
		case Projectile::ACID:
			part = m_scene_manager->createParticleSystem(particle_name, "Acid");
			obj_node->attachObject(part);
			break;
		case Projectile::DIVINE_BEAM:
			part = m_scene_manager->createParticleSystem(particle_name, "Divine_Beam");
			obj_node->attachObject(part);
			break;
		case Projectile:: HYPNOSIS:
			part = m_scene_manager->createParticleSystem(particle_name, "Hypnosis");
			obj_node->attachObject(part);
			break;
		default:
				// Entity erzeugen
				ent = m_scene_manager->createEntity(name,"sphere.mesh");
				obj_node->attachObject(ent);
	}

*/




}

void Scene::deleteProjectile(std::string name)
{
	std::string node_name = name + "Node";

	DEBUG5("deleting projectile %s",name.c_str());

	destroySceneNode(node_name);
}

void Scene::destroySceneNode(std::string& node_name)
{

	Ogre::SceneNode* node = m_scene_manager->getSceneNode(node_name);

	if (node==0)
		return;


	// Iterator ueber die angehaengten Objekte
	Ogre::SceneNode::ObjectIterator it = node->getAttachedObjectIterator();

	std::string name;
	Ogre::MovableObject* obj;
	list<Ogre::MovableObject*> objects;
	list<Ogre::MovableObject*>::iterator i;

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
	// alle bisherigen Objekte aus der Szene loeschen
	m_scene_manager->clearScene();
	clearObjects();

	// Liste der statischen Objekte
	map<int,WorldObject*>* stat_objs = (m_document->getRegionData()->m_static_objects);
	map<int,WorldObject*>::iterator it;
	std::string name;
	for (it = stat_objs->begin(); it !=stat_objs->end();++it)
	{
		name = it->second->getNameId();

		// Objekt in der Szene erzeugen
		createObject(it->second,name,true);

	}

	// Tiles einfuegen
	insertTiles();

}

void  Scene::insertTiles()
{
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
}


