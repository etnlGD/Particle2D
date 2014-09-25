#pragma once
#include "IParticleInitializer.h"
#include <math3d/vector2d.h>
#include <math3d/angle.h>

namespace particle2d
{
	using namespace math3d;
	class CParticleSpeedInitializer : public IParticleInitializer
	{
	public:
		CParticleSpeedInitializer(const vec2& dir, radian angle);

		virtual void initialize(SParticle&);

	private:
		vec2 dir;
		radian maxAngle;
	};
}
