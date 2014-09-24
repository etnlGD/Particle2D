#pragma once
#include <math3d/vector2d.h>
#include "SFixedPointColor.h"

namespace particle2d
{
	using namespace math3d;
	struct SParticle
	{
		SParticle() : randomSeed(0), localTime(0.f),
			lifeTime(0.f), color(1.f, 1.f, 1.f), size(1.f, 1.f) {}

		SParticle(unsigned seed) : randomSeed(seed), localTime(0.f), 
			lifeTime(0.f), color(1.f, 1.f, 1.f), size(1.f, 1.f) {}

		unsigned randomSeed;

		//! Elapsed time of the particle since born
		float localTime;

		//! Length of life time of the particle
		float lifeTime;

		//! Position of the particle
		vec2 pos;

		//! Direction and speed of the particle
		vec2 vector;

		//! Current color of the particle
		SColor16 color;

		//! Scale of the particle.
		/** The current scale of the particle. */
		vec2 size;
	};
}
