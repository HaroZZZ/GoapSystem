#include "MOGoapPlanner.h"
#include "MOGoapAgent.h"
#include "MOGoapAction.h"
#include <set>

MOGoapPlanner::MOGoapPlanner()
{
	Calculated = true;
	GoapAgent = nullptr;

	PlannerSettings = new MOGoapPlannerSettings();
	CurrentGoal = nullptr;

}

MOGoapPlanner::MOGoapPlanner(MOGoapPlannerSettings* settings)
{
	Calculated = true;
	GoapAgent = nullptr;
	CurrentGoal = nullptr;

	if (settings != nullptr)
		PlannerSettings = settings;
	else
		PlannerSettings = new MOGoapPlannerSettings();
}


MOGoapGoal* MOGoapPlanner::Plan(MOGoapAgent* agent, MOGoapGoal* blacklistGoal, std::list<MOGoapActionState*>* currentPlan, PlannerDoneFunc ondone /*= nullptr*/)
{
	GoapAgent = agent;
	Calculated = false;

	std::vector<MOGoapAction*> sol;

	std::vector<MOGoapGoal*> possibleGoals;
	for (auto& goal : GoapAgent->GetGoalsSet())
	{
		if(blacklistGoal != nullptr && *goal == *blacklistGoal)
			continue;

		goal->Precalculations(this);

		if (goal->IsGoalPossible())
			possibleGoals.push_back(goal);
	}

	//根据优先级排序目标，让后面A*优先执行高优先级的
	std::sort(possibleGoals.begin(), possibleGoals.end(), [](const MOGoapGoal* x, const MOGoapGoal* y)
		{
			if (x->GetPriority() > y->GetPriority())
				return false;
			else
				return true;
		});
	

	MOGoapState *currentState = agent->GetMemory()->GetWorldState();
	MOGoapNode *start = nullptr;

	for (int i = possibleGoals.size() - 1; i >= 0; --i)
	{
		CurrentGoal = possibleGoals[i];
		possibleGoals.pop_back();

		auto goalState = CurrentGoal->GetGoalState();

		// can't work with dynamic actions, of course
		if (!PlannerSettings->UsingDynamicActions)
		{
			auto wantedGoalCheck = CurrentGoal->GetGoalState();

			MOGoapActionStackData stackData;
			stackData.agent = GoapAgent;
			stackData.currentState = currentState->Clone();
			stackData.goalState = goalState->Clone();
			stackData.next = nullptr;
			stackData.settings = MOGoapState::Instantiate();
			// we check if the goal can be archived through actions first, so we don't brute force it with A* if we can't
			for (auto& action : GoapAgent->GetActionsSet())
			{
				action->Precalculations(stackData);
				if (!action->CheckProceduralCondition(stackData))
				{
					continue;
				}

				auto previous = wantedGoalCheck;
				wantedGoalCheck = MOGoapState::Instantiate();
				previous->MissingDifference(action->GetEffects(stackData), wantedGoalCheck);
			}
			// finally push the current world state
			auto current = wantedGoalCheck;
			wantedGoalCheck = MOGoapState::Instantiate();
			current->MissingDifference(GetCurrentAgent()->GetMemory()->GetWorldState(), wantedGoalCheck);
			// can't validate goal 
			if (wantedGoalCheck->Count() > 0)
			{
				CurrentGoal = nullptr;
				continue;
			}
		}

		//A*
		//if (start != nullptr)
		//{
		//	start->Recycle();
		//	start = nullptr;
		//}
			
		start = MOGoapNode::Instantiate(this, goalState->Clone(), nullptr, nullptr, nullptr);

		auto leaf = AStar(start,goalState, PlannerSettings->MaxIterations,PlannerSettings->PlanningEarlyExit);

		if (leaf == nullptr)
		{
			CurrentGoal = nullptr;
			continue;
		}

		auto result = leaf->CalculatePath();
		if (result.size() == 0)
		{
			CurrentGoal = nullptr;
			continue;
		}

		CurrentGoal->SetPlan(std::move(result));
		break;
	}

	Calculated = true;
	//onDonePlan call
	if (ondone != nullptr && CurrentGoal != nullptr)
	{
		ondone(CurrentGoal);
	}

	//print plan list

	return CurrentGoal;
}

MOGoapNode* MOGoapPlanner::AStar(MOGoapNode* start, MOGoapState* goal, int maxiterations /*= 100*/, bool earlyExit /*= true*/, bool clearNodes /*= true*/)
{
	Frontier.clear();
	stateToNode.clear();
	explored.clear();

	if (clearNodes)
	{
		ClearNodes();
		createdNodes.push_back(start);
	}

	bool goalReached = false;

	Frontier.push_back(start);

	int iterations = 0;

	while ((Frontier.size() > 0) && (iterations < MaxDepth) && (Frontier.size() + 1 < MaxDepth))
	{
		auto node = LowestFinList(Frontier);
		//将要探索的节点
		Frontier.remove(node);

		if (node->IsGoal(goal))
		{
			goalReached = true;
			return node;
		}

		//已经探索过
		explored[node->GetWorldState()] = node;

		auto adjacents = node->Expand();

		for (auto child : adjacents)
		{
			iterations++;
			if (clearNodes)
			{
				createdNodes.push_back(child);
			}

			if (earlyExit && child->IsGoal(goal))
			{
				return child;
			}

			auto childCost = child->GetCost();

			auto state = child->GetWorldState();
			
			if (explored.find(state) != explored.end())
			{
				continue;
			}

			auto similiarNode = stateToNode.find(state);
			if (similiarNode != stateToNode.end())
			{
				if (similiarNode->second->GetF() > childCost)
					Frontier.remove(similiarNode->second);
				else
					break;
			}

			Frontier.push_back(child);
			stateToNode[state] = child;
		}
	}
	return nullptr;
}

MOGoapNode* MOGoapPlanner::LowestFinList(std::list<MOGoapNode*>& oplist)
{
	MOGoapNode* node = nullptr;

	float minF = FLT_MAX;
	for (auto& n : oplist)
	{
		if ((n->GetF()) < minF)
		{
			node = n;
			minF = n->GetF();
		}
	}
	return node;
}

bool MOGoapPlanner::ContainsNode(MOGoapNode node, const std::list<MOGoapNode>& list)
{
	auto res = std::find(list.begin(), list.end(), node);

	if (res != list.end())
	{
		return true;
	}
		
	return false;
}

void MOGoapPlanner::ClearNodes()
{
	for (auto node : createdNodes)
	{
		node->Recycle();
	}
	createdNodes.clear();
}

