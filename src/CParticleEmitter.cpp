#include "CParticleEmitter.h"

#include <random>
#include <stdint.h>
#include "IDistribution.h"
#include "SParticle.h"

namespace particle2d
{
	typedef CParticleEmitter::InitializerIterator InitializerIterator;
	typedef CParticleEmitter::EmitCountDistribution EmitCountDistribution;

	CParticleEmitter::CParticleEmitter() :
		mPPSDistribution(nullptr), mAccum(0) {}

	CParticleEmitter::~CParticleEmitter()
	{
		delete mPPSDistribution;
	}

	InitializerIterator CParticleEmitter::addParticleInitializer(
		IParticleInitializer* initializer)
	{
		mInitializers.push_back(iptr<IParticleInitializer>(initializer));
		return --mInitializers.end();
	}

	InitializerIterator CParticleEmitter::insertParticleInitializer(
		InitializerIterator pos, IParticleInitializer* initializer)
	{
		return mInitializers.insert(pos, iptr<IParticleInitializer>(initializer));
	}

	void CParticleEmitter::removeParticleInitializer(InitializerIterator pos)
	{
		mInitializers.erase(pos);
	}

	void CParticleEmitter::removeParticleInitializer(
		InitializerIterator begin, InitializerIterator end)
	{
		mInitializers.erase(begin, end);
	}

	EmitCountDistribution* CParticleEmitter::createDefaultDistribution()
	{
		typedef std::default_random_engine defaultURNG;
		return EmitCountDistribution::create<defaultURNG>(
				std::uniform_real_distribution<float>(80, 100));
	}

	unsigned CParticleEmitter::_update_accum_count(float deltaTime)
	{
		if (mPPSDistribution == nullptr)
		{
			mPPSDistribution = createDefaultDistribution();
			assert(mPPSDistribution != nullptr);
		}

		mAccum += mPPSDistribution->generate() * deltaTime;
		if (mAccum >= 1)
		{
			unsigned newParticleCount = (unsigned) mAccum;
			mAccum -= newParticleCount;
			return newParticleCount;
		}

		return 0;
	}

}
