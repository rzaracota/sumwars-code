
-- initialises a dialog in the most common way
function initDialog()
	createDialogue();
	setTopicBase("global");
	addAllPlayersToDialog();
	addStandardRoles();
end;
--pitch black darkness
function fadeOut(millisecs)
	setLight("ambient",{0,0,0},millisecs)
	setLight("directional",{0,0,0},millisecs)
	setLight("hero",{0,0,0},millisecs)
end;
--fades to a set of lightstrengthes
--lighttypes: {red,green,blue}
function fadeIn(millisecs, ambient,hero,directional)
	setLight("ambient",ambient,millisecs)
	setLight("directional",directional,millisecs)
	setLight("hero",hero,millisecs)
end;


