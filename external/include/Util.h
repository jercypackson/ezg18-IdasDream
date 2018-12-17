#pragma once

#include <type_traits>

template <typename E>
constexpr auto to_underlying(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}

template <typename E, typename E2>
constexpr unsigned int make_key(E e1, E2 e2) noexcept
{
	return static_cast<unsigned int>(to_underlying(e1) + to_underlying(e2));
};