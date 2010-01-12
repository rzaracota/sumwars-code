function printlist(tab)
	for key,value in pairs(tab) do
		print(key,value);
	end;
end;

function jump(region, location)
	player = getPlayers()[1];
	teleportPlayer(player,region,location);
end;

function camp()
	player = getPlayers()[1];
	teleportPlayer(player,"medMercCamp","entry_north");
end;

function gold(amount)
	plpos()
	set(pl,"gold",amount)
end;