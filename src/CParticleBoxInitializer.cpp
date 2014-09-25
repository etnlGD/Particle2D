#include "CParticleBoxInitializer.h"
#include "Randomizer.h"
#include <algorithm>

namespace particle2d
{
	CParticleBoxInitializer::CParticleBoxInitializer(
		float x1, float x2, float y1, float y2)
	{
		mLeftBottomPoint.set(std::min(x1, x2), std::min(y1, y2));
		mRightTopPoint.set(std::max(x1, x2), std::max(y1, y2));
	}

	void CParticleBoxInitializer::generateOutlinePoint(vec2& pos, vec2& normal)
	{
		Randomizer& randomizer = Randomizer::shared();

		float dx = mRightTopPoint.x - mLeftBottomPoint.x;
		float dy = mRightTopPoint.y - mLeftBottomPoint.y;
		float coord = randomizer.frand(dx + dy);
		if (coord < dx)
		{
			// horizontal
			pos.x = mLeftBottomPoint.x + coord;
			if (randomizer.brand())
			{
				pos.y = mLeftBottomPoint.y;
				normal.set(0, -1);
			}
			else
			{
				pos.y = mRightTopPoint.y;
				normal.set(0, 1);
			}
		}
		else
		{
			// vertical
			pos.y = mLeftBottomPoint.y + (coord - dx);
			if (randomizer.brand())
			{
				pos.x = mLeftBottomPoint.x;
				normal.set(-1, 0);
			}
			else
			{
				pos.x = mRightTopPoint.x;
				normal.set(1, 0);
			}
		}
	}

	void CParticleBoxInitializer::generateCentralPoint(vec2& pos, vec2& normal)
	{
		Randomizer& randomizer = Randomizer::shared();
		pos.x = randomizer.frand(mLeftBottomPoint.x, mRightTopPoint.x);
		pos.y = randomizer.frand(mLeftBottomPoint.y, mRightTopPoint.y);

		if (isUsingNormalDirectionAsSpeed())
		{
			vec2 center = mLeftBottomPoint.mid(mRightTopPoint);
			normal = (pos - center).dir();
		}
	}

}
