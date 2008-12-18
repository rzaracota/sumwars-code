#include "itemfactory.h"

#define USE_ITEMLOADER

#include <iostream>

std::map<Item::Subtype, DropChance> ItemFactory::m_item_probabilities;

std::map<Item::Subtype, Item::Type> ItemFactory::m_item_types;

std::map<Item::Subtype,ItemBasicData*> ItemFactory::m_item_data;


ItemFactory::ItemFactory()
{
}


Item* ItemFactory::createItem(Item::Type type, Item::Subtype subtype, int id, float magic_power)
{
	if (id ==0)
	{
		id = World::getWorld()->getValidId();
	}
	
	DEBUG5("creating item %i / %s",type, subtype.c_str());
	
	if (type == Item::GOLD_TYPE)
	{
		return createGold(0,id);
	}
	
	std::map<Item::Subtype,ItemBasicData*>::iterator it;
	it = m_item_data.find(subtype);
	Item * item;
	float min_enchant =30;
	float max_enchant =100;

	if (it != m_item_data.end())
	{
		ItemBasicData* idata = it->second;
		item = new Item(*idata);
		item->m_id = id;

		createMagicMods(item,idata->m_modchance,magic_power, idata->m_min_enchant, idata->m_max_enchant);

		// Preis ausrechnen
		item->calcPrice();

		return item;
	}



	item = new Item;
	item->m_id = id;

	item->m_type = type;
	item->m_subtype = subtype;

	CreatureBaseAttrMod * m_creature_base_attr_mod=0;
	CreatureDynAttrMod * m_creature_dyn_attr_mod=0;
	WeaponAttr * m_weaponattr=0;




	float modchance[31];
	memset(modchance,0,31*sizeof(float));

	if (type == Item::ARMOR ||
		type == Item::HELMET ||
		type == Item::GLOVES ||
		type == Item::SHIELD ||
		type == Item::RING ||
		type == Item::AMULET)
	{
		m_creature_base_attr_mod = new CreatureBaseAttrMod();
		item->m_useup_effect = 0;
		item->m_equip_effect = m_creature_base_attr_mod;
		item->m_weapon_attr = 0;
		if (type == Item::RING)
		{
			item->m_weapon_attr = new WeaponAttr();
		}

		if (type == Item::ARMOR)
		{
			modchance[HEALTH_MOD]=0.05;
			modchance[ARMOR_MOD]=0.6;
			modchance[DEXTERITY_MOD]=0.05;
			modchance[WILLPOWER_MOD]=0.05;
			modchance[RESIST_PHYS_MOD]=0.03;
			modchance[RESIST_FIRE_MOD]=0.06;
			modchance[RESIST_ICE_MOD]=0.06;
			modchance[RESIST_AIR_MOD]=0.06;
			modchance[RESIST_ALL_MOD]=0.04;
			if (subtype =="leath_arm")
			{
					min_enchant = 30;
					max_enchant= 100;
					item->m_price = 60;
					item->m_size = Item::BIG;
					item->m_equip_effect->m_darmor = 20;
			}
			else if (subtype =="tiled_arm")
			{
					min_enchant = 50;
					max_enchant= 300;
					item->m_price = 100;
					item->m_size = Item::BIG;
					item->m_equip_effect->m_darmor = 40;
			}
			else if (subtype =="heavy_arm")
			{
					min_enchant = 100;
					max_enchant= 600;
					item->m_price = 300;
					item->m_size = Item::BIG;
					item->m_equip_effect->m_darmor = 100;
			}
		}

		else if (type == Item::HELMET)
		{
			modchance[HEALTH_MOD]=0.1;
			modchance[ARMOR_MOD]=0.5;
			modchance[MAGIC_POWER_MOD]=0.1;
			modchance[WILLPOWER_MOD]=0.1;
			modchance[RESIST_PHYS_MOD]=0.02;
			modchance[RESIST_FIRE_MOD]=0.05;
			modchance[RESIST_ICE_MOD]=0.05;
			modchance[RESIST_AIR_MOD]=0.05;
			modchance[RESIST_ALL_MOD]=0.04;
			if (subtype =="steel_hlm")
			{
				min_enchant = 50;
					max_enchant= 150;
					item->m_price = 60;
					item->m_size = Item::MEDIUM;
					item->m_equip_effect->m_darmor = 10;
			}
			else if (subtype =="divine_hlm")
			{
					min_enchant = 100;
					max_enchant= 400;
					item->m_price = 400;
					item->m_size = Item::MEDIUM;
					item->m_equip_effect->m_darmor = 30;
					item->m_equip_effect->m_dwillpower = 20;
			}
		}
		else if (type == Item::GLOVES)
		{
			modchance[ARMOR_MOD]=0.1;
			modchance[DEXTERITY_MOD]=0.2;
			modchance[MAGIC_POWER_MOD]=0.2;
			modchance[STRENGTH_MOD]=0.2;
			modchance[RESIST_FIRE_MOD]=0.034;
			modchance[RESIST_ICE_MOD]=0.033;
			modchance[RESIST_AIR_MOD]=0.033;
			modchance[ATTACK_SPEED_MOD]=0.2;

			if (subtype =="leath_gl")
			{
				min_enchant = 20;
				max_enchant= 80;
				item->m_price = 60;
				item->m_size = Item::MEDIUM;
				item->m_equip_effect->m_darmor = 2;
			}

		}

		else if (type == Item::SHIELD)
		{
			modchance[BLOCK_MOD]=0.6;
			modchance[RESIST_PHYS_MOD]=0.05;
			modchance[RESIST_FIRE_MOD]=0.1;
			modchance[RESIST_ICE_MOD]=0.1;
			modchance[RESIST_AIR_MOD]=0.1;
			modchance[RESIST_ALL_MOD]=0.05;
			if (subtype =="wood_sh")
			{
				min_enchant = 40;
					max_enchant= 150;
					item->m_price = 15;
					item->m_size = Item::MEDIUM;
					item->m_equip_effect->m_dblock = 10;
			}
			else if (subtype =="iron_sh")
			{
				 min_enchant = 60;
					max_enchant= 300;
					item->m_price = 45;
					item->m_size = Item::BIG;
					item->m_equip_effect->m_dblock = 20;

			}


		}
		else if (type == Item::RING)
		{
			modchance[WILLPOWER_MOD]=0.2;
			modchance[MAGIC_POWER_MOD]=0.25;
			modchance[RESIST_FIRE_MOD]=0.10;
			modchance[RESIST_ICE_MOD]=0.10;
			modchance[RESIST_AIR_MOD]=0.10;
			modchance[RESIST_ALL_MOD]=0.10;
			modchance[DAMAGE_MULT_FIRE_MOD]=0.05;
			modchance[DAMAGE_MULT_ICE_MOD]=0.05;
			modchance[DAMAGE_MULT_AIR_MOD]=0.05;

			min_enchant = std::min(200.0,magic_power*0.2);
			max_enchant= std::min(850.0,magic_power*1.5);
			item->m_price = 350;
			item->m_size = Item::SMALL;
		}
		else if (type == Item::AMULET)
		{
			modchance[WILLPOWER_MOD]=0.2;
			modchance[MAGIC_POWER_MOD]=0.20;
			modchance[RESIST_FIRE_MOD]=0.10;
			modchance[RESIST_ICE_MOD]=0.10;
			modchance[RESIST_AIR_MOD]=0.10;
			modchance[RESIST_ALL_MOD]=0.30;
			min_enchant = std::min(200.0,magic_power*0.2);
			max_enchant= std::min(850.0,magic_power*1.5);
			item->m_size = Item::SMALL;

		}
	}
	else if (type == Item::WEAPON)
	{
		modchance[DEXTERITY_MOD]=0.1;
		modchance[MAGIC_POWER_MOD]=0.05;
		modchance[STRENGTH_MOD]=0.1;
		modchance[DAMAGE_PHYS_MOD]=0.15;
		modchance[DAMAGE_FIRE_MOD]=0.10;
		modchance[DAMAGE_ICE_MOD]=0.10;
		modchance[DAMAGE_AIR_MOD]=0.10;
		modchance[ATTACK_MOD]= 0.1;
		modchance[POWER_MOD] = 0.1;
		modchance[DAMAGE_MULT_PHYS_MOD]=0.1;


		m_creature_base_attr_mod = new CreatureBaseAttrMod();
		m_weaponattr = new WeaponAttr();
		item->m_useup_effect = 0;
		item->m_equip_effect = m_creature_base_attr_mod;
		item->m_weapon_attr = m_weaponattr;

		if (subtype =="short_sw")
		{
			min_enchant = 30;
				max_enchant= 120;
				item->m_price = 150;
				item->m_size = Item::MEDIUM;
				item->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] = 5;
				item->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] = 10;
				item->m_weapon_attr->m_damage.m_attack = 20;
				item->m_weapon_attr->m_damage.m_power = 30;
				item->m_weapon_attr->m_attack_range = 1.0;
				item->m_weapon_attr->m_two_handed = false;
				item->m_weapon_attr->m_dattack_speed = 500;
		}
		else if (subtype =="long_sw")
		{
			min_enchant = 60;
				max_enchant= 250;
				item->m_price = 200;
				item->m_size = Item::MEDIUM;
				item->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] = 5;
				item->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] = 20;
				item->m_weapon_attr->m_damage.m_attack = 30;
				item->m_weapon_attr->m_damage.m_power = 40;
				item->m_weapon_attr->m_attack_range = 1.5;
				item->m_weapon_attr->m_two_handed = false;
				item->m_weapon_attr->m_dattack_speed = 0;
		}
		else if (subtype =="wood_bow")
		{

				min_enchant = 50;
				max_enchant= 200;
				item->m_price = 100;
				item->m_size = Item::MEDIUM;
				item->m_weapon_attr->m_weapon_type=WeaponAttr::BOW;
				item->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] = 5;
				item->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] = 15;
				item->m_weapon_attr->m_damage.m_attack = 20;
				item->m_weapon_attr->m_damage.m_power = 40;
				item->m_weapon_attr->m_attack_range = 40.0;
				item->m_weapon_attr->m_two_handed = true;
				item->m_weapon_attr->m_dattack_speed = 1000;
		}
		else if (subtype =="long_bow")
		{
			min_enchant = 80;
				max_enchant= 300;
				item->m_price = 350;
				item->m_size = Item::BIG;
				item->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] = 5;
				item->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] = 30;
				item->m_weapon_attr->m_damage.m_attack = 30;
				item->m_weapon_attr->m_damage.m_power = 60;
				item->m_weapon_attr->m_attack_range = 70.0;
				item->m_weapon_attr->m_two_handed = true;
				item->m_weapon_attr->m_dattack_speed = 1000;
		}
		else if (subtype =="fire_bow")
		{
			min_enchant = 150;
				max_enchant= 600;
				item->m_price = 800;
				item->m_size = Item::BIG;
				item->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] = 10;
				item->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] = 30;
				item->m_weapon_attr->m_damage.m_min_damage[Damage::FIRE] = 20;
				item->m_weapon_attr->m_damage.m_max_damage[Damage::FIRE] = 45;
				item->m_weapon_attr->m_damage.m_attack = 30;
				item->m_weapon_attr->m_damage.m_power = 50;
				item->m_weapon_attr->m_attack_range = 60.0;
				item->m_weapon_attr->m_two_handed = true;
				item->m_weapon_attr->m_dattack_speed = 500;
		}
		else if (subtype =="battle_axe")
		{
			min_enchant = 100;
				max_enchant= 400;
				item->m_price = 500;
				item->m_size = Item::BIG;
				item->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] = 20;
				item->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] = 40;
				item->m_weapon_attr->m_damage.m_attack = 50;
				item->m_weapon_attr->m_damage.m_power = 60;
				item->m_weapon_attr->m_attack_range = 2.0;
				item->m_weapon_attr->m_two_handed = true;
				item->m_weapon_attr->m_dattack_speed = -500;
		}
		else if (subtype =="ice_wand")
		{
			item->m_price = 700;
				item->m_size = Item::MEDIUM;
				item->m_equip_effect->m_dmagic_power = 70;
				item->m_weapon_attr->m_damage.m_min_damage[Damage::ICE] = 10;
				item->m_weapon_attr->m_damage.m_max_damage[Damage::ICE] = 30;
				item->m_weapon_attr->m_damage.m_attack = 30;
				item->m_weapon_attr->m_damage.m_power = 70;
				item->m_weapon_attr->m_damage.m_special_flags = Damage::EXTRA_UNDEAD_DMG;
				item->m_weapon_attr->m_damage.m_special_flags = Damage::EXTRA_DEMON_DMG;
				item->m_weapon_attr->m_attack_range = 4.0;
				item->m_weapon_attr->m_two_handed = false;
				item->m_weapon_attr->m_dattack_speed = 0;
		}
		else if (subtype =="ice_staff")
		{
			item->m_price = 700;
			item->m_size = Item::BIG;
			item->m_equip_effect->m_dmagic_power = 70;
			item->m_weapon_attr->m_damage.m_min_damage[Damage::ICE] = 10;
			item->m_weapon_attr->m_damage.m_max_damage[Damage::ICE] = 30;
			item->m_weapon_attr->m_damage.m_attack = 30;
			item->m_weapon_attr->m_damage.m_power = 70;
			item->m_weapon_attr->m_damage.m_special_flags = Damage::EXTRA_UNDEAD_DMG;
			item->m_weapon_attr->m_damage.m_special_flags = Damage::EXTRA_DEMON_DMG;
			item->m_weapon_attr->m_attack_range = 4.0;
			item->m_weapon_attr->m_two_handed = true;
			item->m_weapon_attr->m_dattack_speed = 0;
		}
		else if (subtype =="holy_flail")
		{
			item->m_price = 500;
				item->m_size = Item::MEDIUM;
				item->m_equip_effect->m_dwillpower = 35;
				item->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] = 5;
				item->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] = 50;
				item->m_weapon_attr->m_damage.m_attack = 20;
				item->m_weapon_attr->m_damage.m_power = 50;
				item->m_weapon_attr->m_damage.m_special_flags = Damage::EXTRA_UNDEAD_DMG;
				item->m_weapon_attr->m_attack_range = 1.5;
				item->m_weapon_attr->m_two_handed = false;
				item->m_weapon_attr->m_dattack_speed = -500;
		}
	}
	else if (type == Item::POTION)
	{
		magic_power =0;

		m_creature_dyn_attr_mod = new CreatureDynAttrMod();
		item->m_useup_effect = m_creature_dyn_attr_mod;
		item->m_equip_effect = 0;
		item->m_weapon_attr = 0;

		 if (subtype =="heal_1")
		{

				item->m_price = 20;
				item->m_size = Item::SMALL;
				item->m_useup_effect->m_dhealth = 50;
		}
		else if (subtype =="heal_2")
		{
			item->m_price = 80;
				item->m_size = Item::SMALL;
				item->m_useup_effect->m_dhealth = 200;
		}
		else if (subtype =="heal_bl")
		{
			item->m_price = 100;
				item->m_size = Item::SMALL;
				item->m_useup_effect->m_dstatus_mod_immune_time[Damage::BLIND] = 8000;
		}
		else if (subtype =="heal_fr")
		{
			item->m_price = 50;
				item->m_size = Item::SMALL;
				item->m_useup_effect->m_dstatus_mod_immune_time[Damage::FROZEN] = 1;
		}
	}

	// Verzauberungen bestimmen
	item->m_magic_power= magic_power;

	if (magic_power>min_enchant)
	{

		createMagicMods(item,modchance,magic_power, min_enchant, max_enchant);
	}

	// Preis ausrechnen
	item->calcPrice();

	return item;

}

