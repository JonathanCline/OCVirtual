#pragma once

#include "ComponentID.h"

namespace PROJECT_NAMESPACE
{
	struct ComponentType;

	struct Component
	{
	public:

		bool good() const noexcept;
		explicit operator bool() const noexcept;

		void release() noexcept;
		void reset() noexcept;
		[[nodiscard("owning-ish")]] ComponentID extract() noexcept;

		const auto& id() const noexcept { return this->id_; };
		const auto& type() const noexcept { return this->type_; };

		friend inline bool operator==(const Component& _lhs, const ComponentID& _rhs) noexcept
		{
			return _lhs.id() == _rhs;
		};
		friend inline bool operator==(const ComponentID& _lhs, const Component& _rhs) noexcept
		{
			return _rhs == _lhs;
		};

		friend inline bool operator!=(const Component& _lhs, const ComponentID& _rhs) noexcept
		{
			return !(_lhs == _rhs);
		};
		friend inline bool operator!=(const ComponentID& _lhs, const Component& _rhs) noexcept
		{
			return !(_lhs == _rhs);
		};

		friend inline bool operator==(const Component& _lhs, const Component& _rhs) noexcept
		{
			return _lhs.id() == _rhs;
		};
		friend inline bool operator!=(const Component& _lhs, const Component& _rhs) noexcept
		{
			return !(_lhs == _rhs);
		};


		explicit Component(ComponentType& type);
		Component(ComponentID _id, ComponentType& type);

		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component(Component&&) noexcept;
		Component& operator=(Component&&) noexcept;

		~Component();
	
	private:
		ComponentID id_;
		ComponentType* type_;
	};

};
