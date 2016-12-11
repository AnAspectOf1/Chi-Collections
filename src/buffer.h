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
		Buffer( Size count, const List<Byte>& other ) : Array<Byte, Alloc>( count, other ) {}
		Buffer( const List<Byte>& other ) : Array<Byte, Alloc>( other ) {}
	};
}

#endif//_CHI_BUFFER_H
