@Echo off

Color 0B

Rem Clear the screen and write a header.
Cls
Echo *****************************************************************************
Echo *                                                                           * 
Echo *               Summoning Wars installation script                          *
Echo *It suggests that you have already built sumwars.exe Release executable file*
Echo *            It doesn't support DEBUG version of the game                   *
Echo *****************************************************************************

Rem Get the confirmation to copy the data.
Set /p install= Do you really want to install SumWars in current directory (Y/N)? 
If /i not "%install%" == "Y" (
  Echo Ok, I will not do this.
  GoTo end
) Else (
  Echo Let's do it
)

If not Exist "plugins_win.cfg" (
  Call :AskSVN
) Else ( 
  Call :check
)

Echo Copy ALUT

If Exist "%ALUTDIR%\." (
  Copy "%ALUTDIR%\lib\alut.dll" alut.dll
) Else (
  Echo Cannot find env var [ALUTDIR]
  Echo Please define it so that the file "[ALUTDIR]\lib\alut.dll" can be found. 
)

Echo Copy CEGUI

If Exist "%CEGUIDIR%\." (
  Copy "%CEGUIDIR%\bin\CEGUIBase.dll" CEGUIBase.dll
  Copy "%CEGUIDIR%\bin\CEGUIExpatParser.dll" CEGUIExpatParser.dll
  Copy "%CEGUIDIR%\bin\CEGUIFalagardWRBase.dll" CEGUIFalagardWRBase.dll
  Copy "%CEGUIDIR%\bin\CEGUILuaScriptModule.dll" CEGUILuaScriptModule.dll
  Copy "%CEGUIDIR%\bin\CEGUIOgreRenderer.dll" CEGUIOgreRenderer.dll
  Copy "%CEGUIDIR%\bin\CEGUISILLYImageCodec.dll" CEGUISILLYImageCodec.dll
  Rem also add the xml parser
  If Exist "%CEGUIDIR%\bin\CEGUITinyXMLParser.dll" (
    Copy "%CEGUIDIR%\bin\CEGUITinyXMLParser.dll" CEGUITinyXMLParser.dll
  )
) Else (
  Echo Can't find CEGUI folder, ensure that environmant variable is set correctly
  Color 0C
  Call :fail
  GoTo end
)

Echo OGRE libraries

If Exist "%OGRE_HOME%\." (
  Copy "%OGRE_HOME%\bin\release\cg.dll" cg.dll"
  Copy "%OGRE_HOME%\bin\release\OgreMain.dll" OgreMain.dll
  Copy "%OGRE_HOME%\bin\release\OgrePaging.dll" OgrePaging.dll
  Copy "%OGRE_HOME%\bin\release\OgreRTShaderSystem.dll" OgreRTShaderSystem.dll
  Copy "%OGRE_HOME%\bin\release\OgreTerrain.dll" OgreTerrain.dll
  Copy "%OGRE_HOME%\bin\release\OIS.dll" OIS.dll"
  Copy "%OGRE_HOME%\bin\release\Plugin_CgProgramManager.dll" Plugin_CgProgramManager.dll
  Copy "%OGRE_HOME%\bin\release\Plugin_OctreeSceneManager.dll" Plugin_OctreeSceneManager.dll
  Copy "%OGRE_HOME%\bin\release\Plugin_OctreeZone.dll" Plugin_OctreeZone.dll
  Copy "%OGRE_HOME%\bin\release\Plugin_ParticleFX.dll" Plugin_ParticleFX.dll
  Copy "%OGRE_HOME%\bin\release\Plugin_PCZSceneManager.dll" Plugin_PCZSceneManager.dll
  Copy "%OGRE_HOME%\bin\release\RenderSystem_Direct3D9.dll" RenderSystem_Direct3D9.dll
  Copy "%OGRE_HOME%\bin\release\RenderSystem_GL.dll" RenderSystem_GL.dll
) Else (
  Echo Can't find OGRE folder, please ensure that the environment variable [OGRE_HOME] is set correctly ^!
  Echo Please define it so that the file "[OGRE_HOME]\bin\release\OgreMain.dll" can be found. 
  Color 0C
  Call :fail
  GoTo end
)

Echo Copy PhysFS

