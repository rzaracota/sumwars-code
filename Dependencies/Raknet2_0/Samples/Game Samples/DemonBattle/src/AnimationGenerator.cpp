#include "AnimationGenerator.h"
#include "..\Common\Include\d3dutil.h"
#include <assert.h>

// How much time in ms to wait before using the next keyframe
static int DEFAULT_TIME_BETWEEN_KEYFRAMES=200;

//#include "MemoryManager.h"

AnimationGenerator::AnimationGenerator()
{
	loaded=false;
	md2Model=0;
}

AnimationGenerator::~AnimationGenerator()
{
	for (unsigned long i=0L; i < animationSequenceArray.size(); i++)
		delete animationSequenceArray[i];

	// The list will clear itself in its own destructor
}

// Set previousKeyframeIndex to currentKeyframeIndex, set currentKeyframeIndex to nextKeyframeIndex,
// set nextKeyframeIndex to nextNextKeyframeIndex,
// if (keyframesToNextAnimationIndex--==1) then set currentAnimationIndex to nextAnimationIndex.
// Calculate nextNextKeyframeIndex based on currentAnimationIndex, nextKeyframeIndex
void AnimationGenerator::IncrementKeyframeIndices(void)
{
	assert(loaded);
	int start = md2Model->animationDataArray[currentAnimationIndex]->start;
	int end = md2Model->animationDataArray[currentAnimationIndex]->end;

	// record currentKeyframeIndex going into this function
	int CK = currentKeyframeIndex;

	previousKeyframeIndex = currentKeyframeIndex;
	currentKeyframeIndex = nextKeyframeIndex;
	nextKeyframeIndex = nextNextKeyframeIndex;

	assert(nextAnimationIndex>=0);

	if (keyframesToNextAnimationIndex--==1)
	{
		currentAnimationIndex=nextAnimationIndex;
		animationSequenceArray[nextAnimationIndex]->elapsedTime=0L;
	}
	// if pauseAtEnd is true and keyframes is less than 0 then we automatically played an animation and want to stop updating at the end of it
	if (pauseAtEnd==false || nextNextKeyframeIndex<end-1)
	{
		nextNextKeyframeIndex=nextKeyframeIndex+1;
		if (nextNextKeyframeIndex==end)
			nextNextKeyframeIndex=start;
	}
	
	// Check if the user wanted to automatically play a new animation at the end of a particular sequence.  If so set to it
	// If we go into this function with the value of end-1, then we just finished that keyframe set
	if (CK==end-1 && keyframesToNextAnimationIndex<0)
	{
		if (autoCycleToNextAnimation>=0)
		{
			lastSetAnimation=autoCycleToNextAnimation;
			SetNewAnimation(autoCycleToNextAnimation);
			autoCycleToNextAnimation=-1;
			playingSingleCycleAnimation=false;
		}
//		else if (playingSingleCycleAnimation==true)
//			playingSingleCycleAnimation=false;
	}

	assert(nextAnimationIndex>=0);
}

