#pragma once

using random_engine = std::default_random_engine;
inline random_engine& global_rng_()
{
	static random_engine engine{ std::random_device{}() };
	return engine;
}

template <typename T = unsigned,
	typename = std::enable_if_t<std::is_arithmetic<T>::value>>
inline T rand_(
	T min = std::numeric_limits<T>::min(),
	T max = std::numeric_limits<T>::max())
{
	using distribution_t = std::conditional<
		std::is_integral<T>::value,
		std::uniform_int_distribution<T>,
		std::uniform_real_distribution<T>
	>::type;
	
	static distribution_t distribution;
	return distribution(global_rng_(), distribution_t::param_type(min, max));
}

inline void srand_(random_engine::result_type seed)
{
	global_rng_().seed(seed);
}