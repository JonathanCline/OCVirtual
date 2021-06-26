#include "Computer.h"

#include "Network/Network.h"
#include "OCVirtual.h"

#include <lua.hpp>

namespace PROJECT_NAMESPACE
{

	struct ComputerLib
	{

		static int shutdown(lua_State* _lua)
		{


			return 0;
		};

		constexpr static luaL_Reg methods[] =
		{
			luaL_Reg{ "shutdown", shutdown },
			luaL_Reg{ NULL, NULL }
		};


		static int openlib(lua_State* _lua)
		{

			return 1;
		};
	};


};
