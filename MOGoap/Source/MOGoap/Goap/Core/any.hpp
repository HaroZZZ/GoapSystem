#pragma once
#include <map>
#include <string>
#include <typeinfo>
#include "CoreMinimal.h"


namespace MoGoapCore
{

	class any
	{
	public:
		any() :content(0)
		{

		}

		template<typename ValueType>
		any(const ValueType& value)
			: content(new holder<ValueType>(value))
		{

		}

		any(const any& other) : content(other.content ? other.content->clone() : nullptr) { }

		~any() noexcept
		{
			delete content;
		}

		any& swap(any& rhs) noexcept
		{
			placeholder* temp = content;
			content = rhs.content;
			rhs.content = temp;
			return *this;
		}

		any& operator=(const any& rhs)
		{
			any(rhs).swap(*this);
			return *this;
		}

		any& operator=(any&& rhs)
		{
			rhs.swap(*this);
			any().swap(rhs);
			return *this;
		}

		template<typename ValueType>
		any& operator=(ValueType&& rhs)
		{
			any(std::forward<ValueType>(rhs)).swap(*this);
			return *this;
		}

		bool empty() const
		{
			return !content;
		}

		void clear()
		{
			any().swap(*this);
		}

		const std::type_info& type() const
		{
			return content ? content->type() : typeid(void);
		}
	public:
		class placeholder
		{
		public:
			virtual ~placeholder()
			{

			}

			virtual const std::type_info& type() const = 0;

			virtual placeholder* clone() const = 0;
		};

		template<typename ValueType>
		class holder final : public placeholder
		{
		public:
			holder(const ValueType& value) :held(value) {	}

			holder(ValueType&& value)
				:held(std::forward(value))
			{

			}

			virtual const std::type_info& type() const override
			{
				return typeid(ValueType);
			}

			virtual placeholder* clone() const override
			{
				return new holder(held);
			}

			holder& operator=(const holder&) = delete;

			ValueType held;
		};

	public:
		placeholder* content;
	};

	template<typename ValueType>
	ValueType* any_cast(const any* operand)
	{
		if (operand->content == nullptr)
			return nullptr;
		else
			static_cast<holder<std::remove_cv_t<ValueType>>*>(operand->content)->held;
	}

	bool equal(const any& lhs, const any& rhs);
}