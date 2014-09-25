#include "CppObjectiveBridge.h"
#include "Particle2D.h"
#include <random>
#include "IOSParticleRenderer.h"

using namespace std;
using namespace particle2d;
using namespace ios;

extern void* particle_system_ios_create(void* native_render_ptr, 
										const char* args)
{
	(void) args;	// dummy by now.
	CParticleSystem2D* psys = new CParticleSystem2D(256);

	{
		CParticleEmitter* emitter = new CParticleEmitter;
		emitter->setPPSDistribution<default_random_engine>(uniform_real_distribution<float>(60, 80));

		{
			CParticleBoxInitializer* initer = new CParticleBoxInitializer(-1, 1, 1.05f, 1);
			emitter->addParticleInitializer(initer);
			initer->drop();
		}

		{
			CParticleSpeedInitializer* initer = new CParticleSpeedInitializer(vec2(0, -0.6f), degree(20));
			emitter->addParticleInitializer(initer);
			initer->drop();
		}

		{
			CParticleBasicInitializer* initer = new CParticleBasicInitializer;
			initer->setLifetimeDistribution<default_random_engine>(uniform_real_distribution<float>(3.f, 3.333f));

			initer->setSizeDistribution<default_random_engine>(
				uniform_real_distribution<float>(1.f, 2.2f),
				uniform_real_distribution<float>(1.f, 2.2f));

			emitter->addParticleInitializer(initer);
			initer->drop();
		}

		psys->addEmitter(emitter);
		emitter->drop();
	}

	{
		CParticleFadeOutAffector* affector = new CParticleFadeOutAffector;
		affector->setTargetColor(vec4());
		psys->addAffector(affector, 0.75f, 1.f);
		affector->drop();
	}

	{
		IParticleRenderer* renderer = new IOSParticleRenderer(native_render_ptr);
		psys->setRenderer(renderer);
		renderer->drop();
	}

	return psys;
}

extern void particle_system_ios_animate(void* particle_system_ptr, 
										float time)
{
	auto psys = static_cast<CParticleSystem2D*>(particle_system_ptr);
	if (psys != nullptr)
		psys->update(time);
}

extern void particle_system_ios_destroy(void* particle_system_ptr)
{
	auto psys = static_cast<CParticleSystem2D*>(particle_system_ptr);
	psys->drop();
}
