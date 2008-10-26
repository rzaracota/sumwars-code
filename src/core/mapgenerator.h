#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "matrix2d.h"
#include "unionfindtree.h"
#include "objectfactory.h"
#include "spawnpoint.h"
#include <iomanip>

#include "region.h"
#include "random.h"
#include <queue>

#ifndef PI
#define PI 3.14159
#endif

/**
 * \class MapGenerator
 * \brief Klasse zum Karten generieren
 */
class MapGenerator
{
	
	
	public:
	/**
	 * \struct MapData
	 * \brief alle Daten die waehrend des Aufbaus der Karte notwendig sind
	 */
	struct MapData
	{
		/**
		 * \var Matrix2d<char>* m_base_map
		 * \brief die grundlegende Karte. Freie Felder sind mit 1 markiert, blockierte mit -1
		 */
		Matrix2d<char>* m_base_map;
		
		
		/**
		 * \var Matrix2d<char>* m_template_map 
		 * \brief Karte die angibt wie gross das groesste Template ist, das an diese Position passt
		 */
		Matrix2d<char>* m_template_map;
		
		/**
		 * \var Matrix2d<int>* m_template_index_map
		 * \brief enthaelt die Indizee der Punkte in der m_template_places Datenstruktur
		 */
		Matrix2d<int>* m_template_index_map;

		/**
		 * \var Matrix2d<float>* m_environment
		 * \brief Karte fuer die Art der Umgebung
		 */
		Matrix2d<float>* m_environment;
		
		/**
		 * \var std::list<std::pair<int,int> > m_border
		 * \brief Menge der Punkte, die den Rand des betretbaren Gebietes bilden
		 */
		std::list<std::pair<int,int> > m_border;
		
		/**
		 * \var std::map< int, std::vector<int> > m_template_places
		 * \brief Orte an denen Templates platziert werden koennen
		 */
		std::map< int, std::vector<int> > m_template_places;
		

		/**
		 * \var Region* m_region
		 * \brief die Region die erstellt wird
		 */
		Region* m_region;
		 
	};
	

	/**
	 * \fn static void  createMapData(MapData* mdata, RegionData* rdata)
	 * \brief allokiert den Speicher fuer die Karte
	 * \param rdata Daten fuer die Region die erstellt werden soll
	 * \param mdata temporaere Daten zum Aufbau der Region
	 */
	static void  createMapData(MapData* mdata, RegionData* rdata);
	
	/**
	 * \fn static void createBaseMap(MapData* mdata, RegionData* rdata)
	 * \brief erstellt die grundlegende Karte
	 * \param rdata Daten fuer die Region die erstellt werden soll
	 * \param mdata temporaere Daten zum Aufbau der Region
	 */
	static void createBaseMap(MapData* mdata, RegionData* rdata);
	
	/**
	 * \fn static void createTemplateMap(MapData* mdata, RegionData* rdata)
	 * \brief erzeugt eine Karte die anzeigt wo die Templates platziert werden koennen
	\param rdata Daten fuer die Region die erstellt werden soll
	 * \param mdata temporaere Daten zum Aufbau der Region
	 */
	static void createTemplateMap(MapData* mdata, RegionData* rdata);
	
	/**
	 * \fn static bool insertGroupTemplates(MapData* mdata, RegionData* rdata)
	 * \brief Fuegt die Objektgruppen in die Region ein
	 * \param rdata Daten fuer die Region die erstellt werden soll
	 * \param mdata temporaere Daten zum Aufbau der Region
	 * \return Gibt true zurueck, wenn alle obligatorischen Objektgruppen erfolgreich eingefuegt wurden
	 */
	static bool insertGroupTemplates(MapData* mdata, RegionData* rdata);
	
	/**
	 * \fn static void createBorder(MapData* mdata, RegionData* rdata)
	 * \brief erstellt die Berandung
	 * \param rdata Daten fuer die Region die erstellt werden soll
	 * \param mdata temporaere Daten zum Aufbau der Region
	 */
	static void createBorder(MapData* mdata, RegionData* rdata);
	
	/**
	 * \fn static void insertSpawnpoints(MapData* mdata, RegionData* rdata)
	 * \brief Fuegt der Region die Spawnpoints hinzu
	 * \param rdata Daten fuer die Region die erstellt werden soll
	 * \param mdata temporaere Daten zum Aufbau der Region
	 */
	static void insertSpawnpoints(MapData* mdata, RegionData* rdata);
	
	/**
	 * \fn static void createExits(MapData* mdata, RegionData* rdata)
	 * \brief Fuegt der Region die Ausgaenge hinzu
	 * \param rdata Daten fuer die Region die erstellt werden soll
	 * \param mdata temporaere Daten zum Aufbau der Region
	 */
	static void createExits(MapData* mdata, RegionData* rdata);
	
	/**
	 * \fn static Region* createRegion(RegionData* rdata)
	 * \brief erzeugt aus den angegebenen Daten eine Region
	 * \param rdata Daten zur Erzeugung der Region
	 */
	static Region* createRegion(RegionData* rdata);
	
	/**
	 * \fn static bool getTemplatePlace(MapData* mdata, Shape* shape, Vector & place)
	 * \brief ermittelt einen Ort fuer das Template
	 * \param mdata temporaere Daten zum Aufbau der Region
	 * \param shape Grundflaeche des Templates
	 * \param place Rueckgabewert fuer den Ort
	 * \return gibt true aus, wenn ein Ort gefunden wurde, sonst false
	 */
	static bool getTemplatePlace(MapData* mdata, Shape* shape, Vector & place);
	
	/**
	 * \fn createPerlinNoise(Matrix2d<float> *data, int dimx, int dimy,int startfreq,float persistance, bool bounds)
	 * \brief erzeugt Zufallskarte mit dem Perlin Noise Algorithmus
	 * \param data Ausgabeparameter
	 * \param dimx Ausdehnung der Karte in x-Richtung
	 * \param dimy Ausdehnung der Karte in y-Richtung
	 * \param startfreq Startfrequenz fuer den Algorithmus. Beeinflusst die Groesse der Strukturen. Muss eine Potenz von 2 sein
	 * \param persistance Beeinflusst die Komplexitaet der Strukturen
	 * \param bounds bestimmt, ob die Karte eine Begrenzung hat
	 */
	static void createPerlinNoise(Matrix2d<float> *data, int dimx, int dimy,int startfreq, float persistance, bool bounds);
};

struct WeightedLocation
{
	
	int m_x;
	int m_y;
	
	float m_value;
	
};

extern "C"
{
	bool operator<(WeightedLocation first, WeightedLocation second);
}



#endif

