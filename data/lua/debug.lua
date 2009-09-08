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
	door = createObject("door","loc1");
	switch = createObject("switch","loc3");
	scriptobjectvar[switch]["door"] = door;
	
	local pos = getLocation("loc1");
	pos2 ={pos[1] - 0.7; pos[2] +4};
	createObject("wall_m1",pos2);
	pos2 ={pos[1] - 0.7; pos[2] -4};
	createObject("wall_m1",pos2);
	
end;

function vv()
	insertTrigger("object_use");
	addTriggerVariable("_id",door);
	addTriggerVariable("used_object",door);
end;