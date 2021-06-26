#pragma once

#include <Array.h>

#include <ranges>
#include <concepts>
#include <functional>

namespace ocv
{
	// Usage:
	// std::string _str = "foo";
	// const auto _cvec = range_cast<std::vector<char>>(_str);
	
	// Casts a range into another range
	template <std::ranges::range OutT, std::ranges::range InT>
	requires (std::constructible_from<OutT, decltype(std::forward<InT>(std::declval<InT&&>()))> ||
		std::constructible_from<OutT, std::ranges::iterator_t<InT>, std::ranges::iterator_t<InT>>)
		constexpr inline OutT range_cast(InT&& _in) noexcept
	{
		if constexpr (std::constructible_from<OutT, decltype(std::forward<InT>(_in))>)
		{
			return OutT(std::forward<InT>(_in));
		}
		else
		{
			return OutT(std::ranges::begin(_in), std::ranges::end(_in));
		};
	};


	struct fobj_tag {};

	template <typename T>
	concept fobj = std::derived_from<T, fobj_tag>;

	struct op_not_t : fobj_tag
	{
		constexpr auto operator()(const auto& v) const
		{
			return !v;
		};

	};
	constexpr inline op_not_t op_not{};

	struct is_even_t : fobj_tag
	{
		constexpr auto operator()(auto v) const
		{
			return (v % 2) == 0;
		};
	};
	constexpr inline is_even_t is_even{};
	constexpr inline auto operator|(fobj auto lhs, fobj auto rhs) noexcept
	{
		return[&lhs, &rhs] <typename... Ts> (Ts&&... v)
		{
			return rhs(lhs(std::forward<Ts>(v)...));
		};
	};

	template <typename T>
	struct matches_view : fobj_tag
	{
		constexpr bool operator()(const auto& b) const noexcept
		{
			return this->value == b;
		};

		constexpr matches_view(const T& _val) :
			value{ _val }
		{};
		constexpr matches_view(T&& _val) :
			value{ std::move(_val) }
		{};

		T value;
	};

	struct matches_t : fobj_tag
	{
		template <typename T>
		constexpr auto operator()(T&& _val) const noexcept
		{
			return matches_view{ std::forward<T>(_val) };
		};
	};
	constexpr inline matches_t matches{};






};
