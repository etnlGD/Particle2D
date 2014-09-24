#pragma once

namespace particle2d
{
	struct SParticle
	{
		//! Elapsed time of the particle since born
		float localTime;

		//! Length of life time of the particle
		float lifeTime;

		//! Position of the particle
		vec2 pos;

		//! Direction and speed of the particle
		core::vector3df vector;

		//! Current color of the particle
		video::SColor16 color;

		//! Scale of the particle.
		/** The current scale of the particle. */
		core::dimension2df size;
	};
}
