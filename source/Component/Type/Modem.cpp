#include "Modem.h"

#include "Ledger/ComponentLedger.h"
#include "Component/ComponentType.h"

#include <Ranges.h>

namespace PROJECT_NAMESPACE
{
	struct ModemComponent
	{
		bool is_wireless = false;
	};
	
	struct ModemComponentLib
	{
		static int isWireless(lua_State* _lua)
		{
			// 1 : address
			const auto _address = UUID::from_string(luaL_checkstring(_lua, 1)).value();
			lua_pushboolean(_lua, true);
			return 1;
		};

		constexpr static luaL_Reg methods[] =
		{
			luaL_Reg{ "isWireless", isWireless },
			luaL_Reg{ NULL, NULL }
		};

		static int open(lua_State* _lua)
		{
			luaL_newlibtable(_lua, methods);
			
			const auto _idx = lua_gettop(_lua);

			
			return 1;
		};
	};

	constexpr bool operator!(const luaL_Reg& lhs)
	{
		return !(lhs.func != NULL && lhs.name != NULL);
	};

	ComponentType modem_component_type()
	{
		ComponentType _out{};
		_out.type = "modem";
		_out.ledger = std::make_unique<ComponentLedger<ModemComponent>>();
		_out.openfunc = ModemComponentLib::open;

		using MethodsT = decltype(_out.methods);
		_out.methods = range_cast<MethodsT>(ModemComponentLib::methods);
		_out.methods.pop_back();

		return _out;
	};

};
