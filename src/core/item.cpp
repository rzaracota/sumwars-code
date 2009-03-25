#include "item.h"
#include "itemfactory.h"

ItemBasicData::ItemBasicData()
{
	m_useup_effect=0;
	m_equip_effect=0;
	m_weapon_attr=0;
	m_level_req = 0;
	m_char_req = Item::REQ_NONE;

	for (int i=0;i<31;i++)
	{
		m_modchance[i] =0;
	}
	m_min_enchant =0;
	m_max_enchant =0;

}

ItemBasicData::~ItemBasicData()
{
	if (m_useup_effect!=0)
		delete m_useup_effect;

	if (m_equip_effect!=0)
		delete m_equip_effect;

	if (m_weapon_attr!=0)
		delete m_weapon_attr;

}


Item::Item()
{
	m_useup_effect=0;
	m_equip_effect=0;
	m_weapon_attr=0;
	m_level_req = 0;
	m_magic_power =0;
	m_rarity = NORMAL;
	m_char_req = REQ_NONE;
	m_size = SMALL;
}

Item::Item(ItemBasicData& data)
{
	m_type = data.m_type;
	m_subtype = data.m_subtype;
	m_size = data.m_size;
	m_price = data.m_price;

	if (data.m_useup_effect)
	{
		DEBUG5("copy useup effect");
		m_useup_effect = new CreatureDynAttrMod;
		memcpy(m_useup_effect,data.m_useup_effect,sizeof(CreatureDynAttrMod));
	}
	else
	{
		m_useup_effect =0;
	}

	if (data.m_equip_effect)
	{
		DEBUG5("copy equip effect");
		m_equip_effect = new CreatureBaseAttrMod;
		memcpy(m_equip_effect,data.m_equip_effect,sizeof(CreatureBaseAttrMod));
	}
	else
	{
		m_equip_effect =0;
	}

	if (data.m_weapon_attr)
	{
		DEBUG5("copy weapon attr");

		m_weapon_attr = new WeaponAttr;
		memcpy(m_weapon_attr,data.m_weapon_attr,sizeof(WeaponAttr));
		memcpy(&(m_weapon_attr->m_damage) , &(data.m_weapon_attr->m_damage),sizeof(Damage));

	}
	else
	{
		m_weapon_attr =0;
	}

	m_level_req = data.m_level_req;
	m_char_req = data.m_char_req;
}

Item::~Item()
{
	if (m_useup_effect) delete m_useup_effect;
	if (m_equip_effect) delete m_equip_effect;
	if (m_weapon_attr) delete  m_weapon_attr;
}


std::string Item::getName()
{
	std::ostringstream ret;

	if (m_size == GOLD)
	{
		ret << m_price << " ";
	}
    ret <<  (gettext((getString()).c_str()));


	return ret.str();
}


std::string Item::getString()
{
	return ItemFactory::getItemName(m_subtype);
}


void Item::toString(CharConv* cv)
{

	cv->toBuffer((char) m_type);
	cv->toBuffer(m_subtype,10);
	cv->toBuffer(m_id);
	cv->toBuffer<char>(m_rarity);
	if (m_type == GOLD_TYPE)
	{
		cv->toBuffer(m_price);
	}

}

void Item::fromString(CharConv* cv)
{
	// Daten werden extern eingelesen

	char tmp;
	cv->fromBuffer<char>(tmp);
	m_rarity = (Rarity) tmp;
	if (m_type == GOLD_TYPE)
	{
		cv->fromBuffer(m_price);
	}
}

