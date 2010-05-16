// ----------------------------------------------------------------------
// RakNet version 1.032
// MasterCommon.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) January 24, 2003
// Common code between the master server and the master client
// ----------------------------------------------------------------------

#include "MasterCommon.h"
#include <string.h>

// We differentiate servers by IP and port
int operator==(const MasterServerDataListStruct& left, const MasterServerDataListStruct& right)
{
	if (strcmp(left.object->IP, right.object->IP)==0 && left.object->port==right.object->port) return !0;
	return 0;
}

int operator > (const MasterServerDataListStruct& left, const MasterServerDataListStruct& right)
{
	if (strcmp(left.object->IP, right.object->IP)>0 || left.object->port>right.object->port) return !0;
	return 0;
}

int operator < (const MasterServerDataListStruct& left, const MasterServerDataListStruct& right)
{
	if (strcmp(left.object->IP, right.object->IP)<0 || left.object->port<right.object->port) return !0;
	return 0;
}

MasterServerDataListStruct::MasterServerDataListStruct() {object=0;}
MasterServerDataListStruct::MasterServerDataListStruct(MasterServerDataStruct *Object) {object = Object;}

//-------------------------------------------------------------------------------------------------------
// Get the number of servers returned
//-------------------------------------------------------------------------------------------------------
unsigned long MasterCommon::GetMasterServerDataListSize(void)
{
	return masterGameDataList.size();
}

//-------------------------------------------------------------------------------------------------------
// Get the master server data at the specified index.  This may change during updates
//-------------------------------------------------------------------------------------------------------
MasterServerDataStruct * MasterCommon::GetMasterServerDataAtIndex(unsigned long i)
{
	if (GetMasterServerDataListSize()< i)
		return 0;

	// Copy the actual data portion for the user
	return masterGameDataList[i].object;

}

//-------------------------------------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------------------------------------
MasterCommon::~MasterCommon()
{
	ClearMasterServerList();
}

//-------------------------------------------------------------------------------------------------------
// Clear out the server list
//-------------------------------------------------------------------------------------------------------
void MasterCommon::ClearMasterServerList(void)
{
	if (GetMasterServerDataListSize()==0)
		return;

	// Clear the list.
	for (unsigned long i=0; i < masterGameDataList.size(); i++)
		delete masterGameDataList[i].object;
	masterGameDataList.clear();
}

