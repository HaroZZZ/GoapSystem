#include "MOGoapState.h"
#include "GameFramework/Actor.h"

TypeFactory* TypeFactory::instance;

using namespace std;
ADDEQUALMETHOD(int);
ADDEQUALMETHOD(bool);
ADDEQUALMETHOD(string);
ADDEQUALMETHOD(FVector);
ADDPOINTEREQUALMETHOD(AActor);

std::stack<MOGoapState*> MOGoapState::cachedStates;

bool MoGoapCore::equal(const any& lhs, const any& rhs)
{
	if (lhs.type() == rhs.type()) {
		//auto& tmp = TypeFactory::getInstance()->EqualMaps;
		//auto tname = lhs.type().name();
		//auto sname = typeid(lhs).name();
		auto func = TypeFactory::getInstance()->GetFuncByName(lhs.type().name());
		if (func != nullptr) {
			return func(lhs, rhs);
		}
	}

	return false;
}


MOGoapState* MOGoapState::Instantiate(MOGoapState* old /*= nullptr*/)
{
	MOGoapState* state = nullptr;
	if (cachedStates.empty())
	{
		state = new MOGoapState();
	}
	else
	{
		state = cachedStates.top();
		cachedStates.pop();
	}

	state->Init(old);
	return state;
}

MOGoapState* MOGoapState::Clone()
{
	return Instantiate(this);
}

void MOGoapState::Recycle()
{
	cachedStates.push(this);
}

void MOGoapState::Init(MOGoapState* old)
{
	values.clear();
	if (old != nullptr)
	{
		for (auto& pair : old->values)
		{
			values[pair.first] = pair.second;
		}
	}
}

void MOGoapState::AddFromState(MOGoapState* old)
{
	if (old == nullptr) return;
	for (auto& pair : old->values)
	{
		this->values[pair.first] = pair.second;
	}
}

bool MOGoapState::HasAny(MOGoapState* other)
{
	if (other == nullptr) return false;
	for (auto& pair : other->values)
	{
		auto thisValue = values.find(pair.first);
		if (thisValue != values.end())
		{

			if (equal(thisValue->second, pair.second))
			{
				return true;
			}
		}
	}
	return false;
}

bool MOGoapState::HasAnyConflict(MOGoapState* effects, MOGoapState* precond)
{
	for (auto& pair : precond->values)
	{
		auto otherValue = pair.second;

		std::map<std::string, any>::const_iterator  thisValue = values.find(pair.first);
		if (thisValue != values.cend())
		{
			std::map<std::string, any>::const_iterator effectValue = effects->values.find(pair.first);
			if (!equal(thisValue->second, otherValue) && (effectValue == effects->values.cend() || !equal(effectValue->second, otherValue)))
			{
				return true;
			}
		}
		else continue;
	}
	return false;
}

int MOGoapState::MissingDifference(MOGoapState* other, MOGoapState* difference)
{
	int count = 0;
	for (auto& pair : values)
	{
		std::map<std::string, any>::const_iterator otherValue = other->values.find(pair.first);
		if ((otherValue != other->values.cend() && !equal(pair.second, otherValue->second)) || (otherValue == other->values.cend()))
		{
			count++;
			if (difference != nullptr)
			{
				difference->values[pair.first] = pair.second;
			}
		}
	}
	return count;
}

int MOGoapState::ReplaceWithMissingDifference(MOGoapState* other)
{
	int count = 0;
	auto buffer = std::move(values);
	values.clear();

	for (auto& pair : buffer)
	{
		std::map<std::string, any>::const_iterator otherValue = other->values.find(pair.first);
		if (otherValue == other->values.cend() || (otherValue != other->values.cend() && !equal(pair.second, otherValue->second)))
		{
			count++;
			values[pair.first] = pair.second;
		}
	}
	return count;
}

bool MOGoapState::HasAnyConflict(MOGoapState* other)
{
	for (auto& pair : other->values)
	{
		std::map<std::string, any>::const_iterator thisValue = values.find(pair.first);
		if (thisValue != values.cend())
		{
			if (!equal(thisValue->second, pair.second))
			{
				return true;
			}
		}
		else continue;
	}
	return false;
}

EqualPtr TypeFactory::GetFuncByName(std::string type)
{
	if (EqualMaps.find(type) != EqualMaps.end())
	{
		return EqualMaps[type];
	}
	return nullptr;
}

void TypeFactory::RegistNewType(std::string type, EqualPtr method)
{
	EqualMaps[type] = method;
}
