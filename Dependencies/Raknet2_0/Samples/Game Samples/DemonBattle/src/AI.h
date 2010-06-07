#ifndef __AI_H
#define __AI_H

enum AIDecision {DECISION_NO_ACTION, DECISION_RECHARGE_MANA_FOR_SHOOT, DECISION_RECHARGE_MANA_FOR_SUMMON, DECISION_WAIT_TO_SHOOT, DECISION_WAIT_TO_SUMMON, DECISION_NO_TARGETS_TO_SHOOT, DECISION_SHOOT, DECISION_SUMMON};

struct D3DXVECTOR3;

class AI
{
public:
	~AI() {}
	static AI* Instance(void) {return &I;}
	void Update(long time);
	AIDecision GetDecision(void);
	void Init(void) {currentState=AI_IDLE;}
	// How much power to use if firing up at a 45 degree angle at a moving target
	// Also returns time to target in seconds
	float CalculateFiringPower(float x0target, float velocityTarget, float heightTarget, float x0Fireball, float heightCastle, float *timeToTarget);
	void AbortShot(D3DXVECTOR3 *rayOrg);
private:
	void HandleGainLevel(void);
	AI() {}
	static AI I;
	enum {AI_SHOOTING, AI_SUMMONING, AI_IDLE} currentState;
};

#endif