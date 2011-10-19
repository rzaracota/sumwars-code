@echo off
COLOR 0B
cls
echo *****************************************************************************
echo *                                                                           * 
echo *               Summoning Wars installation script                          *
echo *It suggests that you have already built sumwars.exe Release executable file*
echo *            It doesn't support DEBUG version of the game                   *
echo *****************************************************************************
set /p install= Do you really want to install SumWars in current directory (Y/N)?
if /i not "%install%" == "Y" (
echo Ok, i will not do this.
goto end
) ELSE (
echo Let's do it
)

if not EXIST "plugins_win.cfg" ( CALL :AskSVN
) ELSE ( 
       CALL :check
  )

echo Copy ALUT

if EXIST "%ALUTDIR%\." (
copy "%ALUTDIR%\lib\alut.dll" alut.dll
)

echo Copy CEGUI

if EXIST "%CEGUIDIR%\." (
copy "%CEGUIDIR%\bin\CEGUIBase.dll" CEGUIBase.dll
copy "%CEGUIDIR%\bin\CEGUIExpatParser.dll" CEGUIExpatParser.dll
copy "%CEGUIDIR%\bin\CEGUIFalagardWRBase.dll" CEGUIFalagardWRBase.dll
copy "%CEGUIDIR%\bin\CEGUILuaScriptModule.dll" CEGUILuaScriptModule.dll
copy "%CEGUIDIR%\bin\CEGUIOgreRenderer.dll" CEGUIOgreRenderer.dll
copy "%CEGUIDIR%\bin\CEGUISILLYImageCodec.dll" CEGUISILLYImageCodec.dll
) ELSE (
echo Can't find CEGUI folder, enshure that environmant variable is set correctly
COLOR 0C
CALL :fail
goto end
)

echo OGRE libraries

if EXIST "%OGRE_HOME%\." (
copy "%OGRE_HOME%\bin\release\cg.dll" cg.dll"
copy "%OGRE_HOME%\bin\release\OgreMain.dll" OgreMain.dll
copy "%OGRE_HOME%\bin\release\OgrePaging.dll" OgrePaging.dll
copy "%OGRE_HOME%\bin\release\OgreRTShaderSystem.dll" OgreRTShaderSystem.dll
copy "%OGRE_HOME%\bin\release\OgreTerrain.dll" OgreTerrain.dll
copy "%OGRE_HOME%\bin\release\OIS.dll" OIS.dll"
copy "%OGRE_HOME%\bin\release\Plugin_CgProgramManager.dll" Plugin_CgProgramManager.dll
copy "%OGRE_HOME%\bin\release\Plugin_OctreeSceneManager.dll" Plugin_OctreeSceneManager.dll
copy "%OGRE_HOME%\bin\release\Plugin_OctreeZone.dll" Plugin_OctreeZone.dll
copy "%OGRE_HOME%\bin\release\Plugin_ParticleFX.dll" Plugin_ParticleFX.dll
copy "%OGRE_HOME%\bin\release\Plugin_PCZSceneManager.dll" Plugin_PCZSceneManager.dll
copy "%OGRE_HOME%\bin\release\RenderSystem_Direct3D9.dll" RenderSystem_Direct3D9.dll
copy "%OGRE_HOME%\bin\release\RenderSystem_GL.dll" RenderSystem_GL.dll
) ELSE (
echo Can't find CEGUI folder, enshure that environmant variable is set correctly
COLOR 0C
CALL :fail
goto end
)

echo Copy PhysFS

if EXIST "%PHYSFSDIR%\lib\." (
copy "%PHYSFSDIR%\lib\physfs.dll" physfs.dll
)

if EXIST "%PHYSFSDIR%\bin\." (
copy "%PHYSFSDIR%\bin\physfs.dll" physfs.dll
)

if not EXIST physfs.dll (
echo Can't find PHYSFS folder, enshure that environmant variable is set correctly
COLOR 0C
CALL :fail
goto end
)

echo Copy POCO

if EXIST "%POCO_HOME%\." (
copy "%POCO_HOME%\bin\PocoFoundation.dll" PocoFoundation.dll
) ELSE (
echo Can't find POCO folder, enshure that environmant variable is set correctly
COLOR 0C
CALL :fail
goto end
)

echo Copy Lua

if EXIST "%LUA_DEV%\." (
copy "%LUA_DEV%\lua5.1.dll" lua51.dll
) ELSE (
echo Can't find LUA folder, enshure that environmant variable is set correctly
COLOR 0C
CALL :fail
goto end
)

echo Copy Ogg

