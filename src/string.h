#ifndef _CHI_STRING_H
#define _CHI_STRING_H

#include "array.h"
#include "int.h"
#include <cstring>


namespace chi {

	class BaseString : public virtual List<char> {
	public:
		virtual void append( char character ) = 0;
		virtual Size length() const = 0;
	};

	template <class Alloc = StdAllocator<char>>
	class String : public Array<char, Alloc>, public BaseString {
	public:
		String() : Array<char, Alloc>( 1, '\0' ) {}
		String( Size length ) : Array<char, Alloc>( length + 1, '\0' ) {}
		String( const char* string ) {
			Size length = ::strlen( string );
			
			this->alloc.allocate( length + 1 );
			this->alloc[ length ] = '\0';
		}
		String( char character ) : Array<char>( 2 ) {
			this->alloc[0] = character;
			this->alloc[1] = '\0';
		}

		void append( char character ) {
			Size count = this->count();
			this->grow( 1 );
			(*this)[ count - 1 ] = character;
		}

		Size length() const	{ return this->count() - 1; }

		String& operator=( const char* string ) {
			Size length = ::strlen( string );
			
			this->resize( length + 1 );
			
			for ( Size i = 0; i <= length; i++ ) {
				this->alloc[i] = string[i];
			}
		}
	};
}

#endif//_CHI_STRING_H
