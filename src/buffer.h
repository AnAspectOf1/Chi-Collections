#ifndef _CHI_BUFFER_H
#define _CHI_BUFFER_H

#include "allocator.h"
#include "array.h"
#include "int.h"


namespace chi {

	template <class Alloc=StdAllocator<Byte>>
	class Buffer : public Array<Byte, Alloc> {
	public:
		Buffer( Size length = 0 ) : Array<Byte, Alloc>( length ) {}
	};
}

#endif//_CHI_BUFFER_H
