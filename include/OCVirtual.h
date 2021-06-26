#pragma once

#include <UUID.h>

#include <lua.hpp>

#include <cstdint>
#include <vector>
#include <string>
#include <filesystem>

namespace PROJECT_NAMESPACE
{
	class ComputerInstance;

	int open_computer(lua_State* _lua);
	int open_component(lua_State* _lua);
	int open_debug(lua_State* _lua);

	size_t open_libs(lua_State* _lua, ComputerInstance& _instance);



	using path = std::filesystem::path;

	ComputerInstance* start_computer(const path& _init);
	void delete_computer(ComputerInstance*& _computer);






};
