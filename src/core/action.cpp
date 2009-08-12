#include "action.h"
#include "elementattrib.h"

#include "eventsystem.h"
#include "projectile.h"

std::map<Action::ActionType,Action::ActionInfo> Action::m_action_info;

void Action::init()
{
/*	
	DEBUG5("initialising actions");
	Action::ActionInfo* a;


	a = &(Action::m_action_info["noaction"]);
	a->m_timer_nr=0;
	a->m_standard_time=2000;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=-1;
	a->m_target_type = SELF;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0]= "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "no action";

	a = &(Action::m_action_info["use"]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.5;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0]= "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "use";

	a = &(Action::m_action_info["die"]);
	a->m_timer_nr=0;
	a->m_standard_time=1000;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=-1;
	a->m_target_type = SELF;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0]= "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "die";
	a->m_description = "die";
	
	a = &(Action::m_action_info["dead"]);
	a->m_timer_nr=0;
	a->m_standard_time=1000;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=-1;
	a->m_target_type = SELF;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0]= "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "dead";
	a->m_description = "dead";


	a = &(Action::m_action_info["walk"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "walk";
	a->m_critical_perc=-1;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "walk";
	a->m_description = "walk";

	a = &(Action::m_action_info["take_item"]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.7;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "take up item";
	a->m_description = "take item";
	
	a = &(Action::m_action_info["speak"]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.7;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "speak";
	a->m_description = "speak with someone";
	
	a = &(Action::m_action_info["attack"]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = "attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Attack";
	a->m_description = "Basic melee attack";

	a = &(Action::m_action_info["range_attack"]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Ranged attack";
	a->m_description = "Basic ranged attack";


	a = &(Action::m_action_info["magic_attack"]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.5;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Magic attack";
	a->m_description = "Basic magic attack";


	a = &(Action::m_action_info["holy_attack"]);
	a->m_timer_nr=0;
	a->m_standard_time=1;
	a->m_timer=0;
	a->m_base_action = "holy_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 0;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Holy attack";
	a->m_description = "Basic holy melee attack";


	// Faehigkeiten des Kriegers
	a = &(Action::m_action_info["bash"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=2000;
	a->m_base_action = "attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Bash";
	a->m_description = "Attacks one enemy for serious damage.";



	a = &(Action::m_action_info["hammer_bash"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=2000;
	a->m_base_action = "attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "around_blow";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Hammer blow";
	a->m_description = "Attacks one enemy with a brutal blow and damages nearby enemies as well.";


	a = &(Action::m_action_info["around_blow"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = "attack";
	a->m_critical_perc=0.8;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = "bash";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Sweeping blow";
	a->m_description = "Attacks every enemy in weapon range with slightly reduced damage.";

	
	
	a = &(Action::m_action_info["whirl_blow"]);
	a->m_timer_nr=1;
	a->m_standard_time=1.5;
	a->m_timer=3000;
	a->m_base_action = "attack";
	a->m_critical_perc=0.8;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = "around_blow";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Whirl blow";
	a->m_description = "Attacks every enemy in weapon range.";

	
	a = &(Action::m_action_info["smash"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = "attack";
	a->m_critical_perc=0.8;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = "hammer_bash";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Smash blow";
	a->m_description = "Attacks an enemy with an unblockable, armor piercing blow.";

	
	a = &(Action::m_action_info["hate_mage"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = "attack";
	a->m_critical_perc=0.8;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = "bash";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Silencer";
	a->m_description = "Attacks an enemy with a chance to inflict silence.";


	a = &(Action::m_action_info["charge"]);
	a->m_timer_nr=1;
	a->m_standard_time=0.5;
	a->m_timer=6000;
	a->m_base_action = "attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "bash";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Charge";
	a->m_description = "Launches an assault at an enemy with increasing speed. The faster the warrior is, the more damage a charge inflicts.";
	
	a = &(Action::m_action_info["storm_charge"]);
	a->m_timer_nr=1;
	a->m_standard_time=0.5;
	a->m_timer=6000;
	a->m_base_action = "attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =0;
	a->m_req_level = 60;
	a->m_req_ability[0] = "charge";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Charge scythe";
	a->m_description = "Launches an even stronger charge attack, that may inflict stun.";

	
	a = &(Action::m_action_info["decoy"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=150000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Taunt";
	a->m_description = "Causes surrounding monsters to attack the warrior more likely.";

	a = &(Action::m_action_info["scare"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=150000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "berserk";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Defiances";
	a->m_description = "Nearby monsters will attack the warrior, if they are able to. The warriors defence is increased.";
	
	a = &(Action::m_action_info["firesword"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=240000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Fire and sword";
	a->m_description = "The warrior calls the spirits of fire upon his weapon, causing it to deal firedamage. The damage depends on the warriors spellpower.";
	
	a = &(Action::m_action_info["flamesword"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=240000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "flamearmor";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Pact of flames";
	a->m_description = "The warrior calls upon the fires of hell, causing his weapon to deal firedamage for a significant time. His enemies might catch fire when hit.";
	
	a = &(Action::m_action_info["flamearmor"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=300000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "firesword";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Flame armor";
	a->m_description = "The warrior surrounds himself with angry spirits of fire, who will burn his enemies to ashes, if they dare to come to close.";
	
	a = &(Action::m_action_info["berserk"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=200000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "decoy";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Battlecry";
	a->m_description = "The warrior releases a mighty cry that causes his enemies to lose concentration, ultimatly reducing their defence.";
	
	a = &(Action::m_action_info["warcry"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=200000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = "scare";
	a->m_req_ability[1] = "fury";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Warcry";
	a->m_description = "The battle hardened warriors scream leaves his enemies defence wide open, sometimes confusing them as well.";
	
	a = &(Action::m_action_info["regenerate"]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=200000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Regeneration";
	a->m_description = "The warrior regains up to 50 percent hitpoints of his total hitpoints through the sheer power of his will.";
	
	a = &(Action::m_action_info["anger"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=300000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Barbarian anger";
	a->m_description = "The warrior sets himself in the state of berserk, attacking his enemies with vicious blows, while completely neglecting his defence.";
	
	a = &(Action::m_action_info["fury"]);
	a->m_timer_nr=2;
	a->m_standard_time=500;
	a->m_timer=200000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = "anger";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "barbarian power";
	a->m_description = "The warrior becomes even more bloodthirsty, launching at his enemies with increased attackspeed and damage in a berserk state. Even while neglecting his defence, the impact is not as big as with the barbarian anger.";
	
	a = &(Action::m_action_info["steadfast"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Perseverance";
	a->m_description = "The warriors defence increases proportional to the cuts and bruises he receives in combat. The more serious the wounds, the more difficult he is to kill.";
	
	a = &(Action::m_action_info["block"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = "steadfast";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Block";
	a->m_description = "The warriors mastery of shields and weapons allows him to block damage at an increased rate.";
	
	a = &(Action::m_action_info["weaponmaster"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "block";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Weaponmaster";
	a->m_description = "The warriors mastery of weapons increases the damage of his basic attacks by 20%.";
	
	a = &(Action::m_action_info["monster_hunter"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Monster hunter";
	a->m_description = "For each monster the warrior kills, his strength increases for a short amount of time.";
	
	a = &(Action::m_action_info["monster_slayer"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = "monster_hunter";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Monster slayer";
	a->m_description = "The warrior has become the worst nightmare of monsters, with every one of them falling at his feet increasing his strength and attackspeed for a short amount of time.";
	
	a = &(Action::m_action_info["endurance"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 60;
	a->m_req_ability[0] = "monster_slayer";
	a->m_req_ability[1] = "weaponmaster";
	a->m_req_ability[2] = "noaction";
	a->m_name ="Warmaster";
	a->m_description = "The warrior has become used to the ways of the battlefield to a point, where he can regenerate his abilities faster than normal.";

	// Faehigkeiten des Magiers
	a = &(Action::m_action_info["fire_bolt"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=2500;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Firebolt";
	a->m_description = "The mage unleashes a small bolt of fire, that damages a single enemy.";
	
	a = &(Action::m_action_info["fire_strike"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=2500;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "fire_bolt";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Fireblow";
	a->m_description = "The mage casts a strong bolt of fire, that damages one enemy.";
	
	a = &(Action::m_action_info["fire_wave"]);
	a->m_timer_nr=2;
	a->m_standard_time=1301;
	a->m_timer=15000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "inferno_ball";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Flame wave";
	a->m_description = "The mage conjures a wave of flames, that damages enemies in a circle around him.";
	
	a = &(Action::m_action_info["fire_storm"]);
	a->m_timer_nr=2;
	a->m_standard_time=1301;
	a->m_timer=15000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = "fire_wave";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Firestorm";
	a->m_description = "The mage conjures a deadly wave of fire, that burns every enemy in a circle around him.";
	
	a = &(Action::m_action_info["fire_ball"]);
	a->m_timer_nr=1;
	a->m_standard_time=1001;
	a->m_timer=6000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "fire_bolt";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Fireball";
	a->m_description = "The mage casts a bolt of fire, that explodes upon hitting an enemy. Other enemies nearby the victim are also caught in the explosion.";
	
	a = &(Action::m_action_info["inferno_ball"]);
	a->m_timer_nr=1;
	a->m_standard_time=1001;
	a->m_timer=6000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "fire_ball";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Infernoball";
	a->m_description = "The mage casts an even more powerful fireball.";
	
	a = &(Action::m_action_info["fire_wall"]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=12000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "fire_strike";
	a->m_req_ability[1] = "fire_ball";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Firecolumn";
	a->m_description = "The mage conjurs a column made of hellfire, that will stay for 5 seconds and turn every enemy stupid enough to stay in there to ashes.";
	
	a = &(Action::m_action_info["inflame"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Ignite";
	a->m_description = "The mage embraces the heat of fire, so that victims of his firespells will likely suffer burn.";
	
	a = &(Action::m_action_info[ "ice_bolt"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=3000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Icicle";
	a->m_description = "The mage casts a shard of pure ice, that damages his foes.";

	a = &(Action::m_action_info["ice_spike"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=3000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "ice_bolt";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Spear of ice";
	a->m_description = "The mage attacks one enemy with a powerful spear of magical ice.";
	
	a = &(Action::m_action_info["snow_storm"]);
	a->m_timer_nr=2;
	a->m_standard_time=1401;
	a->m_timer=14000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "freeze";
	a->m_req_ability[1] = "ice_ring";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Snow storm";
	a->m_description = "The mage calls upon a snow storm to kill every enemy in a circle.";
	
	a = &(Action::m_action_info["blizzard"]);
	a->m_timer_nr=2;
	a->m_standard_time=1401;
	a->m_timer=14000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = "frost_ring";
	a->m_req_ability[1] = "snow_storm";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Blizzard";
	a->m_description = "The mage summons a leathal cold blizzard that freezes every enemy caught in it to death.";
	
	a = &(Action::m_action_info["ice_ring"]);
	a->m_timer_nr=1;
	a->m_standard_time=1001;
	a->m_timer=7000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "ice_bolt";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Ice ring";
	a->m_description = "The mage casts a ring of ice around himself that will damage enemies in range.";
	
	a = &(Action::m_action_info["frost_ring"]);
	a->m_timer_nr=1;
	a->m_standard_time=1001;
	a->m_timer=7000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "ice_ring";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Frost ring";
	a->m_description = "The mage calls upon a deadlier version of the icering.";
	
	a = &(Action::m_action_info["freeze"]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=11000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "ice_bolt";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Freeze";
	a->m_description = "The mage catches an enemy in a block of pure ice, instantly sealing his movements.";
	
	a = &(Action::m_action_info["chill"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Chilling cold";
	a->m_description = "The mage has understood everything about ice and snow, allowing him to increase the effectiveness of his icespells by 20%.";
	
	a = &(Action::m_action_info["lightning"]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=2000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Lightning";
	a->m_description = "The mage summons a lightning to attack one foe.";
	
	a = &(Action::m_action_info["lightning_strike"]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=2000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "lightning";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Lightning strike";
	a->m_description = "The mage calls upon a mighty lightning to kill an enemy.";

	
	a = &(Action::m_action_info["thunderstorm"]);
	a->m_timer_nr=2;
	a->m_standard_time=1301;
	a->m_timer=12000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "chain_lightning";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Thunderstorm";
	a->m_description = "The mage summons a thunderstorm that will assault enemies in its range with lightnings.";
	
	a = &(Action::m_action_info["thunderstorm2"]);
	a->m_timer_nr=2;
	a->m_standard_time=1301;
	a->m_timer=12000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] ="thunderstorm";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Tempest";
	a->m_description = "The mage summons a ferocious tempest that bombards enemies in its wake with lightnings.";
	
	a = &(Action::m_action_info["chain_lightning"]);
	a->m_timer_nr=1;
	a->m_standard_time=801;
	a->m_timer=7000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "lightning";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Chainlightning";
	a->m_description = "The mage unleashes a ball of lightning, that may jump from one enemy to another. It hits a maximum of five enemies and the damage is reduced everytime it jumps.";
	
	a = &(Action::m_action_info["chain_lightning2"]);
	a->m_timer_nr=1;
	a->m_standard_time=801;
	a->m_timer=7000;
	a->m_base_action = "magic_attack";
	a->m_critical_perc=0.8;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "chain_lightning";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Ball lightning";
	a->m_description = "The mage unleashes a mighty ball lightning, that can jump over to nearby enemies. A maximum of nine enemies is hit and the damage is reduced with every jump.";
	
	a = &(Action::m_action_info["static_shield"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=40000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.8;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "ionisation";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Static shield";
	a->m_description = "The mage surrounds himself with an armor of lightning, that will stun and damage enemies who are bolt enough to attack in close combat.";
	
	a = &(Action::m_action_info["ionisation"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Ionisation";
	a->m_description = "The mage has become one with the storm, leading to 20 percent extra damage from his lightning spells.";

	
	// Schuetze Faehigkeiten
	a = &(Action::m_action_info["triple_shot"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.3;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Tripple Shot";
	a->m_description = "The archers mastery of ranged weapons allows him to shoot three projectiles in a short amount of time.";

	a = &(Action::m_action_info["guided_triple_shot"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.3;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "multishot";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Deadly Swarm";
	a->m_description = "The archer shoots four consecutive projectiles that are imbued with vengeful spirits. They will pursue the enemy, even if he tries to run.";
	
	a = &(Action::m_action_info["multishot"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=5000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.7;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = "triple_shot";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Fanshot";
	a->m_description = "The archer shoots five projectiles at a time, aimed at enemies in front of him. The damage dealt by each projectile suffers a little.";
	
	a = &(Action::m_action_info["volley_shot"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=5000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.7;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = "guided_triple_shot";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Iron Hail";
	a->m_description = "The archer handles his weapons with elflike precision, attacking up to seven enemys in front of him. The damage dealt by each projectile suffers a little.";
	
	a = &(Action::m_action_info["pierce"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=2000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Elfwanderer";
	a->m_description = "The archer unleashes a mighty shot that pierces right through an enemy and might hit another enemy behind him.";
	
	a = &(Action::m_action_info["weak_point"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Weak Spot";
	a->m_description = "The archer attacks a weak point of his enemy, causing the victim to go berserk.";
	
	a = &(Action::m_action_info["blind_rage"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "weak_point";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Blind Rage";
	a->m_description = "The archer attacks the enemies eyes, causing him to go berserk, but be blinded at the same time.";

	
	a = &(Action::m_action_info["vacuum"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=6000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Vacuum";
	a->m_description = "The archer attacks with an arrow imbued with a weaker windspirit, that causes an enemy hit by it to be stunned.";

	
	a = &(Action::m_action_info["death_roulette"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = "guided_triple_shot";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Deadly Roulette";
	a->m_description = "The projectile has a fifty percent probability to attack another target, if it hits someone. Be careful not to stand in its way, for it distinguishes not between friend and foe.";
	
	a = &(Action::m_action_info[ "exploding_arrow"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=7000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =0;
 	a->m_req_level = 15;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Explosion Arrow";
	a->m_description = "The projectile is imbued with the power of a weak firespirit. It will deal damage in a small area, if it hits.";
	
	a = &(Action::m_action_info["exploding_cascade"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=7000;
	a->m_base_action = "range_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =0;
	a->m_req_level = 60;
	a->m_req_ability[0] = "exploding_arrow";
	a->m_req_ability[1] = "volley_shot";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Blast Cascade";
	a->m_description = "The projectile is imbued with the power of a firespirit. It will explode with areadamage on hitting. Additionally four more explosions will go of in every direction.";
	
	a = &(Action::m_action_info["aimed_shot"]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=140000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = SELF;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Snipe";
	a->m_description = "The archers deadly precision causes his shots to deal more damage than normal.";
	
	a = &(Action::m_action_info["bow_spirit"]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=140000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = SELF;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = "aimed_shot";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Bow Spirit";
	a->m_description = "The archer becomes one with his weapon, dealing more damage over a longer time. Additionally his critical strike chance is increased. This effect stacks with Elfeyes.";
	
	a = &(Action::m_action_info["ice_arrows"]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=240000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Ice Arrows";
	a->m_description = "The archer calls upon the spirits of ice to imbue his projectiles with icedamage.";
	
	a = &(Action::m_action_info["freezing_arrows"]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=240000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = "ice_arrows";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Frost Arrows";
	a->m_description = "The archer summons a spirit of ice into his weapon, causing his shots to deal icedamage over a greater amount of time. There is a chance that his enemies will be frozen solid if they are hit.";
	
	a = &(Action::m_action_info["wind_arrows"]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=240000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Wind Arrows";
	a->m_description = "The archer has been grantet the power to imbue his projectiles with winddamage by the spirits of wind.";
	
	a = &(Action::m_action_info["storm_arrows"]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=240000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "wind_arrows";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Storm Arrows";
	a->m_description = "The mighty spirits of wind themselfes help the archer by imbuing his projectiles with their power, causing them to deal more winddamage for a longer time.";
	
	a = &(Action::m_action_info["wind_walk"]);
	a->m_timer_nr=2;
	a->m_standard_time=801;
	a->m_timer=60000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = SELF;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Windwalk";
	a->m_description = "The spirits of wind help the archer to move faster than normal and avoid damage.";
	
	a = &(Action::m_action_info["evade"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Catlike Reflexes";
	a->m_description = "The archers nimble mind allows him to avoid damage better.";
	
	a = &(Action::m_action_info["critical_strike"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = "evade";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Elfeyes";
	a->m_description = "The archer shoots with deadly precision, leading to an increased critical hit rate.";
	
	a = &(Action::m_action_info["concentration"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 15;
	a->m_req_ability[0] = "evade";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Concentration";
	a->m_description = "The archers steady mind can no longer be confused.";
	
	a = &(Action::m_action_info["mental_wall"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 60;
	a->m_req_ability[0] = "concentration";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Mind Wall";
	a->m_description = "The archer has become immune to anger. He will never go berserk again.";
	
	a = &(Action::m_action_info["resist_ice"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "North Roamer";
	a->m_description = "The archer has made a pact with the spirits of wind and ice. His resistance against wind- and icedamage is increased by 20 percent.";
	
	a = &(Action::m_action_info["resist_air"]);
	a->m_timer_nr=0;
	a->m_standard_time=0;
	a->m_timer=0;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.0;
	a->m_target_type = PASSIVE;
	a->m_flags =0;
	a->m_req_level = 35;
	a->m_req_ability[0] = "resist_ice";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "North Hunter";
	a->m_description = "The archers good longterm relations with the spirits of wind and ice has caused his resistence and his maximum resistance of these elements to rise by 10 percent.";

	
	// Priester Faehigkeiten
	a = &(Action::m_action_info["holy_light"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = "holy_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Bright Blow";
	a->m_description = "The priest attacks an enemy with a blow imbued with holy power. Undead will be severely damaged by the sacred energies.";
	
	a = &(Action::m_action_info["holy_fire"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = "holy_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "holy_light";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Sanctified Strike";
	a->m_description = "The priest attacks with godly power, easily sending undead creatures back to their graves.";
	
	a = &(Action::m_action_info["burning_rage"]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=180000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "blazing_shield";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Burning Rage";
	a->m_description = "The priest inflicts holy anger in his partymembers. They will attack with greater power. With increasing willpower the priest can hold this effect longer.";

	a = &(Action::m_action_info["cure_blind_mute"]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=3000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Gentle Glow";
	a->m_description = "The priest bathes a partymember in a mild light, releaving him of blindness and silence.";

	a = &(Action::m_action_info["cure_blind_mute_party"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=6000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "cure_blind_mute";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Omniscent Eye";
	a->m_description = "The party will be immune to blind and berserk for some time. With increasing willpower the priest can hold this effect longer.";
	
	a = &(Action::m_action_info["blazing_shield"]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=160000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Blazing Shield";
	a->m_description = "The party is surrounded by holy energy. Everyones defense is increased. With increasing willpower the priest can hold this effect longer.";
	
	a = &(Action::m_action_info["light_beam"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=3000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Glistening Glare";
	a->m_description = "The priest casts a dazzling light, that can cause an enemy to be affected by blind.";
	
	a = &(Action::m_action_info["burning_sun"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=7000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = "light_beam";
	a->m_req_ability[1] = "cure_blind_mute";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Glaring Sun";
	a->m_description = "The priest calls forth a mighty explosion of light that can make every enemy in an area to be affected by blind.";

	
	a = &(Action::m_action_info["break_binding"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = "holy_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Breaking Blow";
	a->m_description = "The priest attacks an enemy with energy that causes elementals to be blown back where they come from.";
	
	a = &(Action::m_action_info["disrupt_binding"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=4000;
	a->m_base_action = "holy_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = "break_binding";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Dismissing Strike";
	a->m_description = "The priest strikes with all his might. While normal monsters have little to fear from this attack elementals will be easily destroyed by it.";
	
	a = &(Action::m_action_info["magic_shield"]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=200000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Magic Shield";
	a->m_description = "The priests protecting influence raises the willpower of all partymembers temporarily. With increasing willpower the priest can hold this effect longer.";
	
	a = &(Action::m_action_info["cure_pois_burn"]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=3000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "heal";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Ease Pain";
	a->m_description = "The priest treats a partymemener with healing water that washes away every poison and stops burns.";
	
	a = &(Action::m_action_info["cure_pois_burn_party"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=6000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "acid";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Remedial Water";
	a->m_description = "The priests grants every partymember immunity to poison and burn for a small amount of time while healing these effects. With increasing willpower the priest can hold this effect longer.";
	
	a = &(Action::m_action_info["acid"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=5000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "cure_pois_burn";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Acid";
	a->m_description = "The priest turns normal water into deadly acid, poisoning one enemy.";

	a = &(Action::m_action_info["heal"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=5000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Cure";
	a->m_description = "The priest restores the hitpoints of one partymemeber.";

	a = &(Action::m_action_info["heal_party"]);
	a->m_timer_nr=2;
	a->m_standard_time=701;
	a->m_timer=10000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "heal";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Grand Cure";
	a->m_description = "The priest restores hitpoints for every partymember.";

	a = &(Action::m_action_info["divine_wind"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=3000;
	a->m_base_action = "holy_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Windstrike";
	a->m_description = "The priest creates a vortex with his attack, that can suck a demonic creature right back to hell.";

	a = &(Action::m_action_info["divine_storm"]);
	a->m_timer_nr=1;
	a->m_standard_time=1;
	a->m_timer=6000;
	a->m_base_action = "holy_attack";
	a->m_critical_perc=0.66;
	a->m_target_type = MELEE;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "divine_wind";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "God Strike";
	a->m_description = "Only strong and durable demons will be able to resist the priests sacred strike.";

	a = &(Action::m_action_info["blade_storm"]);
	a->m_timer_nr=2;
	a->m_standard_time=1001;
	a->m_timer=160000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 1;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Bladestorm";
	a->m_description = "The priest strengthens the weaponarms of his comrades, leading to increased attackspeed for everyone. With increasing willpower the priest can hold this effect longer.";

	a = &(Action::m_action_info["cure_conf_bsrk"]);
	a->m_timer_nr=1;
	a->m_standard_time=501;
	a->m_timer=3000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "noaction";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Restore Concentration";
	a->m_description = "The priest heals one friend from confusion and berserk.";

	a = &(Action::m_action_info["cure_conf_bsrk_party"]);
	a->m_timer_nr=2;
	a->m_standard_time=501;
	a->m_timer=6000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 35;
	a->m_req_ability[0] = "cure_conf_bsrk";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Spirit Shield";
	a->m_description = "The priest grants immunity to berserk and confuse to all his partymembers for a small amount of time. With increasing willpower the priest can hold this effect longer.";

	a = &(Action::m_action_info["hypnosis"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=4000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 15;
	a->m_req_ability[0] = "cure_conf_bsrk";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Hypnosis";
	a->m_description = "The priest confuses a monster, causing it to attack at random.";

	a = &(Action::m_action_info["hypnosis2"]);
	a->m_timer_nr=1;
	a->m_standard_time=701;
	a->m_timer=7000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = RANGED;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 60;
	a->m_req_ability[0] = "hypnosis";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Hypnotic Vortex";
	a->m_description = "The priest creates a distracting vortex with the ability to confuse every enemy caught in it.";

	a = &(Action::m_action_info["keen_mind"]);
	a->m_timer_nr=2;
	a->m_standard_time=701;
	a->m_timer=180000;
	a->m_base_action = "noaction";
	a->m_critical_perc=0.66;
	a->m_target_type = PARTY_MULTI;
	a->m_flags =MUTE_AFFECTED;
	a->m_req_level = 5;
	a->m_req_ability[0] = "blade_storm";
	a->m_req_ability[1] = "noaction";
	a->m_req_ability[2] = "noaction";
	a->m_name = "Keen Mind";
	a->m_description = "The priest increases everyones spellpower for a short time. Everyones spells will surely be more effective. With increasing willpower the priest can hold this effect longer.";
	*/
}


