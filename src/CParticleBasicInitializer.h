#pragma once
#include "IParticleInitializer.h"
#include "IDistribution.h"

namespace particle2d
{
	class CParticleBasicInitializer : public IParticleInitializer
	{
	public:
		CParticleBasicInitializer();

		virtual ~CParticleBasicInitializer();

		virtual void initialize(SParticle&);

		template <typename URNG, typename StdDistribution>
		void setSizeDistribution(const StdDistribution& widthDistribution,
								 const StdDistribution& heightDistribution)
		{
			delete mWidthDistribution;
			mWidthDistribution = _create<URNG>(widthDistribution);

			delete mHeightDistribution;
			mHeightDistribution = _create<URNG>(heightDistribution);
		}

		template <typename URNG, typename StdDistribution>
		void setLifetimeDistribution(const StdDistribution& timeDistribution)
		{
			delete mTimeDistribution;
			mTimeDistribution = _create<URNG>(timeDistribution);
		}

		template <typename URNG, typename StdDistribution>
		void setColorDistribution(const StdDistribution& alphaDistribution,
								  const StdDistribution& redDistribution,
								  const StdDistribution& greenDistribution,
								  const StdDistribution& blueDistribution)
		{
			delete mAlphaDistribution;
			mAlphaDistribution = _create<URNG>(alphaDistribution);

			delete mRedDistribution;
			mRedDistribution = _create<URNG>(redDistribution);

			delete mGreenDistribution;
			mGreenDistribution = _create<URNG>(greenDistribution);

			delete mBlueDistribution;
			mBlueDistribution = _create<URNG>(blueDistribution);
		}

	private:
		template <typename URNG, typename StdDistribution>
		static IDistribution<float>* _create(const StdDistribution& d)
		{
			return IDistribution<float>::create<URNG>(d);
		}

	private:
		IDistribution<float>* mWidthDistribution;
		IDistribution<float>* mHeightDistribution;
		IDistribution<float>* mTimeDistribution;
		IDistribution<float>* mRedDistribution;
		IDistribution<float>* mGreenDistribution;
		IDistribution<float>* mBlueDistribution;
		IDistribution<float>* mAlphaDistribution;
	};
}
