#pragma once

using random_engine = std::default_random_engine;
class random
{
public:
	random() = default;

	template <typename T = unsigned,
		typename = std::enable_if_t<std::is_arithmetic<T>::value>>
		inline T get(
			T min = std::numeric_limits<T>::min(),
			T max = std::numeric_limits<T>::max()) const
	{
		using distribution_t = std::conditional<
			std::is_floating_point<T>::value,
			std::uniform_real_distribution<T>,
			std::uniform_int_distribution<T>
		>::type;

		static distribution_t distribution;
		return distribution(engine_(), distribution_t::param_type(min, max));
	}

	inline void seed(random_engine::result_type const seed) const
	{
		engine_().seed(seed);
	}

private:
	inline random_engine& engine_() const
	{
		static random_engine engine{ std::random_device{}() };
		return engine;
	}	
};