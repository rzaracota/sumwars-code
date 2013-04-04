luajit-2 ./rename.lua '@find . -path "./src/*[A-Z]*.h" -or -path "./src/*[A-Z]*.cpp" -or -path "./share/*[A-Z]*.layout"' \
  '!return table.concat(f.PathSplit, "/", 1, #f.PathSplit-1) .. "/" .. f.PathSplit[#f.PathSplit]:lower()' \
  '@find . -path "./src/*.h" -or -path "./src/*.cpp" -or -path "./share/*.xml"' \
  'hg mv "%s" "%s"'
