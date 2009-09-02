-- Kurzformen fuer oft benutzte Funktionen


-- Pruefen von Eigenschaften
function isMonster(obj)
		return (getObjectValue(obj,"type") == "MONSTER")
		end;

function isPlayer(obj)
		return (getObjectValue(obj,"type") == "PLAYER")
		end;

function isFixedObject(obj)
		return (getObjectValue(obj,"type") == "FIXED_OBJECT")
		end;

function isCreature(obj)
		local t = getObjectValue(obj,"type")
		return (t == "MONSTER" or t=="PLAYER" or t=="NPC")
		end;

function isAlive(obj)
		return (isCreature(obj) and getObjectValue(obj,"health")>0) ;
end;


-- Getter
function getPosition(obj)
	return getObjectValue(obj,"position");
end;
		
function getType(obj)
	return getObjectValue(obj,"subtype");
end;

function getHealth(obj)
		return getObjectValue(obj,"health");
end;

-- Setter
function setHealth(obj,val)
	setObjectValue(obj,"health",val);
end;


function kill(obj)
	if (isCreature(obj) and getHealth(obj)>0) then
		setHealth(obj,0);
	end;
end;

function fullHeal(obj)
	if (isCreature(obj)) then
		setHealth(obj,getObjectValue(obj,"max_health"));
	end;
end;

function lookAt(obj,pos)
	local opos = getPosition(obj);
	if (opos == nil) then
		return;
	end;
	local dirx = pos[1] - opos[1];
	local diry = pos[2] - opos[2];
	local angle = math.atan2(diry,dirx)*180 / 3.14159;
	setObjectValue(obj,"angle",angle);
end;

function lookAtObject(obj,obj2)
	lookAt(obj,get(obj2,"position"));
end;

function playersLookAt(pos)
	for key,id in pairs(getPlayers()) do
		lookAt(id,pos);
	end;
end;


-- Objekte mit bestimmten Eigenschaften herrausfiltern
function getPlayersInArea(area)
		return filter(getObjectsInArea(area),isPlayer);
end;

function getMonstersInArea(area)
		return filter(getObjectsInArea(area,"normal","monster"),isMonster);
end;

function getPartyleader()
	return getRolePlayers("leader");
end;

function getPlayers()
	return getRolePlayers("all");
end;

function getWarriors()
	return getRolePlayers("warrior");
end;

function getMages()
	return getRolePlayers("mage");
end;

function getArchers()
	return getRolePlayers("archer");
end;

function getPriests()
	return getRolePlayers("priest");
end;

function getMales()
	return getRolePlayers("male");
end;

function getFemales()
	return getRolePlayers("female");
end;


-- Sprecher-Rollen verteilen
function getRole(role)
	if (role == "PL") then
		return getPartyleader()[1];
	elseif (role == "ANY") then
		if (getPlayers()[2] ~= nil and getPlayers()[2] ~= getPartyleader()[1]) then
			return getPlayers()[2];
		else
			return getPlayers()[1];
		end;
	elseif (role == "ANYast") then
		if (getPlayers()[3] ~= nil and getPlayers()[3] ~= getPartyleader()[1]) then
			return getPlayers()[3];
		elseif (getPlayers()[2] ~= nil and getPlayers()[2] ~= getPartyleader()[1]) then
			return getPlayers()[2];
		else
			return getPlayers()[1];
		end;
	elseif (role == "ANYplus") then
		if (getPlayers()[3] ~= nil and getPlayers()[3] ~= getPartyleader()[1]) then
			return getPlayers()[3];
		else
			return getPlayers()[2];
		end;
	elseif (role == "WAR") then
		if (getWarriors()[1] ~= nil) then
			return getWarriors()[1];
		else
			return getRole("ANY");
		end;
	elseif (role == "PRI") then
		if (getPriests()[1] ~= nil) then
			return getPriests()[1];
		else
			return getRole("ANY");
		end;
	elseif (role == "ARC") then
		if (getArchers()[1] ~= nil) then
			return getArchers()[1];
		else
			return getRole("ANY");
		end;
	elseif (role == "MAG") then
		if (getMages()[1] ~= nil) then
			return getMages()[1];
		else
			return getRole("ANY");
		end;
	elseif (role == "WARast") then
		return getWarriors()[1];
	elseif (role == "PRIast") then
		return getPriests()[1];
	elseif (role == "ARCast") then
		return getArchers()[1];
	elseif (role == "MAGast") then
		return getMages()[1];
	elseif (role == "MALEast") then
		return getMales()[1];
	elseif (role == "FEMALEast") then
		return getFemales()[1];
	end;