void Item::toStringComplete(CharConv* cv)
{
	toString(cv);

	if (m_type == GOLD_TYPE)
		return;

	cv->toBuffer(m_price);
	cv->toBuffer(m_level_req);
	cv->toBuffer(m_char_req);

	cv->toBuffer(m_magic_power);

	char mask = NOINFO;

	if (m_useup_effect!=0)
		mask |= USEUP_INFO;

	if (m_equip_effect!=0)
		mask |= EQUIP_INFO;

	if (m_weapon_attr!=0)
		mask |= WEAPON_INFO;

	cv->toBuffer(mask);

	int i;
	if (m_useup_effect!=0)
	{
		DEBUG5("writing useup effect");
		cv->toBuffer(m_useup_effect->m_dhealth);
		for (i=0;i<NR_STATUS_MODS;i++)
		{
			cv->toBuffer(m_useup_effect->m_dstatus_mod_immune_time[i]);
		}
	}

	if (m_equip_effect!=0)
	{
		DEBUG5("writing equip effect");
		cv->toBuffer(m_equip_effect->m_darmor);
		cv->toBuffer(m_equip_effect->m_dblock);
		cv->toBuffer(m_equip_effect->m_dmax_health);
		cv->toBuffer(m_equip_effect->m_dattack);
		cv->toBuffer(m_equip_effect->m_dstrength);
		cv->toBuffer(m_equip_effect->m_ddexterity);
		cv->toBuffer(m_equip_effect->m_dmagic_power);
		cv->toBuffer(m_equip_effect->m_dwillpower);
		for (i=0;i<4;i++)
		{
			cv->toBuffer(m_equip_effect->m_dresistances[i]);
		}

		for (i=0;i<4;i++)
		{
			cv->toBuffer(m_equip_effect->m_dresistances_cap[i]);
		}

		cv->toBuffer(m_equip_effect->m_dwalk_speed);
		cv->toBuffer(m_equip_effect->m_dattack_speed);
		cv->toBuffer(m_equip_effect->m_xspecial_flags);

		for (i=0;i<6;i++)
		{
			cv->toBuffer(m_equip_effect->m_xabilities[i]);
		}
		cv->toBuffer(m_equip_effect->m_ximmunity);

	}

	if (m_weapon_attr!=0)
	{
		DEBUG5("writing weapon attr");
		cv->toBuffer<char>(m_weapon_attr->m_weapon_type);
		m_weapon_attr->m_damage.toString(cv);
		cv->toBuffer(m_weapon_attr->m_attack_range);
		cv->toBuffer(m_weapon_attr->m_two_handed);
		cv->toBuffer(m_weapon_attr->m_dattack_speed);

	}




}

void Item::fromStringComplete(CharConv* cv)
{
	fromString(cv);

	if (m_type == GOLD_TYPE)
		return;

	cv->fromBuffer<int>(m_price);
	cv->fromBuffer<char>(m_level_req);
	cv->fromBuffer<char>(m_char_req);
	cv->fromBuffer<float>(m_magic_power);

	char mask = NOINFO;
	cv->fromBuffer<char>(mask);

	if (mask & USEUP_INFO)
	{
		if (m_useup_effect !=0)
			delete m_useup_effect;

		m_useup_effect= new CreatureDynAttrMod();
	}

	if (mask & EQUIP_INFO)
	{
		if (m_equip_effect !=0)
			delete m_equip_effect;

		m_equip_effect=new CreatureBaseAttrMod();
	}

	if (mask & WEAPON_INFO)
	{
		if (m_weapon_attr !=0)
			delete m_weapon_attr;

		m_weapon_attr = new WeaponAttr();
	}


	int i;
	if (m_useup_effect!=0)
	{
		DEBUG5("loading useup effect");
		cv->fromBuffer<float>(m_useup_effect->m_dhealth );
		for (i=0;i<NR_STATUS_MODS;i++)
		{
			cv->fromBuffer<float>(m_useup_effect->m_dstatus_mod_immune_time[i]);
		}
	}

	if (m_equip_effect!=0)
	{
		DEBUG5("loading equip effect");
		cv->fromBuffer<short>(m_equip_effect->m_darmor);
		cv->fromBuffer<short>(m_equip_effect->m_dblock);
		cv->fromBuffer<float>(m_equip_effect->m_dmax_health);
		cv->fromBuffer<short>(m_equip_effect->m_dattack);
		cv->fromBuffer<short>(m_equip_effect->m_dstrength);
		cv->fromBuffer<short>(m_equip_effect->m_ddexterity);
		cv->fromBuffer<short>(m_equip_effect->m_dmagic_power);
		cv->fromBuffer<short>(m_equip_effect->m_dwillpower);
		for (i=0;i<4;i++)
		{
			cv->fromBuffer<short>(m_equip_effect->m_dresistances[i]);
		}

		for (i=0;i<4;i++)
		{
			cv->fromBuffer<short>(m_equip_effect->m_dresistances_cap[i]);
		}

		cv->fromBuffer<short>(m_equip_effect->m_dwalk_speed);
		cv->fromBuffer<short>(m_equip_effect->m_dattack_speed);
		cv->fromBuffer<int>(m_equip_effect->m_xspecial_flags );

		for (i=0;i<6;i++)
		{
			cv->fromBuffer<int>(m_equip_effect->m_xabilities[i]);
		}
		cv->fromBuffer<char>(m_equip_effect->m_ximmunity);

	}

	if (m_weapon_attr!=0)
	{
		DEBUG5("loading weapon attr");
		char tmp;
		cv->fromBuffer<char>(tmp);
		m_weapon_attr->m_weapon_type = (WeaponAttr::WeaponType) tmp;
		m_weapon_attr->m_damage.fromString(cv);
		cv->fromBuffer<float>(m_weapon_attr->m_attack_range);
		cv->fromBuffer<bool>(m_weapon_attr->m_two_handed);
		cv->fromBuffer<short>(m_weapon_attr->m_dattack_speed);

	}


}

