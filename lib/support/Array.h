#pragma once

#include <concepts>
#include <functional>
#include <type_traits>
#include <ranges>

namespace ocv
{
	template <typename T, typename... Ts>
	requires (std::constructible_from<T, decltype(std::forward<Ts>(std::declval<Ts&&>()))> && ...)
		constexpr inline auto make_array(Ts&&... _val) noexcept
	{
		return std::array<T, sizeof...(Ts)>{ std::forward<Ts>(_val)... };
	};

	template <typename T>
	struct array_size;
	template <template <typename T, size_t N> typename ArrayT, typename T, size_t N>
	struct array_size<ArrayT<T, N>>
	{
		constexpr static size_t value = N;
	};
	template <typename T, size_t Len>
	struct array_size<T[Len]>
	{
		constexpr static size_t value = Len;
	};

	template <typename T>
	constexpr inline size_t array_size_v = array_size<T>::value;





};