Item* ItemFactory::createGold(int value, int id)
{
	Item* item = new Item;
	if (id ==0)
	{
		id = World::getWorld()->getValidId();
	}
	
	item->m_size = Item::GOLD;
	item->m_type = Item::GOLD_TYPE;
	item->m_id = id;
	item->m_subtype = "gold";
	item->m_price = value;
	
	return item;
}


void ItemFactory::createMagicMods(Item* item, float* modchance, float magic_power, float min_enchant, float max_enchant)
{
	// Modifikationen des Items auswuerfeln
	DEBUG4("mods auswuerfeln");

	magic_power = std::min(magic_power, max_enchant*4);
	
	// bisher zugeteilte Staerke der Modifikation
	float mod_power[31];
	memset(mod_power,0,31*sizeof(float));

	float sum =0;
	int i;
	for (i=0;i<31;i++)
	{
		sum += modchance[i];
	}

		// Staerke der aktuellen Verzauberung
	float mp;
	float sqrtmp;
	float logmp;

	float dmgdiff,dmgavg;

		// durch Verzauberungen benoetigtes Level
	int levelreq =0;

		// aktuelle Verzauberung
	int mod;

		// Anzahl verschiedene Verzauberungen
	int num_mods=0;
	while (magic_power>min_enchant && num_mods<4)
	{
			// Staerke auswuerfeln
		mp = Random::randrangef(min_enchant,max_enchant);
		mp = std::min(mp, magic_power);
		sqrtmp = sqrt(mp);
		logmp = log(mp);
		magic_power -= mp;
		DEBUG5("ausgewuerfelt: Starke der Verzauberung: %f",mp);

			// Modifikation auswuerfeln
		mod = Random::randDiscrete(modchance,31,sum);
		DEBUG4("Art der Verzauberung: %i",mod);

		num_mods++;

		levelreq = std::max(levelreq,(int) (mp*0.06-1));
		levelreq = std::min(80,levelreq);

		dmgavg = mp*0.06;

			// Wirkung der Modifikation
		switch (mod)
		{
			case HEALTH_MOD:
				item->m_equip_effect->m_dmax_health += ceil(mp*0.15);
				break;

			case ARMOR_MOD:
				item->m_equip_effect->m_darmor += (short) ceil(mp*0.08);
				break;

			case BLOCK_MOD:
				item->m_equip_effect->m_dblock += (short) ceil(mp*0.08);
				break;

			case STRENGTH_MOD:
				item->m_equip_effect->m_dstrength += (short) ceil(mp*0.03);
				break;

			case DEXTERITY_MOD:
				item->m_equip_effect->m_ddexterity += (short) ceil(mp*0.03);
				break;

			case WILLPOWER_MOD:
				item->m_equip_effect->m_dwillpower += (short) ceil(mp*0.03);
				break;

			case MAGIC_POWER_MOD:
				item->m_equip_effect->m_dmagic_power += (short) ceil(mp*0.03);
				break;

			case RESIST_PHYS_MOD:
				item->m_equip_effect->m_dresistances[Damage::PHYSICAL] += (short) ceil(sqrtmp*0.5);
				break;

			case RESIST_FIRE_MOD:
				item->m_equip_effect->m_dresistances[Damage::FIRE] += (short) ceil(sqrtmp*1.0);
				break;

			case RESIST_ICE_MOD:
				item->m_equip_effect->m_dresistances[Damage::FIRE] += (short) ceil(sqrtmp*1.0);
				break;

			case RESIST_AIR_MOD:
				item->m_equip_effect->m_dresistances[Damage::AIR] += (short) ceil(sqrtmp*1.0);
				break;

			case RESIST_ALL_MOD:
				item->m_equip_effect->m_dresistances[Damage::FIRE] += (short) ceil(sqrtmp*0.5);
				item->m_equip_effect->m_dresistances[Damage::ICE] += (short) ceil(sqrtmp*0.5);
				item->m_equip_effect->m_dresistances[Damage::AIR] += (short) ceil(sqrtmp*0.5);
				break;

			case DAMAGE_PHYS_MOD:
				dmgavg *=0.6;
				dmgdiff = Random::randrangef(dmgavg*0.2,dmgavg*0.6);
				item->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] += ceil(dmgavg-dmgdiff);
				item->m_weapon_attr->m_damage.m_max_damage[Damage::PHYSICAL] += ceil(dmgavg+dmgdiff);
				break;

			case DAMAGE_FIRE_MOD:
				dmgdiff = Random::randrangef(dmgavg*0.2,dmgavg*0.5);
				item->m_weapon_attr->m_damage.m_min_damage[Damage::FIRE] += ceil(dmgavg-dmgdiff);
				item->m_weapon_attr->m_damage.m_max_damage[Damage::FIRE] += ceil(dmgavg+dmgdiff);
				break;

			case DAMAGE_ICE_MOD:
				dmgdiff = Random::randrangef(dmgavg*0.1,dmgavg*0.3);
				item->m_weapon_attr->m_damage.m_min_damage[Damage::ICE] += ceil(dmgavg-dmgdiff);
				item->m_weapon_attr->m_damage.m_max_damage[Damage::ICE] += ceil(dmgavg+dmgdiff);
				break;

			case DAMAGE_AIR_MOD:
				dmgdiff = Random::randrangef(dmgavg*0.4,dmgavg*0.8);
				item->m_weapon_attr->m_damage.m_min_damage[Damage::AIR] += ceil(dmgavg-dmgdiff);
				item->m_weapon_attr->m_damage.m_max_damage[Damage::AIR] += ceil(dmgavg+dmgdiff);
				break;

			case DAMAGE_MULT_PHYS_MOD:
				item->m_weapon_attr->m_damage.m_multiplier[Damage::PHYSICAL] *= (1+sqrtmp*0.01);
				break;

			case DAMAGE_MULT_FIRE_MOD:
				item->m_weapon_attr->m_damage.m_multiplier[Damage::FIRE] *= (1+sqrtmp*0.006);
				break;

			case DAMAGE_MULT_ICE_MOD:
				item->m_weapon_attr->m_damage.m_multiplier[Damage::ICE] *= (1+sqrtmp*0.006);
				break;

			case DAMAGE_MULT_AIR_MOD:
				item->m_weapon_attr->m_damage.m_multiplier[Damage::AIR] *= (1+sqrtmp*0.006);
				break;

			case ATTACK_SPEED_MOD:
				item->m_equip_effect->m_dattack_speed += (short)  (sqrtmp*15);
				break;

			case ATTACK_MOD:
				item->m_weapon_attr->m_damage.m_attack += ceil(mp*0.05);
				break;

			case POWER_MOD:
				item->m_weapon_attr->m_damage.m_power += ceil(mp*0.05);
				break;

		}

			// jede Modifikation darf nur einmal vorkommen, entfernen

		sum -= modchance[mod];
		modchance[mod]=0;
	}

	item->m_level_req = std::max(item->m_level_req,(char) levelreq);
	DEBUG5("level req %i",item->m_level_req);
}

