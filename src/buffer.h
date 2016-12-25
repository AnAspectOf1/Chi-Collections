#ifndef _CHI_BUFFER_H
#define _CHI_BUFFER_H

#include "allocator.h"
#include "array.h"
#include "int.h"


namespace qi {

	typedef ArrayBase<Byte> BufferBase;
	typedef ArraySlice<Byte> BufferSlice;

	template <class Alloc=StdAllocator<Byte>>
	class Buffer : public Array<Byte, Alloc> {
	public:
		Buffer( Size length = 0 ) : Array<Byte, Alloc>( length ) {}
		Buffer( Size length, Byte filler ) : Array<Byte, Alloc>( length, filler ) {}
		Buffer( Size length, const List<Byte>& other ) : Array<Byte, Alloc>( length, other ) {}
		Buffer( const List<Byte>& other ) : Array<Byte, Alloc>( other ) {}
	};
}

#endif//_CHI_BUFFER_H
