#ifndef _CHI_ARRAY_H
#define _CHI_ARRAY_H

#include "allocator.h"
#include "collection.h"
#include "exception.h"
#include "int.h"
#include "list.h"
#include <cstdlib>
#include <new>


namespace chi {

	template <class T, class Alloc = StdAllocator<T>>
	class Array : public virtual List<T> {
	protected:
		Alloc alloc;

		void destruct() {
			for ( Size i = 0; i < this->alloc.count(); i++ ) {
				this->alloc[i].~T();
			}
		}

		// Initialize the _ptr using default contructor
		void init() {			
			for ( Size i = 0; i < this->alloc.count(); i++ ) {
				this->alloc[i] = T();
			}
		}
		
		// Initialize the _ptr using copy contructor
		void init( const T& copy ) {
			for ( Size i = 0; i < this->alloc.count(); i++ ) {
				this->alloc[i] = T( copy );
			}
		}

		// Initialize _ptr copying everything from other _ptr
		void init( const T* other ) {			
			for ( Size i = 0; i < this->alloc.count(); i++ ) {
				this->alloc[i] = T( other[i] );
			}
		}

	public:
		Array( Size count = 0 )	{ this->alloc.allocate( count ); init(); }
		Array( Size count, const T& copy )	{ this->alloc.allocate( count ); init( copy ); }
		Array( const Array<T>& other )	{ this->alloc.allocate( other.alloc.count() ); init( other.alloc.ptr() ); }
		~Array() { this->destruct(); this->alloc.free(); }

		T& at( Size index )	{ return this->alloc[ index ]; }
		const T& at( Size index ) const	{ return this->alloc[ index ]; }

		Size capacity() const	{ return this->alloc.capacity(); }

		bool contains( const T& element ) const	{
			return this->find( element ) < this->count();
		}

		Size count() const	{ return this->alloc.count(); }

		// Makes the array empty
		void empty() {
			this->alloc.shrink( this->alloc.count() );
		}

		// Returns the index of the element, otherwise returns count().
		Size find( const T& element ) const {
			for ( Size i = 0; i < this->count(); i++ ) {
				if ( this->alloc[i] == element )
					return i;
			}

			return this->count();
		}

		void grow( Size length ) {
			this->alloc.grow( length );
		}

		T* ptr()	{ return this->alloc.ptr(); }
		const T* ptr() const	{ return this->alloc.ptr(); }

		void shrink( Size length ) {
			CHI_ASSERT( length >= this->count(), "The length to shrink to should be less than the array already is." );

			// Destruct all losing elements
			for ( Size i = this->count() - length; i < this->count(); i++ ) {
				this->alloc[i].~T();
			}

			this->alloc.shrink( length );
		}

		// Resizes the array and copies all elements to a new array if necessary.
		void resize( Size new_length ) {
			if ( new_length > this->count() )
				this->grow( new_length - this->count() );
			else if ( new_length < this->count() )
				this->shrink( this->count() - new_length );
		}

		Array<T>& operator=( const Array<T>& other ) {
			if ( this == &other )	return *this;
		
			this->alloc.resize( other );
			for ( Size i = 0; i < other.count(); i++ ) {
				this->alloc[i] = other.alloc[i];
			}

			return *this;
		}
	};
}


#endif//_CHI_ARRAY_H
