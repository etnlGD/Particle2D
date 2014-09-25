#pragma once
#include "IParticleInitializer.h"
#include "IDistribution.h"
#include <math3d/vector2d.h>

namespace particle2d
{
	using namespace math3d;
	class CParticleGeometricInitializer : public IParticleInitializer
	{
	public:
		typedef IDistribution<float> NormalSpeedSizeDistribution;

	public:
		CParticleGeometricInitializer() : 
			mOutlineOnly(false), mUsingNormalDirectionAsSpeed(false),
			mSpeedSizeDistribution(nullptr) {}

		virtual ~CParticleGeometricInitializer()
		{
			delete mSpeedSizeDistribution;
		}

		virtual void initialize(SParticle& particle);

		template <typename URNG, typename StdDistribution>
		void setNormalSpeedSizeDistribution(const StdDistribution& distribution)
		{
			delete mSpeedSizeDistribution;
			mSpeedSizeDistribution = NormalSpeedSizeDistribution::create<URNG>(distribution);
		}

		void setUseNormalDirectionAsSpeed(bool use)
		{
			mUsingNormalDirectionAsSpeed = use;
		}

		bool isUsingNormalDirectionAsSpeed() const
		{
			return mUsingNormalDirectionAsSpeed;
		}

		void setOutlineOnly(bool outlineOnly)
		{
			mOutlineOnly = outlineOnly;
		}

		bool isOutlineOnly() const
		{
			return mOutlineOnly;
		}

	private:
		virtual void generateOutlinePoint(vec2& pos, vec2& normal) = 0;
		virtual void generateCentralPoint(vec2& pos, vec2& normal) = 0;

	private:
		bool mOutlineOnly;
		bool mUsingNormalDirectionAsSpeed;
		NormalSpeedSizeDistribution* mSpeedSizeDistribution;
	};
}