// If the elapsed time for the current animation is such that the current keyframe is no longer the current keyframe, then call UpdateKeyframe
// Then render based on keyframe values
void AnimationGenerator::ComputeMeshData(unsigned long elapsedTime) 
{
	assert(loaded);
	animationSequenceArray[currentAnimationIndex]->elapsedTime += elapsedTime;
	
	while (animationSequenceArray[currentAnimationIndex]->elapsedTime > animationSequenceArray[currentAnimationIndex]->totalTime)
		animationSequenceArray[currentAnimationIndex]->elapsedTime-=animationSequenceArray[currentAnimationIndex]->totalTime;

	float computedKeyframe;
	int numberOfKeyframes=md2Model->animationDataArray[currentAnimationIndex]->end -md2Model->animationDataArray[currentAnimationIndex]->start;
	float elapsedPercentage = (float) animationSequenceArray[currentAnimationIndex]->elapsedTime / (float) animationSequenceArray[currentAnimationIndex]->totalTime;

	computedKeyframe = (float) md2Model->animationDataArray[currentAnimationIndex]->start + elapsedPercentage * numberOfKeyframes;
	if ((int)computedKeyframe != currentKeyframeIndex)
		IncrementKeyframeIndices();

	m_pd3dDevice->SetFVF (D3DFVF_MODELVERTEX);

	if (currentKeyframeIndex!=nextKeyframeIndex) // Is there a next keyframe or do we stay on the same one? (if stay the same then no animation for this action and we don't need to interpolate)
	{
		// DO Catmull-Rom spline interpolation !!!
		float k0xPrime, k0yPrime, k0zPrime,k1xPrime, k1yPrime, k1zPrime;
		float u, uSquared, uCubed;
		
		int numVertices = md2Model->MeshData[currentKeyframeIndex].vertex.size();

		memcpy(interpolatedVertices, &(md2Model->MeshData[currentKeyframeIndex].vertex[0]), sizeof(MODELVERTEX) * numVertices);

		for (int v=0; v < numVertices; v++)
		{
			// Get the Prime (slope) at each vertex
			k0xPrime = (md2Model->MeshData[nextKeyframeIndex].vertex[v].m_vecPos.x - md2Model->MeshData[previousKeyframeIndex].vertex[v].m_vecPos.x)/2.0f;
			k0yPrime = (md2Model->MeshData[nextKeyframeIndex].vertex[v].m_vecPos.y - md2Model->MeshData[previousKeyframeIndex].vertex[v].m_vecPos.y)/2.0f;
			k0zPrime = (md2Model->MeshData[nextKeyframeIndex].vertex[v].m_vecPos.z - md2Model->MeshData[previousKeyframeIndex].vertex[v].m_vecPos.z)/2.0f;
			k1xPrime = (md2Model->MeshData[nextNextKeyframeIndex].vertex[v].m_vecPos.x - md2Model->MeshData[currentKeyframeIndex].vertex[v].m_vecPos.x)/2.0f;
			k1yPrime = (md2Model->MeshData[nextNextKeyframeIndex].vertex[v].m_vecPos.y - md2Model->MeshData[currentKeyframeIndex].vertex[v].m_vecPos.y)/2.0f;
			k1zPrime = (md2Model->MeshData[nextNextKeyframeIndex].vertex[v].m_vecPos.z - md2Model->MeshData[currentKeyframeIndex].vertex[v].m_vecPos.z)/2.0f;

			// u is the percentage of the way to the next keyframe
			u = computedKeyframe - (int)computedKeyframe;
			uSquared = u * u;
			uCubed = uSquared * u;

			/*
			// Key
			k0x = md2Model->MeshData[int(md2Model->animationDataArray[l].cur)].vertex[v].m_vecPos.x;
			k1x = md2Model->MeshData[nextKeyframeIndex].vertex[v].m_vecPos.x;
			k0y = md2Model->MeshData[int(md2Model->animationDataArray[l].cur)].vertex[v].m_vecPos.y;
			k1y = md2Model->MeshData[nextKeyframeIndex].vertex[v].m_vecPos.y;
			k0z = md2Model->MeshData[int(md2Model->animationDataArray[l].cur)].vertex[v].m_vecPos.z;
			k1z = md2Model->MeshData[nextKeyframeIndex].vertex[v].m_vecPos.z;
			
			// Formula:
			k(u) =  k(0)×(2u3-3u2+1) +
			k(1)×(3u2-2u3) +
			k'(0)×(u3-2u2+u) +
			k'(1)×(u3-u2)
			*/

			interpolatedVertices[v].m_vecPos.x = md2Model->MeshData[currentKeyframeIndex].vertex[v].m_vecPos.x * (2*uCubed - 3*uSquared +1) + md2Model->MeshData[nextKeyframeIndex].vertex[v].m_vecPos.x * (3*uSquared - 2*uCubed) + k0xPrime * (uCubed - 2*uSquared+u) + k1xPrime * (uCubed - uSquared);
			interpolatedVertices[v].m_vecPos.y = md2Model->MeshData[currentKeyframeIndex].vertex[v].m_vecPos.y * (2*uCubed - 3*uSquared +1) + md2Model->MeshData[nextKeyframeIndex].vertex[v].m_vecPos.y * (3*uSquared - 2*uCubed) + k0yPrime * (uCubed - 2*uSquared+u) + k1yPrime * (uCubed - uSquared);
			interpolatedVertices[v].m_vecPos.z = md2Model->MeshData[currentKeyframeIndex].vertex[v].m_vecPos.z * (2*uCubed - 3*uSquared +1) + md2Model->MeshData[nextKeyframeIndex].vertex[v].m_vecPos.z * (3*uSquared - 2*uCubed) + k0zPrime * (uCubed - 2*uSquared+u) + k1zPrime * (uCubed - uSquared);
			
		}
		
		meshDataPointer=interpolatedVertices;
		/*
		return (SUCCEEDED(m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, //Typ	
												GetTriangleCount (),		  //Anzahl
												interpolatedVertices,		  //Pointer auf Daten
												sizeof(MODELVERTEX))));  //Größe Vertex
												*/
	}
	else
	{
		meshDataPointer= &md2Model->MeshData[currentKeyframeIndex].vertex[0];
		/*
		return (SUCCEEDED(m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, //Typ	
													GetTriangleCount (),		  //Anzahl
													(BYTE**)&md2Model->MeshData[currentKeyframeIndex].vertex[0],		  //Pointer auf Daten
													sizeof(MODELVERTEX))));  //Größe Vertex
													*/
	}
}

