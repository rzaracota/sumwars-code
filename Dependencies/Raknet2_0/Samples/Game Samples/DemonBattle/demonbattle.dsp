# Microsoft Developer Studio Project File - Name="Demon Battle" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Demon Battle - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Demon Battle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Demon Battle.mak" CFG="Demon Battle - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Demon Battle - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Demon Battle - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Demon Battle - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\DemonBattleGame"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DemonBattleGame"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MLd /I ".\src\RakNet" /I ".\common\include" /Zi /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /EHsc /YX /Fp".\Debug/demonbattle.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD CPP /nologo /MLd /I ".\src\RakNet" /I ".\common\include" /Zi /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /EHsc /YX /Fp".\Debug/demonbattle.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\demonbattle.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\demonbattle.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" 
# ADD RSC /l 1033 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dxof.lib dxguid.lib d3dx9dt.lib d3d9.lib winmm.lib dsound.lib dinput8.lib Ws2_32.lib dxerr9.lib /nologo /out:".\DemonBattleGame\DemonBattle.exe" /incremental:yes /debug /pdb:".\Debug\demonbattle.pdb" /pdbtype:sept /subsystem:windows /stack:2097152,2097152 /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dxof.lib dxguid.lib d3dx9dt.lib d3d9.lib winmm.lib dsound.lib dinput8.lib Ws2_32.lib dxerr9.lib /nologo /out:".\DemonBattleGame\DemonBattle.exe" /incremental:yes /debug /pdb:".\Debug\demonbattle.pdb" /pdbtype:sept /subsystem:windows /stack:2097152,2097152 /MACHINE:I386

!ELSEIF  "$(CFG)" == "Demon Battle - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\DemonBattleGame"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\DemonBattleGame"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /I ".\src\RakNet" /I ".\common\include" /W3 /Og /Ob2 /Oi /Ot /GT /GA /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /GF /EHsc /Gy PRECOMP_VC7_TOBEREMOVED /Fp".\Release/demonbattle.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD CPP /nologo /ML /I ".\src\RakNet" /I ".\common\include" /W3 /Og /Ob2 /Oi /Ot /GT /GA /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /GF /EHsc /Gy PRECOMP_VC7_TOBEREMOVED /Fp".\Release/demonbattle.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD BASE MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\demonbattle.tlb" /win32 
# ADD MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\demonbattle.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" 
# ADD RSC /l 1033 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dxof.lib dxguid.lib d3dx9.lib d3d9.lib winmm.lib dsound.lib dinput8.lib Ws2_32.lib dxerr9.lib /nologo /out:".\DemonBattleGame\DemonBattle.exe" /incremental:no /pdb:".\Release\demonbattle.pdb" /pdbtype:sept /subsystem:windows /stack:2097152,2097152 /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dxof.lib dxguid.lib d3dx9.lib d3d9.lib winmm.lib dsound.lib dinput8.lib Ws2_32.lib dxerr9.lib /nologo /out:".\DemonBattleGame\DemonBattle.exe" /incremental:no /pdb:".\Release\demonbattle.pdb" /pdbtype:sept /subsystem:windows /stack:2097152,2097152 /MACHINE:I386

!ENDIF

# Begin Target

# Name "Demon Battle - Win32 Debug"
# Name "Demon Battle - Win32 Release"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\directx.ico
# End Source File
# Begin Source File

SOURCE=icon1.ico
# End Source File
# Begin Source File

SOURCE=resource.h
# End Source File
# Begin Source File

SOURCE=.\WinMain.rc
# End Source File
# End Group
# Begin Group "Common Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common\src\d3dapp.cpp
# End Source File
# Begin Source File

SOURCE=.\common\include\d3dapp.h
# End Source File
# Begin Source File

SOURCE=Common\src\d3denumeration.cpp
# End Source File
# Begin Source File

SOURCE=Common\include\d3denumeration.h
# End Source File
# Begin Source File

SOURCE=Common\src\d3dsettings.cpp
# End Source File
# Begin Source File

SOURCE=Common\include\d3dsettings.h
# End Source File
# Begin Source File

SOURCE=.\common\src\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\common\include\d3dutil.h
# End Source File
# Begin Source File

SOURCE=Common\src\dsutil.cpp
# End Source File
# Begin Source File

SOURCE=Common\include\dsutil.h
# End Source File
# Begin Source File

SOURCE=.\common\src\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\common\include\dxutil.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=src\AI.h
# End Source File
# Begin Source File

SOURCE=src\AnimationGenerator.h
# End Source File
# Begin Source File

SOURCE=src\Base3D.h
# End Source File
# Begin Source File

SOURCE=src\Bitmap.h
# End Source File
# Begin Source File

SOURCE=src\BitmapWidget.h
# End Source File
# Begin Source File

