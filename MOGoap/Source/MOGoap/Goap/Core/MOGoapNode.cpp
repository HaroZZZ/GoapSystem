#include "MOGoapNode.h"
#include "MOGoapPlanner.h"
#include "MOGoapAgent.h"
#include "MOGoapState.h"
#include "MOGoapAction.h"

std::stack<MOGoapNode*> MOGoapNode::cachedNodes;

MOGoapNode::MOGoapNode()
{
	g = 0;
	h = 0;

}

MOGoapNode::MOGoapNode(MOGoapAction* a)
{

}

void MOGoapNode::Init(MOGoapPlanner* planner, MOGoapState* newgoal, MOGoapNode* parent, MOGoapAction* action, MOGoapState* settings)
{
	ExpandList.clear();

	this->Planner = planner;
	this->Parent = parent;
	this->NodeAction = action;
	this->ActionSettings = nullptr;

	if (settings != nullptr)
		this->ActionSettings = settings->Clone();

	g = 0;
	h = 0;

	if (parent != nullptr)
	{
		NodeState = parent->GetWorldState()->Clone();

		g = parent->GetPathCost();
	}
	else
	{
		NodeState = planner->GetCurrentAgent()->GetMemory()->GetWorldState()->Clone();
	}

	//在当前状态中保存设置
	NodeState->AddFromState(settings);

	if (action != nullptr)
	{
		NodeGoalState = MOGoapState::Instantiate(newgoal);

		MOGoapActionStackData stackData;
		stackData.currentState = NodeState;
		stackData.goalState = NodeGoalState;
		stackData.next = NodeAction;
		stackData.agent = planner->GetCurrentAgent();
		stackData.settings = ActionSettings;

		Preconditions = NodeAction->GetPreconditions(stackData);
		Effects = NodeAction->GetEffects(stackData);

		// addding the action's cost to the node's total cost
		g += NodeAction->GetCost(stackData);

		// adding the action's effects to the current node's state
		NodeState->AddFromState(Effects);
		// removes from goal all the conditions that are now fullfiled in the action's effects
		NodeGoalState->ReplaceWithMissingDifference(Effects);
		// add all preconditions of the current action to the goal
		NodeGoalState->AddFromState(Preconditions);
	}
	else
	{
		NodeGoalState = newgoal;
	}
	h = NodeGoalState->Count();

	cost = g + h * heuristicMultiplier;

	MOGoapState* diff = MOGoapState::Instantiate();
	MOGoapState* memory = planner->GetCurrentAgent()->GetMemory()->GetWorldState();
	NodeGoalState->MissingDifference(memory,diff);
	GoalMergeWithWorld = diff;
}

MOGoapNode* MOGoapNode::Instantiate(MOGoapPlanner* planner, MOGoapState* newgoal, MOGoapNode* parent, MOGoapAction* action, MOGoapState* actionSettings)
{
	MOGoapNode* node = nullptr;
	if (cachedNodes.empty())
	{
		node = new MOGoapNode();
	}
	else
	{
		node = cachedNodes.top();
		cachedNodes.pop();
	}

	node->Init(planner, newgoal, parent, action, actionSettings);
	return node;
}

void MOGoapNode::Recycle()
{
	NodeState->Recycle();
	NodeState = nullptr;
	NodeGoalState->Recycle();
	NodeGoalState = nullptr;

	cachedNodes.push(this);
}

bool MOGoapNode::operator<(const MOGoapNode& n) const
{
	return GetF() < n.GetF();
}

bool MOGoapNode::IsGoal(MOGoapState* goal)
{
	return GoalMergeWithWorld->Count() <= 0;
}

std::vector<MOGoapNode*> MOGoapNode::Expand()
{
	ExpandList.clear();

	auto agent = Planner->GetCurrentAgent();
	auto actions = agent->GetActionsSet();

	MOGoapActionStackData stackData;
	stackData.currentState = NodeState;
	stackData.goalState = NodeGoalState;
	stackData.next = NodeAction;
	stackData.agent = agent;
	stackData.settings = nullptr;

	for (int index = actions.size() - 1; index >= 0; --index)
	{
		auto possibleAction = actions[index];

		possibleAction->Precalculations(stackData);
		auto settingsList = possibleAction->GetSettings(stackData);
		if (settingsList.size() == 0)
		{
			auto precond = possibleAction->GetPreconditions(stackData);
			auto effects = possibleAction->GetEffects(stackData);

			if (effects->HasAny(NodeGoalState) &&
				!NodeGoalState->HasAnyConflict(effects, precond) &&
				!NodeGoalState->HasAnyConflict(effects) &&
				possibleAction->CheckProceduralCondition(stackData))
			{
				auto newGoal = NodeGoalState;
				ExpandList.push_back(Instantiate(Planner, newGoal, this, possibleAction, nullptr));
			}
		}
		else {
			for (auto settings : settingsList)
			{
				stackData.settings = settings;
				auto precond = possibleAction->GetPreconditions(stackData);
				auto effects = possibleAction->GetEffects(stackData);

				if (effects->HasAny(NodeGoalState) &&
					!NodeGoalState->HasAnyConflict(effects, precond) &&
					!NodeGoalState->HasAnyConflict(effects) &&
					possibleAction->CheckProceduralCondition(stackData))
				{
					auto newGoal = NodeGoalState;
					ExpandList.push_back(Instantiate(Planner, newGoal, this, possibleAction, settings));
				}
			}
		}

	}

	return ExpandList;
}

std::list<MOGoapActionState*> MOGoapNode::CalculatePath()
{
	std::list<MOGoapActionState*> result;
	auto node = this;
	while (node->GetParent() != nullptr)
	{
		result.push_back(new MOGoapActionState(node->NodeAction, node->ActionSettings));
		node = node->GetParent();
	}

	return result;
}

bool MOGoapNode::operator!=(const MOGoapNode& n) const
{
	return !(*this == n);
}

bool MOGoapNode::operator==(const MOGoapNode& n) const
{
	return NodeAction == n.GetAction();
}
