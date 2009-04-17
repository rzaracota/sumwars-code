#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class World;
class Region;
#include "debug.h"
#include "geometry.h"
#include "charconv.h"
#include "networkstruct.h"
#include <string>

/**
 * \class GameObject
 * \brief Basisklasse fuer alle Objekte der Spielwelt
 */
class GameObject
{
	public:
	
		/**
		* \enum Layer
		* \brief Ebene des Objektes. Wird verwendet fuer Kollisionserkennung und Einordnung der Objekte
		*/
		enum Layer
		{
			LAYER_BASE =0x01,
			LAYER_AIR = 0x02,
			LAYER_DEAD = 0x04,
			LAYER_ALL = 0x0F,
	
			LAYER_SPECIAL = 0x10,
		};
		
		/**
		 * \brief Grobe Einteilung des Typs
		 */
		typedef std::string Type;
		
		/**
		 * \brief Feinere Einteilung des Typs
		 */
		typedef std::string Subtype;
		
		/**
		 * \brief Status des Objekts
		 */
		enum State
		{
			STATE_INACTIVE=0,
			STATE_ACTIVE =1,
			STATE_DIEING =2,
			STATE_DEAD =3,
			STATE_REGION_DATA_REQUEST=6,
			STATE_REGION_DATA_WAITING=4,
			STATE_ENTER_REGION =5,
			STATE_QUIT=9,
			STATE_STATIC = 10,
			STATE_SPEAK= 20,
			STATE_NONE = 30,
			STATE_AUTO = 31,
  
			STATE_FLYING = 41,
			STATE_EXPLODING = 42,
			STATE_GROWING = 43,
			STATE_STABLE = 44,
			STATE_VANISHING =45,
			STATE_DESTROYED= 46
		};
		
		/**
		 * \enum BaseType
		 * \brief Sehr grundlegende Unterscheidung zwischen verschiedenen Objektgruppen
		 */
		enum BaseType
		{
			WORLDOBJECT =1,
			PROJECTILE = 2,
			DROPITEM = 3,
		};
		
		/**
		 * \enum Fraction
		 * \brief Parteien fuer die Lebewesen kaempfen koennen
		 */
		enum Fraction
		{
			NOFRACTION = 0,
			FRAC_HUMAN = 1,
			FRAC_DEMON = 2,
			FRAC_UNDEAD = 3,
			FRAC_DWARF = 4,
			FRAC_SUMMONER = 5,
			FRAC_MONSTER = 6,
			FRAC_NEUTRAL_TO_ALL =8,
			FRAC_HOSTILE_TO_ALL=9,
			FRAC_PLAYER_PARTY = 10,
		};
		
		
	public:
		
		/**
		 * \fn GameObject(int id)
		 * \brief Konstruktor
		 * \param id ID des Objekts
		 *
		 * Erstellt ein neues GameObject
		 */
		GameObject( int id);

		
		/**
		 * \fn virtual ~GameObject()
		 * \brief Destruktor
		 */
		virtual ~GameObject()
		{
		}
	
		/**
		  * \fn State getState()
		 * \brief Gibt den aktuellen Status des Objektes aus
		 * \return Status des Objektes
		 */
		State getState()
		{
			return m_state;
		}

		/**
		 * \fn void setState(State s, bool throwevent = true)
		 * \brief Setzt den Status
		 * \param s Status
		 * \param throwevent Wenn auf true gesetzt, wird der Status zwischen den Clients per Event abgeglichen
		 */
		void setState(State s, bool throwevent = true);
		

		/**
		 * \fn int getId()
		 * \brief Gibt die ID des objektes zurueck
		 * \return ID
	 	*/
		int getId()
		{
			return m_id;
		}
		
		/**
		 * \fn void setId(int id)
		 * \brief Setzt die ID eines Objektes
		 */
		void setId(int id)
		{
			m_id = id;
		}
		
		/**
		 * \fn short getRegionId()
		 * \brief Gibt die ID der Region aus
		 */
		short getRegionId()
		{
			return m_region_id;
		}
		
