#pragma once
#include "IParticleAffector.h"
#include "SFixedPointColor.h"
#include <math3d/vector4d.h>

namespace particle2d
{
	enum E_FADEOUT_COLOR_MASK
	{
		EFCM_RED_ENABLE		= 0x1,
		EFCM_GREEN_ENABLE	= 0x2,
		EFCM_BLUE_ENABLE	= 0x4,
		EFCM_ALPHA_ENABLE	= 0x8,
		EFCM_ALL_ENABLE		= 0xf,
	};

	using namespace math3d;

	/// Particle Affector for fading out a color
	class CParticleFadeOutAffector : public IParticleAffector
	{
	public:
		CParticleFadeOutAffector() : 
			mTargetColor(vec4(0, 0, 0, 1)), mColorMask(EFCM_ALL_ENABLE) {}

		/// Sets the targetColor, i.e. the color the particles will interpolate
		/// to over time.
		void setTargetColor(const vec4& targetColor) 
		{ 
			mTargetColor = targetColor; 
		}

		/// Sets the targetColor, i.e. the color the particles will interpolate
		/// to over time.
		const vec4& getTargetColor() const 
		{ 
			return mTargetColor.toVec4(); 
		}

		void setColorMask(E_FADEOUT_COLOR_MASK mask)
		{
			mColorMask = (E_FADEOUT_COLOR_MASK) (mask & EFCM_ALL_ENABLE);
		}

		E_FADEOUT_COLOR_MASK getColorMask() const
		{
			return mColorMask;
		}

		bool isChannelEnabled(E_FADEOUT_COLOR_MASK mask) const
		{ 
			return (mColorMask & mask) == (mask & EFCM_ALL_ENABLE); 
		}

		virtual void affect(SParticle& p, float deltaTime, 
							float progress, float deltaProgress);

	private:
		SColor16 mTargetColor;
		E_FADEOUT_COLOR_MASK mColorMask;
	};
}

