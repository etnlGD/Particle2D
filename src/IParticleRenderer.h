#pragma once
#include "ReferenceCounted.h"
#include "SParticle.h"
#include <vector>

namespace particle2d
{
	class IParticleRenderer : public virtual ReferenceCounted
	{
	public:
		typedef std::vector<SParticle> ParticleVector;
		typedef ParticleVector::iterator ParticleIterator;

	public:
		IParticleRenderer() : source(0) {}

		virtual void onParticleSwapped(ParticleIterator pos)
		{
			// default do nothing.
			(void) pos;
		}

		virtual void setParticleSource(const ParticleVector& source)
		{
			this->source = &source;
		}

		virtual void render() = 0;

	protected:
		const ParticleVector* source;
	};
}

