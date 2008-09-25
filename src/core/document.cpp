/*
	Summoning Wars - document.cpp
	Copyright (C) 2007  Hans Wulf, Michael Kempf

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fstream"
#include "document.h"


// Constructors/Destructors
// Initialisiert Document zu Testzwecken
Document::Document()
	: m_special_keys() , m_shortkey_map()
{

	DEBUG5("reading ip");
	// IP aus Konfigurationsdatei einlesen
	{
		ifstream file("config");
		DEBUG5("file opened");
		if ( file.is_open() )
		{
			file >> m_server_ip;
			DEBUG5("Server-IP: %s", m_server_ip);
		}
		else
		{
			ERRORMSG("config nicht gefunden");
		}
	}
	
	m_world =0;

	bool network_error = false;

	// Informationen zu Aktionen initialisieren
	Action::init();


	// Status der GUI setzen
	getGUIState()->m_left_mouse_hold= false;
	getGUIState()->m_right_mouse_hold= false;
	getGUIState()->m_shift_hold = false;
	getGUIState()->m_sheet= MAIN_MENU;
	getGUIState()->m_shown_windows = SAVEGAME_LIST;
	//getGUIState()->m_pressed_key = OIS::KC_UNASSIGNED;
	getGUIState()->m_pressed_key =0;
	getGUIState()->m_cursor_object ="";
	getGUIState()->m_cursor_object_id =0;
	getGUIState()->m_cursor_item_id =0;
	getGUIState()->m_right_mouse_hold_time=0;
	getGUIState()->m_left_mouse_hold_time=0;

	// Pointer/Inhalte mit 0 initialisieren
	m_gui_state.m_chat_window_content = "";
	m_data_locks=0;
	m_savegame =0;


	// aktuell eingestellte Aktionen setzen

	// Status setzen
	m_state = INACTIVE;

	m_modified =GUISHEET_MODIFIED | WINDOWS_MODIFIED;

	// Shortkeys einstellen
	m_shortkey_map.clear();
	/*
	installShortkey(OIS::KC_I,SHOW_INVENTORY);
	installShortkey(OIS::KC_C,SHOW_CHARINFO);
	installShortkey(OIS::KC_T,SHOW_SKILLTREE);
	installShortkey(OIS::KC_P,SHOW_PARTYMENU);
	installShortkey(OIS::KC_M,SHOW_CHATBOX);
	installShortkey(OIS::KC_W,SWAP_EQUIP);
	installShortkey(OIS::KC_1,USE_POTION);
	installShortkey(OIS::KC_2,(ShortkeyDestination) (USE_POTION+1));
	installShortkey(OIS::KC_3,(ShortkeyDestination) (USE_POTION+2));
	installShortkey(OIS::KC_4,(ShortkeyDestination) (USE_POTION+3));
	installShortkey(OIS::KC_5,(ShortkeyDestination) (USE_POTION+4));
	installShortkey(OIS::KC_6,(ShortkeyDestination) (USE_POTION+5));
	installShortkey(OIS::KC_7,(ShortkeyDestination) (USE_POTION+6));
	installShortkey(OIS::KC_8,(ShortkeyDestination) (USE_POTION+7));
	installShortkey(OIS::KC_9,(ShortkeyDestination) (USE_POTION+8));
	installShortkey(OIS::KC_0,(ShortkeyDestination) (USE_POTION+9));

	// besondere Tasten festlegen
	m_special_keys.clear();
	m_special_keys.insert(OIS::KC_ESCAPE);
	m_special_keys.insert(OIS::KC_LSHIFT);
	m_special_keys.insert(OIS::KC_LCONTROL);
	*/

}

void Document::startGame(bool server)
{
	m_server = server;
	
	
	
	m_world = new World(server);
	m_world->init();
	
	if (server)
	{
		
	}
	else
	{
		ClientNetwork* net = static_cast<ClientNetwork*>(m_world->getNetwork());
		net->serverConnect(m_server_ip,REQ_PORT);
		
	}
	m_state = LOAD_SAVEGAME;
}


