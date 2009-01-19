
#ifndef CREATURESTRUCT_H
#define CREATURESTRUCT_H


#include <list>
#include <string>
#include <memory.h>
#include "debug.h"

#define NR_STATUS_MODS 8

#define NR_EFFECTS 1

/**
 * \struct CreatureBaseAttr
 * \brief Struktor mit den Basisattributen einer Kreatur
 */
struct CreatureBaseAttr
{
	/**
	 * \enum Attribute
	 * \brief Aufzaehlung der Attributwerte
	 */
	enum Attribute
	{
		STRENGTH =0,
		DEXTERITY=1,
		MAGIC_POWER =2,
		WILLPOWER =3
	};
	
	/**
	 * \var m_max_health;
	 * \brief maximale Gesundheitspunkte
	 */
	float m_max_health;
	
	/**
	 * \var m_level;
	 * \brief Erfahrungslevel
	 */
	char m_level;
	
	/**
	 * \var m_armor;
	 * \brief der passive R&uuml;stungswert, beeinflusst den Schaden von Angriffen
	 */
	short m_armor;
	
	/**
	 * \var m_block;
	 * \brief der aktive Verteidigungswert, beeinflusst die Chance Angriffe abzuwehren
	 */
	short m_block;
	
	/**
	 * \var m_attack;
	 * \brief Attackewert, beeinfluss die Trefferchance
	 */
	short m_attack;
	
	
	/**
	 * \var m_strength
	 * \brief Staerke des Lebewesens
	 */
	short m_strength;
	
	/**
	 * \var m_dexterity
	 * \brief Geschicklichkeit des Lebewesens
	 */
	short m_dexterity;
	
	
	/**
	 * \var m_magic_power
	 * \brief Zauberkraft des Lebewesens
	 */
	short m_magic_power;
	
	/**
	 * \var m_willpower;
	 * \brief Willenskraft des Lebewesens
	 */
	short m_willpower;
	
	/**
	 * \var m_resistances[4]
	 * \brief Resistenzen des Lebewesens gegen die vier Schadensarten in Prozent
	 */
	short m_resistances[4];
	
	/**
	 * \var m_resistances_cap[4]
	 * \brief Obergrenzen fuer die Resistenzen des Lebewesens gegen die vier Schadensarten in Prozent
	 */
	short m_resistances_cap[4];
	
	/**
	 * \var m_max_experience
	 * \brief maximaler Erfahrungswert vor Erreichen des n&auml;chsten Level
	 */
	float m_max_experience;
	
	
	/**
	 * \var m_walk_speed
	 * \brief gibt die Geschwindigkeit an, mit der der Spieler laufen kann, 100 entspricht einem Schritt pro Sekunde
	 */
	short m_walk_speed;
	
	/**
	 * \var m_step_length
	 * \brief Schrittlaenge beim laufen
	 */
	float m_step_length;
	
	/**
	 * \var m_attack_speed
	 * \brief gibt die Geschwindigkeit an, mit der der Spieler angreifen kann, 1000 entspricht einem Schlag pro Sekunde 
	 */
	short  m_attack_speed;
	
	/**
	 * \var m_abilities[6]
	 * \brief Bitmaske die angibt, welche Fähigkeiten der Spieler benutzen kann.
	 */
	int m_abilities[6];
	
	/**
	 * \var m_attack_range
	 * \brief Gibt die Reichweite der Waffe an. Fuer Schusswaffen auf  ITEM_BOW_RANGE setzen
	 */
	float m_attack_range;
	
	/**
	 * \var m_special_flags
	 * \brief Bitmaske, die angibt, welche Spezialfähigkeiten aktiviert sind (1 bedeutet aktiviert). Die Bedeutung der verschiedenen Bits ist von genauen Typ des Lebewesens abhängig.
	 */
	int m_special_flags;
	
	/**
	 * \var char m_immunity
	 * \brief Bitmaske, welche angibt gegen welche Statusveraenderungen das Lebewesen immun ist
	 */
	char m_immunity;
	
	/**
	 * \fn int getValue(std::string valname)
	 * \brief Schiebt den gewuenschten Attributwert eines Objektes auf den Lua Stack
	 * \param valname Name des Wertes
	 */
	int getValue(std::string valname);
	
