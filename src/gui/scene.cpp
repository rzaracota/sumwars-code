#include "scene.h"

#include "OgreCEGUIRenderer.h"
#include "OgreCEGUITexture.h"
#include "OgreCEGUIResourceProvider.h"
#include <OgrePanelOverlayElement.h>

#include "CEGUI.h"

#include "graphicmanager.h"





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

	registerMeshes();

	m_region_id = -1;
	
	m_temp_player ="";
	m_temp_player_object =0;

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
	DEBUGX("render target size %i %i",minimap_rt ->getWidth (), minimap_rt ->getHeight ());
	DEBUGX("viewport size %i %i ratio %f",v->getActualWidth(),v->getActualHeight(), ratio);
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
	char_camera->setPosition(Ogre::Vector3(180, 60,0));
	char_camera->lookAt(Ogre::Vector3(0,60,0));
	char_camera->setNearClipDistance(5);
			
	char_camera->setAspectRatio(1.0);
	

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
}


void Scene::registerMeshes()
{
	// Meshes fuer Objekte registrieren
	GraphicManager::registerGraphicMapping("gold","gold.mesh");

}


GraphicObject* Scene::getGraphicObject(int id)
{
	std::map<int,GraphicObject*>::iterator it =  m_graphic_objects.find(id);
	if (it == m_graphic_objects.end())
		return 0;
	return it->second;
}

std::pair<float,float> Scene::getProjection(Vector pos, float height)
{
	Ogre::Vector4 ipos(pos.m_x*50,height*50,pos.m_y*50,1);
	Ogre::Vector4 projpos;
	projpos = m_camera->getProjectionMatrix()*m_camera->getViewMatrix()*ipos;

	return std::make_pair(0.5 + 0.5*projpos.x / projpos.w, 0.5 - 0.5*projpos.y / projpos.w);
}


GraphicObject* Scene::createGraphicObject(GameObject* gobj, std::string name)
{
	// Typ des GraphicObjekts ermitteln
	if (name=="")
	{
		name = gobj->getNameId();
	}
	std::string otype;
	Player* pl = dynamic_cast<Player*>(gobj);
	GraphicObject::Type type;
	if (pl != 0 )
	{
		type = pl->getPlayerLook().m_render_info;
	}
	else if (gobj->getType() == "SCRIPTOBJECT")
	{
		type = static_cast<ScriptObject*>(gobj)->getRenderInfo();
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
		DEBUGX("create graphic Object typ %s for %s",type.c_str(), otype.c_str());
		return GraphicManager::createGraphicObject(type,name, gobj->getId());
	}
}


void Scene::update(float ms)
{
	DEBUGX("update scene");

	// Spielerobjekt
	Player* player = m_document->getLocalPlayer();

	if (player ==0)
		return;

	updateCharacterView();

	if (player->getRegion() ==0)
		return;

	// Nummer der region in der sich der Spieler befindet
	short region_nr = player->getRegionId();
	Region* region = player->getRegion();

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
	DEBUGX("cam position %f %f %f",pos.m_x*50 + r*cos(theta)*cos(phi), r*sin(theta), pos.m_y*50 - r*cos(theta)*sin(phi));

	SoundSystem::setListenerPosition(pos);
	
	// Licht aktualisieren
	float *colour;
	Ogre::Light* light;
	light= m_scene_manager->getLight("HeroLight");
	light->setPosition(Ogre::Vector3(pos.m_x*50,300,pos.m_y*50));
	colour= region->getLight().getHeroLight();
	light->setDiffuseColour(colour[0], colour[1], colour[2]);
	
	colour= region->getLight().getDirectionalLight();
	light = m_scene_manager->getLight("RegionLight");
	light->setDiffuseColour(colour[0], colour[1], colour[2]);
	light->setSpecularColour(colour[0], colour[1], colour[2]);
	
	colour= region->getLight().getAmbientLight();
	m_scene_manager->setAmbientLight(Ogre::ColourValue(colour[0], colour[1], colour[2]));
	
	updateGraphicObjects(ms);
}

