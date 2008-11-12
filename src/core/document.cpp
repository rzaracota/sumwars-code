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
	:  m_shortkey_map(), m_special_keys()
{
   
	strcpy(m_server_ip,"127.0.0.1");

	// Informationen zu Aktionen initialisieren
	Action::init();


	// Status der GUI setzen
	getGUIState()->m_left_mouse_hold= false;
	getGUIState()->m_right_mouse_hold= false;
	getGUIState()->m_shift_hold = false;
	getGUIState()->m_sheet= MAIN_MENU;
	getGUIState()->m_shown_windows = NO_WINDOWS;
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
	

}

void Document::startGame(bool server)
{
	m_server = server;

	// momentan: alle Spiele sind kooperativ
	World::createWorld(server, true);

	if (server)
	{

	}
	else
	{
		ClientNetwork* net = static_cast<ClientNetwork*>(World::getWorld()->getNetwork());
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
	string fname = "../save/";
	fname += m_save_file;
	/*
	if (m_gui_type == GL_GUI)
	{
		fname =  m_save_file;
	}
	*/
	DEBUG5("savegame is %s",fname.c_str());

	std::ifstream file(fname.c_str(),std::ios::in| std::ios::binary);
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

		World::getWorld()->handleSavegame(&cv2);


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


		delete[] data;
	}
	else
	{
		ERRORMSG("konnte Savegame nicht oeffnen: %s",fname.c_str());
		m_state =SHUTDOWN;
	}
	DEBUG5("done");
}

Document::~Document()
{

}
// Methods


void Document::sendCommand(ClientCommand* comm)
{
	World::getWorld()->handleCommand(comm);

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
	std::map<KeyCode, ShortkeyDestination>::iterator it;
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
	command.m_goal = Vector(0,0);
	command.m_id=0;
	command.m_number=0;

	// Paket an den Server senden
	sendCommand(&command);



}


void Document::onRightMouseButtonClick(float x, float y)
{
	ClientCommand command;

	// der lokale Spieler
	Player* pl = static_cast<Player*> (World::getWorld()->getLocalPlayer());
	if (pl==0)
		return;


	// herstellen der Koordinaten im Koordinatensystem des Spiels
	m_gui_state.m_clicked.m_x =x;
	m_gui_state.m_clicked.m_y =y;

	// Paket mit Daten fuellen
	command.m_button=RIGHT_MOUSE_BUTTON;
	command.m_goal = m_gui_state.m_clicked;
	command.m_action = pl->getRightAction();

	m_gui_state.m_left_mouse_hold=false;
	m_gui_state.m_right_mouse_hold_time=0;

	DEBUG5("angeklickte Koordinaten: %f %f",x,y);

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
	Player* pl = static_cast<Player*> (World::getWorld()->getLocalPlayer());
	if (pl==0)
		return;

	// herstellen der Koordinaten im Koordinatensystem des Spiels

	m_gui_state.m_clicked.m_x =x;
	m_gui_state.m_clicked.m_y =y;


	// Paket mit Daten fuellen
	command.m_button=LEFT_MOUSE_BUTTON;
	if (m_gui_state.m_shift_hold)
	{
		command.m_button=LEFT_SHIFT_MOUSE_BUTTON;
	}
	command.m_goal = m_gui_state.m_clicked;
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

	// Linksklick auf die eigene Figur unnoetig
	if (command.m_id == getLocalPlayer()->getId())
	{
		command.m_id=0;
	}

	if (command.m_id!=0)
	{
		DEBUG5("angeklicktes Objekt %i",command.m_id);
	}

	if (command.m_id ==0 && m_gui_state.m_cursor_item_id!=0)
	{
		// Item angeklickt
		command.m_action = Action::TAKE_ITEM;
		command.m_id = m_gui_state.m_cursor_item_id;

		DEBUG5("clicked at item %i",m_gui_state.m_cursor_item_id);
	}

	// Paket an den Server senden
	sendCommand(&command);


}

void Document::onStartScreenClicked()
{
	if (getGUIState()->m_shown_windows == NO_WINDOWS)
	{
		getGUIState()->m_shown_windows = SAVEGAME_LIST;
		m_modified =WINDOWS_MODIFIED;
	}
}

int Document::getObjectAt(float x,float y)
{
	// der lokale Spieler
	WorldObject* pl = World::getWorld()->getLocalPlayer();
	if (pl==0)
		return 0;

	// Region in der sich der lokale Spieler befindet
	Region* reg = pl->getRegion();
	if (reg ==0)
		return 0;

	WorldObject* obj = reg->getObjectAt(Vector(x,y));
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
		command.m_id = cnr;
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
		Player* pl = static_cast<Player*>( World::getWorld()->getLocalPlayer());
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

void Document::sendChatMessage(std::string msg)
{
	
	World::getWorld()->handleMessage(msg);
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
	Player* player = static_cast<Player*>(World::getWorld()->getLocalPlayer());
	if (player==0)
		return;


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

	DEBUG5("Setting Action %i",act);
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
	Player* player = static_cast<Player*>(World::getWorld()->getLocalPlayer());
	if (player==0)
		return;

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


	DEBUG5("Setting Action %i",act);
	m_gui_state.m_right_mouse_hold = false;

	ClientCommand command;
	command.m_button = BUTTON_SET_RIGHT_ACTION;
	command.m_action = act;
	sendCommand(&command);
}

std::string Document::getAbilityDescription(Action::ActionType ability)
{
	std::ostringstream out_stream;
	out_stream.str("");

	// der lokale Spieler
	Player* player = static_cast<Player*>(World::getWorld()->getLocalPlayer());

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

	std::map<KeyCode, ShortkeyDestination>::iterator it = m_shortkey_map.find(key);

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
			onButtonOpenChatClicked();
	
		}
		else if(dest == SHOW_CHATBOX_NO_TOGGLE)
		{
			// Chatfenster oeffnen wenn es gerade geschlossen ist
			if (!(getGUIState()->m_shown_windows & CHAT))
			{
				getGUIState()->m_shown_windows |= CHAT;
				
				// Geoeffnete Fenster haben sich geaendert
				m_modified |= WINDOWS_MODIFIED;

			}

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
	return true;
}

void Document::update(float time)
{
	// Welt eine Zeitscheibe weiter laufen lassen
	if (World::getWorld() != 0)
	{
		World::getWorld()->update(time);

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
			status = World::getWorld()->getNetwork()->getSlotStatus();
			if (m_server || World::getWorld()->getNetwork()->getSlotStatus() == NET_CONNECTED)
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
			World::deleteWorld();


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

	Player* player = static_cast<Player*>(World::getWorld()->getLocalPlayer());
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
			command.m_goal = m_gui_state.m_clicked;
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
			command.m_goal = m_gui_state.m_clicked;
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
		std::ofstream file(doc->m_save_file.c_str(),std::ios::out | std::ios::binary);
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



