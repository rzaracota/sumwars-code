-- Kurzformen fuer oft benutzte Funktionen


-- Pruefen von Eigenschaften
function isMonster(obj)
		return (getObjectValue(obj,"type") == "monster")
		end;

function isPlayer(obj)
		return (getObjectValue(obj,"type") == "player")
		end;

function isFixedObject(obj)
		return (getObjectValue(obj,"type") == "fixed_object")
		end;

function isCreature(obj)
		local t = getObjectValue(obj,"type")
		return (t == "monster" or t=="player")
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


-- Objekte mit bestimmten Eigenschaften herrausfiltern
function getPlayersInArea(area)
		return filter(getObjectsInArea(area),isPlayer);
end;


function getMonstersInArea(area)
		return filter(getObjectsInArea(area),isMonster);
end;

function getPlayers()
	return getRolePlayers("all");
end;


