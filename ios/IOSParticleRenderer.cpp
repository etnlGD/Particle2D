#include "IOSParticleRenderer.h"
#include "CppObjectiveBridge.h"
#include "SParticle.h"

namespace particle2d { namespace ios
{

	IOSParticleRenderer::IOSParticleRenderer(void* nativeData) :
		nativeData(nativeData)
	{
	}

	void IOSParticleRenderer::render()
	{
		if (!source)
			return;

		int idx = 0;
		vec4 color;
		for (const SParticle& p : *source)
		{
			color = p.color.toVec4();
			float x = p.pos.x, y = p.pos.y;
			float w = p.size.x;
			float h = p.size.y;

			particle_system_ios_native_render(nativeData, idx, 
									   x, y, 
									   color.a, color.r, color.g, color.b, 
									   w, h);

			++idx;
		}

		// end signal
		particle_system_ios_native_render(nativeData, -1, 0, 0, 0, 0, 0, 0, 0, 0);
	}

}}
