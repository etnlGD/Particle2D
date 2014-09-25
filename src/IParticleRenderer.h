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
		virtual void onParticleSwapped(ParticleIterator pos) = 0;

		virtual void setParticleSource(const ParticleVector&) = 0;

		virtual void render() = 0;
	};
}