void Document::loadSavegame()
{
	// Savegame einlesen
	DEBUG("lese savegame");
	char head[8];
	int i;
	char* bp = head;
	string fname = "../../save/";
	fname += m_save_file;
	/*
	if (m_gui_type == GL_GUI)
	{
		fname =  m_save_file;
	}
	*/
	DEBUG5("savegame is %s",fname.c_str());
	
	ifstream file(fname.c_str(),ios::in| ios::binary);
	if (file.is_open())
	{
		for (i=0;i<8;i++)
		{
			file.get(head[i]);
		}
		CharConv cv((unsigned char*) head,18);
		char binsave;
		cv.fromBuffer<char>(binsave);
		short version;
		cv.fromBuffer<short>(version);
		int len;
		cv.fromBuffer<int>(len);
		char c;
		cv.fromBuffer<char>(c);
		DEBUG5("lese %i bytes",len);
		char* data= new char[len];
		memcpy(data,head,8);
		for (i=8;i<len;i++)
		{
			file.get(data[i]);
		}
		
		CharConv cv2((unsigned char*) data,len);
		
		m_world->handleSavegame(&cv2);


		// aktuelle Aktion setzen
		// TODO: aus dem Savegame einlesen
		
		/*int n=0;
		if (c==WorldObje)
			n=4;
		if (c==WorldObject::TypeInfo::SUBTYPE_MAGE)
			n=6;
		if (c==WorldObject::TypeInfo::SUBTYPE_ARCHER)
			n=5;
		if (c==WorldObject::TypeInfo::SUBTYPE_PRIEST)
			n=7;
*/
/*
		char* tmp = data +101;
		n = cv->fromBuffer<short>(tmp);
		DEBUG("left action %i",n);
		setLeftAction((Action::ActionType) n);
		m_left_action =(Action::ActionType) n;

		n = cv->fromBuffer<short>(tmp);
		DEBUG("right action %i",n);
		setRightAction((Action::ActionType) n);
		m_right_action=(Action::ActionType) n;
*/
		/*
		setRightAction(Action::ATTACK) ;
		m_left_action = Action::ATTACK;
		setLeftAction(Action::ATTACK);
		m_right_action = Action::ATTACK;
*/

		m_state =RUNNING;
		m_gui_state.m_shown_windows = NO_WINDOWS;
		m_gui_state.m_sheet = GAME_SCREEN;
		m_modified = WINDOWS_MODIFIED | GUISHEET_MODIFIED;
		m_timer.start();


		delete data;
	}
	else
	{
		ERRORMSG("konnte Savegame nicht oeffnen");
	}
	DEBUG5("done");
}

Document::~Document()
{

}
// Methods

/*
ClientWorldObject* Document::getWorldObject(int id)
{
	 map<int,ClientWorldObject*>::iterator iter;

    // Objekt im Binaerbaum suchen
     iter = m_objects_bintree->find(id);

    // Testen ob ein Objekt gefunden wurde
    if (iter == m_objects_bintree->end())
     {
         // keins gefunden, NULL ausgeben
        return 0;
     }
   else
    {
        // Zeiger auf Objekt ausgeben
        return iter->second;
    }
}
*/

void Document::sendCommand(ClientCommand* comm)
{
	m_world->handleCommand(comm);

}

void Document::lock()
{
	while (m_data_locks>0)
		//DEBUG("waiting in lock");
		;

	m_data_locks++;
}

void Document::unlock()
{
	m_data_locks--;

	if (m_data_locks>0)
		ERRORMSG("kritischer Abschnitt verletzt");
}

KeyCode Document::getMappedKey(ShortkeyDestination sd)
{
	map<KeyCode, ShortkeyDestination>::iterator it;
	for (it=m_shortkey_map.begin(); it!= m_shortkey_map.end();++it)
	{
		if (it->second == sd)
		{
			return it->first;
		}
	}

	return 0;
}

void Document::installShortkey(KeyCode key,ShortkeyDestination dest)
{
	// Taste auf die das Ereignis bisher gemappt war
	KeyCode oldkey = getMappedKey(dest);
	// entfernen
	if (oldkey != 0)
	{
		m_shortkey_map.erase(oldkey);
	}

	// Mapping anlegen
	m_shortkey_map[key]=dest;

}

void Document::onButtonSendMessageClicked ( )
{
	DEBUG5("onbuttonmessageclicked");

	// Header anlegen
	/*ClientHeader header;
	short slen = strlen(m_gui_state.m_chat_sendline_content.c_str());
	short len = slen+ sizeof(ClientHeader)+sizeof(short)+1;

	// Puffer fuer das Datenpaket
	char* data = new char[len];
	memset(data,0,len);
	char* bp = data;
	header.m_content = PTYPE_C2S_DATA;	// Daten vom Client zum server
	header.m_chatmessage = true;			// Chatmessage enthalten

	// Header in den Puffer kopieren
	bp = header.toString(bp);

	// Chatnachricht in den Puffer kopieren
	cv->toBuffer(bp,slen+1);
	memcpy(bp,m_gui_state.m_chat_sendline_content.c_str(),slen);

	bp += slen;
	*bp =0;
	bp ++;
	len = bp - data;


	// Nachricht an den Server senden

	NetStatus ret;
	ret = m_network_info.m_network->pushSlotMessage(data,len);

	// Rueckgabewert pruefen
	if (ret != NET_OK)
	{
		DEBUG("Netzwerkfehler beim Daten senden: %i", ret);
		getNetworkInfo()->m_network_error = true;
	}
	delete data;
*/
}


void Document::onButtonSaveExitClicked ( )
{
	
	if (m_state!=SHUTDOWN_REQUEST)
	{
		setState(SHUTDOWN_REQUEST);
	}
	else
	{
		setState(SHUTDOWN);
	}
	
	m_shutdown_timer =0;

	ClientCommand command;

	// Paket mit Daten fuellen
	// Button ist Speichern+Beenden, alle anderen Daten gleich 0
	command.m_button=BUTTON_SAVE_QUIT;
	command.m_coordinate_x=0;
	command.m_coordinate_y=0;
	command.m_id=0;
	command.m_number=0;

	// Paket an den Server senden
	sendCommand(&command);



}


