-- locations: berechnete Orte (Tabelle)
-- center Zentrum des Halbkreises
function generateArc(locations, center, radius, number, arcangle, angle)
	angle = angle or 0;
	arcangle = arcangle or 120;
	number = number or 1;
	local n = table.getn(locations);
	if (number == 1) then
		locations[n+1] = {center[1] + math.cos(angle*math.pi/180)*radius, center[2] + math.sin(angle*math.pi/180)*radius};
		return;
	end;
	local angl = angle - 0.5*arcangle;
	local inc = arcangle / (number-1);
	local i;
	for i=0,number-1 do
		locations[n+1] = {center[1] + math.cos(angl*math.pi/180)*radius, center[2] + math.sin(angl*math.pi/180)*radius};
		angl = angl + inc;
		n = n+1;
	end;
end;