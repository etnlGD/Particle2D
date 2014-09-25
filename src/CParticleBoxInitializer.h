#pragma once
#include "CParticleGeometricInitializer.h"

namespace particle2d
{
	class CParticleBoxInitializer : public CParticleGeometricInitializer
	{
	public:
		CParticleBoxInitializer(float x1, float x2, float y1, float y2);

	protected:
		virtual void generateOutlinePoint(vec2& pos, vec2& normal);
		virtual void generateCentralPoint(vec2& pos, vec2& normal);

	private:
		vec2 mLeftBottomPoint;
		vec2 mRightTopPoint;
	};
}