std::string Item::getDescription()
{

	// String fuer die Beschreibung
	std::ostringstream out_stream;
	out_stream.str("");
	out_stream<<getName()<<"\n";
	int i;
	// Levelbeschraenkung
	out_stream <<gettext("Value")<<": "<<m_price;
	if (m_level_req>0)
	{
		out_stream<<"\n" << gettext("Required level")<<": "<<(int) m_level_req;
	}

	// TODO: Beschraenkung nach Charakterklasse

	// Effekt beim Verbrauchen
	if (m_useup_effect)
	{
		// HP Heilung
		if (m_useup_effect->m_dhealth>0)
		{
			out_stream <<"\n"<< gettext("Heals ")<<(int) m_useup_effect->m_dhealth<<gettext(" hitpoints");
		}

		// Heilen/ Immunisieren gegen Statusmods
		for (i=0;i<8;i++)
		{
			if (	m_useup_effect->m_dstatus_mod_immune_time[i]>0)
			{
				out_stream <<"\n"<< gettext("Heals ")<<Damage::getStatusModName((Damage::StatusMods) i);
				if (m_useup_effect->m_dstatus_mod_immune_time[i]>=1000)
				{
					out_stream <<", "<< gettext("immune for ")<< (int) (m_useup_effect->m_dstatus_mod_immune_time[i]*0.001f)<<"s";
				}
			}
		}

	}

	// Daten einer Waffe
	if (m_weapon_attr)
	{
		if (m_weapon_attr->m_two_handed)
		{
			out_stream <<"\n"<< gettext("Two-handed weapon");
		}
		// Reichweite / Angriffsgeschwindigkeit
		if (m_type == WEAPON)
		{
			out_stream << "\n" << gettext("Range")<<": "<<m_weapon_attr->m_attack_range;
		}

		//out_stream << "\n" << "Angriffe: "<<m_weapon_attr->m_attack_speed*0.001f<<"/s";

		// Schaden
		std::string dmgstring = m_weapon_attr->m_damage.getDamageString(Damage::ITEM);
		if (dmgstring != "")
		{
			if (m_type == WEAPON)
			{
				out_stream << "\n" << gettext("Damage")<<":";
			}
			out_stream<<"\n"<<dmgstring;
		}

	}

	// Effekte von Ausruestungsgegenstaenden
	if (m_equip_effect)
	{
		if (m_equip_effect->m_darmor>0)
		{
			out_stream<<"\n"<<gettext("Armor")<<": "<<m_equip_effect->m_darmor;
		}

		if (m_equip_effect->m_dblock>0)
		{
			out_stream<<"\n"<<gettext("Block")<<": "<<m_equip_effect->m_dblock;
		}

		if (m_equip_effect->m_dmax_health>0)
		{
			out_stream<<"\n"<<"+"<<(int) m_equip_effect->m_dmax_health<< " "<<gettext("max hitpoints");
		}

		if (m_equip_effect->m_dstrength>0)
		{
			out_stream<<"\n"<<"+"<<m_equip_effect->m_dstrength<< " "<<gettext("Strength");
		}

		if (m_equip_effect->m_ddexterity>0)
		{
			out_stream<<"\n"<<"+"<<m_equip_effect->m_ddexterity<< " "<<gettext("Dexterity");
		}

		if (m_equip_effect->m_dmagic_power>0)
		{
			out_stream<<"\n"<<"+"<<m_equip_effect->m_dmagic_power<< " "<<gettext("Magic Power");
		}

		if (m_equip_effect->m_dwillpower>0)
		{
			out_stream<<"\n"<<"+"<<m_equip_effect->m_dwillpower<< " "<<gettext("Willpower");
		}

		for (i=0;i<4;i++)
		{
			if (m_equip_effect->m_dresistances[i]>0)
			{
				out_stream<<"\n"<<"+"<<m_equip_effect->m_dresistances[i]<<" "<<Damage::getDamageResistanceName((Damage::DamageType) i);
			}
		}

		for (i=0;i<4;i++)
		{
			if (m_equip_effect->m_dresistances_cap[i]>0)
			{
				out_stream<<"\n"<<"+"<<m_equip_effect->m_dresistances_cap[i]<<gettext(" max. ")<<Damage::getDamageResistanceName((Damage::DamageType) i);
			}
		}


		// TODO: Angriffsgeschwindigkeit
		// TODO: special Flags
		// TODO: Faehigkeiten
		// TODO: Immunitaeten

	}


	return out_stream.str();
}

