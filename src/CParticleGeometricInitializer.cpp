#include "CParticleGeometricInitializer.h"
#include "SParticle.h"

namespace particle2d
{
	void CParticleGeometricInitializer::initialize(SParticle& particle)
	{
		vec2 pos, speedDir;
		if (mOutlineOnly)
			generateOutlinePoint(pos, speedDir);
		else
			generateCentralPoint(pos, speedDir);


		particle.pos = pos;
		if (mUsingNormalDirectionAsSpeed)
		{
			speedDir *= mSpeedSizeDistribution->generate();
			particle.vector = speedDir;
		}
	}

}
