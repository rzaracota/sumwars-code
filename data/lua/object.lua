
-- puts an object in the NOCOLLISION-layer and sets it 20 units under the earth, meaning it will be not visible anymore
function getObjectOutOfWay(obj)
	set(obj,"layer","nocollision");
	set(obj,"height",-20);
end;
-- gets an object back on the normal floor
function getObjectOnFloor(obj)
	set(obj,"layer","normal");
	set(obj,"height",0);
end;


