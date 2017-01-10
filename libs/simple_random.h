#pragma once

#include <random>

namespace detail
{
	inline auto& engine()
	{
		thread_local auto engine = std::default_random_engine{};
		return engine;
	}
}

inline auto srand_(std::default_random_engine::result_type const seed = std::random_device{}())
{
	detail::engine().seed(seed);
}

template <typename T = unsigned>
inline auto rand_(T const min, T const max)
-> std::enable_if_t<std::is_integral<T>::value, T>
{
	using int_t =
		std::conditional_t<sizeof(T) <= sizeof(unsigned short), unsigned short,
		std::conditional_t<sizeof(T) == sizeof(unsigned int), unsigned int,
		std::conditional_t<sizeof(T) == sizeof(unsigned long), unsigned long,
		unsigned long long>>>;

	using signed_t =
		std::conditional_t<std::is_signed<T>::value, std::make_signed_t<int_t>, int_t>;

	using distribution_t = std::uniform_int_distribution<signed_t>;
	thread_local distribution_t d;
	return static_cast<T>(d(detail::engine(), typename distribution_t::param_type(min, max)));
}

template <typename T>
inline auto rand_(T const min, T const max)
-> std::enable_if_t<std::is_floating_point<T>::value, T>
{
	using distribution_t = std::uniform_real_distribution<T>;
	thread_local distribution_t d;
	return d(detail::engine(), typename distribution_t::param_type(min, max));
}

template <typename T = unsigned, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
inline auto rand_()
{
	return rand_(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
}

template <>
inline auto rand_<bool>()
{
	thread_local std::bernoulli_distribution d;
	return d(detail::engine());
}