bool Action::loadAbilityData(const char* pFilename)
{
	DEBUG5("loading file: %s",pFilename);
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadAbilities(&doc);
		return true;
	}
	else
	{
		DEBUG("Failed to load file %s", pFilename);
		return false;
	}
}

void Action::loadAbilities(TiXmlNode* node)
{
	TiXmlNode* child;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "Ability"))
	{
		loadAbility(node);
	}
	else
	{
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadAbilities(child);
		}
	}
}

void Action::loadAbility(TiXmlNode* node)
{
	ElementAttrib attr;
	attr.parseElement(node->ToElement());
	
	ActionType type;
	attr.getString("type", type);
	
	Action::ActionInfo* a;
	
	a = &(Action::m_action_info[type]);
	
	a->m_projectile_type="";
	attr.getString("name",a->m_name);
	attr.getString("description",a->m_description);
	attr.getString("base_ability",a->m_base_action,"noaction");
	attr.getFloat("time",a->m_standard_time,1);
	attr.getInt("timer_nr",a->m_timer_nr,0);
	attr.getFloat("timer_value",a->m_timer,0);
	attr.getFloat("critical_percent",a->m_critical_perc,0);
	attr.getFloat("radius",a->m_radius,0);
	std::string target;
	attr.getString("target_type",target,"melee");
	if (target =="melee")
		a->m_target_type = MELEE;	
	else if (target =="ranged")
		a->m_target_type = RANGED;	
	else if (target =="self")
		a->m_target_type = SELF;	
	else if (target =="party")
		a->m_target_type = PARTY;	
	else if (target =="party_all")
		a->m_target_type = PARTY_MULTI;	
	else if (target =="passive")
		a->m_target_type = PASSIVE;	
	else
	{
		ERRORMSG("invalid target type for ability %s",type.c_str());
	}
	DEBUG5("ability %s %s %f %i %f %f %s %s",type.c_str(), a->m_name.c_str(), a->m_standard_time, a->m_timer_nr, a->m_timer, a->m_critical_perc, target.c_str(), a->m_base_action.c_str());
		
	TiXmlNode* child;
	TiXmlNode* child2;
	for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
	{
		if (child->Type()==TiXmlNode::ELEMENT) 
		{
			attr.parseElement(child->ToElement());
			
			if (!strcmp(child->Value(), "Flag"))
			{
				attr.parseElement(child->ToElement());
				std::string flag;
				attr.getString("name",flag);
				if (flag == "mute_affected")
				{
					a->m_flags |= MUTE_AFFECTED;
				}
				DEBUG5("  flag: %s",flag.c_str());
			}
			else if (!strcmp(child->Value(), "Effect"))
			{
				loadHybridImplementation(child, a->m_effect);
			}
			else if (!strcmp(child->Value(), "Projectile"))
			{
				attr.parseElement(child->ToElement());
				attr.getString("type", a->m_projectile_type,"");
				attr.getInt("counter", a->m_projectile_counter);
				attr.getFloat("speed", a->m_projectile_speed);
				a->m_projectile_flags =0;
				for ( child2 = child->FirstChild(); child2 != 0; child2 = child2->NextSibling())
				{
					if (child2->Type()==TiXmlNode::ELEMENT) 
					{
						attr.parseElement(child2->ToElement());
						if (!strcmp(child2->Value(), "Flag"))
						{
							std::string flag;
							attr.getString("name",flag);
							if (flag == "explodes")
							{
								a->m_projectile_flags |= Projectile::EXPLODES;
							}
							else if (flag == "multi_explodes")
							{
								a->m_projectile_flags |= Projectile::MULTI_EXPLODES;
							}
							else if (flag == "piercing")
							{
								a->m_projectile_flags |= Projectile::PIERCING;
								
							}
							else if (flag == "bouncing")
							{
								a->m_projectile_flags |= Projectile::BOUNCING;
							}
							else if (flag == "prob_bouncing")
							{
								a->m_projectile_flags |= Projectile::PROB_BOUNCING;
							}
							DEBUG5(" %s flags %i",a->m_projectile_type.c_str(), a->m_projectile_flags);
						}
					}
				}
			}
			else if (child->Type()!=TiXmlNode::COMMENT)
			{
				DEBUG("unexpected element of <Ability>: %s",child->Value());
			}
		}
	}
}