void Document::onRightMouseButtonClick(float x, float y)
{
	ClientCommand command;

	// der lokale Spieler
	Player* pl = static_cast<Player*> (m_world->getLocalPlayer());
	if (pl==0)
		return;
	

	// herstellen der Koordinaten im Koordinatensystem des Spiels
	m_gui_state.m_clicked_x =x;
	m_gui_state.m_clicked_y =y;

	// Paket mit Daten fuellen
	command.m_button=RIGHT_MOUSE_BUTTON;
	command.m_coordinate_x=x;
	command.m_coordinate_y=y;
	command.m_action = pl->getRightAction();

	m_gui_state.m_left_mouse_hold=false;
	m_gui_state.m_right_mouse_hold_time=0;

	DEBUG("angeklickte Koordinaten: %f %f",x,y);

	m_gui_state.m_clicked_object_id=0;
	command.m_id=0;

	int id = getObjectAt(x,y);
	
	m_gui_state.m_clicked_object_id = id;
	command.m_id =id;
	
	command.m_number=0;

	if (command.m_id!=0)
		DEBUG("angeklicktes Objekt %i",command.m_id);


	// Paket an den Server senden
	sendCommand(&command);

}

void Document::onLeftMouseButtonClick(float x, float y)
{

	ClientCommand command;

	// der lokale Spieler
	Player* pl = static_cast<Player*> (m_world->getLocalPlayer());
	if (pl==0)
		return;

	// herstellen der Koordinaten im Koordinatensystem des Spiels

	m_gui_state. m_clicked_x =x;
	m_gui_state. m_clicked_y =y;

	// Paket mit Daten fuellen
	command.m_button=LEFT_MOUSE_BUTTON;
	if (m_gui_state.m_shift_hold)
	{
		command.m_button=LEFT_SHIFT_MOUSE_BUTTON;
	}
	command.m_coordinate_x=x;
	command.m_coordinate_y=y;
	command.m_action = pl->getLeftAction();

	m_gui_state.m_right_mouse_hold=false;
	m_gui_state.m_left_mouse_hold_time=0;


	DEBUG4("angeklickte Koordinaten: %f %f",x,y);

	//TODO: suchen welches objekt angeklickt wurde
	
	m_gui_state.m_clicked_object_id=0;
	command.m_id=0;
	
	
	int id = getObjectAt(x,y);
	if (getGUIState()->m_cursor_object_id != 0)
	{
		id = getGUIState()->m_cursor_object_id;
	}
	
	m_gui_state.m_clicked_object_id = id;
	command.m_id =id;
	command.m_number=0;

	if (command.m_id!=0)
	{
		DEBUG("angeklicktes Objekt %i",command.m_id);
	}

	if (command.m_id ==0 && m_gui_state.m_cursor_item_id!=0)
	{
		// Item angeklickt
		command.m_action = Action::TAKE_ITEM;
		command.m_id = m_gui_state.m_cursor_item_id;
	}

	// Paket an den Server senden
	sendCommand(&command);


}

int Document::getObjectAt(float x,float y)
{
	// der lokale Spieler
	ServerWObject* pl = m_world->getLocalPlayer();
	if (pl==0)
		return 0;
	
	// Region in der sich der lokale Spieler befindet
	Region* reg = pl->getRegion();
	if (reg ==0)
		return 0;
	
	ServerWObject* obj = reg->getSWObjectAt(x,y);
	if (obj != 0)
		return obj->getId();
	
	return 0;

}


void Document::onButtonPartyApply(int pnr)
{
	ClientCommand command;
	command.m_button = BUTTON_PARTY_APPLY;
	command.m_id = pnr;
	sendCommand(&command);
}

void Document::onButtonPartyAccept(int cnr)
{
	ClientCommand command;
	command.m_button = BUTTON_PARTY_ACCEPT;
	
	// Party in der der Spieler Mitglied ist
	Party* party = getParty();
	if (party ==0)
		return;
	
	if (party->getNrCandidates() > cnr)
	{
		command.m_id = party->getCandidates()[cnr];
		sendCommand(&command);
	}
}

void Document::onItemLeftClick(short pos)
{
	ClientCommand command;
	command.m_button = BUTTON_ITEM_LEFT;
	command.m_id = pos;
	sendCommand(&command);

}

void Document::onItemRightClick(short pos)
{
	ClientCommand command;
	command.m_button = BUTTON_ITEM_RIGHT;
	command.m_id = pos;
	sendCommand(&command);

}

/*
void Document::requestItemDetailedInfo( short pos)
{
	ClientCommand command;
	command.m_button = REQUEST_DETAILED_ITEM;
	command.m_id = pos;
	sendCommand(&command);
}

void Document::requestAbilityDamage(Action::ActionType abl)
{
	ClientCommand command;
	command.m_button = REQUEST_ABILITY_DAMAGE;
	command.m_id = abl;
	sendCommand(&command);
}
*/

