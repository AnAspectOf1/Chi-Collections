#ifndef _CHI_STRING_H
#define _CHI_STRING_H

#include "array.h"
#include "int.h"
#include <cstring>

#define OS	(String<>)


namespace chi {

	class StringBase : public virtual ArrayBase<char> {
	public:
		virtual void append( char character ) = 0;
		virtual void append( const char* character ) = 0;
		virtual Size length() const	{ return this->count() - 1; }

		bool contains( char c ) const {
			return this->find( c ) < this->length();
		}

		// Returns the index of the character, otherwise returns length().
		Size find( char c ) const {
			for ( Size i = 0; i < this->length(); i++ ) {
				if ( this->at(i) == c )
					return i;
			}

			return this->length();
		}

		StringBase& operator=( const char* string ) {
			Size length = ::strlen( string );
			
			this->resize( length + 1 );
			
			for ( Size i = 0; i <= length; i++ ) {
				this->at(i) = string[i];
			}

			return *this;
		}

		StringBase& operator=( const StringBase& other ) {
			this->resize( other.size() );

			for ( Size i = 0; i <= other.size(); i++ ) {
				this->at(i) = other[i];
			}

			return *this;
		}

		StringBase& operator+=( char c ) {
			this->append( c ); return *this;
		}

		bool operator==( const char* string ) const {
			for ( Size i = 0; i < this->count(); i++ ) {
				if ( this->at(i) != string[i] )
					return false;

				if ( string[i] == '\0' )
					break;
			}

			return true;
		}
		bool operator!=( const char* string ) const {
			return !( *this == string );
		}
	};

	template <class Alloc = StdAllocator<char>>
	class String : public Array<char, Alloc>, public virtual StringBase {
	public:
		String( Size length = 0 ) : Array<char, Alloc>( length + 1, '\0' ) {}
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

		String<Alloc>& operator=( const char* string ) {
			StringBase::operator=( string );
			return *this;
		}

		String<Alloc>& operator=( const StringBase& other ) {
			StringBase::operator=( other );
			return *this;
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
