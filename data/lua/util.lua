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