#include "Perks.h"


const char *perkNames[NUMBER_OF_PERKS] = {
	"Well Fed",
	"Sharpened Claws",
	"Haste",
	"Thick Hides",
	"High Standards",
	"Strong Arms",
	"Indigestion",
	"Overachiever",
	"Channeler",
	"Insane Babble",
	"Fast Caster",
	"More Sacrifices",
	"Mana Burn",
	"Sloppy imps",
	"Earth Elemental",
	"Fire Elemental",
	"Water Elemental",
	"Air Elemental",
	"Vicious Hex",
	"Unholy Pact",
};

// Can we turn this perk off?
const bool perkLosable[NUMBER_OF_PERKS] = {
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	false,
	false,
	false,
	false,
	false,
	false
};

const char *perkDescriptions[NUMBER_OF_PERKS] = {
	"Your demons feed well after discovering some tender virgins.  Demons grow larger, increasing reach and damage but getting in each other's way.", // 50% larger demons, 5% more damage, can reach twice the length
	"A late adventurer's sword is put to good use: sharpening claws, which cut through hide and flesh all the more easily.",  // Demons do 15% more damage
	"Your demons become quick as the wind, but when caught they give your enemy more mana for killing them.", // Run 15% faster, give 15% more mana to enemy on death
	"Vigorous use of the whip has resulted in your demons having unusually thick hides, which repel all but the strongest attacks.", // 20% better armor
	"You have higher standards than most and only allow the strongest demons in your army.  Your demons are tougher and stronger, but cost more.", // Demons do 15% more damage, have 15% better armor, cost 15% more
	"Your ability to throw incredibly fast is both loved and hated. You love it, they hate it.",  // Throw speed builds up 20% faster
	"That paladin meal didn't go down well and gave you an upset stomach. However, the indigestion makes your fireballs white-hot.", // Fireballs do 10% more damage
	"You always give 110%.  As a result your fireballs do 110% more damage and have 110% the radius.  On the downside they cost 110% the mana.", 
	"You absorb mana like a sponge absorbs water.  5% faster mana recharge.",
	"Your insane babble confuses friend and foe alike. Occasionally your shots will cause targets to switch teams.", // Each shot has a 10% chance of causing a team change to all hit targets
	"New advancements in casting technology allow you to streamline the casting processes, summoning demons faster than ever before.", // Decrease summon time by 25%
	"Your worshippers, pleased by your progress, sacrifice two virgins a day instead of their usual one.  5% faster mana recharge.",
	"Frenzied casting has taken its toll.  You can only hold half the mana as before, but you regain it 10% quicker.", // Max mana has a 1/2 modifier, 10% faster recharge
	"Castle building imps are especially sloppy in these parts.  All castles take damage twice as quickly.",
	"You summon an Earth elemental, who conjures an earthquake that damages the enemy castle but leaves yours untouched.", // 10% of max castle health to damage
	"You summon a Fire elemental, who immediately flames all enemies on the battlefield.", // destroy all enemies
	"You summon a Water elemental, who sprays you with a burst of mana.", // Immediate full mana
	"You summon an Air elemental, who charms half the enemies to join your side.",
	"You sacrifice your own perk so that your opponent loses two.", // Opponent loses at most two perks at random.
	"You make a deal with the devil. You gain three perks immediately. However, your castle is nearly destroyed in the process.", // +3 levels, castle has 1 hp
};