// Run the specified animation.  When done return to the current animation
bool AnimationGenerator::SetSingleAnimation(char *animation, bool PauseAtEnd)
{
	assert(loaded);

	int l = lastSetAnimation;
	if (SetAnimation(animation))
	{
		// New animation sequence.  When this new sequence is running it will change to the original
		autoCycleToNextAnimation = l;
		playingSingleCycleAnimation=true;
		pauseAtEnd=PauseAtEnd;
		return true;
	}

	assert(0);

	 // Bad animation name or already running this animation
	return false;
}

int AnimationGenerator::GetIndexOfAnimation(char *animation)
{
		//Loop through all animation names
	for (int l=0;l<(int)md2Model->animationDataArray.size();l++)
	{
		// Did we find what they specified?
		if (strcmp (md2Model->animationDataArray[l]->name, animation)==0)
			return l;
	}

	return -1;
}
// Sets nextNextKeyframeIndex to the first keyframe of the specified animation.  Set elapsedTime for that animation cycle to 0.
// Set nextAnimationIndex, and keyframesToNextAnimationIndex to 2
bool AnimationGenerator::SetAnimation(char *animation)
{
	assert(loaded);

	int l = GetIndexOfAnimation(animation);
	if (l==-1)
		return false; // Couldn't find it

	if (lastSetAnimation != l) // Don't do it if already at this animation
	{
		lastSetAnimation=l;
		autoCycleToNextAnimation=-1;
		if (playingSingleCycleAnimation==true)
			playingSingleCycleAnimation=false;
		
		if (currentAnimationIndex==l)
		{
			// Cancel what we were doing
			keyframesToNextAnimationIndex=0;
			
			int start = md2Model->animationDataArray[currentAnimationIndex]->start;
			int end = md2Model->animationDataArray[currentAnimationIndex]->end;

			nextKeyframeIndex=currentKeyframeIndex+1;
			if (nextKeyframeIndex==end)
				nextKeyframeIndex=start;

			nextNextKeyframeIndex=nextKeyframeIndex+1;
			if (nextNextKeyframeIndex==end)
				nextNextKeyframeIndex=start;
		}
		else
		{
			SetNewAnimation(l);
		}
	}
	
	return true;
}

void AnimationGenerator::SetNewAnimation(int l)
{
	assert(loaded);

	// Two keyframes from now we will use the first keyframe for the new animation
	nextNextKeyframeIndex=md2Model->animationDataArray[l]->start;
	nextAnimationIndex=l;
	keyframesToNextAnimationIndex=2;
	assert(nextAnimationIndex>=0);
}