void Document::increaseAttribute(CreatureBaseAttr::Attribute attr)
{
	ClientCommand command;
	command.m_button = BUTTON_INCREASE_ATTRIBUTE;
	command.m_id = attr;
	DEBUG("increasing attribute %i",attr);
	sendCommand(&command);
}

void Document::learnAbility(Action::ActionType act)
{
	ClientCommand command;
	command.m_button = BUTTON_LEARN_ABILITY;
	command.m_id = act;
	sendCommand(&command);
}

void Document::dropCursorItem()
{
	ClientCommand command;
	command.m_button = DROP_ITEM;
	sendCommand(&command);
}

void Document::emitDebugSignal(int i)
{
	ClientCommand command;
	command.m_button = DEBUG_SIGNAL;
	command.m_id = i;
	sendCommand(&command);
}

void Document::onButtonInventoryClicked()
{
	// Inventar oeffnen wenn es gerade geschlossen ist und schliessen, wenn es geoeffnet ist
	if (getGUIState()->m_shown_windows & INVENTORY)
	{

		getGUIState()->m_shown_windows &= ~INVENTORY;
		
		// der lokale Spieler
		Player* pl = static_cast<Player*>( m_world->getLocalPlayer());
		if (pl==0)
			return;

		// Item das aktuell in der Hand ist fallen lassen
		if (pl->getEquipement()->getItem(Equipement::CURSOR_ITEM)!=0)
		{
			dropCursorItem();
		}
	}
	else
	{
		// wenn Inventar geoeffnet wird, dann Skilltree schliessen
		getGUIState()->m_shown_windows &= ~SKILLTREE;
		m_gui_state.m_pressed_key = 0;

		getGUIState()->m_shown_windows |= INVENTORY;



	}

	// Geoeffnete Fenster haben sich geaendert
	m_modified |= WINDOWS_MODIFIED;

}

void Document::onButtonCharInfoClicked()
{
	// Charakterinfo oeffnen wenn es gerade geschlossen ist und schliessen, wenn es geoeffnet ist
	if (getGUIState()->m_shown_windows & CHARINFO)
	{
		getGUIState()->m_shown_windows &= ~CHARINFO;
	}
	else
	{
		getGUIState()->m_shown_windows |= CHARINFO;
	}

	// Geoeffnete Fenster haben sich geaendert
	m_modified |= WINDOWS_MODIFIED;
}

void Document::onButtonSkilltreeClicked()
{
	// Skilltree oeffnen wenn er gerade geschlossen ist und schliessen, wenn er geoeffnet ist
	if (getGUIState()->m_shown_windows & SKILLTREE)
	{
		getGUIState()->m_shown_windows &= ~SKILLTREE;
	}
	else
	{
		// wenn Skilltree geoeffnet wird, dann Inventar schliessen
		getGUIState()->m_shown_windows &= ~INVENTORY;

		getGUIState()->m_shown_windows |= SKILLTREE;
	}

	m_gui_state.m_pressed_key = 0;

	// Geoeffnete Fenster haben sich geaendert
	m_modified |= WINDOWS_MODIFIED;
}

void Document::onButtonOpenChatClicked()
{
	// Cchatfenster oeffnen wenn es gerade geschlossen ist und schliessen, wenn es geoeffnet ist
	if (getGUIState()->m_shown_windows & CHAT)
	{
		getGUIState()->m_shown_windows &= ~CHAT;
	}
	else
	{
		getGUIState()->m_shown_windows |= CHAT;
	}

	// Geoeffnete Fenster haben sich geaendert
	m_modified |= WINDOWS_MODIFIED;
}

void Document::onSwapEquip()
{
	ClientCommand command;
	command.m_button = BUTTON_SWAP_EQUIP;
	sendCommand(&command);
}

void Document::setLeftAction(Action::ActionType act)
{
	if (m_gui_state.m_pressed_key != 0)
	{
		// Im Skilltree wird Kurztaste ausgewaehlt
		installShortkey(m_gui_state.m_pressed_key,(ShortkeyDestination) (USE_SKILL_LEFT+act));
		return;
	}
	
	
	// wenn kein Spieler gesetzt ist, dann keine Faehigkeit setzen
	// der lokale Spieler
	Player* player = static_cast<Player*>(m_world->getLocalPlayer());
	if (player==0)
		return;
	

	int acti = (int) act;
	Action::ActionInfo* aci = Action::getActionInfo(act);

	// Testen ob die Faehigkeit zur Verfuegung steht
	if (!player->checkAbility(act))
	{
		// Faehigkeit steht nicht zur Verfuegung, abbrechen
		return;
	}

	// passive Faehigkeiten koennen nicht auf Maustasten gelegt werden
	if (aci->m_distance == Action::PASSIVE)
		return;

	// Faehigkeiten die nicht auf einfachen Angriffen beruhen koennen nicht auf die linke Maustaste gelegt werden
	if (aci->m_base_action== Action::NOACTION)
		return;

	DEBUG("Setting Action %i",act);
	m_gui_state.m_left_mouse_hold = false;


	ClientCommand command;
	command.m_button = BUTTON_SET_LEFT_ACTION;
	command.m_action = act;
	sendCommand(&command);
}

