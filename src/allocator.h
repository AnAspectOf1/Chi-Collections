#ifndef _CHI_ALLOCATOR_H
#define _CHI_ALLOCATOR_H

#include "exception.h"
#include "int.h"
#include <cstdlib>


namespace chi {

	template <class T>
	class Allocator {
	public:
		virtual void allocate( Size count ) = 0;
		virtual Size capacity() const = 0;
		virtual Size count() const = 0;
		virtual void free() = 0;
		virtual void _grow( Size count ) = 0;
		virtual T* _ptr() const = 0;
		virtual void _shrink( Size count ) = 0;

		void grow( Size increment ) {
			this->_grow( increment );
		}

		T* ptr()	{ return this->_ptr(); }
		const T* ptr() const	{ return this->_ptr(); }
		
		void shrink( Size decrement ) {
			this->_shrink( decrement );
		}

		void resize( Size count ) {
			if ( count > this->count() )
				this->grow( count - this->count() );
			else if ( count < this->count() )
				this->shrink( this->count() - count );
		}

		T& operator[]( Size index ) {
			return this->ptr()[ index ];
		}
		const T& operator[]( Size index ) const {
			return this->ptr()[ index ];
		}
	};

	// The StdAllocator simply allocates elements only when needed.
	template <class T>
	class StdAllocator : public Allocator<T> {
		T* _pointer;
		Size _count;

	public:
		StdAllocator() : _pointer(0), _count(0) {}

		void allocate( Size count ) {
			if ( count == 0 )
				this->_pointer = 0;
			else {
				T* ptr = (T*)::malloc( count * sizeof(T) );
				if ( ptr == 0 )	throw AllocException();
				this->_pointer = ptr;
			}		
			this->_count = count;
		}

		Size capacity() const	{ return this->_count; }

		Size count() const	{ return this->_count; }

		void free()	{ ::free( this->_pointer ); }

		T* _ptr() const	{ return this->_pointer; }

		void _grow( Size increment ) {
			Size new_size = this->_count + increment;

			T* new_ptr = (T*)::realloc( this->_pointer, new_size );
			if ( new_ptr == 0 )	throw AllocException();

			this->_pointer = new_ptr;
			this->_count = new_size;
		}

		void _shrink( Size decrement ) {
			Size old_count = this->_count;

			this->_count -= decrement;

			for ( Size i = this->count(); i < old_count; i++ ) {
				this->_pointer[i].~T();
			}
		}
	};

	// The FutureAllocator preallocates extra space when it grows so that future elements do not need to make expensive reallocations.
	// It doesn't do anything when it shrinks.
	template <class T>
	class FutureAllocator : public Allocator<T> {
	protected:
		T* _pointer;
		Size _count;
		Size _capacity;
		float multiplier;

	public:
		FutureAllocator( float multiplier = 1.5 ) : _pointer(0), _count(0), _capacity(0), multiplier( multiplier ) {
			CHI_ASSERT( multiplier < 1.0, "Multiplier should not be less than 1." );
		}

		void allocate( Size count ) {
			this->_capacity = this->multiplier * count;
			if ( count == 0 )
				this->_pointer = 0;
			else {
				T* ptr = (T*)::malloc( this->_capacity * sizeof(T) );
				if ( ptr == 0 )	throw AllocException();
				this->_pointer = ptr;
			}		
			this->_count = count;
		}
		
		Size capacity() const	{ return this->_capacity; }

		Size count() const	{ return this->_count; }

		void free()	{ ::free( this->_pointer ); }

		T* _ptr() const	{ return this->_pointer; }

		void _grow( Size increment ) {
			Size new_size = this->_count + increment;

			if ( new_size > this->_capacity ) {
				Size new_capacity = this->multiplier * new_size;

				T* new_ptr = (T*)::realloc( this->_pointer, new_capacity );
				if ( new_ptr == 0 )	throw AllocException();

				this->_pointer = new_ptr;
				this->_capacity = new_capacity;
			}
			
			this->_count = new_size;
		}

		void _shrink( Size decrement ) {
			Size old_count = this->_count;

			this->_count -= decrement;

			for ( Size i = this->count(); i < old_count; i++ ) {
				this->_pointer[i].~T();
			}
		}
	};
}

#endif//_CHI_ALLOCATOR_H
