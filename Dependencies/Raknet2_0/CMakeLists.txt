#RakNet

PROJECT(RAKNET)

CMAKE_MINIMUM_REQUIRED( VERSION 2.8.0)

SET(RakNet_SRC
./Source/PacketPool.cpp
./Source/GetTime.cpp
./Source/StringCompressor.cpp
./Source/RakNetStatistics.cpp
./Source/DataBlockEncryptor.cpp
./Source/ExtendedOverlappedPool.cpp
./Source/AsynchronousFileIO.cpp
./Source/RakClient.cpp
./Source/SHA1.cpp
./Source/BitStream.cpp
./Source/EncodeClassName.cpp
./Source/CheckSum.cpp
./Source/InternalPacketPool.cpp
./Source/HuffmanEncodingTreeFactory.cpp
./Source/RakPeer.cpp
./Source/DistributedNetworkObjectManager.cpp
./Source/SocketLayer.cpp
./Source/NetworkObject.cpp
./Source/NetworkTypes.cpp
./Source/RakServer.cpp
./Source/Rand.cpp
./Source/DistributedNetworkObject.cpp
./Source/SimpleMutex.cpp
./Source/RPCNode.cpp
./Source/ReliabilityLayer.cpp
./Source/AES128.cpp
./Source/DistributedNetworkObjectStub.cpp
./Source/RakNetworkFactory.cpp
./Source/HuffmanEncodingTree.cpp
)

SET(RakNet_INCLUDES ${PROJECT_SOURCE_DIR}/Include)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

#SET(RakNet_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/libRakNet.a)
#MESSAGE(${RakNet_LIBRARY})

INCLUDE_DIRECTORIES(${RakNet_INCLUDES})

ADD_LIBRARY(RakNet STATIC ${RakNet_INCLUDES} ${RakNet_SRC})