#ifndef _CHI_ARRAY_H
#define _CHI_ARRAY_H

#include "allocator.h"
#include "collection.h"
#include "exception.h"
#include "int.h"
#include "list.h"
#include "op.h"
#include <cstdlib>
#include <new>
#include <utility>


namespace chi {

	template <class T>
	class ArraySlice;

	template <class T>
	class ArrayBase : public virtual List<T> {
	public:
		typedef T ElementType;

		virtual T* ptr() = 0;
		virtual const T* ptr() const = 0;
		virtual void grow( Size increment ) = 0;
		virtual void shrink( Size decrement ) = 0;

		
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

		// Shrinks the array so that its size becomes 0.
		void empty() {
			this->shrink( this->count() );
		}

		// Returns the index of the element, otherwise returns (Size)-1.
		template <class C>
		Size find( const C& value ) {
			static_assert( op::implementsEqual<T,C>(), "The element type of the array needs to implement the equal operator '==' for the given type of 'value'." );

			for ( Size i = 0; i < this->count(); i++ ) {
				if ( this->at(i) == value )
					return i;
			}

			return Size(-1);
		}

		void prepend( const T& element ) {
			this->grow(1);
			
			// Move everything up
			for ( Size i = this->count() - 1; i > 0; i-- ) {
				this->at(i) = this->at(i-1);
			}

			this->at(0) = element;
		}

		void remove( Size index ) {
			for ( Size i = index; i < this->count(); i++ ) {
				(*this)[i] = std::move( (*this)[i+1] );
			}

			this->shrink(1);
		}

		// Resizes the array and copies all elements to a new array if necessary.
		void resize( Size new_length ) {
			if ( new_length > this->count() )
				this->grow( new_length - this->count() );
			else if ( new_length < this->count() )
				this->shrink( this->count() - new_length );
		}

		ArraySlice<T> slice( Size count, Size first = 0 ) {
			return ArraySlice<T>( this, count, first );
		}
		const ArraySlice<T> slice( Size count, Size first = 0 ) const {
			return ArraySlice<T>( this, count, first );
		}

		ArrayBase<T>& operator=( const List<T>& other ) {		
			// TODO: Implement an assignment function in alloc so that when the array is grown it doesn't need to have the default constructor automatically called on new elements
			this->resize( other.count() );
			for ( Size i = 0; i < other.count(); i++ ) {
				(*this)[i] = other[i];
			}

			return *this;
		}

		ArrayBase<T>& operator=( const ArrayBase<T>& other ) {
			*this = (const List<T>&)other;
			return *this;
		}

		ArrayBase<T>& operator+=( const T& element ) {
			this->append( element );
			return *this;
		}

		ArrayBase& operator+=( const List<T>& other ) {
			this->append( other );
			return *this;
		}
	};

	template <class T, class Alloc = StdAllocator<T>>
	class Array : public virtual ArrayBase<T> {
	protected:
		Alloc alloc;

		void init( Size count, const T& val ) {
			for ( Size i = 0; i < count; i++ ) {
				this->alloc[i] = val;
			}
		}

	public:
		Array()	{}	// Leaving alloc to call default constructor which is the same as allocating with size 0
		Array( Size count )	{ this->alloc.allocate( count ); }
		Array( Size count, const T& copy )	{ this->alloc.allocate( count ); this->init( count, copy ); }
		Array( const Array<T, Alloc>& other ) {
			this->alloc.allocate( other.count() );
			this->copy( other );
		}
		Array( const List<T>& other ) {
			this->alloc.allocate( other.count() );
			this->copy( other );
		}
		// This is a rather slow constructor as it needs to grow for every element it needs to copy
		Array( const Collection<T>& other ) {
			Iterator<T> i = other.begin();
			Size j = 0;
			while ( i.valid() ) {
				this->alloc->grow(1);
				this->alloc[j] = *i;

				i = i.next();
				j++;
			}
		}
		~Array() {
			this->alloc.free();
		}

		T& _at( Size index ) const	{ return this->alloc[ index ]; }

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

		Size capacity() const	{ return this->alloc.capacity(); }

		bool contains( const T& element ) const	{
			return this->find( element ) < this->count();
		}

		void copy( const T* other, Size count, Size first = 0 ) {
			CHI_ASSERT( first + count > this->count(), "Can't copy more into array than it has allocated" );

			for ( Size i = 0, j = first; i < count; i++, j++ ) {
				this->at(j) = other[i];
			}
		}
		void copy( const List<T>& other, Size first = 0 ) {
			Size other_count = other.count();

			CHI_ASSERT( first + other_count > this->count(), "Can't copy more into array than it has allocated" );

			for ( Size i = 0, j = first; i < other_count; i++, j++ ) {
				this->at(j) = other[i];
			}
		}

		Size count() const	{ return this->alloc.count(); }

		void grow( Size increment ) {
			this->alloc.grow( increment );
		}

		T* ptr()	{ return this->alloc.ptr(); }
		const T* ptr() const	{ return this->alloc.ptr(); }

		void shrink( Size decrement ) {
			CHI_ASSERT( decrement > this->count(), "The decrement to shrink should not be more than the array's element count." );

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
		
			// TODO: Implement an assignment function in alloc so that when the array is grown it doesn't need to have the default constructor automatically called on new elements
			this->alloc.resize( other.count() );
			for ( Size i = 0; i < other.count(); i++ ) {
				this->alloc[i] = other[i];
			}

			return *this;
		}
	};

	// The ArraySlice extends a regular slice by providing additional array functions
	template <class T>
	class ArraySlice : public Slice<T>, public virtual ArrayBase<T> {
		friend ArrayBase<T>;

		ArrayBase<T>* array;

	protected:
		ArraySlice( const ArrayBase<T>* array, Size count, Size first = 0 ) : Slice<T>( const_cast<ArrayBase<T>*>(array), count, first ), array(const_cast<ArrayBase<T>*>(array)) {}

	public:

		void grow( Size increment ) {
			this->array->grow( increment );

			for ( Size i = this->array->count(); i >= this->_first + this->_count; i-- ) {
				(*this->array)[i + increment] = (*this->array)[i];
			}
		}

		void shrink( Size decrement ) {
			for ( Size i = 0, j = this->_first + this->_count - decrement; i < decrement; i++, j++ ) {
				(*this->array)[j] = (*this->array)[j + decrement];
			}

			this->array->shrink( decrement );
		}

		T* ptr() override	{ return this->array->ptr(); }
		const T* ptr() const override	{ return this->array->ptr(); }
	};
}


#endif//_CHI_ARRAY_H
