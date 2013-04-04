--[[ renameLowercase.sh
luajit-2 ./rename.lua '@find . -path "./src/*[A-Z]*.h" -or -path "./src/*[A-Z]*.cpp" -or -path "./share/*[A-Z]*.layout"' \
  '!return table.concat(f.PathSplit, "/", 1, #f.PathSplit-1) .. "/" .. f.PathSplit[#f.PathSplit]:lower()' \
  '@find . -path "./src/*.h" -or -path "./src/*.cpp" -or -path "./share/*.xml"' \
  'hg mv "%s" "%s"'
]]

local replacementPatterns = {'"%s"', '/%s"', '/%s/', '&quot;%s&quot;', '/%s&quot;'}

local function split(str, pat)
	local t = {}
	local fpat = "(.-)" .. pat
	local last_end = 1
	local s, e, cap = str:find(fpat, 1)
	while s do
		if s ~= 1 or cap ~= "" then
			table.insert(t,cap)
		end
		last_end = e+1
		s, e, cap = str:find(fpat, last_end)
	end
	if last_end <= #str then
		cap = str:sub(last_end)
		table.insert(t, cap)
	end
	return t
end

local function splitPaths(State, Name, SplitName)
	for _,v in ipairs(State) do
		v[SplitName] = split(v[Name], '/+')
	end
end

local function handleArg(Arg, State, Name)
	
	assert(Arg, "Missing argument " .. Name)
	
	if Arg:sub(1,1) == "!" then
		local func, err = loadstring(Arg:sub(2))
		if func == nil then
			print(Arg, ": ", err)
			return false
		end
		
		local e = getfenv(func)
		
		for _,v in ipairs(State) do
			e.f = v
			v[Name] = func()
		end
		
		return true
	end

	local hFile = nil
	if Arg:sub(1,1) == "@" then
		hFile = io.popen(Arg:sub(2), "r")
	else
		hFile = io.open(Arg, "r")
	end
	
	local i = 1;
	for l in hFile:lines() do
		if State[i] == nil then
			State[i] = {}
		end
		
		State[i][Name] = l
		i = i +1
	end
	hFile:close()
	return true
end

local function buildLookup(State, SplitName, LookupName)

	local count = 0
	for _,v in ipairs(State) do
		local res = {}
		for i = #v[SplitName],1,-1 do
			for _,p in ipairs(replacementPatterns) do
				res[#res+1] = p:format(table.concat(v[SplitName], "/", i))
				count = count + 1
			end
		end
		v[LookupName] = res
	end
	
	return count
end

local function readFile(Path)
	local f = io.open(Path, "r")
	local s = f:read("*a")
	f:close()
	return s
end

local function writeFile(Path, Content)
	
	local f = io.open(Path, "w")
	f:write(Content)
	f:close()
	
end

local function replaceStr(str, Look, Replacement)

	local c = 0
	local res = ""
	local i = 1
	local s,e = str:find(Look, i, true)
	while s do
		res = res .. str:sub(i,s-1) .. Replacement
		i = e + 1
		s,e = str:find(Look, i, true)
		c = c + 1
	end
	
	res = res .. str:sub(i)
	return res, c
end

local function processContent(Content, fileRename)

	local updates = 0
	for _,v in ipairs(fileRename) do
		for i = 1,#v.PathLookup,1 do
			local count
			Content, count = replaceStr(Content, v.PathLookup[i], v.NewPathLookup[i])
			updates = updates + count
		end
	end
	
	print("", updates, "Updates\n")

	return Content
end

local function processFiles(fileEdit, fileRename)

	local i = 1
	for _,v in ipairs(fileEdit) do
		print(("Editing file %4i/%i"):format(i, #fileEdit), v["EditPath"])
		local content = readFile(v["EditPath"])
		local newcontent = processContent(content, fileRename)
		if content ~= newcontent then
			writeFile(v["EditPath"], newcontent)
		end
		i = i + 1
	end
end

local function renameFiles(State, Command)
	Command = Command or 'mv "%s" "%s"'
	local i = 1
	for _,v in ipairs(State) do
		local cmd = Command:format(v.Path, v.NewPath)
		
		print(("Renaming file %4i/%i"):format(i, #State), cmd)
		i = i + 1
		os.execute(cmd)
	end
end

local function main(Args)

	if #Args < 3 then
		print[[
rename.lua <Path> <NewPath> <EditPath> [<Command>]
	<Path>      list of files/directories which will be renamed during the process
	<NewPath>   equivalent list of paths with new names for the files/directories
	<EditPath>  list of files which will be updated to incorporate the new filenames
	<Command>   optional parameter to specify the shellcommand used to rename the files, defaults to "mv"
	
	path parameters:
	'pathlist.txt'           use the content of the file as paths
	'@find . -path "*.cpp"'  use the output of any command as paths
	'!return f.path:lower()' use a lua script to build the new filenames from f.Path and f.PathSplit respectively
		]]
		return -1
	end

	local fileRename = {}
	local fileEdit = {}
	
	handleArg(Args[1], fileRename, "Path") 
	splitPaths(fileRename, "Path", "PathSplit")
	handleArg(Args[2], fileRename, "NewPath") 
	splitPaths(fileRename, "NewPath", "NewPathSplit")
	handleArg(Args[3], fileEdit, "EditPath")
	
	table.sort(fileRename, function (a,b) return #a.PathSplit < #b.PathSplit or (#a.PathSplit == #b.PathSplit and a.Path < b.Path) end)
	table.sort(fileEdit, function (a,b) return a.EditPath < b.EditPath end)
	
	local p = buildLookup(fileRename, "PathSplit", "PathLookup")
	buildLookup(fileRename, "NewPathSplit", "NewPathLookup")
	
	print(#fileRename, "files to rename with", p, "patterns to look for.")
	
	processFiles(fileEdit, fileRename)
	
	renameFiles(fileRename, Args[4])
	
end

main(arg)
