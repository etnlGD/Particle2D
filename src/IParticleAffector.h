#pragma once
#include "ReferenceCounted.h"

namespace particle2d
{
	struct SParticle;
	class IParticleAffector : public virtual ReferenceCounted
	{
	public:

		virtual void affect(SParticle& p, float deltaTime, 
							float progress, float deltaProgress) = 0;
	};
}
