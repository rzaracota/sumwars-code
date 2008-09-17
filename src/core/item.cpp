#include "item.h"


string Item::getName()
{
    #ifndef WIN32
        return (gettext((getString()).c_str()));
    #else
        return getString();
    #endif
}


string Item::getString()
{
	/*
	switch (m_type)
	{
		case NOITEM:
			return "noitem";
			break;
		case ARMOR:
			switch (m_subtype)
			{
				case LEATHER_ARMOR:
					return "leather_armor";
				case TILED_ARMOR:
					return "tiled_armor";
				case HEAVY_ARMOR:
					return "heavy_armor";

				default:
					return "armor";
			}
			break;
		case HELMET:
			switch (m_subtype)
			{
				case STEEL_HELMET:
					return "steel_helmet";
				case DIVINE_HELMET:
					return "divine_helmet";

				default:
					return "helmet";
			}
			break;
		case GLOVES:
			return "gloves";
			break;
		case WEAPON:
			switch (m_subtype)
			{
				case SHORT_SWORD:
					return "short_sword";
				case LONG_SWORD:
					return "long_sword";
				case WOODEN_BOW:
					return "wooden_bow";
				case LONG_BOW:
					return "long_bow";
				case BURNING_BOW:
					return "burning_bow";
				case BATTLE_AXE:
					return "battle_axe";
				case HOLY_FLAIL:
					return "holy_flail";

				default:
					return "weapon";
			}
			break;
		case SHIELD:
			switch (m_subtype)
			{
				case WOODEN_SHIELD:
					return "wooden_shield";
				case IRON_SHIELD:
					return "iron_shield";

				default:
					return "shield";
			}
			break;
		case POTION:
			switch (m_subtype)
			{
				case SMALL_HEAL_POTION:
					return "small_heal_potion";
				case BIG_HEAL_POTION:
					return "big_heal_potion";
				case HEAL_BLIND_POTION:
					return "heal_blind_potion";
				case UNFREEZE_POTION:
					return "instant_unfreeze_potion";

				default:
					return "potion";
			}
			break;
		case RING:
			return "ring";
			break;
		case AMULET:
			return "amulet";
			break;
		default:
			return "unknown";
			break;
	}
	*/
	return m_subtype;
}


void Item::toString(CharConv* cv, short pos)
{

	cv->toBuffer((char) m_type);
	char stmp[11];
	stmp[10] = '\0';
	strncpy(stmp,m_subtype.c_str(),10);
	cv->toBuffer(stmp,10);
	cv->toBuffer((char) m_size);
	cv->toBuffer(pos);


}

void Item::fromString(CharConv* cv, short& pos)
{
	char ctmp;
	short tmp;
	cv->fromBuffer<char>(ctmp);
	m_type = (Type) ctmp;
	char stmp[11];
	stmp[10] ='\0';
	cv->fromBuffer(stmp,10);
	m_subtype = stmp;
	cv->fromBuffer<char>(ctmp);
	m_size = (Size) ctmp;
	cv->fromBuffer<short>(pos);

}



