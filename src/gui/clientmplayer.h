#ifndef CLIENTMPLAYER_H
#define CLIENTMPLAYER_H

#include "clientwobject.h"
#include "../shared/itemlist.h"
#include <string>
#include "../shared/damage.h"

/**
 * \class class ClientMPlayer
 * \brief Klasse fuer die Spielerfigur auf Serverseite
 */
class ClientMPlayer : public ClientWObject
{
	public:
		/**
	 * \fn ClientMPlayer(int id)
		 * \brief Konstruktor
		 * \param id ID des Objektes
		 */
		ClientMPlayer(int id);
		
		/**
		 * \fn ~ClientMPlayer()
		 * \brief Destruktor
		 */
		virtual ~ClientMPlayer();
		
		/**
		 * \fn void fromString(CharConv* cv)
		 * \brief Erzeugt das Objekt aus einem String
		 * \param buf Objekt als String
		 * \return Zeiger hinter den gelesenen Datenbereich
		 */
		virtual void fromString(CharConv* cv);
		
		/**
		 * \fn bool checkAbility(Action::ActionType at)
		 * \brief Testet ob eine Faehigkeit erlernt ist
		 */
		bool checkAbility(Action::ActionType at);
		
		/**
		 * \fn bool checkAbilityLearnable(Action::ActionType at)
		 * \brief Testet ob eine Faehigkeit erlernbar ist
		 */
		bool checkAbilityLearnable(Action::ActionType at);
		
		/** 
		 * \var char m_level
		 * \brief Level des Spielers
		 */
		char m_level;
		
		/**
		 * \var m_health
		 * \brief aktuelle Lebenspunkte des Spielers
		 */
		float m_health;
		
		/**
		 * \var m_max_health
		 * \brief Maximale Lebenspunkte des Spielers
		 */
		float m_max_health;
		
		/**
		 * \var short m_strength
		 * \brief Staerke des Spielers
		 */
		short m_strength;
		
		/**
		 * \var short m_dexterity
		 * \brief Geschick des Spielers
		 */
		short m_dexterity;
		
		/**
		 * \var short m_magic_power
		 * \brief Magie des Spielers
		 */
		short m_magic_power;
		
		/**
		 * \var short m_willpower
		 * \brief Willenskraft des Spielers
		 */
		short m_willpower;
		
		/**
		 * \var short m_attribute_points
		 * \brief Anzahl noch zu verteilender Attributspunkte
		 */
		short m_attribute_points;
		
		/**
		 * \var m_attack_speed
		 * \brief gibt die Geschwindigkeit an, mit der der Spieler angreifen kann, 1000 entspricht einem Schlag pro Sekunde 
		 */
		short  m_attack_speed;
		
		/**
		 * \var m_attack_range
		 * \brief Gibt die Reichweite der Waffe an.
		 */
		float m_attack_range;
		
		/**
		 * \var m_power
		 * \brief Gibt die Durchschlagskraft an.
		 */
		float m_power;
		
		/**
		 * \var Damage m_base_damage
		 * \brief Schaden der mit der Basisattacke ausgeteilt wird
		 */
		Damage m_base_damage;
		
		/**
		 * \var  Damage m_left_damage
		 * \brief Schaden der mit der auf Linksklick ausgeloesten Faehigkeit ausgeteilt wird
		 */
		Damage m_left_damage;
		
		/**
		 * \var Damage m_right_damage
		 * \brief Schaden der mit der auf Rechtsklick ausgeloesten Faehigkeit ausgeteilt wird
		 */
		Damage m_right_damage;
		
		/**
		 * \var m_resistances[4]
		 * \brief Resistenzen des Spielers gegen die vier Schadensarten in Prozent
		 */
		short m_resistances[4];
	
		/**
		 * \var m_resistances_cap[4]
		 * \brief Obergrenzen fuer die Resistenzen des Spielers gegen die vier Schadensarten in Prozent
		 */
		short m_resistances_cap[4];
		
		/**
		 * \var m_experience
		 * \brief aktueller Erfahrungswert
		 */
		float m_experience;
		
		/**
		 * \var m_max_experience
		 * \brief maximaler Erfahrungswert vor Erreichen des naechsten Level
		 */
		float m_max_experience;
		
		/** \var m_armor;
		 * \brief Ruestungswert, beeinflusst den Schaden von Angriffen
		 */
		short m_armor;
	
		/**
		 * \var m_block;
		 * \brief Blockwert, beeinflusst die Chance Angriffe abzuwehren
	 	*/
		short m_block;
	
	
		/**
		 * \var m_attack;
		 * \brief Attackewert, beeinflusst die Chance zu treffen
	 	*/
		short m_attack;
		
		/**
		 * \var m_abilities[6]
		 * \brief Bitmaske die angibt, welche Fähigkeiten der Spieler benutzen kann.
		 */
		int m_abilities[6];
		
		/**
		 * \var float m_timer1_perc
		 * \brief Prozentsatz der Zeit von Timer1 die noch ablaufen muss
		 */
		float m_timer1_perc;
		
		/**
		 * \var float m_timer2_perc
		 * \brief Prozentsatz der Zeit von Timer2 die noch ablaufen muss
		 */
		float m_timer2_perc;
		
		/**
		 * \var Equipement* m_equipement
		 * \brief Ausruestung des Spielers
		 */
		Equipement* m_equipement;
		
};


#endif // CLIENTMPLAYER_H

