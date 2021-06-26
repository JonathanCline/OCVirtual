#pragma once

#include "Ledger/ComponentLedger.h"
#include <lua.hpp>

#include <string>
#include <vector>
#include <memory>

namespace PROJECT_NAMESPACE
{
	struct ComponentType
	{
		std::string type;
		std::vector<luaL_Reg> methods{};
		lua_CFunction openfunc = nullptr;
		std::unique_ptr<AbstractComponentLedger> ledger;
	};
};