Item::Type  ItemFactory::getBaseType(Item::Subtype subtype)
{
	std::map<Item::Subtype,Item::Type>::iterator it;
	it = m_item_types.find(subtype);
	
	if (it != m_item_types.end())
	{
		return it->second;
	}
	
	return Item::NOITEM;
}

void ItemFactory::registerItemDrop(Item::Type type,Item::Subtype subtype, DropChance chance)
{
	m_item_probabilities.insert(make_pair(subtype, chance));
	m_item_types.insert(make_pair(subtype,type));
}

void ItemFactory::registerItem(Item::Type type,Item::Subtype subtype, ItemBasicData* data)
{
	DEBUG5("registered item %s %p",subtype.c_str(), data->m_weapon_attr);
	m_item_data.insert(make_pair(subtype, data));
	m_item_types.insert(make_pair(subtype,type));
}

void ItemFactory::loadItemData(std::string file)
{
	// Items aus XML Laden
	ItemLoader itemloader;

	std::list<ItemBasicData*>* item_list;
	item_list = itemloader.loadItemBasicData(file.c_str());

	if (item_list != 0)
	{
		std::list<ItemBasicData*>::iterator iter = item_list->begin();
		while (iter != item_list->end())
		{
			// Debugging: Anzeigen der geladenen Items
			/*
			cout << "m_useup_effect" << " = " << (*iter)->m_useup_effect << endl;
			cout << "m_equip_effect" << " = " << (*iter)->m_equip_effect << endl;
			cout << "m_weapon_attr" << " = " << (*iter)->m_weapon_attr << endl;

			cout << "     m_damage.m_min_damage[Damage::PHYSICAL]" << " = " << (*iter)->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] << endl;
			cout << "     m_damage.m_max_damage[Damage::PHYSICAL]" << " = " << (*iter)->m_weapon_attr->m_damage.m_min_damage[Damage::PHYSICAL] << endl;
			cout << "     m_damage.m_min_damage[Damage::AIR]" << " = " << (*iter)->m_weapon_attr->m_damage.m_min_damage[Damage::AIR] << endl;
			cout << "     m_damage.m_max_damage[Damage::AIR]" << " = " << (*iter)->m_weapon_attr->m_damage.m_min_damage[Damage::AIR] << endl;
			cout << "     m_damage.m_min_damage[Damage::ICE]" << " = " << (*iter)->m_weapon_attr->m_damage.m_min_damage[Damage::ICE] << endl;
			cout << "     m_damage.m_max_damage[Damage::ICE]" << " = " << (*iter)->m_weapon_attr->m_damage.m_min_damage[Damage::ICE] << endl;
			cout << "     m_damage.m_min_damage[Damage::FIRE]" << " = " << (*iter)->m_weapon_attr->m_damage.m_min_damage[Damage::FIRE] << endl;
			cout << "     m_damage.m_max_damage[Damage::FIRE]" << " = " << (*iter)->m_weapon_attr->m_damage.m_min_damage[Damage::FIRE] << endl;
			cout << "     m_attack_range" << " = " << (*iter)->m_weapon_attr->m_attack_range << endl;
			cout << "     m_two_handed" << " = " << (*iter)->m_weapon_attr->m_two_handed << endl;
			cout << "     m_dattack_speed" << " = " << (*iter)->m_weapon_attr->m_dattack_speed << endl;

			cout << "m_level_req" << " = " << static_cast<int>((*iter)->m_level_req) << endl;
			cout << "m_char_req" << " = " << static_cast<int>((*iter)->m_char_req) << endl;
			cout << "m_subtype" << " = " << (*iter)->m_subtype << endl;
			cout << "m_type" << " = " << (*iter)->m_type << endl;
			cout << "m_size" << " = " << (*iter)->m_size << endl;
			cout << "m_price" << " = " << (*iter)->m_price << endl;
			cout << "m_min_enchant" << " = " << (*iter)->m_min_enchant << endl;
			cout << "m_max_enchant" << " = " << (*iter)->m_max_enchant << endl;
			cout << "------------------------------------------------" << endl;
			*/
			registerItem((*iter)->m_type,(*iter)->m_subtype, *iter);
			
			iter++;
		}
	}


	std::list<DropChanceData*>* drop_chance_list;
	drop_chance_list = itemloader.loadDropChanceData(file.c_str());

	if (drop_chance_list != 0)
	{
		// Daten auslesen und registrieren
		std::list<DropChanceData*>::iterator iter = drop_chance_list->begin();
		while (iter != drop_chance_list->end())
		{
			registerItemDrop( (*iter)->m_type, (*iter)->m_subtype, DropChance( (*iter)->m_level, (*iter)->m_probability, (*iter)->m_size) );
			iter++;
		}

		// Liste aus Speicher loeschen
		iter = drop_chance_list->begin();
		while (iter != drop_chance_list->end())
		{
			delete *iter;
			iter++;
		}
	}

	delete item_list;
	item_list = 0;
	delete drop_chance_list;
	drop_chance_list = 0;
}

