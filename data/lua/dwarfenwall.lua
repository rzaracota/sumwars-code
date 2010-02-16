
-- helperfunctions for the siege of dwarfenwall and dwarfenwall itself
function sendOutHealer(unitToHeal)
	local healer = createObject("aisen_healer", "locHealerIn")
	local targetLoc = get(unitToHeal,"position");

	if(dwarfenwall.people[unitToHeal]~=nil)then
		--print(dwarfenwall.people[unitToHeal])
		local i = dwarfenwall.people[unitToHeal]
		--command chain for the hit location
		local chain = dwarfenwall.city[i]["healer"];
		for key,loc in pairs(chain) do
			if(loc == "heal")then
				--print("healing for "..unitToHeal);
				addUnitCommand(healer,"heal", unitToHeal);
			else
				--print("walking to "..loc);
				addUnitCommand(healer,"walk",loc,2.5);
			end;
		end;
	else
		-- no information available
		addUnitCommand(healer,"walk",targetLoc,2.5);
		addUnitCommand(healer,"heal",unitToHeal);
		addUnitCommand(healer,"walk","locHealerOut");
	end;
end;

function catapult(impactPos)
	createProjectile("catapult_shot",impactPos);
	setDamageValue("phys_dmg", {60,90});
	setDamageValue("power",3000);
	setDamageValue("attack",3000);
end;

function dwarfenwallPeasant1()
--peasant 1
	dwarfenwall.city[13] = {};
	dwarfenwall.city[13]["location"] = "locPeasant1";
	dwarfenwall.city[13]["important"] = false;
	dwarfenwall.city[13]["moving"] = true;
	dwarfenwall.city[13]["refname"] = "Peasant 1";
	dwarfenwall.city[13]["phase"] = 1;
	dwarfenwall.city[13]["phaseCount"] = 4;
	-- commands for phase 1
	dwarfenwall.city[13][1] = {};
	dwarfenwall.city[13][1] = {
		{'addUnitCommand(dwarfenwall.city[13]["unit"],"walk","locPeasant3",2)',0},
		{'addUnitCommand(dwarfenwall.city[13]["unit"],"walk","locPeasant4",2.5)',0}
	}
	-- commands for phase 2
	dwarfenwall.city[13][2] = {
		{'lookAtEachOther(dwarfenwall.city[11]["unit"],dwarfenwall.city[13]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[13]["unit"],"talk",1500,true)',0},
		{'clearUnitCommands(dwarfenwall.city[13]["unit"])',1500},
		{'setObjectAnimation(dwarfenwall.city[13]["unit"],"think",1000)',1500};
		{'setObjectAnimation(dwarfenwall.city[11]["unit"],"talk",1500,true)',1500};
		{'startTimer("peasantNextPhase",5000);addTriggerVariable("id",13)',0}
	}
	-- commands for phase 3
	dwarfenwall.city[13][3]={
		{'clearUnitCommands(dwarfenwall.city[11]["unit"])',0},
		{'addUnitCommand(dwarfenwall.city[13]["unit"],"walk","locPeasant8",0.3)',0}
	}
	-- commands for phase 4
	dwarfenwall.city[13][4]={
		{'deleteAndRespawn(dwarfenwall.city[13]["unit"])',100},
		{'startTimer("peasantNextPhase",350);addTriggerVariable("id",13)',150}
	}
end;

function dwarfenwallPeasant2()
--peasant 2	
	dwarfenwall.city[14] = {};
	dwarfenwall.city[14]["location"] = "locPeasant5";
	dwarfenwall.city[14]["important"] = false;
	dwarfenwall.city[14]["moving"] = true;
	dwarfenwall.city[14]["refname"] = "Peasant 2";
	dwarfenwall.city[14]["phase"] = 1;
	dwarfenwall.city[14]["phaseCount"] = 6;
	-- commands for phase 1
	dwarfenwall.city[14][1] = {};
	dwarfenwall.city[14][1] = {
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant6",2.5);',0}
	}
	-- commands for phase 2
	dwarfenwall.city[14][2] = {};
	dwarfenwall.city[14][2] = {
		{'lookAtEachOther(dwarfenwall.city[12]["unit"], dwarfenwall.city[14]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"talk",800,true)',0},
		{'clearUnitCommands(dwarfenwall.city[14]["unit"])',1500},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"angry",1000)',1500},
		{'setObjectAnimation(dwarfenwall.city[12]["unit"],"talk",800,true)',1500},
		{'startTimer("peasantNextPhase",4000);addTriggerVariable("id",14)',0}
	}
	-- commands for phase 3
	dwarfenwall.city[14][3] = {};
	dwarfenwall.city[14][3] = {
		{'setObjectAnimation(dwarfenwall.city[12]["unit"],"",800)',0},
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant5",2)',0},
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant3",1)',0},
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant2",2.5)',0}
	}
	-- commands for phase 4
	dwarfenwall.city[14][4] = {};
	dwarfenwall.city[14][4] = {
		{'lookAtEachOther(dwarfenwall.city[10]["unit"], dwarfenwall.city[14]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"talk",800,true)',0},
		{'clearUnitCommands(dwarfenwall.city[14]["unit"])',1500},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"laugh",1000)',1500},
		{'setObjectAnimation(dwarfenwall.city[10]["unit"],"talk",800,true)',1500},
		{'clearUnitCommands(dwarfenwall.city[10]["unit"])',2800},
		{'setObjectAnimation(dwarfenwall.city[10]["unit"],"laugh",1000)',2800},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"talk",800,true)',2800},
		{'startTimer("peasantNextPhase",6000);addTriggerVariable("id",14)',0}
	}
	-- commands for phase 5
	dwarfenwall.city[14][5] = {};
	dwarfenwall.city[14][5] = {
		{'clearUnitCommands(dwarfenwall.city[10]["unit"])',0},
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant8")',0},
	}
	-- commands for phase 6
	dwarfenwall.city[14][6] = {};
	dwarfenwall.city[14][6] = {
		{'deleteAndRespawn(dwarfenwall.city[14]["unit"])',100},
		{'startTimer("peasantNextPhase",350);addTriggerVariable("id",14)',150}
	}
