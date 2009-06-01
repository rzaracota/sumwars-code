#ifndef SCENE_H
#define SCENE_H

#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreTextureManager.h"
#include "OgreParticleSystem.h"
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreRenderTarget.h"
#include "OgreViewport.h"
#include "OgreStaticGeometry.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreRenderTexture.h"

#include <map>
#include <string>

#include "itemloader.h"
#include "objectloader.h"
#include "document.h"
#include "worldobject.h"
#include "sound.h"
#include "player.h"
#include "graphicobject.h"


/**
 * \struct RenderInfo
 * \brief Enthaelt die Informationen die zum Rendern eines Objektes notwendig sind
 */
struct RenderInfo
{
	/**
	 * \var std::string m_mesh
	 * \brief Name des Meshes des Objektes. Leerer String, wenn das Objekt kein Mesh hat
	 */
	std::string m_mesh;

	/**
	* \var std::std::list<pair<std::string,std::string> > m_extra_meshes
	* \brief Liste der weiteren Meshes die an das Hauptmesh angefuegt werden
	* erster Teil des Wertepaares: Name des Knochens
	* zweiter Teil des Wertepaares: Name des Meshes
	*/
	std::list<std::pair<std::string,std::string> > m_extra_meshes;

	/**
	 * \var std::string m_particle_system
	 * \brief Partikelsystem des Objektes. Leerer String, wenn das Objekt kein Partikelsystem hat
	 */
	std::string m_particle_system;

	/**
	 * \var float m_scaling_factor
	 * \brief Faktor um den das Mesh und das Partikelsystem skaliert werden
	 */
	float m_scaling_factor;

	/**
	 * \fn RenderInfo() {}
	 * \brief Konstruktor
	 */
	RenderInfo() {}
	
	/**
	 * \fn RenderInfo(std::string mesh, std::string particle_system, float scaling_factor =1.0)
	 * \brief Konstruktor
	 */
	RenderInfo(std::string mesh, std::string particle_system, float scaling_factor =1.0)
	{
		m_mesh = mesh;
		m_particle_system = particle_system;
		m_scaling_factor = scaling_factor;
	}

};

/**
 * \class Scene
 * \brief Beschreibt die darzustellende Szene
 */
class Scene
{
	public:
		
	/**
	 * \enum ViewportSize
	 * \brief verschiedene Anordnungen der Flaeche in die gerendert wird
	 */
	enum ViewportSize
	{
		VIEW_FULL =1,
		VIEW_LEFT = 2,
  		VIEW_RIGHT =3,
	};

	/**
	 * \fn Scene(Document* doc,Ogre::RenderWindow* window)
	 * \brief Konstruktor
	 * \param doc Dokument dessen Daten dargestellt werden sollen
	 * \param window Fenster in das gerendert wird
	 */
	Scene(Document* doc,Ogre::RenderWindow* window);

	/**
	 * \fn ~Scene()
	 * \brief Destruktor
	 */
	~Scene();

	/**
	 * \fn void update(float ms)
	 * \brief aktualisiert die Szene
	 * \param ms Vergangene Zeit in ms seit dem letzten update
	 */
	void update(float ms=0);

	/**
	 * \fn Ogre::Camera* getCamera()
	 * \brief Gibt die Kamera aus, mit der die Szene aufgenommen wird
	 */
	Ogre::Camera* getCamera()
	{
		return m_camera;
	}

	/**
	 * \fn Ogre::Viewport* getViewport()
	 * \brief Gibt Viewport aus
	 */
	Ogre::Viewport* getViewport()
	{
		return m_viewport;
	}
	
	/**
	 * \fn void changeViewportSize(ViewportSize size)
	 * \brief Aendert die Flaeche in die gerendert wird
	 */
	void changeViewportSize(ViewportSize size);


	/**
	 * \fn std::map<int,string>* getObjects()
	 * \brief Gibt die Liste der in der Szene dargestellten nicht statischen Objekte aus
	 */
	std::map<int,string>* getObjects()
	{
		return m_objects;
	}

