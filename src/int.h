#ifndef _CHI_INT_H
#define _CHI_INT_H

#include <cstddef>
#include <cstdint>
#ifdef _POSIX_SOURCE
#include <sys/types.h>
#endif

namespace qi {

	typedef unsigned char Byte;
	typedef size_t Size;
#ifdef _POSIX_SOURCE
	typedef ssize_t SSize;
#else
	typedef long SSize;
#endif
}

#endif//_CHI_INT_H