end;

function dwarfenwallPeasant3()
--peasant 3	
	dwarfenwall.city[15] = {};
	dwarfenwall.city[15]["location"] = "locPeasant7";
	dwarfenwall.city[15]["important"] = false;
	dwarfenwall.city[15]["moving"] = true;
	dwarfenwall.city[15]["refname"] = "Peasant 3";
	dwarfenwall.city[15]["phase"] = 1;
	dwarfenwall.city[15]["phaseCount"] = 6;
	-- commands for phase 1
	dwarfenwall.city[15][1] = {};
	dwarfenwall.city[15][1] = {
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant4",2.5)',0}
	}
	-- commands for phase 2
	dwarfenwall.city[15][2] = {};
	dwarfenwall.city[15][2] = {
		{'lookAtEachOther(dwarfenwall.city[11]["unit"], dwarfenwall.city[15]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"talk",800,true)',0},
		{'startTimer("peasantNextPhase",3000);addTriggerVariable("id",15)',0}
	}
	-- commands for phase 3
	dwarfenwall.city[15][3] = {};
	dwarfenwall.city[15][3] = {
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant3",1)',0},
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant5",1)',0},
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locSergeant",2.5)',0}
	}
	-- commands for phase 4
	dwarfenwall.city[15][4] = {};
	dwarfenwall.city[15][4] = {
		{'lookAtEachOther(dwarfenwall.city[1]["unit"], dwarfenwall.city[15]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"talk",800,true)',0},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"think",1000)',2000},
		{'setObjectAnimation(dwarfenwall.city[1]["unit"],"talk",800,true)',2000},
		{'setObjectAnimation(dwarfenwall.city[1]["unit"],"",800)',5800},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"angry",1000)',3500},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"talk",800,true)',5800},

		{'startTimer("peasantNextPhase",8000);addTriggerVariable("id",15)',0}
	}
	-- commands for phase 5
	dwarfenwall.city[15][5] = {};
	dwarfenwall.city[15][5] = {
		{'setObjectAnimation(dwarfenwall.city[1]["unit"],"",800)',0},
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant9",1)',0},
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant7",2.5)',0}
	}
	-- commands for phase 6
	dwarfenwall.city[15][6] = {};
	dwarfenwall.city[15][6] = {
		{'startTimer("peasantNextPhase",3000);addTriggerVariable("id",15)',0}
	}
end;

function deleteAndRespawn(unitId)
	--print("unit: "..unitId);
	local id = dwarfenwall.people[unitId]
	local unit = dwarfenwall.city[id]["unit"];
	--print("unit: still "..unit);
	local newUnit = createObject("peasant",dwarfenwall.city[id]["location"],-90);
	--print("new unit: "..newUnit);
	dwarfenwall.city[id]["unit"]= newUnit;
	dwarfenwall.people[newUnit]=dwarfenwall.people[unit];
	dwarfenwall.people[unit]=nil;
	
	setRefName(newUnit, dwarfenwall.city[id]["refname"]);
	
	timedExecute('deleteObject('..unit..')',500);
end;

--rampart
function setGuard(index,rot)
	local guard = createObject(defend_dwall_tmp.siege[index]["subtype"], defend_dwall_tmp.siege[index]["location"],rot);
	addUnitCommand(guard,"guard",defend_dwall_tmp.siege[index]["location"],10,"secondary");
	
	defend_dwall_tmp.units[guard] = index;
	return guard;
end;

--set up the data for a unit in defend_dwall_tmp.siege
--index[int]: the siege index for that post
--location[string]: the location, were that post is
--guard_range[int]: the radius of the area, in wich the unit will move
--subtype[string]: the subtype of the unit on that post
--penalty[int]: the number of penalty-points the players will get if this unit dies
--respawn[int]: the respawntime for that unit in milliseconds
--respawnlocation[string]: the location, were the unit will respawn
--respawntrigger[string]: the trigger that will be thrown after this unit died. the default is newUnit
function createUnitIndex(index, location, guard_range, subtype, penalty, respawn, respawnlocation, respawntrigger)
	defend_dwall_tmp.siege[index] = {};
	defend_dwall_tmp.siege[index]["location"] = location;
	defend_dwall_tmp.siege[index]["guard_range"] = guard_range;
	defend_dwall_tmp.siege[index]["subtype"] = subtype;
	defend_dwall_tmp.siege[index]["penalty"] = penalty;
	
	defend_dwall_tmp.siege[index]["respawn"] = respawn;
	defend_dwall_tmp.siege[index]["respawnlocation"] = respawnlocation;
	if(respawntrigger~=nil)then
		defend_dwall_tmp.siege[index]["respawnTrigger"] = respawntrigger;
	else
		defend_dwall_tmp.siege[index]["respawnTrigger"] = "newUnit";
	end;
end;

function skel_catapult(impactPos)
	createProjectile("catapult_shot",impactPos);
	setDamageValue("phys_dmg", {20,35});
	setDamageValue("fire_dmg", {30,55});
	setDamageValue("power",3000);
	setDamageValue("attack",3000);
end;