void Document::setRightAction(Action::ActionType act)
{
	if (m_gui_state.m_pressed_key != 0)
	{
		// Im Skilltree wird Kurztaste ausgewaehlt
		installShortkey(m_gui_state.m_pressed_key,(ShortkeyDestination) (USE_SKILL_RIGHT+act));
		DEBUG("setting short key %i for action %i",m_gui_state.m_pressed_key,USE_SKILL_RIGHT+act);
		return;
	}

	// wenn kein Spieler gesetzt ist, dann keine Faehigkeit setzen
	// der lokale Spieler
	Player* player = static_cast<Player*>(m_world->getLocalPlayer());
	if (player==0)
		return;
	
	int acti = (int) act;
	Action::ActionInfo* aci = Action::getActionInfo(act);

	// Testen ob die Faehigkeit zur Verfuegung steht
	if (!player->checkAbility(act))
	{
		// Faehigkeit steht nicht zur Verfuegung, abbrechen
		return;
	}

	// passive Faehigkeiten koennen nicht auf Maustasten gelegt werden
	if (aci->m_distance == Action::PASSIVE)
		return;


	DEBUG("Setting Action %i",act);
	m_gui_state.m_right_mouse_hold = false;

	ClientCommand command;
	command.m_button = BUTTON_SET_RIGHT_ACTION;
	command.m_action = act;
	sendCommand(&command);
}

std::string Document::getAbilityDescription(Action::ActionType ability)
{
	ostringstream out_stream;
	out_stream.str("");

	// der lokale Spieler
	Player* player = static_cast<Player*>(m_world->getLocalPlayer());
	
	if (player !=0 )
	{
		// Struktur mit Informationen zur Aktion
		Action::ActionInfo* aci = Action::getActionInfo(ability);

		// Name der Faehigkeit
		out_stream << Action::getName(ability);
		// out_stream << Action::getName(m_ability_pos);

		// Beschreibung
		//out_stream << "\n" << "Beschreibung: bla blubb";
		out_stream << "\n" << gettext("doc_description") << Action::getDescription(ability);

		// Gibt an, ob der Spieler die Faehigkeit besitzt
		bool avlb = true;
		short ac = ability;
		if (!player->checkAbility(ability))
		{
			// Spieler besitzt Faehigkeit nicht
			avlb = false;

			// Level ab dem Faehigkeit verfuegbar ist
			out_stream <<"\n"<< gettext("doc_minimum_level") << aci->m_req_level;
		}

		// Timerinfo
		if (aci->m_timer_nr!=0)
		{
			out_stream <<"\n"<< gettext("doc_timer") << aci->m_timer_nr;
			out_stream <<"\n"<< gettext("doc_timer_runtime") << (int) aci->m_timer*0.001 << " s";
		}

		// Schaden
		Damage dmg;
		player->calcDamage(ability,dmg);
		std::string dmgstring = dmg.getDamageString(Damage::ABILITY);
		if (dmgstring !="")
		{
			out_stream << "\n" << dmgstring;
		}

	}

	return out_stream.str();
}

bool Document::onKeyPress(KeyCode key)
{

	if (m_gui_state.m_shown_windows & SKILLTREE)
	{
		// Skilltree wird angezeigt
		// Tastendruecke werden anders interpretiert

		// Wirkung die bisher auf der Taste liegt
		ShortkeyDestination dest = NO_KEY;
		if (m_shortkey_map.find(key) != m_shortkey_map.end())
		{
			dest = m_shortkey_map.find(key)->second;
		}

		if (m_special_keys.find(key)==m_special_keys.end() && (dest == NO_KEY || dest >= USE_SKILL_LEFT))
		{
			m_gui_state.m_pressed_key = key;
			return true;
		}

	}

	map<KeyCode, ShortkeyDestination>::iterator it = m_shortkey_map.find(key);

	if (it == m_shortkey_map.end())
	{
		// Kein Aktion fuer die Taste vorhanden
		return false;
	}
	else
	{
		// Aktion welche mit der Taste verbunden ist
		ShortkeyDestination dest = it->second;
		if (dest == SHOW_INVENTORY)
		{
			onButtonInventoryClicked();
		}
		else if (dest == SHOW_CHARINFO)
		{
			onButtonCharInfoClicked();
		}
		else if (dest == SHOW_SKILLTREE)
		{
			onButtonSkilltreeClicked();
		}
		else if (dest == SWAP_EQUIP)
		{
			onSwapEquip();
		}
		else if (dest >=USE_POTION && dest < USE_POTION+10)
		{
			onItemRightClick(Equipement::SMALL_ITEMS + (dest-USE_POTION));
		}
		else if (dest>=USE_SKILL_LEFT && dest <USE_SKILL_RIGHT)
		{
			setLeftAction((Action::ActionType) (dest-USE_SKILL_LEFT));
		}
		else if (dest>=USE_SKILL_RIGHT && dest <USE_SKILL_RIGHT+200)
		{
			setRightAction((Action::ActionType) (dest-USE_SKILL_RIGHT));
		}
		else if(dest == SHOW_PARTYMENU)
		{
			emitDebugSignal(0);
		}
		else if(dest == SHOW_CHATBOX)
		{
			emitDebugSignal(3);
		}
		else if (dest == CLOSE_ALL)
		{
			if (m_gui_state.m_shown_windows == NO_WINDOWS)
			{
				onButtonSaveExitClicked();
			}
			else
			{
				m_gui_state.m_shown_windows =  NO_WINDOWS;
			// Geoeffnete Fenster haben sich geaendert
				m_modified |= WINDOWS_MODIFIED;
			}
		}
		else
		{
			return false;
		}

		return true;

	}
}

