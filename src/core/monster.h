/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/**
 * \file monster.h
 * \defgroup Monster \ Monster
 * \ingroup Player
 * \brief Stellt das Objekt Monster zur Verf&uuml;gung
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Monster, sowie die Struktur AiVariables
 */
#ifndef MONSTER_H
#define MONSTER_H
#include <string>
#include "creature.h"
#include "dropslot.h"
#include "monsterbase.h"

/**
 * Liste von WorldObject Zeiger mit jeweils einer Bewertung
 */
typedef std::list<std::pair<WorldObject*,float> > WorldObjectValueList;

/**
 * \struct Ai
 * \brief Variablen für die 'K&uuml;nstliche Intelligenz' des Monsters (Struktur)
 */
struct Ai
{
	/**
	 * \enum AiState
	 * \brief Aufzaehlung von Zustaenden der KI
	 */
	enum AiState
	{
		INACTIVE =0,
		ACTIVE =1,
	};
	
	/**
	 * \var MonsterAIVars m_vars
	 * \brief Steuervariablen
	 */
	MonsterAIVars m_vars;
	
	/**
	 * \var WorldObjectValueList* m_goals;
	 * \brief Liste der moeglichen Ziele
	 */
	WorldObjectValueList* m_goals;
	
	/**
	 * \var WorldObjectValueList* m_visible_goals;
	 * \brief Liste der sichtbaren Ziele
	 */
	WorldObjectValueList* m_visible_goals;
	
	/**
	 * \var float m_command_value
	 * \brief Bewertung des aktuellen Kommandos
	 */
	float m_command_value;
	
	/**
	 * \var bool m_rand_command
	 * \brief wenn true, wird der Befehl unter allen Kommandos zufaellig ausgewaehlt
	 */
	bool m_rand_command;
	
	/**
	 * \var int m_command_count
	 * \brief anzahl bisheriger, korrekter Befehle
	 */
	int m_command_count;
	
	/**
	 * \var int m_chase_player_id
	 * \brief ID des Spielers der verfolgt wird
	 */
	int m_chase_player_id;
	
	
	/**
	 * \var Command m_command
	 * \brief durch die AI berechnetes Kommando
	 */
	Command m_command;
	
	/**
	 * \var float m_mod_time[NR_AI_MODS]
	 * \brief Dauer der Beeinflussungen der AI
	 */
	float m_mod_time[NR_AI_MODS];
	
	/**
	 * \var m_state
	 * 
	 * \brief Status der AI des Monsters
	 */
	int m_state;
};

/**
 * \class Monster
 * 
 * \brief Dieses Objekt stellt ein Monster dar. Das Monster wird unter andrem durch Gruppenbezogene KI charakterisiert.
 * 
 */
class Monster : public Creature {

	public:
	
		/**
		* \fn Monster(World* world, int id)
		* \brief Konstruktor
		* \param world 
		* \param id 
		*
		* Legt ein neues Monster Objekt an.
		*/
		Monster(int id);
		
		/**
		* \fn Monster(World* world, int id,MonsterBasicData& data)
		* \brief Erzeugt ein Monster auf Basis der gegebenen Daten
		* \param id 
		* \param data Daten fuer die Erzeugung des Monsters
		*/
		Monster(int id,MonsterBasicData& data);
		
		/**
		* \fn ~Monster()
		* \brief Destruktor
		*
		* Gibt den Allokierten Speicher wieder frei
		*/
		~Monster();
		/**
		* Accessor Methods
		*/
		/**
		* Operations
		*/
		/**
		* \fn virtual bool init()
		* \brief Initialisierung des Monsters
		* \return bool Gibt an ob die Initialisierung erfolgreich war
		*/
		virtual bool init();
		/**
		* \fn virtual bool update (float time)
		* \brief Updatefunktion des Monsters
		* \param time Verstrichene Zeit
		* \return bool, Erfolgreiches Update?
		*
		* Die Updatefunktion des Monsters wird in regelm&auml;ssigen Abst&auml;nden von der World aufgerufen und ben&ouml;tigt die Angabe, wie viel Zeit f&uuml;r das Update zur Verf&uuml;gung gestellt wird. Der R&uuml;ckgabewert gibt an ob die Funktion erfolgreich ausgef&uuml;hrt wurde. Da die Funktion hier virtuell definiert wird, wird sie erst in den abgeleiteten Klassen implementiert.
		*/
		virtual bool update(float time);
		
		
		/**
		* \fn virtual bool destroy ()
		* \brief Zerstoert das Objekt. Die Funktion ist virtuell und wird durch die abgeleiteten Klassen &uuml;berschrieben.
		* \return bool, der angibt ob die Zerstoerung erfolgreich war
		*/
		virtual bool destroy();
		
		/**
		* \fn virtual void updateCommand()
		* \brief aktualisiert das aktuelle Kommando
		*/
		virtual void updateCommand();
		
		/**
		* \fn virtual void calcBestCommand()
		* \brief Berechnet das aus Sicht der AI beste Kommando
		*/
		virtual void calcBestCommand();
		
		/**
		* \fn virtual void evalCommand(Action::ActionType act)
		* \brief Bewertet das Kommando. Die Bewertung wird in der AI Struktur gespeichert, falls das Kommando besser ist, als das aktuell dort gespeicherte
		* \param act zu bewertende Aktion
		*/
		virtual void evalCommand(Action::ActionType act);
		
		/**
		* \fn virtual void die()
		* \brief Wird aufgerufen, wenn das Monster stirbt. Verteilt Erfahrungspunkte und droppt die Items.
		*/
		virtual void die();
		
		/**
		 * \fn bool takeDamage(Damage* damage)
		 * \brief Das Lebewesen nimmt Schaden in der in damage angegebenen Art und Hoehe. Verändert die dynamischen Attribute des Lebewesens.
		 * \param damage Schadensart und -hoehe
		 */
		virtual bool takeDamage(Damage* damage);
		
		/**
		* \fn virtual Action::ActionEquip getActionEquip()
		* \brief Gibt aus, ob die Aktion einhaendig oder zweihaendig ausgefuehrt wird
		*/
		virtual Action::ActionEquip getActionEquip()
		{
			// Monster per default immer einhaendig;
			return Action::ONE_HANDED;
		}
	
	protected:
		
		
		/**
		* \var Ai m_ai
		* \brief Alle Daten fuer die Ai
		*/
		Ai m_ai;
		
		/**
		* \var DropSlot m_drop_slots[4]
		* \brief Beschreibung der Items die das Monster droppen kann, wenn es stirbt
		*/
		DropSlot m_drop_slots[4];
	
	
	
};
#endif //MONSTER_H
