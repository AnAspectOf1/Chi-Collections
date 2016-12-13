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

	template<class T>
	class ArrayBase : public virtual List<T> {
	public:
		virtual void grow( Size increment ) = 0;
		virtual void shrink( Size decrement ) = 0;
		virtual void resize( Size new_size ) = 0;
	};

	template <class T, class Alloc = StdAllocator<T>>
	class Array : public virtual ArrayBase<T> {
	protected:
		Alloc alloc;

		void destruct() {
			for ( Size i = 0; i < this->alloc.count(); i++ ) {
				this->alloc[i].~T();
			}
		}

		// Initialize everything using the default contructor
		void init() {			
			for ( Size i = 0; i < this->alloc.count(); i++ ) {
				new (&this->alloc[i]) T();
			}
		}
		
		// Initialize everything using copy contructor
		void init( const T& copy ) {
			for ( Size i = 0; i < this->alloc.count(); i++ ) {
				new (&this->alloc[i]) T( copy );
			}
		}

		// Initialize everything copying everything from other list
		void init( const List<T>& other ) {			
			for ( Size i = 0; i < this->alloc.count(); i++ ) {
				new (&this->alloc[i]) T( other[i] );
			}
		}

		// Initialize everything copying everything from other pointer
		void init( const T* other ) {			
			for ( Size i = 0; i < this->alloc.count(); i++ ) {
				new (&this->alloc[i]) T( other[i] );
			}
		}

	public:
		Array( Size count = 0 )	{ this->alloc.allocate( count ); init(); }
		Array( Size count, const T& copy )	{ this->alloc.allocate( count ); init( copy ); }
		Array( const Array<T, Alloc>& other ) {
			this->alloc.allocate( other.count() );
			this->init( other );
		}
		Array( const List<T>& other ) {
			this->alloc.allocate( other.count() );
			this->init( other );
		}
		~Array() { this->destruct(); this->alloc.free(); }

		void append( const List<T>& other ) {
			Size old_count = this->count();
			this->grow( other.count() );
			
			Size i = 0; Size j = old_count;
			for ( ; i < other.count(); i++, j++ ) {
				(*this)[j] = other[i];
			}
		}

		void append( const T& element ) {
			Size new_index = this->count();
			this->grow( 1 );
			(*this)[ new_index ] = element;
		}

		T& at( Size index )	{ return this->alloc[ index ]; }
		const T& at( Size index ) const	{ return this->alloc[ index ]; }

		Size capacity() const	{ return this->alloc.capacity(); }

		bool contains( const T& element ) const	{
			return this->find( element ) < this->count();
		}

		void copy( const T* other, Size count ) {
			CHI_ASSERT( count > this->count(), "Can't copy more into array than it has allocated" );

			for ( Size i = 0; i < count; i++ ) {
				this->at(i) = other[i];
			}
		}
		void copy( const List<T>& other ) {
			CHI_ASSERT( other.count() > this->count(), "Can't copy more into array than it has allocated" );

			for ( Size i = 0; i < other.count(); i++ ) {
				this->at(i) = other[i];
			}
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

		void shrink( Size decrement ) {
			CHI_ASSERT( decrement > this->count(), "The length to shrink to should not be more than the array already is." );

			// Destruct all losing elements
			for ( Size i = this->count() - decrement; i < this->count(); i++ ) {
				this->alloc[i].~T();
			}

			this->alloc.shrink( decrement );
		}

		// Resizes the array and copies all elements to a new array if necessary.
		void resize( Size new_length ) {
			if ( new_length > this->count() )
				this->grow( new_length - this->count() );
			else if ( new_length < this->count() )
				this->shrink( this->count() - new_length );
		}

		// Apparantly the other assignment operator with the List<T> argument doesn't get called in case of a copy assignment. Therefore this placeholder exists.
		Array<T, Alloc>& operator=( const Array<T, Alloc>& other ) {
			if ( this == &other )	return *this;
			return *this = (List<T>&)other;
		}

		Array<T, Alloc>& operator=( const List<T>& other ) {
		
			this->alloc.resize( other.count() );
			for ( Size i = 0; i < other.count(); i++ ) {
				this->alloc[i] = other[i];
			}

			return *this;
		}

		Array<T, Alloc>& operator+=( const List<T>& other ) {
			this->append( other );
			return *this;
		}
	};
}


#endif//_CHI_ARRAY_H
