#include "Randomizer.h"

namespace particle2d
{
	Randomizer::Randomizer(int32_t seed)
	{
		reset(seed);
	}

	void Randomizer::reset(int32_t seed)
	{
		this->seed = seed;
	}

	int32_t Randomizer::rand()
	{
		// (a*seed)%m with Schrage's method
		int32_t div = seed / DIV_RESULT;
		seed = CONSTANT * (seed - div*DIV_RESULT) - MODULO_RESULT * div;
		if (seed < 0)
			seed += MERSENNE_PRIME;

		return seed;
	}

	int32_t Randomizer::rand(uint32_t max)
	{
		return rand() % max;
	}

	int32_t Randomizer::rand(uint32_t min, uint32_t max)
	{
		// TODO
		return min + rand(max - min);
	}

	float Randomizer::frand()
	{
		return rand() * (1.f / RAND_MAX);
	}

	float Randomizer::frand(float max)
	{
		return frand() * max;
	}

	float Randomizer::frand( float min, float max )
	{
		return min + frand(max - min);
	}

	bool Randomizer::brand()
	{
		return rand(2) == 0;
	}

}
