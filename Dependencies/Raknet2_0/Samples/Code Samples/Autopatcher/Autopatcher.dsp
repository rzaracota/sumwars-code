# Microsoft Developer Studio Project File - Name="Autopatcher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Autopatcher - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Autopatcher.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Autopatcher.mak" CFG="Autopatcher - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Autopatcher - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Autopatcher - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Autopatcher - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /I "./../../../Include/Autopatcher" /I "./../../../Include" /I "./../../../zLib" /I "./../../../Source/Autopatcher" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD CPP /nologo /MTd /I "./../../../Include/Autopatcher" /I "./../../../Include" /I "./../../../zLib" /I "./../../../Source/Autopatcher" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ./../../../lib/RakNetLibStaticDebug.lib ./../../../lib/zLibStatic.lib ws2_32.lib /nologo /out:"Debug\Autopatcher.exe" /incremental:yes /debug /pdb:"Debug\Autopatcher.pdb" /pdbtype:sept /subsystem:console /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ./../../../lib/RakNetLibStaticDebug.lib ./../../../lib/zLibStatic.lib ws2_32.lib /nologo /out:"Debug\Autopatcher.exe" /incremental:yes /debug /pdb:"Debug\Autopatcher.pdb" /pdbtype:sept /subsystem:console /machine:ix86 

!ELSEIF  "$(CFG)" == "Autopatcher - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "./../../../Include/Autopatcher" /I "./../../../Include" /I "./../../../zLib" /I "./../../../Source/Autopatcher" /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD CPP /nologo /MT /I "./../../../Include/Autopatcher" /I "./../../../Include" /I "./../../../zLib" /I "./../../../Source/Autopatcher" /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ./../../../lib/RakNetLibStatic.lib  ./../../../lib/zLibStatic.lib ws2_32.lib /nologo /out:"Release\Autopatcher.exe" /incremental:no /debug /pdbtype:sept /subsystem:console /opt:ref /opt:icf /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ./../../../lib/RakNetLibStatic.lib  ./../../../lib/zLibStatic.lib ws2_32.lib /nologo /out:"Release\Autopatcher.exe" /incremental:no /debug /pdbtype:sept /subsystem:console /opt:ref /opt:icf /machine:ix86 

!ENDIF

# Begin Target

# Name "Autopatcher - Win32 Debug"
# Name "Autopatcher - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=..\..\..\Source\Autopatcher\Autopatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\AutopatcherTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Source\Autopatcher\DownloadableFileDescriptor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=..\..\..\include\autopatcher\Autopatcher.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\autopatcher\DownloadableFileDescriptor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\zLib\zlib.h
# End Source File
# End Group
# End Target
# End Project

