#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H


#include "item.h"
#include "random.h"
#include <map>
#include "dropslot.h"
#include "itemloader.h"




/**
 * \class ItemFactory
 * \brief Dieses Objekt erzeugt Objekte vom Typ ServerItem.
 */
class ItemFactory
{
public:

	/**
	 * \enum ItemMod
	 * \brief moegliche Modifikationen von Items
	 */
	enum ItemMod
	{
		HEALTH_MOD=0,
		ARMOR_MOD =1,
		BLOCK_MOD =2,
		STRENGTH_MOD =3,
		DEXTERITY_MOD = 4,
		MAGIC_POWER_MOD = 5,
		WILLPOWER_MOD = 6,
		RESIST_PHYS_MOD = 7,
		RESIST_FIRE_MOD = 8,
		RESIST_ICE_MOD = 9,
		RESIST_AIR_MOD = 10,
		RESIST_ALL_MOD= 11,
		DAMAGE_PHYS_MOD= 12,
		DAMAGE_FIRE_MOD= 13,
		DAMAGE_ICE_MOD= 14,
		DAMAGE_AIR_MOD= 15,
		DAMAGE_MULT_PHYS_MOD = 16,
		DAMAGE_MULT_FIRE_MOD = 17,
		DAMAGE_MULT_ICE_MOD = 18,
		DAMAGE_MULT_AIR_MOD = 19,
		ATTACK_SPEED_MOD = 20,
		ATTACK_MOD = 22,
		POWER_MOD = 23,
		MAGE_FIRE_SKILLS_MOD = 24,
		MAGE_ICE_SKILLS_MOD = 25,
		MAGE_AIR_SKILLS_MOD = 26,

	};
	/**
	 * \fn ItemFactory()
	 * \brief Konstruktor
	 */
	ItemFactory();

	/**
	 * \fn Item* createItem(Item::Type type, Item::Subtype subtype,int id=0,float magic_power =0)
	 * \brief Erzeugt Objekt anhand zweier Parameter
	 * \param type Typ des Items
	 * \param subtype Untertyp des Items
	 * \param id ID des neuen Items. Wenn 0 angegeben wird, dann wird eine neue ID erzeugt
	 * \param magic_power Verzauberungsstaerke
	 * \return Neues ServerItem
	 */
	static Item* createItem(Item::Type type, Item::Subtype subtype, int id=0, float magic_power =0);

	/**
	 * \fn static void createMagicMods(Item* item, float* modchance, float magic_power, float min_enchant, float max_enchant)
	 * \brief Belegt einen Gegenstanden mit magischen Attributen
	 * \param item Gegenstand der die magischen Eigenschaften erhaelt
	 * \param modchance Array mit Wahrscheinlichkeiten fuer das Auftreten der einzelnen Modifikationen
	 * \param magic_power Staerke der Verzauberung
	 * \param min_enchant minimale Starke einer einzelnen Verzauberung
	 * \param max_enchant maximale Starke einer einzelnen Verzauberung
	 */
	static void createMagicMods(Item* item, float* modchance, float magic_power, float min_enchant, float max_enchant);

	/**
	 * \fn static void registerItem(Item::Type type, Item::Subtype subtype, ItemBasicData* data)
	 * \brief Registriert ein Item
	 * \param type Typ des Items
	 * \param subtype Subtype des items
	 * \param data Daten die zum Erzeugen des Items noetig sind
	 */
	static void registerItem(Item::Type type, Item::Subtype subtype, ItemBasicData* data);
	
	/**
	 * \fn static void registerItemDrop(Item::Type type, Item::Subtype subtype, DropChance chance)
	 * \brief Registriert ein Item, das gedroppt werden kann
	 * \param type Typ des Items
	 * \param subtype Subtyp des Items
	 * \param chance Chance, das das Item gedroppt wird
	 */
	static void registerItemDrop(Item::Type type, Item::Subtype subtype, DropChance chance);

	/**
	 * \fn static Item* createItem(DropSlot &slot)
	 * \brief Erzeugt einen Gegenstand anhand eines Dropslots. Wenn kein Item entsteht wird NULL ausgegeben
	 */
	static Item* createItem(DropSlot &slot);

	/**
	 * \fn static void init()
	 * \brief Registriert die Items
	 */
	static void init();
	
	/**
	 * \fn static void cleanup()
	 * \brief Gibt allokierten Speicher frei
	 */
	static void cleanup();
	
	/**
	 * \fn void loadItemData(std::string file)
	 * \brief Laedt Daten zu Gegenstaenden aus der angegebenen Datei
	 * \param file Item XML Datei
	 */
	static void loadItemData(std::string file);


	private:
		/**
		* \var static std::map<Item::Subtype, DropChance> m_item_probabilities
		 * \brief Gibt fuer jeden Gegenstand die relative Dropchance an
		 */
		static std::map<Item::Subtype, DropChance> m_item_probabilities;
		
		/**
		 * \var static std::map<Item::Subtype, Item::Type> m_item_types
		 * \brief Mappt bekannt Subtypen auf den zugehoerigen Typ
		 */
		static std::map<Item::Subtype, Item::Type> m_item_types;

        /**
         * \var static std::map<Item::Subtype,ItemBasicData*> m_item_data
         * \brief enthaelt Basisdaten zu allen Items
         */
		static std::map<Item::Subtype,ItemBasicData*> m_item_data;
		
	
};

#include "world.h"

#endif //ITEMFACTORY_H