SOURCE=src\ChatColors.h
# End Source File
# Begin Source File

SOURCE=src\ChatMessageContainer.h
# End Source File
# Begin Source File

SOURCE=src\Collidable.h
# End Source File
# Begin Source File

SOURCE=src\Creature.h
# End Source File
# Begin Source File

SOURCE=src\CreditsState.h
# End Source File
# Begin Source File

SOURCE=src\D3DHeaders.h
# End Source File
# Begin Source File

SOURCE=src\Demon.h
# End Source File
# Begin Source File

SOURCE=src\DemonBattleMultiplayer.h
# End Source File
# Begin Source File

SOURCE=src\DirectSound.h
# End Source File
# Begin Source File

SOURCE=src\Entity.h
# End Source File
# Begin Source File

SOURCE=src\FileLoadedVisibleEntity.h
# End Source File
# Begin Source File

SOURCE=src\Fireball.h
# End Source File
# Begin Source File

SOURCE=src\FireballSpell.h
# End Source File
# Begin Source File

SOURCE=src\GameState.h
# End Source File
# Begin Source File

SOURCE=src\HostGameState.h
# End Source File
# Begin Source File

SOURCE=src\INI.H
# End Source File
# Begin Source File

SOURCE=src\InputSystem.h
# End Source File
# Begin Source File

SOURCE=src\InstructionsState.h
# End Source File
# Begin Source File

SOURCE=src\JoinGameState.h
# End Source File
# Begin Source File

SOURCE=src\LightedSphere.h
# End Source File
# Begin Source File

SOURCE=src\LightSource.h
# End Source File
# Begin Source File

SOURCE=src\LightSourceManager.h
# End Source File
# Begin Source File

SOURCE=src\MainMenuState.h
# End Source File
# Begin Source File

SOURCE=src\ManagerState.h
# End Source File
# Begin Source File

SOURCE=src\Math.h
# End Source File
# Begin Source File

SOURCE=src\MD2Constants.h
# End Source File
# Begin Source File

SOURCE=src\md2model.h
# End Source File
# Begin Source File

SOURCE=src\MemoryManager.h
# End Source File
# Begin Source File

SOURCE=src\ModelContainer.h
# End Source File
# Begin Source File

SOURCE=src\ModelEnum.h
# End Source File
# Begin Source File

SOURCE=src\MouseCursor.h
# End Source File
# Begin Source File

SOURCE=src\new_off.h
# End Source File
# Begin Source File

SOURCE=src\new_on.h
# End Source File
# Begin Source File

SOURCE=src\ParticleSystem.h
# End Source File
# Begin Source File

SOURCE=src\ParticleSystemManager.h
# End Source File
# Begin Source File

SOURCE=src\Perks.h
# End Source File
# Begin Source File

SOURCE=src\Player.h
# End Source File
# Begin Source File

SOURCE=src\Projectile.h
# End Source File
# Begin Source File

SOURCE=src\rand.h
# End Source File
# Begin Source File

SOURCE=src\SoundContainer.h
# End Source File
# Begin Source File

SOURCE=src\SoundEnum.h
# End Source File
# Begin Source File

SOURCE=src\Spell.h
# End Source File
# Begin Source File

SOURCE=src\State.h
# End Source File
# Begin Source File

SOURCE=src\SummonDemonSpell.h
# End Source File
# Begin Source File

SOURCE=src\TerrainMesh.h
# End Source File
# Begin Source File

SOURCE=src\Text.h
# End Source File
# Begin Source File

SOURCE=src\TextInputWidget.h
# End Source File
# Begin Source File

SOURCE=src\TextListContainer.h
# End Source File
# Begin Source File

SOURCE=src\TextManager.h
# End Source File
# Begin Source File

SOURCE=src\textureman.h
# End Source File
# Begin Source File

SOURCE=src\TextWidget.h
# End Source File
# Begin Source File

SOURCE=src\TOOLZ.H
# End Source File
# Begin Source File

SOURCE=src\User.h
# End Source File
# Begin Source File

SOURCE=src\VisibleEntity.h
# End Source File
# Begin Source File

SOURCE=src\Widget.h
# End Source File
# Begin Source File

SOURCE=src\XFile.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=src\AI.cpp
# End Source File
# Begin Source File

SOURCE=src\AnimationGenerator.cpp
# End Source File
# Begin Source File

SOURCE=src\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=src\BitmapWidget.cpp
# End Source File
# Begin Source File

SOURCE=src\ChatMessageContainer.cpp
# End Source File
# Begin Source File

SOURCE=src\Collidable.cpp
# End Source File
# Begin Source File

SOURCE=src\Creature.cpp
# End Source File
# Begin Source File

SOURCE=src\CreditsState.cpp
# End Source File
# Begin Source File

SOURCE=src\Demon.cpp
# End Source File
# Begin Source File