bool  Document::onKeyRelease(KeyCode key)
{
	if (m_gui_state.m_pressed_key != 0)
	{
		m_gui_state.m_pressed_key = 0;
	}
}

void Document::update(float time)
{
	// Welt eine Zeitscheibe weiter laufen lassen
	if (m_world != 0)
	{
		m_world->update(time);
	}
	
	DEBUG5("modified is %i",m_modified);
	DEBUG5("State is %i",m_state);
	NetStatus status;
	switch (m_state)
	{
		case INACTIVE:
			break;

		case START_GAME:
			startGame(m_server);
			break;

		case LOAD_SAVEGAME:
			status = m_world->getNetwork()->getSlotStatus();
			if (m_server || m_world->getNetwork()->getSlotStatus() == NET_CONNECTED)
			{
				loadSavegame();
			}
			if (status == NET_REJECTED || status == NET_SLOTS_FULL || status == NET_TIMEOUT)
			{
				// Verbindung abgelehnt
				m_state = SHUTDOWN;	
			}
			

		case RUNNING:
			updateContent(time);
			break;

		case SHUTDOWN_REQUEST:
			updateContent(time);
			m_shutdown_timer += time;
			if (m_shutdown_timer>400)
			{
				setState(SHUTDOWN);
			}
			break;

		case SHUTDOWN_WRITE_SAVEGAME:
			// Savegame schreiben
			pthread_t thread;
			void* ret;
			pthread_create(&thread,0,&Document::writeSaveFile,this);
			pthread_join(thread, &ret);

			// Spielwelt abschalten
			delete m_world;
			m_world =0;
			
			
			m_state = SHUTDOWN;
			break;

		default:
			break;
	}
	DEBUG5("done");
}

void Document::updateContent(float time)
{
	DEBUG5("update content");

	Player* player = static_cast<Player*>(m_world->getLocalPlayer());
	if (player==0)
	{
		DEBUG5("no local player");
		return;
	}
	
	if (m_gui_state.m_left_mouse_hold)
	{
		DEBUG5("linke Maustaste festgehalten");
		m_gui_state.m_left_mouse_hold_time += time;
		if (m_gui_state.m_left_mouse_hold_time>= 200)
		{
			ClientCommand command;

			command.m_button=LEFT_MOUSE_BUTTON;
			if (m_gui_state.m_shift_hold)
			{
				command.m_button=LEFT_SHIFT_MOUSE_BUTTON;
			}
			command.m_coordinate_x=m_gui_state.m_clicked_x;
			command.m_coordinate_y=m_gui_state.m_clicked_y;
			command.m_id = m_gui_state.m_clicked_object_id;
			command.m_action = player->getLeftAction();
			command.m_number=0;

			m_gui_state.m_left_mouse_hold_time=0;
			sendCommand(&command);

		}
	}

	if (m_gui_state.m_right_mouse_hold_time>= 200)
	{
		DEBUG("rechte Maustaste festgehalten");
		m_gui_state.m_right_mouse_hold_time += time;

		if (m_gui_state.m_right_mouse_hold_time>= 200)
		{
			ClientCommand command;

			command.m_button=RIGHT_MOUSE_BUTTON;
			command.m_coordinate_x=m_gui_state.m_clicked_x;
			command.m_coordinate_y=m_gui_state.m_clicked_y;
			command.m_id = m_gui_state.m_clicked_object_id;
			command.m_action = player->getRightAction();
			command.m_number=0;

			m_gui_state.m_right_mouse_hold_time=0;
			sendCommand(&command);

		}

	}
	
	
	
	/*
	ServerHeader headerp;

	Packet* data=0;
	CharConv* cv;
	static int nodata=0;
	if (m_network_info.m_network->numberSlotMessages()==0)
		nodata++;

	if (nodata>=3)
		DEBUG5("no data for %i turns",nodata);

	int datcount=0;
	// Daten vom Server holen
	m_network_info.m_network->update();

	if ( m_network_info.m_network->getSlotStatus()!=NET_CONNECTED )
	{
		DEBUG5("disconnected");

	}

	while( m_network_info.m_network->getSlotStatus()==NET_CONNECTED &&  m_network_info.m_network->numberSlotMessages()>0 )
	{
		//DEBUG("number of messages %i",m_network_info.m_network->numberSlotMessages());
		datcount++;
		nodata=0;
		m_network_info.m_network->popSlotMessage(data);
		DEBUG5("got information , length %i\n",data->length);

		cv = new CharConv(data);


		headerp.fromString(cv);

		if (headerp.m_content == PTYPE_S2C_DATA)
		{
			
			DEBUG5("got data pkg");
			handleDataPkg(cv,&headerp);
			
		}
		else if (headerp.m_content == PTYPE_S2C_SAVEGAME)
		{
			DEBUG5("got savegame");
			handleSavegame(cv);

		}
		else if (headerp.m_content == PTYPE_S2C_ITEM)
		{
			handleDetailedItem(cv);

		}
		else if (headerp.m_content == PTYPE_S2C_DAMAGE)
		{
			handleAbilityDamage(cv,&headerp);
		}
		else if (headerp.m_content == PTYPE_S2C_REGION)
		{
			handleRegionData(cv);
		}
		else
		{
			WARNING("Got corrupted package type %i",headerp.m_content);
			// TODO: do something to fix this....

		}
		
		delete cv;
		
		m_network_info.m_network->deallocatePacket(data);

	}


	if (datcount>3)
	{
		DEBUG5("got %i data packages",datcount);
	}

	DEBUG5("update finished");
	*/
}


