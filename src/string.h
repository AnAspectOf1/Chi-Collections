#ifndef _CHI_STRING_H
#define _CHI_STRING_H

#include "array.h"
#include "int.h"
#include <cstring>


namespace chi {

	class StringBase : public virtual ArrayBase<char> {
	public:
		virtual void append( char character ) = 0;
		virtual void append( const char* character ) = 0;
		virtual Size length() const	{ return this->count() - 1; }

		StringBase& operator=( const char* string ) {
			Size length = ::strlen( string );
			
			this->resize( length + 1 );
			
			for ( Size i = 0; i <= length; i++ ) {
				this->at(i) = string[i];
			}

			return *this;
		}

		StringBase& operator+=( char c ) {
			this->append( c ); return *this;
		}
	};

	template <class Alloc = StdAllocator<char>>
	class String : public Array<char, Alloc>, public virtual StringBase {
	public:
		String( Size length = 0 ) : Array<char, Alloc>( length + 1, '\0' ) {}
		String( Size length, char c ) : Array<char, Alloc>( length + 1, c ) {
			this->at( length ) = '\0';
		}		
		String( const List<char>& other ) : Array<char, Alloc>( other ) {}
		String( const String<Alloc>& other ) : Array<char, Alloc>( other ) {}
		String( const char* string ) {
			Size length = ::strlen( string );
			this->alloc.resize( length + 1 );
			this->init( string );
		}
		String( char character ) : Array<char>( 2 ) {
			this->alloc[0] = character;
			this->alloc[1] = '\0';
		}

		void append( char character ) {
			Size old_len = this->length();
			this->grow( 1 );
			(*this)[ old_len ] = character;
			(*this)[ this->length() ] = '\0';
		}

		void append( const char* string ) {
			size_t other_length = strlen( string );
			Size old_length = this->length();
			this->grow( other_length );
			for ( Size i = 0; i < other_length; i++ ) {
				this->at(old_length + i) = string[i];
			}
		}

		String<Alloc> operator+( char character ) {
			String<Alloc> new_string( this->length() + 1 );
			new_string.copy( this->ptr(), this->length() );
			new_string[ this->length() ] = character;
			return new_string;
		}

		String<Alloc> operator+( const char* string ) {
			size_t other_len = ::strlen( string );
			Size old_len = this->length();
			String<Alloc> new_string( old_len + other_len );

			new_string.copy( this->ptr(), this->length() );
			for ( Size i = 0; i < other_len; i++ ) {
				new_string[ old_len + i ] = string[i];
			}
			return new_string;
		}
	};
}

#endif//_CHI_STRING_H
