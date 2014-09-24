#pragma once
#include "ReferenceCounted.h"

namespace particle2d
{
	struct SParticle;
	class IParticleInitializer : public virtual ReferenceCounted
	{
	public:
		virtual void initialize(SParticle& particle) = 0;
	};
}

