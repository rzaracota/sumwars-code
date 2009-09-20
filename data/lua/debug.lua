function printlist(tab)
	for key,value in pairs(tab) do
		print(key,value);
	end;
end;
		
function tt()
	teleportPlayer(4,"aisMounPath","entry_east")
end;
		
function xx(layer)
				test = createScriptObject("test","goblin","circle","entry_north",0.5,layer);
end;
		
function cc()
	door = createObject("treasure_box_m1","loc1",90);
end;

function vv()
	
end;

function jump(region, location)
	player = getPlayers()[1];
	teleportPlayer(player,region,location);
end;

function camp()
	player = getPlayers()[1];
	teleportPlayer(player,"medMercCamp","entry_north");
end;