	/**
	 * \fn virtual bool setValue(std::string valname, int& event_mask)
	 * \brief Setzt den gewuenschten Attributwert eines Objektes
	 * \param valname Name des Wertes
	 * \param event_mask Bitmaske, die anzeigt, welche Daten neu uebertragen werden muessen
	 */
	bool setValue(std::string valname, int& event_mask);
	
};

/**
 * \struct CreatureBaseAttrMod
 * \brief Struktur fuer die Modifikationen der Basisattribute einer Kreatur
 */
struct CreatureBaseAttrMod
{
	/**
	 * \var m_darmor;
	 * \brief Aenderung des  R&uuml;stungswerts, beeinflusst den Schaden von Angriffen
	 */
	short m_darmor;
	
	/**
	 * \var m_dblock;
	 * \brief Aenderung des Blockwerts, beeinflusst die Chance Angriffe abzuwehren
	 */
	short m_dblock;
	
	/**
	 * \var float m_dmax_health
	 * \brief Aenderung der maximalen Lebenspunkte
	 */
	float m_dmax_health;
	
	/**
	 * \var m_dattack;
	 * \brief Aenderung des Attackewertes, beeinflusst die Chance zu treffen
	 */
	short m_dattack;
	
	
	/**
	 * \var m_dstrength
	 * \brief Aenderung der Staerke des Lebewesens
	 */
	short m_dstrength;
	
	/**
	 * \var m_ddexterity
	 * \brief Aenderung der Geschicklichkeit des Lebewesens
	 */
	short m_ddexterity;
	
	
	/**
	 * \var m_dmagic_power
	 * \brief Aenderung der Zauberkraft des Lebewesens
	 */
	short m_dmagic_power;
	
	/**
	 * \var m_dwillpower;
	 * \brief Aenderung der Willenskraft des Lebewesens
	 */
	short m_dwillpower;
	
	/**
	 * \var m_dresistances[4]
	 * \brief Aenderung der Resistenzen des Lebewesens gegen die vier Schadensarten in Prozent
	 */
	short m_dresistances[4];
	
	/**
	 * \var m_dresistances_cap[4]
	 * \brief Aenderung der Obergrenzen fuer die Resistenzen des Lebewesens gegen die vier Schadensarten in Prozent
	 */
	short m_dresistances_cap[4];
	/**
	 * \var m_dwalk_speed
	 * \brief Aenderung der  Geschwindigkeit, mit der der Spieler laufen kann, 100 entspricht einem Schritt pro Sekunde
	 */
	 
	short m_dwalk_speed;
	
	/**
	 * \var m_dattack_speed
	 * \brief Aenderung der  Geschwindigkeit, mit der der Spieler angreifen kann, 100 entspricht einem Schlag pro Sekunde 
	 */
	short  m_dattack_speed;
	
	/**
	 * \var m_xspecial_flags
	 * \brief Aenderung der Specialflags. Die angegebene Bitmaske wird mit OR zur bestehenden hinzugefügt.
	 */
 	int m_xspecial_flags;
	
	/**
	 * \var m_time
	 * \brief Zeitdauer, die diese Modifikation noch wirksam ist in Millisekunden
	 */
	float m_time;
	
	/**
	 * \var m_xabilities[6]
	 * \brief Bitmaske die die Aenderungen der Faehigkeiten angibt. Die angegebene Bitmaske wird mit OR zur bestehenden hinzugefügt.
	 */
	int m_xabilities[6];
	
	/**
	 * \var char m_ximmunity
	 * \brief Bitmaske die die Aenderungen der Immunitäten angibt. Die angegebene Bitmaske wird mit OR zur bestehenden hinzugefügt.
	 */
	
	char m_ximmunity;
	
	/**
	 * \fn void init()
	 * \brief Belegt die Struktur mit Nullen
	 */
	void init()
	{
		DEBUG5("init CreatureBaseAttrMod");
		// alles nullen
		memset(this,0, sizeof(CreatureBaseAttrMod));
	}
	
	/**
	 * \fn CreatureBaseAttrMod()
	 * \brief Konstruktor
	 */
	CreatureBaseAttrMod()
	{
		init();
	}
	
};

/**
 * \struct CreatureDynAttr
 * \brief Struktur mit den sich staendig aendernden Attributen eines Lebewesens
 */
struct CreatureDynAttr
{
	/**
	 * \enum Effect
	 * \brief Auflistung von visuellen Effekten
	 */
	enum Effect
	{
		
