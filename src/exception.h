#ifndef _CHI_EXCEPTION_H
#define _CHI_EXCEPTION_H

#ifndef NDEBUG
#include <cstdio>
#define QI_ASSERT( COND, MSG... ) \
	if (COND) { fprintf( stderr, "[Chi]\tAssertion: %s:%s:%d: ", __FILE__, __func__, __LINE__ ); fprintf( stderr, MSG ); fprintf( stderr, ".\n" ); throw AssertException(); }
#else
#define QI_ASSERT( COND, MSG... )
#endif

#define CHI_ASSERT( COND, MSG... ) QI_ASSERT( COND, MSG )


namespace qi {

	class Exception {};

	class AllocException : public Exception {};
	class AssertException : public Exception {};
	class ConstException : public Exception {};
}

#endif//_CHI_EXCEPTION_H
