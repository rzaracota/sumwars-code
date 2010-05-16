#ifndef __FILE_LOADED_VISIBLE_ENTITY_H
#define __FILE_LOADED_VISIBLE_ENTITY_H

#include "Entity.h"
#include "Base3D.h"
#include "XFile.h"
#include "md2model.h"
#include "VisibleEntity.h"

class AnimationGenerator;

enum DamageType {TEAM1_FIREBALL_DAMAGE, TEAM2_FIREBALL_DAMAGE, TEAM1_DEMON_DAMAGE, TEAM2_DEMON_DAMAGE, UNKNOWN_DAMAGE};

class FileLoadedVisibleEntity : public VisibleEntity
{
	protected:
	AnimationGenerator *remoteAnimation;
	LPDIRECT3DTEXTURE9 remoteTexture;
	MD2Model* remoteModel;
	XFile* remoteXFile;

public:
	FileLoadedVisibleEntity();
	virtual ~FileLoadedVisibleEntity() {}
	virtual void Render(long time);
	virtual void Update(long time);

	virtual void AdjustHeightToGround(void);
	// Note that you can use a remote mesh/texture or an xFile, not both.
	virtual void SetRemoteAnimation(AnimationGenerator* RemoteAnimation, MD2Model* model);
	virtual void SetRemoteTexture(LPDIRECT3DTEXTURE9 RemoteTexture);
	virtual void Damage(float damage, DamageType damageType) {}
	 // transmitPacket means was this caused by your own actions?  True then yes, false then another player is the cause.  This only has meaning in multiplayer
	virtual bool SwitchTeams(bool transmitPacket) {return true;}

	// Note that you can use a remote mesh/texture or an xFile, not both.
	virtual void SetXFile(XFile* xFile);

	// The left, right, top, bottom, front, back boundaries of the model's local space
	// These only work for loaded md2 models.
	// Set absolute to true to get the greatest value among all animations
	// Set to false to get the greatest value for the current animation
	virtual float GetXMax(int animationIndex=-1) const;
	virtual float GetXMin(int animationIndex=-1) const;
	virtual float GetYMax(int animationIndex=-1) const;
	virtual float GetYMin(int animationIndex=-1) const;
	virtual float GetZMax(int animationIndex=-1) const;
	virtual float GetZMin(int animationIndex=-1) const;
	virtual void SetScale(float Scale) {scale=Scale;}

};

#endif