void ItemFactory::init()
{

	registerItemDrop(Item::WEAPON,"long_sw",DropChance(10,1,Item::MEDIUM));
	registerItemDrop(Item::WEAPON,"wood_bow",DropChance(3,1,Item::MEDIUM));
	registerItemDrop(Item::WEAPON,"long_bow",DropChance(10,1,Item::BIG));
	registerItemDrop(Item::WEAPON,"battle_axe",DropChance(20,1,Item::BIG));
	registerItemDrop(Item::WEAPON,"holy_flail",DropChance(10,0.4,Item::BIG));

	registerItemDrop(Item::ARMOR,"leath_arm",DropChance(0,1,Item::BIG));
	registerItemDrop(Item::ARMOR,"tiled_arm",DropChance(20,1,Item::BIG));
	registerItemDrop(Item::ARMOR,"heavy_arm",DropChance(40,1,Item::BIG));


	registerItemDrop(Item::HELMET,"steel_hlm",DropChance(5,1,Item::MEDIUM));

	registerItemDrop(Item::RING,"ring",DropChance(-1,0.1,Item::SMALL));


	registerItemDrop(Item::AMULET,"amulet",DropChance(-1,0.1,Item::SMALL));

	registerItemDrop(Item::SHIELD,"wood_sh",DropChance(0,1,Item::MEDIUM));
	registerItemDrop(Item::SHIELD,"iron_sh",DropChance(6,1,Item::BIG));

	registerItemDrop(Item::GLOVES,"leath_gl",DropChance(0,1,Item::MEDIUM));

	registerItemDrop(Item::POTION,"heal_1",DropChance(0,1,Item::SMALL));
	registerItemDrop(Item::ARMOR,"heal_2",DropChance(-10,1,Item::SMALL));
	registerItemDrop(Item::ARMOR,"heal_bl",DropChance(-1,0.5,Item::SMALL));
	registerItemDrop(Item::ARMOR,"heal_fr",DropChance(-1,0.5,Item::SMALL));


}