	/**
	 * \fn std::map<int,string>* getDropItems()
	 * \brief Gibt die Liste der in der Szene dargestellen Gegenstaende aus
	 */
	std::map<int,string>* getDropItems()
	{
		return m_drop_items;
	}
	
	
	/**
	 * \fn std::pair<float,float> getProjection(Vector pos, float height=0)
	 * \brief Gibt aus, an welcher Stelle ein Punkt in Spielkoordinaten auf dem Bildschirm erscheint
	 * \param pos Ort in Spielkoordinaten
	 * \param height Hoehe in Spielkoordinaten
	 */
	std::pair<float,float> getProjection(Vector pos, float height=0);
	
	/**
	 * \fn static void getPlayerLook(GameObject::Subtype subtype, std::list< std::pair<bool, PlayerLook> > &looks)
	 * \brief Gibt fuer eine gegebene Spielerklasse alle Moeglichkeiten des Aussehens aus
	 * \param subtype Spielerklasse
	 * \param looks Ausgabe: Vektor mit allen Varianten
	 */
	static void getPlayerLook(GameObject::Subtype subtype, std::list< std::pair<bool, PlayerLook> > &looks);
	
	
	/**
	 * \fn void updateCharacterView()
	 * \brief aktualisiert die Ansicht des Spielers
	 */
	void updateCharacterView();

	/**
	 * \fn static void registerObjectAnimations(GameObject::Subtype subtype, std::map<Action::ActionType, std::vector<std::string> > &animations)
	 * \brief Registriert die Animationen fuer ein Objekt
	 * \param subtype Subtyp des Objektes
	 * \param animations Liste der Animationen sortiert nach Aktionstyp
	 */
	static void registerObjectAnimations(GameObject::Subtype subtype, std::map<Action::ActionType, std::vector<std::string> > &animations);
	
    /**
	 * \fn void registerAttachedMesh(GameObject::Subtype subtype, std::string bone, std::string mesh)
	 * \brief Registriert fuer ein bestehendes Objekt ein Mesh das an das Objekt angefuegt wird
	 * \param subtype Subtype des Objektes
	 * \param bone Knochen an den Mesh gehaengt wird
	 * \param mesh Mesh des angehaengten Objektes
	 */
	static void registerAttachedMesh(GameObject::Subtype subtype, std::string bone, std::string mesh);
	
	/**
	 * \fn void registerObject(GameObject::Subtype subtype, std::string mesh, std::string particle_system= "", float scaling_factor =1.0)
	 * \brief Registriert fuer ein Objekt das zugehoerige Mesh, Partikelsystem, Skalierungsfaktor
	 * \param subtype Subtyp des Objektes
	 * \param mesh Mesh fuer das Objekt. Leerer String, wenn kein Mesh verwendet wird
	 * \param particle_system Name des Partikelsystems. Leerer string, wenn kein Partikelsystem verwendet wird
	 * \param scaling_factor Faktor um den das Mesh und alle Partikelsystem skaliert werden
	 */
	static void registerObject(GameObject::Subtype subtype, std::string mesh, std::string particle_system= "", float scaling_factor =1.0);
	
	/**
	 * \fn static void registerPlayerLook(GameObject::Subtype subtype, PlayerLook look, std::string mesh, bool male = true)
	 * \brief Registiert ein fuer einen Spielertyp zulaessiges Aussehen
	 * \param subtype Spielertyp
	 * \param look Aussehen
	 * \param male Gibt Geschlecht an
	 * \param mesh Ogre Mesh
	 */
	static void registerPlayerLook(GameObject::Subtype subtype, PlayerLook look, std::string mesh, bool male = true);

	

	/**
	 * \fn void registerItem(Item::Subtype subtype, std::string mesh, std::string particle_system, float scaling_factor =1.0)
	 * \brief Registriert fuer ein Item das zugehoerige Mesh, Partikelsystem, Skalierungsfaktor
	 * \param subtype Subtyp des Items
	 * \param mesh Mesh fuer das Item. Leerer String, wenn kein Mesh verwendet wird
	 * \param particle_system Name des Partikelsystems. Leerer string, wenn kein Partikelsystem verwendet wird
	 */
	static void registerItem(Item::Subtype subtype, std::string mesh, std::string particle_system= "", float scaling_factor =1.0);

