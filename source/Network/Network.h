#pragma once

#include "Component/ComponentID.h"

namespace PROJECT_NAMESPACE
{
	struct Network
	{
	private:
		using container_type = std::vector<ComponentID>;
	public:
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;

		iterator begin() noexcept { return this->components.begin(); };
		const_iterator begin() const noexcept { return this->components.cbegin(); };
		const_iterator cbegin() const noexcept { return this->components.cbegin(); };

		iterator end() noexcept { return this->components.end(); };
		const_iterator end() const noexcept { return this->components.cend(); };
		const_iterator cend() const noexcept { return this->components.cend(); };




		container_type components{};
	};

};
