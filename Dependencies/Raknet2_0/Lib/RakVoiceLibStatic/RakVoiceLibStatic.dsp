# Microsoft Developer Studio Project File - Name="RakVoiceLibStatic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RakVoiceLibStatic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RakVoiceLibStatic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RakVoiceLibStatic.mak" CFG="RakVoiceLibStatic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RakVoiceLibStatic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "RakVoiceLibStatic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RakVoiceLibStatic - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /I "./../../Include" /I "./../../Include/RakVoice" /I "./../../speex-1.1.4/src" /I "./../../speex-1.1.4/libspeex" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD CPP /nologo /MTd /I "./../../Include" /I "./../../Include/RakVoice" /I "./../../speex-1.1.4/src" /I "./../../speex-1.1.4/libspeex" /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\..\..\RakVoiceLibStaticDebug.lib" 
# ADD LIB32 /nologo /out:"Debug\..\..\RakVoiceLibStaticDebug.lib" 

!ELSEIF  "$(CFG)" == "RakVoiceLibStatic - Win32 Release"

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
# ADD BASE CPP /nologo /MT /I "./../../Include" /I "./../../Include/RakVoice" /I "./../../speex-1.1.4/src" /I "./../../speex-1.1.4/libspeex" /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD CPP /nologo /MT /I "./../../Include" /I "./../../Include/RakVoice" /I "./../../speex-1.1.4/src" /I "./../../speex-1.1.4/libspeex" /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\..\..\RakVoiceLibStatic.lib" 
# ADD LIB32 /nologo /out:"Release\..\..\RakVoiceLibStatic.lib" 

!ENDIF

# Begin Target

# Name "RakVoiceLibStatic - Win32 Debug"
# Name "RakVoiceLibStatic - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=..\..\Source\BitStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\GetTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\NetworkTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakVoice\RakVoice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakVoice\RakVoiceFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\SimpleMutex.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=..\..\Source\BitStream.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\GetTime.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakVoice\RakVoice.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakVoice\RakVoiceFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakVoice\RakVoiceInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Source\SimpleMutex.h
# End Source File
# End Group
# Begin Group "Speex"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\arch.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\bits.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\cb_search.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\cb_search.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\cb_search_sse.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\denoise.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\exc_10_16_table.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\exc_10_32_table.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\exc_20_32_table.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\exc_5_256_table.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\exc_5_64_table.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\exc_8_128_table.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\filters.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\filters.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\filters_sse.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\fixed_arm.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\fixed_debug.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\fixed_generic.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\gain_table.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\gain_table_lbr.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\hexc_10_32_table.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\hexc_table.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\high_lsp_tables.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\jitter.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\lbr_48k_tables.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\lpc.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\lpc.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\lsp.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\lsp.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\lsp_tables_nb.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\ltp.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\ltp.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\ltp_sse.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\math_approx.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\math_approx.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\mdf.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\misc.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\misc.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\modes.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\modes.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\nb_celp.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\nb_celp.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\quant_lsp.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\quant_lsp.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\sb_celp.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\sb_celp.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\smallft.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\smallft.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\speex.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\speex_bits.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\speex_callbacks.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\speex_callbacks.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\speex_echo.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\speex_header.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\speex_header.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\speex_jitter.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\speex_stereo.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\stack_alloc.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\stereo.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\vbr.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\vbr.h
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\vq.c
# End Source File
# Begin Source File

SOURCE=..\..\speex-1.1.4\libspeex\vq.h
# End Source File
# End Group
# End Target
# End Project