void ItemFactory::cleanup()
{
	DEBUG("cleanup");

	std::map<Item::Subtype,ItemBasicData*>::iterator it;
	for (it = m_item_data.begin(); it != m_item_data.end(); ++it)
	{
		DEBUG5("deleting item data %s",it->first.c_str());
		delete it->second;
	}
}

Item* ItemFactory::createItem(DropSlot &slot)
{
	DEBUG5("get item by dropslot");
	// wenn maximales Level unter 0 liegt kein Item ausgeben
	if (slot.m_max_level<0)
		return 0;

	DEBUG5("drop item %i %i",slot.m_min_level, slot.m_max_level);
	int size = Random::randDiscrete(slot.m_size_probability,5);

	// kein Item ausgewurfelt
	if (size ==4)
		return 0;

	
	
	DEBUG5("item size: %i",size);
	Item* item =0;
	if (size  == Item::GOLD)
	{
		
		// Gold gedroppt
		item = createGold(Random::randrangei((3+slot.m_max_level*slot.m_max_level)/5+1, 10+(slot.m_max_level*slot.m_max_level+1)));

		return item;
	}
	else
	{
		// *richtiges* Item gedroppt

		// Vector fuer die moeglichen Items
		std::vector<Item::Subtype> types;

		// Vector fuer die Wahrscheinlickeit
		std::vector<float> prob;

		float p;

		std::map<Item::Subtype, DropChance>::iterator i;
		for (i= m_item_probabilities.begin();i!=m_item_probabilities.end();++i)
		{
			// Testen ob die Groesse passt
			if (i->second.m_size != size)
				continue;

			// Testen ob Level ausreicht
			if (fabs(i->second.m_level) > slot.m_max_level || i->second.m_level <slot.m_min_level )
				continue;

			// Wahrscheinlichkeit fuer Drop berechnen
			p = i->second.m_probability;

			if (p>0)
			{
				DEBUG5("possible item subtype %s prob %f",i->first.c_str(),p);
				types.push_back(i->first);
				prob.push_back(p);

			}


		}

		if (prob.empty())
			return 0;

		// Item auswuerfeln
		int res = Random::randDiscrete(prob);

		// Typ, Subtyp bestimmen
		Item::Subtype subtype= types[res];
		Item::Type type = m_item_types.find(subtype)->second;

		// Magiestaerke berechnen
		float magic =0;
		if (Random::random() < slot.m_magic_probability || type == Item::RING  || type == Item::AMULET)
		{
			magic = Random::randrangef(slot.m_magic_power/4,slot.m_magic_power);
			DEBUG5("magic power %f",magic);
		}

		item = createItem(type,subtype,0,magic);
		DEBUG5("item type %i  subtype %s level %i",type,subtype.c_str(), item->m_level_req);
		return item;
	}
	return 0;

}