std::string Action::getName(ActionType type)
{
	ActionInfo* aci = getActionInfo(type);
	if (aci == 0)
	{
		WARNING("unknown action: %s",type.c_str());
		return "";
	}
      return gettext(aci->m_name.c_str()) ;

}


std::string Action::getDescription(ActionType type)
{
	ActionInfo* aci = getActionInfo(type);
	if (aci == 0)
	{
		WARNING("unknown action: %s",type.c_str());
		return "";
	}
       return  gettext(aci->m_description.c_str());
}

Action::ActionType Action::getActionType(std::string name)
{
	std::map<ActionType, ActionInfo>::iterator it;
	it = m_action_info.find(name);
	if (it != m_action_info.end())
	{
		return name;
	}
	return "noaction";
}

void Action::toString(CharConv* cv)
{

	cv->toBuffer(m_type);
	cv->toBuffer(m_time);
	cv->toBuffer((char) m_action_equip);
	cv->toBuffer(m_elapsed_time);


}

void Action::fromString(CharConv* cv)
{
	
	char ctmp;
	cv->fromBuffer(m_type);
	cv->fromBuffer(m_time);
	cv->fromBuffer(ctmp);
	m_action_equip = (ActionEquip) ctmp;
	cv->fromBuffer(m_elapsed_time);
}