end;


-- position: Position, an der Spieler miteinander sprechen sollen
-- range: Reichweite aus der Spieler herangeholt werden
--        ("world","region","area")
-- area: wurde bei range "area" angegeben, so ist hier das Gebiet anzugeben

function addStandardSpeakers(region,location)
	addSpeaker(getRole("PL"),"PL");
	addSpeaker(getRole("ANY"),"ANY");
	addSpeaker(getRole("ANYast"),"ANYast");
	addSpeaker(getRole("ANYplus"),"ANYplus");
	addSpeaker(getRole("WAR"),"WAR");
	addSpeaker(getRole("PRI"),"PRI");
	addSpeaker(getRole("ARC"),"ARC");
	addSpeaker(getRole("MAG"),"MAG");
	addSpeaker(getRole("WARast"),"WARast");
	addSpeaker(getRole("PRIast"),"PRIast");
	addSpeaker(getRole("ARCast"),"ARCast");
	addSpeaker(getRole("MAGast"),"MAGast");
	addSpeaker(getRole("MALEast"),"MALEast");
	addSpeaker(getRole("FEMALEast"),"FEMALEast");
	
	if (region ~= nil and location ~= nil) then
		local players = getPlayers();
		local i,player;
		for i,player in ipairs(players) do
			teleportPlayer(player,region,location);
		end;
	end;
end;

-- Gibt einen Sprecher fuer eine Rolle zurueck, dessen ID nicht in der Liste forbiddenids ist
-- role: string, Sprecherrolle
-- forbiddenids: Tabelle, int->bool, falls fuer eine ID true angegeben ist, so wird dieser Spieler nicht gewaehlt
function getRolePlayer(role, forbiddenids)
	local cand = getRolePlayers(role);
	local id,key
	for key,id in pairs(cand) do
		if (forbiddenids == nil or forbiddenids[id] ~= true) then
			return id;
		end;
	end;
	return 0;
end;

function getRolePlayerNonPref(role, nonprefids)
	local id = getRolePlayer(role,nonprefids);
	if (id == 0) then
		id = getRolePlayer(role);
	end;
	return id;
end;

function getRolePlayerPref(role,prefid)
	local cand = getRolePlayers(role);
	local id,key
	for key,id in pairs(cand) do
		if (id == prefid) then
			return id;
		end;
	end;
	if (cand[1] == nil) then
		return 0;
	end;
	return cand[1];
end;

-- Teleportiert die komplette Party an die Position location in Region region
function assembleParty(region,location)
	if (region ~= nil and location ~= nil) then
		local players = getPlayers();
		local i,player;
		for i,player in ipairs(players) do
			teleportPlayer(player,region,location);
		end;
	end;
end;

-- Fuegt alle Spieler dem Dialog hinzu
function addAllPlayersToDialog()
	local players = getPlayers();
	local i,player;
	for i,player in ipairs(players) do
		addSpeaker(player,get(player,"name"));
	end;
end;

function addStandardRoles()
	local leader = getRolePlayer("leader");
	local blockedspeaker={};
	blockedspeaker[leader] = true;
	addSpeaker(leader,"PL");
	
	local warrior = getRolePlayerNonPref("warrior",blockedspeaker);
	local mage = getRolePlayerNonPref("mage",blockedspeaker);
	local archer = getRolePlayerNonPref("archer",blockedspeaker);
	local priest = getRolePlayerNonPref("priest",blockedspeaker);
	
	addSpeaker(warrior,"warrior");
	addSpeaker(mage,"mage");
	addSpeaker(archer,"archer");
	addSpeaker(priest,"priest");
	
	local magopt = getRolePlayerPref("all",mage);
	local waropt = getRolePlayerPref("all",warrior);
	local arcopt = getRolePlayerPref("all",archer);
	local priopt = getRolePlayerPref("all",priest);
	addSpeaker(magopt,"magopt");
	addSpeaker(waropt,"waropt");
	addSpeaker(arcopt,"arcopt");
	addSpeaker(priopt,"priopt");
end;