	/**
	 * \fn void registerProjectile(Projectile::Subtype type, std::string mesh, std::string particle_system = "", float scaling_factor =1.0)
	 * \brief Registriert fuer ein Projektil das zugehoerige Mesh, Partikelsystem, Skalierungsfaktor
	 * \param type Subtyp des Projektil
	 * \param mesh Mesh fuer das Projektil. Leerer String, wenn kein Mesh verwendet wird
	 * \param particle_system Name des Partikelsystems. Leerer string, wenn kein Partikelsystem verwendet wird
	 * \param scaling_factor Faktor um den das Mesh und alle Partikelsystem skaliert werden
	 */
	static void registerProjectile(Projectile::Subtype type, std::string mesh, std::string particle_system = "", float scaling_factor =1.0);

	
	private:

	/**
	 * \fn GraphicObject* createGraphicObject(GameObject* gobj)
	 * \brief erzeugt zu einem Spielobjekt das passende Graphicobjekt
	 * \param gobj Spielobjekt
	 */
	GraphicObject* createGraphicObject(GameObject* gobj);
	
	/**
	 * \fn void updateGraphicObject(GraphicObject* obj, GameObject* gobj)
	 * \brief aktualisiert das Graphicobjekt
	 * \param obj GraphicObjekt
	 * \param gobj zugehoeriges Spielobjekt
	 * \param time vergangene Zeit in ms
	 */
	void updateGraphicObject(GraphicObject* obj, GameObject* gobj,float time);
	
	/**
	 * \fn bool updatePlayerGraphicObject(GraphicObject* obj, Player* pl)
	 * \brief aktualisiert das GraphikObjekt eines Spielers
	 * \param obj GraphikObjekt
	 * \param pl Spielerobjekt
	 */
	bool updatePlayerGraphicObject(GraphicObject* obj, Player* pl);
	
	
	/**
	 * \fn void updateGraphicObjects(float time)
	 * \brief aktualisiert alle GraphicObjekte
	 * \param time vergangene Zeit in ms
	 */
	void updateGraphicObjects(float time);
	
	/**
	 * \fn void  insertObject(GameObject* obj,std::string& name, bool is_static)
	 * \brief Erzeugt auf Basis eines Objektes eine Ogre Objekt
	 * \param obj Objekt fuer das eine Ogre Entity erzeugt werden soll
	 * \param is_static gibt an, ob das Objekt statisch ist
	 */
	void  insertObject(GameObject* obj, bool is_static = false);

	/**
	 * \fn void deleteGraphicObject(int id)
	 * \brief loescht ein Graphicobjekt
	 * \param id ID des Objekts
	 */
	void deleteGraphicObject(int id);
	
	/**
	 * \fn void updateObjects()
	 * \brief Aktualisiert die Darstellung aller Objekte
	 */
	void updateObjects();

	/**
	 * \fn void updateObject(WorldObject* obj);
	 * \brief Fuegt das Objekt in die Szene ein
	 */
	void updateObject(WorldObject* obj);

	/**
	 * \fn bool updatePlayer(Player* pl, Ogre::Entity* obj_ent, Ogre::SceneManager* scene_manager)
	 * \brief Aktualisiert die an den Spieler angehaengten Meshes
	 * \param pl Spieler Objekt
	 * \param obj_ent Ogre Entity
	 * \param scene_manager Scene in der der Spieler sich befindet
	 * \return Gibt an, ob eine Veraenderung eingetreten ist
	 */
	bool updatePlayer(Player* pl, Ogre::Entity* obj_ent, Ogre::SceneManager* scene_manager);

	/**
	 * \fn void  createObject(WorldObject* obj,std::string& name, bool is_static)
	 * \brief Erzeugt auf Basis eines Objektes eine Ogre Objekt
	 * \param obj Objekt fuer das eine Ogre Entity erzeugt werden soll
	 * \param name Name des Objektes
	 * \param is_static gibt an, ob das Objekt statisch ist
	 */
	void  createObject(WorldObject* obj,std::string& name, bool is_static = false);

	/**
	 * \fn void deleteObject(std::string name)
	 * \brief Entfernt ein Objekt aus der Szene
	 * \param name Name des Objektes in Ogre
	 */
	void deleteObject(std::string name);

	/**
	 * \fn void updateItems()
	 * \brief Aktualisiert die Darstellung aller Items
	 */
	void updateItems();

