#include <IParticleRenderer.h>

namespace particle2d { namespace ios 
{
	class IOSParticleRenderer : public IParticleRenderer
	{
	public:
		IOSParticleRenderer(void* nativeData);

		virtual void render();

	private:
		void* nativeData;
	};
}}
