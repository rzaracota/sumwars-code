# Microsoft Developer Studio Project File - Name="DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DLL.mak" CFG="DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DLL - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /I "./../../Include" /ZI /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "DLL_EXPORTS" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD CPP /nologo /MTd /I "./../../Include" /ZI /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "DLL_EXPORTS" /D "_MBCS" /Gm PRECOMP_VC7_TOBEREMOVED /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /out:"Debug\..\..\RakNetDebug.dll" /incremental:yes /debug /pdb:"Debug\DLL.pdb" /pdbtype:sept /subsystem:windows /implib:"$(OutDir)/../../RakNetDLLDebug.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /out:"Debug\..\..\RakNetDebug.dll" /incremental:yes /debug /pdb:"Debug\DLL.pdb" /pdbtype:sept /subsystem:windows /implib:"$(OutDir)/../../RakNetDLLDebug.lib" /machine:ix86 

!ELSEIF  "$(CFG)" == "DLL - Win32 Release"

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
# ADD BASE CPP /nologo /MT /I "./../../Include" /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "DLL_EXPORTS" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD CPP /nologo /MT /I "./../../Include" /Zi /W3 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "DLL_EXPORTS" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /out:"Release\..\..\RakNet.dll" /incremental:no /pdbtype:sept /subsystem:windows /opt:ref /opt:icf /implib:"$(OutDir)/../../RakNetDLL.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /out:"Release\..\..\RakNet.dll" /incremental:no /pdbtype:sept /subsystem:windows /opt:ref /opt:icf /implib:"$(OutDir)/../../RakNetDLL.lib" /machine:ix86 

!ENDIF

# Begin Target

# Name "DLL - Win32 Debug"
# Name "DLL - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=..\..\Source\AES128.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\AsynchronousFileIO.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\BitStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\CheckSum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\DataBlockEncryptor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\DistributedNetworkObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\DistributedNetworkObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\DistributedNetworkObjectStub.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\EncodeClassName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\ExtendedOverlappedPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\GetTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\HuffmanEncodingTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\HuffmanEncodingTreeFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\InternalPacketPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\NetworkObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\NetworkTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\PacketPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakClient.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakNetStatistics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakNetworkFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakPeer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\RakServer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\rand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\ReliabilityLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\RPCNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\SHA1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\SimpleMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\SocketLayer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Source\StringCompressor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=..\..\Include\AES128.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ArrayList.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\AsynchronousFileIO.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\BigTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\BinarySearchTree.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\BitStream.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\CheckSum.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ClientContextStruct.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DataBlockEncryptor.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DistributedNetworkObject.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DistributedNetworkObjectHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DistributedNetworkObjectManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\DistributedNetworkObjectStub.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\EncodeClassName.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ExtendedOverlappedPool.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\GetTime.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\HuffmanEncodingTree.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\HuffmanEncodingTreeFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\HuffmanEncodingTreeNode.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\InternalPacket.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\InternalPacketPool.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\LinkedList.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\MTUSize.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Multiplayer.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\NetworkObject.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\NetworkTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PacketEnumerations.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PacketPool.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\PacketPriority.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Queue.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\QueueLinkedList.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RakClient.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RakClientInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RakNetStatistics.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RakNetworkFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RakPeer.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RakPeerInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RakServer.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RakServerInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Rand.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\ReliabilityLayer.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RPCNode.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\RSACrypt.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\SHA1.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\SimpleMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\SocketLayer.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\StringCompressor.h
# End Source File
# Begin Source File

SOURCE=..\..\Include\Types.h
# End Source File
# End Group
# End Target
# End Project

