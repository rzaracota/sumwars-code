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
	setScriptObjectAnimation(test,"walk",1000,"true");
end;