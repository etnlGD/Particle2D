#pragma once
#include "ReferenceCounted.h"

namespace particle2d
{
	class IParticleRenderer : public virtual ReferenceCounted
	{
	public:
		typedef std::vector<SParticle> ParticleVector;
		typedef ParticleVector::iterator ParticleIterator;

	public:
		virtual void onParticleSwapped(
			const ParticleVector& particles, ParticleIterator pos) = 0;

	};
}

