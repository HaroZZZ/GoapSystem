#pragma once
#include "MOGoapState.h"

class MOGoapAction;
class MOGoapPlanner;
class MOGoapActionState;

class MOGoapNode
{
public:
	MOGoapNode();

	MOGoapNode(MOGoapAction* a);
private:
	MOGoapState* NodeState;

	float h;

	float g;

	float cost;

	float heuristicMultiplier = 1;

	MOGoapNode* Parent;

	MOGoapAction* NodeAction;

	MOGoapState* NodeGoalState;
	MOGoapState* ActionSettings;

	static std::stack<MOGoapNode*> cachedNodes;

	std::vector<MOGoapNode*> ExpandList;

	MOGoapPlanner* Planner;

	MOGoapState* Preconditions;
	MOGoapState* Effects;

	MOGoapState* GoalMergeWithWorld;

public:
	void Init(MOGoapPlanner* planner,MOGoapState* newgoal,MOGoapNode* parent,MOGoapAction* action,MOGoapState* settings);

	static MOGoapNode* Instantiate(MOGoapPlanner* planner, MOGoapState* newgoal, MOGoapNode* parent, MOGoapAction* action, MOGoapState* actionSettings);

	void Recycle();

	bool operator==(const MOGoapNode& n) const;

	bool operator!=(const MOGoapNode& n) const;

	bool operator<(const MOGoapNode& n) const;

	bool IsGoal(MOGoapState* goal);

	std::vector<MOGoapNode*> Expand();

	std::list<MOGoapActionState*> CalculatePath();

	inline float GetHeuristicCost() const
	{
		return h;
	}

	inline float GetPathCost() const
	{
		return g;
	}

	inline float GetF() const
	{
		return h + g;
	}

	inline float GetCost() const
	{
		return cost;
	}

	inline MOGoapNode* GetParent() const
	{
		return Parent;
	}

	inline MOGoapState* GetWorldState() const
	{
		return NodeState;
	}

	inline MOGoapAction* GetAction() const
	{
		return NodeAction;
	}

	inline MOGoapState* GetGoal()
	{
		return NodeGoalState;
	}

	inline MOGoapState* GetActionSettings()
	{
		return ActionSettings;
	}

	inline void SetH(float value)
	{
		h += value;
	}

	inline void SetG(MOGoapNode p)
	{

	}

	inline void SetH(MOGoapState w)
	{

	}

	inline void SetParent(MOGoapNode* p)
	{
		Parent = p;
	}

	inline void SetWorldState(MOGoapState* w)
	{
		NodeState = w;
	}

	inline void SetAction(MOGoapAction* a)
	{
		NodeAction = a;
	}

	inline void SetSettings(MOGoapState* s)
	{
		ActionSettings = s;
	}

	inline void SetGoal(MOGoapState* goal)
	{
		NodeGoalState = goal;
	}
};