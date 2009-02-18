function printquestvars(tab)
	for key,value in pairs(tab) do
		print("quest: "..key);
		for key2,val2 in pairs(value) do
			print(key2,val2);
		end;
		print("---");
	end;
end;


function removeplayer(id)
	playervars[id] = nil;
end;

function addplayer(id)
	playervars[id] = {};
end;

