#include "Computer.h"

#include "Ledger/ComponentLedger.h"
#include "Component/ComponentType.h"

namespace PROJECT_NAMESPACE
{
	struct ComputerComponentLib
	{
		static int pullSignal(lua_State* _lua)
		{

		};

		static int pushSignal(lua_State* _lua)
		{

		};
		
		static int shutdown(lua_State* _lua)
		{

		};

		static int open(lua_State* _lua)
		{
			lua_newtable(_lua);

			return 1;
		};
	};

	struct ComputerComponent
	{

	};
	using ComputerLedger = ComponentLedger<ComputerComponent>;

	ComponentType computer_component_type()
	{
		ComponentType _out{};
		_out.type = "computer";
		_out.ledger = std::unique_ptr<ComputerLedger>{ new ComputerLedger{} };
		_out.openfunc = ComputerComponentLib::open;
		return _out;
	};

};
