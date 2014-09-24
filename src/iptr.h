#pragma once
#include <ReferenceCounted.h>
#include <boost/intrusive_ptr.hpp>

namespace particle2d {
#define iptr ::boost::intrusive_ptr

	template <typename T>
	void intrusive_ptr_add_ref(T* ref) 
	{
		ref->grab();
	}

	template <typename T>
	void intrusive_ptr_release(T* ref)
	{
		ref->drop();
	}
}

