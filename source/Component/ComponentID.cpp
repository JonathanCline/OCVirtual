#include "ComponentID.h"

#include <random>
#include <numeric>
#include <cstdint>
#include <chrono>

namespace PROJECT_NAMESPACE
{
	namespace
	{
		template <typename T>
		requires std::numeric_limits<T>::is_integer
			inline T rand()
		{
			static std::random_device rnd_dv{};
			static std::mt19937 rnd_mt{ rnd_dv() };
			static std::uniform_int_distribution<T> rnd_dist{ std::numeric_limits<T>::min(), std::numeric_limits<T>::max() };
			return rnd_dist(rnd_mt);
		};
	};

	ComponentID new_component_id()
	{
		const auto _time = std::chrono::steady_clock::now().time_since_epoch().count();
		const auto _node = rand<uint32_t>();
		const auto _hash = rand<uint32_t>();
		return ComponentID{ (uint64_t)_time, _node, _hash };
	};
	void delete_component_id(ComponentID& _id)
	{
		_id = nullcomp;
	};

};