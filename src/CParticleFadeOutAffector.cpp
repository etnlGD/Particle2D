#include "CParticleFadeOutAffector.h"
#include "SParticle.h"

namespace particle2d
{
	void CParticleFadeOutAffector::affect(SParticle& p, float deltaTime, 
										  float progress, float deltaProgress)
	{
		(void) deltaTime;
		SColor16& pcolor = p.color;

		float ratio =  deltaProgress / (1 - progress);
		if (mColorMask & EFCM_RED_ENABLE)
			pcolor.interpolate(mTargetColor, ratio, SColor16::ECC_RED_CHANNEL);

		if (mColorMask & EFCM_GREEN_ENABLE)
			pcolor.interpolate(mTargetColor, ratio, SColor16::ECC_GREEN_CHANNEL);

		if (mColorMask & EFCM_BLUE_ENABLE)
			pcolor.interpolate(mTargetColor, ratio, SColor16::ECC_BLUE_CHANNEL);

		if (mColorMask & EFCM_ALPHA_ENABLE)
			pcolor.interpolate(mTargetColor, ratio, SColor16::ECC_ALPHA_CHANNEL);
	}
}