If Exist "%PHYSFSDIR%" (
  If Exist "%PHYSFSDIR%\lib\." (
    Copy "%PHYSFSDIR%\lib\physfs.dll" physfs.dll
  )
  
  If Exist "%PHYSFSDIR%\bin\." (
    Copy "%PHYSFSDIR%\bin\physfs.dll" physfs.dll
  )
  
  If Exist "%PHYSFSDIR%\Release\." (
    Copy "%PHYSFSDIR%\Release\physfs.dll" physfs.dll
  )
) Else (
  Echo Cannot find [PHYSFSDIR] env var.
  Echo Please define it so that the file "[PHYSFSDIR]\Release\physfs.dll" can be found.
  Echo Or "[PHYSFSDIR]\lib\physfs.dll" can be found; OR "[PHYSFSDIR]\bin\physfs.dll" can be found.
)

If Not Exist physfs.dll (
  Echo Cannot find PHYSFS folder, please ensure that environment variable is set correctly
  Color 0C
  Call :fail
  GoTo end
)

Echo Copy POCO

If Exist "%POCO_HOME%\." (
  Copy "%POCO_HOME%\bin\PocoFoundation.dll" PocoFoundation.dll
) Else (
  Echo Can't find POCO folder, please ensure that the environment variable [POCO_HOME] is set correctly
  Echo Please define it so that the file "[POCO_HOME]\bin\PocoFoundation.dll" can be found.
  Echo Otherwise YOU CANNOT MAKE [DEBUG] MODE BUILDS
REM   Color 0C
REM   Call :fail
REM   GoTo end
)

Echo Copy Lua

If Exist "%LUA_DEV%\." (
  Rem Allow binding with and without the point. Should only use the latter though.
  Rem Only the first one was used initially... no idea why.
  Copy "%LUA_DEV%\lua5.1.dll" lua51.dll
  Copy "%LUA_DEV%\lua5.1.dll" lua5.1.dll
) Else (
  Echo Can't find LUA folder, please ensure that the environment variable [LUA_DEV] is set correctly
  Echo Please define it so that the file "[LUA_DEV]\lua5.1.dll" can be found.
  Color 0C
  Call :fail
  GoTo end
)

Echo Copy Ogg

If Exist "%OGGDIR%\." (
  If Exist "%OGGDIR%\Win32\Release\libogg.dll" (
    Copy "%OGGDIR%\Win32\Release\libogg.dll" libogg.dll
  )
  If Exist "%OGGDIR%\Release\libogg.dll" (
    Copy "%OGGDIR%\Release\libogg.dll" libogg.dll
  )
  If Exist "%OGGDIR%\libogg.dll" (
    Copy "%OGGDIR%\libogg.dll" libogg.dll
  )
) Else (
  Echo Can't find OGG folder, please ensure that the environment variable [OGGDIR] is set correctly
  Echo Please define it so that the file "[OGGDIR]\Win32\Release\libogg.dll" can be found.
  Echo OR "[OGGDIR]\Release\libogg.dll" can be found.
  Echo OR "[OGGDIR]\libogg.dll" can be found.
  Color 0C
  Call :fail
  GoTo end
)

Echo Copy Vorbis

If Exist "%VORBISDIR%\Win32\Release\libvorbis.dll" (
Copy "%VORBISDIR%\Win32\Release\libvorbis.dll" libvorbis.dll
Copy "%VORBISDIR%\Win32\Release\libvorbisfile.dll" libvorbisfile.dll
)
If Exist "%VORBISDIR%\Release\libvorbis.dll" (
Copy "%VORBISDIR%\Release\libvorbis.dll" libvorbis.dll
Copy "%VORBISDIR%\Release\libvorbisfile.dll" libvorbisfile.dll
)
If Exist "%VORBISDIR%\libvorbis.dll" (
Copy "%VORBISDIR%\libvorbis.dll" libvorbis.dll
Copy "%VORBISDIR%\libvorbisfile.dll" libvorbisfile.dll
)
If not Exist "%VORBISDIR%\." (
Echo Can't find Vorbis folder, enshure that environmant variable is set correctly
Color 0C
Call :fail
GoTo end
)

Echo Copy GetText libraries

