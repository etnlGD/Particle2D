#pragma once
#include "ReferenceCounted.h"
#include <boost/scoped_ptr.hpp>

namespace particle2d
{
	class CParticleEmitter;
	class IParticleAffector;
	class IParticleRenderer;
	class CParticleSystem2D : public virtual ReferenceCounted
	{
	public:
		class ActiveListener : public virtual ReferenceCounted
		{
		public:
			virtual void onStatusChanged(CParticleSystem2D&) = 0;
		};

	public:
		explicit CParticleSystem2D(unsigned reservedParticleCount = 64);

		virtual ~CParticleSystem2D();

		/**
		 * The default state is active, therefore:
		 *	+ If current state is not active, then lis will be invoked at once
		 *	+ If current state is active, then lis won't be invoked at present
		 */
		void addActiveListener(ActiveListener* lis);

		void addEmitter(CParticleEmitter*, 
						float timeToBeginEmit = 0, 
						float timeToStopEmit = -1.f);

		void addAffector(IParticleAffector*,
						 float affectingLocalTimeBegin = 0.f,
						 float affectingLocalTimeEnd = 1.f,
						 float timeToBeginAffect = 0.f,
						 float timeToEndAffect = -1.f);

		void setRenderer(IParticleRenderer*);

		void update(float deltaTime);

		void setPurgeAutomatically(bool autoPurge);

		bool isPurgeAutomatically() const;

		void purgeUnusedData();

		bool isActive() const;

	private:
		struct pimpl;
		boost::scoped_ptr<pimpl> d;
	};
}