	/**
	 * \fn void createItem(DropItem* di, std::string& name)
	 * \brief Erzeugt auf Basis eines Items eine Ogre Entity mit dem angegebenen Name
	 * \param di Gegenstand fuer den die Entity erzeugt werden soll
	 * \param name der Entitiy
	 */
	 void createItem(DropItem* di, std::string& name);

	/**
	 * \fn void deleteItem(std::string name)
	 * \brief Entfernt ein Item aus der Szene
	 * \param name Name des Objektes in Ogre
	 */
	void deleteItem(std::string name);

	/**
	 * \fn void updateProjectiles()
	 * \brief Aktualisiert die Darstellung Projektile
	 */
	void updateProjectiles();

	/**
	 * \fn void createProjectile(Projectile* pr, std::string& name)
	 * \brief Erzeugt auf Basis eines Projektiles eine Ogre Entity mit dem angegebenen Name
	 * \param pr Projektil fuer das die Entity erzeugt werden soll
	 * \param name der Entitiy
	 */
	void createProjectile(Projectile* pr, std::string& name);

	/**
	 * \fn void deleteProjectile(std::string name)
	 * \brief Entfernt ein Projektil aus der Szene
	 * \param name Name des Objektes in Ogre
	 */
	void deleteProjectile(std::string name);

	/**
	 * \fn Ogre::ParticleSystem* getParticleSystem(std::string type)
	 * \brief Gibt aus dem Pool ein passendes Partikelsystem aus
	 * \param type Typ des Partikelsystems
	 */
	Ogre::ParticleSystem* getParticleSystem(std::string type);
	
	/**
	 * \fn void putBackParticleSystem(Ogre::ParticleSystem* part)
	 * \brief Fuegt das Partikelsystem wieder in den Pool ein
	 */
	void putBackParticleSystem(Ogre::ParticleSystem* part);
	
	/**
	 * \fn void destroySceneNode(std::string& node_name)
	 * \brief Loescht einen Knoten mit allen angehaengten Objekten
	 * \param node_name Name Knoten der geloescht werden soll
	 */
	void destroySceneNode(std::string& node_name);

	
	
	/**
	 * \var clearObjects()
	 * \brief entfernt alle Objekte aus der Szene
	 */
	void clearObjects();

	/**
	 * \var void createScene()
	 * \brief Erstellt die Szene neu
	 */
	void createScene();


	/**
	 * \fn void registerMeshes()
	 * \brief Registriert fuer alle Objekte, Projektile, Items und Tiles die zugehoerigen Meshes
	 */
	static void registerMeshes();


	
	/**
	 * \fn RenderInfo getObjectRenderInfo(GameObject::Subtype subtype)
	 * \brief Gibt die Informationen zum rendern eines Objektes aus
	 * \param subtype Subtyp des Objektes
	 */
	static RenderInfo getObjectRenderInfo(GameObject::Subtype subtype);
	
	/**
	 * \fn static RenderInfo getPlayerRenderInfo(PlayerLook look)
	 * \brief Gibt zu dem Aussehen des Spielers die Informationen zum Rendern aus
	 * \param look Aussehen des Spielers
	 */
	static RenderInfo getPlayerRenderInfo(PlayerLook look);

	/**
	 * \fn RenderInfo getItemRenderInfo(Item::Subtype subtype)
	 * \brief Gibt die Informationen zum rendern eines Gegenstandes aus
	 * \param subtype Subtyp des Gegenstandes
	 */
	static RenderInfo getItemRenderInfo(Item::Subtype subtype);

	/**
	 * \fn RenderInfo getProjectileRenderInfo(Projectile::Subtype subtype)
	 * \brief Gibt die Informationen zum rendern eines Objektes aus
	 * \param subtype Subtyp des Objektes
	 */
	static RenderInfo getProjectileRenderInfo(Projectile::Subtype subtype);
	
	


	/**
	 * \var std::map<int,string>* m_objects
	 * \brief IDs der objekte, die aktuell dargestellt werden, sowie deren Namen in Ogre
	 */
	std::map<int,string>* m_objects;

