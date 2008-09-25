#ifndef SERVERITEM_H
#define SERVERITEM_H

#include "item.h"
#include "damage.h"
#include "creaturestruct.h"

#include <math.h>


/**
 * \struct WeaponAttr
 * \brief Eigenschaften und Attribute einer Waffe
 */
struct WeaponAttr
{
	/**
	 * \var m_damage
	 * \brief Schaden, den die Waffe austeilt
	 */
	Damage m_damage;

	/**
	 * \var m_attack_range
	 * \brief Gibt die Reichweite der Waffe an. Fuer Schusswaffen auf  ITEM_BOW_RANGE setzen
	 */
	float m_attack_range;

	/**
	 * \var m_two_handed
	 * \brief Ist auf true gesetzt, wenn die Waffe beidhaendig benutzt wird.
	 */
	bool m_two_handed;

	/**
	 * \var m_dattack_speed
	 * \brief Gibt die Aenderung der Angriffsgeschwindigkeit bei Benutzen dieser Waffe an
	 */
	short m_dattack_speed;

	/**
	 * \fn void init()
	 * \brief initialisiert die Datenfelder
	 */
	void init()
	{
		m_attack_range=1;
		m_dattack_speed=0;
		m_two_handed=false;
		m_damage.init();
	}

	/**
	 * \fn WeaponAttr()
	 * \brief Konstruktor
	 */
	WeaponAttr()
	{
		init();
	}
};

/**
 * \struct ItemBasicData
 *  \brief Informationen die noetig sind, um ein bestimmtes Item zu erzeugen
 */
struct ItemBasicData
{
    /**
	 * \var m_useup_effect
	 * \brief Gibt die Veraenderung der dynamischen Attribute an, die beim verbrauchen des Gegenstandes eintritt. Wenn der Zeiger auf NULL gesetzt ist, kann der Gegenstand nicht verbraucht werden.
	 */
	CreatureDynAttrMod* m_useup_effect;

	/**
	 * \var m_equip_effect
	 * \brief Gibt die Veraenderung der Basisattribute an, die beim verwenden des Gegenstandes aus Ausruestungsgegenstand eintritt. Wenn der Zeiger auf NULL gesetzt ist, kann der Gegenstand nicht auf diese Weise verwendet werden.
	 */
	CreatureBaseAttrMod* m_equip_effect;

	/**
	 * \var m_weapon_attr
	 * \brief Gibt den Attribute des Gegenstandes als Waffe an. Sollte bei allen Gegenstaenden, die nicht Waffen sind auf NULL gesetzt sein
	 */
	WeaponAttr* m_weapon_attr;

	/**
	 * \var char m_level_req
	 * \brief Erforderliches Level um das Item zu verwenden
	 */
	char m_level_req;

	/**
	 * \var char m_char_req
	 * \brief Anforderungen an die Charakterklasse
	 */
	char m_char_req;

	/**
	* \var m_subtype;
	* \brief Gibt die Unterart eines Gegenstands an ((Langschwert, Dolch usw)
	*/
	Item::Subtype m_subtype;


	/**
	 * \var m_type;
	 * \brief Gibt Typ des Gegenstands an (Waffe, Helm usw...)
	 */
	Item::Type m_type;

	/**
	* \var m_size
	\brief Gibt die Groesse des Gegenstandes (klein,mittel, gross) an
	*/
	Item::Size m_size;
	
	/**
	 * \var int m_price
	 * \brief Wert des Gegenstandes
	 */
	int m_price;

	/**
	 * \var float m_modchance[31]
	 * \brief relative Chance auf die verschiedenen Modifikationen
	 */
	float m_modchance[31];

	/**
	 * \var float m_min_enchant
	 * \brief minimale Starke einer Verzauberung des Items
	 */
	float m_min_enchant;

	/**
	 * \var float m_max_enchant
	 * \brief maximale Starke einer Verzauberung des Items
	 */
	float m_max_enchant;

	ItemBasicData();

};

/**
 * \struct ServerItem
 * \brief Item mit zusaetzlichen Informationen zu Attributen und Eigenschaften
 */
struct ServerItem : public Item
{

	/**
	 * \fn ServerItem()
	 * \brief Konstruktor
	 */
	ServerItem();

	
	/**
	 * \fn ServerItem(ItemBasicData& data)
	 * \brief erzeugt ein neuen Gegenstand mit den vorgegebenen Daten
	 * \param data Daten auf deren Basis der Gegenstand erzeugt wird
	 */
	ServerItem(ItemBasicData& data);

	/**
	 * \fn virtual ~ServerItem()
	 * \brief Destruktor
	 */
	virtual ~ServerItem();


	/**
	 * \fn enum InfoFlags
	 * \brief Zaehlt auf, welche Informationen zu einem Item vorliegen koennen
	 */
	enum InfoFlags
	{
		NOINFO=0,
		USEUP_INFO=1,
 		EQUIP_INFO=2,
		WEAPON_INFO=4,
	};

	/**
	 * \var m_price;
	 * \brief Gibt Preis des Gegenstands an
	 */
	int m_price;

	/**
	 * \var m_useup_effect
	 * \brief Gibt die Veraenderung der dynamischen Attribute an, die beim verbrauchen des Gegenstandes eintritt. Wenn der Zeiger auf NULL gesetzt ist, kann der Gegenstand nicht verbraucht werden.
	 */
	CreatureDynAttrMod* m_useup_effect;

	/**
	 * \var m_equip_effect
	 * \brief Gibt die Veraenderung der Basisattribute an, die beim verwenden des Gegenstandes aus Ausruestungsgegenstand eintritt. Wenn der Zeiger auf NULL gesetzt ist, kann der Gegenstand nicht auf diese Weise verwendet werden.
	 */
	CreatureBaseAttrMod* m_equip_effect;

	/**
	 * \var m_weapon_attr
	 * \brief Gibt den Attribute des Gegenstandes als Waffe an. Sollte bei allen Gegenstaenden, die nicht Waffen sind auf NULL gesetzt sein
	 */
	WeaponAttr* m_weapon_attr;

	/**
	 * \var char m_level_req
	 * \brief Erforderliches Level um das Item zu verwenden
	 */
	char m_level_req;

	/**
	 * \var char m_char_req
	 * \brief Anforderungen an die Charakterklasse
	 */
	char m_char_req;

	/**
	 * \var float m_magic_power
	 * \brief Gibt Verzauberungsstärke an
	 */
	float m_magic_power;

	/**
	 * \fn char* toSavegame(CharConv* cv)
	 * \brief Erzeugt ein binaeres Savegame und schreibt es in den Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	void toStringComplete(CharConv* cv);


	/**
	 * \fn char* fromSavegame(CharConv* cv)
	 * \brief Laedt die Daten aus einem Savegame
	 * \param buf Zeiger auf Savegame
	 * \return Zeiger hinter den Datenbereich
	 */
	void fromStringComplete(CharConv* cv);


	/**
	 * \fn std::string getDescription()
	 * \brief Gibt eine Beschreibung in Textform aus
	 */
	std::string getDescription();

	/**
	 * \fn calcPrice()
	 * \brief Berechnet den Wert des Items
	 */
	void calcPrice();

};

#endif
