#pragma once

#include "Component/ComponentID.h"

#include <vector>
#include <cassert>
#include <ranges>
#include <algorithm>

namespace PROJECT_NAMESPACE
{
	struct ComponentInstance;

	class AbstractComponentLedger
	{
	public:
		virtual bool contains(ComponentID _id) const = 0;
		virtual void insert(ComponentID _id) = 0;
		virtual size_t erase(ComponentID _id) = 0;
		virtual void clear() noexcept = 0;
		virtual ~AbstractComponentLedger() = default;
	};

	template <std::default_initializable T>
	class ComponentLedger : public AbstractComponentLedger
	{
	public:
		using key_type = ComponentID;

		struct Entry
		{
			using value_type = T;
			using key_type = ComponentLedger::key_type;

			friend inline constexpr bool operator==(const Entry& lhs, const key_type& rhs) noexcept
			{
				return lhs.id == rhs;
			};
			friend inline constexpr bool operator==(const key_type& lhs, const Entry& rhs) noexcept
			{
				return rhs == lhs;
			};
			friend inline constexpr bool operator==(const Entry& lhs, const Entry& rhs) noexcept
			{
				return rhs.id == lhs.id;
			};

			key_type id;
			value_type data{};
		};

		using value_type = Entry;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const value_type*;
		using const_reference = const value_type&;

		using size_type = size_t;
		using difference_type = std::ptrdiff_t;

	private:
		using container_type = std::vector<value_type>;
	public:
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;

		iterator begin() noexcept { return this->entries_.begin(); };
		const_iterator begin() const noexcept { return this->entries_.cbegin(); };
		const_iterator cbegin() const noexcept { return this->entries_.cbegin(); };

		iterator end() noexcept { return this->entries_.end(); };
		const_iterator end() const noexcept { return this->entries_.cend(); };
		const_iterator cend() const noexcept { return this->entries_.cend(); };

	private:
		constexpr static auto make_compare(key_type _cmp)
		{
			return [_cmp](const auto& k) -> bool
			{
				return _cmp == k;
			};
		};

	public:
		bool contains(key_type _id) const final
		{
			return std::ranges::find_if(this->entries_, make_compare(_id)) != this->end();
		};
		void insert(key_type _id) final
		{
			assert(!this->contains(_id));
			this->entries_.push_back(Entry{ _id });
		};
		size_t erase(key_type _id) final
		{
			return std::erase_if(this->entries_, make_compare(_id));
		};
		void clear() noexcept final
		{
			this->entries_.clear();
		};

	private:
		container_type entries_{};
	};

};