	/**
	 * \var std::map<int,string>* m_drop_items
	 * \brief IDs der Gegenstaende, die aktuell dargestellt werden, sowie deren Namen in Ogre
	 */
	std::map<int,string>* m_drop_items;

	/**
	 * \var std::map<int,string>* m_projectiles
	 * \brief IDs der Projektile, die aktuell dargestellt werden, sowie deren Namen in Ogre
	 */
	std::map<int,string>* m_projectiles;

	/**
	 * \var std::map<int,GraphicObject*> m_static_objects
	 * \brief Graphikobjekte fuer alle statischen Objekte
	 */
	std::map<int,GraphicObject*> m_static_objects;
	
	/**
	 * \var std::map<int,GraphicObject*> m_graphic_objects
	 * \brief Graphikobjekte fuer alle nicht statischen Objekte
	 */
	std::map<int,GraphicObject*> m_graphic_objects;
	
	/**
	 * \var static std::map<Projectile::Subtype, RenderInfo> m_projectile_render_info
	 *  \brief Speichert fuer die Projektile die Information zum Rendern
	 */
	static std::map<Projectile::Subtype, RenderInfo> m_projectile_render_info;

	/**
	 * \var static std::map<GameObject::Subtype, RenderInfo> m_object_render_info
	 *  \brief Speichert fuer die Objekte die Information zum Rendern
	 */
	static std::map<GameObject::Subtype, RenderInfo> m_object_render_info;
	
	/**
	 * \var static std::map<PlayerLook, RenderInfo> m_player_render_info
	 * \brief Speichert fuer die Spieler die Information zum Rendern
	 */
	static std::map<PlayerLook, RenderInfo> m_player_render_info;
	
	/**
	 * \var static std::multimap< GameObject::Subtype, std::pair<bool, PlayerLook> > m_player_look
	 * \brief Speichert, fuer welche Spielerklassen welches Aussehen erlaubt ist
	 */
	static std::multimap< GameObject::Subtype, std::pair<bool, PlayerLook> > m_player_look;
	
	/**
	 * \var static std::map<GameObject::Subtype, std::map<Action::ActionType, std::vector<std::string> > > m_object_animations
	 * \brief Animationen der Objekte (erster Schluessel: Objekttyp, zweiter Schluessel Aktionstyp)
	 */
	static std::map<GameObject::Subtype, std::map<Action::ActionType, std::vector<std::string> > > m_object_animations;
	

	/**
	 * \var static std::map<Item::Subtype, RenderInfo> m_item_render_info
	 *  \brief Speichert fuer die Projektile die Information zum Rendern
	 */
	static std::map<Item::Subtype, RenderInfo> m_item_render_info;


	/**
	 * \var Document* m_document
	 * \brief Dokument, dessen Daten dargestellt werden sollen
	 */
	Document* m_document;
	
	/**
	 * \var short m_region_id
	 * \brief ID der Region, die gerade angezeigt wird
	 */
	short m_region_id;

	/**
	 * \var Ogre::RenderWindow* m_window
	 * \brief Fenster in dem die Szene gerendert wird
	 */
	Ogre::RenderWindow* m_window;

	/**
	 * \var Ogre::Camera* m_camera
	 * \brief Kamera mit der die Szene betrachtet wird
	 */
	Ogre::Camera* m_camera;
	

	/**
	 * \var Ogre::Viewport* m_viewport
	 * \brief Darstellungsflaeche in die gerendert wird
	 */
	Ogre::Viewport* m_viewport;

	/**
	 * \var Ogre::SceneManager* m_scene_manager
	 * \brief aktueller Scenemanager
	 */
	Ogre::SceneManager* m_scene_manager;

	
	/**
	 * \var std::multimap<std::string, Ogre::ParticleSystem*> m_particle_system_pool
	 * \brief interner Pool von Partikelsystem
	 */
	std::multimap<std::string, Ogre::ParticleSystem*> m_particle_system_pool;
	
	
	/**
	 * \var ViewportSize m_viewport_size
	 * \brief aktuelle Groesse und Platzierung der Flaeche in die gerendert wird
	 */
	ViewportSize m_viewport_size;
	
	/**
	 * \var std::string m_temp_player
	 * \brief Vorschau Objekt auf den aktuellen Spieler
	 */
	std::string m_temp_player;
	
};

#endif

