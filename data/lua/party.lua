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
		local tele = false;
		local tmp
		for i,player in ipairs(players) do
			tmp = teleportPlayer(player,region,location);
			if (tmp == true) then
				tele = true
			end;
		end;
		if (not tele) then
			insertTrigger("party_complete");
		end;
	end;
end;

-- Gruppiert Objekte in einem Teilkreis
-- objects - Liste der Objekte
-- location - Mittelpunkt des Bogens
-- lookatloc - Mittelpunks des Kreises
function groupObjectsArc(objects,location,lookatloc,radius, arcangle)
	if (location ~= nil) then
		location = locnameToLocation(location)
		lookatloc = locnameToLocation(lookatloc);
		local dir = {location[1] -lookatloc[1], location[2]-lookatloc[2]};
		local angle = math.atan2(dir[2],dir[1])*180/math.pi;
		local i,obj;
		local loc ={};
		if (radius == nil) then
			radius = math.sqrt(dir[1]*dir[1] + dir[2]*dir[2]);
		end;
		generateArc(loc,lookatloc,radius,table.getn(objects),arcangle,angle);
		
		-- zweimal ausfuehren, weil beim ersten Mal haeuffig noch Verdraengung stattfindet
		for i,obj in ipairs(objects) do
			moveObject(obj,loc[i]);
		end;
		for i,obj in ipairs(objects) do
			moveObject(obj,loc[i]);
			lookAt(obj,lookatloc);
		end;
		
		
	end;
end;

-- Fuegt alle Spieler dem Dialog hinzu
function addAllPlayersToDialog()
	local players = getPlayers();
	local i,player;
	for i,player in ipairs(players) do
		addSpeaker(player,get(player,"name"),true);
	end;
end;

function addStandardRoles()
	local leader = getRolePlayer("leader");
	local blockedspeaker={};
	blockedspeaker[leader] = true;
	addSpeaker(leader,"PL");
	addSpeaker(leader,"leader");
	
	local warrior = getRolePlayerNonPref("warrior",blockedspeaker);
	local mage = getRolePlayerNonPref("mage",blockedspeaker);
	local archer = getRolePlayerNonPref("archer",blockedspeaker);
	local priest = getRolePlayerNonPref("priest",blockedspeaker);
	local any = getRolePlayerNonPref("all",blockedspeaker);
	
	addSpeaker(warrior,"warrior",true);
	addSpeaker(mage,"mage",true);
	addSpeaker(archer,"archer",true);
	addSpeaker(priest,"priest",true);
	addSpeaker(any,"any",true);
	
	local magopt = getRolePlayerPref("all",mage);
	local waropt = getRolePlayerPref("all",warrior);
	local arcopt = getRolePlayerPref("all",archer);
	local priopt = getRolePlayerPref("all",priest);
	addSpeaker(magopt,"magopt",true);
	addSpeaker(waropt,"waropt",true);
	addSpeaker(arcopt,"arcopt",true);
	addSpeaker(priopt,"priopt",true);
end;