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

		void append( const char* string ) {
			size_t other_length = strlen( string );
			Size old_length = this->length();
			this->grow( other_length );
			for ( Size i = 0; i < other_length; i++ ) {
				this->at(old_length + i) = string[i];
			}
		}

		Size length() const	{ return this->count() - 1; }

		String<Alloc>& operator=( const char* string ) {
			Size length = ::strlen( string );
			
			this->resize( length + 1 );
			
			for ( Size i = 0; i <= length; i++ ) {
				this->alloc[i] = string[i];
			}
		}

		String<Alloc> operator+( char character ) {
			String<Alloc> new_string( this->length() + 1 );
			new_string[ this->length() ] = character;
			return new_string;
		}

		String<Alloc> operator+( const char* string ) {
			size_t other_len = ::strlen( string );
			Size old_len = this->length();
			String<Alloc> new_string( old_len + other_len );

			for ( Size i = 0; i < other_len; i++ ) {
				new_string[ old_len + i ] = string[i];
			}
			return new_string;
		}
	};
}

#endif//_CHI_STRING_H