		/**
		 * \fn void setRegionId()
		 * \brief Setzt die ID der Region
		 * \param region_id ID der neuen Region
		 */
 		void setRegionId(short region_id)
		{
			m_region_id = region_id;
		}
		
		/**
		 * \fn Region* getRegion()
		 * \brief Gibt die Region, in der das Objekt sich befindet als Zeiger aus. Resultat kann 0 sein.
		 */
		Region* getRegion();
		
		
		/**
		 * \fn BaseType getBaseType()
		 * \brief Gibt den grundlegenden Typ aus
		 */
		BaseType getBaseType()
		{
			return m_base_type;
		}
		
		/**
		 * \fn void setBaseType(BaseType base_type)
		 * \brief Setzt den Basistyp
		 * \param base_type Basistyp
		 */
		void setBaseType(BaseType base_type)
		{
			m_base_type = base_type;
		}
		
		/**
		 * \fn Type& getType()
		 * \brief Gibt den Typ aus
		 */
		Type& getType()
		{
			return m_type;
		}
		
		/**
		 * \fn void setType(Type type)
		 * \brief Setzt den Typ
		 * \param type neuer Typ
		 */
		void setType(Type type);
		
		
		/**
		 * \fn Subype& getSubtype()
		 * \brief Gibt den Subtyp aus
		 */
		Subtype& getSubtype()
		{
			return m_subtype;
		}
		
		/**
		 * \fn void setSubtype(Subtype subtype)
		 * \brief Setzt den Subtyp
		 * \param subtype neuer Subtyp
		 */
		void setSubtype(Subtype subtype);
		
		/**
		 * \fn int getNetEventMask()
		 * \brief Gibt die Bitmaske der NetEvents aus
		 */
		int getNetEventMask()
		{
			return m_event_mask;
		}
		
		/**
		 * \fn void addToNetEventMask(int mask)
		 * \brief Fuegt die angegebene Maske per OR zur aktuell hinzu
		 * \param mask Bitmaske
		 */
		void addToNetEventMask(int mask);
		
	
		/**
		 * \fn void clearNetEventMask()
		 * \brief Setzt die Bitmaske der NetEvents auf 0
		 */
		void clearNetEventMask()
		{
			m_event_mask =0;
		}
		
		/**
		 * \fn int& getEventMaskRef()
		 * \brief Gibt eine Rerenz auf die Eventmaske aus
		 */
		int& getEventMaskRef()
		{
			return m_event_mask;
		}
		
		/**
		 * \fn short getLayer()
		 * \brief Gibt die Ebene des Objektes aus
		 */
		short getLayer()
		{
			return m_layer;
		}
	
		/**
		 * \fn void setLayer(short layer)
		 * \brief Setzt die Ebene des Objektes
		 * \param layer Ebene
		 */
		void setLayer(short layer);
		
		/**
		 * \fn Shape* getShape()
		 * \brief Gibt die Form des Objektes aus
		 */
		Shape* getShape()
		{
			return &m_shape;
		}
		
		/**
		 * \fn Vector getPosition()
		 * \brief Gibt die Position aus
		 */
		Vector getPosition()
		{
			return m_shape.m_center;
		}
		
		/**
		 * \fn void setPosition(Vector pos)
		 * \brief Setzt die Position des Objekes
		 * \param pos neue Position
		 */
		virtual void setPosition(Vector pos);
		
		/**
		 * \fn void setAngle(float angle)
		 * \brief Setzt den Drehwinkel
		 * \param angle neuer Drehwinkel
		 */
		void setAngle(float angle);
		
		
		/**
		 * \fn Vector& getSpeed()
		 * \brief Gibt die Geschwindigkeit des Objektes aus
		 */
		Vector getSpeed()
		{
			return m_speed;
		}
		