if EXIST "%OGGDIR%\Win32\Release\libogg.dll" (
copy "%OGGDIR%\Win32\Release\libogg.dll" libogg.dll
)
if EXIST "%OGGDIR%\Release\libogg.dll" (
copy "%OGGDIR%\Release\libogg.dll" libogg.dll
)
if EXIST "%OGGDIR%\libogg.dll" (
copy "%OGGDIR%\libogg.dll" libogg.dll
)
if not EXIST "%OGGDIR%\." (
echo Can't find OGG folder, enshure that environmant variable is set correctly
COLOR 0C
CALL :fail
goto end
)

echo Copy Vorbis

if EXIST "%VORBISDIR%\Win32\Release\libvorbis.dll" (
copy "%VORBISDIR%\Win32\Release\libvorbis.dll" libvorbis.dll
copy "%VORBISDIR%\Win32\Release\libvorbisfile.dll" libvorbisfile.dll
)
if EXIST "%VORBISDIR%\Release\libvorbis.dll" (
copy "%VORBISDIR%\Release\libvorbis.dll" libvorbis.dll
copy "%VORBISDIR%\Release\libvorbisfile.dll" libvorbisfile.dll
)
if EXIST "%VORBISDIR%\libvorbis.dll" (
copy "%VORBISDIR%\libvorbis.dll" libvorbis.dll
copy "%VORBISDIR%\libvorbisfile.dll" libvorbisfile.dll
)
if not EXIST "%VORBISDIR%\." (
echo Can't find Vorbis folder, enshure that environmant variable is set correctly
COLOR 0C
CALL :fail
goto end
)

echo Copy GetText libraries

if EXIST "%programfiles%\GnuWin32\bin\." (
copy "%programfiles%\GnuWin32\bin\libgettextlib.dll" libgettextlib.dll
copy "%programfiles%\GnuWin32\bin\libgettextpo.dll" libgettextpo.dll
copy "%programfiles%\GnuWin32\bin\libgettextsrc.dll" libgettextsrc.dll
copy "%programfiles%\GnuWin32\bin\libiconv2.dll" libiconv2.dll
copy "%programfiles%\GnuWin32\bin\libintl3.dll" libintl3.dll
) ELSE (
echo Can't find GetText folder, enshure that environmant variable is set correctly
COLOR 0C
CALL :fail
goto end
)

if ERRORLEVEL==1 (
COLOR 0C
CALL :fail

) ELSE (

COLOR 0A
goto end
)

goto end

:check
echo Is this your SumWars source folder /Yes
set /p stry_svn= or maybe you are just installing the game second time /No
   
if /i "%stry_svn%" == "Y" (
  echo Copy sumwars.exe

  if EXIST "Release\sumwars.exe" ( copy "Release\sumwars.exe" sumwars.exe
  ) ELSE ( echo Looks like you didn't compile sumwars.exe
         goto end
         )
		 
   ) ELSE (
   CALL :AskSVN
)
goto :eof


:AskSVN
echo Please provide where is your SumWars source folder
set /p svn_repo= Don't add \ at the end of path 
if not EXIST "%svn_repo%\plugins_win.cfg" (

  echo You have choosen wrong directory
  CALL :AskSVN
) ELSE (
   CALL :AskDiff
   )
goto :eof


:AskDiff
 set /p buildsame= Is your build folder the same as source folder 
 
     if /i not "%buildsame%" == "Y" ( CALL :AskBuild
     ) ELSE (
	 set build=%svn_repo%
     CALL :copyexe
	 CALL :notSVNDir
     )
goto :eof

:AskBuild
echo Please provide your SumWars build folder
set /p build= Don't add \ at the end of path 
if not EXIST "%build%\Release\sumwars.exe" (

echo You have choosen wrong directory
CALL :AskBuild
) ELSE (
CALL :copyexe
)

CALL :notSVNDir
goto :eof

:notSVNDir
echo Copy misc files

copy "%svn_repo%\Authors" Authors
copy "%svn_repo%\authors.txt" authors.txt
copy "%svn_repo%\gpl-3.0.txt" gpl-3.0.txt
copy "%svn_repo%\Copying" Copying
copy "%svn_repo%\Install" Install
copy "%svn_repo%\Readme" Readme
copy "%svn_repo%\resources.cfg" resources.cfg
copy "%svn_repo%\plugins_win.cfg" plugins_win.cfg
xcopy "%svn_repo%\data\*" data /s /i
xcopy "%svn_repo%\resources\*" resources /s /i
xcopy "%svn_repo%\translation\*" translation /s /i
goto :eof

:copyexe
if EXIST "%build%\Release\sumwars.exe" (
echo Copy sumwars.exe
copy "%build%\Release\sumwars.exe" sumwars.exe
) ELSE (
echo Looks like you didn't compile sumwars.exe
COLOR 0C
CALL :fail
goto end)
goto :eof

:fail
echo Some files were not copied, sorry. Most likely game will not work
goto :eof


:end
echo Please delete "Plugin=Plugin_BSPSceneManager" line from plugins_win.cfg
echo and change PluginFolder=lib to PluginFolder=.

echo That's all

pause