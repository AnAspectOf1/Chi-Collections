#ifndef _CHI_LIST_H
#define _CHI_LIST_H

#include "collection.h"
#include "op.h"
#include "slice.h"


namespace chi {

	template <class T>
	class List : public Collection {
	public:
		virtual T& at( Size index ) = 0;
		virtual const T& at( Size index ) const = 0;


		bool contains( const T& element ) const	{
			return this->find( element ) < this->count();
		}

		void copy( const T* other, Size count, Size index = 0 ) {
			CHI_ASSERT( index + count > this->count(), "Can't copy more into array than it has allocated" );

			for ( Size i = 0, j = index; i < count; i++, j++ ) {
				this->at(j) = other[i];
			}
		}
		void copy( const List<T>& other, Size index = 0 ) {
			CHI_ASSERT( index + other.count() > this->count(), "Can't copy more into array than it has allocated" );

			for ( Size i = 0, j = index; i < other.count(); i++, j++ ) {
				this->at(j) = other[i];
			}
		}

		bool equals( const List<T>& other ) const {
			static_assert( op::implementsNotEqual<T>(), "List::equal requires the element type to implement the not-equal '!=' operator" );

			if ( this->count() != other.count() )
				return false;

			for ( Size i = 0; i < other.count(); i++ ) {
				if ( this->at(i) != other.at(i) )
					return false;
			}

			return true;
		}

		// Returns the index of the element, otherwise returns count().
		Size find( const T& element ) const {
			for ( Size i = 0; i < this->count(); i++ ) {
				if ( this->at(i) == element )
					return i;
			}

			return this->count();
		}

		T& first()	{ return this->at(0); }
		const T& first() const	{ return this->at(0); }

		T& last()	{ return this->at( this->count() - 1 ); }
		const T& last() const	{ return this->at( this->count() - 1 ); }

		Size size() const	{ return this->count() * sizeof(T); }

		Slice<T> slice( Size count, Size first = 0 ) {
			CHI_ASSERT( first + count > this->count(), "Can not slice from %d with length %d, this is outside of the lists' count %d", (int)first, (int)count, (int)this->count() );

			return Slice<T>( this, count, first );
		}


		T& operator[]( Size index )	{ return this->at( index ); }
		const T& operator[]( Size index ) const	{ return this->at( index ); }

		bool operator==( const List<T>& other ) const {
			return this->equals( other );
		}
		bool operator!=( const List<T>& other ) const {
			return !this->equals( other );
		}
	};
}

#endif//_CHI_LIST_H
