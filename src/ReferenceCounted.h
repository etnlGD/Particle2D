#pragma once
#include <cassert>

namespace particle2d
{
	/// Reference counting object
	class ReferenceCounted
	{
	public:
		ReferenceCounted() : referenceCounter(1)
		{
		}

		virtual ~ReferenceCounted()
		{
		}

		void grab() const 
		{ 
			++referenceCounter; 
		}

		bool drop() const
		{
			// someone is doing bad reference counting.
			assert(referenceCounter > 0);

			--referenceCounter;
			if (!referenceCounter)
			{
				delete this;
				return true;
			}

			return false;
		}

		int getReferenceCount() const
		{
			return referenceCounter;
		}

	private:
		mutable int referenceCounter;
	};
}

