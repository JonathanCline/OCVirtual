#include "Component.h"

#include <cassert>
#include <random>




namespace PROJECT_NAMESPACE
{
	


	bool Component::good() const noexcept
	{
		return this->id() != nullcomp;
	};
	Component::operator bool() const noexcept
	{
		return this->good();
	};

	void Component::release() noexcept
	{
		this->id_ = nullcomp;
	};
	void Component::reset() noexcept
	{
		if (this->good())
		{
			delete_component_id(this->id_);
			this->release();
		};
		assert(!this->good());
	};
	ComponentID Component::extract() noexcept
	{
		const auto _out = this->id();
		this->release();
		return _out;
	};


	Component::Component(ComponentType& _type) :
		Component{ nullcomp, _type }
	{};
	Component::Component(ComponentID _id, ComponentType& _type) :
		id_{ _id }, type_{ &_type }
	{};

	Component::Component(Component&& _other) noexcept :
		id_{ _other.extract() }, type_{ _other.type() }
	{};
	Component& Component::operator=(Component&& _other) noexcept
	{
		this->reset();
		this->id_ = _other.extract();
		this->type_ = _other.type();
		return *this;
	};

	Component::~Component()
	{
		this->reset();
	};
};
