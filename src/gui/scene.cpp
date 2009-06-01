#include "scene.h"

#include "OgreCEGUIRenderer.h"
#include "OgreCEGUITexture.h"
#include "OgreCEGUIResourceProvider.h"
#include <OgrePanelOverlayElement.h>

#include "CEGUI.h"

#define USE_OBJECTLOADER
#define USE_ITEMLOADER

#include "graphicmanager.h"

std::map<Projectile::Subtype, RenderInfo> Scene::m_projectile_render_info;

std::map<GameObject::Subtype, RenderInfo> Scene::m_object_render_info;

std::map<Item::Subtype, RenderInfo> Scene::m_item_render_info;

std::map<GameObject::Subtype, std::map<Action::ActionType, std::vector<std::string> > > Scene::m_object_animations;

std::map<PlayerLook, RenderInfo> Scene::m_player_render_info;

std::multimap< GameObject::Subtype, std::pair<bool, PlayerLook> > Scene::m_player_look;


Scene::Scene(Document* doc,Ogre::RenderWindow* window)
{
	m_document = doc;
	m_window = window;
	m_scene_manager = Ogre::Root::getSingleton().getSceneManager("DefaultSceneManager");

	GraphicManager::init();
	GraphicManager::setSceneManager(m_scene_manager);
	
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
	m_particle_system_pool.clear();

	m_temp_player ="";

	// Setup fuer die Minimap
	Ogre::Camera* minimap_camera=m_scene_manager->createCamera("minimap_camera");
	minimap_camera->setNearClipDistance(500);
	minimap_camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	minimap_camera->setFOVy(Ogre::Degree(90.0));

	Ogre::TexturePtr minimap_texture = Ogre::TextureManager::getSingleton().createManual( "minimap_tex",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
   512, 512, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );

	Ogre::RenderTarget* minimap_rt = minimap_texture->getBuffer()->getRenderTarget();
	minimap_rt ->setAutoUpdated(false);

	Ogre::Viewport *v = minimap_rt->addViewport( minimap_camera );
	v->setClearEveryFrame( true );
	v->setOverlaysEnabled (false);
	v->setBackgroundColour(Ogre::ColourValue(0,0,0,0) );
	float ratio = Ogre::Real(v->getActualWidth()) / Ogre::Real(v->getActualHeight());
	DEBUG5("render target size %i %i",minimap_rt ->getWidth (), minimap_rt ->getHeight ());
	DEBUG5("viewport size %i %i ratio %f",v->getActualWidth(),v->getActualHeight(), ratio);
	minimap_camera->setAspectRatio(ratio);

	CEGUI::OgreCEGUITexture* ceguiTex = (CEGUI::OgreCEGUITexture*)((CEGUI::OgreCEGUIRenderer*)CEGUI::System::getSingleton().getRenderer())->createTexture((CEGUI::utf8*)"minimap_tex");

	CEGUI::Imageset* textureImageSet = CEGUI::ImagesetManager::getSingleton().createImageset("minimap", ceguiTex);

	textureImageSet->defineImage( "minimap_img",
				CEGUI::Point( 0.0f, 0.0f ),
				CEGUI::Size( ceguiTex->getWidth(), ceguiTex->getHeight() ),
				CEGUI::Point( 0.0f, 0.0f ) );

	// Setup fuer die Spieleransicht

	Ogre::SceneManager* char_scene_mng = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC,"CharacterSceneManager");
	char_scene_mng->setAmbientLight(Ogre::ColourValue(1,1,1));
			
	Ogre::Camera* char_camera = char_scene_mng->createCamera("char_camera");
	char_camera->setPosition(Ogre::Vector3(250, 80,0));
	char_camera->lookAt(Ogre::Vector3(0,80,0));
	char_camera->setNearClipDistance(5);
			
	char_camera->setAspectRatio(1.0);
			
		
			
	Ogre::Entity *player = char_scene_mng->createEntity("Player", "warrior_m.mesh");
			
	Ogre::SceneNode *playerNode = char_scene_mng->getRootSceneNode()->createChildSceneNode("characterNode");
	playerNode->attachObject(player);
	

	Ogre::SceneNode *camNode = char_scene_mng->getRootSceneNode()->createChildSceneNode("CharacterCamNode");
	camNode->attachObject(char_camera);
			
	Ogre::TexturePtr char_texture = Ogre::TextureManager::getSingleton().createManual( "character_tex",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
   512, 512, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );

	Ogre::RenderTarget* char_rt = char_texture->getBuffer()->getRenderTarget();
	char_rt ->setAutoUpdated(false);

	Ogre::Viewport *char_view = char_rt->addViewport(char_camera );
	char_view->setClearEveryFrame( true );
	char_view->setOverlaysEnabled (false);
	char_view->setBackgroundColour(Ogre::ColourValue(0,0,0,1.0) );
	char_rt->update();
	
	CEGUI::OgreCEGUITexture* char_ceguiTex = (CEGUI::OgreCEGUITexture*)((CEGUI::OgreCEGUIRenderer*) CEGUI::System::getSingleton().getRenderer())->createTexture((CEGUI::utf8*)"character_tex");

	CEGUI::Imageset* char_textureImageSet = CEGUI::ImagesetManager::getSingleton().createImageset("character", char_ceguiTex);

	char_textureImageSet->defineImage( "character_img",
			CEGUI::Point( 0.0f, 0.0f ),
			CEGUI::Size( char_ceguiTex->getWidth(), char_ceguiTex->getHeight() ),
			CEGUI::Point( 0.0f, 0.0f ) );
	
	/*
	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("RttMat",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mat->getTechnique(0)->getPass(0)->createTextureUnitState("character_tex");

			
	Ogre::Overlay* overlay = Ogre::OverlayManager::getSingleton().create("TextureOverlay");
	Ogre::PanelOverlayElement* container = (Ogre::PanelOverlayElement*)Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "TextureOverlayPanel");
	overlay->add2D(container);
	container->setDimensions(0.4, 0.4);
	container->setPosition(0.6, 0.6);
	container->setMaterialName("RttMat");
	overlay->show();
*/
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
	registerPlayerLook("warrior","warrior_m","warrior_m.mesh", true);
	registerPlayerLook("warrior","warrior_f","archer_f.mesh", false);

	registerPlayerLook("mage","mage_m","mage_m.mesh", true);
	registerPlayerLook("priest","priest_f","priest_f.mesh", false);
	registerPlayerLook("archer","archer_f","archer_f.mesh",false);

	// Spieler
	registerObject("warrior","warrior_m.mesh","");
	registerObject("mage","mage_m.mesh","");	// TODO
	registerObject("priest","priest_f.mesh",""); // TODO
	registerObject("archer","archer_f.mesh",""); // TODO

	// feste Objekte

	registerObject("smallWall1","smallWall1.mesh","");
	registerObject("smallWall2","smallWall2.mesh","");
	registerObject("stones3","stones3.mesh","");

	// Geschosse
	registerProjectile("ARROW","arrow.mesh","");
	registerProjectile("MAGIC_ARROW","","Magic_Arrow");
	registerProjectile("FIRE_BOLT","","Firebolt");
	registerProjectile("FIRE_BALL","","Fireball");
	registerProjectile("FIRE_WALL","","Firewall");
	registerProjectile("FIRE_WAVE","","Firewave");
	registerProjectile("ICE_BOLT","","Icebolt");
	registerProjectile("BLIZZARD","","Blizzard");
	registerProjectile("ICE_RING","","Icering");
	registerProjectile("FREEZE","","Freeze");
	registerProjectile("LIGHTNING","","Lightning");
	registerProjectile("THUNDERSTORM","","Thunderstorm");
	registerProjectile("CHAIN_LIGHTNING","","Chainlightning");
	registerProjectile("STATIC_SHIELD","","Static_Shield");
	registerProjectile("FIRE_ARROW","arrow.mesh","Fire_Arrow");
	registerProjectile("ICE_ARROW" ,"arrow.mesh","Ice_Arrow");
	registerProjectile("WIND_ARROW","arrow.mesh","Wind_Arrow");
	registerProjectile("GUIDED_ARROW","arrow.mesh","Guided_Arrow");
	registerProjectile("EXPLOSION","","Explosion");
	registerProjectile("FIRE_EXPLOSION","","Fire_Explosion");
	registerProjectile("ICE_EXPLOSION","","Ice_Explosion");
	registerProjectile("WIND_EXPLOSION","","Wind_Explosion");
	registerProjectile("LIGHT_BEAM","","Lightbeam");
	registerProjectile("ELEM_EXPLOSION","","ElemExplosion");
	registerProjectile("ACID","","Acid");
	registerProjectile("DIVINE_BEAM","","Divine_Beam");
	registerProjectile("HYPNOSIS","","Hypnosis");

	GraphicManager::registerGraphicMapping("ARROW","arrow");
	GraphicManager::registerGraphicMapping("MAGIC_ARROW","magic_arrow");
	GraphicManager::registerGraphicMapping("FIRE_BOLT","fire_bolt");
	GraphicManager::registerGraphicMapping("FIRE_BALL","fire_ball");
	GraphicManager::registerGraphicMapping("FIRE_WALL","fire_wall");
	GraphicManager::registerGraphicMapping("FIRE_WAVE","fire_wave");
	GraphicManager::registerGraphicMapping("ICE_BOLT","ice_bolt");
	GraphicManager::registerGraphicMapping("BLIZZARD","blizzard");
	GraphicManager::registerGraphicMapping("ICE_RING","ice_ring");
	GraphicManager::registerGraphicMapping("FREEZE","freeze");
	GraphicManager::registerGraphicMapping("LIGHTNING","lightning");
	GraphicManager::registerGraphicMapping("THUNDERSTORM","thunderstorm");
	GraphicManager::registerGraphicMapping("CHAIN_LIGHTNING","chain_lightning");
	GraphicManager::registerGraphicMapping("STATIC_SHIELD","static_field");
	GraphicManager::registerGraphicMapping("FIRE_ARROW","fire_arrow");
	GraphicManager::registerGraphicMapping("ICE_ARROW" ,"ice_arrow");
	GraphicManager::registerGraphicMapping("WIND_ARROW","wind_arrow");
	GraphicManager::registerGraphicMapping("GUIDED_ARROW","guided_arrow");
	GraphicManager::registerGraphicMapping("EXPLOSION","explosion");
	GraphicManager::registerGraphicMapping("FIRE_EXPLOSION","fire_explosion");
	GraphicManager::registerGraphicMapping("ICE_EXPLOSION","ice_explosion");
	GraphicManager::registerGraphicMapping("WIND_EXPLOSION","wind_explosion");
	GraphicManager::registerGraphicMapping("LIGHT_BEAM","light_beam");
	GraphicManager::registerGraphicMapping("ELEM_EXPLOSION","elem_explosion");
	GraphicManager::registerGraphicMapping("ACID","acid");
	GraphicManager::registerGraphicMapping("DIVINE_BEAM","divine_beam");
	GraphicManager::registerGraphicMapping("HYPNOSIS","hypnosis");

	
	GraphicManager::registerGraphicMapping("gold","gold.mesh");

}

