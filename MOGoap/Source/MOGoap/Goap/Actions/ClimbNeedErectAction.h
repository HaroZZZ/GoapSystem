#pragma once
#include "../Core/MOGoapAction.h"

class ClimbNeedErectAction : public MOGoapAction
{
public:
	ClimbNeedErectAction();

	virtual std::vector<MOGoapState*> GetSettings(const MOGoapActionStackData& stackData) override;

	virtual void Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done = nullptr, ActionCallBack fail = nullptr) override;

	// MUST BE IMPLEMENTED AS THREAD SAFE
	virtual MOGoapState* GetPreconditions(MOGoapActionStackData& stackData) override;

	virtual MOGoapState* GetEffects(MOGoapActionStackData& stackData) override;

	virtual bool CheckProceduralCondition(MOGoapActionStackData& stackData) override;

	virtual void UpdateAction(float DeltaTime) override;

	virtual float GetCost(MOGoapActionStackData& stackData);
private:
	std::vector<MOGoapState*> SettingsList;
};