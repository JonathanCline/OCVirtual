#pragma once

#include <UUID.h>

namespace PROJECT_NAMESPACE
{
	using ComponentID = UUID;
	constexpr inline ComponentID nullcomp = ComponentID{};

	ComponentID new_component_id();
	void delete_component_id(ComponentID& _id);
};
