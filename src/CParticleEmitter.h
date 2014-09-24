#pragma once
#include <list>
#include <iptr.h>
#include "ReferenceCounted.h"
#include "IDistribution.h"
#include "IParticleInitializer.h"

namespace particle2d
{
	class CParticleEmitter : public virtual ReferenceCounted
	{
		typedef std::list<iptr<IParticleInitializer> > InitializerList;
	
	public:
		typedef InitializerList::iterator InitializerIterator;
		typedef IDistribution<float> EmitCountDistribution;

	public:
		CParticleEmitter();

		virtual ~CParticleEmitter();

		template <typename Container, typename URNG>
		void emit(Container& particles, URNG& generator, float deltaTime)
		{
			unsigned newParticleCount = _update_accum_count(deltaTime);
			for (unsigned i = 0; i < newParticleCount; ++i)
			{
				particles.push_back(SParticle(generator()));
				
				SParticle& newParticle = particles.back();
				for (auto initer : mInitializers)
					initer->initialize(newParticle);
			}
		}

		InitializerIterator addParticleInitializer(IParticleInitializer*);
		
		InitializerIterator insertParticleInitializer(
			InitializerIterator pos, IParticleInitializer*);

		void removeParticleInitializer(InitializerIterator pos);

		void removeParticleInitializer(
			InitializerIterator begin, InitializerIterator end);

		/// Set the distribution of speed of generating particle per second.
		template <typename URNG, typename StdDistribution>
		void setPPSDistribution(const StdDistribution& distribution)
		{
			delete mPPSDistribution;
			mPPSDistribution = EmitCountDistribution::create<URNG>(distribution);
		}

	protected:
		virtual EmitCountDistribution* createDefaultDistribution();

	private:
		/// Calculate new accum
		unsigned _update_accum_count(float deltaTime);

	private:
		InitializerList mInitializers;
		EmitCountDistribution* mPPSDistribution;
		float mAccum;
	};
}

