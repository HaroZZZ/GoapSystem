#pragma once
#include <map>
#include <string>
#include <typeinfo>
#include "CoreMinimal.h"
#include <type_traits>
#include "any.hpp"
#include <functional>
#include <stack>

using namespace::MoGoapCore;

class MOGoapState {
public:
	static std::stack<MOGoapState*> cachedStates;

	MOGoapState() = default;

	MOGoapState(const MOGoapState& rhs)
	{
		values = rhs.values;
	}

	MOGoapState(MOGoapState&& rhs)
	{
		values = rhs.values;
	}

	static MOGoapState* Instantiate(MOGoapState* old = nullptr);

	MOGoapState* Clone();

	void Recycle();

	void Init(MOGoapState* old);

	MOGoapState& operator=(const MOGoapState& rhs)
	{
		values = rhs.values;
		return *this;
	}

	~MOGoapState()
	{
		values.clear();
	}

	std::map<std::string, MoGoapCore::any> values;

	void Init(MOGoapState old)
	{
		
	}

	inline void Set(std::string key, MoGoapCore::any value)
	{
		values[key] = value;
	}

	MoGoapCore::any Get(std::string key)
	{
		std::map<std::string, MoGoapCore::any>::const_iterator iter = values.find(key);
		if (iter != values.cend())
		{
			return iter->second;
		}
		else
			return MoGoapCore::any();
	}

	template<typename ValueType>
	ValueType GetValue(std::string key)
	{
		MoGoapCore::any a = Get(key);
		return static_cast<any::holder<ValueType>*>(a.content)->held;
	}

	inline void Remove(std::string key)
	{
		values.erase(key);
	}

	inline bool HasKey(std::string key)
	{
		std::map<std::string, MoGoapCore::any>::const_iterator iter = values.find(key);
		return (iter != values.cend());
	}
	void AddFromState(MOGoapState* old);

	inline int Count() const
	{
		return values.size();
	}

	void Clear()
	{
		values.clear();
	}

	bool HasAny(MOGoapState* other);

	bool HasAnyConflict(MOGoapState* other);

	bool HasAnyConflict(MOGoapState* effects, MOGoapState* precond);

	int MissingDifference(MOGoapState* other, MOGoapState* difference = nullptr);

	int ReplaceWithMissingDifference(MOGoapState* other);

	//override operation ==
	bool operator==(const MOGoapState& rhs) const
	{


		return false;
	}

	bool operator<(const MOGoapState& rhs) const
	{
		return this->Count() < rhs.Count();
	}

	MOGoapState& operator+(const MOGoapState& rhs) 
	{
		for (auto pair : rhs.values)
		{
			values[pair.first] = pair.second;
		}
		return *this;
	}
};

using EqualPtr = std::function<bool(const any&, const any&)>;

class TypeFactory
{
public:
	EqualPtr GetFuncByName(std::string typeName);

	void RegistNewType(std::string typeName, EqualPtr method);

	static TypeFactory* getInstance()
	{
		if (instance != nullptr)
			return instance;
		else {
			instance = new TypeFactory();
			return instance;
		}
	}

	std::map <std::string, EqualPtr> EqualMaps;

	TypeFactory() {	}

	static TypeFactory* instance;
};

class TypeEqual
{
public:
	TypeEqual(std::string typeName, EqualPtr method)
	{
		TypeFactory::getInstance()->RegistNewType(typeName, method);
	}
};

#define ADDEQUALMETHOD(type)																\
	bool equal##_##type(const any& lhs,const any& rhs){										\
	return (static_cast<any::holder<type>*>(lhs.content)->held) ==							\
	(static_cast<any::holder<type>*>(rhs.content)->held);}									\
	TypeEqual g_TypeEqualFuncRegister##type(typeid(type).name(),(EqualPtr)equal##_##type)	\

#define ADDPOINTEREQUALMETHOD(type)																\
	bool equal##_##type(const any& lhs,const any& rhs){										\
	return (static_cast<any::holder<type*>*>(lhs.content)->held) ==							\
	(static_cast<any::holder<type*>*>(rhs.content)->held);}									\
	TypeEqual g_TypeEqualFuncRegister##type(typeid(type*).name(),(EqualPtr)equal##_##type)	\

