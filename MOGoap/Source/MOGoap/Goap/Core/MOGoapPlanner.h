#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <queue>
#include "MOGoapAction.h"
#include "MOGoapGoal.h"
#include "MOGoapPlannerSettings.h"
#include "MOGoapNode.h"
//#include "MOGoapPlanner.generated.h"

class MOGoapAgent;
class MOGoapGoal;
class MOGoapPlannerSettings;

class MOGoapPlanner
{
private:
	MOGoapAgent* GoapAgent;

	MOGoapGoal* CurrentGoal;

	std::vector<MOGoapAction*> Actions;

	MOGoapPlannerSettings* PlannerSettings;

	bool Calculated;

	int MaxDepth;

	std::list<MOGoapNode*> Frontier;
	std::unordered_map<MOGoapState*, MOGoapNode*> stateToNode;
	std::unordered_map<MOGoapState*, MOGoapNode*> explored;
	
	std::vector<MOGoapNode*> createdNodes;

public:
	MOGoapPlanner();

	MOGoapPlanner(MOGoapPlannerSettings* settings);

	MOGoapGoal* Plan(MOGoapAgent* agent, MOGoapGoal* blacklistGoal,std::list<MOGoapActionState*>* currentPlan, PlannerDoneFunc ondone = nullptr);

	MOGoapNode* AStar(MOGoapNode* start,MOGoapState* goal,int maxiterations = 100,bool earlyExit = true,bool clearNodes = true);

	MOGoapNode* LowestFinList(std::list<MOGoapNode*>& oplist);

	bool ContainsNode(MOGoapNode node, const std::list<MOGoapNode>& list);

	void ClearNodes();

	MOGoapGoal* GetCurrentGoal()
	{
		return CurrentGoal;
	}

	MOGoapAgent* GetCurrentAgent()
	{
		return GoapAgent;
	}

	bool IsPlanning()
	{
		return !Calculated;
	}

	MOGoapPlannerSettings* GetSettings()
	{
		return PlannerSettings;
	}

	inline int GetMaxDepth()
	{
		return MaxDepth;
	}
};