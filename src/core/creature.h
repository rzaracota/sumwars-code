/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf, Chris Drechsler, Daniel Erler

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
 * \file creature.h
 * \defgroup Creature \ Lebewesen
 * \ingroup worldobject
 * \brief Klasse Creature
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Creature
 */

#ifndef CREATURE_H
#define CREATURE_H
#include <string>
#include <list>
#include <algorithm>
#include "world.h"
#include "worldobject.h"
#include "action.h"
#include "command.h"
#include "damage.h"
#include "itemlist.h"
#include "creaturestruct.h"
#include "itemfactory.h"
#include "dialogue.h"
// debugging
#include <iostream>



/**
 * \class Creature
 * 
  * \brief Lebewesen
 * 
 * Alle Lebewesen in der Welt sind Objekte der Klasse Creature
 * 
 */

// forward declaration
/**
 * \class Creature
 * \brief Klasse fuer Lebewesen
 */
class Creature : public WorldObject 
 {
//Public stuff
public:
	//Fields
	//Constructors
	/**
	 * \fn Creature(int id)
	 * \brief Konstruktor
	 * \param id Id des Objekts
	 *
	 * Legt ein neues Creature Objekt an.
	 */
	Creature(int id);
	
	/**
	 * \fn ~Creature();
	 * \brief Destruktor
	 *
	 * Gibt den Allokierten Speicher wieder frei
	 */
	~Creature();
	//Accessor Methods
	
	
	
	
	/**
	 * \fn Action* getAction()
	 * \brief Gibt einen Zeiger auf die aktuelle Aktion zur&uuml;ck
	 * \return Action*, Zeiger auf die aktuelle Aktion
	 */
	Action* getAction()
	{
		return &m_action;
	}
	
	/**
	 * \fn Command* getCommand()
	 * \brief Gibt einen Zeiger auf das aktuelle Kommando zur&uuml;ck
	 * \return Command*, Zeiger auf das aktuelle Kommando
	 */
	Command* getCommand()
	{
		return &m_command;
	}
	
	/**
	 * \fn Command* getNextCommand()
	 * \brief Gibt einen Zeiger auf das naechsteKommando zur&uuml;ck
	 * \return Command*, Zeiger auf das naechste Kommando
	 */
	Command* getNextCommand()
	{
		return &m_next_command;
	}
	
	
	
	/**
	 * \fn Damage* getDamage()
	 * \brief Gibt einen Pointer auf die Damagestruktur zurueck, die den Schaden der aktuellen Aktion des Lebewesens bezeichnet.
	 * \return Zeiger auf Damage
	 */
	Damage* getDamage()
	{
		return &m_damage;
	}
	
	/**
	 * \fn CreatureBaseAttr* getBaseAttrMod()
	 * \brief Gibt einen Zeiger auf die Struktur mit den modifizierten Basisattributen des Lebewesens zurueck
	 */
	CreatureBaseAttr* getBaseAttrMod()
	{
		return &m_base_attr_mod;
		
	}
	
	/**
	 * \fn CreatureBaseAttr* getBaseAttr()
	 * \brief Gibt einen Zeiger auf die Struktur mit den Basisattributen des Lebewesens zurueck
	 */
	CreatureBaseAttr* getBaseAttr()
	{
		return &m_base_attr;
		
	}
	
	/**
	 * \fn CreatureDynAttr* getDynAttr()
	 * \brief Gibt einen Zeiger auf die Struktur mit den dynamischen Attributen des Lebewesens zurueck
	 */
	CreatureDynAttr* getDynAttr()
	{
		return &m_dyn_attr;
	}
	
	
	/**
	 * \fn float getTimerPercent(int timer)
	 * \brief Gibt den Prozentsatz des gewaehlten Timers aus
	 */
	float getTimerPercent(int timer);
	
	
	//Operations
	/**
	 * \fn virtual bool init ()
	 * \brief Initialisiert das Objekt. Die Funktion ist virtuell und wird durch die abgeleiteten Klassen &uuml;berschrieben.
	 * \return bool, der angibt ob die initialisierung erfolgreich war
	 */
	virtual bool init ();
	
	/**
	 * \fn virtual bool destroy ()
	 * \brief Zerstoert das Objekt. Die Funktion ist virtuell und wird durch die abgeleiteten Klassen &uuml;berschrieben.
	 * \return bool, der angibt ob die Zerstoerung erfolgreich war
	 */
	virtual bool destroy();
	
	
	/**
	 * \fn virtual bool  update (float time);
	 * \brief Updatefunktion des Lebewesens, welche für die angebene Zeit ausgeführt wird.
	 * \param time Zeit die für update zur Verfügung steht
	 * \return bool, Erfolgreiches Update?
	 *
	 * Die Updatefunktion des Lebewesens wird in regelm&auml;ssigen Abst&auml;nden von der World aufgerufen und ben&ouml;tigt die Angabe, wie viel Zeit f&uuml;r das Update zur Verf&uuml;gung gestellt wird. Der R&uuml;ckgabewert gibt dann an ob die Funktion erfolgreich ausgef&uuml;hrt wurde. Da die Funktion hier virtuell definiert wird, wird sie erst in den abgeleiteten Klassen implementiert.
	 */
	virtual bool  update (float time);
	
	
	/**
	 * \fn reactOnUse(int id)
	 * \brief Beschreibt die Reaktion des Lebewesens, wenn ein anderes Lebewesen auf es die Aktion benutzen ausführt.  
	 * \param id ist die ID des Lebewesens, welche die Aktion benutzen ausgeführt hat
	 */
	bool reactOnUse(int id);
	
	
	/**
	 * \fn void gainExperience (float exp)
	 * \brief Das Lebewesen gewinnt exp Erfahrungspunkte. Wenn dabei die noetigen Erfahrungspunkte fuer ein Levelup erreicht werden wird gainLevel() aufgerufen.
	 * \param exp gewonnene Erfahrungspunkte
	 */
	virtual void gainExperience (float exp);
	
	
	/**
	 * \fn bool takeDamage(Damage* damage)
	 * \brief Das Lebewesen nimmt Schaden in der in damage angegebenen Art und Hoehe. Verändert die dynamischen Attribute des Lebewesens.
	 * \param damage Schadensart und -hoehe
	 */
	virtual bool takeDamage(Damage* damage);
	
	/**
	 * \fn void applyBaseAttrMod(CreatureBaseAttrMod* mod,bool add = true)
	 * \brief Wendet fuer die Zeitdauer time die temporaere Modifikation mod auf das Lebewesen an. Falls time gleich 0 so wird die Veraenderung permanent angewendet
	 * \param mod Modifikation
	 * \param add Gibt an, ob die die Modifikation zur Liste der wirksamen Modifikationen hinzugefuegt werden soll
	 */
	void applyBaseAttrMod(CreatureBaseAttrMod* mod, bool add = true);
	
	/**
	 * \fn applyDynAttrMod(CreatureDynAttrMod* mod);
	 * \brief Wendet eine Modifikation der Dynamischen Attribute an
	 * \param mod Modifikation
	 */
	void applyDynAttrMod(CreatureDynAttrMod* mod);
	
	/**
	 * \fn void removeBaseAttrMod(CreatureBaseAttrMod* mod)
	 * \brief Entfernt alle Veraenderungen, die durch die Modifikation entstanden sind
	 * \param mod Modifikation
	 * return gibt an, ob die Basisattribute komplett neu berechnet werden muessen
	 */
	bool removeBaseAttrMod(CreatureBaseAttrMod* mod);
	

	/**
	 * \fn void getPathDirection(Vector pos,short region, float base_size, short layer,  Vector& dir)
	 * \brief Gibt die Richtung zurueck, in die man vom Startpunkt aus gehen muss um zu dem Lebewesen zu gelangen
	 * \param pos Startpunkt
	 * \param base_size Durchmesser der Standflaeche des Weg suchenden Lebewesens
	 * \param layer Ebene in der auf Kollisionen getestet werden soll
	 * \param region Region in der gesucht werden soll
	 * \param dir Rueckgabevektor
	 */
	void getPathDirection(Vector pos,short region, float base_size, short layer,  Vector& dir);
	
		
	/**
	 * \fn void calcWalkDir(Vector goal,WorldObject* goalobj)
	 * \brief Sucht den Weg zu einem Punkt und setzt die Bewegungsgeschwindigkeit entspechend
	 * \param goal Zielpunkt
	 * \param goalobj Zeiger auf das Zielobject
	 */
	void calcWalkDir(Vector goal,WorldObject* goalobj);
	
	/**
	 * \fn void clearCommand(bool success)
	 * \brief Bricht das aktuelle Kommando ab
	 * \param success Gibt an, ob die Ausfuehrung erfolgreich war
	 */
	void clearCommand(bool success);
	
	/**
	 * \fn virtual void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
		 */
	virtual void toString(CharConv* cv);
	
	/**
	 * \fn virtual void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv);

	/**
	 * \fn bool checkAbility(Action::ActionType at)
	 * \brief Prueft ob die Kreatur ueber die angegebene Faehigkeit verfuegt
	 * \param at Faehigkeit
	 * \return true, wenn die Faehigkeit verfuegbar ist
	 */
	bool checkAbility(Action::ActionType at);

	
		/**
	 * \fn bool checkAbilityLearnable(Action::ActionType at)
	 * \brief Testet ob eine Faehigkeit erlernbar ist
	 * \param at Faehigkeit
	 * \return true, wenn die Faehigkeit erlernbar ist
		 */
	bool checkAbilityLearnable(Action::ActionType at);
	
	/**
	 * \fn virtual void calcDamage(Action::ActionType act,Damage& dmg)
	 * \brief Berechnet den Schaden fuer die aktuell ausgefuehrte Aktion
	 * \param act Aktion, die ausgefuehrt wird
	 * \param dmg Schaden der Aktion
	 */
	virtual void calcDamage(Action::ActionType act,Damage& dmg);
	
	/**
	 * \fn virtual Action::ActionEquip getActionEquip()
	 * \brief Gibt aus, ob die Aktion einhaendig oder zweihaendig ausgefuehrt wird
	 */
	virtual Action::ActionEquip getActionEquip()
	{
		return Action::ONE_HANDED;
	}
	
	/**
	 * \fn virtual int getValue(std::string valname)
	 * \brief Schiebt den gewuenschten Attributwert eines Objektes auf den Lua Stack
	 * \param valname Name des Wertes
	 */
	virtual int getValue(std::string valname);
	
	/**
	 * \fn virtual bool setValue(std::string valname)
	 * \brief Setzt den gewuenschten Attributwert eines Objektes
	 * \param valname Name des Wertes
	 */
	virtual bool setValue(std::string valname);
	
	/**
	 * \fn void insertScriptCommand(Command &cmd, float time=50000)
	 * \brief Fuegt fuer die Kreatur ein neues Kommando per Script hinzu
	 * \param cmd Kommando
	 * \param time Zeit die bleibt um das Kommando zu beenden
	 */
	void insertScriptCommand(Command &cmd, float time=50000);

	/**
	 * \fn bool hasScriptCommand()
	 * \brief Gibt true zurueck, wenn das aktuelle Kommando per Script gesetzt wurde
	 */
	bool hasScriptCommand();
	
	/**
	 * \fn int sellItem(Item* &item)
	 * \brief verkauft das angegebene Item
	 * \param position Positio des verkauften Items
	 * \param item Zeiger auf das verkaufte Item (bei erfolgreichem Verkauf)
	 * \param gold Gold des kaufenden Spielers
	 */
	void sellItem(short position, Item* &item, int& gold);
		
		/**
	 * \fn int buyItem(short position, Item* &item, int& gold)
	 * \brief Kauft das Item 
	 * \param item gekauftes Item (wird bei erfolgreichem Kauf auf 0 gesetzt
	 * \param gold Gold des verkaufenden Spielers
		 */
	void buyItem(Item* &item, int& gold);

	
	/**
	 * \fn void speakText(CreatureSpeakText& text)
	 * \brief Laesst die Kreatur einen bestimmten Text sprechen
	 * \param text Text der angezeigt wird
	 **/
	void speakText(CreatureSpeakText& text);
	
	
	/**
	 * \fn  CreatureSpeakText& getSpeakText()
	 * \brief Gibt den aktuell gesprochenen Text aus
	 */
	CreatureSpeakText& getSpeakText()
	{
		return m_speak_text;
	}
	
	/**
	 * \fn  CreatureTradeInfo& getTradeInfo()
	 * \brief Gibt Informationen zum Handel aus
	 */
	CreatureTradeInfo& getTradeInfo()
	{
		return m_trade_info;
	}
	
	void clearSpeakText();
	
	/**
	 * \fn Dialogue* getDialogue()
	 * \brief Gibt den aktuellen Dialog der Kreatur aus
	 */
	Dialogue* getDialogue()
	{
		return getRegion()->getDialogue(m_dialogue_id);
	}
	
	/**
	 * \fn int getDialogueId()
	 * \brief Gibt die ID des Dialogs aus
	 */
	int getDialogueId()
	{
		return m_dialogue_id;
	}
	
	/**
	 * \fn void setDialogue(int id)
	 * \brief Setzt den aktuellen Dialog
	 * \param id ID des Dialogs
	 */
	void setDialogue(int id);

	
	/**
	 * \fn std::string getRefName()
	 * \brief Gibt den Referenzname aus, mit dem bei Gespraechen auf die Person verwiesen wird
	 */
	virtual std::string getRefName()
	{
		return m_refname;
	}
	
	/**
	 * \fn void setRefName(std::string name)
	 * \brief Setzt den Referenzname
	 * \param name Referenzname
	 */
	void setRefName(std::string name)
	{
		m_refname = name;
	}
	
	/**
	 * fn Equipement* getEquipement()
	 * \brief Gibt einen Zeiger auf die Ausruestung des Spielers zurück
	 * \return Zeiger auf die Ausruestung
	 */
	Equipement* getEquipement()
	{
		return m_equipement;
	}
	
	/**
	 * \fn void setEquipement(Equipement* equ)
	 * \brief Setzt das Inventar
	 * \param equ Inventar
	 */
	void setEquipement(Equipement* equ)
	{
		m_equipement = equ;
	}
	
	/**
	 * \fn Creature* getTradePartner()
	 * \brief Gibt den Handelspartner aus
	 */
	Creature* getTradePartner();
	
	/**
	 * \fn void setTradePartner(int id)
	 * \brief Setzt den Handelspartner
	 * \param id ID des Handelspartners
	 */
	void setTradePartner(int id)
	{
		m_trade_info.m_trade_partner = id;
		m_event_mask |= NetEvent::DATA_TRADE_INFO;
	}
	
	
protected:
	
	/**
 	* \fn virtual void gainLevel()
	* \brief Wird aufgerufen, wenn das Lebewesen ein Levelup erhält. Passt Erfahrung und alle Basisattribute an.
	*/
	virtual void gainLevel();
	
	/**
	 * \fn virtual void calcBaseAttrMod()
	 * \brief Die modifizierten Basisattribute werden neu berechnet. Verwendet die Basisattribute, verwendete Items, sowie temporaere Modifikationen.
	 */
	virtual void calcBaseAttrMod();
	
	
	
	/**
	 * \fn virtual void recalcDamage();
	 * \brief Berechnet den Schaden, der in internen Damage Objekten angezeigt wird neu
	 */
	virtual void recalcDamage();
	
	/**
	 * \fn virtual void calcBaseDamage(Action::ActionType act,Damage& dmg)
	 * \param act Aktion, die ausgefuehrt wird
	 * \param dmg Schaden der Aktion
	 * \brief Berechnet den Basisschaden einer Aktion
	 */
	virtual void calcBaseDamage(Action::ActionType act,Damage& dmg);
	
	/**
	 * \fn virtual void calcAbilityDamage(Action::ActionType act,Damage& dmg)
	 * \param act Aktion, die ausgefuehrt wird
	 * \param dmg Schaden der Aktion
	 * \brief Berechnet den Schaden mit Modifikationen durch Faehigkeiten aus dem Basisschaden
	 */
	virtual void calcAbilityDamage(Action::ActionType act,Damage& dmg);
	
	
	
	/**
	 * \fn virtual void calcAction()
	 * \brief Berechnet die naechste Aktion
	 */
	virtual void calcAction();
	
	/**
	 * \fn virtual void updateCommand()
	 * \brief aktualisiert das aktuelle Kommando
	 */
	virtual void updateCommand();
	
	/**
	 * \fn void calcStatusModCommand()
	 * \brief Berechnet Kommando, die aufgrund eines Statusmods ausgefuehrt wird
	 */
	void calcStatusModCommand();
		
	/**
	 * \fn virtual void initAction()
	 * \brief initialisiert die aktuell gesetzte Aktion
	 */
	virtual void initAction();
	
	/**
	 * \fn virtual float performAction(float &time)
	 * \brief Fuehrt die aktuell gesetzte Aktion fuer die Zeit time aus. Falls der Abschluss der Aktion weniger Zeit in Anspruch nimmt, wird die restliche Zeit zurueckgegeben.
	 * \param time zur Verfuegung stehende Zeit in ms
	 * \return Restzeit
	 */
	virtual void performAction(float &time);
	
	/**
	 * \fn virtual void performActionCritPart(Vector goal, WorldObject* goalobj)
	 * \brief Fuehrt den entscheidenden Part einer Action (Schaden austeilen, Status veraendern usw aus
	 * \param goal Zielpunkt
	 * \param goalobj Zeiger auf der Zielobjekt, ist NULL wenn kein Zielobjekt existiert
	 */
	virtual void performActionCritPart(Vector goal, WorldObject* goalobj);
	
	/**
	 * \fn virtual void die()
	 * \brief Wird aufgerufen, wenn die aktuellen Lebenspunkte unter 0 sinken. Die genaue Wirkung der Funktion ist abhängig vom exakten Typ des Lebewesens (Monster etc)
	*/
	virtual void die();
	
	/**
	 * \fn void handleCollision(Shape* s)
	 * \brief Fuehrt die Behandlung einer Kollision mit einem Objekt mit der angegebenen Grundfläche aus
	 * \param s Form der Grundfläche des kollidierenden Objektes
	 */
	void handleCollision(Shape* s);
	
	/**
	 * \fn void collisionDetection(float time)
	 * \brief Untersucht ob in der naechsten Zeit Kollisionen zu erwarten sind
	 * \param time Zeitraum um den vorrausgerechnet wird
	 */
	void collisionDetection(float time);
	
	
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
	
			

//Private stuff
private:
	//Fields
	
	
	
	
	/**
	 * \var m_action;
	 * \brief Aktion die gerade ausgef&uuml;hrt wird
	 */
	Action m_action;
	
	/**
	 * \var m_command;
	 * \brief Kommando das gerade abgearbeitet wird
	 */
	Command m_command;
	
	/**
	 * \var m_next_command;
	 * \brief Kommando das als naechstes abgearbeitet wird
	 */
	Command m_next_command;
	
	/**
	 * \var std::list<std::pair<Command,float> > m_script_commands
	 * \brief Liste von Kommandos, die durch Scripte vorgeschrieben wurden
	 **/
	std::list<std::pair<Command,float> > m_script_commands;
	
	/**
	 * \var CreatureSpeakText m_speak_text
	 * \brief Folge von Texten, die die Kreature sprechen soll
	 */
	CreatureSpeakText m_speak_text;
	
	/**
	 * \var CreatureTradeInfo m_trade_info
	 * \brief Struktur mit aktuellem Handelsvorgang
	 */
	CreatureTradeInfo m_trade_info;
	
	/**
	 * \var int m_speak_id
	 * \brief ID der Person, zu der die Kreatur gerade spricht
	 */
	int m_speak_id;
	
	/**
	 * \var std::string m_refname
	 * \brief Referenzname, mit dem die Kreatur bei Dialogen angesprochen wird
	 */
	std::string m_refname;
	
	/**
	 * \var float m_script_command_timer
	 * \brief Zeigt an, wie lange das aktuelle Kommando noch dauern darf
	 */
	float m_script_command_timer;
	
	
	/**
	 * \var m_trade_id
	 * \brief Id des aktuellen Handelsvorgangs
	 */
	int m_trade_id;
	
	/**
	 * \var m_base_attr
	 * \brief Basisattribute des Lebewesens
	 */
	CreatureBaseAttr m_base_attr;
	
	/**
	 * \var m_base_attr_mod
	 * \brief Basisattribute des Lebewesens nach Modifikationen durch Items und temporaere Effekte
	 */
	CreatureBaseAttr m_base_attr_mod;
	
	
	/**
	 * \var m_dyn_attr
	 * \brief alle sich schnell aendernden Attribute und temporaeren Modifikationen des Lebewesens
	 */
	CreatureDynAttr m_dyn_attr;
	
	/**
	 * \var m_damage
	 * \brief Objekt welches angibt, welcher Schaden mit der aktuellen Aktion angerichtet wird
	 */
	Damage m_damage;
	
	protected:
	
	
	/**
	 * \var int m_dialogue_id
	 * \brief ID des Dialogs, an dem sich die Kreatur beteiligt
	 */
	int m_dialogue_id;
		
	/**
	 * \var m_timer1
	 * \brief Gibt die Zeit in ms an, fuer welche Aktionen die Timer1 benoetigen nicht benutzt werden koennen
	 */
	float m_timer1;
	
	/**
	 * \var m_timer2
	 * \brief Gibt die Zeit in ms an, fuer welche Aktionen die Timer2 benoetigen nicht benutzt werden koennen
	 */
	float m_timer2;
	
	/**
	 * \var float m_timer2_max
	 * \brief Zeit von Timer2 in ms beim starten des Timers
	 */
	float m_timer2_max;
	
	/**
	 * \var float m_timer1_max
	 * \brief Zeit von Timer1 in ms beim starten des Timers
	 */
	float m_timer1_max;
	
	/**
	 * \var Action::ActionType m_base_action
	 * \brief Grundaktion
	 */
	Action::ActionType m_base_action;
	
	/**
	 * \var m_equipement
	 * \brief Enthaelt die Ausruestung des Spielers
	 */
	Equipement* m_equipement;
	
	/**
	 * \var PathfindInfo* m_small_path_info
	 * \brief Wegsucheinformationen fuer kleine Kreaturen (Information ueber den Weg zu diesem Objekt)
	 */
	PathfindInfo* m_small_path_info;
	
	/**
	 * \var PathfindInfo* m_small_flying_path_info
	 * \brief Wegsucheinformationen fuer kleine fliegende Kreaturen (Information ueber den Weg zu diesem Objekt)
	 */
	PathfindInfo* m_small_flying_path_info;
	
	
	/**
	 * \var PathfindInfo* m_medium_path_info
	 * \brief Wegsucheinformationen fuer mittelgrosse Kreaturen (Information ueber den Weg zu diesem Objekt)
	 */
	PathfindInfo* m_medium_path_info;
	
	
	/**
	 * \var PathfindInfo* m_big_path_info
	 * \brief Wegsucheinformationen fuer grosse Kreaturen (Information ueber den Weg zu diesem Objekt)
	 */
	PathfindInfo* m_big_path_info;
	
	
	/**
	 * \var PathfindInfo* m_small_path_info
	 * \brief eigene Wegsucheinformationen, wenn das Zielobjekt keine Kreatur ist
	 */
	PathfindInfo* m_path_info;
	

	//Constructors
	//Accessor Methods
	//Operations
};

#endif //CREATURE_H

