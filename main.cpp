#include <OCVirtual.h>

#include <iostream>
#include <filesystem>
#include <format>

namespace ns = PROJECT_NAMESPACE;

struct ComputerInstanceDeleter
{
	void operator()(ns::ComputerInstance* _instance)
	{
		ns::delete_computer(_instance);
	};
};
using ComputerHandle = std::unique_ptr<ns::ComputerInstance, ComputerInstanceDeleter>;


int main(int _narg, char* _args[])
{
	const ns::path _source = SOURCE_ROOT "test/prim.lua";
	ComputerHandle _computer{ ns::start_computer(_source) };
	return 0;
};