// Restart the animation sequence
void AnimationGenerator::RestartAnimation(void)
{
	loaded=true;
	currentAnimationIndex=0;
	nextAnimationIndex=0;
	keyframesToNextAnimationIndex=-1;
	autoCycleToNextAnimation=-1;
	lastSetAnimation=0;
	playingSingleCycleAnimation=false;
	pauseAtEnd=false;

	if ((int)md2Model->animationDataArray.size()>currentAnimationIndex)
	{
		animationSequenceArray[currentAnimationIndex]->elapsedTime=0L;
		previousKeyframeIndex=md2Model->animationDataArray[currentAnimationIndex]->end-1;
		currentKeyframeIndex=md2Model->animationDataArray[currentAnimationIndex]->start;
		nextKeyframeIndex=md2Model->animationDataArray[currentAnimationIndex]->start+(1 % (md2Model->animationDataArray[currentAnimationIndex]->start-md2Model->animationDataArray[currentAnimationIndex]->end));
		nextNextKeyframeIndex=md2Model->animationDataArray[currentAnimationIndex]->start+(2 % (md2Model->animationDataArray[currentAnimationIndex]->start-md2Model->animationDataArray[currentAnimationIndex]->end));
	}

	// Not necessary
	/*
	for (int l=0;l<md2Model->animationDataArray.size();l++)
	{
		// Reset the elapsed time for all animations
		md2Model->animationDataArray[l]->elapsedTime=0L;
	}
	*/
}

// Restart the animation sequence
BOOL AnimationGenerator::Render(void)
{
	assert(loaded);
	assert(meshDataPointer);
	return (SUCCEEDED(m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, //Typ	
												md2Model->GetTriangleCount (),		  //Anzahl
												meshDataPointer,		  //Pointer auf Daten
												sizeof(MODELVERTEX))));  //Größe Vertex
}


AnimationGenerator& AnimationGenerator::operator = (AnimationGenerator& input)
{
	assert(input.loaded);
	if (&input == this)
		return *this;

	loaded=input.loaded;
	autoCycleToNextAnimation=input.autoCycleToNextAnimation;
	playingSingleCycleAnimation=input.playingSingleCycleAnimation;
	
	assert(input.md2Model);

	currentAnimationIndex=input.currentAnimationIndex;
	nextAnimationIndex=input.nextAnimationIndex;
	keyframesToNextAnimationIndex=input.keyframesToNextAnimationIndex;
	previousKeyframeIndex=input.previousKeyframeIndex;
	currentKeyframeIndex=input.currentKeyframeIndex;
	nextKeyframeIndex=input.nextKeyframeIndex;
	nextNextKeyframeIndex=input.nextNextKeyframeIndex;
	lastSetAnimation=input.lastSetAnimation;
	memcpy(interpolatedVertices, input.interpolatedVertices, MAX_TRIANGLES * 3 * sizeof(MODELVERTEX));
	md2Model=input.md2Model;
	pauseAtEnd=input.pauseAtEnd;

	// Copy the animation sequence list
	AnimationSequence *as;
	for (unsigned long i=0L; i < input.animationSequenceArray.size(); i++)
	{
		as = new AnimationSequence;
		as->elapsedTime = input.animationSequenceArray[i]->elapsedTime;
		as->totalTime = input.animationSequenceArray[i]->totalTime;
		animationSequenceArray.insert(as);
	}

	return *this;
}

void AnimationGenerator::SetMD2Model(MD2Model* Md2Model)
{
	AnimationSequence *as;

	md2Model=Md2Model;
	// The md2 model must have been loaded
	assert(md2Model->animationDataArray.size()>0);

	for (unsigned long i=0L; i < md2Model->animationDataArray.size(); i++)
	{
		as = new AnimationSequence;		
		animationSequenceArray.insert(as);
		SetAnimationSpeed(i, DEFAULT_TIME_BETWEEN_KEYFRAMES);
	}
}

void AnimationGenerator::SetAnimationSpeed(int keyframeIndex, int timeBetweenKeyframes)
{
	// Let the total time be a factor of the number of keyframes
	animationSequenceArray[keyframeIndex]->totalTime = (md2Model->animationDataArray[keyframeIndex]->end -  md2Model->animationDataArray[keyframeIndex]->start + 1) * timeBetweenKeyframes;
}

int AnimationGenerator::GetStartingKeyframeForAnimationIndex(int a) const
{
	assert(md2Model);
	if (a < 0)
		a = nextAnimationIndex;
	return md2Model->animationDataArray[a]->start;
}