/*
void Document::handleDataPkg(CharConv* cv, ServerHeader* headerp)
{
	//DEBUG("read bits: %i bytes",cv->getBitStream()->GetReadOffset());
	/*
	map<int, ClientWObject*>::iterator iter;
	ClientWObject* cwo=0;
	map<int,Projectile*>::iterator it;
	Projectile* pr=0;

	if (!headerp->m_chatmessage)
	{
		
			//usleep( 25000 );
			//INFO( "get_ob %i",  m_temp_objects_bintree->size() );
		DEBUG5( "Got a begin of update" );

			// Daten sperren
		lock();
		if (m_main_player)
		{
			DEBUG5("mainplayer %i, pointer %p",m_main_player->getId(),m_main_player);
		}

			//Datenstrukturen leeren
			// Anmerkung: main_player wird mit geloescht
		for (iter = m_objects_bintree->begin(); iter!=m_objects_bintree->end(); iter++)
		{
			cwo = iter->second;
			DEBUG5("Speicher freigeben: obj %i at %p", cwo->getId(),cwo);
			if (cwo!=0)
				delete cwo;
		}

		m_objects_bintree->clear();

		for (it=m_projectiles->begin();it!=m_projectiles->end();++it)
		{
			pr = (it->second);
			if (pr)
				delete pr;
		}
		m_projectiles->clear();
		DEBUG5("data cleared");
		// Neue Daten einlesen
		
		m_main_player = new ClientMPlayer(0);
			// Daten aus dem Startpaket auslesen
		m_main_player->fromString(cv);
		cwo = m_main_player;
		DEBUG5("inserting object %i, pointer %p",cwo->getId(),cwo);
		m_objects_bintree->insert(make_pair(cwo->getId(),cwo));

		// TODO: QuestInformationen auslesen

		// Partyinformationen
		if (m_party !=0)
		{
			delete m_party;
			m_party=0;
		}
		m_party = new Party();
		m_party->fromString(cv);


		

		

		int i;
		DEBUG5("number of objects %i",headerp->m_objects);
		for (i=0;i<headerp->m_objects;i++)
		{
			cwo = new ClientWObject(0);
			cwo->fromString(cv);


			DEBUG5("inserting object %i, pointer %p",cwo->getId(),cwo);

			// WorldObject in die temporaeren Datenstrukturen einfuegen
			m_objects_bintree->insert(make_pair(cwo->getId(),cwo));

				//DEBUG4( "got (%f,%f) [%i] type %i subtype %i",cwo->m_coordinate_x,cwo->m_coordinate_y,cwo->m_id,cwo->m_type,cwo->m_subtype );

		}

		// Geschosse einlesen
		DEBUG5("Number of projectiles %i",headerp->m_projectiles);
		for (i=0;i<headerp->m_projectiles;i++)
		{
			pr = new Projectile();
			pr->fromString(cv);
			DEBUG5("got Projectile %i %f %f",pr->getType(),pr->getGeometry()->m_coordinate_x,pr->getGeometry()->m_coordinate_y);


			m_projectiles->insert(make_pair(pr->getId(),pr));
		}

		
		// Items einlesen
		DEBUG5("Number of Items %i",headerp->m_items);
		m_main_player->m_equipement->fromString(cv,headerp->m_items);

		
		
		
		
		// Items am Boden einlesen
		DropItem di;
		map<int,DropItem>::iterator it3;
		for (it3 = m_drop_items->begin(); it3 != m_drop_items->end(); ++it3)
		{
			delete it3->second.m_item;
		}
		m_drop_items->clear();
		DEBUG5("Number of dropped Items %i",headerp->m_drop_items);
		for (i=0;i<headerp->m_drop_items;i++)
		{
			di.m_item = new Item;
			di.fromString(cv);
			m_drop_items->insert(make_pair(di.m_x*10000+di.m_y,di));
		}
		
		// Daten entsperren
		unlock();
		
	}
	

	DEBUG5("objects modified");
	m_modified |= OBJECTS_MODIFIED;


}


void Document::handleSavegame(CharConv* cv)
{
	DEBUG5("received Save file");
	if (m_savegame!=0)
		delete m_savegame;

	int len = cv->getBitStream()->GetNumberOfUnreadBits()/8;
	DEBUG5("savegame length %i",len);
	m_savegame = new char[len];
	cv->getBitStream()->Read(m_savegame, len);
	
	//hexwrite(datap,len);


	if (m_state ==SHUTDOWN_REQUEST)
	{
		// Savegame erhalten, Shutdown
		m_state = SHUTDOWN_WRITE_SAVEGAME;
	}

}

void Document::handleDetailedItem(CharConv* cv)
{
	if (m_detailed_item !=0)
	{
		delete m_detailed_item;
	}

	// Daten des Items einlesen
	m_detailed_item = new ServerItem();
	m_detailed_item->fromStringComplete(cv, m_detailed_item_pos);
	DEBUG4("received item at %i",m_detailed_item_pos);

	// Itemdaten wurden geaendert
	m_modified |= ITEM_MODIFIED;
	DEBUG5("m_modified %i",m_modified);

}

void Document::handleAbilityDamage(CharConv* cv, ServerHeader* headerp)
{
	// Feld fuer detailliertes Item wird missbraucht fuer den Aktionstyp...
	m_ability_pos = (Action::ActionType) headerp->m_detailed_item;

	// Schaden einlesen
	m_ability_damage.init();
	m_ability_damage.fromString(cv);

	// geaenderte Daten anzeigen
	m_modified |= ABILITY_MODIFIED;
}

void Document::handleRegionData(CharConv* cv)
{
	DEBUG("got region data");

	// Groesse der Region einlesen
	cv->fromBuffer<short>(m_region_data.m_dimx);
	cv->fromBuffer<short>(m_region_data.m_dimy);

	// Anzahl der Objekte
	short nr;
	 cv->fromBuffer<short>(nr);

	DEBUG("receiving %i objects",nr);

	map<int,WorldObject*>::iterator it;

	// alte Objekte loeschen
	for (it = m_region_data.m_static_objects->begin(); it!=m_region_data.m_static_objects->end();++it)
	{
		delete (it->second);
	}
	m_region_data.m_static_objects->clear();

	if (m_region_data.m_tiles!=0)
	{
		delete m_region_data.m_static_objects;
	}
	WorldObject* wo;

	// neue Objekte einlesen
	int i,j;
	for (i=0;i<nr;i++)
	{
		wo = new WorldObject(0);
		wo->fromString(cv);

		// einfuegen in Binaerbaum
		m_region_data.m_static_objects->insert(make_pair(wo->getId(),wo));

		DEBUG("got WorldObject %s %p",wo->getNameId().c_str(),wo);
	}

	// Matrix fuer die Tiles anlegen
	if (m_region_data.m_tiles!=0)
		delete (m_region_data.m_tiles);


	m_region_data.m_tiles = new Matrix2d<char>(2*m_region_data.m_dimx,2*m_region_data.m_dimy);

	// Tiles einlesen
	for (i=0;i<m_region_data.m_dimx*2;i++)
	{
		for (j=0;j<m_region_data.m_dimy*2;j++)
		{
			cv->fromBuffer<char>(*(m_region_data.m_tiles->ind(i,j)));
			//printf("%i ",*(m_region_data.m_tiles->ind(i,j)));
		}
		//printf("\n");
	}


	// Daten zur Region wurden geaendert
	m_modified |= REGION_MODIFIED;

}
*/

void* Document::writeSaveFile(void* doc_ptr)
{
	Document* doc = (Document*) doc_ptr;
	if (doc->m_savegame!=0)
	{

		// Savegame schreiben
		char* bp = doc->m_savegame;


		// Laenge der Datei
		int len = *((int*) (bp+3));

		int i;
		bp = doc->m_savegame;
		// Daten byteweise in Datei schreiben
		ofstream file(doc->m_save_file.c_str(),ios::out | ios::binary);
		DEBUG("length of file %i",len);
		//hexwrite(bp,len);
		if ( file.is_open() )
		{
			DEBUG("writing save file");
			for (i=0;i<len;i++)
			{
				file<< bp[i];
			}

		}
	}

	return 0;
}



