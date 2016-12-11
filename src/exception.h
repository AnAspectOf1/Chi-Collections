#ifndef _CHI_EXCEPTION_H
#define _CHI_EXCEPTION_H

#ifndef NDEBUG
#include <cstdio>
#define CHI_ASSERT( COND, MSG... ) \
	if (COND) { fprintf( stderr, "[Chi]\tAssertion: %s:%s:%d: ", __FILE__, __func__, __LINE__ ); fprintf( stderr, MSG ); fprintf( stderr, ".\n" ); }
#else
#define CHI_ASSERT( COND, MSG... )
#endif


namespace chi {

	class Exception {};
}

#endif//_CHI_EXCEPTION_H
