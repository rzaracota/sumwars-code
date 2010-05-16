#ifndef __ANIMATION_GENERATOR_H
#define __ANIMATION_GENERATOR_H

#pragma once

#include "string.h"
#include "Base3D.h"
#include "MD2Constants.h"

#include "md2model.h"

struct AnimationSequence
{
	// Time elapsed into the animation
	unsigned long elapsedTime;

	// How much time to spend on this animation
	unsigned long totalTime;

	AnimationSequence() {elapsedTime=0L;}
};

class AnimationGenerator
{
public:
	AnimationGenerator();
	~AnimationGenerator();

	// Assign an md2 model for this animation generator class to generate keyframes for.  You MUST do this before calling anything else
	void SetMD2Model(MD2Model* Md2Model);

	// If the elapsed time for the current animation is such that the current keyframe is no longer the current keyframe, then call UpdateKeyframe
	// Then render based on keyframe values
	BOOL Render(void);

	// DO Catmull-Rom spline interpolation if necessary.  Internally sets meshDataPointer to point to the computed mesh
	void ComputeMeshData(unsigned long elapsedTime);

	// Change to this animation.  Interpolates between current animation and this animation for the remainder of the current keyframe and subsequent one after that
	// Returns true on success, false on animation not found.
	bool SetAnimation(char *animation);

	// Run the specified animation.  When done return to the current animation or pause according to the bool
	bool SetSingleAnimation(char *animation, bool PauseAtEnd);

	// Returns the internal index of the specified animation
	int GetIndexOfAnimation(char *animation);

	// Are we going to or in the process of running a single animation?
	// This function is useful for timing events to happen at the end of a single animation since with it you can tell when a single animation ends
	bool IsSingleAnimationRunning(void) {return playingSingleCycleAnimation;}

	// Restart the animation sequence.  Call this immediately after calling SetMD2Model and also anytime you want to reset the animation
	void RestartAnimation(void);

	// Get a pointer to the mesh data generated
	MODELVERTEX*const* GetMeshDataPointer(void) const {return &meshDataPointer;}

	// Assignment operator
	AnimationGenerator& operator = (AnimationGenerator& input);

	int GetCurrentKeyframe(void) const {return currentKeyframeIndex;}
	// Defaults to the current animation.  You can pass the result of GetIndexOfAnimation for some other named animation
	int GetStartingKeyframeForAnimationIndex(int a=-1) const;
	void SetAnimationSpeed(int keyframeIndex, int timeBetweenKeyframes);

private:
	// ***** Catmull-Rom spline interpolation ***
	int currentAnimationIndex, nextAnimationIndex, keyframesToNextAnimationIndex;
	int previousKeyframeIndex, currentKeyframeIndex, nextKeyframeIndex, nextNextKeyframeIndex;
	// Set previousKeyframeIndex to currentKeyframeIndex, set currentKeyframeIndex to nextKeyframeIndex, set nextKeyframeIndex to nextNextKeyframeIndex, if (keyframesToNextAnimationIndex--==0) then set currentAnimationIndex to nextAnimationIndex.  Calculate nextNextKeyframeIndex based on currentAnimationIndex, nextKeyframeIndex
	void IncrementKeyframeIndices(void);
	MODELVERTEX* meshDataPointer;
	
	int lastSetAnimation;
	MODELVERTEX interpolatedVertices[MAX_TRIANGLES * 3];
	MD2Model* md2Model;

    void SetNewAnimation(int l);

	bool loaded, pauseAtEnd;
	// Used to run single animation cycles
	int autoCycleToNextAnimation;
	bool playingSingleCycleAnimation;
	
	BasicDataStructures::List<AnimationSequence*> animationSequenceArray;
};


#endif