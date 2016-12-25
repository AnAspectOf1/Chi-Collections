#ifndef _CHI_DYNAMIC_H
#define _CHI_DYNAMIC_H

#include "array.h"
#include "buffer.h"
#include "string.h"


namespace qi {

	template <class T>
	class DynamicArray: public Array<T, FutureAllocator<T>> {
	public:
		DynamicArray( Size count = 0 ) : Array<T, FutureAllocator<T>>( count ) {}
		DynamicArray( Size count, const List<T>& other ) : Array<T, FutureAllocator<T>>( count, other ) {}
	};
	typedef Buffer<FutureAllocator<Byte>> DynamicBuffer;
	typedef String<FutureAllocator<char>> DynamicString;
}

#endif//_CHI_DYNAMIC_H
