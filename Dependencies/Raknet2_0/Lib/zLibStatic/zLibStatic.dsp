# Microsoft Developer Studio Project File - Name="zLibStatic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=zLibStatic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zLibStatic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zLibStatic.mak" CFG="zLibStatic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zLibStatic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "zLibStatic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zLibStatic - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\..\..\zLibStaticDebug.lib" 
# ADD LIB32 /nologo /out:"Debug\..\..\zLibStaticDebug.lib" 

!ELSEIF  "$(CFG)" == "zLibStatic - Win32 Release"

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
# ADD BASE CPP /nologo /MT /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD CPP /nologo /MT /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\..\..\zLibStatic.lib" 
# ADD LIB32 /nologo /out:"Release\..\..\zLibStatic.lib" 

!ENDIF

# Begin Target

# Name "zLibStatic - Win32 Debug"
# Name "zLibStatic - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=..\..\zLib\adler32.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\deflate.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\gzio.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\infback.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\inffast.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\inflate.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\trees.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\..\zLib\zutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=..\..\zLib\crc32.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\deflate.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\inffast.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\inflate.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\trees.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\zconf.in.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\zLib\zutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\zLibStatic.txt
# End Source File
# End Target
# End Project