		BLEEDING =0,
	};
	
	
	/**
	 * \var m_health;
	 * \brief Anzahl der aktuellen Gesundheitspunkte
	 */
	float m_health;
	
	/**
	 * \var m_experience
	 * \brief Erfahrungswert
	 */
	float m_experience;
	
	/**
	 * \var m_last_attacker_id;
	 * \brief ID des letzten Angreifers, von dem diese Kreatur zuletzt angegriffen wurde
	 */	
	int m_last_attacker_id;
	
	/**
	 * \var float m_effect_time[NR_EFFECTS]
	 * \brief Gibt an wielange verschiedene Effekte noch wirksam sind
	 */
	float m_effect_time[NR_EFFECTS];

	/**
	 * \var m_status_mod_time[NR_STATUS_MODS]
	 * \brief Gibt an wie lange die verschiedenen Status Modifikationen noch wirken. Ein Wert von 0 bedeutet, dass die Statusmodifikation nicht wirksam ist
	 */
	float m_status_mod_time[NR_STATUS_MODS];
	
	/**
	 * \var m_status_mod_immune_time[NR_STATUS_MODS]
	 * \brief Gibt an, wie lange der Spieler gegen verschiedene Statusveränderungen immun ist.
	 */
	float m_status_mod_immune_time[NR_STATUS_MODS];
	
	/**
	 * \var m_temp_mods
	 * \brief Vektor mit alle aktuell wirkenden temporaeren Modifikation auf die Basisattribute
	 */
	std::list<CreatureBaseAttrMod> m_temp_mods;
	
	
	/**
	 * \fn int getValue(std::string valname)
	 * \brief Schiebt den gewuenschten Attributwert eines Objektes auf den Lua Stack
	 */
	int getValue(std::string valname);
	
	/**
	 * \fn virtual bool setValue(std::string valname, int& event_mask)
	 * \brief Setzt den gewuenschten Attributwert eines Objektes
	 * \param valname Name des Wertes
	 * \param event_mask Bitmaske, die anzeigt, welche Daten neu uebertragen werden muessen
	 */
	bool setValue(std::string valname, int& event_mask);
};

/**
 * \struct CreatureDynAttrMod
 * \brief Struktur fuer die Aenderung der dynamischen Attribute eines Lebewesens
 */
struct CreatureDynAttrMod
{
	/**
	 * \var  m_dhealth
	 * \brief Aenderung der aktuellen Lebenspunkte
	 **/
	float m_dhealth;
	
	/**
	 * \var float m_dstatus_mod_immune_time[NR_STATUS_MODS];
	 * \brief Gibt an, wie lange der Spieler gegen verschiedene Statusveraenderungen immunisiert wird. Heilt
	 **/
	float m_dstatus_mod_immune_time[NR_STATUS_MODS];

	/**
	 * \fn void init()
	 * \brief Belegt die Datenelemente mit Nullen
	 */
	void init()
	{
		DEBUG5("init CreatureDynAttrMod");
		// alles nullen
		memset(this,0, sizeof(CreatureDynAttrMod));
	}
	
	/**
	 * \fn CreatureDynAttrMod()
	 * \brief Konstruktor
	 */
	CreatureDynAttrMod()
	{
		init();
	}
	
};

/**
 * \struct CreatureSpeakText
 * \brief Struktur fuer den Text den eine Kreatur spricht
 */
struct CreatureSpeakText
{
	/**
	 * \var std::string m_text
	 * \brief Der Text
	 */
	std::string m_text;
	
	/**
	 * \var float m_time
	 * \brief Gibt an, wie lange der Text zu sehen ist
	 */
	float m_time;
	
	/**
	 * \var std::list < std::pair<std::string, std::string> > m_answers
	 * \brief Liste von moeglichen Antworten und  dem zugehoerigen Thema
	 */
	std::list < std::pair<std::string, std::string> > m_answers;
	
	/**
	 * \fn void operator=( CreatureSpeakText& other)
	 * \brief Zuweisungsoperator
	 * \param other zugewiesene Struktur
	 */
	void operator=( CreatureSpeakText& other);
	
	/**
	 * \fn bool empty()
	 * \brief Gibt aus, ob Text vorhanden ist
	 */
	bool empty();
	
	/**
	 * \fn void clear()
	 * \brief Loescht den Text
	 */
	void clear();
	
};

#endif
