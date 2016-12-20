#ifndef _CHI_ALLOCATOR_H
#define _CHI_ALLOCATOR_H

#include "exception.h"
#include "int.h"
#include "list.h"
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

		T* ptr() const	{ return this->_ptr(); }
		
		void shrink( Size decrement ) {
			this->_shrink( decrement );
		}

		void resize( Size count ) {
			if ( count > this->count() )
				this->grow( count - this->count() );
			else if ( count < this->count() )
				this->shrink( this->count() - count );
		}


		T& operator[]( Size index ) const {
			return this->ptr()[ index ];
		}
	};

	// The StdAllocator simply allocates elements only when needed.
	template <class T>
	class StdAllocator : public Allocator<T> {
	protected:
		T* __ptr;
		Size _count;
		Size _cap;

	public:
		StdAllocator() : __ptr(0), _count(0), _cap(0) {}

#define CHI_ALLOCATE( INIT ) \
	CHI_ASSERT( this->__ptr != 0 && this->__ptr != (T*)-1, "Allocating with StdAllocator while already allocated, this is a memory leak" ); \
	\
	if ( count == 0 ) \
		this->__ptr = 0; \
	else { \
		/*T* ptr = (T*)::malloc( count * sizeof(T) );*/ \
		T* ptr = new (std::nothrow) T( INIT ); \
		if ( ptr == 0 )	throw AllocException(); \
		this->__ptr = ptr; \
	\
		/*for ( Size i = 0; i < count; i++ ) { \
			new (this->__ptr + i) INIT; \
		}*/ \
	} \
	\
	this->_count = count;

		void allocate( Size count ) {
			CHI_ASSERT( this->__ptr != 0 && this->__ptr != (T*)-1, "Allocating with StdAllocator while already allocated, this is a memory leak" );
	
			if ( count == 0 )
				this->__ptr = 0;
			else {
				/*T* ptr = (T*)::malloc( count * sizeof(T) );*/
				T* ptr = new (std::nothrow) T[ count ];
				if ( ptr == 0 )	throw AllocException();
				this->__ptr = ptr;
		
				/*for ( Size i = 0; i < count; i++ ) { \
					new (this->__ptr + i) INIT; \
				}*/
			} 
			
			this->_count = count;
		}

		Size capacity() const	{ return this->_cap; }

		Size count() const	{ return this->_count; }

		void destruct() {
			for ( Size i = 0; i < this->_count; i++ ) {
				this->__ptr[i].~T();
			}
		}

		void free()	{
			CHI_ASSERT( this->__ptr == (T*)-1, "Double free in StdAllocator" );

			//this->destruct();
			delete[] this->__ptr;
#ifndef NDEBUG
			this->__ptr = (T*)-1;
#endif
		}

		T* _ptr() const	{ return this->__ptr; }

		void _grow( Size increment ) {
			Size new_size = this->_count + increment;

			// Allocate new pointer
			T* new_ptr = new T[ new_size ];
			if ( new_ptr == 0 )	throw AllocException();

			// Copy all elements to new ptr
			Size i = 0;
			for ( ; i < this->_count; i++ ) {
				new_ptr[i] = this->__ptr[i];
			}

			this->free();
			
			// Update pointer
			this->__ptr = new_ptr;
			this->_cap = new_size;
			this->_count = new_size;

			// Initialize new elements with default contructor
			/*for ( ; this->_count < new_size; this->_count++ ) {
				new (this->__ptr + this->_count) T();
			}*/
		}

		void _shrink( Size decrement ) {
			Size new_count = this->_count - decrement;
			this->_count = new_count;

			// Destruct removed elements
			/*for ( ; new_count < this->_count; this->_count-- ) {
				this->__ptr[this->_count - 1].~T();
			}*/
		}
	};

	// The FutureAllocator preallocates extra space when it grows so that future elements do not need to make expensive reallocations.
	// It doesn't do anything when it shrinks.
	template <class T>
	class FutureAllocator : public StdAllocator<T> {
	protected:
		float multiplier;

	public:
		FutureAllocator( float multiplier = 1.5 ) : StdAllocator<T>(), multiplier( multiplier ) {
			CHI_ASSERT( multiplier < 1.0, "Multiplier should not be less than 1." );
		}


		void allocate( Size count ) {
			this->_cap = this->multiplier * count;
			if ( count == 0 )
				this->__ptr = 0;
			else {
				/*T* ptr = (T*)::malloc( this->_cap * sizeof(T) );*/
				T* ptr = new (std::nothrow) T[ this->_cap ];
				if ( ptr == 0 )	throw AllocException();
				this->__ptr = ptr;

				/*for ( Size i = 0; i < count; i++ ) { \
					new (this->__ptr + i) INIT; \
				}*/
			}
			this->_count = count;
		}

		void _grow( Size increment ) {
			Size new_size = this->_count + increment;

			if ( new_size > this->_cap ) {
				Size new_capacity = this->multiplier * new_size;

				T* new_ptr = new (std::nothrow) T[ new_capacity ];
				if ( new_ptr == 0 )	throw AllocException();

				// Copy all existing elements to new ptr
				Size i = 0;
				for ( ; i < this->_count; i++ ) {
					new_ptr[i] = this->__ptr[i];
				}				

				this->free();

				this->__ptr = new_ptr;
				this->_cap = new_capacity;
			}
			
			this->_count = new_size;
			// Just initialize the new elements with the default constructor
			/*for ( ; this->_count < new_size; this->_count++ ) {
				new (this->__ptr + this->_count) T();
			}*/
		}
	};
}

#endif//_CHI_ALLOCATOR_H
