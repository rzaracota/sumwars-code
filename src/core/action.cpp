#include "action.h"

Action::ActionInfo Action::m_base_info[192];

std::map<std::string, Action::ActionType> Action::m_enum_string_to_type;

void Action::init()
{
	DEBUG5("initialising actions");
	Action::ActionInfo* a;


	a = &(Action::m_base_info[Action::NOACTION]);
	a->m_timer_nr=0;
	a->m_standard_time=2000;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=-1;
	a->m_distance = SELF;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0]= Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "noaction";
	a->m_animation[NO_WEAPON].push_back("idle");
	a->m_animation[ONE_HANDED].push_back("idle");
	a->m_animation[TWO_HANDED].push_back("idle");

	a = &(Action::m_base_info[Action::USE]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.5;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0]= Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "use";
	a->m_animation[NO_WEAPON].push_back("idle");
	a->m_animation[ONE_HANDED].push_back("idle");
	a->m_animation[TWO_HANDED].push_back("idle");

	a = &(Action::m_base_info[Action::DIE]);
	a->m_timer_nr=0;
	a->m_standard_time=1000;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=-1;
	a->m_distance = SELF;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0]= Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "die";
	a->m_animation[NO_WEAPON].push_back("die");
	a->m_animation[ONE_HANDED].push_back("die");
	a->m_animation[TWO_HANDED].push_back("die");


	a = &(Action::m_base_info[Action::WALK]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::WALK;
	a->m_critical_perc=-1;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "walk";
	a->m_animation[NO_WEAPON].push_back("walkOneHand");
	a->m_animation[ONE_HANDED].push_back("walkOneHand");
	a->m_animation[TWO_HANDED].push_back("walkOneHand");

	a = &(Action::m_base_info[Action::TAKE_ITEM]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.7;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "take_item";

	a = &(Action::m_base_info[Action::SPEAK]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.7;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "speak";

	a = &(Action::m_base_info[Action::ATTACK]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = Action::ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "attack";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[Action::RANGE_ATTACK]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "range_attack";
    a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::MAGIC_ATTACK]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.5;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "magic_attack";
    a->m_animation[NO_WEAPON].push_back("attackMagicUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackMagicOneHand");
	a->m_animation[TWO_HANDED].push_back("attackMagicTwoHands");

	a = &(Action::m_base_info[Action::HOLY_ATTACK]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = Action::HOLY_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "holy_attack";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	// Faehigkeiten des Kriegers
	a = &(Action::m_base_info[Action::BASH]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=2000;
	a->m_base_action = Action::ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "bash";
	a->m_animation[NO_WEAPON].push_back("specialAttackOneHand");
	a->m_animation[ONE_HANDED].push_back("specialAttackOneHand");
	a->m_animation[TWO_HANDED].push_back("specialAttackOneHand");


	a = &(Action::m_base_info[Action::HAMMER_BASH]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=2000;
	a->m_base_action = Action::ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::AROUND_BLOW;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "hammer_bash";
	a->m_animation[NO_WEAPON].push_back("specialAttackOneHand");
	a->m_animation[ONE_HANDED].push_back("specialAttackOneHand");
	a->m_animation[TWO_HANDED].push_back("specialAttackOneHand");

	a = &(Action::m_base_info[Action::AROUND_BLOW]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = Action::ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::BASH;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "around_blow";
	a->m_animation[NO_WEAPON].push_back("sweepStrikeOneHand");
	a->m_animation[ONE_HANDED].push_back("sweepStrikeOneHand");
	a->m_animation[TWO_HANDED].push_back("sweepStrikeOneHand");

	a = &(Action::m_base_info[Action::WHIRL_BLOW]);
	a->m_timer_nr=1;
	a->m_standard_time=1.5;
	a->m_timer=3000;
	a->m_base_action = Action::ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::AROUND_BLOW;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "whirl_blow";
	a->m_animation[NO_WEAPON].push_back("sweepStrikeOneHand");
	a->m_animation[ONE_HANDED].push_back("sweepStrikeOneHand");
	a->m_animation[TWO_HANDED].push_back("sweepStrikeOneHand");

	a = &(Action::m_base_info[Action::SMASH]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = Action::ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::HAMMER_BASH;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "smash";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[Action::HATE_MAGE]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = Action::ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::BASH;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "hate_mage";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");


	a = &(Action::m_base_info[Action::CHARGE]);
	a->m_timer_nr=1;
	a->m_standard_time=0.5;
	a->m_timer=6000;
	a->m_base_action = Action::ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::BASH;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "charge";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[Action::STORM_CHARGE]);
	a->m_timer_nr=1;
	a->m_standard_time=0.5;
	a->m_timer=6000;
	a->m_base_action = Action::ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =0;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::CHARGE;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "storm_charge";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[Action::DECOY]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=150000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "decoy";

	a = &(Action::m_base_info[Action::SCARE]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=150000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::BERSERK;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "scare";

	a = &(Action::m_base_info[Action::FIRESWORD]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=240000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "firesword";

	a = &(Action::m_base_info[Action::FLAMESWORD]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=240000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::FLAMEARMOR;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "flamesword";

	a = &(Action::m_base_info[Action::FLAMEARMOR]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=300000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::FIRESWORD;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "flamearmor";

	a = &(Action::m_base_info[Action::BERSERK]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=200000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::DECOY;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "berserk";

	a = &(Action::m_base_info[Action::WARCRY]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=200000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::SCARE;
	a->m_req_ability[1] = Action::FURY;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "warcry";

	a = &(Action::m_base_info[Action::REGENERATE]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=200000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "regenerate";

	a = &(Action::m_base_info[Action::ANGER]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=300000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "anger";

	a = &(Action::m_base_info[Action::FURY]);
	a->m_timer_nr=2;
	a->m_standard_time=500;
	a->m_timer=200000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::ANGER;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "fury";

	a = &(Action::m_base_info[Action::STEADFAST]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "steadfast";

	a = &(Action::m_base_info[Action::BLOCK]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::STEADFAST;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "block";

	a = &(Action::m_base_info[Action::WEAPONMASTER]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::BLOCK;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "weaponmaster";

	a = &(Action::m_base_info[Action::MONSTER_HUNTER]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "monster_hunter";

	a = &(Action::m_base_info[Action::MONSTER_SLAYER]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::MONSTER_HUNTER;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "monster_slayer";

	a = &(Action::m_base_info[Action::ENDURANCE]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::MONSTER_SLAYER;
	a->m_req_ability[1] = Action::WEAPONMASTER;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "endurance";


	// Faehigkeiten des Magiers
	a = &(Action::m_base_info[Action::FIRE_BOLT]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=2500;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "fire_bolt";
    a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::FIRE_STRIKE]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=2500;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::FIRE_BOLT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "fire_strike";
    a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::FIRE_WAVE]);
	a->m_timer_nr=2;
	a->m_standard_time=1301;
	a->m_timer=15000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::INFERNO_BALL;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "fire_wave";
    a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[Action::FIRE_STORM]);
	a->m_timer_nr=2;
	a->m_standard_time=1301;
	a->m_timer=15000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::FIRE_WAVE;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "fire_storm";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[Action::FIRE_BALL]);
	a->m_timer_nr=1;
	a->m_standard_time=1001;
	a->m_timer=6000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::FIRE_BOLT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "fire_ball";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::INFERNO_BALL]);
	a->m_timer_nr=1;
	a->m_standard_time=1001;
	a->m_timer=6000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::FIRE_BALL;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "inferno_ball";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::FIRE_WALL]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=12000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::FIRE_STRIKE;
	a->m_req_ability[1] = Action::FIRE_BALL;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "fire_wall";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[Action::INFLAME]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "inflame";

	a = &(Action::m_base_info[Action::ICE_BOLT]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=3000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "ice_bolt";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");


	a = &(Action::m_base_info[Action::ICE_SPIKE]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=3000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::ICE_BOLT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "ice_spike";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::SNOW_STORM]);
	a->m_timer_nr=2;
	a->m_standard_time=1401;
	a->m_timer=14000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::FREEZE;
	a->m_req_ability[1] = Action::ICE_RING;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "snow_storm";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[Action::BLIZZARD]);
	a->m_timer_nr=2;
	a->m_standard_time=1401;
	a->m_timer=14000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::FROST_RING;
	a->m_req_ability[1] = Action::SNOW_STORM;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "blizzard";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[Action::ICE_RING]);
	a->m_timer_nr=1;
	a->m_standard_time=1001;
	a->m_timer=7000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::ICE_BOLT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "ice_ring";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::FROST_RING]);
	a->m_timer_nr=1;
	a->m_standard_time=1001;
	a->m_timer=7000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::ICE_RING;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "frost_ring";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::FREEZE]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=11000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::ICE_BOLT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "freeze";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::CHILL]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "chill";

	a = &(Action::m_base_info[Action::LIGHTNING]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=2000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "lightning";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::LIGHTNING_STRIKE]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=2000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::LIGHTNING;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "lightning_strike";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::THUNDERSTORM]);
	a->m_timer_nr=2;
	a->m_standard_time=1301;
	a->m_timer=12000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::CHAIN_LIGHTNING;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "thunderstorm";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[Action::THUNDERSTORM2]);
	a->m_timer_nr=2;
	a->m_standard_time=1301;
	a->m_timer=12000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::THUNDERSTORM;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "thunderstorm2";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[Action::CHAIN_LIGHTNING]);
	a->m_timer_nr=1;
	a->m_standard_time=801;
	a->m_timer=7000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::LIGHTNING;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "chain_lightning";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::CHAIN_LIGHTNING2]);
	a->m_timer_nr=1;
	a->m_standard_time=801;
	a->m_timer=7000;
	a->m_base_action = Action::MAGIC_ATTACK;
	a->m_critical_perc=0.8;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::CHAIN_LIGHTNING;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "chain_lightning2";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::STATIC_SHIELD]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=40000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.8;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::IONISATION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "static_shield";
	a->m_animation[NO_WEAPON].push_back("castMagicWeak");
	a->m_animation[ONE_HANDED].push_back("castMagicWeak");
	a->m_animation[TWO_HANDED].push_back("castMagicWeak");

	a = &(Action::m_base_info[Action::IONISATION]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "ionisation";


	// Schuetze Faehigkeiten
	a = &(Action::m_base_info[Action::TRIPLE_SHOT]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.3;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "triple_shot";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");


	a = &(Action::m_base_info[Action::GUIDED_TRIPLE_SHOT]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.3;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::MULTISHOT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "guided_triple_shot";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::MULTISHOT]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=5000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.7;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::TRIPLE_SHOT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "multishot";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::VOLLEY_SHOT]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=5000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.7;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::GUIDED_TRIPLE_SHOT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "volley_shot";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::PIERCE]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=2000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "pierce";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::WEAK_POINT]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "weak_point";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::BLIND_RAGE]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::WEAK_POINT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "blind_rage";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::VACUUM]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=6000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "vacuum";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::DEATH_ROULETTE]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::GUIDED_TRIPLE_SHOT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "death_roulette";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::EXPLODING_ARROW]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=7000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =0;
 	a->m_req_level = 15;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "exploding_arrow";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[Action::EXPLOSION_CASCADE]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=7000;
	a->m_base_action = Action::RANGE_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =0;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::EXPLODING_ARROW;
	a->m_req_ability[1] = Action::VOLLEY_SHOT;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "exploding_cascade";
	a->m_animation[NO_WEAPON].push_back("attackRangedUnarmed");
	a->m_animation[ONE_HANDED].push_back("attackRangedUnarmed");
	a->m_animation[TWO_HANDED].push_back("attackRangedBow");

	a = &(Action::m_base_info[AIMED_SHOT]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=140000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = SELF;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "aimed_shot";

	a = &(Action::m_base_info[BOW_SPIRIT]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=140000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = SELF;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::AIMED_SHOT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "bow_spirit";

	a = &(Action::m_base_info[ICE_ARROWS]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=240000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "ice_arrows";
	a->m_animation[NO_WEAPON].push_back("magicArrow");
	a->m_animation[ONE_HANDED].push_back("magicArrow");
	a->m_animation[TWO_HANDED].push_back("magicArrow");

	a = &(Action::m_base_info[FREEZING_ARROWS]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=240000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::ICE_ARROWS;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "freezing_arrows";
	a->m_animation[NO_WEAPON].push_back("magicArrow");
	a->m_animation[ONE_HANDED].push_back("magicArrow");
	a->m_animation[TWO_HANDED].push_back("magicArrow");

	a = &(Action::m_base_info[WIND_ARROWS]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=240000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "wind_arrows";
	a->m_animation[NO_WEAPON].push_back("magicArrow");
	a->m_animation[ONE_HANDED].push_back("magicArrow");
	a->m_animation[TWO_HANDED].push_back("magicArrow");

	a = &(Action::m_base_info[STORM_ARROWS]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=240000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::WIND_ARROWS;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "storm_arrows";
	a->m_animation[NO_WEAPON].push_back("magicArrow");
	a->m_animation[ONE_HANDED].push_back("magicArrow");
	a->m_animation[TWO_HANDED].push_back("magicArrow");

	a = &(Action::m_base_info[WIND_WALK]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=60000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "wind_walk";

	a = &(Action::m_base_info[Action::EVADE]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "evade";

	a = &(Action::m_base_info[Action::CRITICAL_STRIKE]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::EVADE;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "critical_strike";

	a = &(Action::m_base_info[Action::CONCENTRATION]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::EVADE;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "concentration";

	a = &(Action::m_base_info[Action::MENTAL_WALL]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::CONCENTRATION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "mental_wall";

	a = &(Action::m_base_info[Action::RESIST_ICE]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "resist_ice";

	a = &(Action::m_base_info[Action::RESIST_AIR]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.0;
	a->m_distance = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::RESIST_ICE;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "resist_air";


	// Priester Faehigkeiten
	a = &(Action::m_base_info[HOLY_LIGHT]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = Action::HOLY_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "holy_light";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[HOLY_FIRE]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = Action::HOLY_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::HOLY_LIGHT;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "holy_fire";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[BURNING_RAGE]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=180000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::BLAZING_SHIELD;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "burning_rage";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[CURE_BLIND_MUTE]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=3000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "cure_blind_mute";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[CURE_BLIND_MUTE_PARTY]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=6000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::CURE_BLIND_MUTE;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "cure_blind_mute_party";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[BLAZING_SHIELD]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=160000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "blazing_shield";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[LIGHT_BEAM]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=3000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "light_beam";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[BURNING_SUN]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=7000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::LIGHT_BEAM;
	a->m_req_ability[1] = Action::CURE_BLIND_MUTE;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "burning_sun";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[BREAK_BINDING]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = Action::HOLY_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "break_binding";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[DISRUPT_BINDING]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = Action::HOLY_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::BREAK_BINDING;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "disrupt_binding";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[MAGIC_SHIELD]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=200000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "magic_shield";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[CURE_POIS_BURN]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=3000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::HEAL;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "cure_pois_burn";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[CURE_POIS_BURN_PARTY]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=6000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::ACID;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "cure_pois_burn_party";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[ACID]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=5000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::CURE_POIS_BURN;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "acid";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[HEAL]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=5000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "heal";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[HEAL_PARTY]);
	a->m_timer_nr=2;
	a->m_standard_time=701;
	a->m_timer=10000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::HEAL;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "heal_party";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[DIVINE_WIND]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = Action::HOLY_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "divine_wind";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[DIVINE_STORM]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=6000;
	a->m_base_action = Action::HOLY_ATTACK;
	a->m_critical_perc=0.66;
	a->m_distance = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::DIVINE_WIND;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "divine_storm";
	a->m_animation[NO_WEAPON].push_back("attackUnarmed");
	a->m_animation[ONE_HANDED].push_back("attack");
	a->m_animation[TWO_HANDED].push_back("attackTwoHands");

	a = &(Action::m_base_info[BLADE_STORM]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=160000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "blade_storm";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[CURE_CONF_BSRK]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=3000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::NOACTION;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "cure_conf_bsrk";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[CURE_CONF_BSRK_PARTY]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=6000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = Action::CURE_CONF_BSRK;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "cure_conf_bsrk_party";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[HYPNOSIS]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=4000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = Action::CURE_CONF_BSRK;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "hypnosis";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[HYPNOSIS2]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=7000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = Action::HYPNOSIS;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "hypnosis2";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	a = &(Action::m_base_info[KEEN_MIND]);
	a->m_timer_nr=2;
	a->m_standard_time=701;
	a->m_timer=180000;
	a->m_base_action = Action::NOACTION;
	a->m_critical_perc=0.66;
	a->m_distance = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = Action::BLADE_STORM;
	a->m_req_ability[1] = Action::NOACTION;
	a->m_req_ability[2] = Action::NOACTION;
	a->m_enum_name = "keen_mind";
	a->m_animation[NO_WEAPON].push_back("castMagicStrong");
	a->m_animation[ONE_HANDED].push_back("castMagicStrong");
	a->m_animation[TWO_HANDED].push_back("castMagicStrong");

	for (int i =0;i<192;i++)
	{
		a = &(Action::m_base_info[(ActionType) i]);
		if (a->m_enum_name != "")
		{
			m_enum_string_to_type.insert(make_pair(a->m_enum_name,(ActionType) i));
		}
	}

	updateTranslation();
}




string Action::getName(ActionType type)
{
        return ( gettext((m_base_info[type].m_name).c_str()) );

}


string Action::getDescription(ActionType type)
{
        return ( gettext((m_base_info[type].m_description).c_str()) );

}

Action::ActionType Action::getActionType(std::string name)
{
	std::map<std::string, ActionType>::iterator it;
	it = m_enum_string_to_type.find(name);
	if (it != m_enum_string_to_type.end())
	{
		return it->second;
	}
	return NOACTION;
}

void Action::toString(CharConv* cv)
{

	cv->toBuffer((short) m_type);
	cv->toBuffer(m_time);
	cv->toBuffer((char) m_action_equip);
	cv->toBuffer(m_animation_number);
	cv->toBuffer(m_elapsed_time);


}

void Action::fromString(CharConv* cv)
{
	short tmp;
	char ctmp;
	cv->fromBuffer(tmp);
	m_type = (ActionType) tmp;
	cv->fromBuffer(m_time);
	cv->fromBuffer(ctmp);
	m_action_equip = (ActionEquip) ctmp;
	cv->fromBuffer(m_animation_number);
	cv->fromBuffer(m_elapsed_time);
}

void Action::updateTranslation()
{
	Action::ActionInfo* a;


	a = &(Action::m_base_info[Action::NOACTION]);
	a->m_name = gettext("no action");
	a->m_enum_name = gettext("noaction");

	a = &(Action::m_base_info[Action::DIE]);
	a->m_name = gettext("die");
	a->m_description = gettext("die");

	a = &(Action::m_base_info[Action::WALK]);
	a->m_name = gettext("walk");
	a->m_description = gettext("walk");

	a = &(Action::m_base_info[Action::TAKE_ITEM]);
	a->m_name = gettext("take up item");
	a->m_description = gettext("take item");

	a = &(Action::m_base_info[Action::SPEAK]);
	a->m_name = gettext("speak");
	a->m_description = gettext("speak with someone");

	a = &(Action::m_base_info[Action::ATTACK]);
	a->m_name = gettext("Attack");
	a->m_description = gettext("Basic melee attack");

	a = &(Action::m_base_info[Action::RANGE_ATTACK]);
	a->m_name = gettext("Ranged attack");
	a->m_description = gettext("Basic ranged attack");

	a = &(Action::m_base_info[Action::MAGIC_ATTACK]);
	a->m_name = gettext("Magic attack");
	a->m_description = gettext("Basic magic attack");

	a = &(Action::m_base_info[Action::HOLY_ATTACK]);
	a->m_name = gettext("Holy attack");
	a->m_description = gettext("Basic holy melee attack");

	// Faehigkeiten des Kriegers
	a = &(Action::m_base_info[Action::BASH]);
	a->m_name = gettext("Bash");
	a->m_description = gettext("Attacks one enemy for serious damage.");


	a = &(Action::m_base_info[Action::HAMMER_BASH]);
	a->m_name = gettext("Hammer blow");
	a->m_description = gettext("Attacks one enemy with a brutal blow and damages nearby enemies as well.");

	a = &(Action::m_base_info[Action::AROUND_BLOW]);
	a->m_name = gettext("Sweeping blow");
	a->m_description = gettext("Attacks every enemy in weapon range with slightly reduced damage.");

	a = &(Action::m_base_info[Action::WHIRL_BLOW]);
	a->m_name = gettext("Whirl blow");
	a->m_description = gettext("Attacks every enemy in weapon range.");

	a = &(Action::m_base_info[Action::SMASH]);
	a->m_name = gettext("Smash blow");
	a->m_description = gettext("Attacks an enemy with an unblockable, armor piercing blow.");

	a = &(Action::m_base_info[Action::HATE_MAGE]);
	a->m_name = gettext("Silencer");
	a->m_description = gettext("Attacks an enemy with a chance to inflict silence.");


	a = &(Action::m_base_info[Action::CHARGE]);
	a->m_name = gettext("Charge");
	a->m_description = gettext("Launches an assault at an enemy with increasing speed. The faster the warrior is, the more damage a charge inflicts.");

	a = &(Action::m_base_info[Action::STORM_CHARGE]);
	a->m_name = gettext("Charge scythe");
	a->m_description = gettext("Launches an even stronger charge attack, that may inflict stun.");

	a = &(Action::m_base_info[Action::DECOY]);
	a->m_name = gettext("Taunt");
	a->m_description = gettext("Causes surrounding monsters to attack the warrior more likely.");

	a = &(Action::m_base_info[Action::SCARE]);
	a->m_name = gettext("Defiances");
	a->m_description = gettext("Nearby monsters will attack the warrior, if they are able to. The warriors defence is increased.");

	a = &(Action::m_base_info[Action::FIRESWORD]);
	a->m_name = gettext("Fire and sword");
	a->m_description = gettext("The warrior calls the spirits of fire upon his weapon, causing it to deal firedamage. The damage depends on the warriors spellpower.");

	a = &(Action::m_base_info[Action::FLAMESWORD]);
	a->m_name = gettext("Pact of flames");
	a->m_description = gettext("The warrior calls upon the fires of hell, causing his weapon to deal firedamage for a significant time. His enemies might catch fire when hit.");

	a = &(Action::m_base_info[Action::FLAMEARMOR]);
	a->m_name = gettext("Flame armor");
	a->m_description = gettext("The warrior surrounds himself with angry spirits of fire, who will burn his enemies to ashes, if they dare to come to close.");

	a = &(Action::m_base_info[Action::BERSERK]);
	a->m_name = gettext("Battlecry");
	a->m_description = gettext("The warrior releases a mighty cry that causes his enemies to lose concentration, ultimatly reducing their defence.");

	a = &(Action::m_base_info[Action::WARCRY]);
	a->m_name = gettext("Warcry");
	a->m_description = gettext("The battle hardened warriors scream leaves his enemies defence wide open, sometimes confusing them as well.");

	a = &(Action::m_base_info[Action::REGENERATE]);
	a->m_name = gettext("Regeneration");
	a->m_description = gettext("The warrior regains up to 50 percent hitpoints of his total hitpoints through the sheer power of his will.");

	a = &(Action::m_base_info[Action::ANGER]);
	a->m_name = gettext("Barbarian anger");
	a->m_description = gettext("The warrior sets himself in the state of berserk, attacking his enemies with vicious blows, while completely neglecting his defence.");

	a = &(Action::m_base_info[Action::FURY]);
	a->m_name = gettext("barbarian power");
	a->m_description = gettext("The warrior becomes even more bloodthirsty, launching at his enemies with increased attackspeed and damage in a berserk state. Even while neglecting his defence, the impact is not as big as with the barbarian anger.");

	a = &(Action::m_base_info[Action::STEADFAST]);
	a->m_name = gettext("Perseverance");
	a->m_description = gettext("The warriors defence increases proportional to the cuts and bruises he receives in combat. The more serious the wounds, the more difficult he is to kill.");

	a = &(Action::m_base_info[Action::BLOCK]);
	a->m_name = gettext("Block");
	a->m_description = gettext("The warriors mastery of shields and weapons allows him to block damage at an increased rate.");

	a = &(Action::m_base_info[Action::WEAPONMASTER]);
	a->m_name = gettext("Weaponmaster");
	a->m_description = gettext("The warriors mastery of weapons increases the damage of his basic attacks by 20%.");

	a = &(Action::m_base_info[Action::MONSTER_HUNTER]);
	a->m_name = gettext("Monster hunter");
	a->m_description = gettext("For each monster the warrior kills, his strength increases for a short amount of time.");

	a = &(Action::m_base_info[Action::MONSTER_SLAYER]);
	a->m_name = gettext("Monster slayer");
	a->m_description = gettext("The warrior has become the worst nightmare of monsters, with every one of them falling at his feet increasing his strength and attackspeed for a short amount of time.");

	a = &(Action::m_base_info[Action::ENDURANCE]);
	a->m_name =gettext( "Warmaster");
	a->m_description = gettext("The warrior has become used to the ways of the battlefield to a point, where he can regenerate his abilities faster than normal.");


	// Faehigkeiten des Magiers
	a = &(Action::m_base_info[Action::FIRE_BOLT]);
	a->m_name = gettext("Firebolt");
	a->m_description = gettext("The mage unleashes a small bolt of fire, that damages a single enemy.");

	a = &(Action::m_base_info[Action::FIRE_STRIKE]);
	a->m_name = gettext("Fireblow");
	a->m_description = gettext("The mage casts a strong bolt of fire, that damages one enemy.");

	a = &(Action::m_base_info[Action::FIRE_WAVE]);
	a->m_name = gettext("Flame wave");
	a->m_description = gettext("The mage conjures a wave of flames, that damages enemies in a circle around him.");

	a = &(Action::m_base_info[Action::FIRE_STORM]);
	a->m_name = gettext("Firestorm");
	a->m_description = gettext("The mage conjures a deadly wave of fire, that burns every enemy in a circle around him.");

	a = &(Action::m_base_info[Action::FIRE_BALL]);
	a->m_name = gettext("Fireball");
	a->m_description = gettext("The mage casts a bolt of fire, that explodes upon hitting an enemy. Other enemies nearby the victim are also caught in the explosion.");

	a = &(Action::m_base_info[Action::INFERNO_BALL]);
	a->m_name = gettext("Infernoball");
	a->m_description = gettext("The mage casts an even more powerful fireball.");

	a = &(Action::m_base_info[Action::FIRE_WALL]);
	a->m_name = gettext("Firecolumn");
	a->m_description = gettext("The mage conjurs a column made of hellfire, that will stay for 5 seconds and turn every enemy stupid enough to stay in there to ashes.");

	a = &(Action::m_base_info[Action::INFLAME]);
	a->m_name = gettext("Ignite");
	a->m_description = gettext("The mage embraces the heat of fire, so that victims of his firespells will likely suffer burn.");

	a = &(Action::m_base_info[Action::ICE_BOLT]);
	a->m_name = gettext("Icicle");
	a->m_description = gettext("The mage casts a shard of pure ice, that damages his foes.");


	a = &(Action::m_base_info[Action::ICE_SPIKE]);
	a->m_name = gettext("Spear of ice");
	a->m_description = gettext("The mage attacks one enemy with a powerful spear of magical ice.");

	a = &(Action::m_base_info[Action::SNOW_STORM]);
	a->m_name = gettext("Snow storm");
	a->m_description = gettext("The mage calls upon a snow storm to kill every enemy in a circle.");

	a = &(Action::m_base_info[Action::BLIZZARD]);
	a->m_name = gettext("Blizzard");
	a->m_description = gettext("The mage summons a leathal cold blizzard that freezes every enemy caught in it to death.");

	a = &(Action::m_base_info[Action::ICE_RING]);
	a->m_name = gettext("Ice ring");
	a->m_description = gettext("The mage casts a ring of ice around himself that will damage enemies in range.");

	a = &(Action::m_base_info[Action::FROST_RING]);
	a->m_name = gettext("Frost ring");
	a->m_description = gettext("The mage calls upon a deadlier version of the icering.");

	a = &(Action::m_base_info[Action::FREEZE]);
	a->m_name = gettext("Freeze");
	a->m_description = gettext("The mage catches an enemy in a block of pure ice, instantly sealing his movements.");

	a = &(Action::m_base_info[Action::CHILL]);
	a->m_name = gettext("Chilling cold");
	a->m_description = gettext("The mage has understood everything about ice and snow, allowing him to increase the effectiveness of his icespells by 20%.");

	a = &(Action::m_base_info[Action::LIGHTNING]);
	a->m_name = gettext("Lightning");
	a->m_description = gettext("The mage summons a lightning to attack one foe.");

	a = &(Action::m_base_info[Action::LIGHTNING_STRIKE]);
	a->m_name = gettext("Lightning strike");
	a->m_description = gettext("The mage calls upon a mighty lightning to kill an enemy.");

	a = &(Action::m_base_info[Action::THUNDERSTORM]);
	a->m_name = gettext("Thunderstorm");
	a->m_description = gettext("The mage summons a thunderstorm that will assault enemies in its range with lightnings.");

	a = &(Action::m_base_info[Action::THUNDERSTORM2]);
	a->m_name = gettext("Tempest");
	a->m_description = gettext("The mage summons a ferocious tempest that bombards enemies in its wake with lightnings.");

	a = &(Action::m_base_info[Action::CHAIN_LIGHTNING]);
	a->m_name = gettext("Chainlightning");
	a->m_description = gettext("The mage unleashes a ball of lightning, that may jump from one enemy to another. It hits a maximum of five enemies and the damage is reduced everytime it jumps.");

	a = &(Action::m_base_info[Action::CHAIN_LIGHTNING2]);
	a->m_name = gettext("Ball lightning");
	a->m_description = gettext("The mage unleashes a mighty ball lightning, that can jump over to nearby enemies. A maximum of nine enemies is hit and the damage is reduced with every jump.");

	a = &(Action::m_base_info[Action::STATIC_SHIELD]);
	a->m_name = gettext("Static shield");
	a->m_description = gettext("The mage surrounds himself with an armor of lightning, that will stun and damage enemies who are bolt enough to attack in close combat.");

	a = &(Action::m_base_info[Action::IONISATION]);
	a->m_name = gettext("Ionisation");
	a->m_description = gettext("The mage has become one with the storm, leading to 20 percent extra damage from his lightning spells.");


	// Schuetze Faehigkeiten
	a = &(Action::m_base_info[Action::TRIPLE_SHOT]);
	a->m_name = gettext("Tripple Shot");
	a->m_description = gettext("The archers mastery of ranged weapons allows him to shoot three projectiles in a short amount of time.");


	a = &(Action::m_base_info[Action::GUIDED_TRIPLE_SHOT]);
	a->m_name = gettext("Deadly Swarm");
	a->m_description = gettext("The archer shoots four consecutive projectiles that are imbued with vengeful spirits. They will pursue the enemy, even if he tries to run.");

	a = &(Action::m_base_info[Action::MULTISHOT]);
	a->m_name = gettext("Fanshot");
	a->m_description = gettext("The archer shoots five projectiles at a time, aimed at enemies in front of him. The damage dealt by each projectile suffers a little.");

	a = &(Action::m_base_info[Action::VOLLEY_SHOT]);
	a->m_name = gettext("Iron Hail");
	a->m_description = gettext("The archer handles his weapons with elflike precision, attacking up to seven enemys in front of him. The damage dealt by each projectile suffers a little.");

	a = &(Action::m_base_info[Action::PIERCE]);
	a->m_name = gettext("Elfwanderer");
	a->m_description = gettext("The archer unleashes a mighty shot that pierces right through an enemy and might hit another enemy behind him.");

	a = &(Action::m_base_info[Action::WEAK_POINT]);
	a->m_name = gettext("Weak Spot");
	a->m_description = gettext("The archer attacks a weak point of his enemy, causing the victim to go berserk.");

	a = &(Action::m_base_info[Action::BLIND_RAGE]);
	a->m_name = gettext("Blind Rage");
	a->m_description = gettext("The archer attacks the enemies eyes, causing him to go berserk, but be blinded at the same time.");

	a = &(Action::m_base_info[Action::VACUUM]);
	a->m_name = gettext("Vacuum");
	a->m_description = gettext("The archer attacks with an arrow imbued with a weaker windspirit, that causes an enemy hit by it to be stunned.");

	a = &(Action::m_base_info[Action::DEATH_ROULETTE]);
	a->m_name = gettext("Deadly Roulette");
	a->m_description = gettext("The projectile has a fifty percent probability to attack another target, if it hits someone. Be careful not to stand in its way, for it distinguishes not between friend and foe.");

	a = &(Action::m_base_info[Action::EXPLODING_ARROW]);
	a->m_name = gettext("Explosion Arrow");
	a->m_description = gettext("The projectile is imbued with the power of a weak firespirit. It will deal damage in a small area, if it hits.");

	a = &(Action::m_base_info[Action::EXPLOSION_CASCADE]);
	a->m_name = gettext("Blast Cascade");
	a->m_description = gettext("The projectile is imbued with the power of a firespirit. It will explode with areadamage on hitting. Additionally four more explosions will go of in every direction.");

	a = &(Action::m_base_info[AIMED_SHOT]);
	a->m_name = gettext("Snipe");
	a->m_description = gettext("The archers deadly precision causes his shots to deal more damage than normal.");

	a = &(Action::m_base_info[BOW_SPIRIT]);
	a->m_name = gettext("Bow Spirit");
	a->m_description = gettext("The archer becomes one with his weapon, dealing more damage over a longer time. Additionally his critical strike chance is increased. This effect stacks with Elfeyes.");

	a = &(Action::m_base_info[ICE_ARROWS]);
	a->m_name = gettext("Ice Arrows");
	a->m_description = gettext("The archer calls upon the spirits of ice to imbue his projectiles with icedamage.");

	a = &(Action::m_base_info[FREEZING_ARROWS]);
	a->m_name = gettext("Frost Arrows");
	a->m_description = gettext("The archer summons a spirit of ice into his weapon, causing his shots to deal icedamage over a greater amount of time. There is a chance that his enemies will be frozen solid if they are hit.");

	a = &(Action::m_base_info[WIND_ARROWS]);
	a->m_name = gettext("Wind Arrows");
	a->m_description = gettext("The archer has been grantet the power to imbue his projectiles with winddamage by the spirits of wind.");

	a = &(Action::m_base_info[STORM_ARROWS]);
	a->m_name = gettext("Storm Arrows");
	a->m_description = gettext("The mighty spirits of wind themselfes help the archer by imbuing his projectiles with their power, causing them to deal more winddamage for a longer time.");

	a = &(Action::m_base_info[WIND_WALK]);
	a->m_name = gettext("Windwalk");
	a->m_description = gettext("The spirits of wind help the archer to move faster than normal and avoid damage.");

	a = &(Action::m_base_info[Action::EVADE]);
	a->m_name = gettext("Catlike Reflexes");
	a->m_description = gettext("The archers nimble mind allows him to avoid damage better.");

	a = &(Action::m_base_info[Action::CRITICAL_STRIKE]);
	a->m_name = gettext("Elfeyes");
	a->m_description = gettext("The archer shoots with deadly precision, leading to an increased critical hit rate.");

	a = &(Action::m_base_info[Action::CONCENTRATION]);
	a->m_name = gettext("Concentration");
	a->m_description = gettext("The archers steady mind can no longer be confused.");

	a = &(Action::m_base_info[Action::MENTAL_WALL]);
	a->m_name = gettext("Mind Wall");
	a->m_description = gettext("The archer has become immune to anger. He will never go berserk again.");

	a = &(Action::m_base_info[Action::RESIST_ICE]);
	a->m_name = gettext("North Roamer");
	a->m_description = gettext("The archer has made a pact with the spirits of wind and ice. His resistance against wind- and icedamage is increased by 20 percent.");

	a = &(Action::m_base_info[Action::RESIST_AIR]);
	a->m_name = gettext("North Hunter");
	a->m_description = gettext("The archers good longterm relations with the spirits of wind and ice has caused his resistence and his maximum resistance of these elements to rise by 10 percent.");


	// Priester Faehigkeiten
	a = &(Action::m_base_info[HOLY_LIGHT]);
	a->m_name = gettext("Bright Blow");
	a->m_description = gettext("The priest attacks an enemy with a blow imbued with holy power. Undead will be severely damaged by the sacred energies.");

	a = &(Action::m_base_info[HOLY_FIRE]);
	a->m_name = gettext("Sanctified Strike");
	a->m_description = gettext("The priest attacks with godly power, easily sending undead creatures back to their graves.");

	a = &(Action::m_base_info[BURNING_RAGE]);
	a->m_name = gettext("Burning Rage");
	a->m_description = gettext("The priest inflicts holy anger in his partymembers. They will attack with greater power. With increasing willpower the priest can hold this effect longer.");

	a = &(Action::m_base_info[CURE_BLIND_MUTE]);
	a->m_name = gettext("Gentle Glow");
	a->m_description = gettext("The priest bathes a partymember in a mild light, releaving him of blindness and silence.");

	a = &(Action::m_base_info[CURE_BLIND_MUTE_PARTY]);
	a->m_name = gettext("Omniscent Eye");
	a->m_description = gettext("The party will be immune to blind and berserk for some time. With increasing willpower the priest can hold this effect longer.");

	a = &(Action::m_base_info[BLAZING_SHIELD]);
	a->m_name = gettext("Blazing Shield");
	a->m_description = gettext("The party is surrounded by holy energy. Everyones defense is increased. With increasing willpower the priest can hold this effect longer.");

	a = &(Action::m_base_info[LIGHT_BEAM]);
	a->m_name = gettext("Glistening Glare");
	a->m_description = gettext("The priest casts a dazzling light, that can cause an enemy to be affected by blind.");

	a = &(Action::m_base_info[BURNING_SUN]);
	a->m_name = gettext("Glaring Sun");
	a->m_description = gettext("The priest calls forth a mighty explosion of light that can make every enemy in an area to be affected by blind.");

	a = &(Action::m_base_info[BREAK_BINDING]);
	a->m_name = gettext("Breaking Blow");
	a->m_description = gettext("The priest attacks an enemy with energy that causes elementals to be blown back where they come from.");

	a = &(Action::m_base_info[DISRUPT_BINDING]);
	a->m_name = gettext("Dismissing Strike");
	a->m_description = gettext("The priest strikes with all his might. While normal monsters have little to fear from this attack elementals will be easily destroyed by it.");

	a = &(Action::m_base_info[MAGIC_SHIELD]);
	a->m_name = gettext("Magic Shield");
	a->m_description = gettext("The priests protecting influence raises the willpower of all partymembers temporarily. With increasing willpower the priest can hold this effect longer.");

	a = &(Action::m_base_info[CURE_POIS_BURN]);
	a->m_name = gettext("Ease Pain");
	a->m_description = gettext("The priest treats a partymemener with healing water that washes away every poison and stops burns.");

	a = &(Action::m_base_info[CURE_POIS_BURN_PARTY]);
	a->m_name = gettext("Remedial Water");
	a->m_description = gettext("The priests grants every partymember immunity to poison and burn for a small amount of time while healing these effects. With increasing willpower the priest can hold this effect longer.");

	a = &(Action::m_base_info[ACID]);
	a->m_name = gettext("Acid");
	a->m_description = gettext("The priest turns normal water into deadly acid, poisoning one enemy.");

	a = &(Action::m_base_info[HEAL]);
	a->m_name = gettext("Cure");
	a->m_description = gettext("The priest restores the hitpoints of one partymemeber.");

	a = &(Action::m_base_info[HEAL_PARTY]);
	a->m_name = gettext("Grand Cure");
	a->m_description = gettext("The priest restores hitpoints for every partymember.");

	a = &(Action::m_base_info[DIVINE_WIND]);
	a->m_name = gettext("Windstrike");
	a->m_description = gettext("The priest creates a vortex with his attack, that can suck a demonic creature right back to hell.");

	a = &(Action::m_base_info[DIVINE_STORM]);
	a->m_name = gettext("God Strike");
	a->m_description = gettext("Only strong and durable demons will be able to resist the priests sacred strike.");

	a = &(Action::m_base_info[BLADE_STORM]);
	a->m_name = gettext("Bladestorm");
	a->m_description = gettext("The priest strengthens the weaponarms of his comrades, leading to increased attackspeed for everyone. With increasing willpower the priest can hold this effect longer.");

	a = &(Action::m_base_info[CURE_CONF_BSRK]);
	a->m_name = gettext("Restore Concentration");
	a->m_description = gettext("The priest heals one friend from confusion and berserk.");

	a = &(Action::m_base_info[CURE_CONF_BSRK_PARTY]);
	a->m_name = gettext("Spirit Shield");
	a->m_description = gettext("The priest grants immunity to berserk and confuse to all his partymembers for a small amount of time. With increasing willpower the priest can hold this effect longer.");

	a = &(Action::m_base_info[HYPNOSIS]);
	a->m_name = gettext("Hypnosis");
	a->m_description = gettext("The priest confuses a monster, causing it to attack at random.");

	a = &(Action::m_base_info[HYPNOSIS2]);
	a->m_name = gettext("Hypnotic Vortex");
	a->m_description = gettext("The priest creates a distracting vortex with the ability to confuse every enemy caught in it.");

	a = &(Action::m_base_info[KEEN_MIND]);
	a->m_name = gettext("Keen Mind");
	a->m_description = gettext("The priest increases everyones spellpower for a short time. Everyones spells will surely be more effective. With increasing willpower the priest can hold this effect longer.");
}

