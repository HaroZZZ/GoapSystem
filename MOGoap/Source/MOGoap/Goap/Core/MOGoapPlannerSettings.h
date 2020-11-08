#pragma once

class MOGoapPlannerSettings
{
public:
	bool PlanningEarlyExit = false;
	// increase both if your agent has a lot of actions
	int MaxIterations = 1000;
	int MaxNodesToExpand = 10000;
	// set this to true if using dynamic actions, such as GenericGoTo or GatherResourceAction
	// a dynamic action is an action that has dynamic preconditions or effects (changed in runtime/precalcultions)
	bool UsingDynamicActions = true;

	bool DebugPlan = false;
};