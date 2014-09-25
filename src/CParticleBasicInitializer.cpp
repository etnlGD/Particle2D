#include "CParticleBasicInitializer.h"
#include "SParticle.h"

namespace particle2d
{
	CParticleBasicInitializer::CParticleBasicInitializer() :
		mWidthDistribution(0), mHeightDistribution(),
		mTimeDistribution(0),
		mAlphaDistribution(0), mRedDistribution(0),
		mGreenDistribution(0), mBlueDistribution(0)
	{
	}

	CParticleBasicInitializer::~CParticleBasicInitializer()
	{
		delete mWidthDistribution;
		delete mHeightDistribution;
		delete mTimeDistribution;
		delete mAlphaDistribution;
		delete mRedDistribution;
		delete mGreenDistribution;
		delete mBlueDistribution;
	}

	void CParticleBasicInitializer::initialize(SParticle& particle)
	{
		if (mWidthDistribution)
		{
			particle.size.x = mWidthDistribution->generate();
			particle.size.y = mHeightDistribution->generate();
		}

		if (mTimeDistribution)
		{
			particle.lifeTime = mTimeDistribution->generate();
		}

		if (mAlphaDistribution)
		{
			float a = mAlphaDistribution->generate();
			float r = mRedDistribution->generate();
			float g = mGreenDistribution->generate();
			float b = mBlueDistribution->generate();
			particle.color.setARGB(a, r, g, b);
		}
	}

}