void Scene::registerObject(GameObject::Subtype subtype, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_object_render_info.insert(std::make_pair(subtype,RenderInfo(mesh,particle_system,scaling_factor)));
}

void Scene::registerObjectAnimations(GameObject::Subtype subtype, std::map<Action::ActionType, std::vector<std::string> > &animations)
{
	if (!animations.empty())
	{
		m_object_animations[subtype] = animations;
	}
}

void Scene::registerAttachedMesh(GameObject::Subtype subtype, std::string bone, std::string mesh)
{
    // Renderinformationen suchen
    std::map<GameObject::Subtype, RenderInfo>::iterator it = m_object_render_info.find(subtype);
    if (it != m_object_render_info.end())
    {
        // gefunden
        it->second.m_extra_meshes.push_back(std::make_pair(bone,mesh));
    }
}

void Scene::registerPlayerLook(GameObject::Subtype subtype, PlayerLook look, std::string mesh, bool male)
{
	m_player_render_info.insert(std::make_pair(look, RenderInfo(mesh, "")));
	m_player_look.insert(std::make_pair(subtype, std::make_pair(male,look)));
}

void Scene::registerItem(Item::Subtype subtype, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_item_render_info.insert(std::make_pair(subtype,RenderInfo(mesh,particle_system,scaling_factor)));
}



