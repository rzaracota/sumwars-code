// ----------------------------------------------------------------------
// This API and the code herein created by and wholly and privately owned by Kevin Jenkins except where specifically indicated otherwise.
// Licensed under the "RakNet" brand by Rakkarsoft LLC and subject to the terms of the relevant licensing agreement available at http://www.rakkarsoft.com
// NetworkObject.cpp
// Created by Kevin Jenkins (rakkar@rakkar.org) January 24, 2003
// Implementation of the NetworkObject class.  Replace the commented
// error and warning messages with your own debug implementations
// ----------------------------------------------------------------------

#include "NetworkObject.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"

extern RakServerInterface* rakServer;
extern RakClientInterface* rakClient;

// Note you will need to save and load this if your game supports saving and loading so you start at the same index you left off.
// If you don't do this you can overwrite indices
unsigned short NetworkObject::staticItemID=0;
BasicDataStructures::AVLBalancedBinarySearchTree<ObjectIDNode> NetworkObject::IDTree;

int operator==(const ObjectIDNode& left, const ObjectIDNode& right)
{
	if (left.objectID == right.objectID) return !0;
	return 0;
}

int operator > (const ObjectIDNode& left, const ObjectIDNode& right)
{
	if (left.objectID > right.objectID) return !0;
	return 0;
}

int operator < (const ObjectIDNode& left, const ObjectIDNode& right)
{
	if (left.objectID < right.objectID) return !0;
	return 0;
}


ObjectIDNode::ObjectIDNode() {object=0;}
ObjectIDNode::ObjectIDNode(unsigned short ObjectID, NetworkObject *Object) {objectID = ObjectID; object = Object;}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NetworkObject::NetworkObject()
{
	// Server mode or single player mode
	if (rakServer->IsActive() || rakClient->IsConnected() == false)
	{
		NetworkObject* collision;

		serverAssignedID=true;
		do
		{
			objectID = staticItemID++;
			collision = GET_OBJECT_FROM_ID(objectID);
		} while (collision);
		IDTree.add(ObjectIDNode(objectID, this));
	}
	else
		serverAssignedID=false;
}

//-------------------------------------------------------------------------------------

NetworkObject::~NetworkObject()
{
	if (serverAssignedID)
	{
		NetworkObject *object = GET_OBJECT_FROM_ID(objectID);
		if (object == this)
			IDTree.del(ObjectIDNode(objectID, 0));
		// else
		// 	printf("Warning: Deleting object with ID %i that does not match the object with that ID in the tree.  Leaving the existing node in the tree.  Possible cause: Assigning the ID of this object to another object and then deleting this object.  Correct action: Delete this object before assigning its ID to another object.", objectID);
	}
}

//////////////////////////////////////////////////////////////////////
// Public Methods
//////////////////////////////////////////////////////////////////////

unsigned short NetworkObject::GetID(void) const
{
	// Dedicated client, hasn't had the ID assigned yet
	if (serverAssignedID==false && rakClient->IsConnected() && rakServer->IsActive()==false)
		return UNASSIGNED_OBJECT_ID;
	else
		return objectID;
};

//-------------------------------------------------------------------------------------

unsigned short NetworkObject::GetStaticItemID(void) {return staticItemID;}

//-------------------------------------------------------------------------------------

void NetworkObject::SetStaticItemID(unsigned short i) {staticItemID = i;}

//-------------------------------------------------------------------------------------

void NetworkObject::SetID(unsigned short id)
{
	if (id==UNASSIGNED_OBJECT_ID)
	{
		// puts("Warning: NetworkObject passed UNASSIGNED_OBJECT_ID.  SetID ignored");
		return;
	}

	if (serverAssignedID==true && objectID == id)
	{
		// printf("NetworkObject passed %i which already exists in the tree.  SetID ignored", id);
		return;
	}

	NetworkObject* collision = GET_OBJECT_FROM_ID(id);
	if (collision) // Tree should have only unique values
	{
		//printf("Warning: NetworkObject::SetID passed %i, which has an existing node in the tree.  Old node removed, which will cause the item pointed to to be inaccessible to the network", id);
		IDTree.del(ObjectIDNode(id, collision));
		collision->serverAssignedID=false;
	}
	
	if (serverAssignedID==false || objectID==UNASSIGNED_OBJECT_ID) // Object has not had an ID assigned so does not already exist in the tree
	{
		objectID = id;
		IDTree.add(ObjectIDNode(objectID, this));
	}
	else // Object already exists in the tree and has an assigned ID
	{
		IDTree.del(ObjectIDNode(objectID, this)); // Delete the node with whatever ID the existing object is using
		objectID = id;
		IDTree.add(ObjectIDNode(objectID, this));
	}

	serverAssignedID=true;
}

//-------------------------------------------------------------------------------------

NetworkObject* GET_OBJECT_FROM_ID(unsigned short x) {if (x==UNASSIGNED_OBJECT_ID) return 0; ObjectIDNode *n = NetworkObject::IDTree.get_pointer_to_node(ObjectIDNode((x), 0)); if (n) return n->object; return 0;}


//////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////