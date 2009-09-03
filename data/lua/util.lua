function listToString(list)
		local i;
		local str="{";
		for i = 1,#list do
			if (i ~= 1) then
				str = str..",";
			end;
			str = str..list[i];
		end;
		return str.."}";
end;

function locnameToLocation(loc)
	if (type(loc) == "string") then
		loc = getLocation(loc);
	end;
	return loc;
end;