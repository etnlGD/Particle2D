#pragma once
#include <math3d/math3d.h>
#include <algorithm>

namespace particle2d
{
	using namespace math3d;

	struct CTimeRange
	{
		inline CTimeRange(float bgn, float end) : 
			timeBgn(bgn), timeEnd(end), timePos(bgn) {}

		float sweep(float timediff)
		{
			using namespace std; // for std::min and std::max
			if (timediff <= 0 || hasFinished())
				return 0.f;

			float sweepBgn = max(timePos, timeBgn);
			
			timePos += timediff;
			float sweepEnd = isForever() ? timePos : min(timePos, timeEnd);

			return max(0.f, sweepEnd - sweepBgn);
		}

		inline bool hasFinished() const
		{
			return !isForever() && timePos >= timeEnd;
		}

		inline bool isForever() const 
		{
			return timeEnd <= timeBgn;
		}

	private:
		float timeBgn;
		float timeEnd;
		float timePos;
	};
}