void Item::calcPrice()
{
	if (m_useup_effect !=0)
	{
		// Trank, beim generieren erzeugten wert nutzen
		return;
	}

	// Nutzwert des Gegenstandes
	float value =0;
	// Faktor fuer den wert
	float mult =1;

	int i;
	if (m_weapon_attr !=0)
	{
		// Wert des Schadens;
		float dvalue=0;
		//dmult *= std::min(2.0,sqrt(m_weapon_attr->m_attack_range));
		// Schaden der Waffe
		Damage & dmg = m_weapon_attr->m_damage;
		// phys Schaden
		dvalue += 0.5*(dmg.m_min_damage[0] + dmg.m_max_damage[0])*dmg.m_multiplier[0] * (1+dmg.m_crit_perc*2);
		// elementar Schaden
		for (i=1;i<4;i++)
		{
			dvalue += 0.5*(dmg.m_min_damage[i] + dmg.m_max_damage[i])*dmg.m_multiplier[i];

		}
		dvalue *= (2000 + m_weapon_attr->m_dattack_speed)/1000.0;

		dvalue += dmg.m_attack*0.1;
		dvalue += dmg.m_power*0.1;

		for (i=0;i<8;i++)
		{
			dvalue += dmg.m_status_mod_power[i]*0.2;
		}

		// TODO: Flags einberechnen
		value += dvalue * std::min(1.5,sqrt(m_weapon_attr->m_attack_range));

	}

	if (m_equip_effect!=0)
	{
		// Modifikation beim anlegen
		CreatureBaseAttrMod* cbasm = m_equip_effect;

		value += cbasm->m_darmor*0.5;
		value += cbasm->m_dblock*0.5;
		value += cbasm->m_dattack*0.1;
		value += cbasm->m_dmax_health*0.2;
		value += cbasm->m_dstrength;
		value += cbasm->m_ddexterity;
		value += cbasm->m_dwillpower;
		value += cbasm->m_dmagic_power;
		mult *= (1+cbasm->m_dattack_speed/2000.0);

		for (i=0;i<4;i++)
		{
			value += cbasm->m_dresistances[i]*0.6;
		}

		for (i=0;i<4;i++)
		{
			value += cbasm->m_dresistances_cap[i];
		}


		// TODO: Special Flags einberechnen

		// TODO: Immunitaeten einberechnen

		// TODO: Skills mit einberechnen

	}

	value = (2+0.5*(1+0.1*value)*value)* mult;

	value = std::min (value,100000.0f);
	m_price = (int) value;
}