		/**
		 * \fn setSpeed(Vector speed, bool throwevent = true)
		 * \brief Setzt die Geschwindigkeit des Objektes
		 * \param speed neue Geschwindigkeit
		 * \param throwevent Wenn auf true gesetzt, wird die Richtung ueber das Netzwerk ausgetauscht
		 */
		void setSpeed(Vector speed, bool throwevent = true);
		
		/**
		 * \fn void toString(CharConv* cv)
		 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
		 * \param cv Ausgabepuffer
		 */
		virtual void toString(CharConv* cv);


		/**
		 * \fn virtual void fromString(CharConv* cv)
		 * \brief Erzeugt das Objekt aus einem String
		 * \param cv Eingabepuffer
		 */
		virtual void fromString(CharConv* cv);
		
		/**
		 * \fn std::string getName()
		 * \brief Gibt den Name des Objektes aus
		 */
		virtual std::string getName();


		/**
		 * \fn std::string getNameId()
		 * \brief Gibt Name mit angehaengter ID aus
		 */
		std::string getNameId();
		
		/**
		 * \fn virtual void writeNetEvent(NetEvent* event, CharConv* cv)
		 * \brief Schreibt die Daten zu einem NetEvent in den Bitstream
		 * \param event NetEvent das beschrieben wird
		 * \param cv Bitstream
		 */
		virtual void writeNetEvent(NetEvent* event, CharConv* cv);
	
	
		/**
		 * \fn virtual void processNetEvent(NetEvent* event, CharConv* cv)
		 * \brief Fuehrt die Wirkung eines NetEvents auf das Objekt aus. Weitere Daten werden aus dem Bitstream gelesen
	 	*/
		virtual void processNetEvent(NetEvent* event, CharConv* cv);
		
		
		/**
		 * \fn virtual bool update ( float time)
		 * \brief Aktualisiert das GameObject, nachdem eine bestimmte Zeit vergangen ist. Alle Aktionen des Objekts werden auf diesem Weg ausgeloest. Die Funktion ist virtuell und wird von den abgeleiteten Klassen ueberschrieben
		 * \param time Menge der vergangenen Zeit in Millisekunden
		 * \return bool, der angibt, ob die Aktualisierung fehlerfrei verlaufen ist
		 */
		virtual  bool  update ( float time) 
		{
			return true;
		}
		
		/**
		 * \fn float getHeight()
		 * \brief Gibt die Hoehe aus
		 * 
		 */
		float getHeight()
		{
			return m_height;
		}
		
		/**
		 * \fn void setHeight(float height)
		 * \brief Setzt die Hoehe
		 * \param height Hoehe
		 */
		void setHeight(float height)
		{
			m_height = height;
		}
		
	private:
		/**
		 * \var int m_id
		 * \brief ID des Objektes
		 */
		int m_id;
		
		/**
		 * \var short m_region_id
		 * \brief ID der Region, in der sich das Objekt befindet
		 */
		short m_region_id;
		
		/**
		 * \var State m_state
		 * \brief Status des Objektes
		 */
		State m_state;
		
		/**
		 * \var BaseType m_base_type
		 * \brief Grundlegende Art des Objektes
		 */
		BaseType m_base_type;
		
		/**
		 * \var Type m_type
		 * \brief Typ des Objekts
		 */
		Type m_type;
		
		/**
		 * \var Subtype m_subtype
		 * \brief Untertyp des Objekts
		 */
		Subtype m_subtype;
		
		/**
		 * \var Shape m_shape
		 * \brief Position und Form des Objektes
		 */
		Shape m_shape;
		
		/**
		 * \var int m_event_mask
		 * \brief Bitmaske die Modifikationen anzeigt
		 */
		int m_event_mask;
		
		/**
		 * \var float m_height
		 * \brief Hoehe des Objektes
		 */
		float m_height;
		
		/**
		 * \var short m_layer
		 * \brief Gibt die Ebene an, in der sich das Objekt befindet
		 */
		short m_layer;
		
		/**
		 * \var Vector m_speed
		 * \brief Gibt die Geschwindigkeit des Objektes an
		 */
		Vector m_speed;
};

typedef std::string PlayerLook;

#endif
