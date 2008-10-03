#ifndef SCENE_H
#define SCENE_H

#include <map>
#include "Ogre.h"
#include <string>

#include "itemloader.h"
#include "objectloader.h"
#include "document.h"
#include "serverwobject.h"
// TODO : include anpassen (es wird nur ein Spieler allgemein benoetigt)


using namespace std;

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
	* \var std::list<pair<std::string,std::string> > m_extra_meshes
	* \brief Liste der weiteren Meshes die an das Hauptmesh angefuegt werden
	* erster Teil des Wertepaares: Name des Knochens
	* zweiter Teil des Wertepaares: Name des Meshes
	*/
	std::list<pair<std::string,std::string> > m_extra_meshes;

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
	 * \fn Scene(Document* doc)
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
	 * \fn map<int,string>* getObjects()
	 * \brief Gibt die Liste der in der Szene dargestellten nicht statischen Objekte aus
	 */
	map<int,string>* getObjects()
	{
		return m_objects;
	}

	/**
	 * \fn map<int,string>* getDropItems()
	 * \brief Gibt die Liste der in der Szene dargestellen Gegenstaende aus
	 */
	map<int,string>* getDropItems()
	{
		return m_drop_items;
	}

	private:

	/**
	 * \fn void updateObjects()
	 * \brief Aktualisiert die Darstellung aller Objekte
	 */
	void updateObjects();

	/**
	 * \fn void updateObject(ServerWObject* obj);
	 * \brief Fuegt das Objekt in die Szene ein
	 */
	void updateObject(ServerWObject* obj);


	/**
	 * \fn void  createObject(WorldObject* obj,std::string& name, bool is_static)
	 * \brief Erzeugt auf Basis eines Objektes eine Ogre Objekt
	 * \param obj Objekt fuer das eine Ogre Entity erzeugt werden soll
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
	 * \param itm Gegenstand fuer den die Entity erzeugt werden soll
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
	 * \fn void update Projectiles()
	 * \brief Aktualisiert die Darstellung Projektile
	 */
	void updateProjectiles();

	/**
	 * \fn void createProjectile(Projectile* pr, std::string& name)
	 * \brief Erzeugt auf Basis eines Projektiles eine Ogre Entity mit dem angegebenen Name
	 * \param itm Projektil fuer das die Entity erzeugt werden soll
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
	 * \fn void insertTiles()
	 * \brief Fuegt die Tiles der Region in die Szene ein
	 */
	void insertTiles();

	/**
	 * \fn void registerMeshes()
	 * \brief Registriert fuer alle Objekte, Projektile, Items und Tiles die zugehoerigen Meshes
	 */
	void registerMeshes();


	/**
	 * \fn void registerObject(WorldObject::TypeInfo::ObjectType, std::string mesh, std::string particle_system, float scaling_factor =1.0)
	 * \brief Registriert fuer ein Objekt das zugehoerige Mesh, Partikelsystem, Skalierungsfaktor
	 * \param subtype Subtyp des Objektes
	 * \param mesh Mesh fuer das Objekt. Leerer String, wenn kein Mesh verwendet wird
	 * \param particle_system Name des Partikelsystems. Leerer string, wenn kein Partikelsystem verwendet wird
	 */
	void registerObject(WorldObject::TypeInfo::ObjectSubtype subtype, std::string mesh, std::string particle_system= "", float scaling_factor =1.0);

    /**
     * \fn void registerAttachedMesh(WorldObject::TypeInfo::ObjectSubtype subtype, std::string bone, std::string mesh)
     * \brief Registriert fuer ein bestehendes Objekt ein Mesh das an das Objekt angefuegt wird
     * \param subtype Subtype des Objektes
     * \param bone Knochen an den Mesh gehaengt wird
     * \param mesh Mesh des angehaengten Objektes
     */
    void registerAttachedMesh(WorldObject::TypeInfo::ObjectSubtype subtype, std::string bone, std::string mesh);

	/**
	 * \fn void registerItem(Item::Subtype subtype, std::string mesh, std::string particle_system, float scaling_factor =1.0)
	 * \brief Registriert fuer ein Item das zugehoerige Mesh, Partikelsystem, Skalierungsfaktor
	 * \param subtype Subtyp des Items
	 * \param mesh Mesh fuer das Item. Leerer String, wenn kein Mesh verwendet wird
	 * \param particle_system Name des Partikelsystems. Leerer string, wenn kein Partikelsystem verwendet wird
	 */
	void registerItem(Item::Subtype subtype, std::string mesh, std::string particle_system= "", float scaling_factor =1.0);

	/**
	 * \fn void registerProjectile(, std::string mesh, std::string particle_system, float scaling_factor =1.0)
	 * \brief Registriert fuer ein Projektil das zugehoerige Mesh, Partikelsystem, Skalierungsfaktor
	 * \param type Subtyp des Projektil
	 * \param mesh Mesh fuer das Projektil. Leerer String, wenn kein Mesh verwendet wird
	 * \param particle_system Name des Partikelsystems. Leerer string, wenn kein Partikelsystem verwendet wird
	 */
	void registerProjectile(Projectile::ProjectileType type, std::string mesh, std::string particle_system = "", float scaling_factor =1.0);

	/**
	 * \fn void registerTile(Tile tile, std::string mesh, std::string particle_system, float scaling_factor =1.0)
	 * \brief Registriert fuer ein Tile das zugehoerige Mesh, Partikelsystem, Skalierungsfaktor
	 * \param tile Typ des Tile
	 * \param mesh Mesh fuer das Tile. Leerer String, wenn kein Mesh verwendet wird
	 * \param particle_system Name des Partikelsystems. Leerer string, wenn kein Partikelsystem verwendet wird
	 */
	void registerTile(Tile tile, std::string mesh, std::string particle_system ="", float scaling_factor =1.0);

	/**
	 * \fn RenderInfo getObjectRenderInfo(WorldObject::TypeInfo::ObjectSubtype subtype)
	 * \brief Gibt die Informationen zum rendern eines Objektes aus
	 * \param subtype Subtyp des Objektes
	 */
	RenderInfo getObjectRenderInfo(WorldObject::TypeInfo::ObjectSubtype subtype);

	/**
	 * \fn RenderInfo getItemRenderInfo(Item::Subtype subtype)
	 * \brief Gibt die Informationen zum rendern eines Gegenstandes aus
	 * \param subtype Subtyp des Gegenstandes
	 */
	RenderInfo getItemRenderInfo(Item::Subtype subtype);

		/**
	 * \fn RenderInfo getProjectileRenderInfo( (Projectile::ProjectileType type)
	 * \brief Gibt die Informationen zum rendern eines Objektes aus
	 * \param subtype Subtyp des Objektes
		 */
	RenderInfo getProjectileRenderInfo(Projectile::ProjectileType subtype);

	/**
	 * \fn RenderInfo getTileRenderInfo(Tile tile)
	 * \brief Gibt die Informationen zum rendern eines Tiles aus
	 * \param Tile Type des Tiles
	 */
	RenderInfo getTileRenderInfo(Tile tile);


	/**
	 * \var map<int,string>* m_objects
	 * \brief IDs der objekte, die aktuell dargestellt werden, sowie deren Namen in Ogre
	 */
	map<int,string>* m_objects;

	/**
	 * \var map<int,string>* m_drop_items
	 * \brief IDs der Gegenstaende, die aktuell dargestellt werden, sowie deren Namen in Ogre
	 */
	map<int,string>* m_drop_items;

	/**
	 * \var map<int,string>* m_projectiles
	 * \brief IDs der Projektile, die aktuell dargestellt werden, sowie deren Namen in Ogre
	 */
	map<int,string>* m_projectiles;

	/**
	 * \var map<Projectile::ProjectileType, RenderInfo> m_projectile_render_info
	 *  \brief Speichert fuer die Projektile die Information zum Rendern
	 */
	map<Projectile::ProjectileType, RenderInfo> m_projectile_render_info;

	/**
	 * \var map<WorldObject::TypeInfo::ObjectSubtype, RenderInfo> m_object_render_info
	 *  \brief Speichert fuer die Objekte die Information zum Rendern
	 */
	map<WorldObject::TypeInfo::ObjectSubtype, RenderInfo> m_object_render_info;

	/**
	 * \var map<Item::Subtype, RenderInfo> m_item_render_info
	 *  \brief Speichert fuer die Projektile die Information zum Rendern
	 */
	map<Item::Subtype, RenderInfo> m_item_render_info;

	/**
	 * \var map<Tile, RenderInfo> m_tile_render_info
	 *  \brief Speichert fuer die Tiles die Information zum Rendern
	 */
	map<Tile, RenderInfo> m_tile_render_info;



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


};

#endif