void  Scene::insertObject(GameObject* gobj, bool is_static)
{
	GraphicObject* obj = createGraphicObject(gobj);

	DEBUGX("insert graphic object for %s",gobj->getNameId().c_str());
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
	//node->setDirection(sin(angle),0,-cos(angle),Ogre::Node::TS_WORLD);
	node->setOrientation(1,0,0,0);
	node->yaw(Ogre::Radian(-angle));
	
	DropItem* di = dynamic_cast<DropItem*>(gobj);
	if (di != 0)
	{
		float anglex = di->getAngleX();
		node->pitch(Ogre::Radian(anglex));
	}
	
	// Aktion setzen
	std::string action = gobj->getActionString();
	float perc = gobj->getActionPercent();

	Player* pl = dynamic_cast<Player*>(gobj);
	if (pl != 0)
	{
		updatePlayerGraphicObject(obj,pl);
	}
	
	obj->updateAction(action,perc);
	DEBUGX("object %s action %s perc %f",gobj->getNameId().c_str(), action.c_str(), perc);
	
	// Zustaende aktualisieren
	std::set<std::string> flags;
	gobj->getFlags(flags);
	if (pl != 0)
	{
		std::set<std::string> flags_suffix;
		
		std::string suffix = pl->getActionWeaponSuffix();
		std::set<std::string>::iterator it;
		std::string tmp;
		for (it = flags.begin(); it !=flags.end(); ++it)
		{
			tmp = (*it);
			tmp += suffix;
			flags_suffix.insert(tmp);
		}
		obj->updateAllStates(flags_suffix);
	}
	else
	{
		obj->updateAllStates(flags);
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
	
	std::string& suffix = pl->getPlayerLook().m_item_suffix;
	
	Item* itm;
	itm = pl->getWeapon();
	if (itm !=0 && itm->m_weapon_attr !=0)
	{
		if (itm->m_weapon_attr->m_weapon_type == "bow" || itm->m_weapon_attr->m_weapon_type == "crossbow")
		{
			minfo.m_bone = "itemLeftHand";
		}
		else
		{
			minfo.m_bone = "itemRightHand";
		}
		minfo.m_source = GraphicManager::getGraphicType(itm->m_subtype);
		if (minfo.m_source.find(".mesh") == std::string::npos)
			minfo.m_source += suffix;
	}
	else
	{
		minfo.m_source="";
	}
	update |= obj->updateSubobject(minfo);
	
	// Schild
	minfo.m_objectname="shield";
	minfo.m_type = MovableObjectInfo::SUBOBJECT;
	itm = pl->getShield();
	if (itm !=0)
	{
		minfo.m_bone = "itemLeftHand";
		minfo.m_source = GraphicManager::getGraphicType(itm->m_subtype);
		if (minfo.m_source.find(".mesh") == std::string::npos)
			minfo.m_source += suffix;
	}
	else
	{
		minfo.m_source="";
	}
	update |= obj->updateSubobject(minfo);
	
	DEBUGX("update %s : bone %s source %s", minfo.m_objectname.c_str(), minfo.m_bone.c_str(), minfo.m_source.c_str());
	
	// Handschuhe
	minfo.m_objectname="gloves";
	minfo.m_type = MovableObjectInfo::SUBOBJECT;
	itm = pl->getEquipement()->getItem(Equipement::GLOVES);
	
	if (itm !=0)
	{
		std::string type = itm->m_subtype;
		
		minfo.m_bone = "";
		minfo.m_source = GraphicManager::getGraphicType(itm->m_subtype);
		if (minfo.m_source.find(".mesh") == std::string::npos)
			minfo.m_source += suffix;
	}
	else
	{
		minfo.m_source="";
	}
	update |= obj->updateSubobject(minfo);
	
	// Helm
	minfo.m_objectname="helmet";
	minfo.m_type = MovableObjectInfo::SUBOBJECT;
	itm = pl->getEquipement()->getItem(Equipement::HELMET);
	minfo.m_bone = "crown";
	
	if (itm !=0)
	{
		std::string type = itm->m_subtype;
		
		
		minfo.m_source = GraphicManager::getGraphicType(itm->m_subtype);
		if (minfo.m_source.find(".mesh") == std::string::npos)
			minfo.m_source += suffix;
	}
	else
	{
		minfo.m_source= pl->getPlayerLook().m_hair;
	}
	update |= obj->updateSubobject(minfo);
	
	// Ruestung
	minfo.m_objectname="armor";
	minfo.m_type = MovableObjectInfo::SUBOBJECT;
	itm = pl->getEquipement()->getItem(Equipement::ARMOR);
	
	if (itm !=0)
	{
		std::string type = itm->m_subtype;
		
		minfo.m_bone = "";
		minfo.m_source = GraphicManager::getGraphicType(itm->m_subtype);
		if (minfo.m_source.find(".mesh") == std::string::npos)
			minfo.m_source += suffix;

	}
	else
	{
		minfo.m_source="";
	}
	update |= obj->updateSubobject(minfo);
	
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
			DEBUGX("deleting graphic object %i",jt->first);
			jtold = jt;
			++jt;
			
			deleteGraphicObject(jtold->first);
		}
		else if (jt == m_graphic_objects.end() || (it != game_objs.end() && it->first < jt->first))
		{
			// Objekt it existiert im Spiel, aber noch nicht graphisch
			DEBUGX("inserting graphic object %i",it->first);	
			if (it->second->getLayer() != GameObject::LAYER_SPECIAL)
			{
				insertObject(it->second,false);
			}
			
			++it;
		}
		else if (it->first == jt->first)
		{
			// GraphikObjekt aktualisieren
			DEBUGX("update graphic objekt %i",it->first);
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


void Scene::updateCharacterView()
{
	bool update = false;
	
	Ogre::SceneManager* scene_mgr = Ogre::Root::getSingleton().getSceneManager("CharacterSceneManager");
	GraphicManager::setSceneManager(scene_mgr);
	
	Player* pl = m_document->getLocalPlayer();

	if ((pl ==0 && m_temp_player!=""))
	{
		DEBUG("deleting inv player");
		m_temp_player="";
		GraphicManager::destroyGraphicObject(m_temp_player_object);
		m_temp_player_object =0;
		update = true;
	}

	if (pl !=0)
	{
		std::string correctname = pl->getNameId();
		correctname += pl->getPlayerLook().m_render_info;
		
		if ((correctname != m_temp_player))
		{
			DEBUGX("updating inv player %s to %s",m_temp_player.c_str(), correctname.c_str());
			GraphicManager::destroyGraphicObject(m_temp_player_object);
			m_temp_player_object =0;
			update = true;
		}
		
		m_temp_player = correctname;
		
		if (m_temp_player_object ==0)
		{
			update = true;
			m_temp_player_object = createGraphicObject(pl,"tempplayer");
			m_temp_player_object->setExactAnimations(true);
		}
		
		update |= updatePlayerGraphicObject(m_temp_player_object,pl);
	}	
	
	if (update)
	{
		if (m_temp_player_object !=0)
		{
			// aktion temporaer fuer den Renderaufruf auf noaction setzen
			Action actsave = *(pl->getAction());
			Action tmpact("noaction");
			pl->setAction(tmpact);
			std::string act = pl->getActionString();
			
			m_temp_player_object->updateAction(act,0.5);
			m_temp_player_object->update(0);
			
			pl->setAction(actsave);
		}
		
		Ogre::Resource* res= Ogre::TextureManager::getSingleton().createOrRetrieve ("character_tex",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first.getPointer();
		Ogre::Texture* texture = dynamic_cast<Ogre::Texture*>(res);
		Ogre::RenderTarget* target = texture->getBuffer()->getRenderTarget();
	
	
		target->update();
		DEBUGX("player image is now %s",m_temp_player.c_str());
	}
	
	GraphicManager::setSceneManager(m_scene_manager);
	
}


void Scene::clearObjects()
{
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
	DEBUGX("create Scene");

	// alle bisherigen Objekte aus der Szene loeschen
	clearObjects();
	
	GraphicManager::destroyGraphicObject(m_temp_player_object);
	m_temp_player_object =0;
	m_temp_player="";
	SoundSystem::clearObjects();
	
	GraphicManager::clearParticlePool();
	
	m_scene_manager->clearScene();
	
	updateCharacterView();

	// Liste der statischen Objekte
	Ogre::StaticGeometry* static_geom = m_scene_manager->createStaticGeometry ("StaticGeometry");

	std::list<WorldObject*> stat_objs;

	Region* region = m_document->getLocalPlayer()->getRegion();

	float *colour;
	m_scene_manager->setAmbientLight(Ogre::ColourValue(0.4,0.4,0.4));
	
	colour= region->getLight().getHeroLight();
	Ogre::Light *light = m_scene_manager->createLight("HeroLight");
	light->setType(Ogre::Light::LT_POINT);
	light->setDiffuseColour(colour[0], colour[1], colour[2]);
	light->setSpecularColour(0.0, 0.0, 0.0);
	light->setAttenuation(1000,0.5,0.000,0.00001);
	light->setCastShadows(false);
	DEBUGX("hero light %f %f %f",colour[0], colour[1], colour[2]);

	colour= region->getLight().getDirectionalLight();
	light = m_scene_manager->createLight("RegionLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDiffuseColour(colour[0], colour[1], colour[2]);
	light->setSpecularColour(colour[0], colour[1], colour[2]);
	light->setDirection(Ogre::Vector3(-1,-1,-1));
	DEBUGX("directional light %f %f %f",colour[0], colour[1], colour[2]);

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
		DEBUGX("camera pos %i %i %i",dimx*100,std::max(dimx,dimy)*300,dimy*100);
		minimap_camera->setPosition(Ogre::Vector3(dimx*100,std::max(dimx,dimy)*200,10+dimy*100));
		minimap_camera->lookAt(Ogre::Vector3(dimx*100,0,dimy*100));
#if (OGRE_VERSION >= ((1 << 16) | (6 << 8)))
			// >= Ogre 1.6
			minimap_camera->setOrthoWindow(std::max(dimx,dimy)*200, std::max(dimx,dimy)*200);
#else
			minimap_camera->setNearClipDistance(std::max(dimx,dimy)*100);
#endif
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

		colour= region->getLight().getAmbientLight();
		m_scene_manager->setAmbientLight(Ogre::ColourValue(colour[0], colour[1], colour[2]));
		DEBUGX("ambient light %f %f %f",colour[0], colour[1], colour[2]);
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
					ground->setCastShadows(true);
					ground->setQueryFlags(0);
					node->attachObject(ground);

					static_geom->addSceneNode(node);
				}
			}
		}

		// Schatten
		//m_scene_manager->setAmbientLight(Ogre::ColourValue(0.0,0.0,0.0));
		/*
		light = m_scene_manager->getLight("RegionLight");
		light->setCastShadows(true);
		m_scene_manager->setShadowTextureConfig(0,2048,2048,Ogre::PF_X8R8G8B8);
		m_scene_manager->setShadowColour( Ogre::ColourValue(0.5, 0.5, 0.5) );
		m_scene_manager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
		*/
		//m_scene_manager->setAmbientLight(Ogre::ColourValue(0.0,0.0,0.0));
		//m_scene_manager->setShadowFarDistance (10);
	}

}

void Scene::getMeshInformation(const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3* &vertices,  size_t &index_count,
							unsigned long* &indices,  const Ogre::Vector3 &position,  const Ogre::Quaternion &orient,  const Ogre::Vector3 &scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

    // Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );

        // We only need to add the shared vertices once
		if(submesh->useSharedVertices)
		{
			if( !added_shared )
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}

        // Add the indices
		index_count += submesh->indexData->indexCount;
	}


    // Allocate space for the vertices and indices
	vertices = new Ogre::Vector3[vertex_count];
	indices = new unsigned long[index_count];

	added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
		{
			if(submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem =
					vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf =
					vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex =
					static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
			float* pReal;

			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

		if ( use32bitindexes )
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
						static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
} 
