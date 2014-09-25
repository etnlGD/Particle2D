#include "CParticleSpeedInitializer.h"
#include "SParticle.h"
#include "Randomizer.h"
#include <math3d/trigonometry.h>

namespace particle2d
{
	CParticleSpeedInitializer::CParticleSpeedInitializer(
			const vec2& dir, radian angle) :
		dir(dir.dir()), maxAngle(angle)
	{
	}

	void CParticleSpeedInitializer::initialize(SParticle& p)
	{
		if (maxAngle != radian::ZERO)
		{
			radian angle = Randomizer::shared().frand(-0.5f, 0.5f) * maxAngle;
			float cs = cos(angle);
			float sn = sin(angle);
			p.vector += vec2(dir.x*cs - dir.y*sn, dir.x*sn + dir.y*cs);
		}
		else
		{
			p.vector += dir;
		}
	}

}

