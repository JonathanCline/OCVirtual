#pragma once

#include <ostream>
#include <istream>
#include <string>
#include <cstdint>
#include <optional>
#include <sstream>
#include <chrono>
#include <format>
#include <string_view>

namespace ocv
{
	struct UUID
	{
	public:
		constexpr static std::string_view format_string = "{:X}-{:X}-{:X}-{:X}-{:X}";

		friend inline std::ostream& operator<<(std::ostream& _ostr, const UUID& _u)
		{
			constexpr auto _tHighMask = 0xFFFF000000000000;
			const auto _tHigh = (_u.time_ & _tHighMask) >> 48;
			const auto _tLow = _u.time_ & ~_tHighMask;
			return _ostr << std::format(format_string, _u.name_hash_, _u.node_upper_, _u.node_lower_, _tHigh, _tLow);
		};
		friend inline std::istream& operator>>(std::istream& _istr, UUID& _u)
		{
			std::string _pStr{};
			_istr >> _pStr;

			size_t _at = 0;
			size_t _last = 0;

			_at = _pStr.find('-', _last);
			_u.name_hash_ = (uint32_t)std::stoul(_pStr.substr(_last, (_at - _last)), nullptr, 16);
			_last = _at + 1;

			_at = _pStr.find('-', _last);
			_u.node_upper_ = (uint32_t)std::stoul(_pStr.substr(_last, (_at - _last)), nullptr, 16);
			_last = _at + 1;

			_at = _pStr.find('-', _last);
			_u.node_lower_ = (uint32_t)std::stoul(_pStr.substr(_last, (_at - _last)), nullptr, 16);
			_last = _at + 1;

			_at = _pStr.find('-', _last);
			auto _timeUpper = (uint64_t)std::stoul(_pStr.substr(_last, (_at - _last)), nullptr, 16);
			_last = _at + 1;

			auto _timeLower = (uint64_t)std::stoull(_pStr.substr(_last, (_pStr.size() - _last)), nullptr, 16);
			_u.time_ = ((_timeUpper << 48) | (_timeLower));

			return _istr;

		};

		constexpr bool operator==(const UUID& _rhs) const noexcept = default;
		constexpr bool operator!=(const UUID& _rhs) const noexcept
		{
			return !(*this == _rhs);
		};

		constexpr bool operator>(const UUID& _rhs) const noexcept
		{
			return	(this->name_hash_ > _rhs.name_hash_) || (this->node_upper_ > _rhs.node_upper_) ||
				(this->node_lower_ > _rhs.node_lower_) || (this->time_ > _rhs.time_);
		};
		constexpr bool operator<(const UUID& _rhs) const noexcept
		{
			return	(this->name_hash_ < _rhs.name_hash_) || (this->node_upper_ < _rhs.node_upper_) ||
				(this->node_lower_ < _rhs.node_lower_) || (this->time_ < _rhs.time_);
		};

		static inline bool is_UUID_string(const std::string& _str)
		{
			std::stringstream _sstr{ _str };

			std::string _gotLine{};
			std::getline(_sstr, _gotLine, '-');
			if (_sstr.gcount() > 8)
				return false;
			for (auto& c : _gotLine)
				if (!std::isxdigit(c))
					return false;

			for (uint8_t i = 0; i < 3; ++i)
			{
				std::getline(_sstr, _gotLine, '-');
				if (_sstr.gcount() > 4)
					return false;
				for (auto& c : _gotLine)
					if (!std::isxdigit(c))
						return false;
			};

			_sstr >> _gotLine;
			if (_sstr.gcount() > 12)
				return false;
			for (auto& c : _gotLine)
				if (!std::isxdigit(c))
					return false;

			return true;
		};
		static inline std::optional<UUID> from_string(const std::string& _str)
		{
			if (is_UUID_string(_str))
			{
				std::stringstream _sstr{ _str };
				UUID _out{};
				_sstr >> _out;
				return std::optional<UUID>{ _out };
			}
			else
			{
				return std::optional<UUID>{};
			};
		};

		std::string string() const
		{
			std::stringstream sstr{};
			sstr << *this;
			return sstr.str();
		};

		constexpr UUID(const uint64_t& _time, const uint32_t& _node, const uint32_t& _name_hash) noexcept :
			time_{ _time }, node_upper_{ (uint16_t)(_node & 0xFFFF0000) }, node_lower_{ (uint16_t)(_node & 0x0000FFFF) }, name_hash_{ _name_hash }
		{};
		UUID(uint32_t _node, const std::string_view& _name) :
			UUID{ (uint64_t)std::chrono::system_clock::now().time_since_epoch().count(), _node, (uint32_t)std::hash<std::string_view>{}(_name) }
		{};

		constexpr UUID() noexcept = default;

		constexpr UUID(const UUID& other) noexcept = default;
		constexpr UUID& operator=(const UUID& other) noexcept = default;

		constexpr UUID(UUID&& other) noexcept = default;
		constexpr UUID& operator=(UUID&& other) noexcept = default;

	private:
		uint32_t name_hash_ = 0;
		uint16_t node_upper_ = 0;
		uint16_t node_lower_ = 0;
		uint64_t time_ = 0;
	};

};
