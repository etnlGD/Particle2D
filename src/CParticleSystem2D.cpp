#include "CParticleSystem2D.h"

#include <list>
#include <vector>
#include <random>
#include <chrono>
#include "iptr.h"
#include "SParticle.h"
#include "CTimeRange.h"
#include "CParticleEmitter.h"
#include "IParticleAffector.h"
#include "IParticleRenderer.h"

namespace particle2d
{
	using namespace std;

	struct CParticleSystem2D::pimpl
	{
		struct SRangedEmitter
		{
			SRangedEmitter(CParticleEmitter* e, float begin, float end) :
				emitter(e), range(begin, end) {}

			iptr<CParticleEmitter> emitter;
			CTimeRange range;
		};

		struct SRangedAffector
		{
			SRangedAffector(IParticleAffector* affector, 
					float affectingLocalTimeBegin, float affectingLocalTimeEnd, 
					float timeToBeginAffect, float timeToEndAffect) :
				affector(affector), 
				range(timeToBeginAffect, timeToEndAffect),
				minProgress(affectingLocalTimeBegin), 
				maxProgress(affectingLocalTimeEnd)
			{
			}

			iptr<IParticleAffector> affector;
			CTimeRange range;
			float minProgress;
			float maxProgress;
		};

		pimpl(unsigned reservedParticleCount) :
			active(true), autoPurge(true)
		{
			particles.reserve(reservedParticleCount);
		}

		list<SRangedEmitter> emitters;
		list<SRangedAffector> affectors;
		iptr<IParticleRenderer> renderer;
		vector<SParticle> particles;

		list<iptr<ActiveListener> > activeListeners;
		bool active;
		bool autoPurge;

		void _set_active(CParticleSystem2D& self, bool active)
		{
			assert(this->active != active);
			this->active = active;
			for (auto listener : activeListeners)
				listener->onStatusChanged(self);
		}
	};

	CParticleSystem2D::CParticleSystem2D(unsigned reservedParticleCount) :
		d(new pimpl(reservedParticleCount))
	{
	}

	CParticleSystem2D::~CParticleSystem2D()
	{
	}

	void CParticleSystem2D::addEmitter(CParticleEmitter* emitter, 
									   float timeToBeginEmit, 
									   float timeToStopEmit)
	{
		d->emitters.emplace_back(emitter, timeToBeginEmit, timeToStopEmit);

		if (!d->active && !d->emitters.back().range.hasFinished())
			d->_set_active(*this, true);
	}

	void CParticleSystem2D::addAffector(IParticleAffector* affector, 
										float affectingLocalTimeBegin, 
										float affectingLocalTimeEnd, 
										float timeToBeginAffect, 
										float timeToEndAffect)
	{
		d->affectors.emplace_back(affector, 
			affectingLocalTimeBegin, affectingLocalTimeEnd, 
			timeToBeginAffect, timeToEndAffect);
	}

	void CParticleSystem2D::setRenderer(IParticleRenderer* renderer)
	{
		d->renderer = renderer;
		
		if (renderer)
			renderer->setParticleSource(d->particles);
	}

	void CParticleSystem2D::update(float deltaTime)
	{
		if (!isActive())
			return;

		auto& particles = d->particles;
		auto seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine rng((unsigned) seed);

		bool hasActiveEmitter = false;
		for (pimpl::SRangedEmitter& rangedEmitter : d->emitters)
		{
			float effectTime = rangedEmitter.range.sweep(deltaTime);
			if (effectTime > 0)
				rangedEmitter.emitter->emit(particles, rng, effectTime);
			
			if (!rangedEmitter.range.hasFinished())
				hasActiveEmitter = true;
		}

		for (pimpl::SRangedAffector& rangedAffector : d->affectors)
		{
			float effectTime = rangedAffector.range.sweep(deltaTime);
			if (effectTime <= 0)
				continue; // This affector isn't working

			IParticleAffector* affector = rangedAffector.affector.get();
			for (SParticle& p : particles)
			{
				float bgn = rangedAffector.minProgress * p.lifeTime;
				float end = rangedAffector.maxProgress * p.lifeTime;

				if (p.localTime >= bgn && p.localTime <= end)
				{
					float invLen = 1.f / (end - bgn);
					float progress  = (p.localTime - bgn) * invLen;
					float dProgress = min(effectTime * invLen, 1 - progress);
					affector->affect(p, effectTime, progress, dProgress);
				}
			}
		}

		IParticleRenderer* renderer = d->renderer.get();
		
		// animate all particles
		for (auto it = particles.begin(); it != particles.end(); )
		{
			it->localTime += deltaTime;

			// erase is pretty expensive!
			if (it->localTime > it->lifeTime)
			{
				if (it == --particles.end())
				{
					particles.pop_back();
					break;
				}
				else
				{
					*it = particles.back();
					particles.pop_back();

					if (renderer)
						renderer->onParticleSwapped(it);
				}
			}
			else
			{
				it->pos += (it->vector * deltaTime);
				++it;
			}
		}

		if (!hasActiveEmitter && particles.empty())
		{
			// finished
			d->_set_active(*this, false);

			if (isPurgeAutomatically())
				purgeUnusedData();
		}
		
		// Renderer update bounding box?
	}

	void CParticleSystem2D::purgeUnusedData()
	{
		d->particles.shrink_to_fit();
	}

	bool CParticleSystem2D::isActive() const
	{
		return d->active;
	}

	void CParticleSystem2D::addActiveListener(ActiveListener* lis)
	{
		if (lis != nullptr)
		{
			d->activeListeners.push_back(iptr<ActiveListener>(lis));
			
			if (!isActive())
				lis->onStatusChanged(*this);
		}
	}

	bool CParticleSystem2D::isPurgeAutomatically() const
	{
		return d->autoPurge;
	}

	void CParticleSystem2D::setPurgeAutomatically( bool autoPurge )
	{
		d->autoPurge = autoPurge;
	}

}