void Scene::registerProjectile(Projectile::Subtype type, std::string mesh, std::string particle_system, float scaling_factor)
{
	m_projectile_render_info.insert(std::make_pair(type,RenderInfo(mesh,particle_system,scaling_factor)));
}

RenderInfo  Scene::getObjectRenderInfo(GameObject::Subtype subtype)
{
	std::map<GameObject::Subtype, RenderInfo>::iterator i= m_object_render_info.find(subtype);
	if (i != m_object_render_info.end())
	{
		return i->second;
	}
	else
	{
		// Standardmesh
		DEBUG("no mesh for %s",subtype.c_str());
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

RenderInfo  Scene::getProjectileRenderInfo(Projectile::Subtype type)
{
	std::map<Projectile::Subtype, RenderInfo>::iterator i= m_projectile_render_info.find(type);
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

void Scene::getPlayerLook(GameObject::Subtype subtype, std::list< std::pair<bool, PlayerLook> > &looks)
{
	std::multimap< GameObject::Subtype, std::pair<bool, PlayerLook> >::iterator it,jt;
	it = m_player_look.lower_bound(subtype);
	jt = m_player_look.upper_bound(subtype);

	while (it != jt)
	{
		looks.push_back(it->second);
		++it;
	}
}


std::pair<float,float> Scene::getProjection(Vector pos, float height)
{
	Ogre::Vector4 ipos(pos.m_x*50,height*50,pos.m_y*50,1);
	Ogre::Vector4 projpos;
	projpos = m_camera->getProjectionMatrix()*m_camera->getViewMatrix()*ipos;

	return std::make_pair(0.5 + 0.5*projpos.x / projpos.w, 0.5 - 0.5*projpos.y / projpos.w);
}

void Scene::changeViewportSize(ViewportSize size)
{
	m_viewport_size = size;
	/*
	if (size == VIEW_FULL)
	{
		m_viewport->setDimensions(0,0,1,1);
	}
	else if (size == VIEW_LEFT)
	{
		m_viewport->setDimensions(0,0,0.6,1);
	}
	else if (size == VIEW_RIGHT)
	{
		m_viewport->setDimensions(0.4,0,1,1);
	}

	m_camera->setAspectRatio(Ogre::Real(m_viewport->getActualWidth()) / Ogre::Real(m_viewport->getActualHeight()));
	*/
}

GraphicObject* Scene::createGraphicObject(GameObject* gobj)
{
	// Typ des GraphicObjekts ermitteln
	std::string otype;
	Player* pl = dynamic_cast<Player*>(gobj);
	GraphicObject::Type type;
	if (pl != 0 )
	{
		type = pl->getPlayerLook();
	}
	else
	{
		otype = gobj->getSubtype();
		type = GraphicManager::getGraphicType(otype);
	}
	
	
	
	// GraphicObjekt erstellen und ausgeben
	if (type == "")
	{
		return 0;
	}
	else
	{
		DEBUG5("create graphic Object typ %s for %s",type.c_str(), otype.c_str());
		return GraphicManager::createGraphicObject(type,gobj->getNameId(), gobj->getId());
	}
}


void Scene::update(float ms)
{
	DEBUG5("update scene");

	// Spielerobjekt
	Player* player = m_document->getLocalPlayer();

	if (player ==0)
		return;

	updateCharacterView();

	if (player->getRegion() ==0)
		return;

	// Nummer der region in der sich der Spieler befindet
	short region_nr = player->getRegionId();




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
	DEBUG5("cam position %f %f %f",pos.m_x*50 + r*cos(theta)*cos(phi), r*sin(theta), pos.m_y*50 - r*cos(theta)*sin(phi));

	Ogre::Light* light= m_scene_manager->getLight("HeroLight");
	light->setPosition(Ogre::Vector3(pos.m_x*50,300,pos.m_y*50));

	updateGraphicObjects(ms);
	
	/*
	// alle Objekte aktualisieren
	updateObjects();


	DEBUG5("update items");
	// alle Items aktualisieren
	updateItems();


	updateProjectiles();
*/
}

void  Scene::insertObject(GameObject* gobj, bool is_static)
{
	GraphicObject* obj = createGraphicObject(gobj);

	DEBUG5("insert graphic object for %s",gobj->getNameId().c_str());
	if (is_static)
	{
		m_static_objects.insert(std::make_pair(gobj->getId(), obj));
	}
	else
	{
		m_graphic_objects.insert(std::make_pair(gobj->getId(), obj));
	}
	if (obj != 0)
	{	
		Ogre::SceneNode* node = obj->getTopNode();
		
		// Monster zufaellig skalieren
		if (gobj->getType() == "MONSTER")
		{
			float scale = 0.9 + 0.2*Random::random();
			node->setScale(scale,scale,scale);
		
		}
		
		if (gobj->getBaseType() == GameObject::WORLDOBJECT)
		{
			if (gobj->getState() == WorldObject::STATE_ACTIVE)
			{
				obj->setQueryMask(WORLDOBJECT);
			}
		}
		else if (gobj->getBaseType() == GameObject::DROPITEM)
		{
			obj->setQueryMask(ITEM);
		}
		
		updateGraphicObject(obj,gobj,0.0);
	}
}

void Scene::updateGraphicObject(GraphicObject* obj, GameObject* gobj,float time)
{
	if (obj ==0 || gobj==0)
		return;
	
	Ogre::SceneNode* node = obj->getTopNode();
	
	// Objektes positionieren
	float x=gobj->getShape()->m_center.m_x;
	float z=gobj->getShape()->m_center.m_y;
	float y = gobj->getHeight();
	Ogre::Vector3 vec(x*50,y*50,z*50);
	node->setPosition(vec);
		
	// Objekt drehen
	float angle = gobj->getShape()->m_angle;
	node->setDirection(sin(angle),0,-cos(angle),Ogre::Node::TS_WORLD);
	
	
	DropItem* di = static_cast<DropItem*>(gobj);
	if (di != 0)
	{
		float anglex = di->getAngleX();
		node->pitch(Ogre::Radian(anglex));
	}
	
	
	// Aktion setzen
	std::string action = gobj->getActionString();
	float perc = gobj->getActionPercent();
	
	obj->updateAction(action,perc);
	DEBUG5("object %s action %s perc %f",gobj->getNameId().c_str(), action.c_str(), perc);

	Player* pl = dynamic_cast<Player*>(gobj);
	if (pl != 0)
	{
		updatePlayerGraphicObject(obj,pl);
	}
	
	obj->update(time);
}

bool Scene::updatePlayerGraphicObject(GraphicObject* obj, Player* pl)
{
	MovableObjectInfo minfo;
	bool update = false;
	
	// Waffe
	minfo.m_objectname="weapon";
	minfo.m_type = MovableObjectInfo::SUBOBJECT;
	
	Item* itm;
	itm = pl->getWeapon();
	if (itm !=0 && itm->m_weapon_attr !=0)
	{
		if (itm->m_weapon_attr->m_weapon_type == WeaponAttr::BOW || itm->m_weapon_attr->m_weapon_type == WeaponAttr::CROSSBOW)
		{
			minfo.m_bone = "itemLeftHand";
		}
		else
		{
			minfo.m_bone = "itemRightHand";
		}
		minfo.m_source = GraphicManager::getGraphicType(itm->m_subtype);
	}
	else
	{
		minfo.m_source="";
	}
	obj->updateSubobject(minfo);
	
	// Schild
	minfo.m_objectname="shield";
	minfo.m_type = MovableObjectInfo::SUBOBJECT;
	itm = pl->getShield();
	if (itm !=0)
	{
		minfo.m_bone = "itemLeftHand";
		minfo.m_source = GraphicManager::getGraphicType(itm->m_subtype);
	}
	else
	{
		minfo.m_source="";
	}
	obj->updateSubobject(minfo);
	
	DEBUG5("update %s : bone %s source %s", minfo.m_objectname.c_str(), minfo.m_bone.c_str(), minfo.m_source.c_str());
	return update;
}

void  Scene::deleteGraphicObject(int id)
{
	std::map<int,GraphicObject*>::iterator it;
	it = m_graphic_objects.find(id);
	if (it != m_graphic_objects.end())
	{
		GraphicManager::destroyGraphicObject(it->second);
		m_graphic_objects.erase(id);
	}
	else
	{
		it = m_static_objects.find(id);
		if (it !=  m_static_objects.end())
		{
			GraphicManager::destroyGraphicObject(it->second);
			m_static_objects.erase(id);
		}
	}
}

void Scene::updateGraphicObjects(float time)
{
	Player* player = m_document->getLocalPlayer();
	GameObjectMap& game_objs = player->getRegion()->getGameObjects();
	
	GameObjectMap::iterator it = game_objs.begin();
	std::map<int,GraphicObject*>::iterator jtold, jt = m_graphic_objects.begin();
	
	while (it != game_objs.end() || jt != m_graphic_objects.end())
	{
		if (it == game_objs.end() || (jt != m_graphic_objects.end() && it->first > jt->first))
		{
			// Objekt jt existiert als Graphisch, aber nicht mehr im Spiel
			DEBUG5("deleting graphic object %i",jt->first);
			jtold = jt;
			++jt;
			
			deleteGraphicObject(jtold->first);
		}
		else if (jt == m_graphic_objects.end() || (it != game_objs.end() && it->first < jt->first))
		{
			// Objekt it existiert im Spiel, aber noch nicht graphisch
			DEBUG5("inserting graphic object %i",it->first);	
			
			insertObject(it->second,false);
			
			++it;
		}
		else if (it->first == jt->first)
		{
			// GraphikObjekt aktualisieren
			DEBUG5("update graphic objekt %i",it->first);
			updateGraphicObject(jt->second, it->second,time);
			++it;
			++jt;
		}
		else
		{
			ERRORMSG("Fehler beim Abgleich Graphik <-> ingame");
		}
	}
	
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

		DEBUG5("object %s %i at %f %f",obj->getSubtype().c_str(), obj->getId(), obj->getShape()->m_center.m_x,obj->getShape()->m_center.m_y);
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
			Vector pos = di->getPosition();
			float z = di->getHeight();

			// Ortsvektor des Objektes
			Ogre::Vector3 vec(pos.m_x*50,z*50,pos.m_y*50);

			// an die richtige Stelle verschieben
			Ogre::SceneNode* itm_node = m_scene_manager->getSceneNode(node_name);
			itm_node->setPosition(vec);

			float angle = di->getShape()->m_angle;
			itm_node->setDirection(sin(angle),0,-cos(angle),Ogre::Node::TS_WORLD);

			angle = di->getAngleX();
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
	Ogre::Entity* obj_ent=0;
	Ogre::SceneNode* node;
	
	if (!m_scene_manager->hasSceneNode(node_name))
	{
		// Objekt existiert noch nicht in der Szene


		//Objekt anlegen
		createObject(obj,name);
		DEBUG5("created obj %s",name.c_str());
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
	node->setDirection(sin(angle),0,-cos(angle),Ogre::Node::TS_WORLD);

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
	
	if (obj->getType() != "FIXED_OBJECT")
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
		if (m_object_animations.count(cr->getSubtype()) != 0)
		{
			animations = &(m_object_animations[cr->getSubtype()][act]);
		}

		std::string anim_name = "";
		if (!animations->empty())
		{
			anim_name = (*animations)[cr->getAction()->m_animation_number % animations->size()];
		}

		obj_ent = m_scene_manager->getEntity(name);
		if (obj_ent == 0)
		{
				ERRORMSG("Object %s not found",name.c_str());
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
    if (obj->getType() == "PLAYER")
    {
        // Objekt ist ein Spieler
        Player* cmp = static_cast<Player*>(obj);
        //Equipement* equ =  cmp->getEquipement();

		updatePlayer(cmp,obj_ent,m_scene_manager);
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
	if (obj->getType() == "PLAYER")
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

	if (obj->getType() != "FIXED_OBJECT")
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
				SoundTarget target = cr->getSubtype();

				if (obj->getType() == "PLAYER")
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
			SoundTarget target = cr->getSubtype();

			if (obj->getType() == "PLAYER")
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

bool Scene::updatePlayer(Player* pl, Ogre::Entity* obj_ent, Ogre::SceneManager* scene_manager)
{
	bool ret = false;
	
	std::string name = pl->getNameId();
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

	itm = pl->getWeapon();
	if (itm !=0 && itm->m_weapon_attr !=0)
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

	itm = pl->getShield();
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
				scene_manager->destroyEntity(attch_ent);

				attch_ent = scene_manager->createEntity(name+bone,jt->second);
				obj_ent->attachObjectToBone(bone,attch_ent);

				node = attch_ent->getParentNode();
				node->setInheritScale(false);
				
				ret = true;
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
			scene_manager->destroyEntity(attch_ent);
			
			ret = true;
		}
	}

    // Behandeln der Meshes die an keinem Knochen gefunden wurden
	for (jt = goal_atch.begin(); jt != goal_atch.end();++jt)
	{
		bone = jt->first;
		DEBUG5("attached mesh %s at bone %s",jt->second.c_str(),bone.c_str());
		attch_ent = scene_manager->createEntity(name+bone,jt->second);

		obj_ent->attachObjectToBone(bone,attch_ent);
		node = attch_ent->getParentNode();
		node->setInheritScale(false);

		ret = true;
	}
	return ret;
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
	obj_node->setDirection(sin(angle),0,-cos(angle),Ogre::Node::TS_WORLD);

	// Informationen zum Rendern anfordern
	RenderInfo ri;
	Player* pl = dynamic_cast<Player*>(obj);
	if (pl != 0 )
	{
		ri = getPlayerRenderInfo(pl->getPlayerLook());
	}
	else
	{
		ri = getObjectRenderInfo(obj->getSubtype());
	}

	// Je nach Typ das richtige Mesh benutzen
	Ogre::Entity* obj_ent;
	obj_ent = m_scene_manager->createEntity(name, ri.m_mesh);

    obj_node->attachObject(obj_ent);
	
	// Monster zufaellig skalieren
	if (obj->getType() == "MONSTER")
	{
		float scale = 0.9 + 0.2*Random::random();
		obj_node->setScale(scale,scale,scale);
		
	}

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
	if (obj->getType() == "PLAYER")
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
	if (di->getHeight() > 0.1f)
	{
		SoundTarget target = di->getSubtype();
		target += ":drop";
		SoundName sound = SoundSystem::getSoundName(target);
		obj->setSound(sound);
		obj->play();
	}
	DEBUG5("created item %s",name.c_str());
	// Ortsvektor des Items
	Vector pos = di->getPosition();
	Ogre::Vector3 vec(pos.m_x*50,0,pos.m_y * 50);

	// in die Liste einfuegen
	m_drop_items->insert(std::make_pair(di->getId(),name));

	// Node anlegen
	Ogre::SceneNode* obj_node;
	obj_node =m_scene_manager->getRootSceneNode()->createChildSceneNode(node_name,vec);


	// Informationen zum Rendern anfordern
	RenderInfo ri = getItemRenderInfo(di->getSubtype());

	// Je nach Typ das richtige Mesh benutzen
	Ogre::Entity* ent;
	ent = m_scene_manager->createEntity(name, ri.m_mesh);

	// Objekt drehen
	float angle = di->getShape()->m_angle;
	obj_node->setDirection(sin(angle),0,-cos(angle),Ogre::Node::TS_WORLD);

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
		Ogre::Vector3 vec(pr->getShape()->m_center.m_x*50,110,pr->getShape()->m_center.m_y*50);

		// an die richtige Stelle verschieben
		m_scene_manager->getSceneNode(node_name)->setPosition(vec);

		// Objekt drehen
		float angle = pr->getShape()->m_angle;
		m_scene_manager->getSceneNode(node_name)->setDirection(sin(angle),0,-cos(angle),Ogre::Node::TS_WORLD);

		if (pr->getTimer()<200)
		{
			SoundTarget target = SoundSystem::getProjectileSound(pr->getSubtype());
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
	Ogre::Vector3 vec(pr->getShape()->m_center.m_x*50,110,pr->getShape()->m_center.m_y*50);

	// in die Liste einfuegen
	m_projectiles->insert(std::make_pair(pr->getId(),name));

	DEBUG5("speed %f %f",pr->getSpeed().m_x, pr->getSpeed().m_y);
	DEBUG5("angle %f ",pr->getShape()->m_angle*180/3.14);

	// Node anlegen
	Ogre::SceneNode* obj_node;
	obj_node =m_scene_manager->getRootSceneNode()->createChildSceneNode(node_name,vec);

	Projectile::Subtype type = pr->getSubtype();
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
	DEBUG5("deleting scene node %s",node_name.c_str());
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

	Ogre::Entity* attch_ent;
	Ogre::Entity* obj_ent;

	std::list<Ogre::Entity*> attch_obj;
	std::list<Ogre::Entity*>::iterator kt;


	 // Knochen an den ein Mesh angehaengt ist
	std::string bone;



	while (it.hasMoreElements())
	{
		// Name des aktuellen Objektes
		name = it.peekNextKey();
		// Zeiger auf aktuelles Objekt
		obj = it.peekNextValue();

		DEBUG5("deleting object %s",name.c_str());

		objects.push_back(obj);

		// Schleife ueber die angehaengten Kindelemente
		obj_ent = m_scene_manager->getEntity(name);

		attch_obj.clear();

		Ogre::Entity::ChildObjectListIterator iter = obj_ent->getAttachedObjectIterator();
		while (iter.hasMoreElements())
		{
			bone = iter.peekNextKey();
			bone = bone.substr(name.size());
			DEBUG5("attached mesh %s",bone.c_str());

			attch_ent = static_cast<Ogre::Entity*>(iter.getNext());
			attch_obj.push_back(attch_ent);
		}

		obj_ent->detachAllObjectsFromBone();
		for (kt = attch_obj.begin(); kt != attch_obj.end(); ++kt)
		{
			m_scene_manager->destroyMovableObject(*kt);
		}

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

void Scene::updateCharacterView()
{
	
	bool update = false;
	
	Ogre::SceneManager* scene_mgr = Ogre::Root::getSingleton().getSceneManager("CharacterSceneManager");

	Player* pl = m_document->getLocalPlayer();

	if ((pl ==0 && m_temp_player!=""))
	{
		m_temp_player="";
		scene_mgr->clearScene();
		update = true;
	}

	if (pl !=0)
	{
		if ((pl->getNameId() != m_temp_player))
		{
			scene_mgr->clearScene();
			update = true;
		}
		m_temp_player = pl->getNameId();
		
		
		Ogre::SceneNode* node;
		if (!scene_mgr->hasSceneNode("characterNode"))
		{
			node = scene_mgr->getRootSceneNode()->createChildSceneNode("characterNode");
			update = true;
		}
		else
		{
			node = scene_mgr->getSceneNode("characterNode");
		}
	
		RenderInfo ri;
		ri = getPlayerRenderInfo(pl->getPlayerLook());
	
		Ogre::Entity* obj_ent;
		if (!scene_mgr->hasEntity("characterEntity"))
		{
			obj_ent = scene_mgr->createEntity("characterEntity", ri.m_mesh);
			node->attachObject(obj_ent);
			update = true;
		}
		else
		{
			obj_ent = scene_mgr->getEntity("characterEntity");
		}
		
		update |= updatePlayer(pl,obj_ent,scene_mgr);
		
	}	
	
	if (update)
	{
		Ogre::Resource* res= Ogre::TextureManager::getSingleton().createOrRetrieve ("character_tex",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first.getPointer();
		Ogre::Texture* texture = dynamic_cast<Ogre::Texture*>(res);
		Ogre::RenderTarget* target = texture->getBuffer()->getRenderTarget();
	
	
		target->update();
		DEBUG5("player image is now %s",m_temp_player.c_str());
	}
	
}


void Scene::clearObjects()
{
	m_objects->clear();
	m_drop_items->clear();
	m_projectiles->clear();
	
	std::map<int,GraphicObject*>::iterator it;
	for (it = m_static_objects.begin(); it != m_static_objects.end(); ++it)
	{
		GraphicManager::destroyGraphicObject(it->second);
	}
	m_static_objects.clear();
	
	for (it = m_graphic_objects.begin(); it != m_graphic_objects.end(); ++it)
	{
		GraphicManager::destroyGraphicObject(it->second);
	}
	m_graphic_objects.clear();
}

void Scene::createScene()
{
	DEBUG5("create Scene");

	// alle Partikelsysteme loeschen
	std::multimap<std::string, Ogre::ParticleSystem*>::iterator kt;
	for (kt = m_particle_system_pool.begin(); kt !=  m_particle_system_pool.end(); ++kt)
	{
		DEBUG5("destroy particle system %s",kt->second->getName().c_str());
		m_scene_manager->destroyParticleSystem(kt->second);
	}
	m_particle_system_pool.clear();

	// alle bisherigen Objekte aus der Szene loeschen
	clearObjects();
	m_scene_manager->clearScene();
	SoundSystem::clearObjects();


	// Liste der statischen Objekte
	Ogre::StaticGeometry* static_geom = m_scene_manager->createStaticGeometry ("StaticGeometry");

	std::list<WorldObject*> stat_objs;

	Region* region = m_document->getLocalPlayer()->getRegion();

	float *colour;
	m_scene_manager->setAmbientLight(Ogre::ColourValue(0.4,0.4,0.4));
	
	colour= region->getHeroLight();
	Ogre::Light *light = m_scene_manager->createLight("HeroLight");
	light->setType(Ogre::Light::LT_POINT);
	light->setDiffuseColour(colour[0], colour[1], colour[2]);
	light->setSpecularColour(0.0, 0.0, 0.0);
	light->setAttenuation(1000,0.5,0.000,0.00001);
	DEBUG5("hero light %f %f %f",colour[0], colour[1], colour[2]);

	colour= region->getDirectionalLight();
	light = m_scene_manager->createLight("RegionLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDiffuseColour(colour[0], colour[1], colour[2]);
	light->setSpecularColour(colour[0], colour[1], colour[2]);
	light->setDirection(Ogre::Vector3(-1,-1,-1));
	DEBUG5("directional light %f %f %f",colour[0], colour[1], colour[2]);

	if (region !=0)
	{
		WorldObjectMap* stat_objs = region->getStaticObjects();
		WorldObjectMap::iterator it;
		for (it = stat_objs->begin(); it !=stat_objs->end();++it)
		{
			// Objekt in der Szene erzeugen
			insertObject(it->second,true);
		}

		short dimx = region->getDimX();
		short dimy = region->getDimY();
		Ogre::Camera* minimap_camera=m_scene_manager->getCamera("minimap_camera");
		DEBUG5("camera pos %i %i %i",dimx*100,std::max(dimx,dimy)*300,dimy*100);
		minimap_camera->setPosition(Ogre::Vector3(dimx*100,std::max(dimx,dimy)*200,10+dimy*100));
		minimap_camera->lookAt(Ogre::Vector3(dimx*100,0,dimy*100));
		minimap_camera->setNearClipDistance(std::max(dimx,dimy)*100);

		Ogre::Resource* res= Ogre::TextureManager::getSingleton().createOrRetrieve ("minimap_tex",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first.getPointer();
		Ogre::Texture* texture = dynamic_cast<Ogre::Texture*>(res);
		Ogre::RenderTarget* target = texture->getBuffer()->getRenderTarget();

		//m_minimap_camera->setPosition(Ogre::Vector3(6600,1000,11400));
		//m_minimap_camera->lookAt(Ogre::Vector3(6600,0,11380));
		//Ogre::Vector3 up = m_minimap_camera->getUp();
		//m_minimap_camera->setFrustumExtents (0,dimx*200,0,dimy*200);
		//DEBUG("camera up %f %f %f",up.x, up.y, up.z);



		m_scene_manager->setAmbientLight(Ogre::ColourValue(1.0,1.0,1.0));
		target->update();

		colour= region->getAmbientLight();
		m_scene_manager->setAmbientLight(Ogre::ColourValue(colour[0], colour[1], colour[2]));
		DEBUG5("ambient light %f %f %f",colour[0], colour[1], colour[2]);
		//m_scene_manager->setAmbientLight(Ogre::ColourValue(0.0,0.0,0.0));

		// Boden erstellen
		if (region->getGroundMaterial() != "")
		{
			std::stringstream stream;
			short dimx = region->getDimX();
			short dimy = region->getDimY();
			Ogre::SceneNode *node;
			Ogre::Entity* ground;
			for (int i=0; i< dimx; ++i)
			{
				for (int j=0; j<dimy; ++j)
				{
					stream.str("");
					stream << "GroundNode"<<i<<"_"<<j;
					node = m_scene_manager->getRootSceneNode()->createChildSceneNode(stream.str());
					node->setPosition(Ogre::Vector3(i*200+100,0,j*200+100));

					stream.str("");
					stream << "GroundEntity"<<i<<"_"<<j;
					ground = m_scene_manager->createEntity(stream.str(), "ground");
					ground->setMaterialName(region->getGroundMaterial());
					ground->setCastShadows(false);
					ground->setQueryFlags(0);
					node->attachObject(ground);

					static_geom->addSceneNode(node);
				}
			}
		}


	}

	m_temp_player="";

}

