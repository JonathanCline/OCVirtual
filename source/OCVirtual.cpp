#include "OCVirtual.h"

#include "World/Computer.h"

#include "Component/Component.h"
#include "Component/ComponentType.h"

#include "Component/Type/Computer.h"
#include "Component/Type/Modem.h"

#include <Ranges.h>

#include <array>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace PROJECT_NAMESPACE
{

	class ComputerInstance
	{
	public:
		std::vector<Component> components{};
		std::vector<std::unique_ptr<ComponentType>> component_types{};
		lua_State* lua;
	};


	int open_computer(lua_State* _lua)
	{
		lua_newtable(_lua);
		const auto _idx = lua_gettop(_lua);

		lua_pushnil(_lua);
		lua_copy(_lua, _idx - 1, -1);
		lua_pushcclosure(_lua, [](lua_State* _lua) -> int
			{
				return 0;
			}, 1);
		lua_setfield(_lua, _idx, "shutdown");

		return 1;
	};
	int open_component(lua_State* _lua)
	{
		lua_newtable(_lua);
		const auto _idx = lua_gettop(_lua);

		assert(lua_islightuserdata(_lua, 1));

		lua_pushnil(_lua);
		lua_copy(_lua, 1, -1);
		lua_pushcclosure(_lua, [](lua_State* _lua) -> int
		{
			const auto _computer = static_cast<ComputerInstance*>(lua_touserdata(_lua, lua_upvalueindex(1)));
			if (_computer == nullptr)
			{
				std::terminate();
			};

			lua_pushlightuserdata(_lua, _computer);
			lua_pushinteger(_lua, 0);
			lua_pushcclosure(_lua, [](lua_State* _lua) noexcept -> int
			{
				const auto _computerIndex = lua_upvalueindex(1);
				const auto _positionIndex = lua_upvalueindex(2);
				const auto _computer = static_cast<ComputerInstance*>(lua_touserdata(_lua, _computerIndex));
				const auto _position = (size_t)luaL_checkinteger(_lua, _positionIndex);
				if (_computer == nullptr)
				{
					std::terminate();
				};

				// Set next position value
				lua_pushinteger(_lua, _position + 1);
				lua_copy(_lua, -1, _positionIndex);
				lua_pop(_lua, 1);

				// Return the component at position, or nil if at end
				auto& _components = _computer->components;
				if (_position < _components.size())
				{
					auto& _comp = _components.at(_position);
					const auto _address = _comp.id().string();
					const auto& _type = _comp.type()->type;
					lua_pushstring(_lua, _address.c_str());
					lua_pushstring(_lua, _type.c_str());
					return 2;
				}
				else
				{
					return 0;
				};
			}, 2);
			return 1;
		}, 1);
		lua_setfield(_lua, _idx, "list");

		lua_pushnil(_lua);
		lua_copy(_lua, 1, -1);
		lua_pushcclosure(_lua, [](lua_State* _lua) -> int
			{
				const auto _computer = static_cast<ComputerInstance*>(lua_touserdata(_lua, lua_upvalueindex(1)));
				if (_computer == nullptr)
				{
					std::terminate();
				};

				ComponentID _address{};
				{
					auto _addressOpt = ComponentID::from_string(luaL_checkstring(_lua, 1));
					if (!_addressOpt) [[unlikely]]
					{
						return luaL_argerror(_lua, 1, "expected component address");
					};
					_address = *_addressOpt;
				};
				const auto _fname = luaL_checkstring(_lua, 2);

				auto& _components = _computer->components;
				auto _component = std::ranges::find_if(_components, matches(_address));
				if (_component == std::ranges::end(_components))
				{
					return luaL_argerror(_lua, 1, "expected component address");
				};

				const auto& _methods = _component->type()->methods;
				const auto _method = std::ranges::find_if(_methods, [&_fname](const auto& m)
					{
						return _fname == std::string{ m.name };
					});
				if (_method == std::ranges::end(_methods))
				{
					return luaL_argerror(_lua, 2, "not a function");
				};

				lua_remove(_lua, 2);
				return std::invoke(_method->func, _lua);
			}, 1);
		lua_setfield(_lua, _idx, "invoke");

		lua_pushnil(_lua);
		lua_copy(_lua, 1, -1);
		lua_pushcclosure(_lua, [](lua_State* _lua) -> int
			{
				const auto _computer = static_cast<ComputerInstance*>(lua_touserdata(_lua, lua_upvalueindex(1)));
				if (_computer == nullptr)
				{
					std::terminate();
				};

				ComponentID _address{};
				{
					auto _addressOpt = ComponentID::from_string(luaL_checkstring(_lua, 1));
					if (!_addressOpt) [[unlikely]]
					{
						return luaL_argerror(_lua, 1, "expected component address");
					};
					_address = *_addressOpt;
				};

				auto& _components = _computer->components;
				auto _component = std::ranges::find_if(_components, matches(_address));
				if (_component == std::ranges::end(_components))
				{
					return luaL_argerror(_lua, 1, "expected component address");
				};

				const auto& _methods = _component->type()->methods;
				lua_newtable(_lua);
				const auto _outIdx = lua_gettop(_lua);
				for (auto& m : _methods)
				{
					lua_pushstring(_lua, m.name);
					lua_rawseti(_lua, _outIdx, lua_rawlen(_lua, _outIdx) + 1);
				};
				return 1;
			}, 1);
		lua_setfield(_lua, _idx, "methods");

		lua_pushnil(_lua);
		lua_copy(_lua, 1, -1);
		lua_pushcclosure(_lua, [](lua_State* _lua) -> int
			{
				const auto _computer = static_cast<ComputerInstance*>(lua_touserdata(_lua, lua_upvalueindex(1)));
				if (_computer == nullptr)
				{
					std::terminate();
				};

				ComponentID _address{};
				{
					auto _addressOpt = ComponentID::from_string(luaL_checkstring(_lua, 1));
					if (!_addressOpt) [[unlikely]]
					{
						return luaL_argerror(_lua, 1, "expected component address");
					};
					_address = *_addressOpt;
				};

				auto& _components = _computer->components;
				auto _component = std::ranges::find_if(_components, matches(_address));
				if (_component == std::ranges::end(_components))
				{
					return luaL_argerror(_lua, 1, "expected component address");
				};

				lua_pushstring(_lua, _component->type()->type.c_str());
				return 1;
			}, 1);
		lua_setfield(_lua, _idx, "type");

		return 1;
	};


	int open_debug(lua_State* _lua)
	{
		lua_newtable(_lua);
		const auto _idx = lua_gettop(_lua);

		lua_pushcfunction(_lua, [](lua_State* _lua) -> int
			{
				return 0;
			});
		lua_setfield(_lua, _idx, "traceback");

	

		return 1;
	};

	using libreg = std::pair<const char*, lua_CFunction>;
	constexpr auto standard_libs = make_array<libreg>
		(
			libreg{ LUA_STRLIBNAME, luaopen_string },
			libreg{ LUA_MATHLIBNAME, luaopen_math },
			libreg{ LUA_TABLIBNAME, luaopen_table },
			libreg{ LUA_COLIBNAME, luaopen_coroutine },
			libreg{ LUA_DBLIBNAME, open_debug },
			libreg{ "base", luaopen_base}
		);
	
	using libreg = std::pair<const char*, lua_CFunction>;
	constexpr auto opencomputers_libs = make_array<libreg>
		(
			libreg{ "component", open_component },
			libreg{ "computer", open_computer }
		);

	size_t open_libs(lua_State* _lua, ComputerInstance& _instance)
	{
		size_t _count = 0;
		for (auto& lib : standard_libs)
		{
			++_count;
			const auto& [_name, _openfunc] = lib;
			const auto _count = _openfunc(_lua);
			if (_count != 1)
			{
				std::terminate();
			};
			lua_setglobal(_lua, _name);
		};

		const auto _top = lua_gettop(_lua);
		for (auto& lib : opencomputers_libs)
		{
			++_count;
			lua_settop(_lua, _top);
			lua_pushlightuserdata(_lua, &_instance);
			const auto& [_name, _openfunc] = lib;
			const auto _count = _openfunc(_lua);
			if (_count != 1)
			{
				std::terminate();
			};
			lua_setglobal(_lua, _name);
		};


		return _count;
	};


	namespace fs = std::filesystem;
	auto dofile(lua_State* _lua, const fs::path& _file)
	{
		const auto _pathStr = _file.string();
		return luaL_dofile(_lua, _pathStr.c_str());
	};
};

namespace PROJECT_NAMESPACE
{

	ComputerInstance* start_computer(const path& _init)
	{
		std::unique_ptr<ComputerInstance> _instance{ new ComputerInstance{} };
		_instance->component_types.push_back(std::unique_ptr<ComponentType>{ new ComponentType{ modem_component_type() } });
		_instance->component_types.push_back(std::unique_ptr<ComponentType>{ new ComponentType{ computer_component_type() } });

		auto& _components = _instance->components;
		_components.push_back(Component{ new_component_id(), *_instance->component_types.front() });
		_components.push_back(Component{ new_component_id(), *_instance->component_types.front() });
		_components.push_back(Component{ new_component_id(), *_instance->component_types.back() });

		auto& _lua = _instance->lua;
		_lua = luaL_newstate();
		open_libs(_lua, *_instance);
		const auto _result = dofile(_lua, _init);
		if (_result != LUA_OK)
		{
			std::cout << luaL_optstring(_lua, -1, "lua error!") << std::endl;
		};

		return _instance.release();
	};
	void delete_computer(ComputerInstance*& _computer)
	{
		if (_computer)
		{
			lua_close(_computer->lua);
			delete _computer;
			_computer = nullptr;
		};
	};

};


