#ifndef DAMAGE_H
#define DAMAGE_H


#include "worldobject.h"
#include <stdlib.h>
#include <string>

#define NR_STATUS_MODS 8

/**
 * \enum AIMods
 * \brief Modifikationen fuer die AI
 */
	enum AIMods
{
	TAUNT =0,
 	NR_AI_MODS,
};

/**
 * \struct Damage
 * \brief Parameter fuer ausgeteilten Schaden
 */
struct Damage 
{
	
	
	/**
	 * \var m_min_damage[4]
	 * \brief Gibt den minimalen Schaden fuer die vier verschiedenen Schadensarten an.
	 */
	float m_min_damage[4];
	
	/**
	 * \var m_max_damage[4]
	 * \brief Gibt den maximalen Schaden fuer die vier verschiedenen Schadensarten an.
	 */
	float m_max_damage[4];
	
	/**
	 * \var m_multiplier[4]
	 * \brief Multiplikatoren fuer die einzelnen Schadensarten
	 */
	float m_multiplier[4];
	
	/**
	 * \var m_attack
	 * \brief Gibt den Attackwert an. Der Attackewert beeinflusst Trefferchance.
	 */
	float m_attack;
	
	/**
	 * \var m_power
	 * \brief Gibt die Durchschlagskraft an. Die Durchschlagskraft beeinflusst, wie stark der Schaden durch die Ruestung gemindert wird
	 */
	float m_power;
	
	/**
	 * \var  m_attacker_id
	 * \brief ID des Angreifers
	 */
	int m_attacker_id;
	
	/**
	 * \var WorldObject::Fraction m_attacker_fraction
	 * \brief Fraktion des Angreifers
	 */
	WorldObject::Fraction m_attacker_fraction;
	

	/**
	 * \var float m_crit_perc
	 * \brief Chance auf kritischen Treffer
	 */
	float m_crit_perc;
	
	/**
	 * \var m_status_mod_power
	 * \brief Gibt fuer alle Statusveraenderungn die Zauberkraft an. Eine Zauberkraft von 0 bedeutet, dass der betreffende Effekt nicht wirksam wird.
	 */
	short m_status_mod_power[NR_STATUS_MODS];
	
	/**
	 * \var short m_ai_mod_power[NR_AI_MODS]
	 * \brief Staerke der Einfluesse auf die AI
	 */
	short m_ai_mod_power[NR_AI_MODS];
	
	
	/**
	 * \var m_special_flags
	 * \brief bitkodierte Zusammenfassung besonderer Eigenschaften des Schaden, wie besonderer Schaden gegen bestimmte Typen von Monstern.
	 */
	short m_special_flags;
	
	/**
	 * \enum DamageType
	 * \brief Aufzaehlung der Schadensarten
	 */
	enum DamageType
	{
		PHYSICAL =0,
		AIR =1,
		ICE =2,
		FIRE =3
	};
	
	/**
	 * \enum StatusMods
	 * \brief Aufzaehlung der Statusmodifikationen
	 */
	enum StatusMods
	{
		BLIND =0,
		POISONED =1,
		BERSERK = 2,
		CONFUSED = 3,
		MUTE = 4,
		PARALYZED=5,
		FROZEN = 6,
		BURNING = 7,
		
	};
	
	/**
	 * \enum SpecialFlags
	 * \brief Aufzaehlung von Spezialeigenschaften des Schadens
	 */
	enum SpecialFlags
	{
		NOFLAGS =0,
		UNBLOCKABLE = 0x1,
		IGNORE_ARMOR = 0x2,
		EXTRA_HUMAN_DMG = 0x10,
		EXTRA_DEMON_DMG = 0x20,
		EXTRA_UNDEAD_DMG = 0x40,
		EXTRA_DWARF_DMG = 0x80,
		EXTRA_DRAKE_DMG = 0x100,
		EXTRA_FAIRY_DMG = 0x200,
		EXTRA_GOBLIN_DMG = 0x400,
		EXTRA_ANIMAL_DMG = 0x800,
		EXTRA_SUMMONED_DMG = 0x1000
	};
	
	/**
	 * \enum Usage
	 * \brief Verwendung der Beschreibung von Schaden (unterschiedlich ausfuehrlich)
	 */
	enum Usage
	{
		NORMAL =0,
		ABILITY =1,
		ITEM =2,
	};	
		
  
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 */
	void toString(CharConv* cv);
	
	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	void fromString(CharConv* cv);
	
	/**
	 * \fn void init()
	 * \brief initialisiert die Datenfelder
	 */
	void init();
	
	/**
	 * \fn void normalize()
	 * \brief rechnet alle Multiplikatoren in die Schadenswerte ein
	 */
	void normalize();
	
	/**
	 * \fn Damage()
	 * \brief Konstruktor. Belegt die Datenfelder mit 0
	 */
	Damage();
	
	/**
	 * \fn static std::string getDamageTypeName(DamageType dt)
	 * \brief Gibt zu einem Schadenstyp den Name als String aus
	 */
	static std::string getDamageTypeName(DamageType dt);
	
	/**
	 * \fn static std::string getDamageResistanceName(DamageType dt)
	 * \brief Gibt zu einem Schadenstyp den Name der Resistenz als String aus
	 */
	static std::string getDamageResistanceName(DamageType dt);
	
	/**
	 * \fn static std::string getStatusModName(StatusMods sm)
	 * \brief Gibt zu einer Statusveraenderung den Name als String aus
	 */
	static std::string getStatusModName(StatusMods sm);
	
	/**
	 * \fn float getSumMinDamage()
	 * \brief Gibt die Summe des Minimalschaden ueber die Schadensarten aus
	 */
	float getSumMinDamage();
	
	/**
	 * \fn float getSumMaxDamage()
	 * \brief Gibt die Summe des Maximalschadens ueber die Schadensarten aus
	 */
	float getSumMaxDamage();
	
	/**
	 * \fn std::string getDamageString((Usage usage= NORMAL)
	 * \brief Erzeugt einen String der den angegebenen Schaden anzeigt
	 * \param usage Gibt an wofuer die Beschreibung verwendet wird (beeinflusst welche Daten angegeben werden)
	 */
	std::string getDamageString(Usage usage= NORMAL);
	
	/**
	 * \fn int getValue(std::string valname)
	 * \brief Schiebt den gewuenschten Attributwert eines Objektes auf den Lua Stack
	 * \param valname Name des Wertes
	 */
	int getValue(std::string valname);
	
	/**
	 * \fn virtual bool setValue(std::string valname)
	 * \brief Setzt den gewuenschten Attributwert eines Objektes
	 * \param valname Name des Wertes
	 */
	bool setValue(std::string valname);
	
	/**
	 * \fn void operator=(Damage& other)
	 * \brief Zuweisungsoperator
	 */
	void operator=(Damage& other);
};
	
#endif