bool Action::loadHybridImplementation(TiXmlNode* node, HybridImplementation& impl)
{
	TiXmlNode* child;
	TiXmlText* text;
	ElementAttrib attr;
	for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
	{
		if (child->Type()==TiXmlNode::ELEMENT) 
		{
			attr.parseElement(child->ToElement());
			
			if (!strcmp(child->Value(), "CppImplementation"))
			{
				std::string type;
				ElementAttrib attr;
				attr.parseElement(child->ToElement());
				attr.getString("type",type);
				
				impl.m_cpp_impl.push_back(type);
			}
			else if (!strcmp(child->Value(), "LuaImplementation"))
			{
				text = child->FirstChild()->ToText();
				if (text != 0)
				{
					impl.m_lua_impl = EventSystem::createCodeReference(text->Value());
				}
			}
		}
	}
	return true;
}

void Action::toxml()
{
	TiXmlDocument doc;
	
	std::map<ActionType,ActionInfo>::iterator it;
	TiXmlElement * elem;
	TiXmlElement * elem2;
	for (it = m_action_info.begin(); it != m_action_info.end(); ++it)
	{
		elem = new TiXmlElement("Ability");
		elem->SetAttribute("type",it->first.c_str());
		elem->SetAttribute("name",it->second.m_name.c_str());
		elem->SetAttribute("description",it->second.m_description.c_str());
		elem->SetDoubleAttribute("time",it->second.m_standard_time);
		elem->SetDoubleAttribute("timer_nr",it->second.m_timer_nr);
		elem->SetDoubleAttribute("timer_value",it->second.m_timer);
		elem->SetAttribute("base_ability",it->second.m_base_action.c_str());
		elem->SetDoubleAttribute("critical_percent",it->second.m_critical_perc);
		if (it->second.m_target_type == MELEE)			
			elem->SetAttribute("target_type","melee");
		else if (it->second.m_target_type == RANGED)
			elem->SetAttribute("target_type","ranged");
		else if (it->second.m_target_type == PARTY)
			elem->SetAttribute("target_type","party");
		else if (it->second.m_target_type == PASSIVE)
			elem->SetAttribute("target_type","passive");
		else if (it->second.m_target_type == PARTY_MULTI)
			elem->SetAttribute("target_type","party_all");
		if (it->second.m_flags & MUTE_AFFECTED)
		{
			elem2 = new TiXmlElement("Flag");
			elem2->SetAttribute("name","mute_affected");
			elem->LinkEndChild(elem2);
		}
		doc.LinkEndChild(elem);
	}
	doc.SaveFile("../data/abilities/base_abilities.xml");
}