SOURCE=src\DemonBattleMultiplayer.cpp
# End Source File
# Begin Source File

SOURCE=src\DirectSound.cpp
# End Source File
# Begin Source File

SOURCE=src\Entity.cpp
# End Source File
# Begin Source File

SOURCE=src\FileLoadedVisibleEntity.cpp
# End Source File
# Begin Source File

SOURCE=src\Fireball.cpp
# End Source File
# Begin Source File

SOURCE=src\FireballSpell.cpp
# End Source File
# Begin Source File

SOURCE=src\GameState.cpp
# End Source File
# Begin Source File

SOURCE=src\HostGameState.cpp
# End Source File
# Begin Source File

SOURCE=src\INI.CPP
# End Source File
# Begin Source File

SOURCE=src\InputSystem.cpp
# End Source File
# Begin Source File

SOURCE=src\InstructionsState.cpp
# End Source File
# Begin Source File

SOURCE=src\JoinGameState.cpp
# End Source File
# Begin Source File

SOURCE=src\LightedSphere.cpp
# End Source File
# Begin Source File

SOURCE=src\LightSource.cpp
# End Source File
# Begin Source File

SOURCE=src\LightSourceManager.cpp
# End Source File
# Begin Source File

SOURCE=src\Main.cpp
# End Source File
# Begin Source File

SOURCE=src\MainMenuState.cpp
# End Source File
# Begin Source File

SOURCE=src\ManagerState.cpp
# End Source File
# Begin Source File

SOURCE=src\Math.cpp
# End Source File
# Begin Source File

SOURCE=src\MD2Model.cpp
# End Source File
# Begin Source File

SOURCE=src\MemoryManager.cpp
# End Source File
# Begin Source File

SOURCE=src\ModelContainer.cpp
# End Source File
# Begin Source File

SOURCE=src\MouseCursor.cpp
# End Source File
# Begin Source File

SOURCE=src\necrobattle.cpp
# End Source File
# Begin Source File

SOURCE=src\NetworkUpdate.cpp
# End Source File
# Begin Source File

SOURCE=src\ParticleSystem.cpp
# End Source File
# Begin Source File

SOURCE=src\ParticleSystemManager.cpp
# End Source File
# Begin Source File

SOURCE=src\Perks.cpp
# End Source File
# Begin Source File

SOURCE=src\Player.cpp
# End Source File
# Begin Source File

SOURCE=src\Projectile.cpp
# End Source File
# Begin Source File

SOURCE=src\rand.cpp
# End Source File
# Begin Source File

SOURCE=src\SoundContainer.cpp
# End Source File
# Begin Source File

SOURCE=src\SoundEnum.cpp
# End Source File
# Begin Source File

SOURCE=src\Spell.cpp
# End Source File
# Begin Source File

SOURCE=src\State.cpp
# End Source File
# Begin Source File

SOURCE=src\SummonDemonSpell.cpp
# End Source File
# Begin Source File

SOURCE=src\TerrainMesh.cpp
# End Source File
# Begin Source File

SOURCE=src\Text.cpp
# End Source File
# Begin Source File

SOURCE=src\TextInputWidget.cpp
# End Source File
# Begin Source File

SOURCE=src\TextListContainer.cpp
# End Source File
# Begin Source File

SOURCE=src\TextManager.cpp
# End Source File
# Begin Source File

SOURCE=src\textureman.cpp
# End Source File
# Begin Source File

SOURCE=src\TextWidget.cpp
# End Source File
# Begin Source File

SOURCE=src\TOOLZ.CPP
# End Source File
# Begin Source File

SOURCE=src\User.cpp
# End Source File
# Begin Source File

SOURCE=src\VisibleEntity.cpp
# End Source File
# Begin Source File

SOURCE=src\Widget.cpp
# End Source File
# Begin Source File

SOURCE=src\XFile.cpp
# End Source File
# End Group
# Begin Group "Raknet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\RakNet\RakNet.lib
# End Source File
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=src\RakNet\BitStream.cpp
# End Source File
# Begin Source File

SOURCE=src\RakNet\GetTime.cpp
# End Source File
# Begin Source File

SOURCE=src\RakNet\Multiplayer.cpp
# End Source File
# Begin Source File

SOURCE=src\RakNet\NetworkObject.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=src\RakNet\BinarySearchTree.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\BitStream.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\ClientServerFactory.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\DemonBattleTypes.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\GetTime.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\LinkedList.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\ArrayList.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\Multiplayer.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\NetworkObject.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\NetworkStructures.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\NetworkTypes.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\PacketEnumerations.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\PacketPriority.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\QueueLinkedList.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\RakClientInterface.h
# End Source File
# Begin Source File

SOURCE=src\RakNet\RakServerInterface.h
# End Source File
# End Group
# End Group
# End Target
# End Project