If Exist "%GNUWINDIR%" (
  If Exist "%GNUWINDIR%\bin\." (
    Copy "%GNUWINDIR%\bin\libgettextlib.dll" libgettextlib.dll
    Copy "%GNUWINDIR%\bin\libgettextpo.dll" libgettextpo.dll
    Copy "%GNUWINDIR%\bin\libgettextsrc.dll" libgettextsrc.dll
    Copy "%GNUWINDIR%\bin\libiconv2.dll" libiconv2.dll
    Copy "%GNUWINDIR%\bin\libintl3.dll" libintl3.dll
  ) Else (
    Echo Cannot find [bin] subfolder: ["%GNUWINDIR%\bin\]. Corrupt installation?
  )  
) Else (
  Echo Cannot find env var for [GNUWINDIR]. Checking default path. [%programfiles%\GnuWin32]
  If Exist "%programfiles%\GnuWin32\bin\." (
    Copy "%programfiles%\GnuWin32\bin\libgettextlib.dll" libgettextlib.dll
    Copy "%programfiles%\GnuWin32\bin\libgettextpo.dll" libgettextpo.dll
    Copy "%programfiles%\GnuWin32\bin\libgettextsrc.dll" libgettextsrc.dll
    Copy "%programfiles%\GnuWin32\bin\libiconv2.dll" libiconv2.dll
    Copy "%programfiles%\GnuWin32\bin\libintl3.dll" libintl3.dll
  ) Else (
    Echo Can't find GetText folder, enshure that environmant variable is set correctly
    Color 0C
    Call :fail
    GoTo end
  )
)


If ERRORLEVEL==1 (
  Color 0C
  Call :fail
) Else (
  Color 0A
  GoTo end
)

GoTo end

:check
Echo Is this your SumWars source folder /Yes
Set /p stry_svn= or maybe you are just installing the game second time /No
   
If /i "%stry_svn%" == "Y" (
  Echo Copy sumwars.exe

  If Exist "Release\sumwars.exe" ( Copy "Release\sumwars.exe" sumwars.exe
  ) Else ( Echo Looks like you didn't compile sumwars.exe
         GoTo end
         )
		 
   ) Else (
   Call :AskSVN
)
GoTo :eof


:AskSVN
Echo Please provide where is your SumWars source folder
Set /p svn_repo= Don't add \ at the end of path 
If not Exist "%svn_repo%\plugins_win.cfg" (

  Echo You have choosen wrong directory
  Call :AskSVN
) Else (
   Call :AskDiff
   )
GoTo :eof


:AskDiff
 Set /p buildsame= Is your build folder the same as source folder 
 
     If /i not "%buildsame%" == "Y" ( Call :AskBuild
     ) Else (
	 set build=%svn_repo%
     Call :copyexe
	 Call :notSVNDir
     )
GoTo :eof

:AskBuild
Echo Please provide your SumWars build folder
Set /p build= Don't add \ at the end of path 
If not Exist "%build%\Release\sumwars.exe" (

Echo You have choosen wrong directory
Call :AskBuild
) Else (
Call :copyexe
)

Call :notSVNDir
GoTo :eof

:notSVNDir
Echo Copy misc files

Copy "%svn_repo%\Authors" Authors
Copy "%svn_repo%\authors.txt" authors.txt
Copy "%svn_repo%\gpl-3.0.txt" gpl-3.0.txt
Copy "%svn_repo%\Copying" Copying
Copy "%svn_repo%\Install" Install
Copy "%svn_repo%\Readme" Readme
Copy "%svn_repo%\resources.cfg" resources.cfg
Copy "%svn_repo%\plugins_win.cfg" plugins_win.cfg
xCopy "%svn_repo%\data\*" data /s /i
xCopy "%svn_repo%\resources\*" resources /s /i
xCopy "%svn_repo%\translation\*" translation /s /i
GoTo :eof

:copyexe
If Exist "%build%\Release\sumwars.exe" (
  Echo Copy sumwars.exe
  Copy "%build%\Release\sumwars.exe" sumwars.exe
) Else (
  Echo Current dir is [%CD%]
  If Exist "build\Release\sumwars.exe" (
    Copy "build\Release\sumwars.exe" sumwars.exe
  ) Else (
    Echo Looks like you didn't compile sumwars.exe
    Color 0C
    Call :fail
    GoTo end
  )
)
GoTo :eof

:fail
Echo Some files were not copied, sorry. Most likely game will not work
GoTo :eof


:end
Echo Please delete "Plugin=Plugin_BSPSceneManager" line from plugins_win.cfg
Echo and change PluginFolder=lib to PluginFolder=.

Echo That's all

pause